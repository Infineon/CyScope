#File: netalyzer.tcl
#Syscomp Electronic Design
#Network Analysis Toolbox

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

namespace eval net {

#Frequency Parameters
set startFrequency 3.2
# KEES used to be 20000000
set endFrequency 10000
set startValuePow [expr {log10(3.2)}]
set endValuePow [expr {log10(10e3)}]
set frequencyLogStep 1.1
set frequencyLinStep 100
set frequencyStepMode "log"

#Device Status
set currentFrequency 0
set chAScale "25V Max"
set chBScale "25V Max"
set status "Idle"
set analyzeEnable 0

#Graph Geometry
set plotWidth 500
set plotHeight 300
set topBorder 15
set bottomBorder 15
set leftBorder 30
set rightBorder 30

#Scope Display Geometry
set scopePlotWidth 256
set scopePlotHeight 256

set testFrequencies {}

set sineRef {}
set cosRef {}

set testFrequencies {}
set xValues {}

set xFreq {}
set yMag {}
set yPhase {}
set quality {}

set refFreq {}
set refMag {}
set refPhase {}
set refQuality {}
set referenceEnabled 0

#Constants
set pi [expr {atan(1) * 4}]

set topTick 0
set botTick 0

set maxAmplitude 50

set scientificNotation 1

set phaseOffset 180

}

proc net::toggleOpMode {} {

	if {$::opMode == "Netalyzer"} {
		grid forget [getScopePath]
		grid forget [getWavePath]
		if {[winfo exists .dFloat]} {
			set digio::state docked
			digio::floatDigio
		}
		grid forget [getDigioPath]
		set scope::triggerMode "Single-Shot"
		scope::selectTriggerMode
		#Remove the scope view menu
		grid remove .menubar.scopeView
		net::buildNetalyzer
		
		
	} else {
		set net::analyzeEnable 0
		after 3000 {set net::status "Idle"}
		while {$net::status!="Idle"} {
			update
		}
		destroy .net
		#Find the "Clear plots command in the view menu and remove it"
		#set temp [.menubar.view.viewmenu index "Clear Bode Plots"]
		#.menubar.view.viewmenu delete $temp
		set scope::triggerMode "Auto"
		scope::selectTriggerMode
		arrangeCircuitGear
		scope::adjustVertical a update
		scope::adjustVertical b update
		scope::adjustTimebase update
		destroy .menubar.netView
		grid .menubar.scopeView -row 0 -column 1
	}


}

proc net::buildNetalyzer {} {
	global osType

	frame .net
	
	frame .net.controls
	
	frame .net.controls.inputs	\
		-relief groove	\
		-borderwidth 2
	
	label .net.controls.inputs.startLabel	\
		-text "Start Frequency"	\
		-width 20	\
		-font {-weight bold -size -12}
		
	button .net.controls.inputs.startValue	\
		-textvariable net::startFrequency	\
		-width 10	\
		-command net::setStartFrequency
	
	scale .net.controls.inputs.startFrequency	\
		-length 200		\
		-from 0.51		\
		-to 4.0			\
		-orient horizontal	\
		-command {net::clearPlots;net::updateStartValue}	\
		-variable net::startValuePow	\
		-showvalue 0	\
		-resolution 0.01
		
	label .net.controls.inputs.endLabel	\
		-text "End Frequency"	\
		-width 20	\
		-font {-weight bold -size -12}
	
	button .net.controls.inputs.endValue	\
		-textvariable net::endFrequency	\
		-width 10	\
		-command net::setEndFrequency
		
	scale .net.controls.inputs.endFrequency	\
		-length 200		\
		-from 0.51		\
		-to 4.0			\
		-orient horizontal	\
		-command {net::clearPlots;net::updateEndValue}	\
		-variable net::endValuePow	\
		-showvalue 0	\
		-resolution 0.01
		
	label .net.controls.inputs.stepLabel	\
		-text "Frequency Step"	\
		-width 20	\
		-font {-weight bold -size -12}
		
	button .net.controls.inputs.stepValue	\
		-textvariable net::frequencyLogStep	\
		-width 10	\
		-command net::setFrequencyStep
		
	scale .net.controls.inputs.frequencyStep	\
		-length 200		\
		-orient horizontal	\
		-showvalue 0
	#Set up the scale parameters	
	net::selectStepMode
	
	frame .net.controls.inputs.stepMode
		
	radiobutton .net.controls.inputs.stepMode.log	\
		-text "Logarithmic"	\
		-value "log"	\
		-variable net::frequencyStepMode	\
		-command net::selectStepMode
		
	radiobutton .net.controls.inputs.stepMode.linear	\
		-text "Linear"	\
		-value "linear"	\
		-variable net::frequencyStepMode	\
		-command net::selectStepMode
		
	grid .net.controls.inputs.stepMode.log -row 0 -column 0
	grid .net.controls.inputs.stepMode.linear -row 0 -column 1
		
	label .net.controls.inputs.waveAmpLabel	\
		-text "Max Waveform Amplitude"	\
		-font {-weight bold -size -12}
	
	label .net.controls.inputs.waveAmpValue	\
		-textvariable net::maxAmplitude	\
		-width 10
	
	scale .net.controls.inputs.waveAmp	\
		-from 0	\
		-to 100	\
		-variable net::maxAmplitude	\
		-orient horizontal	\
		-length 200		\
		-showvalue 0	\
		-tickinterval 10	\
		-resolution 1
	
	frame .net.controls.inputs.buttons
		
	button .net.controls.inputs.buttons.start	\
		-text "START"		\
		-width 8			\
		-height 2			\
		-command {set net::analyzeEnable 1; net::analyze}
		
	button .net.controls.inputs.buttons.stop	\
		-text "STOP"		\
		-width 8			\
		-height 2			\
		-command {set net::analyzeEnable 0}
		
	grid .net.controls.inputs.buttons.start -row 0 -column 0 
	grid .net.controls.inputs.buttons.stop -row 0 -column 1 	
		
	grid .net.controls.inputs.startLabel -row 0 -column 0 -sticky w
	grid .net.controls.inputs.startValue -row 0 -column 1
	grid .net.controls.inputs.startFrequency -row 1 -column 0 -columnspan 2
	
	grid .net.controls.inputs.endLabel -row 2 -column 0 -sticky w
	grid .net.controls.inputs.endValue -row 2 -column 1
	grid .net.controls.inputs.endFrequency -row 3 -column 0 -columnspan 2
	
	grid .net.controls.inputs.stepLabel -row 4 -column 0 -sticky w
	grid .net.controls.inputs.stepValue -row 4 -column 1
	grid .net.controls.inputs.frequencyStep -row 5 -column 0 -columnspan 2
	grid .net.controls.inputs.stepMode -row 6 -column 0 -columnspan 2
	
	grid .net.controls.inputs.waveAmpLabel -row 7 -column 0
	grid .net.controls.inputs.waveAmpValue -row 7 -column 1
	grid .net.controls.inputs.waveAmp -row 8 -column 0 -columnspan 2
	
	grid .net.controls.inputs.buttons -row 9 -column 0 -columnspan 2

	
	frame .net.controls.readouts	\
		-relief groove	\
		-borderwidth 2
		
	label .net.controls.readouts.frequencyLabel	\
		-text "Current Frequency:"
	
	label .net.controls.readouts.frequency	\
		-textvariable net::currentFrequency	\
		-width 10	\
		-relief sunken
		
	label .net.controls.readouts.chALabel	\
		-text "Channel A Preamp:"
	
	label .net.controls.readouts.chA	\
		-textvariable net::chAScale	\
		-width 10	\
		-relief sunken
		
	label .net.controls.readouts.chBLabel	\
		-text "Channel B Preamp:"
		
	label .net.controls.readouts.chB	\
		-textvariable net::chBScale	\
		-width 10	\
		-relief sunken
		
	label .net.controls.readouts.ampLabel	\
		-text "Waveform Amplitude (%):"
		
	label .net.controls.readouts.amp	\
		-textvariable wave::amplitude	\
		-width 10	\
		-relief sunken
		
	label .net.controls.readouts.stateLabel	\
		-text "Analyzer Status:"
		
	label .net.controls.readouts.state		\
		-textvariable net::status		\
		-width 10	\
		-relief sunken
		
	grid .net.controls.readouts.frequencyLabel -row 0 -column 0
	grid .net.controls.readouts.frequency -row 0 -column 1
	grid .net.controls.readouts.chALabel -row 1 -column 0
	grid .net.controls.readouts.chA -row 1 -column 1
	grid .net.controls.readouts.chBLabel -row 2 -column 0
	grid .net.controls.readouts.chB -row 2 -column 1
	grid .net.controls.readouts.ampLabel -row 3 -column 0
	grid .net.controls.readouts.amp -row 3 -column 1
	grid .net.controls.readouts.stateLabel -row 4 -column 0
	grid .net.controls.readouts.state -row 4 -column 1
	
	grid .net.controls.inputs -row 0 -column 0 -pady 10
	grid .net.controls.readouts -row 1 -column 0 -pady 10 -ipadx 20 -ipady 5
	
	#Frame for graphs
	frame .net.graphs	\
		-relief groove	\
		-borderwidth 2
	
	#Magnitude Response Canvas
	canvas .net.graphs.mag	\
		-width [expr {$net::plotWidth+$net::leftBorder+$net::rightBorder}]	\
		-height [expr {$net::plotHeight+$net::topBorder+$net::bottomBorder}]	\
		-background white	\
		-borderwidth 2
		
	#Draw Magnitude Y-Axis
	.net.graphs.mag create line	\
		$net::leftBorder [expr {$net::topBorder+$net::plotHeight}]	\
		$net::leftBorder [expr {$net::topBorder}]
	
	#Phase Response Canvas
	canvas .net.graphs.phase	\
		-width [expr {$net::plotWidth+$net::leftBorder+$net::rightBorder}]	\
		-height [expr {$net::plotHeight+$net::topBorder+$net::bottomBorder}]	\
		-background white	\
		-borderwidth 2
		
	#Draw Phase Axes
	.net.graphs.phase create line	\
		$net::leftBorder $net::topBorder	\
		[expr {$net::leftBorder+$net::plotWidth}] $net::topBorder
	.net.graphs.phase create line	\
		$net::leftBorder $net::topBorder	\
		$net::leftBorder [expr {$net::topBorder+$net::plotHeight}]
	net::yScale phase
	
	grid .net.graphs.mag -row 0 -column 0
	grid .net.graphs.phase -row 1 -column 0
	
	#Cursors
	bind .net.graphs.mag <Enter> {.net.graphs.mag configure -cursor crosshair}
	bind .net.graphs.phase <Enter> {.net.graphs.phase configure -cursor crosshair}
	bind .net.graphs.mag <Motion> {net::updateMagCursor %x %y}
	bind .net.graphs.phase <Motion> {net::updatePhaseCursor %x %y}
	bind .net.graphs.mag <Leave> {.net.graphs.mag delete magCursor}
	bind .net.graphs.phase <Leave> {.net.graphs.phase delete phaseCursor}
	
	#Frame for scope display
	frame .net.scope	\
		-relief groove	\
		-borderwidth 2
		
	label .net.scope.title	\
		-text "Oscilloscope Display"	\
		-font {-weight bold -size 10}
		
	canvas .net.scope.display	\
		-width $net::scopePlotWidth	\
		-height  $net::scopePlotHeight	\
		-background white
		
	grid .net.scope.title -row 0 -column 0
	grid .net.scope.display -row 1 -column 0
		
	
	grid .net.controls -row 0 -column 0
	grid .net.scope -row 1 -column 0
	grid .net.graphs -row 0 -column 1 -rowspan 3
	
	grid .net -row 1 -column 0
	
	#View menu for the network analyzer
	#View Menu
	menubutton .menubar.netView \
		-text "View"	\
		-menu .menubar.netView.viewMenu
	menu .menubar.netView.viewMenu -tearoff 0
	if {$osType == "windows"} {
		.menubar.netView.viewMenu add command	\
			-label "Debug Console"	\
			-command {console show}
	}
	#Command to clear bode plots
	.menubar.netView.viewMenu add command	\
		-label "Clear Bode Plots"	\
		-command {net::clearPlots}
	#Command to select scientific notation
	.menubar.netView.viewMenu add check	\
		-label "Scientific Notation"	\
		-variable net::scientificNotation	\
		-command net::drawXScale
	grid .menubar.netView -row 0 -column 1
	#Command to save current plot as reference plot
	.menubar.netView.viewMenu add command	\
		-label "Save Plots as Reference"	\
		-command net::saveReference
	#Command to select reference plot
	.menubar.netView.viewMenu add check	\
		-label "Show Reference Plots"		\
		-variable net::referenceEnabled	\
		-command net::toggleReference
	
	net::drawXScale

	#Pop up window for selecting phase offset
	menu .net.graphs.phase.popup -tearoff 0
	.net.graphs.phase.popup add radiobutton	\
		-label "Phase Axis: \[+360,0\]"	\
		-variable net::phaseOffset	\
		-value 360	\
		-command {net::yScale phase}
	.net.graphs.phase.popup add radiobutton	\
		-label "Phase Axis: \[+270,-90\]"	\
		-variable net::phaseOffset	\
		-value 270	\
		-command {net::yScale phase}
	.net.graphs.phase.popup add radiobutton	\
		-label "Phase Axis: \[+180,-180\]"	\
		-variable net::phaseOffset	\
		-value 180	\
		-command {net::yScale phase}
	.net.graphs.phase.popup add radiobutton	\
		-label "Phase Axis: \[+90,-270\]"	\
		-variable net::phaseOffset	\
		-value 90	\
		-command {net::yScale phase}
	.net.graphs.phase.popup add radiobutton	\
		-label "Phase Axis: \[0, -360\]"	\
		-variable net::phaseOffset	\
		-value 0	\
		-command {net::yScale phase}
	bind .net.graphs.phase <Button-3> {+tk_popup .net.graphs.phase.popup %X %Y}

}

proc net::updateStartValue {dummy} {

	if {$net::startValuePow >= $net::endValuePow} {
		set net::startValuePow [expr {$net::endValuePow-0.1}]
	}
	
	set net::startFrequency [format %.1f [expr {pow(10,$net::startValuePow)}]]

	net::drawXScale

}

proc net::updateEndValue {dummy} {

	if {$net::endValuePow <= $net::startValuePow} {
		set net::endValuePow [expr {$net::startValuePow+0.1}]
	}

	set net::endFrequency [format %.1f [expr {pow(10,$net::endValuePow)}]]
	
	net::drawXScale

}

proc net::drawXScale {} {
	variable testFrequencies

	if {![winfo exists .net.graphs.mag]} {return}

	.net.graphs.mag delete xScaleTag
	.net.graphs.phase delete xScaleTag
	
	if {$net::frequencyStepMode == "log"} {
	
		#Determine which decade the start frequency is in
		set startDecade [expr round(floor($net::startValuePow))]
		
		#Determine which decade the end frequency is in
		set endDecade [expr {floor($net::endValuePow)}]

		#Determine how many decades we are going to span
		set decadeSpan [expr {$endDecade-$startDecade+1}]
		puts "$startDecade $endDecade $decadeSpan"
		
		set decadeWidth [expr {$net::plotWidth/$decadeSpan}]

		for {set decade $startDecade} {$decade <=[expr {$endDecade+1}]} {incr decade} {
			if {$net::scientificNotation} {
				set tickText "1E$decade"
			} else {
				set tickText [expr {round(pow(10,$decade))}]
			}
			.net.graphs.mag create text 	\
				[expr {$net::leftBorder + (log(pow(10,$decade)/pow(10,$startDecade)))/log(10)*$decadeWidth }] [expr {$net::plotHeight+$net::topBorder +10}]	\
				-text $tickText	\
				-font {-size 8}	\
				-tag xScaleTag
			for {set i 1} {$i < 10} {incr i} {
				#x location of tick
				set frequency [expr {$i*pow(10,$decade)}]
				set currentX [expr {$::net::leftBorder+(log($frequency/pow(10,$startDecade)))/log(10)*$decadeWidth}]
				set currentX [expr {round($currentX)}]
				.net.graphs.mag create line \
					$currentX $net::topBorder		\
					$currentX [expr {$net::plotHeight+$net::topBorder + 3}]	\
					-tag xScaleTag
				.net.graphs.phase create line	\
					$currentX $net::topBorder	\
					$currentX [expr {$net::plotHeight+$net::topBorder+3}]	\
					-tag xScaleTag
			}
		}
	
	} else {
	
		#set startFrequency [expr {pow(10,$net::startValuePow)}]
		puts "Start frequency is $net::startFrequency"
		
		#set endFrequency [expr {pow(10,$net::endValuePow)}]
		puts "End frequency is $net::endFrequency"
	
		set j 0
		for {set i $net::startFrequency} {$i <= $net::endFrequency} {set i [expr {$i+($net::endFrequency-$net::startFrequency)/10.0}]} {
		
			puts "i $i"
			set currentX [expr {$net::leftBorder + $net::plotWidth*$j/10.0}]
			
			if {$i > 1E6} {
				set tickText [format "%3.3f" [expr $i/1E6]]
				append tickText "M"
			} elseif { $i > 1E3} {
				set tickText [format "%3.3f" [expr $i/1E3]]
				append tickText "k"
			} else {
				set tickText [format "%3.3f" $i]
			}
			
			.net.graphs.mag create text	\
				$currentX [expr {$net::plotHeight+$net::topBorder + 10}]	\
				-text $tickText	\
				-font {-size 8}	\
				-tag xScaleTag
			
			.net.graphs.mag create line \
  					$currentX $net::topBorder		\
  					$currentX [expr {$net::plotHeight+$net::topBorder + 3}]	\
  					-tag xScaleTag
			.net.graphs.phase create line	\
  					$currentX $net::topBorder	\
  					$currentX [expr {$net::plotHeight+$net::topBorder+3}]	\
  					-tag xScaleTag
					
			incr j
		
		}
	
	
	}
	
	if {$net::referenceEnabled} {
		net::plotRefMag
		net::plotRefPhase
	}
	
}

proc net::analyze {} {
	variable testFrequencies
	
	if {$net::maxAmplitude == 0} {
		tk_messageBox		\
			-title "Amplitude Warning"	\
			-message "Please set the maximum waveform amplitude above 0%"	\
			-type ok	\
			-icon warning
		return
	}
	
	set net::status "Analyzing"
	
	#Set up lists to hold the results of the analysis
	set net::xFreq {}
	set net::yMag {}
	set net::yPhase {}
	set net::quality {}
	
	#Set up for analysis - set preamps to high scale
	set scope::verticalIndexA 6
	set net::chAScale "25V Max"
	scope::adjustVertical a same

	set scope::verticalIndexB 6
	set net::chBScale "25V Max"
	scope::adjustVertical b same
	
	#Make sure we're using a sine wave for analysis
	wave::programWaveform sine.dat
	
	#Set the output of the waveform generator
	set wave::amplitude $net::maxAmplitude
	wave::adjustAmplitude $wave::amplitude
	
	#Set up the frequency list
	set net::testFrequencies {}
	set net::xValues {}
	set temp $net::startFrequency
	
	if {$net::frequencyStepMode == "log"} {
	
		#Determine which decade the start frequency is in
		set startDecade [expr round(floor($net::startValuePow))]
		
		#Determine which decade the end frequency is in
		set endDecade [expr {floor($net::endValuePow)}]

		#Determine how many decades we are going to span
		set decadeSpan [expr {$endDecade-$startDecade+1}]
		puts "$startDecade $endDecade $decadeSpan"
		
		set decadeWidth [expr {$net::plotWidth/$decadeSpan}]
		
		#Add the first point to the lists
		while {$temp <= $net::endFrequency} {
			#Calculate the frequency value
			lappend net::testFrequencies $temp
			#Calculate the corresponding x location for this frequency
			set currentX [expr {$::net::leftBorder+log($temp/(pow(10,floor($net::startValuePow))))/log(10)*$decadeWidth}]
			lappend net::xValues [expr {round($currentX)}]
			#Increment to the next frequency point
			set temp [expr {$temp*$net::frequencyLogStep}]
		}
	} else {
		#Linear Step Mode
		
		#Populate the X-Coordinate and Test Frequency Lists
		for {set i $net::startFrequency} {$i <= $net::endFrequency} {set i [expr {$i+$net::frequencyLinStep}]} {
			#Store the frequency value
			lappend net::testFrequencies $i
			#Calculate the corresponding x-coordinate for this frequency
			set currentX [expr {$net::leftBorder+($i-$net::startFrequency)/($net::endFrequency-$net::startFrequency)*$net::plotWidth}]
			lappend net::xValues $currentX
		}
	}
	
	#Perform analysis at each frequency point in the list
	foreach currentFrequency $testFrequencies {

		puts "=================="
		puts "Testing at $currentFrequency Hz"
		
		#Update the display
		set net::currentFrequency [cursor::formatFrequency $currentFrequency]
		
		#Perform the analysis at this frequency
		set temp [net::analyzeFrequency $currentFrequency]
		
		#Check to see if the user cancelled the analysis
		if {!$net::analyzeEnable} {
			set net::status "Idle"
			return
		}
		
		lappend net::xFreq $currentFrequency
		lappend net::yMag [lindex $temp 0]
		lappend net::yPhase [lindex $temp 1]
		lappend net::quality [lindex $temp 2]
		
		#Dynamically update the display
		net::plotRefMag
		net::plotMag 
		net::plotRefPhase
		net::plotPhase
		
	}
	
	set net::status "Idle"
	set net::analyzeEnable 0
	

}

# KEES found it!
proc net::adjustSampleRate {freq} {

	if {$freq >= 97656} {
		set sampleRate 0
	} elseif {$freq >= 48828} {
		set sampleRate 0
	} elseif {$freq >= 24414} {
		set sampleRate 0
	} elseif {$freq >= 6103} {
		set sampleRate 0
	} elseif {$freq >= 3051} {
		set sampleRate 1
	} elseif {$freq >= 1525} {
		set sampleRate 2
	} elseif { $freq >= 762} {
		set sampleRate 3
	} elseif {$freq >= 381} {
		set sampleRate 4
	} elseif {$freq >= 190} {
		set sampleRate 5
	} elseif {$freq >=95} {
		set sampleRate 6
	} elseif {$freq >=47} {
		set sampleRate 7
	} elseif {$freq >=23} {
		set sampleRate 8
	} elseif {$freq >= 5} {
		set sampleRate 9
	} else {
		set sampleRate 9
	}
	
	return $sampleRate

}

proc net::createSineRef {freq} {

	set net::sineRef {}
	
	set tStep [expr {1.0/[scope::getSampleRate]}]
	
	for {set i 0} {$i < 1024} {incr i} {
		set t [expr {$i*$tStep}]
		set temp [expr {sin(2*$net::pi*$freq*$t)}]
		lappend net::sineRef $temp
	}

}

proc net::createCosRef {freq} {

	set net::cosRef {}
	
	set tStep [expr {1.0/[scope::getSampleRate]}]
	
	for {set i 0} {$i < 1024} {incr i} {
		set t [expr {$i*$tStep}]
		set temp [expr {cos(2*$net::pi*$freq*$t)}]
		lappend net::cosRef $temp
	}

}

proc net::multiplyWaveforms {waveA waveB} {
	
	set product {}
	
	set averageA [calculateAverage $waveA]
	set averageB [calculateAverage $waveB]
		
	foreach pointA $waveA pointB $waveB {
		lappend product [expr {($pointA-$averageA)*($pointB-$averageB)}]
	}
	
	return $product
}

proc net::integrateWaveform {waveform} {
	
	set length [llength $waveform]
	set sum 0
	foreach point $waveform {
		set sum [expr {$sum+$point}]
	}
	return [expr {$sum/$length}]
}

proc net::findMinMax {data} {

	#set average [calculateAverage $data]

	set length [llength $data]
	set length [expr {$length - 1}]
	set maximum 0
	set minimum 1023
	for {set i 0} {$i < $length} {incr i} {
		set datum [lindex $data $i]
		#set datum [expr {$datum - $average}]
		if {$datum > $maximum} {
			set maximum $datum
		}
		if {$datum < $minimum} {
			set minimum $datum
		}
	}
	set returnValues {}
	lappend returnValues $minimum
	lappend returnValues $maximum
	return $returnValues
}

proc net::calculateAverage {data} {
	#Determine the number of samples
	set numSamples [llength $data]
	
	#Determine the average of the samples
	set average 0
	for {set i 0} {$i < $numSamples} {incr i} {
		set average [expr {$average + [lindex $data $i]}]
	}
	set average [expr {$average/$numSamples}]
	return $average
}

proc net::analyzeFrequency { freq } {

	#Set the output frequency of the generator
	wave::sendFrequency $freq
	
	#Make sure we capture 1023 samples after the trigger
	sendCommand "S C 3 255"
	
	#Set the sample rate to match the output frequency
	set scope::timebaseIndex [lsearch $scope::samplingRates [net::adjustSampleRate $freq]]
        #GJL - add range checking for timebaseIndex
	if {$scope::timebaseIndex > 15} {
	    set scope::timebaseIndex 15
	}
	if {$scope::timebaseIndex < 0} {
	    set scope::timebaseIndex 0
	}
	scope::updateScopeControlReg
	
	#Create the reference sine wave for this frequency
	net::createSineRef $freq
	
	#Create the reference cosine wave for this frequency
	net::createCosRef $freq
	
	#Analysis Loop
	set captureOK 0
	set numIterations 0
	while {!$captureOK} {
		update
		
		#Check to see if the user cancelled the analysis
		if {!$net::analyzeEnable} {
			return
		}
		
		#See if we're in an infinite loop
		if {$numIterations > 10} {
			tk_messageBox		\
				-title "Analysis Failure"	\
				-message "Analysis Failed."	\
				-type ok	\
				-icon warning
			set net::analyzeEnable 0
			return
		}
	
	        # KEES change the trigger level to something thats not zero volts, 500 mv = 501
		#Set the trigger level to 0V, we always trigger off channel A
		#The trigger level has to be updated on each pass because the hardware offset
		#values are specific to the range setting of the preamp and this range can change
		#during the recursion.
		if {$scope::verticalIndexA < 4} {
			set offset $scope::offsetA2
		} else {
			set offset $scope::offsetA1
		}
		set triggerLevel [expr {486-$offset}]
		set byteHigh [expr {round(floor($triggerLevel/pow(2,8)))}]
		set byteLow [expr {$triggerLevel%round(pow(2,8))}]
		sendCommand "S T $byteHigh $byteLow"
	
		set captureOK [net::analysisRecursion $freq]
		incr numIterations
	}
	

	
	#Retrieve data from scope and convert it
	set dataA {}
	foreach datum [lindex $scope::scopeData 0] {
		lappend dataA [scope::convertSample $datum a]
	}
	set dataB {}
	foreach datum [lindex $scope::scopeData 1] {
		lappend dataB [scope::convertSample $datum b]
	}
	
	#Components for Channel A
	set eii [net::multiplyWaveforms $dataA $net::sineRef]
	set iPhaseA [net::integrateWaveform $eii]
	
	set eiq [net::multiplyWaveforms $dataA $net::cosRef]
	set qPhaseA [net::integrateWaveform $eiq]
	
	#Calculate magnitude
	set magA [expr {(2.0)*sqrt(pow($iPhaseA,2)+pow($qPhaseA,2))}]
	
	#Calculate phase
	#set radA [expr {atan($qPhaseA/$iPhaseA)}]
	set radA [expr {atan2($qPhaseA,$iPhaseA)}]
	set degA [expr {$radA/$net::pi*180.0}]

	#Components for Channel B
	set eii [net::multiplyWaveforms $dataB $net::sineRef]
	set iPhaseB [net::integrateWaveform $eii]
	
	set eiq [net::multiplyWaveforms $dataB $net::cosRef]
	set qPhaseB [net::integrateWaveform $eiq]

	#Calculate magnitude
	set magB [expr {(2.0)*sqrt(pow($iPhaseB,2)+pow($qPhaseB,2))}]
	
	#Calculate phase
	set radB [expr {atan2($qPhaseB,$iPhaseB)}]
	set degB [expr {$radB/$net::pi*180.0}]

	#Calculate the overall phase response
	set phase [expr {$degB-$degA}]
	if {$phase > $net::phaseOffset} {
		set phase [expr {-360 + $phase} ]
	} elseif { $phase < [expr {$net::phaseOffset-360}]} {
		set phase [expr {360 + $phase}]
	}
	
	set ratio [expr {20*log($magB/$magA)/log(10)}]
	
	#Return the magnitude and phase values
	set returnValues {}
	lappend returnValues $ratio
	lappend returnValues $phase
	lappend returnValues $captureOK
	
}


proc net::analysisRecursion {freq} {

	#Acquire the waveform
	scope::acquireWaveform
	set netTimeout [after 5000 {set export::exportData -1}]
	vwait export::exportData
	
	#Check to see if the acquisition was successful
	if {$export::exportData == -1} {
		if {!$net::analyzeEnable} {return}
		tk_messageBox		\
			-title "Analyzer Error"	\
			-message "Input signal was not detected.\nPlease ensure that the waveform generator and channel A of\nthe oscilloscope are connected to the input of the circuit under test."	\
			-type ok	\
			-icon warning
		set net::analyzeEnable 0
	} else {
		after cancel $netTimeout
	}
	
	
	#Check to make sure that the net analyzer is still enabled
	if {$net::analyzeEnable} {
		net::updateScopeDisplay
	} else {
		return 0
	}
	
	#Get samples from scope
	set dataA [lindex $scope::scopeData 0]
	set dataB [lindex $scope::scopeData 1]
	
	#Calculate Channel A Min/Max
	set temp [net::findMinMax $dataA]
	set minimum [lindex $temp 0]
	set maximum [lindex $temp 1]
	set average [net::calculateAverage $dataA]
	set sigAmplitudeA [expr {$maximum - $minimum}]
	puts "A: min $minimum max $maximum average $average sigAmp $sigAmplitudeA"
	
	#Check to see if we need to change the preamp on channel A
	if {$sigAmplitudeA < 53 && ($scope::verticalIndexA == 6)&&($average > 426 && $average<598)} {
		set scope::verticalIndexA 3
		set net::chAScale "2.5V Max"
		scope::adjustVertical a same
		puts "Adjusting Channel A preamp to 2.5V scale."
		return 0
	} 
	if {($maximum > 1000 || $minimum < 28) && ($scope::verticalIndexA == 3) } {
		set scope::verticalIndexA 6
		set net::chAScale "25V Max"
		scope::adjustVertical a same
		puts "Adjusting Channel A preamp to 25V scale."
		return 0
	} elseif {$maximum > 1000} {
		puts "Input signal is too large!"
		return 0
	}
	
	#Calculate Channel B Min/Max
	set temp [net::findMinMax $dataB]
	set minimum [lindex $temp 0]
	set maximum [lindex $temp 1]
	set average [net::calculateAverage $dataB]
	set sigAmplitudeB [expr {$maximum-$minimum}]
	puts "B: min $minimum max $maximum average $average sigAmp $sigAmplitudeB"
	
	#Check to see if we need to change the preamp on channel B
	if {$sigAmplitudeB < 53 && ($scope::verticalIndexB == 6)&&($average > 426 && $average<598)} {
		set scope::verticalIndexB 3
		set net::chBScale "2.5V Max"
		scope::adjustVertical b same
		puts "Adjusting Channel B preamp to 2.5V scale."
		return 0
	} elseif {$sigAmplitudeB < 53 && $wave::amplitude < $net::maxAmplitude} {
		#Increase the waveform amplitude
		set temp $wave::amplitude
		set temp [expr {round($temp*1.1)}]
		if {$temp > 100} {set temp 100}
		set wave::amplitude $temp
		wave::adjustAmplitude $temp
		puts "Increasing waveform output amplitude to $temp%"
		return 0
	}
	
	if {($maximum > 1000 || $minimum < 28) && ($scope::verticalIndexB == 3)} {
		set scope::verticalIndexB 6
		set net::chBScale "25V Max"
		scope::adjustVertical b same
		return 0
	} elseif {($maximum > 1000 || $minimum < 28)  && $wave::amplitude > 1} {
		#Decrease the waveform amplitude
		set temp $wave::amplitude
		set temp [expr {round($temp*0.9)}]
		if {$temp < 1} { set temp 1}
		set wave::amplitude $temp
		wave::adjustAmplitude $temp
		puts "Decreasing waveform output amplitude to $temp%"
		return 0
	}
	
	#See if we need to flag this reading as questionable
	if {$sigAmplitudeB < 10} {
		return 2
	} else {
		return 1
	}
}


proc net::plotMag {} {

	set y $net::yMag

	set length [llength $y]
	if {$length < 2} {return}
	
	.net.graphs.mag delete magTag

	net::yScale mag
	
	#Determine how many ticks to display
	set ticks [expr {($net::topTick-$net::botTick)/20}]
	set magMajorTick [expr {$net::plotHeight/$ticks}]
	
	for {set index 1} {$index < $length} {incr index} {
	
		set x1 [lindex $net::xValues [expr {$index-1}]]
		set x2 [lindex $net::xValues $index]
		set y1 [lindex $y [expr {$index-1}]]
		set y1 [expr {$net::topBorder+($net::topTick-$y1)/20*($magMajorTick)}]
		set y2 [lindex $y $index]
		set y2 [expr {$net::topBorder+($net::topTick-$y2)/20*($magMajorTick)}]
		
		if { [lindex $net::quality $index] == 1 } {
			.net.graphs.mag create line	\
				$x1 $y1 $x2 $y2	\
				-tag magTag	\
				-fill red	\
				-width 2	
		} else {
			.net.graphs.mag create line	\
				$x1 $y1 $x2 $y2	\
				-tag magTag	\
				-fill red	\
				-width 2	\
				-dash .
		}


	}

}

proc net::plotRefMag {} {

	#Make sure the reference plot is enabled
	if {!$net::referenceEnabled} {return}

	set y $net::refMag

	set length [llength $y]
	if {$length < 2} {return}
	
	.net.graphs.mag delete refMagTag

	net::yScale mag
	
	#Determine how many ticks to display
	set ticks [expr {($net::topTick-$net::botTick)/20}]
	set magMajorTick [expr {$net::plotHeight/$ticks}]
	
	#Determine which decade the start frequency is in
	set startDecade [expr round(floor($net::startValuePow))]
	
	#Determine which decade the end frequency is in
	set endDecade [expr {floor($net::endValuePow)}]

	#Determine how many decades we are going to span
	set decadeSpan [expr {$endDecade-$startDecade+1}]
	
	#Calculate the corresponding x location for this frequency
	set decadeWidth [expr {$net::plotWidth/$decadeSpan}]
	
	for {set index 1} {$index < $length} {incr index} {
	
		#Get the frequency value for this sample
		set temp [lindex $net::refFreq [expr {$index-1}]]
		if {$net::frequencyStepMode == "log"} {
			set currentX [expr {$::net::leftBorder+log($temp/(pow(10,floor($net::startValuePow))))/log(10)*$decadeWidth}]
			set x1 [expr {round($currentX)}]
		} else {
			set x1 [expr {$net::leftBorder+($temp-$net::startFrequency)/($net::endFrequency-$net::startFrequency)*$net::plotWidth}]
		}
		
		#Get the frequency value for this sample
		set temp [lindex $net::refFreq $index]
		#Calculate the corresponding x location for this frequency
		if {$net::frequencyStepMode == "log"} {
			set currentX [expr {$::net::leftBorder+log($temp/(pow(10,floor($net::startValuePow))))/log(10)*$decadeWidth}]
			set x2 [expr {round($currentX)}]
		} else {
			set x2 [expr {$net::leftBorder+($temp-$net::startFrequency)/($net::endFrequency-$net::startFrequency)*$net::plotWidth}]
		}
		
		set y1 [lindex $y [expr {$index-1}]]
		set y1 [expr {$net::topBorder+($net::topTick-$y1)/20*($magMajorTick)}]
		set y2 [lindex $y $index]
		set y2 [expr {$net::topBorder+($net::topTick-$y2)/20*($magMajorTick)}]
		
		if { [lindex $net::refQuality $index] == 1 } {
			.net.graphs.mag create line	\
				$x1 $y1 $x2 $y2	\
				-tag refMagTag	\
				-fill grey	\
				-width 2	
		} else {
			.net.graphs.mag create line	\
				$x1 $y1 $x2 $y2	\
				-tag refMagTag	\
				-fill grey	\
				-width 2	\
				-dash .
		}


	}

}

proc net::plotPhase {} {
	
	#Get the phase values from the analysis variable
	set y $net::yPhase
	
	#Make sure we have at least 2 points to plot
	set length [llength $y]
	if {$length < 2} {return}
	
	#Clear the display
	.net.graphs.phase delete phaseTag
	
	#Plot data points
	set plotData {}
	for {set index 1} {$index < $length} {incr index} {
		set x1 [lindex $net::xValues [expr {$index-1}]]
		set x2 [lindex $net::xValues $index]
		set y1 [lindex $y [expr {$index-1}]]
		set y1 [expr {$net::topBorder + ($net::plotHeight/2.0)- (($y1+180-$net::phaseOffset)/360)*$net::plotHeight}]
		set y2 [lindex $y $index]
		set y2 [expr {$net::topBorder + ($net::plotHeight/2.0)- (($y2+180-$net::phaseOffset)/360)*$net::plotHeight}]
		
		if {[lindex $net::quality $index] == 1} {
			.net.graphs.phase create line	\
				$x1 $y1 $x2 $y2	\
				-tag phaseTag	\
				-fill red	\
				-width 2
		} else {
			.net.graphs.phase create line	\
				$x1 $y1 $x2 $y2	\
				-tag phaseTag	\
				-fill red	\
				-width 2	\
				-dash .
		}

	}
}

proc net::plotRefPhase {} {
	
	#Make sure the reference plot is enabled
	if {!$net::referenceEnabled} {return}
	
	#Get the phase values from the analysis variable
	set y $net::refPhase
	
	#Make sure we have at least 2 points to plot
	set length [llength $y]
	if {$length < 2} {return}
	
	#Clear the display
	.net.graphs.phase delete refPhaseTag
	
	#Determine which decade the start frequency is in
	set startDecade [expr round(floor($net::startValuePow))]
	
	#Determine which decade the end frequency is in
	set endDecade [expr {floor($net::endValuePow)}]

	#Determine how many decades we are going to span
	set decadeSpan [expr {$endDecade-$startDecade+1}]
	
	#Calculate the corresponding x location for this frequency
	set decadeWidth [expr {$net::plotWidth/$decadeSpan}]
	
	#Plot data points
	set plotData {}
	for {set index 1} {$index < $length} {incr index} {
		
		#Get the frequency value for this sample
		set temp [lindex $net::refFreq [expr {$index-1}]]
		if {$net::frequencyStepMode == "log"} {
			set currentX [expr {$::net::leftBorder+log($temp/(pow(10,floor($net::startValuePow))))/log(10)*$decadeWidth}]
			set x1 [expr {round($currentX)}]
		} else {
			set x1 [expr {$net::leftBorder+($temp-$net::startFrequency)/($net::endFrequency-$net::startFrequency)*$net::plotWidth}]
		}
		
		#Get the frequency value for this sample
		set temp [lindex $net::refFreq $index]
		#Calculate the corresponding x location for this frequency
		if {$net::frequencyStepMode == "log"} {
			set currentX [expr {$::net::leftBorder+log($temp/(pow(10,floor($net::startValuePow))))/log(10)*$decadeWidth}]
			set x2 [expr {round($currentX)}]
		} else {
			set x2 [expr {$net::leftBorder+($temp-$net::startFrequency)/($net::endFrequency-$net::startFrequency)*$net::plotWidth}]
		}
		
		set y1 [lindex $y [expr {$index-1}]]
		set y1 [expr {$net::topBorder + ($net::plotHeight/2.0)- (($y1+180-$net::phaseOffset)/360)*$net::plotHeight}]
		set y2 [lindex $y $index]
		set y2 [expr {$net::topBorder + ($net::plotHeight/2.0)- (($y2+180-$net::phaseOffset)/360)*$net::plotHeight}]
		
		if {[lindex $net::refQuality $index] == 1} {
			.net.graphs.phase create line	\
				$x1 $y1 $x2 $y2	\
				-tag refPhaseTag	\
				-fill grey	\
				-width 2
		} else {
			.net.graphs.phase create line	\
				$x1 $y1 $x2 $y2	\
				-tag refPhaseTag	\
				-fill grey	\
				-width 2	\
				-dash .
		}
	}
}

proc net::yScale {graph} {
	variable referenceEnabled

	if {$graph == "mag"} {
	
		#Find the maximum and minimum magnitude values in the current magnitude data
		set minimum 60
		set maximum -60
		foreach mag $net::yMag {
			if {$mag > $maximum} {
				set maximum $mag
			}
			if {$mag < $minimum} {
				set minimum $mag
			}
		}
		
		#Check the reference magnitude data too, if it is enabled
		if {$net::referenceEnabled} {
			foreach refMag $net::refMag {
				if {$refMag > $maximum} {
					set maximum $refMag
				}
				if {$refMag < $minimum} {
					set minimum $refMag
				}
			}
		}
		
    # KEES added a few more options here to give it a bit more resolution
    #Determine the largest tick mark
    if {$maximum < 0.0} {
			set topTick 0.0
    } elseif {$maximum < 5.0} {
			set topTick 5.0
    } elseif {$maximum < 10.0} {
			set topTick 10.0
    } elseif {$maximum < 15.0} {
			set topTick 15.0
		} elseif {$maximum < 20.0} {
			set topTick 20.0
    } elseif {$maximum < 25.0} {
			set topTick 25.0
    } elseif {$maximum < 30.0} {
			set topTick 30.0
    } elseif {$maximum < 35.0} {
			set topTick 35.0
		} elseif {$maximum < 40.0} {
			set topTick 40.0
    } elseif {$maximum < 45.0} {
			set topTick 45.0
    } elseif {$maximum < 50.0} {
			set topTick 50.0
    } elseif {$maximum < 55.0} {
			set topTick 55.0
		} else {
			set topTick 60.0
		}
		
		#Determine the smallest tick mark
		if {$minimum > 0.0} {
			set botTick 0.0
    } elseif {$minimum > -5.0} {
			set botTick -5.0
    } elseif {$minimum > -10.0} {
			set botTick -10.0
    } elseif {$minimum > -15.0} {
			set botTick -15.0
		} elseif {$minimum > -20.0} {
			set botTick -20.0
    } elseif {$minimum > -25.0} {
			set botTick -25.0
    } elseif {$minimum > -30.0} {
			set botTick -30.0
    } elseif {$minimum > -35.0} {
			set botTick -35.0
		} elseif {$minimum > -40.0} {
			set botTick -40.0
    } elseif {$minimum > -45.0} {
			set botTick -45.0
    } elseif {$minimum > -50.0} {
			set botTick -50.0
    } elseif {$minimum > -55.0} {
			set botTick -55.0
		} else {
			set botTick -60.0
		}
		#Determine the largest tick mark
		# if {$maximum < 0} {
			# set topTick 0
		# } elseif {$maximum < 20} {
			# set topTick 20
		# } elseif {$maximum < 40} {
			# set topTick 40
		# } else {
			# set topTick 60
		# }
		
		#Determine the smallest tick mark
		# if {$minimum > 0} {
			# set botTick 0
		# } elseif {$minimum > -20} {
			# set botTick -20
		# } elseif {$minimum > -40} {
			# set botTick -40
		# } else {
			# set botTick -60
		# }
		
		set net::topTick $topTick
		set net::botTick $botTick
		
		#Determine how many ticks to display
    # KEES fucking around, used to be 20, trying 5.0 (added .0 to deal with rounding problem)
		set ticks [expr {($topTick-$botTick)/5.0}]
		set magMajorTick [expr {$net::plotHeight/$ticks}]
		
		#Draw the scale
		.net.graphs.mag delete magScale
		set tickValue $topTick
		for {set i 0} {$i <= $ticks} {incr i} {
			set y [expr {$net::topBorder +$i*$magMajorTick}]
      # KEES had to change this to 5 too
			set tickValue [expr {$topTick - 5*$i}]
			.net.graphs.mag create text	\
				[expr {$net::leftBorder - 15}] $y	\
				-text $tickValue	\
				-font {-size 8}	\
				-tag magScale
			.net.graphs.mag create line	\
				$net::leftBorder $y	\
				[expr {$net::leftBorder+$net::plotWidth}] $y	\
				-tag magScale
		}
	} elseif {$graph == "phase"} {

		#Delete any previous markings
		.net.graphs.phase delete phaseScale

		#Split the display into 90 degree increments, KEES changed from 4 to 8.0 added .0 to deal with display rounding problem
		set phaseMajorTick [expr {$net::plotHeight/8.0}]
		
		#Draw the horizontal ticks
    # KEES changed this from 5 to 9, changed on line 1376 from 90 to 45 too
		for {set i 0} {$i< 9} {incr i} {
			set y [expr {$net::topBorder + $i*$phaseMajorTick}]
			.net.graphs.phase create text	\
				[expr {$net::leftBorder - 15}] $y	\
				-text [expr {$net::phaseOffset-45*$i}] 	\
				-font {-size 8}	\
				-tag phaseScale
			.net.graphs.phase create line	\
				$net::leftBorder $y	\
				[expr {$net::leftBorder+$net::plotWidth}] $y	\
				-tag phaseScale
		}
	}

}

proc net::clearPlots {} {
	
	.net.graphs.mag delete magTag
	.net.graphs.phase delete phaseTag

}

proc net::updateScopeDisplay {} {

	set dataA [lindex $scope::scopeData 0]
	set dataB [lindex $scope::scopeData 1]
	
	set averageA [calculateAverage $dataA]
	set averageB [calculateAverage $dataB]

	.net.scope.display delete netWaveA
	.net.scope.display delete netWaveB
	
	#Plot channel A waveform
	set plotData {}
	for {set i 0} {$i < 1024} {incr i} {
		#lappend plotData [expr {$i/(1024/$net::scopePlotWidth)}]
		lappend plotData $i
		set datum [lindex $dataA $i]
		set datum [expr {$datum-$averageA}]
		lappend plotData [expr {$net::scopePlotHeight/2+$datum/(1024/$net::scopePlotHeight)}]
	}
	.net.scope.display create line	\
		$plotData	\
		-tag netWaveA	\
		-fill red
		
	#Plot channel B waveform
	set plotData {}
	for {set i 0} {$i < 1024} {incr i} {
		#lappend plotData [expr {$i/(1024/$net::scopePlotWidth)}]
		lappend plotData $i
		set datum [lindex $dataB $i]
		set datum [expr {$datum-$averageB}]
		lappend plotData [expr {$net::scopePlotHeight/2+$datum/(1024/$net::scopePlotHeight)}]
	}
	.net.scope.display create line	\
		$plotData	\
		-tag netWaveB	\
		-fill blue
	

}

#Save current network data as refence plot
proc net::saveReference {} {

	set net::refMag $net::yMag
	set net::refPhase $net::yPhase
	set net::refFreq $net::xFreq
	set net::refQuality $net::quality

}

proc net::toggleReference {} {

	if {$net::referenceEnabled} {
		net::plotRefMag
		net::plotRefPhase
	} else {
		.net.graphs.mag delete refMagTag
		.net.graphs.phase delete refPhaseTag
	}

}

proc net::setStartFrequency {} {

	#Dialog box for new frequency
	set newFreq [Dialog_Prompt newFstart "Set Start Frequency:"]
	
	if {$newFreq == ""} {return}
	
	#Make sure that we got a valid frequency setting
	if { [string is double -strict $newFreq] } {
		if { $newFreq >= 1 && $newFreq <= $net::endFrequency} {
			set net::startValuePow [expr {log10($newFreq)}]
			set net::startFrequency [format %.1f $newFreq]
			net::clearPlots
			net::updateStartValue $net::startValuePow
		} else {
			tk_messageBox	\
			-title "Invalid Frequency"	\
			-default ok		\
			-message "Frequency out of range.  Start frequency must be greater than 1Hz\nand less than the end frequency."	\
			-type ok			\
			-icon warning
		}
	} else {
		tk_messageBox	\
			-title "Invalid Frequency"	\
			-default ok		\
			-message "Frequency must be a number\nbetween 1 Hz and 2 MHz."	\
			-type ok			\
			-icon warning
	}
	

}

proc net::setEndFrequency {} {

	#Dialog box for new frequency
	set newFreq [Dialog_Prompt newFend "Set End Frequency:"]
	
	if {$newFreq == ""} {return}
	
	#Make sure that we got a valid frequency setting
	if { [string is double -strict $newFreq] } {
		if { $newFreq >= $net::startFrequency && $newFreq <= 10000} {
			set net::endValuePow [expr {log10($newFreq)}]
			set net::endFrequency [format %.1f $newFreq]
			net::clearPlots
			net::updateEndValue $net::endValuePow
		} else {
			tk_messageBox	\
			-title "Invalid Frequency"	\
			-default ok		\
			-message "Frequency out of range.  Start frequency must be less than end frequency\nand greater than 3.2 Hz."	\
			-type ok			\
			-icon warning
		}
	} else {
		tk_messageBox	\
			-title "Invalid Frequency"	\
			-default ok		\
			-message "Frequency must be a number\nbetween 1 Hz and 2 MHz."	\
			-type ok			\
			-icon warning
	}
	

}

proc net::selectStepMode {} {

	if {$net::frequencyStepMode == "log"} {
		.net.controls.inputs.frequencyStep	configure	\
			-from 1.01		\
			-to 1.25		\
			-resolution 0.01	\
			-variable net::frequencyLogStep
			
		.net.controls.inputs.stepValue configure	\
			-textvariable net::frequencyLogStep
	
	} else {
		.net.controls.inputs.frequencyStep	configure	\
			-from 0.1		\
			-to 100000		\
			-resolution 0.1	\
			-variable net::frequencyLinStep
			
		.net.controls.inputs.stepValue configure	\
			-textvariable net::frequencyLinStep
	}
	
	net::drawXScale

}

proc net::setFrequencyStep {} {

	#Dialog box for new frequency
	set newStep [Dialog_Prompt newFstep "Set Frequency Step:"]
	
	if {$newStep == ""} {return}
	
	#Make sure that we got a valid frequency setting
	if { [string is double -strict $newStep] } {
		if {$net::frequencyStepMode == "log"} {
			if {$newStep >= 1.01 && $newStep <= 1.25} {
				set net::frequencyLogStep $newStep
			} else {
				tk_messageBox	\
					-title "Invalid Frequency"	\
					-default ok		\
					-message "Frequency out of range.  Logarithmic frequency step must be between 1.01 and 1.25"	\
					-type ok			\
					-icon warning
			}
		} else {
			if {$newStep >= 0.1 && $newStep <= 100000} {
				set net::frequencyLinStep $newStep
			} else {
				tk_messageBox	\
					-title "Invalid Frequency"	\
					-default ok		\
					-message "Frequency out of range.  Linear frequency step must be between 0.1 and 100kHz"	\
					-type ok			\
					-icon warning
			}
		}
	} else {
		if {$net::frequencyStepMode == "log"} {
			tk_messageBox	\
				-title "Invalid Frequency"	\
				-default ok		\
				-message "Logarithmic frequency step must be a number\nbetween 1.01 and 1.25"	\
				-type ok			\
				-icon warning
		} else {
			tk_messageBox	\
				-title "Invalid Frequency"	\
				-default ok		\
				-message "Linear frequency step must be a number\nbetween 0.1 and 100000"	\
				-type ok			\
				-icon warning
		}
	}
	

}

proc net::updateMagCursor {xCoord yCoord} {

	#puts "$xCoord,$yCoord"
	.net.graphs.mag delete magCursor
	
	if {$xCoord < $net::leftBorder} {return}
	
	#Calculate the frequency coordinate
	if {$net::frequencyStepMode=="log"} {
		#Determine which decade the start frequency is in
		set startDecade [expr round(floor($net::startValuePow))]
		
		#Determine which decade the end frequency is in
		set endDecade [expr {floor($net::endValuePow)}]

		#Determine how many decades we are going to span
		set decadeSpan [expr {$endDecade-$startDecade+1}]
		
		set decadeWidth [expr {$net::plotWidth/$decadeSpan}]
		
		set currentDecade [expr {$startDecade+($xCoord-$net::leftBorder)/$decadeWidth}]
		#puts "Current Decade $currentDecade"
		
		set f [expr {pow(10,$currentDecade)}]
		#puts "Frequency $f"
	} else {
		
		set f [expr {$net::startFrequency+1.0*($xCoord-$net::leftBorder)/$net::plotWidth*($net::endFrequency-$net::startFrequency)}]
		puts "f $f"
	}

	if {$f > 1E6} {
		set freqText [format "%3.3f" [expr $f/1E6]]
		append freqText "M"
	} elseif { $f > 1E3} {
		set freqText [format "%3.3f" [expr $f/1E3]]
		append freqText "k"
	} else {
		set freqText [format "%3.3f" $f]
	}
	append freqText "Hz"

	#Determine which side of the cursor to position the reading
	if {$xCoord < [expr {$net::plotWidth/2.0}]} {
		set anchorPos "w"
		set anchorX [expr {$xCoord+5}]
	} else {
		set anchorPos "e"
		set anchorX [expr {$xCoord-5}]
	}

	.net.graphs.mag create text 	\
		$anchorX [expr {$yCoord-5}]	\
		-text $freqText	\
		-font {-size -15 -weight bold}	\
		-fill red	\
		-anchor $anchorPos	\
		-tag magCursor
		
	if {($net::topTick == 0) && ($net::botTick == 0)} {
		return
	}
	
	#Determine the amplitude reading
	
	set amp [expr {$net::topTick+1.0*($yCoord-$net::topBorder)/($net::plotHeight)*($net::botTick-$net::topTick)}]
	
	set ampText [format %.3f $amp]
	append ampText "dB"
	
	.net.graphs.mag create text	\
		$anchorX [expr {$yCoord+10}]	\
		-text $ampText	\
		-font {-size -15 -weight bold}	\
		-fill red	\
		-anchor $anchorPos	\
		-tag magCursor

}

proc net::updatePhaseCursor {xCoord yCoord} {

	.net.graphs.phase delete phaseCursor
	
	if {$xCoord < $net::leftBorder} {return}
	
	#Calculate the frequency coordinate
	if {$net::frequencyStepMode=="log"} {
		#Determine which decade the start frequency is in
		set startDecade [expr round(floor($net::startValuePow))]
		
		#Determine which decade the end frequency is in
		set endDecade [expr {floor($net::endValuePow)}]

		#Determine how many decades we are going to span
		set decadeSpan [expr {$endDecade-$startDecade+1}]
		
		set decadeWidth [expr {$net::plotWidth/$decadeSpan}]
		
		set currentDecade [expr {$startDecade+($xCoord-$net::leftBorder)/$decadeWidth}]
		
		set f [expr {pow(10,$currentDecade)}]
	} else {
		
		set f [expr {$net::startFrequency+1.0*($xCoord-$net::leftBorder)/$net::plotWidth*($net::endFrequency-$net::startFrequency)}]
		puts "f $f"
	}

	if {$f > 1E6} {
		set freqText [format "%3.3f" [expr $f/1E6]]
		append freqText "M"
	} elseif { $f > 1E3} {
		set freqText [format "%3.3f" [expr $f/1E3]]
		append freqText "k"
	} else {
		set freqText [format "%3.3f" $f]
	}
	append freqText "Hz"

	#Determine which side of the cursor to position the reading
	if {$xCoord < [expr {$net::plotWidth/2.0}]} {
		set anchorPos "w"
		set anchorX [expr {$xCoord+5}]
	} else {
		set anchorPos "e"
		set anchorX [expr {$xCoord-5}]
	}

	.net.graphs.phase create text 	\
		$anchorX [expr {$yCoord-5}]	\
		-text $freqText	\
		-font {-size -15 -weight bold}	\
		-fill red	\
		-anchor $anchorPos	\
		-tag phaseCursor
	
	#Determine the phase reading
	set phase [expr {$net::phaseOffset+(-360.0)*($yCoord-$net::topBorder)/($net::plotHeight)}]
	
	set phaseText [format %.1f $phase]
	append phaseText "deg"
	
	.net.graphs.phase create text	\
		$anchorX [expr {$yCoord+10}]	\
		-text $phaseText	\
		-font {-size -15 -weight bold}	\
		-fill red	\
		-anchor $anchorPos	\
		-tag phaseCursor

}