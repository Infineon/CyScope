#File: updateCheck.tcl
#Syscomp Electronic Design Ltd.
#www.syscompdesign.com
#JG
#Copyright 2009 Syscomp Electronic Design

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

package provide updateCheck 1.0
package require http

namespace eval updateCheck  {

set revisionURL "http://www.syscompdesign.com/CGR101.txt"
set updateResult 0

# GJL: Turn off checking for updates
set updateCheckEnable 0

}

proc updateCheck::updateAvailable {} {

	if {[catch {set currentRev [http::data [http::geturl $updateCheck::revisionURL]]}]} {
		puts "Unable to check for updates.  No http connection"
		return ""
	} else {
		if {[string length $currentRev]>6} {
			puts "Current revision string too long: [string length $currentRev]"
			return 0
		}
		if {$currentRev != $::softwareVersion} {
			puts "Update available.  Current version $::softwareVersion.  Available $currentRev"
			return $currentRev
		} else {
			puts "Software up to date.  Current version $::softwareVersion.  Available $currentRev"
			return 0
		}
	}
}

proc updateCheck::updateCheck {inform} {
	variable updateResult

	set updateResult [updateCheck::updateAvailable]

	if {$updateResult==""} {
		puts "Unable to read software revision from web server."
		if {$inform} {
			tk_messageBox	\
				-default ok	\
				-icon info	\
				-message "Unable to check for updates.\nCheck your internet connection."	\
				-parent .	\
				-title "Connection Error"	\
				-type ok
		}
		return
	}

	if {$updateResult!=0} {
		tk_messageBox	\
			-default ok	\
			-icon info	\
			-message "A software update is available.\nPlease visit www.syscompdesign.com\nto download the latest software.\nCurrent Software Version: $::softwareVersion\nAvailable Software Version: $updateCheck::updateResult"	\
			-parent .	\
			-title "Update Available"	\
			-type ok
	} else {
		puts "Software is up to date"
		if {$inform} {
			tk_messageBox	\
				-default ok	\
				-icon info	\
				-message "No updates available.\n"	\
				-parent .	\
				-title "Update Available"	\
				-type ok
		}
	}

}

proc updateCheck::readSettings {} {

	#See if there are preferences saved
	if [catch {open update.cfg r} fileId] {
		puts "No update preferences found"
	} else {
		if {[gets $fileId line] >= 0} {
			puts "Update preference is $line"
			set updateCheck::updateCheckEnable $line
		} else {
			puts "Error reading update preference setting from file."
		}
	}

}

proc updateCheck::saveSettings {} {

	set fileId [open update.cfg w]
	puts $fileId $updateCheck::updateCheckEnable
	close $fileId

}

.menubar.help.helpMenu add command	\
	-label "Check for update..."	\
	-command {updateCheck::updateCheck 1}
	
.menubar.help.helpMenu add check	\
	-label "Check for updates on startup"	\
	-variable updateCheck::updateCheckEnable	\
	-command updateCheck::saveSettings

updateCheck::readSettings

if {$updateCheck::updateCheckEnable} {
	updateCheck::updateCheck 0
}



