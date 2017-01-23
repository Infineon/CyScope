#File: dialog.tcl
#Dialog Box Procedures

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


#Dialog Procedures
#-------------------
#These procedures are used to creating dialog boxes which prompt
#the user to enter values.  Reference:
#"Practical Programming in Tcl and Tk"
#by Brent B. Welch
#Prentice Hall PTR, 2000, New Jersey
#pg 519
proc Dialog_Create { top title args} {
	global dialog
	if [winfo exists $top] {
		switch -- [wm state $top] {
			normal {
				#Raise the buried window
				raise $top
			}
			withdrawn -
			iconic {
				#Open and restore geometry
				wm deiconify $top
				catch { wm geometry $top $dialog(geo,$top)}
			}
		}
		return 0
	} else {
		eval {toplevel $top} $args
		wm title $top $title
		return 1
	}
}
proc Dialog_Wait { top varName {focus {}}} {
	upvar $varName var
	
	#Poke the variable if the user nukes the window
	bind $top <Destroy> [list set $varName 0]
	
	#Grab focus for the dialog
	if {[string length $focus] == 0} {
		set focus $top
	}
	set old [focus -displayof $top]
	focus $focus
	catch {tkwait visibility $top}
	catch {grab $top}
	
	#Wait for the dialog to complete
	tkwait variable $varName
	catch {grab release $top}
	#focus $old
}
proc Dialog_Dismiss {top} {
	global dialog
	#Save the current size and position
	catch {
		#window may have been deleted
		set dialog(geo,$top) [wm geometry $top]
		wm withdraw $top
	}
}
proc Dialog_Prompt { promptId promptString } {
	global prompt
	set f .$promptId
	
	set prompt(result) ""
	
	if [Dialog_Create $f "Prompt" -borderwidth 10] {
		message $f.msg -text $promptString -aspect 1000
		entry $f.entry -textvariable prompt(result)
		set b [frame $f.buttons]
		pack $f.msg $f.entry $f.buttons -side top -fill x
		pack $f.entry -pady 5
		button $b.ok -text OK -command {set prompt(ok) 1}
		button $b.cancel -text Cancel \
			-command {set prompt(ok) 0}
		pack $b.ok -side left
		pack $b.cancel -side right
		bind $f.entry <Return> {set prompt(ok) 1 ; break}
	}
	set prompt(ok) 0
	Dialog_Wait $f prompt(ok) $f.entry
	Dialog_Dismiss $f
	if {$prompt(ok)} {
		return $prompt(result)
	} else {
		return {}
	}
}
