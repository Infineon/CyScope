
package provide digio 1.0
package require Img
package require BWidget

namespace eval digio {

#---=== Digital I/O Global Variables ===---
variable digout
set digout(6) 0
set digout(5) 0
set digout(4) 0
set digout(3) 0
set digout(2) 0
set digout(1) 0
set digout(0) 0

#Digital I/O Images:
#GJL
# Bit 6 is the LED, bit 5 is the Button (Switch)
set bitOffImages(6) [image create photo -file "$images/LEDOff.gif"]
set bitOnImages(6) [image create photo -file "$images/LEDOn.gif"]
set bitOffImages(5) [image create photo -file "$images/SWOff.gif"]
set bitOnImages(5) [image create photo -file "$images/SWOn.gif"]
set bitOffImages(4) [image create photo -file "$images/Bit4Off.gif"]
set bitOnImages(4) [image create photo -file "$images/Bit4On.gif"]
set bitOffImages(3) [image create photo -file "$images/Bit3Off.gif"]
set bitOnImages(3) [image create photo -file "$images/Bit3On.gif"]
set bitOffImages(2) [image create photo -file "$images/Bit2Off.gif"]
set bitOnImages(2) [image create photo -file "$images/Bit2On.gif"]
set bitOffImages(1) [image create photo -file "$images/Bit1Off.gif"]
set bitOnImages(1) [image create photo -file "$images/Bit1On.gif"]
set bitOffImages(0) [image create photo -file "$images/Bit0Off.gif"]
set bitOnImages(0) [image create photo -file "$images/Bit0On.gif"]

#Frequency Settings for PWM
set freqs {"72 kHz" "36 kHz" "9 kHz" "4.5 kHz" "1.125 kHz" "564 Hz" "281 Hz" "141 Hz" "70 Hz" "35 Hz"}
set pwmDuty 0

#Interrupt Images
set IntOnImage [image create photo -file "$images/Inton.gif"]
set IntOffImage [image create photo -file "$images/Intoff.gif"]

#Interrupt Modes
set intModes {"Disable" "Rising" "Falling" "High" "Low"}
set intMode [lindex $intModes 0]

#Floating/Docked State
set state docked

#Input value
set inValue 0

#---=== Export Public Procedures ===---
namespace export setDigioPath
namespace export getDigioPath
namespace export buildDigio

}

#---=== Procedures ===---
proc ::digio::setDigioPath {digioPath} {
	variable digio
	
	set digio(path) $digioPath
}

proc ::digio::getDigioPath {} {
	variable digio
	
	return $digio(path)
}

proc ::digio::buildDigio {} {

	set digioPath [getDigioPath]
	
	#Frame for Digital I/O Controls
	frame $digioPath
	
	#Digital Ouptut Controls
	frame $digioPath.out	\
		-relief raised	\
		-borderwidth 2

	label $digioPath.out.title	\
		-text "Digital Outputs"	\
		-font {-weight bold -size -14}

	for {set i 0} {$i < 7} { incr i} {
		button $digioPath.out.$i	\
			-image $::digio::bitOffImages($i)	\
			-command "::digio::toggleOutBit $i"
	}

	grid $digioPath.out.title -row 0 -column 0 -columnspan 4 -sticky w
	#GJL
        # Bit 6 is the LED	
	grid $digioPath.out.6 -row 1 -column 1 -pady 9
	grid $digioPath.out.4 -row 1 -column 2
	grid $digioPath.out.3 -row 1 -column 3
	grid $digioPath.out.2 -row 1 -column 4
	grid $digioPath.out.1 -row 1 -column 5
	grid $digioPath.out.0 -row 1 -column 6
	
	#Digital Input Indicators
	frame $digioPath.in	\
		-relief raised	\
		-borderwidth 2
		
	label $digioPath.in.title	\
		-text "Digital Inputs"	\
		-font {-weight bold -size -14}
		
	for {set i 0} {$i <6} {incr i} {
		label $digioPath.in.$i	\
			-image $::digio::bitOffImages($i)
	}

	grid $digioPath.in.title -row 0 -column 0 -columnspan 4 -sticky w
        # GJL	
        # Bit 5 is the button	
        grid $digioPath.in.5 -row 1 -column 1 -pady 9
	grid $digioPath.in.4 -row 1 -column 2
	grid $digioPath.in.3 -row 1 -column 3
	grid $digioPath.in.2 -row 1 -column 4
	grid $digioPath.in.1 -row 1 -column 5
	grid $digioPath.in.0 -row 1 -column 6

	#PWM Control
	frame $digioPath.pwm	\
		-relief raised	\
		-borderwidth 2
		
	label $digioPath.pwm.title	\
		-text "PWM"	\
		-font {-weight bold -size -14}
		
	canvas $digioPath.pwm.display	\
		-width 66	\
		-height 15	\
		-background white
		
	label $digioPath.pwm.value	\
		-textvariable pwmDuty	\
		-width 3

	scale $digioPath.pwm.control	\
		-from 0		\
		-to 100			\
		-variable digio::pwmDuty	\
		-orient horizontal	\
		-showvalue 0	\
		-length 120	\
		-tickinterval 50	\
		-resolution 1	\
		-command ::digio::updatePWM
	if {$::osType!="Darwin"} {	
		ComboBox $digioPath.pwm.freq	\
			-values $digio::freqs	\
			-width 12	\
			-textvariable ::digio::pwmFreq	\
			-bwlistbox 1	\
			-hottrack 1		\
			-editable 0		\
			-modifycmd ::digio::selectFreq
		$digioPath.pwm.freq setvalue first
	} else {
		spinbox $digioPath.pwm.freq	\
			-values $digio::freqs	\
			-width 12	\
			-textvariable ::digio::pwmFreq	\
			-state readonly	\
			-readonlybackground white	\
			-command ::digio::selectFreq	\
			-wrap on
	}
	
	grid $digioPath.pwm.title -row 0 -column 0 -sticky w
	grid $digioPath.pwm.display -row 0 -column 1 -sticky we
	grid $digioPath.pwm.value -row 0 -column 2 -sticky e
	grid $digioPath.pwm.control -row 1 -column 0 -columnspan 3
	grid $digioPath.pwm.freq -row 2 -column 0 -columnspan 3
	
	#Interrupt Indicator
	frame $digioPath.int	\
		-relief raised	\
		-borderwidth 2
		
	label $digioPath.int.title	\
		-text "Interrupt"		\
		-font {-weight bold -size -14}
	
	button $digioPath.int.status	\
		-image $digio::IntOffImage	\
		-command ::digio::selectIntMode
		
	ComboBox $digioPath.int.mode	\
		-values $digio::intModes	\
		-width 8	\
		-textvariable ::digio::intMode	\
		-bwlistbox 1	\
		-hottrack 1		\
		-editable 0		\
		-modifycmd ::digio::selectIntMode
	$digioPath.int.mode setvalue first	
	
	grid $digioPath.int.title -row 0 -column 0
	grid $digioPath.int.status -row 1 -column 0 -pady 1
	grid $digioPath.int.mode -row 2 -column 0 -pady 1

	grid $digioPath.out -row 0 -column 0 -ipady 3 -padx 2
	grid $digioPath.in -row 0 -column 1  -ipady 3 -padx 2
	grid $digioPath.int -row 0 -column 2 -padx 2
	grid $digioPath.pwm -row 0 -column 3 -padx 2

}

# Toggle Output Bit
#----------------------
#This procedure is called when the user clicks on an output bit to change it's state.
proc ::digio::toggleOutBit {bitNum} {
	variable digout
	variable bitOnImages
	variable bitOffImages
	
	set digPath [getDigioPath]

	if {$digout($bitNum)==1} {
		set digout($bitNum) 0
		$digPath.out.$bitNum configure -image $bitOffImages($bitNum)
	} else {
		set digout($bitNum) 1
		$digPath.out.$bitNum configure -image $bitOnImages($bitNum)
	}
	
	::digio::updateDigio

}

# Update Digital I/O Hardware Registers
#-----------------------------------------------
# This procedures sends commands to the instrument to update the digital
# I/O registers.
proc ::digio::updateDigio {} {
	variable digout
		
	set digReg 0
	
	set digReg [expr {$digReg+$digout(0)*1}]
	set digReg [expr {$digReg+$digout(1)*2}]
	set digReg [expr {$digReg+$digout(2)*4}]
	set digReg [expr {$digReg+$digout(3)*8}]
	set digReg [expr {$digReg+$digout(4)*16}]
        # GJL        
        # LED is bit 6
	set digReg [expr {$digReg+$digout(6)*64}]
	
	sendCommand "D O $digReg"
}

# Update PWM Settings
#-------------------------
# This procedure services the PWM slider.  It updates the PWM display and
# sends commands to the hardware to update the PWM output.
proc ::digio::updatePWM {sliderArg} {

	#Calculate the duty cycle (8-bit)
	set dutyCycle [expr {round($sliderArg/100.0*255)}]
	
	sendCommand "D D $dutyCycle"
	
	#Update the PWM Display
	set digioPath [getDigioPath]
	
	$digioPath.pwm.display delete pwmTag
	
	set plotData {}
	
	puts $sliderArg

	lappend plotData 3
	lappend plotData 12

	for {set i 0} {$i < 3} {incr i} {
		if {$sliderArg > 0} {
			lappend plotData [expr {3+20*$i}]
			lappend plotData 2
			set temp [expr {($sliderArg/100.0)*20+(3+20*$i)}]
			lappend plotData $temp
			lappend plotData 2
			if {$sliderArg < 100} {
				lappend plotData $temp
				lappend plotData 12
				lappend plotData [expr {3+20*($i+1)}]
				lappend plotData 12
			}
		}
	}
	lappend plotData 63
	lappend plotData 12
	
	$digioPath.pwm.display create line	\
		$plotData	\
		-tag pwmTag	\
		-fill black		\
		-width 1

}

proc ::digio::readDigIn {} {

	sendCommand "D I "


}

proc ::digio::updateDigIn {value} {

	set digioPath [getDigioPath]
	
	set digio::inValue $value
	
	for {set i 5} {$i >=0} {set i [expr {$i-1}]} {
		if {$value >= [expr {pow(2,$i)}]} {
			$digioPath.in.$i configure -image $::digio::bitOnImages($i)
		} else {
			$digioPath.in.$i configure -image $::digio::bitOffImages($i)
		}
		set value [expr {$value%int(pow(2,$i))}]
	}
	
	
		
}

proc ::digio::selectFreq {} {

	switch $::digio::pwmFreq {
		"72 kHz" {
			sendCommand "D F 0"
		} "36 kHz" {
			sendCommand "D F 1"
		} "9 kHz" {
			sendCommand "D F 2"
		} "4.5 kHz" {
			sendCommand "D F 3"
		} "1.125 kHz" {
			sendCommand "D F 4"
		} "564 Hz" {
			sendCommand "D F 5"
		} "281 Hz" {
			sendCommand "D F 6"
		} "141 Hz" {
			sendCommand "D F 7"
		} "70 Hz" {
			sendCommand "D F 8"
		} "35 Hz" {
			sendCommand "D F 9"
		}
	}

}

proc ::digio::selectIntMode {} {

	set digioPath [getDigioPath]

	$digioPath.int.status configure -image $digio::IntOffImage

	switch $digio::intMode {
		"Rising" {
			sendCommand "D ! R"
		} "Falling" {
			sendCommand "D ! F"
		} "High" {
			sendCommand "D ! H"
		} "Low" {
			sendCommand "D ! L"
		} "Disable" {
			sendCommand "D ! D"
		}
	}

}

proc ::digio::setInt {} {
	
	set digioPath [getDigioPath]

	$digioPath.int.status configure -image $digio::IntOnImage
}

proc digio::floatDigio {} {
	variable digout
	variable bitOnImages
	variable bitOffImages
	
	#Save the interrupt state
	set digioPath [getDigioPath]	
	set interruptState [$digioPath.int.status cget -image]
	#Save the pwm frequency
	set pwmFreq [$digioPath.pwm.freq get]
	#Save the interrupt mode
	set intMode [$digioPath.int.mode get]

	if {$digio::state=="float"} {
		#Hide the digio controls that are attached to the main window
		grid remove .d
	
		#Create a floating window for the digital I/O controls
		toplevel .dFloat
		wm title .dFloat "Digital I/O Controls"
		wm resizable .dFloat 0 0
		digio::setDigioPath .dFloat.d
		digio::buildDigio
		grid .dFloat.d -row 0
		bind .dFloat <Destroy> {set digio::state docked;digio::floatDigio}
		
	} else {
	
		if {[winfo exists .dFloat]} {
			bind .dFloat <Destroy> {destroy .dFloat}
			destroy .dFloat 
		}
		grid .d
		digio::setDigioPath .d
		
	}
	
	#Restore the state of the input and output indicators
	set digioPath [getDigioPath]

	for {set bitNum 0} {$bitNum < 6} {incr bitNum} {
		if {$digout($bitNum)==1} {
			$digioPath.out.$bitNum configure -image $bitOnImages($bitNum)
		} else {
			$digioPath.out.$bitNum configure -image $bitOffImages($bitNum)
		}
	}
	
	digio::updateDigIn $digio::inValue
	
	#Restore the state of the interrupt indicator
	$digioPath.int.status configure -image $interruptState
	#Restore the PWM frequency
	$digioPath.pwm.freq configure -text $pwmFreq
	digio::selectFreq
	#Restore the interrupt mode
	$digioPath.int.mode configure -text $intMode
	::digio::selectIntMode
	
}