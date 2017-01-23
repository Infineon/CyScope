
package provide scope 1.0
package require Img
package require BWidget

namespace eval scope {

#---=== Scope Global Variables ===---

#Scope GUI Images
set zoomInImage [image create photo -file "$::images/MagIn.gif"]
set zoomOutImage [image create photo -file "$::images/MagOut.gif"]
set channelAHeader [image create photo -file "$::images/ChannelA.gif"]
set channelBHeader [image create photo -file "$::images/ChannelB.gif"]
set triggerDisplay [image create photo -file "$::images/aRising.gif"]
# GJL
set gndUpImage [image create photo -file "$::images/GndUp.gif"]
set gndDnImage [image create photo -file "$::images/GndDn.gif"]
set gndZeroImage [image create photo -file "$images/GndZero.gif"]

#Scope Display Geometry
set xPlotWidth 370
set yPlotHeight 330
set xBorder 20
set yBorder 15
set tickLength 4
set xPlotStart $xBorder
set xPlotEnd [expr {$xPlotWidth+$xBorder}]
set yPlotStart $yBorder
set yPlotEnd [expr {$yPlotHeight+$yBorder}]
set xMid [expr {($xPlotEnd-$xPlotStart)/2+$xBorder}]
set yMid [expr {($yPlotEnd-$yPlotStart)/2+$yBorder}]

#Scope Trigger Modes
set triggerModes {"Auto" "Normal" "Single-Shot" "External"}
set triggerSlopes {"Plus" "Minus"}
set triggerSlope [lindex $triggerSlopes 0]
set triggerSources {"A" "B"}

#Scope Vertical Controls
set verticalValues {"50mV/div" "100mV/div" "200mV/div" "500mV/div" "1V/div" "2V/div" "5V/div" "10V/div" "20V/div" "50V/div"}
set verticalIndexA 4
set verticalA [lindex $verticalValues $verticalIndexA]
set verticalIndexB 4
set verticalB [lindex $verticalValues $verticalIndexB]
set aImages(0) [image create photo -file "$images/a50mV.gif"]
set aImages(1) [image create photo -file "$images/a100mV.gif"]
set aImages(2) [image create photo -file "$images/a200mV.gif"]
set aImages(3) [image create photo -file "$images/a500mV.gif"]
set aImages(4) [image create photo -file "$images/a1V.gif"]
set aImages(5) [image create photo -file "$images/a2V.gif"]
set aImages(6) [image create photo -file "$images/a5V.gif"]
set aImages(7) [image create photo -file "$images/a10V.gif"]
set aImages(8) [image create photo -file "$images/a20V.gif"]
set aImages(9) [image create photo -file "$images/a50V.gif"]
set verticalAImage $aImages($verticalIndexA)
set bImages(0) [image create photo -file "$images/b50mV.gif"]
set bImages(1) [image create photo -file "$images/b100mV.gif"]
set bImages(2) [image create photo -file "$images/b200mV.gif"]
set bImages(3) [image create photo -file "$images/b500mV.gif"]
set bImages(4) [image create photo -file "$images/b1V.gif"]
set bImages(5) [image create photo -file "$images/b2V.gif"]
set bImages(6) [image create photo -file "$images/b5V.gif"]
set bImages(7) [image create photo -file "$images/b10V.gif"]
set bImages(8) [image create photo -file "$images/b20V.gif"]
set bImages(9) [image create photo -file "$images/b50V.gif"]
set verticalBImage $bImages($verticalIndexB)
set verticalBoxA 1.0
set verticalBoxB 1.0
set enableA 1
set enableB 1
set invertA 0
set invertB 0

# KEES sets the default time base
set timebaseIndex 6
set masterSampleRate 818.18E3
# KEES mucking about.  commented out is the original sampling rates array
#set samplingRates {0 0 0 0 0 0 0 1 2 4 5 6 7 8 9 10 11 12 14 15}
# GJL - these need to select a range that will give
#       1024 samples in a time that is more than 10 periods.
#       For example, for the last range (1 sec) we need 1024
#       samples to take at least 10 seconds so the sample rate
#       must be less than 1024/10 = 102.4. Othewise, the PSoC
#       will be sending data too frequently for the scope to
#       keep up.
set samplingRates {0 0 0 0 1 3 4 5 6 7 8 9 10 11 12 13}
# KEES
#set timebaseValues {50E-9 100E-9 200E-9 500E-9 1E-6 2E-6 5E-6 10E-6 20E-6 50E-6	\
	100E-6 200E-6 500E-6 1E-3 2E-3 5E-3 10E-3 20E-3 50E-3 100E-3}
# GJL
set timebaseValues {10E-6 20E-6 50E-6	 100E-6 200E-6 500E-6 1E-3 2E-3 5E-3 10E-3 20E-3 50E-3 100E-3 200E-3 500E-3 1}
# GJL
set autoTriggerTimeouts {500 500 500 500 500 500 500 500 500 \
     500 1000 2000 2000 5000 6000 11000}
# KEES
# set timebaseImages [image create photo -file "$images/50ns.gif"]
# lappend timebaseImages [image create photo -file "$images/100ns.gif"]
# lappend timebaseImages [image create photo -file "$images/200ns.gif"]
# lappend timebaseImages [image create photo -file "$images/500ns.gif"]
#set timebaseImages [image create photo -file "$images/1us.gif"]
#lappend timebaseImages [image create photo -file "$images/2us.gif"]
#lappend timebaseImages [image create photo -file "$images/5us.gif"]
set timebaseImages [image create photo -file "$images/10us.gif"]
lappend timebaseImages [image create photo -file "$images/20us.gif"]
lappend timebaseImages [image create photo -file "$images/50us.gif"]
lappend timebaseImages [image create photo -file "$images/100us.gif"]
lappend timebaseImages [image create photo -file "$images/200us.gif"]
lappend timebaseImages [image create photo -file "$images/500us.gif"]
lappend timebaseImages [image create photo -file "$images/1ms.gif"]
lappend timebaseImages [image create photo -file "$images/2ms.gif"]
lappend timebaseImages [image create photo -file "$images/5ms.gif"]
lappend timebaseImages [image create photo -file "$images/10ms.gif"]
lappend timebaseImages [image create photo -file "$images/20ms.gif"]
lappend timebaseImages [image create photo -file "$images/50ms.gif"]
#GJL
lappend timebaseImages [image create photo -file "$images/100ms.gif"]
lappend timebaseImages [image create photo -file "$images/200ms.gif"]
lappend timebaseImages [image create photo -file "$images/500ms.gif"]
lappend timebaseImages [image create photo -file "$images/1s.gif"]

set timebaseImage [lindex $timebaseImages $timebaseIndex]

set triggerPoint 0
# KEES original was 512
set triggerLevel 512

set scopeData {}

set intExt 0

#A/D Converter step size
variable stepSizeHighDefault 0.0521
variable stepSizeLowDefault 0.00592
variable stepSizeAHigh $stepSizeHighDefault
variable stepSizeALow  $stepSizeLowDefault
variable stepSizeBHigh $stepSizeHighDefault
variable stepSizeBLow $stepSizeLowDefault

set waveColorA red
set waveColorB blue

set gridEnable 0
set gridColor grey
set backgroundColor white
set borderColor black

set triggered 1

set autoTriggerHandle ""

set offsetA1 0
set offsetA2 0
set offsetB1 0
set offsetB2 0

set xyEnable 0

set probeA 1.0
set probeB 1.0

set triggerFilterDisable 0

set plotObsolete 0

#---=== Export Public Procedures ===---

namespace export buildScope
namespace export setScopePath
namespace export getScopePath

}

#---=== Procedures ===---

proc scope::setScopePath {scopePath} {
	variable scope
	
	set scope(path) $scopePath
}

proc scope::getScopePath {} {
	variable scope
	
	return $scope(path)
}

proc scope::buildScope {} {

	set scopePath [getScopePath]

	#Create a frame for all of the scope widgets
	frame "$scopePath"	\
		-relief raised	\
		-borderwidth 2
		
	#Create the heading text for the oscilloscope frame
	label "$scopePath.title"	\
		-text "Oscilloscope"	\
		-font {-weight bold -size -16}
		
	#Create a frame for the vertical controls
	frame "$scopePath.vertical"	\
		-relief groove	\
		-borderwidth 2
		
	#Create the heading text for the vertical controls
	label "$scopePath.vertical.title"	\
		-text "Vertical\nControls"	\
		-font {-weight bold -size -14}
		
	#Channel A vertical controls frame
	frame "$scopePath.vertical.cha"	\
		-relief groove	\
		-borderwidth 3	\
		-padx 3	\
		-pady 3
		
	#Channel A header text
	label "$scopePath.vertical.cha.chaLabel"	\
		-text "Channel A"	\
		-font {-weight bold -size -12}
		
	#Menubutton for Channel A Options
	menubutton "$scopePath.vertical.cha.options"	\
		-text "Options"	\
		-relief raised	\
		-menu "$scopePath.vertical.cha.optionsMenu"
	menu "$scopePath.vertical.cha.optionsMenu" -tearoff 0
	$scopePath.vertical.cha.optionsMenu add check	\
		-label "Disable"	\
		-onvalue 0	\
		-offvalue 1	\
		-variable scope::enableA
	$scopePath.vertical.cha.optionsMenu add check	\
		-label "Invert"	\
		-onvalue 1	\
		-offvalue 0	\
		-variable scope::invertA

	#Checkbutton to disable the channel A trace on the display
  	# KEES, I uncommented this block
	# checkbutton "$scopePath.vertical.cha.enableA"	\
		# -text "Disable"	\
		# -onvalue 0	\
		# -offvalue 1	\
		# -variable scope::enableA

	#Button to decrease the vertical sensitivity
	button "$scopePath.vertical.cha.zoomOut"	\
		-image $scope::zoomOutImage	\
		-command {scope::adjustVertical a out}

	
	#Image to show the vertical sensitivity of channel A
	label "$scopePath.vertical.cha.sensitivity" -image $scope::verticalAImage

	#Button to increase the vertical sensitivity
	button "$scopePath.vertical.cha.zoomIn"	\
		-image $scope::zoomInImage	\
		-command {scope::adjustVertical a in}
		
	#Radio buttons to select X1/X10 probe
	radiobutton "$scopePath.vertical.cha.probe1X"	\
		-text "1X"	\
		-variable scope::probeA	\
		-value 1.0	\
		-command {scope::selectProbe a}
# more KEES mucking around    
	# radiobutton "$scopePath.vertical.cha.probe10X"	\
		# -text "10X"	\
		# -variable scope::probeA	\
		# -value 10.0	\
		# -command {scope::selectProbe a}

 	#GJL added
	#Button to shift A gnd position	down
	button "$scopePath.vertical.cha.gndDn"	\
		-image $scope::gndDnImage	\
		-command {cursor::shiftChAGnd 33}
		

	#GJL added
	#Button to set A gnd position to bottom of screen
	button "$scopePath.vertical.cha.gndZero"	\
		-image $scope::gndZeroImage	\
	    -command {cursor::shiftChAGnd [expr 345 - $cursor::chAGndPos]}

	#GJL added
	#Button to shift A gnd position	up
	button "$scopePath.vertical.cha.gndUp"	\
		-image $scope::gndUpImage	\
		-command {cursor::shiftChAGnd -33}

	#Place the channel A vertical controls into their frame
	grid $scopePath.vertical.cha.chaLabel -row 0 -column 0 -columnspan 2
	#grid $scopePath.vertical.cha.enableA -row 1 -column 0 -columnspan 2
	grid $scopePath.vertical.cha.options -row 1 -column 0 -columnspan 2
	grid $scopePath.vertical.cha.sensitivity -row 3 -column 0 -columnspan 2
	grid $scopePath.vertical.cha.zoomOut -row 4 -column 0
	grid $scopePath.vertical.cha.zoomIn -row 4 -column 1	
	# GJL
	grid $scopePath.vertical.cha.gndDn -row 5 -column 0
	grid $scopePath.vertical.cha.gndUp -row 5 -column 1
	
	grid $scopePath.vertical.cha.probe1X -row 6 -column 0
	# GJL
	grid $scopePath.vertical.cha.gndZero -row 6 -column 1
  	# KEES
	#grid $scopePath.vertical.cha.probe10X -row 6 -column 1
	
	
	#Create a frame for the Channel B Controls
	frame $scopePath.vertical.chb	\
		-relief groove	\
		-borderwidth 3	\
		-padx 3	\
		-pady 3
		
	label $scopePath.vertical.chb.chbLabel	\
		-text "Channel B"	\
		-font {-weight bold -size -12}

	#Menubutton for Channel B Options
	menubutton "$scopePath.vertical.chb.options"	\
		-text "Options"	\
		-relief raised	\
		-menu "$scopePath.vertical.chb.optionsMenu"
	menu "$scopePath.vertical.chb.optionsMenu" -tearoff 0
	$scopePath.vertical.chb.optionsMenu add check	\
		-label "Disable"	\
		-onvalue 0	\
		-offvalue 1	\
		-variable scope::enableB
	$scopePath.vertical.chb.optionsMenu add check	\
		-label "Invert"	\
		-onvalue 1	\
		-offvalue 0	\
		-variable scope::invertB

	checkbutton $scopePath.vertical.chb.enableB	\
		-text "Disable"	\
		-onvalue 0	\
		-offvalue 1	\
		-variable scope::enableB
		
	button $scopePath.vertical.chb.zoomOut	\
		-image $scope::zoomOutImage	\
		-command {scope::adjustVertical b out}
		
	label $scopePath.vertical.chb.sensitivity	\
		-image $scope::verticalBImage

	button $scopePath.vertical.chb.zoomIn	\
		-image $scope::zoomInImage	\
		-command {scope::adjustVertical b in}
		
	#Radio buttons to select X1/X10 probe
	radiobutton "$scopePath.vertical.chb.probe1X"	\
		-text "1X"	\
		-variable scope::probeB	\
		-value 1.0	\
		-command {scope::selectProbe b}
    # KEES removing 10X option for PSoC Scope
	# radiobutton "$scopePath.vertical.chb.probe10X"	\
		# -text "10X"	\
		# -variable scope::probeB	\
		# -value 10.0	\
		# -command {scope::selectProbe b}
		
	#GJL added
	#Button to shift B gnd position	down
	button "$scopePath.vertical.chb.gndDn"	\
		-image $scope::gndDnImage	\
		-command {cursor::shiftChBGnd 33}

	#GJL added
	#Button to set B gnd position to bottom of screen
	button "$scopePath.vertical.chb.gndZero"	\
		-image $scope::gndZeroImage	\
	    -command {cursor::shiftChBGnd [expr 345 - $cursor::chBGndPos]}

	
	#GJL added
	#Button to shift B gnd position	up
	button "$scopePath.vertical.chb.gndUp"	\
		-image $scope::gndUpImage	\
		-command {cursor::shiftChBGnd -33}

           
	#Place the Channel B Vertical Controls into their Frame
	grid $scopePath.vertical.chb.chbLabel -row 0 -column 0 -columnspan 2
	#grid $scopePath.vertical.chb.enableB -row 1 -column 0 -columnspan 2
	grid $scopePath.vertical.chb.options -row 2 -column 0 -columnspan 2
	grid $scopePath.vertical.chb.sensitivity -row 3 -column 0 -columnspan 2
	grid $scopePath.vertical.chb.zoomOut -row 4 -column 0
	grid $scopePath.vertical.chb.zoomIn -row 4 -column 1
	# GJL
	grid $scopePath.vertical.chb.gndDn -row 5 -column 0
	grid $scopePath.vertical.chb.gndUp -row 5 -column 1

	grid $scopePath.vertical.chb.probe1X -row 6 -column 0
	# GJL
	grid $scopePath.vertical.chb.gndZero -row 6 -column 1
  	# KEES
	#grid $scopePath.vertical.chb.probe10X -row 6 -column 1
	
	#Arrange the vertical controls frame
	grid $scopePath.vertical.title -row 0 -column 0 -pady 4
	grid $scopePath.vertical.cha -row 1 -column 0 -pady 5
	grid $scopePath.vertical.chb -row 2 -column 0 -pady 5
	
	#Create the scope display
	canvas $scopePath.display	\
		-width [expr $scope::xPlotWidth + 2*$scope::xBorder]	\
		-height [expr $scope::yPlotHeight + 2*$scope::yBorder]	\
		-background $scope::backgroundColor	\
		-borderwidth 2
		
	#Draw the grid lines on the display
	toggleGrid
	
	#Create the Frame for the timebase controls
	frame $scopePath.timebase	\
		-relief groove	\
		-borderwidth 2
		
	label $scopePath.timebase.timebaseLabel	\
		-text "Timebase Controls"	\
		-pady 4	\
		-font {-weight bold -size -14}
		
	label $scopePath.timebase.horizontalLabel	\
		-text "MTB"
		
	button $scopePath.timebase.zoomOut	\
		-image $scope::zoomOutImage	\
		-command {scope::adjustTimebase out}
		
	label $scopePath.timebase.mtb -image $scope::timebaseImage

	button $scopePath.timebase.zoomIn	\
		-image $scope::zoomInImage	\
		-command {scope::adjustTimebase in}
		
	grid $scopePath.timebase.timebaseLabel -row 0 -column 0 -columnspan 3
	grid $scopePath.timebase.zoomOut -row 1 -column 0 -padx 3
	grid $scopePath.timebase.mtb -row 1 -column 1
	grid $scopePath.timebase.zoomIn -row 1 -column 2 -padx 3

	#Frame for the Trigger Controls
	frame $scopePath.trigger	\
		-relief groove	\
		-borderwidth 2	
		
	label $scopePath.trigger.triggerLabel	\
		-text "Trigger Controls"	\
		-font {-weight bold -size -14}
		
	label $scopePath.trigger.modeLabel	\
		-text "Trigger Mode"

	if {$::osType!="Darwin"} {
		ComboBox $scopePath.trigger.modes	\
			-values $scope::triggerModes	\
			-width 12	\
			-textvariable scope::triggerMode	\
			-bwlistbox 1	\
			-hottrack 1		\
			-editable 0		\
			-modifycmd scope::selectTriggerMode
		$scopePath.trigger.modes setvalue first
	} else {
		spinbox $scopePath.trigger.modes	\
			-values $scope::triggerModes	\
			-width 12	\
			-textvariable scope::triggerMode	\
			-state readonly	\
			-readonlybackground white	\
			-command scope::selectTriggerMode	\
			-wrap on
	}

	button $scopePath.trigger.singleShot	\
		-text "Single-Shot Reset"	\
		-command {scope::singleShot}	\
		-state disabled
		
	label $scopePath.trigger.slopeLabel	\
		-text "Trigger Slope"
		
	spinbox $scopePath.trigger.slopes	\
		-values $scope::triggerSlopes	\
		-wrap on	\
		-width 5	\
		-textvariable scope::triggerSlope	\
		-state readonly	\
		-readonlybackground white	\
		-command scope::updateScopeControlReg
		
	label $scopePath.trigger.sourceLabel	\
		-text "Trigger Source"
		
	spinbox $scopePath.trigger.sources	\
		-values $scope::triggerSources	\
		-wrap on	\
		-width 4	\
		-textvariable scope::triggerSource	\
		-state readonly	\
		-readonlybackground white	\
		-command scope::updateScopeControlReg
		
	button $scopePath.trigger.manualTrigger	\
		-text "Manual Trigger"	\
		-command scope::manualTrigger
		
	label $scopePath.trigger.levelLabel	\
		-text "Trigger Level"
		
	label $scopePath.trigger.levelValue	\
		-textvariable scope::triggerVoltage	\
		-relief sunken	\
		-width 8
		
	#Place the Trigger Controls
	grid $scopePath.trigger.triggerLabel -row 0 -column 0 -columnspan 4 -pady 2
	#grid .scope.rhs.trigger.display -row 1 -column 0
	grid $scopePath.trigger.modeLabel -row 1 -column 0
	grid $scopePath.trigger.slopeLabel -row 1 -column 1
	grid $scopePath.trigger.sourceLabel -row 1 -column 2

	grid $scopePath.trigger.modes -row 2 -column 0 -padx 3
	grid $scopePath.trigger.slopes -row 2 -column 1
	grid $scopePath.trigger.sources -row 2 -column 2

	grid $scopePath.trigger.singleShot -row 3 -column 0 -pady 10 -padx 3
	grid $scopePath.trigger.manualTrigger -row 3 -column 1 -columnspan 2
	#grid .scope.rhs.trigger.levelLabel -row 1 -column 4
	#grid .scope.rhs.trigger.levelValue -row 2 -column 4
		
	#Place the scope control frames
	grid $scopePath.title -row 0 -column 0 -columnspan 2 -sticky w
	grid $scopePath.vertical -row 1 -column 0 -rowspan 2 -padx 5
	grid $scopePath.display -row 1 -column 1 -columnspan 2 -padx 5
	grid $scopePath.timebase -row 2 -column 1 -sticky w -padx 5
	grid $scopePath.trigger -row 2 -column 2 -sticky w -padx 5

}

proc scope::toggleGrid {} {

	#Get the path to the scope widgets
	set scopePath [getScopePath]

	if {$scope::gridEnable == 1} {
		$scopePath.display delete scopeGrid
		set scope::gridEnable 0
		return
	}

	#Get the display dimensions
	set xBorder $scope::xBorder
	set yBorder $scope::yBorder
	set xPlotWidth $scope::xPlotWidth
	set yPlotHeight $scope::yPlotHeight
	set tickLength $scope::tickLength
	
	#Draw the display border
	$scopePath.display create line \
		$xBorder $yBorder	\
		[expr {$xBorder+$xPlotWidth}] $yBorder	\
		-width 1	\
		-fill $scope::borderColor
	$scopePath.display create line	\
		$xBorder $yBorder	\
		$xBorder [expr {$yBorder+$yPlotHeight}]	\
		-width 1	\
		-fill $scope::borderColor
	$scopePath.display create line	\
		$xBorder [expr {$yBorder+$yPlotHeight}]	\
		[expr {$xBorder+$xPlotWidth}] [expr {$yBorder+$yPlotHeight}]	\
		-width 1	\
		-fill $scope::borderColor
	$scopePath.display create line	\
		[expr {$xBorder+$xPlotWidth}] $yBorder	\
		[expr {$xBorder+$xPlotWidth}] [expr {$yBorder+$yPlotHeight}]	\
		-width 1	\
		-fill $scope::borderColor
		
	#Draw the major grid lines
	for {set i 1} {$i < 10} {incr i} {
		set xCoord [expr {$xBorder+$i*($xPlotWidth/10.0)}]
		$scopePath.display create line	\
			$xCoord $yBorder	\
			$xCoord [expr {$yBorder+$yPlotHeight}]	\
			-tag scopeGrid	\
			-width 1	\
			-fill $scope::gridColor	\
			-dash .
	}
	for {set i 1} {$i < 10} {incr i} {
		set yCoord [expr {$yBorder+$i*($yPlotHeight/10.0)}]
		$scopePath.display create line	\
			$xBorder $yCoord	\
			[expr {$xBorder+$xPlotWidth}] $yCoord	\
			-tag scopeGrid	\
			-width 1	\
			-fill $scope::gridColor	\
			-dash .
	}
	
	#Draw axes and minor tick marks
	$scopePath.display create line	\
		[expr {($xPlotWidth/2.0)+$xBorder}] $yBorder	\
		[expr {($xPlotWidth/2.0)+$xBorder}] [expr {$yBorder+$yPlotHeight}]	\
		-fill $scope::gridColor	\
		-width 1	\
		-tag scopeGrid
	$scopePath.display create line	\
		$xBorder [expr {$yBorder+($yPlotHeight/2.0)}]	\
		[expr {$xPlotWidth+$xBorder}] [expr {$yBorder+($yPlotHeight/2.0)}]	\
		-fill $scope::gridColor	\
		-width 1	\
		-tag scopeGrid
	set tickLeft [expr {$xBorder+($xPlotWidth/2.0)-($tickLength/2.0)}]
	set tickRight [expr {$xBorder+($xPlotWidth/2.0)+($tickLength/2.0)}]
	for {set i 1} {$i <50} {incr i} {
		$scopePath.display create line	\
		$tickLeft [expr {$yBorder+($yPlotHeight/50.0)*$i}]	\
		$tickRight [expr {$yBorder+($yPlotHeight/50.0)*$i}]	\
		-fill $scope::gridColor	\
		-width 1	\
		-tag scopeGrid
	}
	
	set scope::gridEnable 1
	
}

#Adjust Vertical Settings
#-------------------------
proc scope::adjustVertical {channel direction} {
	
	#Change the plot to indicate that the current traces are out-of-date
	if {!$scope::plotObsolete} {
		set scope::plotObsolete 1
		if {$::opMode=="CircuitGear"} {
			scope::plotScopeData
		}
	}

	switch $channel {
		"a" {
			switch $direction {
				"in" {
					set scope::verticalIndexA [expr $scope::verticalIndexA -1]
					if {$scope::probeA == 1.0} {
						if {$scope::verticalIndexA < 0} {
							set scope::verticalIndexA 0
						}
					} else {
						if {$scope::verticalIndexA < 3} {
							set scope::verticalIndexA 3
						}
					}
				} "out" {
					incr scope::verticalIndexA
					if {$scope::probeA == 1.0} {
						if {$scope::verticalIndexA > 6} {
							set scope::verticalIndexA 6
						}
					} else {
						if {$scope::verticalIndexA > 9} {
							set scope::verticalIndexA 9
						}
					}
				}
			}
		} "b" {
			switch $direction {
				"in" {
					set scope::verticalIndexB [expr $scope::verticalIndexB -1]
					if {$scope::probeB == 1.0} {
						if {$scope::verticalIndexB < 0} {
							set scope::verticalIndexB 0
						}
					} else {
						if {$scope::verticalIndexB < 3} {
							set scope::verticalIndexB 3
						}
					}
				} "out" {
					incr scope::verticalIndexB
					if {$scope::probeB == 1.0} {
						if {$scope::verticalIndexB > 6} {
							set scope::verticalIndexB 6
						}
					} else {
						if {$scope::verticalIndexB > 9} {
							set scope::verticalIndexB 9
						}
					}
				}
			}
		}
	}
	set scope::verticalA [lindex $scope::verticalValues $scope::verticalIndexA]
	set scope::verticalB [lindex $scope::verticalValues $scope::verticalIndexB]
	setVertical
	cursor::measureVoltageCursors
	
	
}

#Adjust Vertical Settings
#-------------------------
#This procedure is used to adjust the vertical settings.  The
#vertical settings are mapped to control values for the hardware
#preamp and the command to adjust the preamp settings are
#sent to the hardware.
proc scope::setVertical {} {

	set scopePath [getScopePath]

	switch $scope::verticalA {
		"50mV/div" {
			set scope::verticalBoxA .05
			set verticalAImage $scope::aImages(0)
		        sendCommand "S P a"
		} "100mV/div" {
			set scope::verticalBoxA 0.1
			set verticalAImage $scope::aImages(1)
		        sendCommand "S P a"
		} "200mV/div" {
			set scope::verticalBoxA 0.2
			set verticalAImage $scope::aImages(2)
                        sendCommand "S P a"
		} "500mV/div" {
			set scope::verticalBoxA 0.5
			set verticalAImage $scope::aImages(3)
                        sendCommand "S P a"
		} "1V/div" {
			set scope::verticalBoxA 1.0
			set verticalAImage $scope::aImages(4)
			if {$scope::probeA == 1.0} {
				sendCommand "S P A"
			} else {
				sendCommand "S P a"
			}
		} "2V/div" {
			set scope::verticalBoxA 2.0
			set verticalAImage $scope::aImages(5)
			if {$scope::probeA == 1.0} {
				sendCommand "S P A"
			} else {
				sendCommand "S P a"
			}
		} "5V/div" {
			set scope::verticalBoxA 5.0
			set verticalAImage $scope::aImages(6)
			if {$scope::probeA == 1.0} {
				sendCommand "S P A"
			} else {
				sendCommand "S P a"
			}
		} "10V/div" {
			set scope::verticalBoxA 10.0
			set verticalAImage $scope::aImages(7)
			sendCommand "S P A"
		} "20V/div" {
			set scope::verticalBoxA 20.0
			set verticalAImage $scope::aImages(8)
			sendCommand "S P A"
		} "50V/div" {
			set scope::verticalBoxA 50.0
			set verticalAImage $scope::aImages(9)
			sendCommand "S P A"
		}
	}
	$scopePath.vertical.cha.sensitivity configure -image $verticalAImage
	
	switch $scope::verticalB {
		"50mV/div" {
			set scope::verticalBoxB .05
			set verticalBImage $scope::bImages(0)
			sendCommand "S P b"
		} "100mV/div" {
			set scope::verticalBoxB 0.1
			set verticalBImage $scope::bImages(1)
			sendCommand "S P b"
		} "200mV/div" {
			set scope::verticalBoxB 0.2
			set verticalBImage $scope::bImages(2)
			sendCommand "S P b"
		} "500mV/div" {
			set scope::verticalBoxB 0.5
			set verticalBImage $scope::bImages(3)
			sendCommand "S P b"
		} "1V/div" {
			set scope::verticalBoxB 1.0
			set verticalBImage $scope::bImages(4)
			if {$scope::probeB == 1.0} {
				sendCommand "S P B"
			} else {
				sendCommand "S P b"
			}
		} "2V/div" {
			set scope::verticalBoxB 2.0
			set verticalBImage $scope::bImages(5)
			if {$scope::probeB == 1.0} {
				sendCommand "S P B"
			} else {
				sendCommand "S P b"
			}
		} "5V/div" {
			set scope::verticalBoxB 5.0
			set verticalBImage $scope::bImages(6)
			if {$scope::probeB == 1.0} {
				sendCommand "S P B"
			} else {
				sendCommand "S P b"
			}
		} "10V/div" {
			set scope::verticalBoxB 10.0
			set verticalBImage $scope::bImages(7)
			sendCommand "S P B"
		} "20V/div" {
			set scope::verticalBoxB 20.0
			set verticalBImage $scope::bImages(8)
			sendCommand "S P B"
		} "50V/div" {
			set scope::verticalBoxB 50.0
			set verticalBImage $scope::bImages(9)
			sendCommand "S P B"
		}
		
	}
	
	$scopePath.vertical.chb.sensitivity configure -image $verticalBImage
	
	
	#Calculate the voltage step size for each sample
	
	#Adjust the trigger level accordingly.  We set the yStart
	#coordinate to the current position of the trigger cursor to 
	#simulate a click and ensure that the trigger level is 
	#referenced to the correct screen position.
	set cursor::yStart $cursor::trigPos
	cursor::moveTrigger  $cursor::trigPos
	[getScopePath].display delete trigLevelValue
	
	#Recalculate cursor positions at new vertical setting
	#calculateCursors
	
}

proc scope::selectProbe {channel} {
	if {$channel == "a"} {
		if {$scope::probeA==10.0} {
    # KEES
			#set scope::verticalIndexA [expr {$scope::verticalIndexA + 3}]
      set scope::verticalIndexA [expr {$scope::verticalIndexA}]
			if {$scope::verticalIndexA > 9} {
				set scope::verticalIndexA 9
			}
		} else {
    # KEES
			#set scope::verticalIndexA [expr {$scope::verticalIndexA -3 }]
      set scope::verticalIndexA [expr {$scope::verticalIndexA}]
			if {$scope::verticalIndexA < 0} {
				set scope::verticalIndexA 0
			}
		}
	}
	
	if {$channel == "b"} {
		if {$scope::probeB==10.0} {
    # KEES
			#set scope::verticalIndexB [expr {$scope::verticalIndexB + 3}]
      set scope::verticalIndexB [expr {$scope::verticalIndexB}]
			if {$scope::verticalIndexB > 9} {
				set scope::verticalIndexB 9
			}
		} else {
    # KEES
			#set scope::verticalIndexB [expr {$scope::verticalIndexB-3}]
      set scope::verticalIndexB [expr {$scope::verticalIndexB}]
			if {$scope::verticalIndexB < 0} {
				set scope::verticalIndexB 0
			}
		}
	}
	
	set scope::verticalA [lindex $scope::verticalValues $scope::verticalIndexA]
	set scope::verticalB [lindex $scope::verticalValues $scope::verticalIndexB]
	
	scope::setVertical
}

#Adjust Timebase
#------------------
#A service routine called when the zoom buttons for the
#timebase are pressed
proc scope::adjustTimebase {direction} {

	#Change the plot to indicate that the current traces are out-of-date
	if {!$scope::plotObsolete} {
		set scope::plotObsolete 1
		scope::plotScopeData
	}

	switch $direction {
		"out" {
			incr scope::timebaseIndex
      # KEES changed from 19 to limit the horizontal axis choices
	 #GJL
			if {$scope::timebaseIndex > 15} {
				set scope::timebaseIndex 15
			}
		} "in" {
			set scope::timebaseIndex [expr {$scope::timebaseIndex -1}]
			if {$scope::timebaseIndex <0 } {
				set scope::timebaseIndex 0
			}
		}
	}
	
	set scope::timebaseImage [lindex $scope::timebaseImages $scope::timebaseIndex]
	set scopePath [getScopePath]
	$scopePath.timebase.mtb configure -image $scope::timebaseImage
	if {$cursor::timeCursorsEnable} {
		cursor::measureTimeCursors
	}
	
	scope::updateScopeControlReg
	cursor::adjustTimeOffset $cursor::timePos
}

proc scope::saveTriggerPoint {data} {
	variable autoTriggerHandle
	
	after cancel $autoTriggerHandle
	
	set temp [lindex $data 1]
	set scope::triggerPoint [expr {$temp*256}]
	set temp [lindex $data 2]
	set scope::triggerPoint [expr {$scope::triggerPoint+$temp}]
	
	if {$scope::triggered} {
		set trigText "Triggered"
	} else {
		set trigText "Not Triggered"
	}
	
	[getScopePath].display delete trigTag
	[getScopePath].display create text	\
		$scope::xMid [expr {$scope::yPlotEnd+7}]	\
		-text $trigText	\
		-fill black		\
		-font {-size -12}	\
		-tag trigTag
	
}

#Process Data Received From the Scope
#-------------------------------------------
#This procedure processes the data received from the scope and stores it in the
#scopeData variable.
proc scope::processScopeData {data} {
	
	#Strip off the response type character
	set data [lrange $data 1 end]

	#Get the length of the data array
	set dataLength [llength $data]
	
	set dataIndex [expr {$scope::triggerPoint*4}]
	set dataCounter 0
	set dataA {}
	set dataB {}
	while {$dataCounter < 1024} {
		#Get high data byte for channel A
		set temp [lindex $data $dataIndex]
		set valueA [expr {256*$temp}]
		#Get low data byte
		incr dataIndex
		if {$dataIndex == 4096} {set dataIndex 0}
		set temp [lindex $data $dataIndex]
		set valueA [expr {$valueA+$temp}]
		#Apply offsets
		if {$scope::verticalIndexA < 4} {
			set valueA [expr {$valueA+$scope::offsetA2}]
		} elseif {$scope::verticalIndexA < 7} {
			if {$scope::probeA == 1.0} {
				set valueA [expr {$valueA+$scope::offsetA1}]
			} else {
				set valueA [expr {$valueA+$scope::offsetA2}]
			}
		} else {
			set valueA [expr {$valueA+$scope::offsetA1}]
		}	
		#Save the value
		lappend dataA $valueA
		
		#Get high data byte for channel B
		incr dataIndex
		if {$dataIndex == 4096} {set dataIndex 0}
		set temp [lindex $data $dataIndex]
		set valueB [expr {256*$temp}]
		#Get low data byte
		incr dataIndex
		if {$dataIndex == 4096} {set dataIndex 0}
		set temp [lindex $data $dataIndex]
		set valueB [expr {$valueB + $temp}]
		#Apply offsets
		if {$scope::verticalIndexB < 4} {
			set valueB [expr {$valueB+$scope::offsetB2}]
		} elseif {$scope::verticalIndexB < 7} {
			if {$scope::probeB == 1.0} {
				set valueB [expr {$valueB+$scope::offsetB1}]
			} else {
				set valueB [expr {$valueB+$scope::offsetB2}]
			}
		} else {
			set valueB [expr {$valueB+$scope::offsetB1}]
		}
		#Save the value
		lappend dataB $valueB
		
		#Set up for next set of samples
		incr dataIndex
		if {$dataIndex == 4096} {set dataIndex 0}
		incr dataCounter
	}
	
	set scope::scopeData {}
	lappend scope::scopeData $dataA
	lappend scope::scopeData $dataB
	
	#Save values for export
	set export::exportData {}
	#Add waveform data for channel A
	lappend export::exportData $dataA
	#Add waveform data for channel B
	lappend export::exportData $dataB
	#Add channel A step size
	lappend export::exportData [getStepSize a]
	#Add channel B step size
	lappend export::exportData [getStepSize b]
	#Add sampling rate
	set sampleRate [lindex $scope::samplingRates $scope::timebaseIndex]
	lappend export::exportData [expr {$scope::masterSampleRate/pow(2,$sampleRate)}]
	
	set scope::plotObsolete 0
	
	if {$::opMode == "CircuitGear"} {
		dataRec::dataRecord
		scope::plotScopeData
		automeasure::automeasure
		fft::updateFFT
		scope::plotXY
		math::updateMath
	}

}

proc scope::getStepSize {channel} {
    # GJL add the if for handling trigger when voltage range is 500mV or less
    if {$scope::verticalIndexA <4} {
	return [scope::convertSample 1021 $channel]
    } else {
	return [scope::convertSample 510 $channel]
    }
}

proc scope::convertSample {sample channel} {

	if {($channel =="a") || ($channel=="A")} {
		if {$scope::verticalIndexA <4} {
			set voltage [expr {(1023-$sample)*$scope::stepSizeALow}]
		} elseif {$scope::verticalIndexA < 7} {
			if {$scope::probeA == 1.0} {
				set voltage [expr {(511-$sample)*$scope::stepSizeAHigh}]
			} else {
				set voltage [expr {(1023-$sample)*$scope::stepSizeALow}]
			}
		} else {
			set voltage [expr {(511-$sample)*$scope::stepSizeAHigh}]
		}
		set voltage [expr {$voltage*$scope::probeA}]
		if {$scope::invertA} {
			set voltage [expr {$voltage*(-1.0)}]
		}
	}
	
	if {($channel == "b") || ($channel=="B")} {
		if {$scope::verticalIndexB <4} {
			set voltage [expr {(1023-$sample)*$scope::stepSizeBLow}]
		} elseif {$scope::verticalIndexB < 7} {
			if {$scope::probeB == 1.0} {
				set voltage [expr {(511-$sample)*$scope::stepSizeBHigh}]
			} else {
				set voltage [expr {(1023-$sample)*$scope::stepSizeBLow}]
			}
		} else {
			set voltage [expr {(511-$sample)*$scope::stepSizeBHigh}]
		}
		set voltage [expr {$voltage*$scope::probeB}]
		if {$scope::invertB} {
			set voltage [expr {$voltage*(-1.0)}]
		}
	}
	
	return $voltage
}

proc scope::plotScopeData {} {
	
	set dataA [lindex $scope::scopeData 0]
	set dataB [lindex $scope::scopeData 1]
	
	if {([llength $dataA]!=1024) || ([llength $dataB]!=1024)} {
		return
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

	#Create a list for Channel A screen points
	set plotDataA {}
	#Create a list for Channel B screen points
	set plotDataB {}
	
	#Calculate the first data point which should be on the left border
	set x1 [expr {$cursor::timePos-($scope::xPlotWidth/($timebaseSetting*10))*((1024-$cursor::sampleOffset-5-($firstSample-1))/$sampleRate)}]
	set x2 [expr {$cursor::timePos-($scope::xPlotWidth/($timebaseSetting*10))*((1024-$cursor::sampleOffset-5-$firstSample)/$sampleRate)}]
	
	#Calculate border point for Channel A
	set temp [lindex $dataA [expr {$firstSample-1}]]
	set voltage [scope::convertSample $temp a]
	set y1 [expr {$cursor::waveOffsetA+$scope::yPlotHeight/2.0-$voltage/($scope::verticalBoxA*5)*$scope::yPlotHeight/2}]
	set temp [lindex $dataA $firstSample]
	set voltage [scope::convertSample $temp a]
	set y2 [expr {$cursor::waveOffsetA+$scope::yPlotHeight/2.0-$voltage/($scope::verticalBoxA*5)*$scope::yPlotHeight/2}]
	set m [expr {($y1-$y2)/($x1-$x2)}]
	set b [expr {$y1-$m*$x1}]
	set yf [expr {$m*$scope::xPlotStart+$b}]
	
	#Add the first points to the list for Channel A
	lappend plotDataA $scope::xPlotStart
	lappend plotDataA $yf
	lappend plotDataA $x2
	lappend plotDataA $y2
	
	#Calculate border point for Channel B
	set temp [lindex $dataB [expr {$firstSample-1}]]
	set voltage [scope::convertSample $temp b]
	set y1 [expr {$cursor::waveOffsetB+$scope::yPlotHeight/2.0-$voltage/($scope::verticalBoxB*5)*$scope::yPlotHeight/2}]
	set temp [lindex $dataB $firstSample]
	set voltage [scope::convertSample $temp b]
	set y2 [expr {$cursor::waveOffsetB+$scope::yPlotHeight/2.0-$voltage/($scope::verticalBoxB*5)*$scope::yPlotHeight/2}]
	set m [expr {($y1-$y2)/($x1-$x2)}]
	set b [expr {$y1-$m*$x1}]
	set yf [expr {$m*$scope::xPlotStart+$b}]
	
	#Add the first points to the list for Channel B
	lappend plotDataB $scope::xPlotStart
	lappend plotDataB $yf
	lappend plotDataB $x2
	lappend plotDataB $y2
	
	#Convert the rest of the samples to screen coordinates
	for {set i [expr {$firstSample+1}]} {$i < $lastSample} {incr i} {
		set x [expr {$::cursor::timePos- ($scope::xPlotWidth/($timebaseSetting*10))*((1024-$::cursor::sampleOffset-5-$i)/$sampleRate)}]	

		#Convert the sample for channel A to screen coordinates
		set temp [lindex $dataA $i]
		set voltage [scope::convertSample $temp a] 
		set y [expr {$cursor::waveOffsetA+$scope::yPlotHeight/2.0-$voltage/($scope::verticalBoxA*5)*$scope::yPlotHeight/2}]
		
		lappend plotDataA $x
		lappend plotDataA $y
		
		#Convert the sample for channel B to screen coordinates
		set temp [lindex $dataB $i]
		set voltage [scope::convertSample $temp b]
		set y [expr {$cursor::waveOffsetB+$scope::yPlotHeight/2.0-$voltage/($scope::verticalBoxB*5)*$scope::yPlotHeight/2}]
		
		lappend plotDataB $x
		lappend plotDataB $y
	
	}
	
	#Calculate the last point that should appear on the right border
	set x1 [lindex $plotDataA end-1]
	set x2 [expr {$cursor::timePos-($scope::xPlotWidth/($timebaseSetting*10))*((1024-$cursor::sampleOffset-5-($lastSample+1))/$sampleRate)}]
	
	#Calculate the last point for channel A
	set y1 [lindex $plotDataA end]
	set temp [lindex $dataA [expr {$lastSample+1}]]
	set voltage [scope::convertSample $temp a]
	set y2 [expr {$cursor::waveOffsetA+$scope::yPlotHeight/2.0-$voltage/($scope::verticalBoxA*5)*$scope::yPlotHeight/2}]
	set m [expr {($y1-$y2)/($x1-$x2)}]
	set b [expr {$y1-$m*$x1}]
	set yl [expr {$m*$scope::xPlotEnd+$b}]
	lappend plotDataA $scope::xPlotEnd
	lappend plotDataA $yl
	
	#Calculate the last point for channel B
	set y1 [lindex $plotDataB end]
	set temp [lindex $dataB [expr {$lastSample+1}]]
	set voltage [scope::convertSample $temp b]
	set y2 [expr {$cursor::waveOffsetB+$scope::yPlotHeight/2.0-$voltage/($scope::verticalBoxB*5)*$scope::yPlotHeight/2}]
	set m [expr {($y1-$y2)/($x1-$x2)}]
	set b [expr {$y1-$m*$x1}]
	set yl [expr {$m*$scope::xPlotEnd+$b}]
	lappend plotDataB $scope::xPlotEnd
	lappend plotDataB $yl
	
	
	set scopePath [getScopePath]
	
	#Update inversion icons
	$scopePath.display delete invertTag
	if {$scope::enableA} {
		if {$scope::invertA} {
			$scopePath.display create text	\
				$scope::xPlotStart [expr {$scope::yPlotEnd+7}]	\
				-text "A Inverted"	\
				-fill $scope::waveColorA		\
				-font {-size -12}	\
				-tag invertTag	\
				-anchor w
		}
	} else {
		$scopePath.display create text	\
			$scope::xPlotStart [expr {$scope::yPlotEnd+7}]	\
			-text "A Disabled"	\
			-fill $scope::waveColorA		\
			-font {-size -12}	\
			-tag invertTag	\
			-anchor w
	}
	if {$scope::enableB} {
		if {$scope::invertB} {
			$scopePath.display create text	\
				$scope::xPlotEnd [expr {$scope::yPlotEnd+7}]	\
				-text "B Inverted"	\
				-fill $scope::waveColorB	\
				-font {-size -12}	\
				-tag invertTag	\
				-anchor e
		}
	} else {
		$scopePath.display create text	\
			$scope::xPlotEnd [expr {$scope::yPlotEnd+7}]	\
			-text "B Disabled"	\
			-fill $scope::waveColorB	\
			-font {-size -12}	\
			-tag invertTag	\
			-anchor e
	}
		
	persist::updatePersist $plotDataA $plotDataB
	#interpolate::interpolate $plotDataA
	
	$scopePath.display delete waveDataA
	if {$scope::enableA} {
		if {$scope::plotObsolete} {
			$scopePath.display create line	\
			$plotDataA	\
			-tag waveDataA	\
			-fill $scope::waveColorA	\
			-dash .
		} else {
			$scopePath.display create line	\
				$plotDataA	\
				-tag waveDataA	\
				-fill $scope::waveColorA
		}
		
	}
	
	$scopePath.display delete waveDataB
	if {$scope::enableB} {
		if {$scope::plotObsolete} {
			$scopePath.display create line	\
				$plotDataB	\
				-tag waveDataB	\
				-fill $scope::waveColorB	\
				-dash .
		} else {
			$scopePath.display create line	\
				$plotDataB	\
				-tag waveDataB	\
				-fill $scope::waveColorB
		}
	}
	
	#Get the next capture from the scope
	if {$scope::triggerMode != "Single-Shot" && !$scope::plotObsolete} {
		scope::acquireWaveform
	}
}

proc scope::acquireWaveform {} {
	variable autoTriggerHandle
	
	
	#Set up trigger settings, sample clock
	scope::updateScopeControlReg
	
	#Request a capture
	sendCommand "S G"
	
	if {$scope::triggerMode == "Auto"} {
		set timeoutValue [lindex $scope::autoTriggerTimeouts $scope::timebaseIndex]
		set autoTriggerHandle [after $timeoutValue {scope::manualTrigger;set scope::triggered 0}]
	
	} else {
		[getScopePath].display delete trigTag
	}
	
	set scope::triggered 1
	
}

proc scope::updateScopeControlReg {} {
	
	set temp 0

	#Select Internal or External/Manual Trigger
	if {$scope::intExt} {
		set temp [expr {$temp+64}]
	}

	#Select trigger polarity
	if {$scope::triggerSlope == "Minus"} {
		set temp [expr {$temp+32}]
	}
	
	#Select trigger source
	if {$scope::triggerSource == "B"} {
		set temp [expr {$temp+16}]
	}
	
	#See if trigger filtering is disabled
	if {$scope::triggerFilterDisable} {
		set temp [expr {$temp+128}]
	}

	#Select the correct sample rate for the timebase setting
puts "GJL:  TBIDX: $scope::timebaseIndex"
	set clockDiv [lindex $scope::samplingRates $scope::timebaseIndex]
	puts "Clock DIV: $clockDiv"
	set temp [expr {$temp+$clockDiv}]

	sendCommand "S R $temp"

}

proc scope::manualTrigger {} {

	puts "Manual Trigger!!"
	
	if {$scope::intExt} {
		#External Trigger
		scope::trig 1
		scope::trig 0
	} else {
		#Internal trigger - switch to external
		set scope::intExt 1
		scope::updateScopeControlReg
		
		scope::trig 1
		scope::trig 0
		
		set scope::intExt 0
	}
	
}

proc scope::trig {tValue} {

	if {$tValue} {
		sendCommand "S D 5"
	} else {
		sendCommand "S D 4"
	}
}

proc scope::singleShot {} {

	[getScopePath].display delete waveDataA
	[getScopePath].display delete waveDataB
	[getScopePath].display delete trigTag
	[getScopePath].display create text	\
		$scope::xMid [expr {$scope::yPlotEnd+7}]	\
		-text "Armed"	\
		-fill red		\
		-tag trigTag	\
		-font {-size -12}
	scope::acquireWaveform	

}

proc scope::selectTriggerMode {} {

	if {$scope::triggerMode!="Single-Shot"} {
		[getScopePath].trigger.singleShot configure -state disabled
	} else {
		[getScopePath].trigger.singleShot configure -state normal
	}
	
	[getScopePath].display delete trigText
	
	if {$scope::triggerMode=="External"} {
		set scope::intExt 1
	} else {
		set scope::intExt 0
	}

	scope::acquireWaveform

}

proc scope::getTimebaseSetting {} {

	return [lindex $scope::timebaseValues $scope::timebaseIndex]

}

proc scope::getSampleRate {} {

	set sampleRate [lindex $scope::samplingRates $scope::timebaseIndex]
	set sampleRate [expr {$scope::masterSampleRate/pow(2,$sampleRate)}]

	return $sampleRate

}

proc scope::showOffsetCal {} {

	if {![winfo exists .offset]} {
	
		toplevel .offset
		wm title .offset "Scope Offset Calibration"
		wm iconname .offset "Offset"
		wm resizable .offset 0 0
		
		label .offset.aHighLabel	\
			-text "Channel A\n1V-5V Range"		\
			-font {-weight bold -size -12}
		
		scale .offset.aHighScale	\
			-from -30	\
			-to 30	\
			-length 150	\
			-resolution 1	\
			-showvalue 1	\
			-variable scope::offsetA1
			
		label .offset.bHighLabel	\
			-text "Channel B\n1V-5V Range"		\
			-font {-weight bold -size -12}
		
		scale .offset.bHighScale	\
			-from -30	\
			-to 30	\
			-length 150	\
			-resolution 1	\
			-showvalue 1	\
			-variable scope::offsetB1
			
		label .offset.aLowLabel	\
			-text "Channel A\n50mV-500mV Range"		\
			-font {-weight bold -size -12}
		
		scale .offset.aLowScale	\
			-from -30	\
			-to 30	\
			-length 150	\
			-resolution 1	\
			-showvalue 1	\
			-variable scope::offsetA2
			
		label .offset.bLowLabel	\
			-text "Channel B\n50mV-500mV Range"		\
			-font {-weight bold -size -12}
		
		scale .offset.bLowScale	\
			-from -30	\
			-to 30	\
			-length 150	\
			-resolution 1	\
			-showvalue 1	\
			-variable scope::offsetB2
			
		button .offset.saveCal	\
			-text "Save Calibration Values to Device"	\
			-command scope::saveOffsets
		

		
		grid .offset.aHighLabel -row 0 -column 0
		grid .offset.aHighScale -row 1 -column 0
		grid .offset.bHighLabel -row 0 -column 3
		grid .offset.bHighScale -row 1 -column 3
		grid .offset.aLowLabel -row 0 -column 1
		grid .offset.aLowScale -row 1 -column 1
		grid .offset.bLowLabel -row 0 -column 4
		grid .offset.bLowScale -row 1 -column 4
	
		grid .offset.saveCal -row 4 -column 0 -columnspan 5 -pady 5
	
		#bind .offset <KeyPress-q> {set scope::offsetA1 [expr {$scope::offsetA1+1}]}
		#bind .offset <KeyPress-a> {set scope::offsetA1 [expr {$scope::offsetA1-1}]}
		
		#bind .offset <KeyPress-w> {set scope::offsetB1 [expr {$scope::offsetB1+1}]}
		#bind .offset <KeyPress-s> {set scope::offsetB1 [expr {$scope::offsetB1-1}]}
		
		#bind .offset <KeyPress-e> {set scope::offsetA2 [expr {$scope::offsetA2+1}]}
		#bind .offset <KeyPress-d> {set scope::offsetA2 [expr {$scope::offsetA2-1}]}
		
		#bind .offset <KeyPress-r> {set scope::offsetB2 [expr {$scope::offsetB2+1}]}
		#bind .offset <KeyPress-f> {set scope::offsetB2 [expr {$scope::offsetB2-1}]}
		
	
	} else {
		#Get rid of the old offset cal window and create a new one
		destroy .offset
		scope::showOffsetCal
	}

}

proc scope::autoOffset {} {
	variable autoTriggerHandle
	
	after cancel $autoTriggerHandle

	sendCommand "S P A"
	sendCommand "S P B"

	set scopePath [getScopePath]
	$scopePath.trigger.modes setvalue @2
	scope::singleShot

	tk_messageBox	\
		-message "Please remove any cables or inputs\nconnected to scope channels.\nClick OK to continue"	\
	
	#Wait for any previous captures to complete
	after 1000
	
	scope::manualTrigger

	vwait scope::scopeData
	
	set dataA [lindex $scope::scopeData 0]
	set dataB [lindex $scope::scopeData 1]
	puts $dataA
	puts $dataB
	
	set averageA 0
	set averageB 0
	
	for {set i 0} {$i < 1024} {incr i} {
		set averageA [expr {$averageA + [lindex $dataA $i]}]
		set averageB [expr {$averageB + [lindex $dataB $i]}]
	}

	set averageA [expr {round($averageA/1024)}]
	set averageB [expr {round($averageB/1024)}]
	
	puts "Averages: $averageA $averageB"

	set averageA [expr {(512-$averageA)}]
	set averageB [expr {(512-$averageB)}]
	
	puts "Averages: $averageA $averageB"

	set offsetA1 $averageA
	set offsetB1 $averageB
	
	sendCommand "S P a"
	sendCommand "S P b"
	
	scope::singleShot
	after 500
	scope::manualTrigger
	vwait scope::scopeData
	
	set dataA [lindex $scope::scopeData 0]
	set dataB [lindex $scope::scopeData 1]
	puts $dataA
	puts $dataB
	
	set averageA 0
	set averageB 0
	
	for {set i 0} {$i < 1024} {incr i} {
		set averageA [expr {$averageA + [lindex $dataA $i]}]
		set averageB [expr {$averageB + [lindex $dataB $i]}]
	}

	set averageA [expr {round($averageA/1024)}]
	set averageB [expr {round($averageB/1024)}]
	
	puts "Averages: $averageA $averageB"

	set averageA [expr {(512-$averageA)}]
	set averageB [expr {(512-$averageB)}]
	
	puts "Averages: $averageA $averageB"
	
	set offsetA2 $averageA
	set offsetB2 $averageB

	set scope::offsetA1 $offsetA1
	set scope::offsetA2 $offsetA2
	set scope::offsetB1 $offsetB1
	set scope::offsetB2 $offsetB2

	#Cleanup
	
	#Restore vertical settings
	scope::setVertical
	
	#Set to autotrigger mode
	$scopePath.trigger.modes setvalue first
	scope::selectTriggerMode

}

proc scope::saveOffsets {} {

	set offsetA1 [expr {128-$scope::offsetA1}]
	set offsetA2 [expr {128-$scope::offsetA2}]
	set offsetB1 [expr {128-$scope::offsetB1}]
	set offsetB2 [expr {128-$scope::offsetB2}]
	
	sendCommand "S F $offsetA1 $offsetA2 $offsetB1 $offsetB2"
	
	sendCommand "S O"

}

proc scope::restoreOffsetCal {offsetData} {

	puts "Offsets $offsetData"
	
	#Check to see if the offset data is valid
	if { ([lindex $offsetData 0]==255) } {return}
	
	set scope::offsetA1 [expr {128-[lindex $offsetData 0]}]
	set scope::offsetA2 [expr {128-[lindex $offsetData 1]}]
	set scope::offsetB1 [expr {128-[lindex $offsetData 2]}]
	set scope::offsetB2 [expr {128-[lindex $offsetData 3]}]
	

}

#Postscript Export
#------------------
#This procedure exports the canvas to a PostScript file specified
#by the user in a pop-up dialog box.
proc scope::psExport {} {

	set types {
                  {{PostScript}       {.ps}        }
	}
	set filename [tk_getSaveFile \
		-filetypes  $types	\
		-defaultextension ".ps"	\
		-initialfile "scopeCap"	\
		-parent .	\
		-title "Save Waveform..." \
	]


	if {$filename != ""} {
		set psFile [open $filename w]
		[getScopePath].display postscript -channel $psFile
		close $psFile
	}
}

proc scope::toggleXYMode {} {

	if {$scope::xyEnable} {
		
	} else {
		[getScopePath].display delete xyLabel
		[getScopePath].display delete xyPlotTag
	}
}

proc scope::plotXY {} {
	
	if {!$scope::xyEnable} {return}
	
	[getScopePath].display delete xyPlotTag
	
	set dataA [lindex $scope::scopeData 0]
	set dataB [lindex $scope::scopeData 1]
	
	set xData {}
	foreach datumA $dataA {
		set actualVoltage [convertSample $datumA a]
		set numDiv [expr {$actualVoltage/$scope::verticalBoxA}]
		set screenx [expr {$numDiv*($scope::xPlotWidth/10.0)+($scope::xPlotWidth/2.0)}]
		set screenx [expr {$screenx + $scope::xBorder}]
		lappend xData $screenx
	}
	
	set yData {}
	foreach datumB $dataB {
		set actualVoltage [convertSample $datumB b]
		set numDiv [expr {$actualVoltage/$scope::verticalBoxB}]
		set screeny [expr {$numDiv*($scope::yPlotHeight/-10.0)+($scope::yPlotHeight/2.0)}]
		set screeny [expr {$screeny + $scope::yBorder}]
		lappend yData $screeny
	}
	
	set plotData {}
	foreach xDatum $xData yDatum $yData {
		lappend plotData $xDatum $yDatum
	}
	
	[getScopePath].display create line	\
		$plotData		\
		-tag xyPlotTag	\
		-fill black
	
}

proc scope::showColorOptions {} {
	#Check to see if the color preferences dialog is already open
	if {[winfo exists .color]} {
		raise .color
		focus .color
		return
	}
	
	#Create the color preferences window
	toplevel .color
	wm resizable .color 0 0
	wm title .color "Color Preferences"
	
	labelframe .color.colors	\
		-text "Oscilloscope Display"	\
		-borderwidth 2		\
		-padx 10	\
		-pady 10
	
	label .color.colors.colorLabel -text "Color"
	
	#Scope Display Background
	label .color.colors.backgroundLabel -text "Scope Background"
	button .color.colors.backgroundButton	\
		-background $scope::backgroundColor	\
		-width 2	\
		-height 1	\
		-command {
			set newColor [tk_chooseColor]
			if {$newColor != ""} {
				set scope::backgroundColor $newColor
				[getScopePath].display configure -background $newColor
				.color.colors.backgroundButton configure -background $newColor
			}
		}
		
	#Scope Grid
	label .color.colors.gridLabel -text "Scope Grid"
	button .color.colors.gridButton	\
		-background $scope::gridColor	\
		-width 2	\
		-height 1	\
		-command {
			set newColor [tk_chooseColor]
			if {$newColor != ""} {
				set scope::gridColor $newColor
				scope::toggleGrid; scope::toggleGrid
				.color.colors.gridButton configure -background $newColor
			}
		}
	
	#Scope Border
	label .color.colors.borderLabel -text "Scope Border"
	button .color.colors.borderButton	\
		-background $scope::borderColor	\
		-width 2	\
		-height 1	\
		-command {
			set newColor [tk_chooseColor]
			if {$newColor != ""} {
				set scope::borderColor $newColor
				scope::toggleGrid; scope::toggleGrid
				.color.colors.borderButton configure -background $newColor
			}
		}
		
	grid .color.colors.colorLabel -row 0 -column 1
	
	grid .color.colors.backgroundLabel -row 1 -column 0
	grid .color.colors.backgroundButton -row 1 -column 1
	grid .color.colors.gridLabel -row 2 -column 0
	grid .color.colors.gridButton -row 2 -column 1
	grid .color.colors.borderLabel -row 3 -column 0
	grid .color.colors.borderButton -row 3 -column 1
	
	#Oscilloscope Trace Colors
	labelframe .color.traces	\
		-text "Oscilloscope Traces"	\
		-borderwidth 2		\
		-padx 10	\
		-pady 10
		
	label .color.traces.colorLabel -text "Color"
	
	#Channel A
	label .color.traces.aLabel -text "Channel A"
	button .color.traces.aButton	\
		-background $scope::waveColorA	\
		-width 2	\
		-height 1	\
		-command {
			set newColor [tk_chooseColor]
			if {$newColor != ""} {
				set scope::waveColorA $newColor
				.color.traces.aButton configure -background $newColor
				cursor::drawChAGndCursor
				cursor::toggleChACursor; cursor::toggleChACursor
			}
		}
		
	#Channel B
	label .color.traces.bLabel -text "Channel B"
	button .color.traces.bButton	\
		-background $scope::waveColorB	\
		-width 2	\
		-height 1	\
		-command {
			set newColor [tk_chooseColor]
			if {$newColor != ""} {
				set scope::waveColorB $newColor
				.color.traces.bButton configure -background $newColor
				cursor::drawChBGndCursor
				cursor::toggleChBCursor; cursor::toggleChBCursor
			}
		}
	
	grid .color.traces.colorLabel -row 0 -column 1
	
	grid .color.traces.aLabel -row 1 -column 0 -padx 14
	grid .color.traces.aButton -row 1 -column 1 -padx 15
	grid .color.traces.bLabel -row 2 -column 0
	grid .color.traces.bButton -row 2 -column 1

	#Cursor Colors
	labelframe .color.cursors	\
		-text "Cursors"	\
		-borderwidth 2		\
		-padx 10	\
		-pady 10
		
	label .color.cursors.colorLabel -text "Color"
	
	#Trigger Cursor
	label .color.cursors.trigLabel -text "Trigger Cursor"
	button .color.cursors.trigButton	\
		-background $cursor::trigColor	\
		-width 2	\
		-height 1	\
		-command {
			set newColor [tk_chooseColor]
			if {$newColor != ""} {
				set cursor::trigColor $newColor
				.color.cursors.trigButton configure -background $newColor

				cursor::drawTriggerCursor
			}
		}
		
	#Trigger Point Cursor
	label .color.cursors.timeLabel -text "Trigger Point Cursor"
	button .color.cursors.timeButton	\
		-background $cursor::timeColor	\
		-width 2	\
		-height 1	\
		-command {
			set newColor [tk_chooseColor]
			if {$newColor != ""} {
				set cursor::timeColor $newColor
				.color.cursors.timeButton configure -background $newColor
				cursor::drawXCursor
			}
		}
	
	grid .color.cursors.colorLabel -row 0 -column 1
	
	grid .color.cursors.trigLabel -row 1 -column 0
	grid .color.cursors.trigButton -row 1 -column 1
	grid .color.cursors.timeLabel -row 2 -column 0
	grid .color.cursors.timeButton -row 2 -column 1
	
	button .color.resetDefaults	\
		-text "Reset to Defaults"	\
		-command scope::resetColorDefaults
	
	button .color.saveExit	\
		-text "Save and Close"	\
		-command {
			scope::saveDisplaySettings
			destroy .color
		}
		
	grid .color.colors -row 0 -column 0 -sticky we
	grid .color.traces -row 1 -column 0 -sticky we
	grid .color.cursors -row 2 -column 0 -sticky we
	grid .color.resetDefaults -row 3 -column 0 -pady 5
	grid .color.saveExit -row 4 -column 0 -pady 5
	
}

proc scope::saveDisplaySettings {} {

	set fileId [open color.cfg w]
	puts $fileId $scope::backgroundColor
	puts $fileId $scope::gridColor
	puts $fileId $scope::borderColor
	puts $fileId $scope::waveColorA
	puts $fileId $scope::waveColorB
	puts $fileId $cursor::trigColor
	puts $fileId $cursor::timeColor
	close $fileId

}

proc scope::readColorSettings {} {

	#See if there are any saved preferences
	if [catch {open color.cfg r} fileId] {
		puts "No custom color preferences found."
	} else {
		#Background Color
		if {[gets $fileId line] >= 0} {
			set scope::backgroundColor $line
			[getScopePath].display configure -background $line
		}
		#Grid Color
		if {[gets $fileId line] >= 0} {
			set scope::gridColor $line
			scope::toggleGrid; scope::toggleGrid
		}
		#Border Color
		if {[gets $fileId line] >= 0} {
			set scope::borderColor $line
			scope::toggleGrid; scope::toggleGrid
		}
		#Trace Color A
		if {[gets $fileId line] >= 0} {
			set scope::waveColorA $line
		}
		#Trace Color B
		if {[gets $fileId line] >= 0} {
			set scope::waveColorB $line
		}
		#Trigger Cursor
		if {[gets $fileId line] >= 0} {
			set cursor::trigColor $line
		}
		#Trigger Point Cursor
		if {[gets $fileId line] >= 0} {
			set cursor::timeColor $line
		}
	}

}

proc scope::resetColorDefaults {} {
	
	set scope::backgroundColor white
	[getScopePath].display configure -background white
	.color.colors.backgroundButton configure -background white
	
	set scope::gridColor grey
	scope::toggleGrid; scope::toggleGrid
	.color.colors.gridButton configure -background grey

	set scope::borderColor black
	scope::toggleGrid; scope::toggleGrid
	.color.colors.borderButton configure -background black
	
	set scope::waveColorA red
	.color.traces.aButton configure -background red
	cursor::drawChAGndCursor
	cursor::toggleChACursor; cursor::toggleChACursor

	set scope::waveColorB blue
	.color.traces.bButton configure -background blue
	cursor::drawChBGndCursor
	cursor::toggleChBCursor; cursor::toggleChBCursor
	
	set cursor::trigColor green
	.color.cursors.trigButton configure -background green
	cursor::drawTriggerCursor
	
	set cursor::timeColor violet
	.color.cursors.timeButton configure -background violet
	cursor::drawXCursor

}
