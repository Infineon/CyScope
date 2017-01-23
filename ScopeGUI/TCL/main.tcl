#File: main.tcl
#Cypress CY8CKIT-059 Based Oscilloscope and Waveform Generator
#Main File

#GJL
set softwareVersion "1.19"
#This is a modified version of Syscomp CircuitGear GUI Version 1.19
#which is provided under the Open Instrumentation Project
#The source code is Copyright 2008 Syscomp Electronic Design
#www.syscompdesign.com

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

#---=== Global Variables ===---

#Folder Location for GUI Images
set images "Images"

#GUI Operational Mode
set opMode "CircuitGear"

#Debug level for printing messages to the console
set debugLevel 1

#Figure out which operating system we're running on
set osType $tcl_platform(platform)
if {$osType == "unix"} {
	if {[exec uname] == "Darwin"} {set osType "Darwin"}
}

# GJL - add lappend before each require command for freewrap to work
# An lappend for twapi is also needed in capture.tcl

#BWidget package used for comboboxes
lappend auto_path ./Lib/bwidget-1.9.10
package require BWidget

#Img package required for screen captures
lappend auto_path ./Lib/Img-win32
package require Img

#---=== Includes ===---
source usbSerial.tcl
source dialog.tcl
source scope.tcl
source waveform.tcl
source digio.tcl

package require usbSerial 1.0
package require scope 1.0
package require waveform 1.0
package require digio 1.0

#Bring the public commands into the global namespace
namespace import ::scope::*
namespace import ::wave::*
namespace import ::usbSerial::*
namespace import ::digio::*


#---=== Procedures ===---

proc showAbout {} {
	tk_messageBox	\
		-message "Cypress Semiconductor Inc.\nCY8CKIT-059 Scope GUI Version $::softwareVersion\n$usbSerial::firmwareIdent\nwww.cypress.com"	\
		-default ok	\
		-icon info	\
		-title "About"
}

proc showManual {} {

	set scriptPath [file dirname [info script]]
	
	 if {$::osType=="windows"} {
		if {[file exists "$scriptPath/../Documentation/circuit-gear-manual.pdf"]} {
			puts "Launching manual from Documenation directory"
			eval exec [auto_execok start] \"\" [list "$scriptPath/../Documentation/circuit-gear-manual.pdf"]
		} else {
			puts "Launcing manual from Source directory"
			eval exec [auto_execok start] \"\" [list "circuit-gear-manual.pdf"]
		}
	} else {
		if {$::osType == "Darwin"} {
			eval exec open [list "circuit-gear-manual.pdf"]
		} else {
			eval exec see [list "circuit-gear-manual.pdf"]
		}
	}



}

proc showChangeLog {} {

	#Make sure the change log isn't already open
	if {[winfo exists .changeLog]} {
		raise .changeLog
		focus .changeLog
		return
	}
	
	#Create a changelog window
	toplevel .changeLog
	wm title .changeLog "CYScope Change Log"
	
	set fileId [open "Changes.txt" r]
	set changeData [read $fileId]
	close $fileId
	
	text .changeLog.log	\
		-width 80	\
		-yscrollcommand ".changeLog.scrollVert set"	\
		-xscrollcommand ".changeLog.scrollHor set"	\
		-wrap none
		
	.changeLog.log insert end $changeData
	.changeLog.log configure -state disabled
		
	scrollbar .changeLog.scrollVert	\
		-command ".changeLog.log yview"	\
		-orient vertical
		
	scrollbar .changeLog.scrollHor	\
		-command ".changeLog.log xview"	\
		-orient horizontal
	
	grid .changeLog.log -row 0 -column 0 -sticky news
	grid .changeLog.scrollVert -row 0 -column 1 -sticky ns
	grid .changeLog.scrollHor -row 1 -column 0 -sticky we
	grid rowconfigure .changeLog .changeLog.log -weight 1
	grid rowconfigure .changeLog .changeLog.scrollVert -weight 1
	grid columnconfigure .changeLog .changeLog.log -weight 1
	grid columnconfigure .changeLog .changeLog.scrollHor -weight 1

}

proc saveSettings {} {

	set types {
		{{Config Files}	{.cfg}}
	}
	
	set settingsFile [tk_getSaveFile -filetypes $types]
	
	if {$settingsFile == ""} {return}
	
	if {[catch {open "$settingsFile.cfg" w} fileId]} {
		tk_messageBox	\
			-message "Unable to write to saved settings file."	\
			-type ok	\
			-icon error
		saveSettings
		return
	}
	
	#Save the vertical settings
	puts $fileId $scope::verticalIndexA
	puts $fileId $scope::verticalIndexB
	puts $fileId $scope::probeA
	puts $fileId $scope::probeB
	
	#Save the timebase settings
	puts $fileId $scope::timebaseIndex
	
	#Save trigger settings
	puts $fileId $scope::triggerMode
	puts $fileId $scope::triggerSlope
	puts $fileId $scope::triggerSource
	
	#Save waveform generator frequency
	puts $fileId $wave::waveFrequency
	
	#Save waveform generator amplitude
	puts $fileId $wave::amplitude
	
	#Save waveform generator offset
	puts $fileId $wave::offset

	#Save current waveform file name
	puts $fileId $wave::waveFile
	
	#Save waveform generator frequency slider mode
	puts $fileId $wave::sliderMode
	
	#Save the state of the digital outputs
	puts $fileId $digio::digout(0)
	puts $fileId $digio::digout(1)
	puts $fileId $digio::digout(2)
	puts $fileId $digio::digout(3)
	puts $fileId $digio::digout(4)
	puts $fileId $digio::digout(5)
	puts $fileId $digio::digout(6)
	puts $fileId $digio::digout(7)
	
	#Save the interrupt settings
	puts $fileId $digio::intMode

	#Save the pwm settings
	puts $fileId $digio::pwmDuty
	puts $fileId $digio::pwmFreq
	
	#Save cursor settings
	if {$scope::triggerSource == "A"} {
		puts $fileId [expr {$cursor::trigPos-$cursor::chAGndPos}]
	} else {
		puts $fileId [expr {$cursor::trigPos-$cursor::chBGndPos}]
	}
	puts $fileId $cursor::chAGndPos
	puts $fileId $cursor::chBGndPos

	close $fileId
	


}

proc loadSettings {} {

	set types {
		{{Config Files}	{.cfg}}
	}
	
	set settingsFile [tk_getOpenFile -filetypes $types]
	if {$settingsFile == ""} {
		return
	}
	
	#Open the file for reading
	if {[catch {open $settingsFile r}  fileId]} {
		tk_messageBox	\
			-message "Unable to open settings file."	\
			-type ok	\
			-icon warning
		return
	}
	
	#Read out all settings from the file
	set settings {}
	while {[gets $fileId line] >= 0} {
		lappend settings $line
	}
	close $fileId
	
	#Restore vertical settings
	set scope::verticalIndexA [lindex $settings 0]
	set scope::verticalIndexB [lindex $settings 1]
	set scope::verticalA [lindex $scope::verticalValues $scope::verticalIndexA]
	set scope::verticalB [lindex $scope::verticalValues $scope::verticalIndexB]
	scope::setVertical
	cursor::measureVoltageCursors
	set scope::probeA [lindex $settings 2]
	set scope::probeB [lindex $settings 3]
	
	#Restore timebase setting
	set scope::timebaseIndex [lindex $settings 4]
	scope::adjustTimebase update

	#Restore trigger settings
	set scope::triggerMode [lindex $settings 5]
	set scope::triggerSlope [lindex $settings 6]
	set scope::triggerSource [lindex $settings 7]
	scope::selectTriggerMode
	scope::updateScopeControlReg
	
	#Restore waveform generator frequency
	set wave::waveFrequency [lindex $settings 8]
	wave::sendFrequency $wave::waveFrequency
	set wave::frequencyDisplay "$wave::waveFrequency Hz"
	
	#Restore waveform generator amplitude
	set wave::amplitude [lindex $settings 9]
	wave::adjustAmplitude $wave::amplitude
	
	#GJL
	#Restore waveform generator offset
	set wave::offset [lindex $settings 10]
	wave::adjustOffset $wave::offset

	#Restore current waveform
	set wave::waveFile [lindex $settings 11]
	if {$wave::waveFile == "noise"} {
		wave::enableNoise
	} else {
		wave::programWaveform $wave::waveFile
	}
	
	#Restore waveform generator frequency slider mode
	set wave::sliderMode [lindex $settings 12]
	
	#Restore digital outputs
	if {[lindex $settings 13]} {digio::toggleOutBit 0}
	if {[lindex $settings 14]} {digio::toggleOutBit 1}
	if {[lindex $settings 15]} {digio::toggleOutBit 2}
	if {[lindex $settings 16]} {digio::toggleOutBit 3}
	if {[lindex $settings 17]} {digio::toggleOutBit 4}
	if {[lindex $settings 18]} {digio::toggleOutBit 5}
	if {[lindex $settings 19]} {digio::toggleOutBit 6}
	if {[lindex $settings 20]} {digio::toggleOutBit 7}

	#Restore interrupt settings
	set digio::intMode [lindex $settings 21]
	digio::selectIntMode

	#Restore PWM settings
	set digio::pwmDuty [lindex $settings 22]
	digio::updatePWM $digio::pwmDuty
	set digio::pwmFreq [lindex $settings 23]
	digio::selectFreq
	
	#Restore cursor settings
	set cursor::trigPos [lindex $settings 24]
	set cursor::yStart $scope::yMid
	cursor::moveTrigger [expr {$scope::yMid + $cursor::trigPos}]
	set cursor::chAGndPos [lindex $settings 25]
	set cursor::yStart $scope::yMid
	cursor::moveChAGnd $cursor::chAGndPos
	set cursor::chBGndPos [lindex $settings 26]
	set cursor::yStart $scope::yMid
	cursor::moveChBGnd $cursor::chBGndPos
	
	
	
}

#---=== End of Procedures ===---

# KEES trying to get the command console to show at startup
#console show

#Construct the scope portion of the GUI
setScopePath ".s"
buildScope

#Construct the waveform generator portion of the GUI
setWavePath ".w"
buildWave

#Construct the digital I/O portion of the GUI
setDigioPath ".d"
buildDigio

#Standard windows options
wm title . "Cypress CY8CKIT-059 PSoC 5LP Oscilloscope and Waveform Generator"
wm resizable . 0 0

#Create the menu bar
frame .menubar -relief raised -borderwidth 1

#Create the drop down menus

#File Menu
menubutton .menubar.file	\
	-text "File"		\
	-menu .menubar.file.filemenu
menu .menubar.file.filemenu -tearoff 0
.menubar.file.filemenu add command	\
	-label "Save Settings"	\
	-command saveSettings
.menubar.file.filemenu add command 	\
	-label "Load Settings"	\
	-command loadSettings
.menubar.file.filemenu add separator
.menubar.file.filemenu add command	\
	-label "Exit"	\
	-command {destroy .}

#View Menu
menubutton .menubar.scopeView \
	-text "View"	\
	-menu .menubar.scopeView.viewMenu
menu .menubar.scopeView.viewMenu -tearoff 0
if {$osType == "windows"} {
	.menubar.scopeView.viewMenu add command	\
		-label "Debug Console"	\
		-command {console show}
}
#Floating Window for Digital I/O
.menubar.scopeView.viewMenu add check	\
	-label "Dock Digital I/O Controls"	\
	-variable digio::state	\
	-command digio::floatDigio	\
	-onvalue docked	\
	-offvalue float
#XY Mode selector
.menubar.scopeView.viewMenu add check	\
	-label "XY Mode"	\
	-variable scope::xyEnable	\
	-command scope::toggleXYMode

#Tools Menu
menubutton .menubar.tools	\
	-text "Tools"	\
	-menu .menubar.tools.toolsMenu
menu .menubar.tools.toolsMenu -tearoff 0
#Add export to Postscript command
.menubar.tools.toolsMenu add command \
	-label "Export scope display to PostScript (PS) File"	\
	-command {scope::psExport}
#Screen capture command
.menubar.tools.toolsMenu add command	\
	-label "Screen Capture (jpg)"		\
	-command {
		set capture::captureMode jpg
		capture::showCapture
	}
.menubar.tools.toolsMenu add command	\
	-label "Screen Capture (png)"		\
	-command {
		set capture::captureMode png
		capture::showCapture
	}
		
.menubar.tools.toolsMenu add separator
#Add offset calibration command to "Tools" menu
.menubar.tools.toolsMenu add command	\
	-label "Calibrate Scope Offsets"	\
	-command scope::showOffsetCal
.menubar.tools.toolsMenu add command	\
	-label "Calibrate Scope Vertical Scale"	\
	-command cal::calibration
.menubar.tools.toolsMenu add separator	
#WaveMaker command
.menubar.tools.toolsMenu add command	\
	-label "WaveMaker Waveform Editor"	\
	-command waveMaker::showWaveMaker
.menubar.tools.toolsMenu add separator	
	

#Hardware Menu
menubutton .menubar.hardware	\
	-text "Hardware"	\
	-menu .menubar.hardware.hardwareMenu
menu .menubar.hardware.hardwareMenu	-tearoff 0
.menubar.hardware.hardwareMenu add command	\
	-label "Connect..."	\
	-command ::usbSerial::openSerialPort
.menubar.hardware.hardwareMenu add separator
#Selector for CircuitGear Mode
.menubar.hardware.hardwareMenu add check	\
	-label "CircuitGear Mode"	\
	-variable opMode			\
	-onvalue "CircuitGear"		\
	-command net::toggleOpMode
#Selector for Network Analyzer Mode
.menubar.hardware.hardwareMenu add check	\
	-label "Network Analyser Mode"	\
	-variable opMode			\
	-onvalue "Netalyzer"		\
	-offvalue "CircuitGear"		\
	-command net::toggleOpMode
.menubar.hardware.hardwareMenu add separator
.menubar.hardware.hardwareMenu add check	\
	-label "Disable Trigger Filtering"	\
	-variable scope::triggerFilterDisable	\
	-onvalue 1	\
	-offvalue 0


#Help Menu
menubutton .menubar.help	\
	-text "Help"		\
	-menu .menubar.help.helpMenu
menu .menubar.help.helpMenu -tearoff 0
.menubar.help.helpMenu add command	\
	-label "About"	\
	-command showAbout
.menubar.help.helpMenu add separator
.menubar.help.helpMenu add command	\
	-label "Manual (pdf)"	\
	-command showManual
.menubar.help.helpMenu add separator
.menubar.help.helpMenu add command	\
	-label "Change Log"	\
	-command showChangeLog
.menubar.help.helpMenu add separator


#Create an indicator for the status of the serial-usb connection
label .menubar.serialPortStatus	\
	-textvariable ::usbSerial::serialStatus	\
	-background red
label .menubar.spacer	\
	-text "    "

#Place the menus on the menubar
grid .menubar.file -row 0 -column 0 -sticky w
grid .menubar.scopeView -row 0 -column 1 -sticky w
grid .menubar.tools -row 0 -column 2 -sticky w
grid .menubar.hardware -row 0 -column 3 -sticky w
grid .menubar.help -row 0 -column 4 -sticky w
grid .menubar.spacer -row 0 -column 5 -sticky w
grid .menubar.serialPortStatus -row 0 -column 6 -sticky w

#Place the major Frames
grid .menubar -row 0 -column 0 -sticky w -columnspan 2
proc arrangeCircuitGear {} {
	grid .s -row 2 -column 0
	grid .w -row 2 -column 1 -padx 5
	grid .d -row 3 -column 0 -sticky we -pady 5 -columnspan 2
}
arrangeCircuitGear

#Add-Ons
source cursors.tcl
source capture.tcl
source automeasure.tcl

proc initializeCGR {} {

	#Retrieve calibration data from the scope
	sendCommand "S O"
	vwait usbSerial::receivedData

	scope::setVertical

	#Cursor Set-Up
	cursor::drawXCursor
	cursor::drawTriggerCursor
	cursor::drawChAGndCursor
	cursor::drawChBGndCursor
	cursor::moveTrigger $cursor::trigPos; [getScopePath].display delete trigLevelValue

	scope::acquireWaveform
	digio::updateDigio

	#Turn on auto-update for digital inputs
	sendCommand "D A"
	
	#Change the generator frequency to ~250Hz
	[getWavePath].freq.freqSlider set 160
	
}

#Center the window on the screen
update
set width [winfo width .]
set height [winfo height .]
set screenWidth [winfo screenwidth .]
set screenHeight [winfo screenheight .]
set x [expr {round(($screenWidth-$width)/2.0)}]
set y 10
set newGeo "+$x"
append newGeo "+$y"
wm geometry . $newGeo

usbSerial::getStoredPort
usbSerial::openSerialPort
#Add-ons
source calibration.tcl
source netalyzer.tcl
source export.tcl
source FFT.tcl
source dataRecorder.tcl
source persist.tcl
source math.tcl
source updateCheck.tcl
source wavemaker.tcl
scope::readColorSettings
