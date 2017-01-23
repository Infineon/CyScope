package provide cursors 1.0

namespace eval cursor {

set timePos $scope::xMid
set sampleOffset 0

set xStart $scope::xMid
set yStart $scope::yMid

set trigPos $scope::yMid

set chAGndPos $scope::yMid
set chBGndPos $scope::yMid

set t1Pos $scope::xPlotStart
set t2Pos [expr {$scope::xPlotWidth/10*3}]
set timeCursorsEnable 0
set timeMeasurePos [expr {$scope::yBorder+20}]

set chACursorEnable 0
set va1Pos [expr {$scope::yMid-$scope::yPlotHeight/10*4}]
set va2Pos [expr {$scope::yMid-$scope::yPlotHeight/10*2}]
set vaMeasurePos [expr {$scope::xBorder+$scope::xPlotWidth/10*1.5}]

set chBCursorEnable 0
set vb1Pos [expr {$scope::yMid+$scope::yPlotHeight/10*2}]
set vb2Pos [expr {$scope::yMid+$scope::yPlotHeight/10*3}]
set vbMeasurePos [expr {$scope::xBorder+$scope::xPlotWidth/10*2.5}]

set waveOffsetA [expr $cursor::chAGndPos-(($scope::yPlotEnd-$scope::yPlotStart)/2)]
set waveOffsetB [expr $cursor::chBGndPos-(($scope::yPlotEnd-$scope::yPlotStart)/2)]

set trigColor green
set timeColor violet

}


proc cursor::drawXCursor {} {

	set scopePath [getScopePath]

	$scopePath.display delete timePosCursor

	#Place the Time Scroll Cursor
	$scopePath.display create line	\
		$cursor::timePos $scope::yPlotStart	\
		$cursor::timePos $scope::yPlotEnd		\
		-tag timePosCursor	\
		-fill $cursor::timeColor
	$scopePath.display create text	\
		$cursor::timePos [expr $scope::yPlotStart - 5] \
		-fill $cursor::timeColor	\
		-tag timePosCursor	\
		-text "X"

	$scopePath.display bind timePosCursor <Button-1> {cursor::markXStart timePosCursor %x}
	$scopePath.display bind timePosCursor <B1-Motion> {cursor::moveTime %x}
	adjustTimeOffset $cursor::timePos

}

#Adjust Time Offset
#-------------------
#This procedure is used to calculate the new address offset for data
#being read back from the scope.  The user is able to move the time
#cursor to either edge of the screen effectively positioning the trigger
#point at any time offset on the display.   This offset is subtracted 
#from the address in the SRAM memory where the trigger occurs.
proc cursor::adjustTimeOffset {cursorPosition} {

	set cursorRatio [expr {($cursorPosition-$scope::xBorder)/($scope::xPlotWidth*1.0)}]
	#puts $cursorRatio
	
	set timebaseSetting [lindex $scope::timebaseValues $scope::timebaseIndex]
	
	set sampleRate [lindex $scope::samplingRates $scope::timebaseIndex]
	set sampleRate [expr {$scope::masterSampleRate/pow(2,$sampleRate)}]
	
	#Calculate timeOffset
	set timeOffset [expr {$timebaseSetting*10-$timebaseSetting*10*$cursorRatio}]
	
	set cursor::sampleOffset [expr {round($timeOffset*$sampleRate)}]
	#puts $cursor::sampleOffset
	
	#Break the value into two bytes
	set byteHigh [expr {round(floor($cursor::sampleOffset/pow(2,8)))}]
	set byteLow [expr {$cursor::sampleOffset%round(pow(2,8))}]
	
	sendCommand "S C $byteHigh $byteLow"
	
	#Change the plot to indicate that the current traces are out-of-date
	if {!$scope::plotObsolete} {
		set scope::plotObsolete 1
		scope::plotScopeData
	}
	
}

#Mark X Start
#-------------
#This procedure  is used by the display when the time cursor is
#moved.  This procedure marks the starting point where the user
#initially grabbed the cursor.
proc cursor::markXStart { cursorTag xPos } {
	
	set dy 0
	
	switch $cursorTag {
		"timePosCursor" {
			set snap [expr {$xPos - $cursor::timePos}]
			set cursor::timePos $xPos
			#Update the hardware
			cursor::adjustTimeOffset $cursor::timePos
		} "t1Cursor" {
			set snap [expr {$xPos - $cursor::t1Pos}]
			set cursor::t1Pos $xPos
			#calculateCursors
		} "t2Cursor" {
			set snap [expr {$xPos - $cursor::t2Pos}]
			set cursor::t2Pos $xPos
			#calculateCursors
		} "vaMeasure" {
			set snap [expr {$xPos - $cursor::vaMeasurePos}]
			set cursor::vaMeasurePos $xPos
		} "vbMeasure" {
			set snap [expr {$xPos - $cursor::vbMeasurePos}]
			set cursor::vbMeasurePos $xPos
		}
		
	}
	
	[getScopePath].display move $cursorTag $snap $dy
	set cursor::xStart $xPos
}

#Mark Y Start
#-------------
#The procedure is used by the display when cursors are moved.
#The procedure marks the starting point where the user initially
#grabs the cursor.
proc cursor::markYStart { cursorTag yPos } {
		
	set dx 0
	switch $cursorTag {
		"va1Cursor" {
			set snap [expr $yPos - $cursor::va1Pos]
			set cursor::va1Pos $yPos
			[getScopePath].display move $cursorTag $dx $snap
			#calculateCursors
		} "va2Cursor" {
			set snap [expr $yPos - $cursor::va2Pos]
			set cursor::va2Pos $yPos
			[getScopePath].display move $cursorTag $dx $snap
			#calculateCursors
		} "vb1Cursor" {
			set snap [expr $yPos - $cursor::vb1Pos]
			set cursor::vb1Pos $yPos
			[getScopePath].display move $cursorTag $dx $snap
			#calculateCursors
		} "vb2Cursor" {
			set snap [expr $yPos - $cursor::vb2Pos]
			set cursor::vb2Pos $yPos
			[getScopePath].display move $cursorTag $dx $snap
			#calculateCursors
		} "chAGnd" {
			set snap [expr $yPos - $cursor::chAGndPos]
			set cursor::chAGndPos $yPos
			[getScopePath].display move $cursorTag $dx $snap
			if { $scope::triggerSource == "A" } {
				[getScopePath].display move trigLevelCursor $dx $snap
				set cursor::trigPos [expr $cursor::trigPos + $snap]
			}
		} "chBGnd" {
			set snap [expr $yPos - $cursor::chBGndPos]
			set cursor::chBGndPos $yPos
			[getScopePath].display move  $cursorTag $dx $snap
			if { $scope::triggerSource == "B"} {
				[getScopePath].display move trigLevelCursor $dx $snap
				set cursor::trigPos [expr $cursor::trigPos + $snap]
			}
		} "trigLevelCursor" {
			set snap [expr $yPos - $cursor::trigPos]
			set cursor::trigPos $yPos
			[getScopePath].display move $cursorTag $dx $snap
		} "timeMeasure" {
			set snap [expr {$yPos - $cursor::timeMeasurePos}]
			set cursor::timeMeasurePos $yPos
			[getScopePath].display move $cursorTag $dx $snap
		}
	}
	
	set cursor::yStart $yPos
}

#Move Time Cursor
#-------------------
#This procdure is called when the user drags the time offset cursor
#on the screen.  It updates the position of the cursor on the screen
#and invokes another procedure "adjustTimeOffset" which crunches
#the numbers and sets up the hardware to read back the appropriate
#data from the scope.
proc cursor::moveTime { xPos } {
	
	#Check to see if we have gone off of the screen
	if { $xPos > $scope::xPlotEnd} {set xPos $scope::xPlotEnd}
	if { $xPos < $scope::xPlotStart} { set xPos $scope::xPlotStart}

	#Move the cursor on the screen
	set dy 0
	set dx [expr $xPos - $cursor::xStart]
	set cursor::xStart $xPos
	set cursor::timePos $xPos
	[getScopePath].display move timePosCursor $dx $dy
	
	#Update the hardware
	cursor::adjustTimeOffset $cursor::timePos
}

proc cursor::drawTriggerCursor {} {
	
	[getScopePath].display delete trigLevelCursor

	[getScopePath].display create line	\
		$scope::xPlotStart $cursor::trigPos	\
		$scope::xPlotEnd $cursor::trigPos	\
		-tag trigLevelCursor	\
		-fill $cursor::trigColor
	[getScopePath].display create text	\
		[expr {$scope::xPlotStart - 5}] $cursor::trigPos	\
		-fill $cursor::trigColor	\
		-tag trigLevelCursor	\
		-text "T"

	[getScopePath].display bind trigLevelCursor <Button-1> { cursor::markYStart trigLevelCursor %y}
	[getScopePath].display bind trigLevelCursor <B1-Motion> {cursor::moveTrigger %y}
	[getScopePath].display bind trigLevelCursor <ButtonRelease-1> {[getScopePath].display delete trigLevelValue}
}

#Move Trigger
#--------------
#This procedure is called when the user drags the trigger cursor
#on the screen.  It calculates the trigger voltage corresponding
#to the current screen position and sends the appropriate command
#to the hardware.
proc cursor::moveTrigger { yPos } {

	
	#Determine whether the trigger source is A or B
	if { $scope::triggerSource == "A" } {
		set reference $cursor::chAGndPos
		set boxSize $scope::verticalBoxA
		if {$scope::verticalIndexA < 4} {
			set offset $scope::offsetA2
		} else {
			set offset $scope::offsetA1
		}
	} else {
		set reference $cursor::chBGndPos
		set boxSize $scope::verticalBoxB
		if {$scope::verticalIndexB < 4} {
			set offset $scope::offsetB2
		} else {
			set offset $scope::offsetB1
		}
	}
	set stepSize [scope::getStepSize $scope::triggerSource]
	
	#Check to see if we have gone off of the screen
	if { $yPos > $scope::yPlotEnd } { set yPos $scope::yPlotEnd }
	if { $yPos < $scope::yPlotStart } { set yPos $scope::yPlotStart }
	
	
	#Calculate the difference between the trigger cursor
	#and its reference in A/D units
	set difference [expr {$reference-$yPos}]
	set numDiv [expr {$difference/($scope::yPlotHeight/10.0)}]


	#Check to see if we have exceeded the maximum or
	#minimum trigger level
	# GJL - allow trigger to be more than 5 divisions from the 	waveform ground	
	#if { $numDiv < -5.0 } {
	#		set numDiv -5.0
	#		set yPos [expr {$reference - ($numDiv*	$scope::yPlotHeight/10.0)}]
#	} 
	#	if {$numDiv > 5.0} {
#		set numDiv 5.0
	#		set yPos [expr {$reference - ($numDiv*	$scope::yPlotHeight/10.0)}]
	#	}

	#Move the cursor on the screen
	set dx 0
	set dy [expr {$yPos - $cursor::yStart}]
	
	set cursor::yStart $yPos
	set cursor::trigPos $yPos
	[getScopePath].display move trigLevelCursor $dx $dy
	
	#Calculate the trigger voltage that corresponds to the
	#current trigger cursor position
	set triggerVoltage [expr $numDiv*$boxSize]
	
	#Calculate the trigger value to send to the hardware
	set triggerSteps [expr {-1*$triggerVoltage/$stepSize}]
	set triggerSteps [expr {int($triggerSteps)}]
	set triggerLevel [expr {511+$triggerSteps-$offset}]
	puts "trigger level $triggerLevel"

	#Break the trigger value into two bytes
	set byteHigh [expr {round(floor($triggerLevel/pow(2,8)))}]
	set byteLow [expr {$triggerLevel%round(pow(2,8))}]
	
	#Format the trigger voltage for the display
	set triggerVoltage [format "%.3f" $triggerVoltage]
	set triggerVoltage "$triggerVoltage V"
	[getScopePath].display delete trigLevelValue
	[getScopePath].display create text	\
		[expr {$scope::xPlotStart+30}] [expr {$cursor::trigPos - 5}]	\
		-fill black	\
		-tag trigLevelValue	\
		-text $triggerVoltage	\
		-font {-weight bold -size -14}
	
	#Update the hardware
	 sendCommand "S T $byteHigh $byteLow" 
	
}

#Move Channel A Ground Reference
#------------------------------------
#This procedure is called when the user grabs the channel reference
#cursor and drags it up or down on the screen.  This procedure moves
#the cursor, ensures that the user doesn't drag the cursor off the 
#screen, and updates the waveform offset for plotting purposes.
proc cursor::moveChAGnd { yPos } {

	# GJL - remove this so that the ground reference can be
	#       off the screen
	#Make sure we haven't gone off the screen
	#if { $yPos > $scope::yPlotEnd } { set yPos 	$scope::yPlotEnd }
	#if { $yPos < $scope::yPlotStart } { set yPos 	$scope::yPlotStart }
	
	#Move the cursor
	set dx 0
	set dy [expr $yPos - $cursor::yStart]
	set cursor::yStart $yPos
	set cursor::chAGndPos $yPos
	[getScopePath].display move chAGnd $dx $dy
	
	#Move the trigger cursor if trigger source is A
	if { $scope::triggerSource == "A" } {
		[getScopePath].display move trigLevelCursor $dx $dy
		set cursor::trigPos [expr $cursor::trigPos + $dy]
	}
	
	#Calculate the waveform screen offset
	set cursor::waveOffsetA [expr $cursor::chAGndPos-(($scope::yPlotEnd-$scope::yPlotStart)/2)]
	
	
}

proc cursor::drawChAGndCursor {} {

	[getScopePath].display delete chAGnd
	
	#Place the ground cursor for Channel A
	[getScopePath].display create line \
		$scope::xPlotStart $cursor::chAGndPos 	\
		$scope::xPlotEnd $cursor::chAGndPos 	\
		-tag chAGnd 	\
		-fill $scope::waveColorA 		\
		-dash .

	[getScopePath].display create text 	\
		[expr $scope::xPlotEnd+5] $cursor::chAGndPos 	\
		-fill $scope::waveColorA 		\
		-tag chAGnd 	\
		-text "A"

	[getScopePath].display bind chAGnd <Button-1> { cursor::markYStart chAGnd %y}
	[getScopePath].display bind chAGnd <B1-Motion> { cursor::moveChAGnd %y }

}

# GJL Added
# Shift Channel A Ground Reference
#------------------------------------
#This procedure is called when the user clicks the button to 
#move the ground reference up or down.
proc cursor::shiftChAGnd { yPos } {

    puts "gnd level $cursor::chAGndPos"
	#Move the cursor
	set dx 0
	set dy $yPos
	set cursor::yStart [expr $cursor::chAGndPos + $yPos]
	set cursor::chAGndPos [expr $cursor::chAGndPos + $yPos]
	[getScopePath].display move chAGnd $dx $dy
	
	#Move the trigger cursor if trigger source is A
	if { $scope::triggerSource == "A" } {
		[getScopePath].display move trigLevelCursor $dx $dy
		set cursor::trigPos [expr $cursor::trigPos + $dy]
	}
	
	#Calculate the waveform screen offset
	set cursor::waveOffsetA [expr $cursor::chAGndPos-(($scope::yPlotEnd-$scope::yPlotStart)/2)]
		
}

#Move Channel B Ground Reference
#------------------------------------
#This procedure is called when the user grabs the channel reference
#cursor and drags it up or down on the screen.  This procedure moves
#the cursor, ensures that the user doesn't drag the cursor off the 
#screen, and updates the waveform offset for plotting purposes.
proc cursor::moveChBGnd { yPos } {

	
	# GJL - remove this so that the ground reference can be
	#       off the screen
	#Make sure we haven't gone off the screen
	#if { $yPos > $scope::yPlotEnd } { set yPos 	$scope::yPlotEnd }
	#if { $yPos < $scope::yPlotStart } { set yPos 	$scope::yPlotStart }

	#Move the cursor
	set dx 0
	set dy [expr $yPos - $cursor::yStart]
	set cursor::yStart $yPos
	set cursor::chBGndPos $yPos
	[getScopePath].display move chBGnd $dx $dy
	
	#Move the trigger cursor too, if the trigger source is B
	if { $scope::triggerSource == "B" } {
		[getScopePath].display move trigLevelCursor $dx $dy
		set cursor::trigPos [expr $cursor::trigPos + $dy]
	}
	
	#Calculate the waveform screen offset
	set cursor::waveOffsetB [expr $cursor::chBGndPos-(($scope::yPlotEnd-$scope::yPlotStart)/2)]
}

proc cursor::drawChBGndCursor {} {

	[getScopePath].display delete chBGnd
	
	#Place the ground cursor for Channel B
	[getScopePath].display create line \
		$scope::xPlotStart $cursor::chBGndPos 	\
		$scope::xPlotEnd $cursor::chBGndPos 	\
		-tag chBGnd 	\
		-fill $scope::waveColorB 		\
		-dash .

	[getScopePath].display create text 	\
		[expr $scope::xPlotEnd+5] $cursor::chBGndPos 	\
		-fill $scope::waveColorB 		\
		-tag chBGnd 	\
		-text "B"

	[getScopePath].display bind chBGnd <Button-1> { cursor::markYStart chBGnd %y}
	[getScopePath].display bind chBGnd <B1-Motion> { cursor::moveChBGnd %y }

}

# GJL Added
# Shift Channel B Ground Reference
#------------------------------------
#This procedure is called when the user clicks the button to 
#move the ground reference up or down.
proc cursor::shiftChBGnd { yPos } {

	#Move the cursor
	set dx 0
	set dy $yPos
	set cursor::yStart [expr $cursor::chBGndPos + $yPos]
	set cursor::chBGndPos [expr $cursor::chBGndPos + $yPos]
	[getScopePath].display move chBGnd $dx $dy
	
	#Move the trigger cursor if trigger source is B
	if { $scope::triggerSource == "B" } {
		[getScopePath].display move trigLevelCursor $dx $dy
		set cursor::trigPos [expr $cursor::trigPos + $dy]
	}
	
	#Calculate the waveform screen offset
	set cursor::waveOffsetB [expr $cursor::chBGndPos-(($scope::yPlotEnd-$scope::yPlotStart)/2)]
		
}

#Toggle Time Cursors
#------------------------
#This process is used to toggle the time cursors.  When the 
#cursor are enabled this procedure draws the cursor lines and
#handles on the screen, binds mouse clicks on the cursors to 
#handler procedures, and adds measurement labels to the GUI.
#When the cursors are disabled, this procedure deletes the cursors
#and their handles from the canvas and removes the measurement
#labels from the GUI.
proc cursor::toggleTimeCursors {} {

	if {!$cursor::timeCursorsEnable} {
		[getScopePath].display create line	\
			$cursor::t1Pos $scope::yPlotStart \
			$cursor::t1Pos $scope::yPlotEnd \
			-tag t1Cursor \
			-fill brown
		[getScopePath].display create text \
			$cursor::t1Pos [expr {$scope::yPlotStart -5}] \
			-fill brown \
			-tag t1Cursor \
			-text "T1"
		[getScopePath].display create line \
			$cursor::t2Pos $scope::yPlotStart \
			$cursor::t2Pos $scope::yPlotEnd \
			-tag t2Cursor \
			-fill brown
		[getScopePath].display create text \
			$cursor::t2Pos [expr {$scope::yPlotStart -5}] \
			-fill brown \
			-tag t2Cursor \
			-text "T2"
		[getScopePath].display bind t1Cursor <Button-1> {cursor::markXStart t1Cursor %x}
		[getScopePath].display bind t1Cursor <B1-Motion> {cursor::moveTimeCursor t1Cursor %x}
		[getScopePath].display bind t2Cursor <Button-1> {cursor::markXStart t2Cursor %x}
		[getScopePath].display bind t2Cursor <B1-Motion> {cursor::moveTimeCursor t2Cursor %x}
		[getScopePath].display bind timeMeasure <Button-1> {cursor::markYStart timeMeasure %y}
		[getScopePath].display bind timeMeasure <B1-Motion> {cursor::moveTimePos %y}
		set cursor::timeCursorsEnable 1
		measureTimeCursors
	} else {
		#Disable the cursors
		[getScopePath].display delete t1Cursor
		[getScopePath].display delete t2Cursor
		[getScopePath].display delete timeMeasure
		set cursor::timeCursorsEnable 0
	}
}

#Move Time Cursor
#----------------
#This procedure is called when the user drags either time cursor on the
#screen.  The procedure ensures that the user does not drag the 
#cursor off of the edge of the screen and updates the global variable
#which stores the current location of the cursor.
proc cursor::moveTimeCursor { cursorName xPos } {

	#Check to see if we have gone off of the screen
	if { $xPos > $scope::xPlotEnd} {set xPos $scope::xPlotEnd}
	if { $xPos < $scope::xPlotStart} { set xPos $scope::xPlotStart}

	#Move the cursor on the screen
	set dy 0
	set dx [expr $xPos - $cursor::xStart]
	set cursor::xStart $xPos
	
	if {$cursorName == "t1Cursor"} {
		set cursor::t1Pos $xPos
	}
	if {$cursorName == "t2Cursor"} {
		set cursor::t2Pos $xPos
	}
	
	[getScopePath].display move $cursorName $dx $dy
	
	cursor::measureTimeCursors
}

proc cursor::measureTimeCursors {} {

	if {$cursor::t2Pos > $cursor::t1Pos} {
		set rightCursor $cursor::t2Pos
		set leftCursor $cursor::t1Pos
	} else {
		set rightCursor $cursor::t1Pos
		set leftCursor $cursor::t2Pos
	}
	
	#Get the timebase setting from the scope
	set horizontalBoxTime [lindex $::scope::timebaseValues $scope::timebaseIndex]
	set horizontalBoxWidth [expr {$scope::xPlotWidth/10.0}]
	set pixelTime [expr {$horizontalBoxTime/$horizontalBoxWidth}]
	
	#Calculate the time between the cursors
	set cursorTime [expr {($rightCursor-$leftCursor)*$pixelTime}]
	if {$cursorTime != 0} {
		set cursorFreq [cursor::formatFrequency [expr {1.0/$cursorTime}]]
	} else {
		set cursorFreq "?"
	}
	set cursorFreq "($cursorFreq)"
	set cursorTime [cursor::formatTime $cursorTime]
	
	
	[getScopePath].display delete timeMeasure
	
	#Determine if we are drawing the time callout between the cursors
	#or outside of them
	if {[expr {$rightCursor - $leftCursor}] > 60} {
		[getScopePath].display create line	\
			$leftCursor $cursor::timeMeasurePos	\
			$rightCursor $cursor::timeMeasurePos	\
			-fill black		\
			-tag timeMeasure	\
			-arrow both
		[getScopePath].display create text \
			[expr {($rightCursor-$leftCursor)/2+$leftCursor}] [expr {$cursor::timeMeasurePos - 7}]	\
			-text $cursorTime	\
			-font {-weight bold -size -12}	\
			-tag timeMeasure	\
			-fill black
		[getScopePath].display create text \
			[expr {($rightCursor-$leftCursor)/2+$leftCursor}] [expr {$cursor::timeMeasurePos + 7}]	\
			-text $cursorFreq	\
			-font {-weight bold -size -12}	\
			-tag timeMeasure	\
			-fill black
	} else {
		[getScopePath].display create line	\
			$leftCursor $cursor::timeMeasurePos	\
			[expr {$leftCursor-40}] $cursor::timeMeasurePos	\
			-fill black		\
			-tag timeMeasure	\
			-arrow first
		[getScopePath].display create line	\
			$rightCursor $cursor::timeMeasurePos	\
			[expr {$rightCursor+40}] $cursor::timeMeasurePos	\
			-fill black		\
			-tag timeMeasure	\
			-arrow first
		#Determine where we will draw the time measure
		if {$rightCursor > $scope::xMid} {
			[getScopePath].display create text \
				[expr {$leftCursor-35}] [expr {$cursor::timeMeasurePos - 10}]	\
				-text $cursorTime	\
				-font {-weight bold -size -12}	\
				-tag timeMeasure	\
				-fill black
			[getScopePath].display create text \
				[expr {$leftCursor-35}] [expr {$cursor::timeMeasurePos + 10}]	\
				-text $cursorFreq	\
				-font {-weight bold -size -12}	\
				-tag timeMeasure	\
				-fill black
		} else {
			[getScopePath].display create text \
				[expr {$rightCursor+35}] [expr {$cursor::timeMeasurePos - 10}]	\
				-text $cursorTime	\
				-font {-weight bold -size -12}	\
				-tag timeMeasure	\
				-fill black
			[getScopePath].display create text \
				[expr {$rightCursor+35}] [expr {$cursor::timeMeasurePos +10}]	\
				-text $cursorFreq	\
				-font {-weight bold -size -12}	\
				-tag timeMeasure	\
				-fill black

		}
	
	
	}
	
	#Update the RMS measurement
	if {$::automeasure::measurementEnable} {
		automeasure::autoRMSVoltage [lindex $scope::scopeData 0] a
		automeasure::autoRMSVoltage [lindex $scope::scopeData 1] b
	}

}

#Format Period
#---------------
#This procedure takes a number representing a time period and
#formats it into a string with the proper units.
proc cursor::formatTime {period} {

	if {$period < 1E-6} {
		set period [format "%3.2f" [expr $period/1E-9]]
		set period "$period ns"
	} elseif {$period < 1E-3} {
		set period [format "%3.2f" [expr $period/1E-6]]
		set period "$period us"
	} elseif {$period < 1.0} {
		set period [format "%3.2f" [expr $period/1E-3]]
		set period "$period ms"
	} else {
		set period [format "%3.2f" $period]
		set period "$period s"
	}
	return $period
}

proc cursor::moveTimePos { yPos } {

	#Make sure we haven't gone off the screen
	if { $yPos > $scope::yPlotEnd } { set yPos $scope::yPlotEnd }
	if { $yPos < $scope::yPlotStart } { set yPos $scope::yPlotStart }
	
	#Move the cursor
	set dx 0
	set dy [expr $yPos - $cursor::yStart]
	set cursor::yStart $yPos
	set cursor::timeMeasurePos $yPos
	[getScopePath].display move timeMeasure $dx $dy
	
}

proc cursor::toggleChACursor {} {

	if {!$cursor::chACursorEnable} {
		#Cursors for Channel A
		[getScopePath].display create line	\
			$scope::xPlotStart $cursor::va1Pos \
			$scope::xPlotEnd $cursor::va1Pos \
			-tag va1Cursor \
			-fill $scope::waveColorA \
			-dash -
		[getScopePath].display create text \
			[expr $scope::xPlotStart + 15] [expr $cursor::va1Pos - 10]\
			-fill $scope::waveColorA \
			-tag va1Cursor \
			-text "VA1"
		[getScopePath].display create line \
			$scope::xPlotStart $cursor::va2Pos\
			$scope::xPlotEnd $cursor::va2Pos\
			-tag va2Cursor \
			-fill $scope::waveColorA \
			-dash -
		[getScopePath].display create text \
			[expr $scope::xPlotStart +15] [expr $cursor::va2Pos -10] \
			-fill $scope::waveColorA \
			-tag va2Cursor \
			-text "VA2"
		
		#Bind Mouse clicks to the cursors
		[getScopePath].display bind va1Cursor <Button-1> {cursor::markYStart va1Cursor %y}
		[getScopePath].display bind va1Cursor <B1-Motion> {cursor::moveVcursor va1Cursor %y}
		
		[getScopePath].display bind va2Cursor <Button-1> {cursor::markYStart va2Cursor %y}
		[getScopePath].display bind va2Cursor <B1-Motion> {cursor::moveVcursor va2Cursor %y}
		
		[getScopePath].display bind vaMeasure <Button-1> {cursor::markXStart vaMeasure %x}
		[getScopePath].display bind vaMeasure <B1-Motion> {cursor::moveChAMeasurePos %x}
		[getScopePath].display bind vaMeasure <ButtonRelease-1> {cursor::measureVoltageCursors}
		
		set cursor::chACursorEnable 1
		measureVoltageCursors
	
	} else {
		#Remove the cursors
		[getScopePath].display delete va1Cursor
		[getScopePath].display delete va2Cursor
		[getScopePath].display delete vaMeasure
		
		set cursor::chACursorEnable 0
	}
}

proc cursor::toggleChBCursor {} {

	if {!$cursor::chBCursorEnable} {
		#Cursors for Channel B
		[getScopePath].display create line	\
			$scope::xPlotStart $cursor::vb1Pos \
			$scope::xPlotEnd $cursor::vb1Pos \
			-tag vb1Cursor \
			-fill $scope::waveColorB \
			-dash -
		[getScopePath].display create text \
			[expr $scope::xPlotStart + 15] [expr $cursor::vb1Pos - 10]\
			-fill $scope::waveColorB \
			-tag vb1Cursor \
			-text "VB1"
		[getScopePath].display create line \
			$scope::xPlotStart $cursor::vb2Pos\
			$scope::xPlotEnd $cursor::vb2Pos\
			-tag vb2Cursor \
			-fill $scope::waveColorB \
			-dash -
		[getScopePath].display create text \
			[expr $scope::xPlotStart +15] [expr $cursor::vb2Pos -10] \
			-fill $scope::waveColorB \
			-tag vb2Cursor \
			-text "VB2"
		
		#Bind Mouse clicks to the cursors
		[getScopePath].display bind vb1Cursor <Button-1> {cursor::markYStart vb1Cursor %y}
		[getScopePath].display bind vb1Cursor <B1-Motion> {cursor::moveVcursor vb1Cursor %y}
		
		[getScopePath].display bind vb2Cursor <Button-1> {cursor::markYStart vb2Cursor %y}
		[getScopePath].display bind vb2Cursor <B1-Motion> {cursor::moveVcursor vb2Cursor %y}
		
		[getScopePath].display bind vbMeasure <Button-1> {cursor::markXStart vbMeasure %x}
		[getScopePath].display bind vbMeasure <B1-Motion> {cursor::moveChBMeasurePos %x}
		[getScopePath].display bind vbMeasure <ButtonRelease-1> {cursor::measureVoltageCursors}
		
		set cursor::chBCursorEnable 1
		measureVoltageCursors
	
	} else {
		#Remove the cursors
		[getScopePath].display delete vb1Cursor
		[getScopePath].display delete vb2Cursor
		[getScopePath].display delete vbMeasure
		
		set cursor::chBCursorEnable 0
	}
}

proc cursor::moveVcursor { vTag vPos } {
	
	#Check to see if we have gone off of the screen
	if { $vPos > $scope::yPlotEnd} {set vPos $scope::yPlotEnd}
	if { $vPos < $scope::yPlotStart} { set vPos $scope::yPlotStart}

	#Move the cursor on the screen
	set dx 0
	set dy [expr $vPos - $cursor::yStart]
	set cursor::yStart $vPos
	
	#Save the new position
	switch $vTag {
		"va1Cursor" {
			set cursor::va1Pos $vPos
		} "va2Cursor" {
			set cursor::va2Pos $vPos
		} "vb1Cursor" {
			set cursor::vb1Pos $vPos
		} "vb2Cursor" {
			set cursor::vb2Pos $vPos
		}
	}
	
	#Move the cursor line on the screen
	[getScopePath].display move $vTag $dx $dy
	
	#Update cursor label displays
	measureVoltageCursors
}

proc cursor::measureVoltageCursors {} {

	if {$cursor::chACursorEnable} {
	
		if {$cursor::va1Pos > $cursor::va2Pos} {
			set topCursor $cursor::va2Pos
			set bottomCursor $cursor::va1Pos
		} else {
			set topCursor $cursor::va1Pos
			set bottomCursor $cursor::va2Pos
		}
		
		#Get the vertical sensitivity settings for channel A from the scope
		set pixelSize [expr {$scope::verticalBoxA*10/$scope::yPlotHeight}]
		
		#Calculate the voltage between the cursors
		set cursorVoltage [expr {($bottomCursor-$topCursor)*$pixelSize}]
		set cursorVoltage [cursor::formatAmplitude $cursorVoltage]
		
		[getScopePath].display delete vaMeasure
		
		#Determine which side of the arrows we will draw the voltage measurement
		if {$cursor::vaMeasurePos < $scope::xMid} {
			set textPos [expr {$cursor::vaMeasurePos+30}]
		} else {
			set textPos [expr {$cursor::vaMeasurePos-30}]
		}
		
		#Determine if we are drawing the voltage measure between the cursors
		#or outside of them
		if {[expr {$bottomCursor - $topCursor}] > 40} {
			[getScopePath].display create line	\
				$cursor::vaMeasurePos $bottomCursor	\
				$cursor::vaMeasurePos $topCursor	\
				-fill $scope::waveColorA		\
				-tag vaMeasure	\
				-arrow both
			[getScopePath].display create text \
				$textPos [expr {$bottomCursor-($bottomCursor-$topCursor)/2}]	\
				-text $cursorVoltage	\
				-font {-weight bold -size -12}	\
				-tag vaMeasure	\
				-fill $scope::waveColorA
		} else {
			[getScopePath].display create line	\
				$cursor::vaMeasurePos $topCursor	\
				$cursor::vaMeasurePos [expr {$topCursor-40}]	\
				-fill $scope::waveColorA		\
				-tag vaMeasure	\
				-arrow first
			[getScopePath].display create line	\
				$cursor::vaMeasurePos $bottomCursor	\
				$cursor::vaMeasurePos [expr {$bottomCursor+40}]	\
				-fill $scope::waveColorA		\
				-tag vaMeasure	\
				-arrow first
			#Determine where we will draw the time measure
			if {$topCursor > $scope::yMid} {
				[getScopePath].display create text \
					$textPos [expr {$topCursor-35}] \
					-text $cursorVoltage	\
					-font {-weight bold -size -12}	\
					-tag vaMeasure	\
					-fill $scope::waveColorA
			} else {
				[getScopePath].display create text \
					$textPos [expr {$bottomCursor + 35}]	\
					-text $cursorVoltage	\
					-font {-weight bold -size -12}	\
					-tag vaMeasure	\
					-fill $scope::waveColorA
			}
		
		
		}
		
	}
	
	if {$cursor::chBCursorEnable} {
	
		if {$cursor::vb1Pos > $cursor::vb2Pos} {
			set topCursor $cursor::vb2Pos
			set bottomCursor $cursor::vb1Pos
		} else {
			set topCursor $cursor::vb1Pos
			set bottomCursor $cursor::vb2Pos
		}
		
		#Get the vertical sensitivity settings for channel A from the scope
		set pixelSize [expr {$scope::verticalBoxB*10/$scope::yPlotHeight}]
		
		#Calculate the voltage between the cursors
		set cursorVoltage [expr {($bottomCursor-$topCursor)*$pixelSize}]
		set cursorVoltage [cursor::formatAmplitude $cursorVoltage]
		
		[getScopePath].display delete vbMeasure
		
		#Determine which side of the arrows we will draw the voltage measurement
		if {$cursor::vbMeasurePos < $scope::xMid} {
			set textPos [expr {$cursor::vbMeasurePos+30}]
		} else {
			set textPos [expr {$cursor::vbMeasurePos-30}]
		}
		
		#Determine if we are drawing the voltage measure between the cursors
		#or outside of them
		if {[expr {$bottomCursor - $topCursor}] > 40} {
			[getScopePath].display create line	\
				$cursor::vbMeasurePos $bottomCursor	\
				$cursor::vbMeasurePos $topCursor	\
				-fill $scope::waveColorB		\
				-tag vbMeasure	\
				-arrow both
			[getScopePath].display create text \
				$textPos [expr {$bottomCursor-($bottomCursor-$topCursor)/2}]	\
				-text $cursorVoltage	\
				-font {-weight bold -size -12}	\
				-tag vbMeasure	\
				-fill $scope::waveColorB
		} else {
			[getScopePath].display create line	\
				$cursor::vbMeasurePos $topCursor	\
				$cursor::vbMeasurePos [expr {$topCursor-40}]	\
				-fill $scope::waveColorB		\
				-tag vbMeasure	\
				-arrow first
			[getScopePath].display create line	\
				$cursor::vbMeasurePos $bottomCursor	\
				$cursor::vbMeasurePos [expr {$bottomCursor+40}]	\
				-fill $scope::waveColorB		\
				-tag vbMeasure	\
				-arrow first
			#Determine where we will draw the time measure
			if {$topCursor > $scope::yMid} {
				[getScopePath].display create text \
					$textPos [expr {$topCursor-35}] \
					-text $cursorVoltage	\
					-font {-weight bold -size -12}	\
					-tag vbMeasure	\
					-fill $scope::waveColorB
			} else {
				[getScopePath].display create text \
					$textPos [expr {$bottomCursor + 35}]	\
					-text $cursorVoltage	\
					-font {-weight bold -size -12}	\
					-tag vbMeasure	\
					-fill $scope::waveColorB
			}
		
		
		}
		
	}

}

#Format Amplitude
#-------------------
#This procedure takes a number representing a voltage and
#formats it into a string with the proper units.
proc cursor::formatAmplitude {voltage} {

	if {[expr {abs($voltage)}] < 1.0} {
		set voltage [format "%3.0f" [expr $voltage/1E-3]]
		set voltage "$voltage mV"
	} else {
		set voltage [format "%2.2f" $voltage]
		set voltage "$voltage V"
	}

	return $voltage
}

proc cursor::formatFrequency {freq} {
	
	if {$freq > 1E6} {
		set freq [format "%3.1f" [expr $freq/1E6]]
		set freq "$freq MHz"
	} elseif { $freq > 1E3} {
		set freq [format "%3.1f" [expr $freq/1E3]]
		set freq "$freq kHz"
	} else {
		set freq [format "%3.1f" $freq]
		set freq "$freq Hz"
	}
	return $freq
	
}

proc cursor::moveChAMeasurePos { xPos } {

	#Make sure we haven't gone off the screen
	if { $xPos > $scope::xPlotEnd } { set xPos $scope::xPlotEnd }
	if { $xPos < $scope::xPlotStart } { set xPos $scope::xPlotStart }
	
	#Move the cursor
	set dy 0
	set dx [expr $xPos - $cursor::xStart]
	set cursor::xStart $xPos
	set cursor::vaMeasurePos $xPos
	[getScopePath].display move vaMeasure $dx $dy
	
}

proc cursor::moveChBMeasurePos { xPos } {

	#Make sure we haven't gone off the screen
	if { $xPos > $scope::xPlotEnd } { set xPos $scope::xPlotEnd }
	if { $xPos < $scope::xPlotStart } { set xPos $scope::xPlotStart }
	
	#Move the cursor
	set dy 0
	set dx [expr $xPos - $cursor::xStart]
	set cursor::xStart $xPos
	set cursor::vbMeasurePos $xPos
	[getScopePath].display move vbMeasure $dx $dy
	
}

proc cursor::screenXToSampleIndex {x} {

	#Get the timebase setting
	set timebaseSetting [lindex $scope::timebaseValues $scope::timebaseIndex]
	
	#Calculate the sampling frequency
	set sampleRate [lindex $scope::samplingRates $scope::timebaseIndex]
	set sampleRate [expr {$scope::masterSampleRate/pow(2,$sampleRate)}]

	return [expr {round(1024-5-$cursor::sampleOffset-($sampleRate*$timebaseSetting*10/$scope::xPlotWidth)*($cursor::timePos-$x))}]
	

}

#Create a pop-up menu for the cursors
menu [getScopePath].display.popup -tearoff 0
[getScopePath].display.popup add command	\
	-label "Toggle Time Cursors"	\
	-command cursor::toggleTimeCursors
[getScopePath].display.popup add command	\
	-label "Toggle Channel A Cursors"	\
	-command cursor::toggleChACursor
[getScopePath].display.popup add command	\
	-label "Toggle Channel B Cursors"	\
	-command cursor::toggleChBCursor
[getScopePath].display.popup add command	\
	-label "Toggle Scope Grid"	\
	-command scope::toggleGrid
if {$::osType == "Darwin"} {
	bind [getScopePath].display <Button-2> {+tk_popup [getScopePath].display.popup %X %Y}
} else {
	bind [getScopePath].display <Button-3> {+tk_popup [getScopePath].display.popup %X %Y}
}
#Add the cursor commands to the view menu
.menubar.scopeView.viewMenu add separator
.menubar.scopeView.viewMenu add command	\
	-label "Toggle Time Cursors"	\
	-command cursor::toggleTimeCursors
.menubar.scopeView.viewMenu add command	\
	-label "Toggle Channel A Cursors"	\
	-command cursor::toggleChACursor
.menubar.scopeView.viewMenu add command	\
	-label "Toggle Channel B Cursors"	\
	-command cursor::toggleChBCursor
.menubar.scopeView.viewMenu add command	\
	-label "Toggle Scope Grid"	\
	-command scope::toggleGrid
.menubar.scopeView.viewMenu add separator
#Add the color preferences command to the view menu
.menubar.scopeView.viewMenu add command	\
	-label "Color Preferences"	\
	-command scope::showColorOptions




