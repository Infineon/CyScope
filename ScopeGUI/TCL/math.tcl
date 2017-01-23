#File: math.tcl
#Syscomp CircuitGear
#Waveform Math Toolbox

#JG
#Copyright 2009 Syscomp Electronic Design
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
	-label "Math Toolbox"	\
	-command {math::showMath}
.menubar.tools.toolsMenu add separator

namespace eval math {

set mathEnabled 0
set mathMode add

set mathHeader [image create photo -file "$images/Math.gif"]
set mathImage [image create photo -file "$images/m1V.gif"]

set verticalBoxM 1.0
set verticalIndexM 7

set mathImages [image create photo -file "$images/m5mV.gif"]
lappend mathImages [image create photo -file "$images/m10mV.gif"]
lappend mathImages [image create photo -file "$images/m20mV.gif"]
lappend mathImages [image create photo -file "$images/m50mV.gif"]
lappend mathImages [image create photo -file "$images/m100mV.gif"]
lappend mathImages [image create photo -file "$images/m200mV.gif"]
lappend mathImages [image create photo -file "$images/m500mV.gif"]
lappend mathImages [image create photo -file "$images/m1V.gif"]
lappend mathImages [image create photo -file "$images/m2V.gif"]
lappend mathImages [image create photo -file "$images/m5V.gif"]
lappend mathImages [image create photo -file "$images/m10V.gif"]
lappend mathImages [image create photo -file "$images/m20V.gif"]
lappend mathImages [image create photo -file "$images/m50V.gif"]

}

proc math::showMath {} {
	
	if $math::mathEnabled {wm deiconify .math; raise .math; focus .math; return}
	
	toplevel .math
	wm title .math "Math Toolbox"
	wm resizable .math 0 0
	bind .math <Destroy> {math::hideMath}
	
	label .math.title -image $math::mathHeader
	
	frame .math.mode	\
		-relief groove	\
		-borderwidth 2
	
	radiobutton .math.mode.add		\
		-text "Add: A + B"	\
		-variable math::mathMode	\
		-value add

	radiobutton .math.mode.subtract		\
		-text "Subtract: A - B"		\
		-variable math::mathMode	\
		-value subtract
		
	radiobutton .math.mode.multiply	\
		-text "Multiply: A * B"		\
		-variable math::mathMode		\
		-value multiply

	grid .math.mode.add -row 0 -column 0
	grid .math.mode.subtract -row 1 -column 0
	grid .math.mode.multiply -row 2 -column 0
	
	frame .math.mag	\
		-relief groove	\
		-borderwidth 2
		
	button .math.mag.zoomOut	\
		-image $scope::zoomOutImage	\
		-command {math::adjustMath out}
		
	button .math.mag.zoomIn	\
		-image $scope::zoomInImage	\
		-command {math::adjustMath in}
		
	label .math.mag.sensitivity -image $math::mathImage
	
	grid .math.mag.zoomOut -row 0 -column 0
	grid .math.mag.sensitivity -row 0 -column 1
	grid .math.mag.zoomIn -row 0 -column 2
	
	grid .math.title -row 0 -column 0
	grid .math.mode -row 1 -column 0
	grid .math.mag -row 2 -column 0
	
	#grid .measurements.auto.mathLabel -row 1 -column 3
	#grid .measurements.auto.freqMath -row 2 -column 3
	#grid .measurements.auto.periodMath -row 3 -column 3
	#grid .measurements.auto.averageMath -row 4 -column 3
	#grid .measurements.auto.maxMath -row 5 -column 3
	#grid .measurements.auto.minMath -row 6 -column 3
	#grid .measurements.auto.pkPkMath -row 7 -column 3
	#grid .measurements.auto.rmsMath -row 8 -column 3
	
	set math::mathEnabled 1
}

proc math::hideMath {} {
	
	#Get the path to the scope widgets
	set scopePath [getScopePath]
	
	$scopePath.display delete math
	
	#grid remove .measurements.auto.mathLabel
	#grid remove .measurements.auto.freqMath
	#grid remove .measurements.auto.periodMath
	#grid remove .measurements.auto.averageMath
	#grid remove .measurements.auto.maxMath
	#grid remove .measurements.auto.minMath
	#grid remove .measurements.auto.pkPkMath
	#grid remove .measurements.auto.rmsMath
	
	set math::mathEnabled 0
}

proc math::updateMath {} {

	if {!$math::mathEnabled} {
		return
	}

	#Get the path to the scope widgets
	set scopePath [getScopePath]

	$scopePath.display delete math
	
	set dataA [lindex $export::exportData 0]
	set dataB [lindex $export::exportData 1]
	set stepA [lindex $export::exportData 2]
	set stepB [lindex $export::exportData 3]
	
	set mathData {}
	
	set voltageA {}
	set voltageB {}
	
	#Compute the real life voltage for the A/D readings
	foreach datumA $dataA datumB $dataB {
		lappend voltageA [scope::convertSample $datumA A]
		lappend voltageB [scope::convertSample $datumB B]
	}
	
	switch $math::mathMode {
		"add" {
			foreach datumA $voltageA datumB $voltageB {
				lappend mathData [expr {$datumA + $datumB}]
			}
		} "subtract" {
			foreach datumA $voltageA datumB $voltageB {
				lappend mathData [expr {$datumA - $datumB}]
			}
		} "multiply" {
			foreach datumA $voltageA datumB $voltageB {
				lappend mathData [expr {$datumA * $datumB}]
			}
		}
	}
	
	#Get the timebase setting from our list of settings
	set timebaseSetting [lindex $scope::timebaseValues $scope::timebaseIndex]
	
	#Calculate the Sampling Frequency
	set sampleRate [lindex $scope::samplingRates $scope::timebaseIndex]
	set sampleRate [expr {$scope::masterSampleRate/pow(2,$sampleRate)}]

	#Determine the first sample that should appear on the screen
	set firstSample [expr {1024-5-$cursor::sampleOffset-($cursor::timePos-$scope::xPlotStart)*($timebaseSetting*10*$sampleRate)/$scope::xPlotWidth}]
	set firstSample [expr {round(ceil($firstSample))}]
	#Determine the last sample that should be drawn on the screen
	set lastSample [expr {1024-5-$cursor::sampleOffset-($cursor::timePos-$scope::xPlotEnd)*($timebaseSetting*10*$sampleRate)/$scope::xPlotWidth}]
	set lastSample [expr {round(floor($lastSample))}]
	
	#Calculate the first data point which should be on the left border
	set x1 [expr {$cursor::timePos-($scope::xPlotWidth/($timebaseSetting*10))*((1024-$cursor::sampleOffset-5-($firstSample-1))/$sampleRate)}]
	set x2 [expr {$cursor::timePos-($scope::xPlotWidth/($timebaseSetting*10))*((1024-$cursor::sampleOffset-5-$firstSample)/$sampleRate)}]
	
	#Calculate the left border point
	set temp [lindex $mathData [expr {$firstSample-1}]]
	set y1 [expr {$scope::yPlotHeight/2.0+$scope::yBorder-$temp/($math::verticalBoxM*5)*$scope::yPlotHeight/2}]
	set temp [lindex $mathData $firstSample]
	set y2 [expr {$scope::yPlotHeight/2.0+$scope::yBorder-$temp/($math::verticalBoxM*5)*$scope::yPlotHeight/2}]
	set m [expr {($y1-$y2)/($x1-$x2)}]
	set b [expr {$y1-$m*$x1}]
	set yf [expr {$m*$scope::xPlotStart+$b}]
	
	set plotData {}
	lappend plotData $scope::xPlotStart
	lappend plotData $yf
	lappend plotData $x2
	lappend plotData $y2
	
	for {set i [expr {$firstSample+1}]} {$i < $lastSample} {incr i} {
		set xDatum [expr {$::cursor::timePos- ($scope::xPlotWidth/($timebaseSetting*10))*((1024-$::cursor::sampleOffset-5-$i)/$sampleRate)}]	
		set yDatum [lindex $mathData $i]
		set y [expr {$scope::yPlotHeight/2.0+$scope::yBorder-$yDatum/($math::verticalBoxM*5)*$scope::yPlotHeight/2}]
		lappend plotData $xDatum $y
	}
	
	#Calculate the last point that should appear on the right border
	set x1 [lindex $plotData end-1]
	set x2 [expr {$cursor::timePos-($scope::xPlotWidth/($timebaseSetting*10))*((1024-$cursor::sampleOffset-5-($lastSample+1))/$sampleRate)}]
	
	#Calculate the last point for channel A
	set y1 [lindex $plotData end]
	set temp [lindex $mathData [expr {$lastSample+1}]]
	set y2 [expr {$scope::yPlotHeight/2.0+$scope::yBorder-$temp/($math::verticalBoxM*5)*$scope::yPlotHeight/2}]
	set m [expr {($y1-$y2)/($x1-$x2)}]
	set b [expr {$y1-$m*$x1}]
	set yl [expr {$m*$scope::xPlotEnd+$b}]
	lappend plotData $scope::xPlotEnd
	lappend plotData $yl
	
	$scopePath.display create line	\
		$plotData		\
		-tag math		\
		-fill violet
}

proc math::adjustMath {direction} {
	
	switch $direction {
		"in" {
			set math::verticalIndexM [expr {$math::verticalIndexM - 1}]
		} "out" {
			set math::verticalIndexM [expr {$math::verticalIndexM + 1}]
		}
	}
	
	if { $math::verticalIndexM < 0} { set math::verticalIndexM 0 }
	if { $math::verticalIndexM > 12} { set math::verticalIndexM 12 }
	
	switch $math::verticalIndexM {
		0 {
			set math::verticalBoxM 0.005
			set math::mathImage [lindex $math::mathImages 0]
		} 1 {
			set math::verticalBoxM 0.01
			set math::mathImage [lindex $math::mathImages 1]
		} 2 {
			set math::verticalBoxM 0.02
			set math::mathImage [lindex $math::mathImages 2]
		} 3 {
			set math::verticalBoxM 0.05
			set math::mathImage [lindex $math::mathImages 3]
		} 4 {
			set math::verticalBoxM 0.1
			set math::mathImage [lindex $math::mathImages 4]
		} 5 {
			set math::verticalBoxM 0.2
			set math::mathImage [lindex $math::mathImages 5]
		} 6 {
			set math::verticalBoxM 0.5
			set math::mathImage [lindex $math::mathImages 6]
		} 7 {
			set math::verticalBoxM 1.0
			set math::mathImage [lindex $math::mathImages 7]
		} 8 {
			set math::verticalBoxM 2.0
			set math::mathImage [lindex $math::mathImages 8]
		} 9 {
			set math::verticalBoxM 5.0
			set math::mathImage [lindex $math::mathImages 9]
		} 10 {
			set math::verticalBoxM 10.0
			set math::mathImage [lindex $math::mathImages 10]
		} 11 {
			set math::verticalBoxM 20.0
			set math::mathImage [lindex $math::mathImages 11]
		} 12 {
			set math::verticalBoxM 50.0
			set math::mathImage [lindex $math::mathImages 12]
		}
	}
	
	.math.mag.sensitivity configure -image $math::mathImage
	
}


