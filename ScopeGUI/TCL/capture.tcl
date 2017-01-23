#File: capture.tcl
#Syscomp USB Instruments

#JG
#Copyright 2006 Syscomp Electronic Design
#www.syscompdesign.com

#Based on the code presented by David Easton on March 17, 2003
#on http://wiki.tcl.tk/9127

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

package provide capture 1.0

# GJL - add path to get freewrap to work
lappend auto_path ./Lib/twapi-4.1.27
package require twapi

namespace eval capture {

	set captureMode jpg

}

proc capture::captureWindow { win} {
	
	regexp {([0-9]*)x([0-9]*)\+([0-9]*)\+([0-9]*)} [winfo geometry $win] - w h x y
	
	foreach otherWindow [wm stackorder .] {
		lower $otherWindow
	}
	
	#set image [image create photo -format window -data $win]
	wm withdraw $win
	wm deiconify $win
	update
	set snap [image create photo]
	$snap put $win
	
	foreach child [winfo children $win] {
		#Make sure that the child is part of the toplevel that we
		#are trying to capture
		if {[winfo toplevel $child] == $win} {
			capture::captureWindowSub $child $snap 0 0
		}
	}
	return $snap
}

proc capture::captureWindowSub { win image px py} {
	
	if {![winfo ismapped $win]} {
		return
	}
	
	regexp {([0-9]*)x([0-9]*)\+([0-9]*)\+([0-9]*)} [winfo geometry $win] - w h x y
	
	incr px $x
	incr py $y
	
	#set tempImage [image create photo -format window -data $win]
	set tempImage [image create photo]
	$tempImage put $win
	
	$image copy $tempImage -to $px $py
	image delete $tempImage
	
	foreach child [winfo children $win] {
		capture::captureWindowSub $child $image $px $py
	}
}

proc capture::windowToFile {win} {

	
	if {$::osType == "windows"} {
		set img [msWindowsCapture $win]
	} else {
		set img [capture::captureWindow $win]
	}
	
	set name [wm title $win]
	if {$capture::captureMode == "jpg"} {
		set types {{"Image Files" {.jpg}}}
		set initFile "$name.jpg"
		set ext ".jpg"
	} else {
		set types {{"Image Files" {.png}}}
		set initFile "$name.png"
		set ext ".png"
	}
		
	set filename [tk_getSaveFile	\
		-filetypes $types	\
		-initialfile $initFile	\
		-defaultextension $ext
		]

	if {[llength $filename]} {
		if {$capture::captureMode == "jpg"} {
			$img write -format jpeg $filename
		} else {
			$img write -format png $filename
		}
		puts "Written to file: $filename"
	} else {
		puts "Write cancelled"
	}
	image delete $img
}

proc capture::showCapture {} {

	if {![winfo exists .capture]} {
		
		toplevel .capture
		wm title .capture "Screen Capture"
		wm iconname .capture "Capture"
		wm resizable .capture 0 0
		
		frame .capture.instructions	\
			-relief groove	\
			-borderwidth 2
		label .capture.instructions.title	\
			-text "Steps to Successfully Export an Image"	\
			-font {-weight bold -size 10}
		label .capture.instructions.step1	\
			-text "1. Minimize other windows"
		label .capture.instructions.step2	\
			-text "2. Ensure that no windows are overlapping"
		
		frame .capture.windows	\
			-relief groove	\
			-borderwidth 2
		label .capture.windows.title	\
			-text "Select windows to Export"	\
			-font {-weight bold -size 10}
		grid .capture.windows.title	\
			-row 0 -column 0 -sticky w
			
		#Get a list of the active windows
		set activeWindows [wm stackorder .]
		
		#Create checkbuttons for each of the active windos
		set i 0
		foreach w $activeWindows {
			set name [wm title $w]
			button .capture.windows.captureCheck$i	\
				-text $name	\
				-command "capture::windowToFile [lindex $activeWindows $i]"
			grid .capture.windows.captureCheck$i -row [expr {$i+1}] -column 0 -sticky we
			incr i
		}
		grid .capture.windows -row 0 -column 0
	} else {
		#Get rid of the old capture window and create a new one with
		#an updated active window list
		destroy .capture
		capture::showCapture
	}
	#Make sure we bring the capture window to the front
	wm deiconify .capture
	raise .capture
}

proc msWindowsCapture {win} {

	puts "Raising window"
	raise $win
	wm deiconify $win
	update
	puts "Focusing window"
	focus $win
	update
	puts "Sending print screen"
	update
	#twapi::block_input
	#::twapi::send_keys %{PRTSC}
	#set id [twapi::register_hotkey alt-PRTSC {puts "Got alt-prtscr"; update}]
	#twapi::send_keys %{PRTSC}
	twapi::send_keys %({PRTSC})
	#twapi::unregister_hotkey $id
	#twapi::unblock_input
	puts "Print screen done"
	focus $win
	update
	#twapi::open_clipboard
	
	#set retVal [catch {twapi::read_clipboard 8} clipData]
	
	#twapi::close_clipboard
	
	#set im [image create photo -format BMP -data $clipData]
	
	puts "Getting clipboard image"
	set im [Clipboard2Img]
	puts "Done"
	update
	
	return $im

}

 proc Clipboard2Img {} {
     twapi::open_clipboard

     # Assume clipboard content is in format 8 (CF_DIB)
     set retVal [catch {twapi::read_clipboard 8} clipData]
     if { $retVal != 0 } {
         error "Invalid or no content in clipboard"
     }

     # First parse the bitmap data to collect header information
     binary scan $clipData "iiissiiiiii" \
            size width height planes bitcount compression sizeimage \
            xpelspermeter ypelspermeter clrused clrimportant

     # We only handle BITMAPINFOHEADER right now (size must be 40)
     if {$size != 40} {
         error "Unsupported bitmap format. Header size=$size"
     }

     # We need to figure out the offset to the actual bitmap data
     # from the start of the file header. For this we need to know the
     # size of the color table which directly follows the BITMAPINFOHEADER
     if {$bitcount == 0} {
         error "Unsupported format: implicit JPEG or PNG"
     } elseif {$bitcount == 1} {
         set color_table_size 2
     } elseif {$bitcount == 4} {
         # TBD - Not sure if this is the size or the max size
         set color_table_size 16
     } elseif {$bitcount == 8} {
         # TBD - Not sure if this is the size or the max size
         set color_table_size 256
     } elseif {$bitcount == 16 || $bitcount == 32} {
         if {$compression == 0} {
             # BI_RGB
             set color_table_size $clrused
         } elseif {$compression == 3} {
             # BI_BITFIELDS
             set color_table_size 3
         } else {
             error "Unsupported compression type '$compression' for bitcount value $bitcount"
         }
     } elseif {$bitcount == 24} {
         set color_table_size $clrused
     } else {
         error "Unsupported value '$bitcount' in bitmap bitcount field"
     }

     set phImg [image create photo]
     set filehdr_size 14                 ; # sizeof(BITMAPFILEHEADER)
     set bitmap_file_offset [expr {$filehdr_size+$size+($color_table_size*4)}]
     set filehdr [binary format "a2 i x2 x2 i" \
                  "BM" [expr {$filehdr_size + [string length $clipData]}] \
                  $bitmap_file_offset]

     append filehdr $clipData
     $phImg put $filehdr -format bmp

     twapi::close_clipboard
     return $phImg
 }
