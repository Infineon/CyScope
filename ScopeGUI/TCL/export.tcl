#File: export.tcl
#Syscomp Electronic Design
#Waveform Export Procedures

#JG
#Copyright 2008 Syscomp Electronic Design
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

.menubar.tools.toolsMenu add command	\
	-label "Export Waveform (CSV)..."	\
	-command export::exportCSV
.menubar.tools.toolsMenu add separator

namespace eval export {

set exportData {}

}

proc export::exportCSV {} {
	variable exportData
	
	if {$exportData == {}} {
	
		tk_messageBox	\
			-message "No Data to Export"	\
			-default ok	\
			-icon warning	\
			-title "Export Warning"	\
			-type ok
			
		return
	}
	
	if {$::opMode=="CircuitGear"} {
		set aData [lindex $exportData 0]
		set bData [lindex $exportData 1]
		set stepA [lindex $exportData 2]
		set stepB [lindex $exportData 3]
		set samplingRate [lindex $exportData 4]
	
		set dataLength 1024
	
		#Calculate the time step per x data point
		set timeStep [expr {1/$samplingRate}]
	} elseif {$::opMode=="Netalyzer"} {
		set freqData $net::xFreq
		set magData $net::yMag
		set phaseData $net::yPhase
		set qualityData $net::quality
	} else {
		return
	}
	
	set types {
		{{CSV Files} {.csv}}
	}
	
	set dataFile [tk_getSaveFile	\
		-filetypes $types	\
		-initialfile "CGR101.csv"	\
		-defaultextension .csv]
	
	if {$dataFile ==""} {return}
	
	if {[catch {open $dataFile w} fileId]} {
		tk_messageBox	\
			-message "Unable to Open File for Writing"	\
			-type ok
		return
	}
	
	if {$::opMode=="CircuitGear"} {
		puts $fileId "Time \[s\],Channel A \[V\],Channel B \[V\]"
		set i 0
		foreach aValue $aData bValue $bData {
			puts -nonewline $fileId [expr {$i*$timeStep}]
			puts -nonewline $fileId ","
			puts -nonewline $fileId [scope::convertSample $aValue a]
			puts -nonewline $fileId ","
			puts $fileId [scope::convertSample $bValue b]
			incr i
		}
	} elseif {$::opMode=="Netalyzer"} {
		puts $fileId "Frequency \[Hz\],Magnitude \[dB\],Phase \[deg\],Quality"
		foreach freqValue $freqData magValue $magData phaseValue $phaseData qualityValue $qualityData {
			puts $fileId "$freqValue,$magValue,$phaseValue,$qualityValue"
		}
	}
	
	close $fileId

}