#File: automeasure.tcl
#Syscomp GUI
#Scope Automatic Measurement Procedures

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

package provide automeasure 1.0

namespace eval automeasure {

	set enableA(frequency) 0
	set enableB(frequency) 0

	set measurementEnable 0
	
	#Images
	set autoFrequency [image create photo -file "$::images/AutoFrequency.gif"]
	set autoAverage [image create photo -file "$::images/AutoAverage.gif"]
	set autoMax [image create photo -file "$::images/AutoMax.gif"]
	set autoMin [image create photo -file "$::images/AutoMin.gif"]
	set autoPkPk [image create photo -file "$::images/AutoPkPk.gif"]
	set autoRMS [image create photo -file "$::images/AutoRMS.gif"]

	set rawAverageA 0
	set rawAverageB 0

}

proc automeasure::showMeasurements {} {

	#Check to see if the measurements window is already open
	if {[winfo exists .measurements]} {
		raise .measurements
		focus .measurements
		return
	}

	#Create the mesurements window
	toplevel .measurements
	wm resizable .measurements 0 0
	wm title .measurements "Measurements Window"
	wm protocol .measurements WM_DELETE_WINDOW {set automeasure::measurementEnable 0; destroy .measurements}
	
	#Create a frame for the auto measurements
	frame .measurements.auto	\
		-relief groove	\
		-borderwidth 2
		
	label .measurements.auto.autoLabel	\
	-text "Auto Measurements"	\
	-font {-weight bold -size 10}
	label .measurements.auto.aLabel	\
		-text "A"	\
		-font {-weight bold -size 10}
	label .measurements.auto.bLabel	\
		-text "B"	\
		-font {-weight bold -size 10}
	label .measurements.auto.mathLabel	\
		-text "Math"	\
		-font {-weight bold -size 10}

	label .measurements.auto.freqLabel	\
		-text "Frequency:"	\
		-width 12
	label .measurements.auto.freqA	\
		-relief sunken	\
		-textvariable automeasure::autoFrequencyA	\
		-width 12
	label .measurements.auto.freqB	\
		-relief sunken	\
		-textvariable automeasure::autoFrequencyB	\
		-width 12
	label .measurements.auto.freqMath	\
		-relief sunken	\
		-textvariable automeasure::autoFrequencyMath	\
		-width 12
	label .measurements.auto.freqImage	\
		-image $automeasure::autoFrequency

	label .measurements.auto.periodLabel	\
		-text "Period:"	\
		-width 12
	label .measurements.auto.periodA	\
		-relief sunken	\
		-textvariable automeasure::autoPeriodA	\
		-width 12
	label .measurements.auto.periodB	\
		-relief sunken	\
		-textvariable automeasure::autoPeriodB	\
		-width 12
	label .measurements.auto.periodMath	\
		-relief sunken	\
		-textvariable automeasure::autoPeriodMath	\
		-width 12
		
	label .measurements.auto.averageLabel	\
		-text "Average:"	\
		-width 12
	label .measurements.auto.averageA	\
		-relief sunken	\
		-textvariable automeasure::autoAverageA	\
		-width 12
	label .measurements.auto.averageB	\
		-relief sunken	\
		-textvariable automeasure::autoAverageB	\
		-width 12
	label .measurements.auto.averageMath	\
		-relief sunken	\
		-textvariable automeasure::autoAverageMath	\
		-width 12
	label .measurements.auto.averageImage	\
		-image $automeasure::autoAverage
		
	label .measurements.auto.maxLabel	\
		-text "Maximum:"	\
		-width 12
	label .measurements.auto.maxA	\
		-relief sunken	\
		-textvariable automeasure::autoMaxA	\
		-width 12
	label .measurements.auto.maxB	\
		-relief sunken	\
		-textvariable automeasure::autoMaxB	\
		-width 12
	label .measurements.auto.maxMath	\
		-relief sunken	\
		-textvariable automeasure::autoMaxMath	\
		-width 12
	label .measurements.auto.maxImage	\
		-image $automeasure::autoMax

	label .measurements.auto.minLabel	\
		-text "Minimum:"	\
		-width 12
	label .measurements.auto.minA	\
		-relief sunken	\
		-textvariable automeasure::autoMinA	\
		-width 12
	label .measurements.auto.minB	\
		-relief sunken	\
		-textvariable automeasure::autoMinB	\
		-width 12
	label .measurements.auto.minMath	\
		-relief sunken	\
		-textvariable automeasure::autoMinMath	\
		-width 12
	label .measurements.auto.minImage	\
		-image $automeasure::autoMin
		
	label .measurements.auto.pkPkLabel	\
		-text "Peak-Peak:"	\
		-width 12
	label .measurements.auto.pkPkA	\
		-relief sunken	\
		-textvariable automeasure::autoPkPkA	\
		-width 12
	label .measurements.auto.pkPkB	\
		-relief sunken	\
		-textvariable automeasure::autoPkPkB	\
		-width 12
	label .measurements.auto.pkPkMath	\
		-relief sunken	\
		-textvariable automeasure::autoPkPkMath	\
		-width 12
	label .measurements.auto.pkPkImage	\
		-image $automeasure::autoPkPk
		
	label .measurements.auto.rmsLabel	\
		-text "RMS:"	\
		-width 12
	label .measurements.auto.rmsA	\
		-relief sunken	\
		-textvariable automeasure::autoRMSA	\
		-width 12
	label .measurements.auto.rmsB	\
		-relief sunken	\
		-textvariable automeasure::autoRMSB	\
		-width 12
	label .measurements.auto.rmsMath	\
		-relief sunken	\
		-textvariable automeasure::autoRMSMath	\
		-width 12
	label .measurements.auto.rmsImage	\
		-image $automeasure::autoRMS

	grid .measurements.auto.autoLabel -row 0 -column 0 -columnspan 4
	grid .measurements.auto.aLabel -row 1 -column 1
	grid .measurements.auto.bLabel -row 1 -column 2

	grid .measurements.auto.freqLabel -row 2 -column 0
	grid .measurements.auto.freqA -row 2 -column 1
	grid .measurements.auto.freqB -row 2 -column 2
	grid .measurements.auto.freqImage -row 2 -column 4 -rowspan 2

	grid .measurements.auto.periodLabel -row 3 -column 0
	grid .measurements.auto.periodA -row 3 -column 1
	grid .measurements.auto.periodB -row 3 -column 2

	grid .measurements.auto.averageLabel -row 4 -column 0
	grid .measurements.auto.averageA -row 4 -column 1
	grid .measurements.auto.averageB -row 4 -column 2
	grid .measurements.auto.averageImage -row 4 -column 4

	grid .measurements.auto.maxLabel -row 5 -column 0
	grid .measurements.auto.maxA -row 5 -column 1
	grid .measurements.auto.maxB -row 5 -column 2
	grid .measurements.auto.maxImage -row 5 -column 4

	grid .measurements.auto.minLabel -row 6 -column 0
	grid .measurements.auto.minA -row 6 -column 1
	grid .measurements.auto.minB -row 6 -column 2
	grid .measurements.auto.minImage -row 6 -column 4

	grid .measurements.auto.pkPkLabel -row 7 -column 0
	grid .measurements.auto.pkPkA -row 7 -column 1
	grid .measurements.auto.pkPkB -row 7 -column 2
	grid .measurements.auto.pkPkImage -row 7 -column 4

	grid .measurements.auto.rmsLabel -row 8 -column 0
	grid .measurements.auto.rmsA -row 8 -column 1
	grid .measurements.auto.rmsB -row 8 -column 2
	grid .measurements.auto.rmsImage -row 8 -column 4
	
	grid .measurements.auto -row 0 -column 0
	
	set automeasure::measurementEnable 1

}


proc automeasure::automeasure {} {

	if {$automeasure::measurementEnable} {
		automeasure::averages
		automeasure::frequencies
		automeasure::amplitude A
		automeasure::amplitude B
		if {$cursor::timeCursorsEnable} {
			automeasure::autoRMSVoltage [lindex $scope::scopeData 0] a
			automeasure::autoRMSVoltage [lindex $scope::scopeData 1] b
		}
		
	}
	
}

proc automeasure::averages {} {
	
	set dataA [lindex $scope::scopeData 0]
	set verticalBox $scope::verticalBoxA
	set dataB [lindex $scope::scopeData 1]
	set verticalBox $scope::verticalBoxB

	#Calculate the average value of the waveforms
	set averageA 0
	set averageB 0
	set i 0
	foreach datumA $dataA datumB $dataB {
		set averageA [expr {$averageA+$datumA}]
		set averageB [expr {$averageB+$datumB}]
		incr i
	}
	set averageA [expr {$averageA*1.0/$i}]
	set averageB [expr {$averageB*1.0/$i}]
	
	set automeasure::autoAverageA [cursor::formatAmplitude [scope::convertSample $averageA a]]
	set automeasure::autoAverageB [cursor::formatAmplitude [scope::convertSample $averageB b]]
	
	set automeasure::rawAverageA $averageA
	set automeasure::rawAverageB $averageB

}

proc automeasure::average {src} {

	if {$src == "A"} {
		set data [lindex $scope::scopeData 0]
		set verticalBox $scope::verticalBoxA
	} else {
		set data [lindex $scope::scopeData 1]
		set verticalBox $scope::verticalBoxB
	}
	
	#Calculate the average value of the waveform
	set average 0
	for {set i 0} {$i < 1024} {incr i} {
		set temp [lindex $data $i]
		set voltage [scope::convertSample $temp $src]
		set average [expr {$average+$voltage}]
	}
	set average [expr {$average/1024}]
	set average [cursor::formatAmplitude $average]
	
	if {$src == "A"} {
		set automeasure::autoAverageA $average
	} else {
		set automeasure::autoAverageB $average
	}

}

proc automeasure::frequencies {} {

	set dataA [lindex $scope::scopeData 0]
	set dataB [lindex $scope::scopeData 1]
	
	set averageA $automeasure::rawAverageA
	set averageB $automeasure::rawAverageB
	
	set prevCompA 0
	set prevCompB 0
	set compOutA {}
	set compOutB {}
	set upperThresholdA [expr {$averageA + 2}]
	set upperThresholdB [expr {$averageB + 2}]
	set lowerThresholdA [expr {$averageA - 2}]
	set lowerThresholdB [expr {$averageB - 2}]
	set i 0
	set crossingsA {}
	set crossingsB {}
	foreach datumA $dataA datumB $dataB {
		#Channel A Comparator
		if {($prevCompA == 0)} {
			if {$datumA > $upperThresholdA} {
				lappend compOutA 1
				set prevCompA 1
				lappend crossingsA $i
			} else {
				lappend compOutA 0
			}
		} else {
			if {$datumA < $lowerThresholdA} {
				lappend compOutA 0
				set prevCompA 0
			} else {
				lappend compOutA 1
			}
		}
		#Channel B Comparator
		if {($prevCompB == 0)} {
			if {$datumB > $upperThresholdB} {
				lappend compOutB 1
				set prevCompB 1
				lappend crossingsB $i
			} else {
				lappend compOutB 0
			}
		} else {
			if {$datumB < $lowerThresholdB} {
				lappend compOutB 0
				set prevCompB 0
			} else {
				lappend compOutB 1
			}
		}
		
		incr i
	}
	
	#Compute the frequency and period for channel A
	if {[llength $crossingsA] >= 3} {
	
		#Strip off the first crossing as it is an artifact of the hysterisis
		set crossingsA [lrange $crossingsA 1 end]
	
		#Determine the average number of samples 
		#between  average crossings
		set betweenCrossingsA 0
		for {set i 1} {$i < [llength $crossingsA]} {incr i} {
			set betweenCrossingsA [expr {$betweenCrossingsA + [expr [lindex $crossingsA $i] - [lindex $crossingsA [expr $i-1]]]}]
		}
		set betweenCrossingsA [expr {$betweenCrossingsA*1.0/[expr {[llength $crossingsA]-1}]}]
		
		#Determine the amount of time represented by the
		#average  number of samples between average crossings
		set samplePeriod [expr {1.0/[scope::getSampleRate]}]
		set periodA [expr {$betweenCrossingsA*$samplePeriod}]
		set frequencyA [expr {1.0/$periodA}]
		#Format the results for display
		set frequencyA [cursor::formatFrequency $frequencyA]
		set periodA [cursor::formatTime $periodA]
	} else {
		set frequencyA "? Hz"
		set periodA "? s"
	}
	
	#Compute the frequency and period for channel B
	if {[llength $crossingsB] >= 3} {
	
		#Strip off the first crossing as it is an artifact of the hysterisis
		set crossingsB [lrange $crossingsB 1 end]
	
		#Determine the average number of samples 
		#between  average crossings
		set betweenCrossingsB 0
		for {set i 1} {$i < [llength $crossingsB]} {incr i} {
			set betweenCrossingsB [expr {$betweenCrossingsB + [expr [lindex $crossingsB $i] - [lindex $crossingsB [expr $i-1]]]}]
		}
		set betweenCrossingsB [expr {$betweenCrossingsB*1.0/[expr {[llength $crossingsB]-1}]}]
		
		#Determine the amount of time represented by the
		#average  number of samples between average crossings
		set samplePeriod [expr {1.0/[scope::getSampleRate]}]
		set periodB [expr {$betweenCrossingsB*$samplePeriod}]
		set frequencyB [expr {1.0/$periodB}]
		#Format the results for display
		set frequencyB [cursor::formatFrequency $frequencyB]
		set periodB [cursor::formatTime $periodB]
	} else {
		set frequencyB "? Hz"
		set periodB "? s"
	}
			
	
	
	set automeasure::autoFrequencyA $frequencyA
	set automeasure::autoPeriodA $periodA
	set automeasure::autoFrequencyB $frequencyB
	set automeasure::autoPeriodB $periodB
}

proc automeasure::frequency {src} {
		
	if {$src == "A"} {
		set data [lindex $scope::scopeData 0]
	} else {
		set data [lindex $scope::scopeData 1]
	}	
	
	#Determine the average of the samples
	set average 0
	for {set i 0} {$i < 1024} {incr i} {
		set sampleValue [lindex $data $i]
		set average [expr {$average + $sampleValue}]
	}
	set average [expr {$average*1.0/1024}]
	
	#Determine where the waveform crosses the average
	set crossings {}
	for {set i 1} {$i < 1024} {incr i} {
		if {[lindex $data $i] > $average && [lindex $data [expr $i-1]] <= $average} {
			lappend crossings $i
			set i [expr {$i+20}]
		}
	}
	
	#Compute the frequency and period
	if {[llength $crossings] >= 2} {
		#Determine the average number of samples 
		#between  average crossings
		set betweenCrossings 0
		for {set i 1} {$i < [llength $crossings]} {incr i} {
			set betweenCrossings [expr {$betweenCrossings + [expr [lindex $crossings $i] - [lindex $crossings [expr $i-1]]]}]
		}
		set betweenCrossings [expr {$betweenCrossings*1.0}]
		set betweenCrossings [expr {$betweenCrossings/[expr {[llength $crossings]-1}]}]
			
		#Determine the amount of time represented by the
		#average  number of samples between average crossings
		set samplePeriod [expr {1.0/[scope::getSampleRate]}]
		set period [expr {$betweenCrossings*$samplePeriod}]
		set frequency [expr {1.0/$period}]
		#Format the results for display
		set frequency [cursor::formatFrequency $frequency]
		set period [cursor::formatTime $period]
	} else {
		set frequency "? Hz"
		set period "? s"
	}
	
	if {$src == "A"} {
		set automeasure::autoFrequencyA $frequency
		set automeasure::autoPeriodA $period
	} else {
		set automeasure::autoFrequencyB $frequency
		set automeasure::autoPeriodB $period
	}
	
}

proc automeasure::amplitude {src} {

	if {$src == "A"} {
		set data [lindex $scope::scopeData 0]
		set verticalBox $scope::verticalBoxA
	} else {
		set data [lindex $scope::scopeData 1]
		set verticalBox $scope::verticalBoxB
	}
	
	#Calculate the average value of the waveform
	set maximum -5000
	set minimum 5000
	foreach datum $data {
		set voltage [scope::convertSample $datum $src]
		if {$voltage > $maximum} {
			set maximum $voltage
		}
		if {$voltage < $minimum} {
			set minimum $voltage
		}
	}
	set peakToPeak [expr {$maximum - $minimum}]
	set maximum [cursor::formatAmplitude $maximum]
	set minimum [cursor::formatAmplitude $minimum]
	set peakToPeak [cursor::formatAmplitude $peakToPeak]

	if {$src == "A"} {
		set automeasure::autoMaxA $maximum
		set automeasure::autoMinA $minimum
		set automeasure::autoPkPkA $peakToPeak
	} else {
		set automeasure::autoMaxB $maximum
		set automeasure::autoMinB $minimum
		set automeasure::autoPkPkB $peakToPeak
	}


}

proc automeasure::autoRMSVoltage {data src} {

	if {[llength $data] == 0} {return}

	#Make sure the cursors aren't on top of one another
	if {$cursor::t1Pos==$cursor::t2Pos} {
		return "?"
	}
	
	#Determine relative position of cursors to one another
	if {$cursor::t1Pos>$cursor::t2Pos} {
		set start $cursor::t2Pos
		set end $cursor::t1Pos
	} else {
		set start $cursor::t1Pos
		set end $cursor::t2Pos
	}
	
	#Determine the sample position of each cursor
	set startSample [cursor::screenXToSampleIndex $start]
	set endSample [cursor::screenXToSampleIndex $end]

	puts "Start Sample: $startSample"
	puts "End Sample: $endSample"

	set sum 0
	for {set i $startSample} {$i <= $endSample} {incr i} {
		set datum [lindex $data $i]
		if {$src != "math"} {
			set datum [scope::convertSample $datum $src]
		}
		set sum [expr {$sum+($datum*$datum)}]
	}
	set mean [expr {$sum/($endSample-$startSample)}]
	set rms [expr {sqrt($mean)}]
	set rms [cursor::formatAmplitude $rms]
	
	if {$src == "a"} {
		set automeasure::autoRMSA $rms
	} elseif {$src == "b"} {
		set automeasure::autoRMSB $rms
	} elseif {$src == "math"} {
		set automeasure::autoRMSMath $rms
	}

	

}

automeasure::showMeasurements
.menubar.scopeView.viewMenu add separator
.menubar.scopeView.viewMenu add command \
	-label "Auto Measurements"	\
	-command automeasure::showMeasurements