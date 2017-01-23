#File: calibration.tcl
#Syscomp USB Oscilloscope GUI
#Scope Display Persistence Package

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


package provide persist 1.0

namespace eval persist {

	variable chA [list]
	variable chB [list]
	variable levelsOfPersistence Off

}

proc persist::updatePersist {plotDataA plotDataB} {

	if {$persist::levelsOfPersistence == "Off"} {
		set scopePath [getScopePath]
		$scopePath.display delete persistA
		$scopePath.display delete persistB
		return
	}

	if {$persist::levelsOfPersistence == "infinite"} {
		set persist::chA [list $plotDataA]
		set persist::chB [list $plotDataB]
	} else {
		if { [llength $persist::chA] >= $persist::levelsOfPersistence } {
			set persist::chA [lreplace $persist::chA 0 0]
		}
		set persist::chA [linsert $persist::chA end $plotDataA]
		
		if { [llength $persist::chB] >= $persist::levelsOfPersistence } {
			set persist::chB [lreplace $persist::chB 0 0]
		}
		set persist::chB [linsert $persist::chB end $plotDataB]
	}
	
	persist::plotPersist
}

proc persist::plotPersist {} {

	set scopePath [getScopePath]

	if {$persist::levelsOfPersistence != "infinite"} {
		$scopePath.display delete persistA
		$scopePath.display delete persistB
	}
	
	if {$scope::enableA} {
		set i 15
		foreach oldWaveform $persist::chA {
			set hexColor [format %x $i]
			if {$persist::levelsOfPersistence == "infinite"} {
				set fillColor $scope::waveColorA
			} else {
				set fillColor "#FF$hexColor$hexColor$hexColor$hexColor"
			}
			$scopePath.display create line	\
				$oldWaveform	\
				-tag persistA	\
				-fill $fillColor
			if {$persist::levelsOfPersistence != "infinite"} {
				set i [expr {$i-16/$persist::levelsOfPersistence}]
			}
		}
	}

	if {$scope::enableB} {
		set i 15
		foreach oldWaveform $persist::chB {
			set hexColor [format %x $i]
			if {$persist::levelsOfPersistence == "infinite"} {
				set traceColor $scope::waveColorB
			} else {
				set traceColor "#00" 
				append traceColor $hexColor $hexColor "FF"
			}
			
			$scopePath.display create line	\
				$oldWaveform	\
				-tag persistB	\
				-fill $traceColor
			if {$persist::levelsOfPersistence != "infinite"} {
				set i [expr {$i-16/$persist::levelsOfPersistence}]
			}
		}
	}

}

proc persist::changeLevels {} {

	set scopePath [getScopePath]
	$scopePath.display delete persistA
	$scopePath.display delete persistB
	set persist::chA {}
	set persist::chB {}

}

.menubar.scopeView.viewMenu add separator

menu .menubar.scopeView.persist -tearoff 0
.menubar.scopeView.persist add check	\
	-label "Off"	\
	-variable persist::levelsOfPersistence	\
	-onvalue "Off"	\
	-command persist::changeLevels
.menubar.scopeView.persist add check	\
	-label "4"	\
	-variable persist::levelsOfPersistence	\
	-onvalue "4"	\
	-command persist::changeLevels
.menubar.scopeView.persist add check	\
	-label "8"	\
	-variable persist::levelsOfPersistence	\
	-onvalue "8"	\
	-command persist::changeLevels
.menubar.scopeView.persist add check	\
	-label "16"	\
	-variable persist::levelsOfPersistence	\
	-onvalue "16"	\
	-command persist::changeLevels
.menubar.scopeView.persist add check	\
	-label "Infinite"	\
	-variable persist::levelsOfPersistence	\
	-onvalue "infinite"	\
	-command persist::changeLevels

.menubar.scopeView.viewMenu add cascade	\
	-menu .menubar.scopeView.persist		\
	-label "Digital Persistence"




