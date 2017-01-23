#File: FFT.tcl
#Syscomp Electronic Design
#Spectrum Analysis Toolbox

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

#The fourier math package is the basis for spectrum analysis
package require math::fourier 1.0.1

#Add a menubar item to the main window to access the FFT
.menubar.tools.toolsMenu add command	\
	-label "Spectrum Analysis"	\
	-command fft::createFFT
.menubar.tools.toolsMenu add separator

namespace eval fft {

#FFT Display Geometry
set fftCanvasSize 500
set fftXBorder 40
set fftXLength [expr {$fftCanvasSize-2*$fftXBorder}]
set fftYBorder 20
set fftYLength [expr {$fftCanvasSize-2*$fftYBorder }]
set linearMax 5

#FFT Interface Control Variables
set fftScale log
set fftWindow rectangular
set fftSource a
set fftEnable 0

set pi 3.1415926535897931


}

#Create Spectrum Analysis Window
#----------------
#This procedure creates the spectrum analysis window, initializes all widgets,
#and performs the geometry placement.  It also sets the fftEnable global variable
#which signals that the FFT analysis is running.
proc fft::createFFT {} {
	variable fftEnable
	variable fftCanvasSize
	variable fftXBorder
	variable fftXLength
	variable fftYBorder
	variable fftYLength
	variable freqPos

	#Check to see if the FFT windows is already running, if so, raise it
	if {[winfo exists .fft]} {
		raise .fft
		return
	}

	#Create the FFT Window
	toplevel .fft
	wm resizable .fft 0 0
	wm title .fft "Frequency Spectrum"
	
	label .fft.title	\
	-text "Frequency Spectrum Analysis"	\
	-font {-weight bold -size 14}
	
	#Vertical Scale Controls
	frame .fft.scale	\
		-relief groove	\
		-borderwidth 2

	label .fft.scale.title	\
		-text "Vertical Scale"	\
		-font {-weight bold -size 10}

	radiobutton .fft.scale.linear	\
		-text "Linear"	\
		-value "linear"	\
		-variable fft::fftScale
		
	button .fft.scale.linearMax	\
		-text "Set Max Scale"	\
		-command fft::setMaxScale

	radiobutton .fft.scale.log	\
		-text "Logarithmic"	\
		-value "log"			\
		-variable fft::fftScale
		
	grid .fft.scale.title -row 0 -column 0 -sticky we
	grid .fft.scale.linear -row 1 -column 0 -sticky w
	grid .fft.scale.linearMax -row 2 -column 0 -sticky w
	grid .fft.scale.log -row 3 -column 0 -sticky w

	#FFT Windowing Controls
	frame .fft.window	\
		-relief groove	\
		-borderwidth 2
		
	label .fft.window.title	\
		-text "FFT Window"	\
		-font {-weight bold -size 10}

	radiobutton .fft.window.rectangular	\
		-text "Rectangular"	\
		-value rectangular	\
		-variable fft::fftWindow
		
	radiobutton .fft.window.hamming	\
		-text "Hamming"	\
		-value hamming	\
		-variable fft::fftWindow
		
	radiobutton .fft.window.hann	\
		-text "Hann"	\
		-value hann	\
		-variable fft::fftWindow
		
	radiobutton .fft.window.triangular	\
		-text "Triangular"	\
		-value triangular	\
		-variable fft::fftWindow
		
	grid .fft.window.title -row 0 -column 0 -columnspan 2 -sticky we
	grid .fft.window.rectangular -row 1 -column 0 -sticky w
	grid .fft.window.hamming -row 2 -column 0 -sticky w
	grid .fft.window.hann -row 3 -column 0 -sticky w
	grid .fft.window.triangular -row 4 -column 0 -sticky w
	

	#FFT Data Source Controls
	frame .fft.source	\
		-relief groove	\
		-borderwidth 2
		
	label .fft.source.title	\
		-text "FFT Source"	\
		-font {-weight bold -size 10}
		
	radiobutton .fft.source.a	\
		-text "Channel A"		\
		-value a			\
		-variable fft::fftSource
		
	radiobutton .fft.source.b	\
		-text "Channel B"		\
		-value b			\
		-variable fft::fftSource
		
	grid .fft.source.title -row 0 -column 0 -sticky we
	grid .fft.source.a -row 1 -column 0 -sticky we
	grid .fft.source.b -row 2 -column 0 -sticky we
	
	#Sampling Information
	frame .fft.sample	\
		-relief groove	\
		-borderwidth 2
		
	label .fft.sample.rateLabel	\
		-text "Sampling Rate"
		
	label .fft.sample.rate	\
		-relief sunken	\
		-textvariable fft::fftSampleRate	\
		-width 15
		
	label .fft.sample.numLabel	\
		-text "Number of Samples"
	
	label .fft.sample.numSamples	\
		-relief sunken	\
		-textvariable fft::fftSamples	\
		-width 15
		
	grid .fft.sample.rateLabel -row 0 -column 0
	grid .fft.sample.rate -row 1 -column 0
	grid .fft.sample.numLabel -row 2 -column 0
	grid .fft.sample.numSamples -row 3 -column 0
	
	#Button to break out the controls

	
	#FFT Display
	canvas .fft.display	\
	-width $fftCanvasSize		\
	-height $fftCanvasSize	\
	-background white

	#Draw the Axes
	.fft.display create line	\
		$fftXBorder $fftYBorder $fftXBorder [expr {$fftYBorder + $fftYLength}]	\
		-fill black
	.fft.display create line	\
		$fftXBorder [expr {$fftYBorder+$fftYLength}] [expr {$fftXBorder+$fftXLength}] [expr {$fftYBorder+$fftYLength}]	\
		-fill black
		
	#Draw the frequency cursor
	.fft.display create line	\
		[expr {$fftXBorder+$fftXLength/2}] $fftYBorder	\
		[expr {$fftXBorder+$fftXLength/2}] [expr {$fftYBorder+$fftYLength}]	\
		-fill green	\
		-tag freqCursor
	.fft.display create polygon	\
		[expr {$fftXBorder+$fftXLength/2-5}] $fftYBorder	\
		[expr {$fftXBorder+$fftXLength/2+5}] $fftYBorder	\
		[expr {$fftXBorder+$fftXLength/2}] [expr {$fftYBorder+7}]	\
		[expr {$fftXBorder+$fftXLength/2-3}] $fftYBorder	\
		-fill green	\
		-tag freqCursor
	
	#Initialize Frequency Cursor Position
	set freqPos [expr {$fftXBorder+$fftXLength/2}]
	.fft.display bind freqCursor <Button-1> {fft::markFreqStart freqCursor %x}
	.fft.display bind freqCursor <B1-Motion> {fft::moveFreqCursor %x}
	
	#Left and right arrows move cursor by one pixel
	bind .fft <KeyPress-Left> {set fft::freqStart $fft::freqPos; fft::moveFreqCursor [expr {$fft::freqPos-1}]}
	bind .fft <KeyPress-Right> {set fft::freqStart $fft::freqPos; fft::moveFreqCursor [expr {$fft::freqPos+1}]}
		
	#Arrange the FFT user interface
	grid .fft.title -row 0 -column 0 -columnspan 2 -sticky w
	grid .fft.display -row 1 -column 1 -rowspan 4
	grid .fft.scale -row 1 -column 0 -sticky we
	grid .fft.source -row 2 -column 0 -sticky we
	grid .fft.window -row 3 -column 0 -sticky we
	grid .fft.sample -row 4 -column 0 -sticky we -padx 4

	bind .fft <Destroy> {set fft::fftEnable 0}
	set fftEnable 1

}

#Update Spectrum Display
#----------------
#This procedure is called each time there is new data available from the scope.  This
#is where we perform the actual FFT number crunching and plot the spectrum on the
#display.
proc fft::updateFFT {} {
	variable fftYLength
	variable fftXLength
	variable fftXBorder
	variable fftYBorder
	variable fftCanvasSize
	variable fftScale
	variable fftSource
	variable fftSamples
	variable fftSampleRate
	variable fft
	
	if {!$fft::fftEnable} {return}
	
	#Determine which channel data we will be processing
	if {$fftSource == "a" } {
		set fftData [lindex $export::exportData 0]
		set stepSize [lindex $export::exportData 2]
	} else {
		set fftData [lindex $export::exportData 1]
		set stepSize [lindex $export::exportData 3]
	}
	
	#Convert the data into voltage values
	set norm {}
	foreach datum $fftData {
		lappend norm [expr (512-$datum)*$stepSize]
	}
	
	#Determine the number of samples
	set normLength [llength $norm]
	
	#Windowing
	switch $fft::fftWindow {
		"hamming" {
			set n 0
			for {set n 0} {$n <= [expr {$normLength-1}]} {incr n} {
				set datum [lindex $norm $n]
				set norm [lreplace $norm $n $n [expr { (0.54-0.46*cos(2*$fft::pi*$n/($normLength-1)))*$datum}]]
			}
		} "hann" {
			set n 0
			for {set n 0} {$n <= [expr {$normLength-1}]} {incr n} {
				set datum [lindex $norm $n]
				set norm [lreplace $norm $n $n [expr { (0.5*(1-cos(2*$fft::pi*$n/($normLength-1))))*$datum}]]
			}
		} "triangular" {
			set n 0
			for {set n 0} {$n <= [expr {$normLength-1}]} {incr n} {
				set datum [lindex $norm $n]
				set norm [lreplace $norm $n $n [expr {(2.0/($normLength-1))*(($normLength-1)/2.0-abs($n-($normLength-1)/2.0))*$datum}]]
			}
		}
	}
	
	#Determine the length of the FFT to the next higher power of 2
	set powTwo [expr {log10($normLength)/log10(2)}]
	set powTwo [expr {ceil($powTwo)}]
	set fftLength [expr {pow(2,$powTwo)}]
	set fftSamples $fftLength
	
	#Pad the data with zeros to ensure that we have a record length
	#that is a power of 2
	while {[llength $norm] < $fftLength} {lappend norm 0}
	
	#Calculate the effective sampling rate by dividing the hardware sampling rate
	#by the burstIncrement.  The burstIncrement is used to subsample the data.
	set fftSampleRate [lindex $export::exportData 4]
	
	#Format the effective sampling rate for display purposes
	if {$fftSampleRate >=1E6} {
		set fftSampleRate [expr {$fftSampleRate/1.0E6}]
		set fftSampleRate [format "%.1f" $fftSampleRate]
		set fftSampleRate "$fftSampleRate MS/s"
	} elseif {$fftSampleRate >=1E3} {
		set fftSampleRate [expr {$fftSampleRate/1.0E3}]
		set fftSampleRate [format "%.1f" $fftSampleRate]
		set fftSampleRate "$fftSampleRate kS/s"
	} else {
		set fftSampleRate [format "%.1f" $fftSampleRate]
		set fftSampleRate "$fftSampleRate S/s"
	}
	
	#Crunch the numbers: FFT analysis
	set fft [::math::fourier::dft $norm]
	
	#Extract the spectrum data from the FFT data
	set spectrum {}
	set halfLength [expr {([llength $fft]/2)+1}]
	for {set index 0} {$index < $halfLength} {incr index} {
		#Read the complex number list from the FFT data
		set complex [lindex $fft $index]
		#Seperate the complex number into the real and complex parts
		set r [lindex $complex 0]
		set i [lindex $complex 1]
		#Compute the power sprectrum
		set temp [expr {sqrt(($r*$r)+($i*$i))}]
		#Except for the DC value, we double the result
		if { $index > 0} {
			set temp [expr {$temp*2.0}]
		}
		lappend spectrum $temp
	}
	
	#Linear Scaling
	set spectrumLinear {}
	foreach element $spectrum {
		lappend spectrumLinear [expr {$element/$fftLength}]
	}

	#Logarithmic Scaling
	set spectrumLog {}
	if {$fftScale == "log"} {
		#The A/D Step Size is used as the 0dB reference
		set ref $stepSize
		#Compute the logarithmically scaled spectrum
		foreach element $spectrumLinear {
			if {$element < $stepSize} {
				#Make sure that we don't take the log of zero and discard
				#numbers from the FFT analysis that are smaller than the A/D
				#step size.
				set element $stepSize
			} 
			#Compute the power spectrum
			lappend spectrumLog [expr {20.0*log10($element/$ref)}]
		}
	}
	
	#Scale the data for the screen and create an array for plotting
	set plotData {}
	if {$fftScale == "linear"} {
		#set linMax [expr {ceil(512.0*$stepSize)}]
		set linMax $fft::linearMax
		foreach datum $spectrumLinear {
			lappend plotData [expr {$fftYBorder+$fftYLength-($datum/$linMax*$fftYLength)}]
		}
	} else {
		#The maximum dynamic range is 60 dB (20*log(1024)), so the log scale span is 60 dB
		foreach datum $spectrumLog {
			lappend plotData [expr {$fftYBorder+$fftYLength-$fftYLength*($datum/60.0)}]
		}
	}
	
	#Scale the spectrum to take up the full X-Axis, the x-axis labelling is taken
	#care of by the cursor routines.
	set xLength [llength $plotData]
	set xLength [expr {$xLength*1.0}]
	set drawSpectrum {}
	for {set index 0} {$index < $xLength} {incr index} {
		#Calculate the relative position of this sample on the x-axis
    # KEES logarithmic frequency
		set x [expr {((1.0+log10(($index+1)/$xLength)/2.75)*$fftXLength)+$fftXBorder}]
		#Pull the corresponding power spectrum value from the list
		set point [lindex $plotData $index]
		#Save this x,y pair for plotting
		lappend drawSpectrum $x $point
	}
	
	#Spectrum colors should match the coloring of the traces in the main window.
	if {$fftSource == "a"} {
		set spectrumColor red
	} else {
		set spectrumColor blue
	}
	
	#Plot the FFT data on the spectrum display
	.fft.display delete fftPlot
	.fft.display create line	\
		$drawSpectrum	\
		-fill $spectrumColor	\
		-tag fftPlot
		
	#Show the reference for the logarithmic scale
	if {$fftScale == "log"} {
		.fft.display delete fftRef
		set refText "0dB = $ref V"
		.fft.display create text	\
			[expr {$fftXBorder+$fftXLength/2.0}] [expr {$fftYBorder/2.0}]	\
			-fill black	\
			-tag fftRef	\
			-text $refText
	} else {
		#Linear display, remove the log reference
		.fft.display delete fftRef
	}
	
	#Update the Y-Axis
	if {$fftScale == "linear"} {
		fftLinearScale
	} else {
		fftLogScale
	}
	
	#Copy power spectrum data to fft global variable for cursor processing
	if {$fftScale =="linear"} {
		set fft $spectrumLinear
	} else {
		set fft $spectrumLog
	}
	
	#Update the cursors since the values may have changed since the cursor was moved last
	updateFFTCursor
	
}

#Draw Linear Y Axis Markings
#----------------
#This procedure is called to annotate the y-axis with linear tick marks.  The maximum value is set by the user.
# The intermediate tick marks are then calculated automatically.
proc fft::fftLinearScale {} {
	variable fftSource
	variable fftYLength
	variable fftYBorder
	variable fftXBorder
	variable fftXLength
	variable linearMax

	#Determine the data source and read out the A/D step size
	if {$fftSource == "a" } {
		set stepConst [lindex $export::exportData 2]
	} else {
		set stepConst [lindex $export::exportData 3]
	}
	
	#Determine the maximum value that can be measured at this vertical sensitivity
	#set maxValue [expr {ceil(512.0*$stepConst)}]
	set maxValue $linearMax
	
	#Update the Y-Axis Tick Marks
	.fft.display delete fftYAxis
	for {set i 0} {$i <= 10} {incr i} {
		#set y [expr {$fftYBorder+$fftYLength-($fftYLength*$i/10.0)}]
		set y [expr {$fftYBorder+$fftYLength*($i/10.0)}]
		set label [expr {(10-$i)*($maxValue/10.0)}]
		set label [format "%.3f" $label]
		.fft.display create text	\
			[expr {$fftXBorder-($fftXBorder/2.0)}] $y	\
			-text $label	\
			-fill black	\
			-tag fftYAxis
		.fft.display create line	\
			[expr {$fftXBorder-2}] $y	\
			[expr {$fftXBorder+2}] $y	\
			-fill black	\
			-tag fftYAxis
		.fft.display create line	\
			$fftXBorder $y	\
			[expr $fftXBorder+$fftXLength] $y	\
			-fill grey	\
			-tag fftYAxis	\
			-dash .
	}
	
	#Units for the Y axis
	.fft.display create text \
		[expr {$fftXBorder-($fftXBorder/2.0)}] [expr {$fftYBorder/2.0}]	\
		-text {[V]}	\
		-fill black	\
		-tag fftYAxis
}

#Draw Logarithmic Y Axis Markings
#----------------
#This procedure is called to annotate the y-axis with logarithmic tick marks.  The maximum value fixed at
#60 dB due to the 10-bit vertical sensitivity of the scope.
proc fft::fftLogScale {} {
	variable fftSource
	variable fftYLength
	variable fftYBorder
	variable fftXBorder
	variable fftXLength
	
	#Draw tick marks every 10dB along the y-axis
	.fft.display delete fftYAxis
	for {set i 0} {$i <= 60} {set i [expr {$i+10}]} {
		set y [expr {$fftYBorder+$fftYLength-$fftYLength*($i/60.0)}]
		.fft.display create text	\
			[expr {$fftXBorder-($fftXBorder/2.0)}] $y	\
			-text $i	\
			-fill black	\
			-tag fftYAxis
		.fft.display create line	\
			[expr {$fftXBorder-2}] $y	\
			[expr {$fftXBorder+2}] $y	\
			-fill black	\
			-tag fftYAxis
		.fft.display create line	\
			$fftXBorder $y	\
			[expr $fftXBorder+$fftXLength] $y	\
			-fill grey	\
			-tag fftYAxis	\
			-dash .
	}

	#Units for the Y axis
	.fft.display create text \
		[expr {$fftXBorder-($fftXBorder/2.0)}] [expr {$fftYBorder/2.0}]	\
		-text {[dB]}	\
		-fill black	\
		-tag fftYAxis
	
}

#Mark Frequency Cursor Start Position
#-------------
#The procedure is used by the display when measurement cursor is moved.
#The procedure marks the starting point where the user initially
#grabs the cursor.
proc fft::markFreqStart {cursorTag xPos} {
	variable freqPos
	variable freqStart
	
	#Snapping in case the user grabbed the handle
	set snap [expr {$xPos - $freqPos}]
	set freqPos $xPos
	.fft.display move $cursorTag $snap 0
	#Mark this start postion
	set freqStart $xPos
}

#Move Frequency Cursor
#-------------------
#This procdure is called when the user drags the frequency cursor along the screen.  It updates the position
#of the cursor on the screen and calls the updateFFTCursor routine which updates the measurement readings.
proc fft::moveFreqCursor {xPos} {
	variable freqPos
	variable freqStart
	variable fftXBorder
	variable fftXLength
		
	#Make sure we don't go off the screen
	if { $xPos <= $fftXBorder } { set xPos [expr {$fftXBorder+1}] }
	if { $xPos > [expr {$fftXBorder + $fftXLength}] } {
		set xPos [expr {$fftXBorder + $fftXLength}]
	}
	#Move the cursor on the screen
	set dx [expr {$xPos - $freqStart}]
	set freqStart $xPos
	set freqPos $xPos
	.fft.display move freqCursor $dx 0
	
	fft::updateFFTCursor
}

#Update Frequency Cursor Readings
#--------------------------------------
#This routine is called to update the frequency position reading and the amplitude reading of the frequency
#cursor.  The routine reads the amplitude data from the FFT global array and displays it on the screen.  It
#also calculates and displays the frequency position of the cursor.
proc fft::updateFFTCursor {} {
	variable freqPos
	variable fftXBorder
	variable fftXLength
	variable fftYBorder
	variable fftYLength
	variable fft
	variable fftScale
	variable fftSource
	variable linearMax

		
	#Read out the A/D step size for the selected channel.
	if {$fftSource == "a" } {
		set stepSize [lindex $export::exportData 2]
	} else {
		set stepSize [lindex $export::exportData 3]
	}
		
	#Make sure that the FFT window is still open (in case somebody closes it in the middle of the update)
	if {![info exists fft]} {return}

	#Determine the horizontal position of the cursor
	set xPos [expr {1.0*($freqPos-$fftXBorder)/$fftXLength}]
	
	#Use the horizontal position to read out the corresponding FFT amplitude data
	set fftLength [llength $fft]
  # KEES trying to get the cursor to show the right data
	set fftIndex [expr {round(pow(10.0, 2.75*($xPos-1))*$fftLength)-1}]
	set fftValue [lindex $fft $fftIndex]
	#Format for 3 decimal places
	set fftValue [format "%.3f" $fftValue]
	
	#Draw the amplitude cross-hair indicator
	if {$fftScale == "linear"} {
		#set linMax [expr {ceil(512.0*$stepSize)}]
		set linMax $::fft::linearMax
		set h [expr {round($fftYLength+$fftYBorder-($fftValue/$linMax*$fftYLength))}]
	} else {
		set h [expr {$fftYBorder+$fftYLength-$fftYLength*($fftValue/60.0)}]
	}
	.fft.display delete fLevel
	.fft.display create line \
		[expr {$freqPos-3}] $h	\
		[expr {$freqPos+3}] $h	\
		-tag fLevel	\
		-fill green
	
	#Label the cross hair with the amplitude value
	.fft.display delete fValue
	if {$freqPos>[expr {0.75*($fftXLength+$fftXBorder)}]} {
		set textX [expr {$freqPos - 28}]
	} else {
		set textX [expr {$freqPos + 28}]
	}
	.fft.display create text	\
		$textX [expr {$h-7}]	\
		-fill green	\
		-text $fftValue	\
		-tag fValue
		
	#Calculate and display the frequency value
	set sRate [lindex $export::exportData 4]
	set fStep [expr {$sRate/(2*($fftLength-1))}]
	set fX [expr {$fStep*$fftIndex}]
	#Format the frequency rate for display purposes
	if {$fX >=1E6} {
		set fX [expr {$fX/1.0E6}]
		set fX [format "%.1f" $fX]
		set fX "$fX MHz"
	} elseif {$fX >=1E3} {
		set fX [expr {$fX/1.0E3}]
		set fX [format "%.1f" $fX]
		set fX "$fX kHz"
	} else {
		set fX [format "%.1f" $fX]
		set fX "$fX Hz"
	}

	.fft.display delete fText
	.fft.display create text	\
		$freqPos [expr {$fftYBorder+$fftYLength+7}]	\
		-fill black	\
		-text $fX	\
		-tag fText
}

proc fft::setMaxScale {} {
	variable linearMax

	set newMax [Dialog_Prompt linMax "New Linear Scale Maximum:"]
	
	if {$newMax == ""} {return}
	
	if {[string is double -strict $newMax]} {
		if {($newMax > 0) && ($newMax <=27)} {
			set linearMax $newMax
		} elseif {$newMax > 27} {
			tk_messageBox	\
				-title "Invalid Scale Setting"	\
				-default ok	\
				-message "Maximum scale value is 27.0V."	\
				-type ok	\
				-icon warning	\
				-parent .fft
			set linearMax 27.0
		}
	} else {
		tk_messageBox	\
			-title "Invalid Scale Setting"	\
			-default ok	\
			-message "Scale maximum must be between 0V and 27V"	\
			-type ok	\
			-icon warning	\
			-parent .fft
	}
	
	fft::fftLinearScale


}
