
package provide waveform 1.0
package require Img

namespace eval wave {

#---=== Waveform Global Variables ===---
# KEES Used to be 0.01
set minFrequencyLimit 3.2
# KEES used to be 3000000
set maxFrequencyLimit 10000
# KEES Used to be 0.01
set minFrequency 3.2
# KEES used to be 3000000
set maxFrequency 10000
set sliderRange 350
set ampSliderLength 395
# GJL
set offsetSliderLength 395
set ddsResolution 0.09313225746
set defaultFrequency 100.0
set frequencyDisplay $defaultFrequency
set waveFrequency $defaultFrequency
set waveFile "./Waves/sine.dat"

#Waveform Images:
set freqImage [image create photo -file "$images/freqImage.gif"]
set ampImage [image create photo -file "$images/ampImage.gif"]
set offsetImage [image create photo -file "$images/offsetImage.gif"]

#---=== Export Public Procedures ===---
namespace export buildWave
namespace export setWavePath
namespace export getWavePath

}

#---=== Procedures ===---
proc ::wave::setWavePath {wavePath} {
	variable wave
	
	set wave(path) $wavePath
}

proc ::wave::getWavePath {} {
	variable wave
	
	return $wave(path)
}

proc ::wave::buildWave {} {

	set wavePath [getWavePath]
	
	#Frame for Waveform Generator Controls
	frame $wavePath	\
		-relief raised	\
		-borderwidth 2

	#Waveform frequency controls
	frame $wavePath.freq	\
		-relief groove	\
		-borderwidth 2
		
	label $wavePath.title	\
		-text "Waveform Generator"	\
		-font {-weight bold -size -16}
		
	scale $wavePath.freq.freqSlider	\
		-from $::wave::sliderRange	\
		-to 1		\
		-variable ::wave::frequencyPosition	\
		-orient vertical	\
		-tickinterval 0	\
		-resolution 1	\
		-showvalue 0	\
		-length $::wave::sliderRange	\
		-command ::wave::adjustFrequency
		
	label $wavePath.freq.title	\
		-image $::wave::freqImage

	button $wavePath.freq.topValue	\
		-textvariable ::wave::maxFrequency	\
		-width 8	\
		-command {::wave::setMaxFrequency}
		
	button $wavePath.freq.bottomValue	\
		-textvariable ::wave::minFrequency	\
		-width 8	\
		-command {::wave::setMinFrequency}

	grid $wavePath.freq.title -row 0 -column 0 -sticky n
	grid $wavePath.freq.topValue -row 1 -column 0
	grid $wavePath.freq.freqSlider -row 2 -column 0
	grid $wavePath.freq.bottomValue -row 3 -column 0
	
	#Waveform Amplitude Controls
	frame $wavePath.amp	\
		-relief groove	\
		-borderwidth 2
		
	scale $wavePath.amp.ampSlider	\
		-from 100		\
		-to 0			\
		-variable ::wave::amplitude	\
		-orient vertical	\
		-showvalue 1	\
		-length $wave::ampSliderLength	\
		-tickinterval 5	\
		-resolution 1	\
		-command ::wave::adjustAmplitude
		
	label $wavePath.amp.title	\
		-image $::wave::ampImage

	grid $wavePath.amp.title -row 0 -column 0 -sticky n
	grid $wavePath.amp.ampSlider -row 1 -column 0 -sticky n

	# GJL
	#Waveform Offset Controls
	frame $wavePath.offset	\
		-relief groove	\
		-borderwidth 2
		
	scale $wavePath.offset.offsetSlider	\
		-from 100		\
		-to 0			\
		-variable ::wave::offset	\
		-orient vertical	\
		-showvalue 1	\
		-length $wave::offsetSliderLength	\
		-tickinterval 5	\
		-resolution 1	\
		-command ::wave::adjustOffset
		
	label $wavePath.offset.title	\
		-image $::wave::offsetImage

	grid $wavePath.offset.title -row 0 -column 0 -sticky n
	grid $wavePath.offset.offsetSlider -row 1 -column 0 -sticky n

	#Waveform selection controls
	frame $wavePath.wave	\
		-relief groove	\
		-borderwidth 2

	canvas $wavePath.wave.waveDisplay	\
		-width 100	\
		-height 100	\
		-background white	\
		-borderwidth 2
		
	button $wavePath.wave.freqDisplay	\
		-relief sunken	\
		-borderwidth 3	\
		-textvariable ::wave::frequencyDisplay	\
		-font {-weight bold -size -14}	\
		-width 10	\
		-background black	\
		-foreground red	\
		-command {::wave::setFrequency}
		
	button $wavePath.wave.sine	\
		-text "Sine"		\
		-command {::wave::programWaveform ./Waves/sine.dat}
		
	button $wavePath.wave.square	\
		-text "Square"	\
		-command {::wave::programWaveform ./Waves/square.dat}
		
	button $wavePath.wave.triangle	\
		-text "Triangle"	\
		-command {::wave::programWaveform ./Waves/triangle.dat}
		
	button $wavePath.wave.sawtooth	\
		-text "Sawtooth"	\
		-command {::wave::programWaveform ./Waves/sawtooth.dat}
		
	button $wavePath.wave.custom	\
		-text "Custom"	\
		-command {::wave::openWaveform}
		
	button $wavePath.wave.noise	\
		-text "Noise"	\
		-command {::wave::enableNoise}

	grid $wavePath.wave.waveDisplay -row 0 -column 0
	grid $wavePath.wave.freqDisplay -row 1 -column 0
	grid $wavePath.wave.sine -row 2 -column 0 -sticky we
	grid $wavePath.wave.square -row 3 -column 0 -sticky we
	grid $wavePath.wave.triangle -row 4 -column 0 -sticky we
	grid $wavePath.wave.sawtooth -row 5 -column 0 -sticky we
	grid $wavePath.wave.custom -row 6 -column 0 -sticky we
	grid $wavePath.wave.noise -row 7 -column 0 -sticky we

	#Sweep controls 
	frame $wavePath.sweep	\
		-relief groove	\
		-borderwidth 2
		
	label $wavePath.sweep.modeTitle	\
		-text "Sweep Mode:"
		
	radiobutton $wavePath.sweep.logMode	\
		-text "Logarithmic"	\
		-value log	\
		-variable ::wave::sliderMode
		
	#Select Logarithmic Mode by Default
	$wavePath.sweep.logMode select
		
	radiobutton $wavePath.sweep.linMode	\
		-text "Linear"	\
		-value linear	\
		-variable ::wave::sliderMode
		
	grid $wavePath.sweep.modeTitle -row 0 -column 0 -sticky nw
	grid $wavePath.sweep.logMode -row 1 -column 0 -sticky nw
	grid $wavePath.sweep.linMode -row 2 -column 0 -sticky nw

	#Waveform Generator Controls
	grid $wavePath.title  -row 0 -column 0 -columnspan 3 -sticky w
	grid $wavePath.amp    -row 1 -column 0 -sticky n -rowspan 2 -padx 5
	grid $wavePath.offset -row 1 -column 1 -sticky n -rowspan 2 -padx 5
	grid $wavePath.freq   -row 1 -column 2 -rowspan 2 -padx 5
	grid $wavePath.wave   -row 1 -column 3 -sticky n -padx 5
	grid $wavePath.sweep  -row 2 -column 3 -sticky n -padx 5

}

# Adjust Waveform Generator Frequency
#--------------------------------------------
# This procedure is a service routine for the waveform generator frequency
# slider control.  It converts the slider position into a frequency, calls the
# procedure to update the hardware, and updates the frequency display.
proc ::wave::adjustFrequency {sliderArg} {
	variable minFrequency
	variable maxFrequency
	variable sliderRange
	variable frequencyDisplay
	variable sliderMode
	variable waveFrequency
	
	
	if {$sliderMode == "log"} {
		#Logarithmic interpretation of slider position
		set logMin [expr {log10($minFrequency)}]
		set logMax [expr {log10($maxFrequency)}]
		set b $logMin
		set m [expr {($logMax-$logMin)/($sliderRange-1)}]
		set y [expr {$m*($sliderArg-1)+$b}]
		set frequency [expr {pow(10,$y)}]
		
	} else {
		#Linear interpretation of slider position
		set b $minFrequency
		set m [expr {($maxFrequency-$minFrequency)/($sliderRange-1)}]
		set y [expr {$m*($sliderArg-1)+$b}]
		set frequency $y
	}
	
	#Round to the nearest tenth of a hertz
	set waveFrequency [format "%.1f" $frequency]
	
	#Update the hardware with the new frequency
	::wave::sendFrequency $waveFrequency
	
	#Update the frequency display
	set frequencyDisplay "$waveFrequency Hz"
	
}

# Send Frequency Setting to Hardware
#-------------------------------------------
# This procedure accepts a frequency value, converts it to a four byte number,
# and sends it to the hardware.
proc ::wave::sendFrequency {freq} {
	variable ddsResolution
	
	#Calculate the phase integer
	set freqOutput [expr {round($freq/$ddsResolution)}]

	#Break the phase integer into a four byte number
	set byte3 [expr {round(floor($freqOutput/pow(2,24)))}]
	set freqOutput [expr {$freqOutput%round(pow(2,24))}]
	
	set byte2 [expr {round(floor($freqOutput/pow(2,16)))}]
	set freqOutput [expr {$freqOutput%round(pow(2,16))}]
	
	set byte1 [expr {round(floor($freqOutput/pow(2,8)))}]
	set freqOutput [expr {$freqOutput%round(pow(2,8))}]
	
	set byte0 $freqOutput
	
	sendCommand "W F $byte3 $byte2 $byte1 $byte0"
	
}

# Adjust Waveform Amplitude
#--------------------------------
# This is a service procedure for the amplitude slider control.  It converts
# the slider argument into an amplitude value and sends it to the hardware.
proc ::wave::adjustAmplitude {sliderArg} {
	
	set amplitude [expr {round(255.0*($sliderArg/100.0))}]
	
	sendCommand "W A $amplitude"
}

# Adjust Waveform Offset
#--------------------------------
# This is a service procedure for the offset slider control.  It converts
# the slider argument into an offset value and sends it to the hardware.
proc ::wave::adjustOffset {sliderArg} {
	
	set offset [expr {round(255.0*($sliderArg/100.0))}]
	
	sendCommand "W O $offset"
}

# Manually Set Frequency
#----------------------------
# This procedure is called when the user wants to manually set the waveform generator
# output frequency.  It presents the user with a dialog box where they can enter
# the desired output frequency.
proc ::wave::setFrequency {} {
	variable minFrequencyLimit
	variable maxFrequencyLimit
	variable frequencyDisplay
	variable waveFrequency
	
	#Dialog box for user to enter the new frequency
	set newFreq [Dialog_Prompt newF "New Frequency:"]
	
	if {$newFreq == ""} {return}
	
	#Make sure that we got a valid frequency setting
	if { [string is double -strict $newFreq] } {
		if { $newFreq >= $minFrequencyLimit && $newFreq <= $maxFrequencyLimit} {
			sendFrequency $newFreq
			set waveFrequency [format "%.1f" $newFreq]
			set frequencyDisplay "$waveFrequency Hz"
		} else {
			tk_messageBox	\
			-title "Invalid Frequency"	\
			-default ok		\
			-message "Frequency out of range"	\
			-type ok			\
			-icon warning
		}
	} else {
		
	
		tk_messageBox	\
			-title "Invalid Frequency"	\
			-default ok		\
			-message "Frequency must be a number\nbetween $minFrequencyLimit and $maxFrequencyLimit"	\
			-type ok			\
			-icon warning
		return
	}
	

}

# Open Waveform
#-------------------
# This procedure is called when the user wants to program a custom waveform
# into the device.
proc ::wave::openWaveform {} {
	
	#Get the name of the file
	set fileName [tk_getOpenFile]
	
	#Make sure the user didn't hit cancel
	if {$fileName == ""} {return}
	
	programWaveform $fileName
}

# Program Waveform
#----------------------
# This procedure takes the waveform data stored in "fileName" and sends it to the
# instrument.
proc ::wave::programWaveform {fileName} {

	puts "Programming $fileName"

	#Attempt to open the input file
	if [catch {open $fileName r} channel] {
		tk_messageBox \
			-message "Cannot open $fileName" \
			-default ok \
			-icon error \
			-title "File Error"
		puts stderr "Cannot open $fileName"
	} else {
	
		set wavePath [getWavePath]
		
		$wavePath.wave.sine configure -state disabled
		$wavePath.wave.square configure -state disabled
		$wavePath.wave.triangle configure -state disabled
		$wavePath.wave.sawtooth configure -state disabled
		$wavePath.wave.custom configure -state disabled
		$wavePath.wave.noise configure -state disabled
		
		set sampleIndex 0
		set samples [list]
	
		while { [gets $channel line] >= 0} {
			lappend samples $line
			incr sampleIndex
		}
		close $channel
		
		puts "Samples $samples"

		#Check number of samples here
		
		set sampleIndex 0
		while {$sampleIndex < 256} {
			set sampleValue [lindex $samples $sampleIndex]
			
			sendCommand "W S $sampleIndex $sampleValue"
			incr sampleIndex

			update
		}
		
		sendCommand "W P"
		
		#Update the waveform display
		set plotData {}
		set scaleFactor [expr {266.0/90}]
		#Get a few samples from the "previous" cycle
		for {set i 250} {$i < 256} {incr i} {
			#X-Coordinate
			lappend plotData [expr {5+($i-250)/$scaleFactor}]
			#Y-Coordinate
			set sample [lindex $samples $i]
			lappend plotData [expr {52.0+(($sample-128)/$scaleFactor)}]
		}
		#Draw one complete cycle of the waveform
		for {set i 0} {$i <256} {incr i} {
			#X-Coordinate
			lappend plotData [expr {5+(5/$scaleFactor)+$i/$scaleFactor}]
			#Y-Coordinate
			set sample [lindex $samples $i]
			lappend plotData [expr {52.0+(($sample-128)/$scaleFactor)}]
		}
		#Get a few samples from the "next" cycle
		for {set j 0} {$j < 6} {incr j} {
			#X-Coordinate
			lappend plotData [expr {5+5/$scaleFactor+$j/$scaleFactor+$i/$scaleFactor}]
			#Y-Coordinate
			set sample [lindex $samples $j]
			lappend plotData [expr {52.0+(($sample-128)/$scaleFactor)}]
		}

		::wave::updateDisplay $plotData
		
		set wave::waveFile $fileName
	}

	#Make sure the waveform controls are enabled
	[getWavePath].freq.freqSlider configure -state normal
	
	#Make sure we enable the waveform output and disable noise output
	sendCommand "W W"
	
	$wavePath.wave.sine configure -state normal
	$wavePath.wave.square configure -state normal
	$wavePath.wave.triangle configure -state normal
	$wavePath.wave.sawtooth configure -state normal
	$wavePath.wave.custom configure -state normal
	$wavePath.wave.noise configure -state normal

}

# Update Waveform Display
#-------------------------------
#This procedure updates the waveform display by drawing the waveform supplied
#by "plotData" on the canvas.  Data in the plotData array should be x y pairs.
proc ::wave::updateDisplay {plotData} {
	
	set wavePath [getWavePath]
	
	$wavePath.wave.waveDisplay delete waveDisplayData
	
	$wavePath.wave.waveDisplay create line	\
		$plotData	\
		-tag waveDisplayData	\
		-fill red
	
}

#Set Maximum Frequency
#---------------
#This procedure prompts the user for a new max frequency value.
#The frequency supplied by the user is checked to ensure that
#it is a valid number and a valid frequency setting.
proc ::wave::setMaxFrequency {} {
	
	set newMax [Dialog_Prompt setMax "New Maximum Frequency:"]
	
	if {$newMax == ""} { return }
	
	if { [string is double -strict $newMax]} {
		if {$newMax > $wave::minFrequency && $newMax <= $wave::maxFrequencyLimit} {
			set wave::maxFrequency [format "%.1f" $newMax]
			set wavePath [getWavePath]
			::wave::adjustFrequency [$wavePath.freq.freqSlider get]
		} else {
			tk_messageBox	\
			-title "Invalid Frequency"	\
			-default ok		\
			-message "Invalid Frequency.\nMax frequency is $wave::maxFrequencyLimit."	\
			-type ok			\
			-icon warning
		}
	} else {
		tk_messageBox	\
			-title "Invalid Frequency"	\
			-default ok		\
			-message "Frequency must be a number\nbetween $wave::minFrequencyLimit and $wave::maxFrequencyLimit."	\
			-type ok			\
			-icon warning
		return
	}
}

#Set Minimum Frequency
#---------------
#This procedure prompts the user for a new min frequency value.
#The frequency supplied by the user is checked to ensure that
#it is a valid number and a valid frequency setting.
proc ::wave::setMinFrequency {} {

	set newMin [Dialog_Prompt setMin "New Minimum Frequency:"]
	
	if {$newMin == ""} {return}
	
	if { [string is double -strict $newMin] } {
		if { $newMin < $wave::maxFrequency && $newMin >= $wave::minFrequencyLimit} {
			set wave::minFrequency [format "%.1f" $newMin]
			set wavePath [getWavePath]
			::wave::adjustFrequency [$wavePath.freq.freqSlider get]
		} else {
			tk_messageBox	\
			-title "Invalid Frequency"	\
			-default ok		\
			-message "Invalid Frequency.\nMin frequency is $wave::minFrequencyLimit."	\
			-type ok			\
			-icon warning
		}
	} else {
		tk_messageBox	\
			-title "Invalid Frequency"	\
			-default ok		\
			-message "Frequency must be a number\nbetween $wave::minFrequencyLimit and $wave::maxFrequencyLimit."	\
			-type ok			\
			-icon warning
		return
	}
}

proc ::wave::enableNoise {} {

	set wavePath [getWavePath]
	# KEES used to be -state disabled
	#$wavePath.freq.freqSlider configure -state enabled
	
	sendCommand "W N"
	
	for {set i 0} {$i <=100} {incr i} {
		#X-Coordinate
		lappend plotData $i
		#Y-Coordinate
		lappend plotData [expr {52.0+80*(rand()-0.5)}]
	}

	::wave::updateDisplay $plotData
	
	set wave::waveFile "noise"

}