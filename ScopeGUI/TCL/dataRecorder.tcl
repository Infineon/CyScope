#File: dataRecorder.tcl
#Syscomp USB Oscilloscope GUI
#Data Recorder Toolbox

#JG
#Copyright 2007 Syscomp Electronic Design
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

.menubar.tools.toolsMenu add command \
	-label "Data Recorder"	\
	-command {dataRec::showRecorder}
.menubar.tools.toolsMenu add separator

namespace eval dataRec {
set recorderEnabled 0
set dataLogEnable 0
set numberLogs 0
set logPeriod 5
set recorderStatus "Idle"
set recordDir "."
set logNow 0

}

proc dataRec::showRecorder {} {
	
	if $dataRec::recorderEnabled {wm deiconify .recorder; raise .recorder; return}
	
	toplevel .recorder
	wm title .recorder "Data Recorder Toolbox"
	wm resizable .recorder 0 0
	bind .recorder <Destroy> {set dataRec::recorderEnabled 0}
	
	button .recorder.enableBox	\
		-text "Start Logging" \
		-command dataRec::toggleRecording
		
	label .recorder.logsText	\
		-text "Log Count:"
		
	label .recorder.logsCount	\
		-textvariable dataRec::numberLogs	\
		-width 10	\
		-relief sunken
		
	label .recorder.periodText 	\
		-text "Log Period (s):"
	
	entry .recorder.periodTime	\
		-textvariable dataRec::logPeriod	\
		-width 10	\
		-justify center
		
	label .recorder.statusText	\
		-text "Recorder Status:"
		
	label .recorder.status	\
		-textvariable dataRec::recorderStatus	\
		-width 20	\
		-relief sunken
		
	button .recorder.chooseDir	\
		-text "Choose Export Directory"	\
		-command {set dataRec::recordDir [tk_chooseDirectory]}
	
	entry .recorder.currentDir	\
		-textvariable dataRec::recordDir	\
		-width 30
		
	grid .recorder.enableBox -row 0 -column 0 -columnspan 2
	grid .recorder.logsText -row 1 -column 0 -sticky e
	grid .recorder.logsCount -row 1 -column 1 -sticky w
	grid .recorder.periodText -row 2 -column 0 -sticky e
	grid .recorder.periodTime -row 2 -column 1 -sticky w
	grid .recorder.statusText -row 3 -column 0 -sticky e
	grid .recorder.status -row 3 -column 1 -sticky w
	grid .recorder.chooseDir -row 4 -column 0 -columnspan 2
	grid .recorder.currentDir -row 5 -column 0 -columnspan 2
	
	
	set dataRec::recorderEnabled 1
}

proc dataRec::toggleRecording {} {
	
	if $dataRec::dataLogEnable {
		.recorder.enableBox configure -text "Start Logging"
		set dataRec::dataLogEnable 0
		.recorder.periodTime configure -state normal
	} else {
	
		#Check to make sure that timer interval is an integer
		if {![string is integer -strict $dataRec::logPeriod]} {
			tk_messageBox	\
				-message "Invalid Log Period\nPeriod must be integer >= 1"	\
				-type ok
				return
		}
		if {$dataRec::logPeriod < 1} {
			tk_messageBox	\
				-message "Invalid Log Period\nPeriod must be integer >= 1"	\
				-type ok
				return
		}
	
		.recorder.enableBox configure -text "Stop Logging"
		set dataRec::dataLogEnable 1
		set dataRec::numberLogs 0
		.recorder.periodTime configure -state disabled
		set dataRec::logNow 1
	}
	
}

proc dataRec::dataRecord {} {
	
	if {!$dataRec::logNow} {
		return
	}
	
	set dataRec::logNow 0
	
	if $dataRec::dataLogEnable {
	
		#Get date for file (i.e. Mon-Day-Year-HH-MM-SS)
		set now [clock seconds]
		set month [clock format $now -format %b]
		set day [clock format $now -format %d]
		set year [clock format $now -format %Y]
		set hour [clock format $now -format %k]
		set minute [clock format $now -format %M]
		set seconds [clock format $now -format %S]
		
		set filename "$month-$day-$year-$hour-$minute-$seconds.csv"
		
		set fullPath "$dataRec::recordDir/$filename"
		
		#Copied from Export.tcl:
		set aData [lindex $export::exportData 0]
		set bData [lindex $export::exportData 1]
		set stepA [lindex $export::exportData 2]
		set stepB [lindex $export::exportData 3]
		set samplingRate [lindex $export::exportData 4]
		set dataLength 1024
		set timeStep [expr {1/$samplingRate}]
		
		set fileId [open $fullPath w]
		puts $fileId "Time \[s\],Channel A \[V\],Channel B \[V\]"
		set i 0
		for {set i 0} {$i <1024} {incr i} {
			set aValue [lindex $aData $i]
			set bValue [lindex $bData $i]
			puts -nonewline $fileId [expr {$i*$timeStep}]
			puts -nonewline $fileId ","
			puts -nonewline $fileId [scope::convertSample $aValue a]
			puts -nonewline $fileId ","
			puts $fileId [scope::convertSample $bValue b]
		}
		close $fileId
		
		incr dataRec::numberLogs
		
		if $dataRec::recorderEnabled {
			set msPeriod [expr $dataRec::logPeriod*1000]
			after $msPeriod {set dataRec::logNow 1}
		}
	}
	
}
