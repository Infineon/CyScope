#File: usb_serial.tcl
#USB Serial Port Interface Procedures

#This program is free software; you can redistribute it and/or
#modify it under the terms of the GNU General Public License as
#published by the Free Software Foundation; either version 2 of
#the License, or (at your option) any later verison.
#
#This program is distributed in the hope that it will be useful, but
#WITHOUT ANY WARRANTY; without even the implied warranty of
#MECHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See
#the GNU General Public License for more details.
#
#You should have received a copy of the GNU General Public License
#along with this program; if not, write to the Free Software
#Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301
#USA

#======================================

package provide usbSerial 1.0
set portHandle stdout

namespace eval usbSerial {

#---=== USB-Serial Global Variables ===---
set serialPort "?"
set comNum "?"
set serialStatus "Disconnected"
set baudRate 230400
set receivedData {}
set responseType "?"
set validPorts {}

set firmwareIdent "Unknown"

set usbWarnImage [image create photo -file "$::images/USB-Warn.png"]

set answer ""

set portShow showAvailable

#Create a cache for converting signed integers to
#unsigned ones.  Suggested in comp.lang.tcl in 
#April 11, 2005
set cvt {}
for {set i 0} {$i <256} {incr i} {
	lappend cvt $i
}

#---=== Export Public Procedures
namespace export serialSettings
namespace export sendCommand

}

proc usbSerial::showConnectOptions {} {
	variable serialStatus
	
	if {[winfo exists .serialAsk]} {
		raise .serialAsk
		return
	}
	
	#Opening the port failed, ask the user what to do
	toplevel .serialAsk
	wm title .serialAsk "Connection Error"
	
	label .serialAsk.img	\
		-image $usbSerial::usbWarnImage
		
	label .serialAsk.info	\
		-text "Unable to connect to device."	\
		-font {-weight bold -size -14}	\
		-anchor center
		
	label .serialAsk.autoInfo	\
		-text "Auto-Detect - Search all available communication ports and detect device"	\
		-font {-size -12}	\
		-anchor w
		
	label .serialAsk.manualInfo	\
		-text "Manual Selection - Display a list of communication ports"	\
		-font {-size -12}	\
		-anchor w
		
	frame .serialAsk.buttons	\
		-relief flat
	button .serialAsk.buttons.auto		\
		-text "Auto-Detect..."	\
		-width 20	\
		-command {set usbSerial::answer auto}
	button .serialAsk.buttons.manual	\
		-text "Manual Select"	\
		-width 20	\
		-command {set usbSerial::answer manual}
	button .serialAsk.buttons.cancel	\
		-text "Cancel"	\
		-width 20	\
		-command {set usbSerial::answer cancel}
	
	if {$::osType != "Darwin"} {
		grid .serialAsk.buttons.auto -row 0 -column 0 -padx 10
	}
	grid .serialAsk.buttons.manual -row 0 -column 1 -padx 10
	grid .serialAsk.buttons.cancel -row 0 -column 2 -padx 10
		
	grid .serialAsk.img -row 0 -column 0 -rowspan 3
	grid .serialAsk.info -row 0 -column 1
	if {$::osType != "Darwin"} {
		grid .serialAsk.autoInfo -row 1 -column 1 -sticky w
	}
	grid .serialAsk.manualInfo -row 2 -column 1 -sticky w
	grid .serialAsk.buttons -row 3 -column 0 -columnspan 2
	
	bind .serialAsk <Destroy> {set usbSerial::answer cancel}
	
	update
	raise .serialAsk
	focus .serialAsk
	grab .serialAsk
	
	#Center the serial port settings window over the parent
	#See http://wiki.tcl.tk/534
	set w [winfo width .]
	set h [winfo height .]
	set x [winfo rootx .]
	set y [winfo rooty .]
	set xpos "+[ expr {$x+($w-[winfo width .serialAsk])/2}]"
	set ypos "+[ expr {$y+($h-[winfo height .serialAsk])/2}]"
	wm geometry .serialAsk "$xpos$ypos"
	
	set usbSerial::answer ""
	vwait usbSerial::answer
	
	#Copy the answer, because it will be changed when the window is destroyed
	set temp $usbSerial::answer
	destroy .serialAsk
	
	#Set the indicator on the menubar to "Disconnected"
	set serialStatus "Disconnected"
	.menubar.serialPortStatus configure \
		-background red
		
	#Send all commands to the console
	set portHandle stdout

	#Try again?
	if {$temp == "manual"} { serialSettings }
	if {$temp == "auto"} { 
		if {$::osType == "windows"} {
			usbSerial::autodetectWindows
		} else {
			usbSerial::autodetectLinux
		}
	}
}

proc usbSerial::openSerialPort {} {
	global portHandle
	global serialCheck
	
	#Attempt to open the serial port
	if { [catch {set portHandle [open $usbSerial::serialPort r+]} result] } {
		
		if {$usbSerial::serialPort!="?"} {
			tk_messageBox	\
				-title "Communication Error"	\
				-default ok		\
				-message "Port COM$usbSerial::comNum can not be opened.  Already in use?"	\
				-type ok			\
				-icon warning
		}
		
		#Close the port, in case it is already open
		usbSerial::closeSerialPort
		
		showConnectOptions

		return 0

	} else {
		#The scope defaults to 230kbps on power up,
		if {$::osType!="Darwin"} {
    # KEES modified the line below to remove hardware handhsaking
      if {[catch {fconfigure $portHandle -mode $usbSerial::baudRate,n,8,1	-blocking 0	-buffering line -encoding binary -translation {binary lf}} result]} {
				#Display "Disconnected" status on the menubar
				set usbSerial::serialStatus "Disconnected"
				.menubar.serialPortStatus configure \
					-background red
				tk_messageBox	\
					-title "Communication Error"	\
					-default ok		\
					-message "Device on COM$usbSerial::comNum can not be configured."	\
					-type ok			\
					-icon warning
				
				::usbSerial::closeSerialPort
				
				showConnectOptions
				
				return 0
			}
		} else {
			exec stty -f $usbSerial::serialPort $usbSerial::baudRate cs8 -parenb -cstopb crtscts
			fconfigure $portHandle	\
				-blocking 0		\
				-buffering line	\
				-handshake rtscts	\
				-encoding binary	\
				-translation {binary lf}
		}
				
		#We are now going to query the device.
		#We set up  and intermediate fileevent handler to deal with 
		#identification data received from the instrument
		fileevent $portHandle readable {
			set incomingData [gets $portHandle]
			puts "incomingData: $incomingData"
			if {  [lsearch $incomingData "CircuitGear"] !=-1 } {
				#Poke the serialCheck variable
				set serialCheck found
				#Global variable to store firmware information
				set usbSerial::firmwareIdent $incomingData
			} else {
				puts "No match"
			}
		}		
		puts "Querying device..."
		#Query the device.
		sendCommand ""
		sendCommand ""
		flush $portHandle
		after 500
		set junk [read $portHandle]
		sendCommand i
		
		#Wait for a response from the device
		set serialCheck waiting
		after 1500 {set serialCheck timeout}
		vwait serialCheck
		
		#Check to see if we found the device...
		if { $serialCheck == "found" } {
			puts "Connected."
	# KEES modified		
			#Enable handshaking
#			fconfigure $portHandle -handshake rtscts 
			
			#Display "Connected" status on menu bar
			set usbSerial::serialStatus "Connected"
			.menubar.serialPortStatus configure -background green
			usbSerial::setupFileevent
			#Connect was successful
			bind . Destroy {usbSerial::closeSerialPort; destroy .}
			initializeCGR
			return 1
			
		} else {
			puts "Failed."
			
			#Display "Disconnected" status on the menubar
			set usbSerial::serialStatus "Disconnected"
			.menubar.serialPortStatus configure \
				-background red
			tk_messageBox	\
				-title "Communication Error"	\
				-default ok		\
				-message "Device on COM$usbSerial::comNum did not respond."	\
				-type ok			\
				-icon warning
			
			::usbSerial::closeSerialPort
			
			showConnectOptions
			
			return 0
			
		}
	}

}

#Set Up Fileevent
#-----------------
#This procedure is called to initialize the fileevent handler
#for data received from the instrument.
proc ::usbSerial::setupFileevent {} {
	global portHandle
	
	#Read in any left over data (such as line ends) from the
	#autodetection routines.
	set junk [read $portHandle]

	fileevent $portHandle readable {
		::usbSerial::processResponse
	}
}

#Process Response
#-------------------
#This procedure processes the message received from the instrument.  It examines
#the "responseType" and calls the appropriate routine to deal with the message.
proc ::usbSerial::processResponse {} {
		
	#Read in all available data from the serial port
	set incomingData [read $::portHandle]
	
	#Convert the data bytes into signed integers
	if { [llength {$incomingData}] > 0 } {
		binary scan $incomingData c* signed
		#Convert the bytes into unsigned integers (0-255)
		foreach byte $signed {
			lappend ::usbSerial::receivedData [lindex $::usbSerial::cvt [expr {$byte & 255}]]
		}
	}
	
	#See if we have data in the buffer to process
	if {[llength $usbSerial::receivedData] > 0} {
		set usbSerial::responseType [lindex $::usbSerial::receivedData 0]
		set usbSerial::responseType [format %c $usbSerial::responseType]
	} else {
		return
	}
		
	#Get the total length of the message (number of bytes)
	set responseLength [llength $usbSerial::receivedData]
	
	#Process the message based on it's message type
	switch $usbSerial::responseType {
		"A" {
			#Address from scope capture
			if {$responseLength >=3} {
				#Address received from scope
				set temp [lrange $usbSerial::receivedData 0 2]
				#Deal with left-over data in the receive buffer
				if {$responseLength > 3} {
					set usbSerial::receivedData [lrange $usbSerial::receivedData 3 end]
				} else {
					set usbSerial::receivedData {}
				}
				#Convert and store the trigger point
				scope::saveTriggerPoint $temp
				#Read the scope data buffer
				sendCommand "S B "
			} else {
				return
			}
		} "D" {
			#Data from scope capture
			if {$responseLength >=4097} {
				#Sort out data received from scope
				set temp [lrange $usbSerial::receivedData 0 4096]
				#Deal with left-over data in the receive buffer
				if {$responseLength > 4097} {
					set usbSerial::receivedData [lrange $usbSerial::receivedData 4097 end]
				} else {
					set usbSerial::receivedData {}
				}
				#Process the capture data returned from the scope
				scope::processScopeData $temp
			} else {
				return
			}
		} "E" {
			#Received an error, display the error
			if { [lsearch $usbSerial::receivedData 10] != -1} {
				set index [lsearch $usbSerial::receivedData 10]
				set temp [lrange $usbSerial::receivedData 0 $index]
				if { $responseLength > [expr {$index + 1}] } {
					set usbSerial::receivedData [lrange $usbSerial::receivedData [expr {$index+1}] end]
				} else {
					set usbSerial::receivedData {}
				}
				#Error from instrument
				puts "Error!"
				foreach char $temp {
					puts -nonewline [format %c $char ]
				}
			} else {
				return
			}
		} "S" {
			#Received scope state, display the state
			if { [lsearch $usbSerial::receivedData 10] != -1} {
				set index [lsearch $usbSerial::receivedData 10]
				set temp [lrange $usbSerial::receivedData 0 $index]
				if { $responseLength > [expr {$index + 1}] } {
					set usbSerial::receivedData [lrange $usbSerial::receivedData [expr {$index+1}] end]
				} else {
					set usbSerial::receivedData {}
				}
				#State machine info from scope
				#set temp [lindex $usbSerial::receivedData 1]
				set temp $usbSerial::receivedData
				puts "State is $temp"
			} else {
				return
			}
		} "I" {
			#Received digital I/O input data, update display
			if { $responseLength >= 2} {
				puts "receivedData: $usbSerial::receivedData"
				set temp [lindex $usbSerial::receivedData 1]
				if { $responseLength > 2 } {
					set usbSerial::receivedData [lrange $usbSerial::receivedData 2 end]
				} else {
					set usbSerial::receivedData {}
				}
				#State machine info from scope
				::digio::updateDigIn $temp
			} else {
				return
			}
		} "O" {
			#Received offset calibration info from the device
			if { $responseLength >= 5} {
				set temp [lrange $usbSerial::receivedData 1 4]
				if { $responseLength > 5 } {
					set usbSerial::receivedData [lrange $usbSerial::receivedData 6 end]
				} else {
					set usbSerial::receivedData {}
				}
				#State machine info from scope
				scope::restoreOffsetCal $temp
			} else {
				return
			}
		} "!" {
			#Received an interrupt from the devicee
			if { [lsearch $usbSerial::receivedData 10] != -1} {
				set index [lsearch $usbSerial::receivedData 10]
				set temp [lrange $usbSerial::receivedData 0 $index]
				if { $responseLength > [expr {$index + 1}] } {
					set usbSerial::receivedData [lrange $usbSerial::receivedData [expr {$index+1}] end]
				} else {
					set usbSerial::receivedData {}
				}
				#State machine info from scope
				digio::setInt
			} else {
				return
			}
		} default {
			#We received an unknown message type
			puts "Unknown response: $usbSerial::responseType"
			set temp [llength $usbSerial::receivedData]
			puts "Buffer length $temp"
			puts $usbSerial::receivedData
			set usbSerial::receivedData {}
		}
	}	
	
	#If there is more data in the receive buffer, repeat this procedure
	if { [llength $usbSerial::receivedData] > 0 } {
		usbSerial::processResponse
	}

}


#Close Serial Port
#-----------------
#This procedure closes any USB serial port currently open as
#"portHandle". 
proc ::usbSerial::closeSerialPort {} {
	global portHandle
	variable serialStatus	
	
	if {$portHandle != "stdout"} {
		catch { [close $portHandle]}
	}
	set serialStatus "Disconnected"
	.menubar.serialPortStatus configure -background red
	set portHandle stdout
}



#Send Command to Hardware
#------------------------------
# This procedure takes the argument "command" and sends it
# to the serial port.  The argument is also printed to stdout.
proc ::usbSerial::sendCommand {command} {
	global osType portHandle
	
	puts $portHandle $command
	if {$osType == "Darwin"} {flush $portHandle}
	if $::debugLevel { puts $command }
}

#Serial Settings
#---------------
#When invoked, this procedure creates a new window which allows
#the user to adjust the serial port settings.  The settings are read 
#from, and stored to, a file called port.cfg.
proc ::usbSerial::serialSettings {} {
	global osType	\
		otherPort

	#Create a new window
	toplevel .serial
	wm title .serial "Port Settings"
	wm iconname .serial "Port Settings"
	grab .serial
	focus .serial
		
	#Create a frame to hold the port selection widgets
	frame .serial.ports 	\
		-relief sunken	\
		-borderwidth 2
		
	label .serial.serialTitle	\
		-text "Port Settings"	\
		-font {-weight bold}
		
	grid .serial.serialTitle -row 0 -column 0 -columnspan 2 -sticky w
	grid .serial.ports -row 1 -column 0 -columnspan 2 -sticky we
	grid columnconfigure .serial 0 -weight 1
	grid columnconfigure .serial.ports 0 -weight 1
	
	#if {$osType == "windows"} {
		
		label .serial.ports.title	\
			-text "Please Select A COM Port:"	\
			-font {-weight bold -size -12}
			
		radiobutton .serial.ports.showAvailable	\
			-text "Show Available Ports"	\
			-value showAvailable	\
			-variable usbSerial::portShow	\
			-command "usbSerial::updateValidPorts 1"
		
		radiobutton .serial.ports.showAll	\
			-text "Show All Ports"	\
			-value showAll	\
			-variable usbSerial::portShow	\
			-command "usbSerial::updateValidPorts 0"
		
		ttk::treeview .serial.ports.portList	\
			-yscrollcommand {.serial.ports.portScroll set}	\
			-columns "Description"	\
			-selectmode browse
		.serial.ports.portList column #0 -width 225
		.serial.ports.portList column Description -width 400
		

		if {$usbSerial::portShow == "showAvailable"} {
			usbSerial::updateValidPorts 1
		} else {
			usbSerial::updateValidPorts 0
		}

		scrollbar .serial.ports.portScroll	\
			-orient vertical	\
			-command {.serial.ports.portList yview}
			
		button .serial.connect	\
			-text "Connect"	\
			-command {
				destroy .serial
				if {[usbSerial::openSerialPort]==1} {
					set fileId [open port.cfg w]
					puts $fileId $usbSerial::comNum
					close $fileId
				}
			}
			
		grid .serial.ports.title -row 0 -column 0 -sticky w -columnspan 3
		grid .serial.ports.showAll -row 1 -column 0
		grid .serial.ports.showAvailable -row 1 -column 1
		grid .serial.ports.portList -row 2 -column 0 -columnspan 2 -sticky we
		grid .serial.ports.portScroll -row 2 -column 2 -sticky ns
		grid .serial.connect -row 3 -column 0 -columnspan 3
		
			
		#grid .serial.ports.autodetect -row 3 -column 0 -columnspan $i
	#} elseif {$osType == "unix"} {
	#	
	#	#Create a list of valid port names
	#	for {set i 0} {$i < 4} {incr i} {
	#		lappend validPorts "/dev/ttyUSB$i"
	#	}
	#	
	#	#Create buttons for these ports
	#	for {set i 0} {$i < 4} {incr i} {
	#		radiobutton .serial.ports.ttyButton$i	\
	#			-text "/dev/ttyUSB$i"	\
	#			-variable usbSerial::serialPort	\
	#			-value "/dev/ttyUSB$i"	
	#		grid .serial.ports.ttyButton$i -row [expr $i] -column 0
	#	}
	#	#Radio button and entry widgets so that the user
	#	#can manually enter the name of a serial port
	#	radiobutton .serial.ports.otherButton	\
	#		-text "Other:"	\
	#		-variable usbSerial::serialPort	\
	#		-value other
	#	grid .serial.ports.otherButton -row [expr $i+1] -column 0
	#	entry .serial.ports.otherEntry	\
	#		-textvariable otherPort
	#	grid .serial.ports.otherEntry -row [expr $i+2] -column 0
	#	
	#} elseif {$osType == "Darwin"} {
	#	set darwinPort [glob /dev/cu.*]
	#	for {set i 0} {$i < [llength $darwinPort]} {incr i} {
	#		radiobutton .serial.ports.cuButton$i	\
	#			-text [lindex $darwinPort $i]	\
	#			-variable usbSerial::serialPort	\
	#			-value [lindex $darwinPort $i]
	#		grid .serial.ports.cuButton$i -row $i -column 0
	#	}
	#	radiobutton .serial.ports.otherButton	\
	#		-text "Other:"	\
	#		-variable usbSerial::serialPort	\
	#		-value other
	#	incr i
	#	grid .serial.ports.otherButton -row $i -column 0
	#	entry .serial.ports.otherEntry	\
	#		-textvariable otherPort
	#	incr i
	#	grid .serial.ports.otherEntry -row $i -column 0
	#
	#}
	
	#Center the serial port settings window over the parent
	#See http://wiki.tcl.tk/534
	update
	set w [winfo width .]
	set h [winfo height .]
	set x [winfo rootx .]
	set y [winfo rooty .]
	set xpos "+[ expr {$x+($w-[winfo width .serial])/2}]"
	set ypos "+[ expr {$y+($h-[winfo height .serial])/2}]"
	wm geometry .serial "$xpos$ypos"
	raise .serial
	update
	
}

proc usbSerial::autodetectWindows {} {
	global portHandle	\
		serialCheck
	
	#Close the serial port in case the user has already
	#connected to the instrument
	usbSerial::closeSerialPort
	
	#Create a text widget where we can display the progress of the autodetection
	#process.
	toplevel .auto
	wm title .auto "Auto-Detect"
	label .auto.autoDetectTitle	\
			-text "Auto-Detecting..."	\
			-font {-weight bold -size -14}	\
			-anchor center
	text .auto.autoStatus	\
		-height 10		\
		-yscrollcommand ".auto.textScroll set"
	scrollbar .auto.textScroll	\
		-orient vert	\
		-command ".auto.autoStatus yview"
	.auto.autoStatus insert end "Beginning autodetection...\n"
	
	#Replace the serial port radio buttons with the text widget
	grid .auto.autoDetectTitle -row 0 -column 0 -columnspan 2
	grid .auto.autoStatus -row 1 -column 0
	grid .auto.textScroll -row 1 -column 1 -sticky ns
	
	#Center the serial port settings window over the parent
	#See http://wiki.tcl.tk/534
	update
	set w [winfo width .]
	set h [winfo height .]
	set x [winfo rootx .]
	set y [winfo rooty .]
	set xpos "+[ expr {$x+($w-[winfo width .auto])/2}]"
	set ypos "+[ expr {$y+($h-[winfo height .auto])/2}]"
	wm geometry .auto "$xpos$ypos"

	#Here we loop through COM ports 1-99, attempting to open them
	for {set i 3} {$i <100} {incr i} {
		if {$i < 10} {
			set serialPort "COM$i:"
		} else {
			set serialPort "\\\\.\\COM$i"
		}
		if { [catch {set portHandle [open $serialPort r+]} result] } {
			puts "Unable to open $serialPort: $result"
			flush stdout
			.auto.autoStatus insert end "COM$i: Unavailable\n"
			update
		} else {
			#We were able to open a port.  Configure baud
			#settings and prepare to query the device.
			if { [catch {fconfigure $portHandle -mode 230400,n,8,1 -blocking 0 -buffering line} result] } {
				puts "Unable to configure $serialPort: $result"
				flush stdout
				.auto.autoStatus insert end "COM$i: $result\n"
				update
				usbSerial::closeSerialPort
			} else {
			
				#Here we set up an intermediate fileevent handler
				#to receive descriptor strings from the instrument.
				fileevent $portHandle readable {
					set incomingData [gets $portHandle]
					puts "incomingData: $incomingData"
					if {   [lsearch $incomingData "CircuitGear"] !=-1  } {
						set serialCheck found
					} else {
						puts "No match"
					}
				}

				#Query the device.
				puts $portHandle ""
				puts $portHandle ""
				flush $portHandle
				after 500
				set junk [read $portHandle]
				puts $portHandle "i"
				.auto.autoStatus insert end "COM$i: Opened..."
				flush stdout
				update
				.auto.autoStatus insert end "Querying..."
				
				#Set up a timeout in case the device on this COM
				#port does not repsond to the query.
				set serialCheck waiting
				set serialCheckHandle [after 1500 {set serialCheck timeout}]
				vwait serialCheck
				
				if { $serialCheck == "found" } {
					#Device responded on this COM port.  We've
					#located the scope.
					.auto.autoStatus insert end "Found!\n"
					tk_messageBox	\
						-title "Auto-Detect Successful"	\
						-default ok		\
						-message "CYScope Found on COM$i"	\
						-type ok			\
						-icon info	\
						-parent .auto
					puts "Circuit Gear found on $serialPort"
					catch { [close $portHandle]}
					set usbSerial::serialPort $serialPort
					set usbSerial::comNum $i
					#Save the COM port into the port.cfg file
					set fileId [open port.cfg w]
					puts $fileId $usbSerial::comNum
					close $fileId
					destroy .auto
					update
					after 750
					usbSerial::openSerialPort
					return 1
				} else {
					puts "No Device on $serialPort"
					.auto.autoStatus insert end "Not A Syscomp Device\n"
					catch { [close $portHandle]}
				}
			}
		}
	}
	#At this point, we've been through all COM ports without any
	#luck.  We relay the bad news to the user.
	tk_messageBox	\
			-title "Hardware Error"	\
			-default ok		\
			-message "Unable to Autodetect"	\
			-type ok			\
			-icon warning	\
			-parent .auto
	
	#Autodetection failed.
	destroy .auto
	return 0

}

proc usbSerial::autodetectLinux {} {
	global portHandle	\
		serialCheck
	
	#Close the serial port in case the user has already
	#connected to the instrument
	usbSerial::closeSerialPort
	
	#Create a text widget where we can display the progress of the autodetection
	#process.
	toplevel .auto
	wm title .auto "Auto-Detect"
	label .auto.autoDetectTitle	\
			-text "Auto-Detecting..."	\
			-font {-weight bold -size -14}	\
			-anchor center
	text .auto.autoStatus	\
		-height 10		\
		-yscrollcommand ".auto.textScroll set"
	scrollbar .auto.textScroll	\
		-orient vert	\
		-command ".auto.autoStatus yview"
	.auto.autoStatus insert end "Beginning autodetection...\n"
	
	#Replace the serial port radio buttons with the text widget
	grid .auto.autoDetectTitle -row 0 -column 0 -columnspan 2
	grid .auto.autoStatus -row 1 -column 0
	grid .auto.textScroll -row 1 -column 1 -sticky ns
	
	#Center the serial port settings window over the parent
	#See http://wiki.tcl.tk/534
	update
	set w [winfo width .]
	set h [winfo height .]
	set x [winfo rootx .]
	set y [winfo rooty .]
	set xpos "+[ expr {$x+($w-[winfo width .auto])/2}]"
	set ypos "+[ expr {$y+($h-[winfo height .auto])/2}]"
	wm geometry .auto "$xpos$ypos"

	#Here we loop through COM ports 1-99, attempting to open them
	for {set i 0} {$i <100} {incr i} {
		
		set serialPort "/dev/ttyUSB$i"

		if { [catch {set portHandle [open $serialPort r+]} result] } {
			puts "Unable to open $serialPort: $result"
			flush stdout
			.auto.autoStatus insert end "/dev/ttyUSB$i Unavailable\n"
			update
		} else {
			#We were able to open a port.  Configure baud
			#settings and prepare to query the device.
			if { [catch {fconfigure $portHandle -mode 230400,n,8,1 -blocking 0 -buffering line} result] } {
				puts "Unable to configure $serialPort: $result"
				flush stdout
				.auto.autoStatus insert end "$serialPort: $result\n"
				update
				usbSerial::closeSerialPort
			} else {
			
				#Here we set up an intermediate fileevent handler
				#to receive descriptor strings from the instrument.
				fileevent $portHandle readable {
					set incomingData [gets $portHandle]
					puts "incomingData: $incomingData"
					if {   [lsearch $incomingData "CircuitGear"] !=-1  } {
						set ::serialCheck found
						puts "serialCheck $::serialCheck"
					} else {
						puts "No match"
					}
				}

				puts "Querying $serialPort"

				#Flush the device
				puts $portHandle ""
				puts $portHandle ""
				flush $portHandle
				after 500
				set junk [read $portHandle]
				.auto.autoStatus insert end "$serialPort: Opened..."
				update
				flush stdout
				.auto.autoStatus insert end "Querying..."
				update
				
				#Set up a timeout in case the device on this COM
				#port does not repsond to the query.
				set serialCheck waiting
				set serialCheckHandle [after 1500 {set serialCheck timeout}]
				puts $portHandle "i"
				flush $portHandle
				vwait ::serialCheck
				after cancel $serialCheckHandle 
				puts "serialCheck $serialCheck"
				
				if { $serialCheck == "found" } {
					#Device responded on this COM port.  We've
					#located the scope.
					.auto.autoStatus insert end "Found!\n"
					tk_messageBox	\
						-title "Auto-Detect Successful"	\
						-default ok		\
						-message "CYScope Found on $serialPort"	\
						-type ok			\
						-icon info	\
						-parent .auto
					puts "Circuit Gear found on $serialPort"
					catch { [close $portHandle]}
					set usbSerial::serialPort $serialPort
					set usbSerial::comNum $serialPort
					#Save the COM port into the port.cfg file
					set fileId [open port.cfg w]
					puts $fileId $usbSerial::comNum
					close $fileId
					destroy .auto
					usbSerial::openSerialPort
					return 1
				} else {
					puts "No Device on $serialPort"
					.auto.autoStatus insert end "Not A Syscomp Device\n"
					catch { [close $portHandle]}
				}
			}
		}
	}
	#At this point, we've been through all COM ports without any
	#luck.  We relay the bad news to the user.
	tk_messageBox	\
			-title "Hardware Error"	\
			-default ok		\
			-message "Unable to Autodetect"	\
			-type ok			\
			-icon warning	\
			-parent .auto
	
	#Autodetection failed.
	destroy .auto
	return 0

}



proc usbSerial::winDetectPorts {} {

	set serialLog [exec EnumSer.exe]
	
	#Find the start index of the com port list
	set startIndex [lsearch -all $serialLog "reports"]
	set startIndex [lindex $startIndex 3]
	incr startIndex
	
	#Find the end index of the com port list
	set endIndex [lsearch -all $serialLog "Take"]
	set endIndex [lindex $endIndex 3]
	set endIndex [expr {$endIndex-1}]
	
	#See if there are any com ports present
	if {($startIndex == $endIndex) || ($startIndex > $endIndex)} {
		tk_messageBox	\
			-message "No Com Ports Detected"	\
			-default ok	\
			-parent .	\
			-title "Serial Detection"	\
			-type ok
		return -1
	}
	
	#Create a list of the available com ports
	set comList {}
	set i $startIndex
	while {$i <= $endIndex} {
		#COM port is the first entry on the line
		set comPort [lindex $serialLog $i]
		incr i
		
		#Gather all data up to and including the ">" character
		set description {}
		while {[string first > [lindex $serialLog $i]] < 0} {
			lappend description [lindex $serialLog $i]
			incr i
		}
		lappend description [lindex $serialLog $i]
		incr i
		
		set description [join $description]
		
		lappend comList [list $comPort $description]
	}

	return $comList

}

proc usbSerial::linDetectPorts {} {
	
	#See if there are any usb-serial ports available in the dev directory
	if {[catch {set devList [glob -directory /dev/ ttyUSB*]} result]} {
		set devList ""
	}
	
	#We did not find any ports
	if {$devList == ""} {
		update
		tk_messageBox	\
			-message "No Com Ports Detected"	\
			-default ok	\
			-parent .serial	\
			-title "Serial Detection"	\
			-type ok
		return -1
	}
	
	set comList {}
	foreach dev $devList {
		set description [exec ls -al $dev]
		lappend comList [list $dev $description]
	}
	
	return $comList
	
}

proc usbSerial::macDetectPorts {} {
	
	#See if there are any usb-serial ports available in the dev directory
	if {[catch {set devList [glob -directory /dev/ cu.usbserial*]} result]} {
		set devList ""
	}
	
	#We did not find any ports
	if {$devList == ""} {
		update
		tk_messageBox	\
			-message "No Com Ports Detected"	\
			-default ok	\
			-parent .serial	\
			-title "Serial Detection"	\
			-type ok
		return -1
	}
	
	set comList {}
	foreach dev $devList {
		set description [exec ls -al $dev]
		lappend comList [list $dev $description]
	}
	
	return $comList
	
}

proc usbSerial::updateValidPorts {detectPorts} {
	variable validPorts
	
	#Clear the current list of valid com ports
	set validPorts {}
	.serial.ports.portList delete [.serial.ports.portList children {}]

	if {$::osType == "windows"} {
		
		#Get a list of the current ports on the system
		set portList [usbSerial::winDetectPorts]
		
		if {$detectPorts} {
			#Make sure ports were detected
			if {$portList == -1} {
				set usbSerial::portShow showAll
			}
		}
	
		#See if we need to detect the ports or just create a big list
		if {$detectPorts && ($portList != -1)} {
			#Limit the list to the ports available on the system
			set validPorts $portList
		} else {
			for {set i 1} {$i < 100} {incr i} {
				set comString "COM$i"
				foreach port $portList {
					if {[lsearch -glob $port $comString] >= 0} {
						set description [lindex $port 1]
						break
					} else {
						set description "<Not Present>"
					}
				}
				lappend validPorts [list $comString $description]
			}
		}
		
		foreach port $validPorts {
			set temp [list [lindex $port 1]]
			if {[lindex $temp 0]== "<USB Serial Port>"} {
				set color "Dark Green"
			} elseif {[lindex $temp 0] == "<Not Present>"} {
				set color "Black"
			} else {
				set color "Orange"
			}
			
			set portId [lindex $port 0]
			.serial.ports.portList insert {} end -id $portId -text $portId  -values "$temp" -tags $portId
			.serial.ports.portList tag configure $portId -foreground $color
			.serial.ports.portList tag bind $portId <<TreeviewSelect>> {
				update
				set temp [.serial.ports.portList focus]
				set usbSerial::comNum [string range $temp 3 end]
				if {$usbSerial::comNum < 10} {
					set usbSerial::serialPort "COM$usbSerial::comNum"
				} else {
					set usbSerial::serialPort "\\\\.\\COM$usbSerial::comNum"
				}
			}
		}
	} elseif {$::osType == "unix"} {
		
		if {$detectPorts} {
			#Get a list of the current ports on the system
			set portList [usbSerial::linDetectPorts]
			#Make sure ports were detected
			if {$portList == -1} {
				.serial.ports.showAll invoke
				return
			}
		}
		
		#See if we need to detect the ports or just create a big list
		if {$detectPorts} {
			#Limit the list to the ports available on the system
			set validPorts $portList
		} else {
			for {set i 0} {$i < 100} {incr i} {
				set comString "/dev/ttyUSB$i"
				set description ""
				lappend validPorts [list $comString $description]
			}
		}
		
		foreach port $validPorts {
			set temp [list [lindex $port 1]]
			set color black
			
			set portId [lindex $port 0]
			.serial.ports.portList insert {} end -id $portId -text $portId  -values "$temp" -tags $portId
			.serial.ports.portList tag configure $portId -foreground $color
			.serial.ports.portList tag bind $portId <<TreeviewSelect>> {
				update
				set temp [.serial.ports.portList focus]
				set usbSerial::comNum $temp
				set usbSerial::serialPort $temp
			}
		}
		
	}  elseif {$::osType == "Darwin"} {
		
		if {$detectPorts} {
			#Get a list of the current ports on the system
			set portList [usbSerial::macDetectPorts]
			#Make sure ports were detected
			if {$portList == -1} {
				.serial.ports.showAll invoke
				return
			}
		}
		
		#See if we need to detect the ports or just create a big list
		if {$detectPorts} {
			#Limit the list to the ports available on the system
			set validPorts $portList
		} else {
			lappend validPorts [list /dev/cu.usbSerial ""]
			for {set i 0} {$i < 100} {incr i} {
				set comString "/dev/ttyUSB$i"
				set description ""
				lappend validPorts [list $comString $description]
			}
		}
		
		foreach port $validPorts {
			set temp [list [lindex $port 1]]
			set color black
			
			set portId [lindex $port 0]
			.serial.ports.portList insert {} end -id $portId -text $portId  -values "$temp" -tags $portId
			.serial.ports.portList tag configure $portId -foreground $color
			.serial.ports.portList tag bind $portId <<TreeviewSelect>> {
				update
				set temp [.serial.ports.portList focus]
				set usbSerial::comNum $temp
				set usbSerial::serialPort $temp
			}
		}
		
	}
}

proc usbSerial::getStoredPort {} {

	if [catch {open port.cfg r+} fileId] {
		return 0
	} else {
		if {$::osType == "windows"} {
			if { [gets $fileId line] >= 0} {
				set usbSerial::comNum $line
				if {$usbSerial::comNum < 10} {
					set usbSerial::serialPort "COM$usbSerial::comNum"
				} else {
					set usbSerial::serialPort "\\\\.\\COM$usbSerial::comNum"
				}
				close $fileId
				return 1
			} else {
				close $fileId
				return 0
			}
		} else {
			if {[gets $fileId line] >= 0} {
				set usbSerial::serialPort $line
				close $fileId
				return 1
			} else {
				close $fileId
				return 0
			}
		}
	}

}

