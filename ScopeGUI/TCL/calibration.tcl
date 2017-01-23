#File: calibration.tcl
#Syscomp USB Oscilloscope GUI
#Scope Vertical Calibration Procedures

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



namespace eval cal {

variable scaleAHigh
variable scaleALow
variable scaleBHigh
variable scaleBLow

}

#Calibration GUI
#------------------
#Thi procedure creates the calibration window and sets up all the widgets
#associate with calibrating the vertical scales.
proc cal::calibration {} {

	#Create a new window for the calibration controls
	if {[winfo exists .calibrate]} {
		raise .calibrate
		return
	}
	toplevel .calibrate
	wm title .calibrate "Scope Vertical Calibration"

	#Create a frame for each channel
	frame .calibrate.a -relief groove -borderwidth 3
	frame .calibrate.b -relief groove -borderwidth 3
	
	#Channel A Calibration Controls
	label .calibrate.chanA	\
		-text "Channel A"
	
	label .calibrate.a.highLabel	\
		-text "High Range (1V-5V/div)\nA/D Step \[mV/step\]"
	scale .calibrate.a.high	\
		-from 0.070	\
		-to 0.040	\
		-variable cal::scaleAHigh	\
		-orient vertical	\
		-tickinterval 0	\
		-resolution 0.0001	\
		-length 200		\
		-command {cal::calibrateVertical a high}
		
	label .calibrate.a.lowLabel	\
		-text "Low Range (50mV-500mV/div)\nA/D Step \[mV/step\]"
	scale .calibrate.a.low	\
		-from 0.007	\
		-to 0.0040	\
		-variable cal::scaleALow	\
		-orient vertical	\
		-tickinterval 0	\
		-resolution 0.00001	\
		-length 200	\
		-command {cal::calibrateVertical a low}
	
	grid .calibrate.a.highLabel -row 0 -column 0
	grid .calibrate.a.high -row 1 -column 0
	grid .calibrate.a.lowLabel -row 0 -column 1
	grid .calibrate.a.low -row 1 -column 1

	#Channel B Calibration Controls
	label .calibrate.chanB	\
		-text "Channel B"
	
	#Channel B Calibration Controls
	label .calibrate.b.highLabel	\
		-text "High Range (1V-5V/div)\nA/D Step \[mV/step\]"
	scale .calibrate.b.high	\
		-from 0.070	\
		-to 0.040	\
		-variable cal::scaleBHigh	\
		-orient vertical	\
		-tickinterval 0	\
		-resolution 0.0001	\
		-length 200		\
		-command {cal::calibrateVertical b high}
		
	label .calibrate.b.lowLabel	\
		-text "Low Range (50mV-500mV/div)\nA/D Step \[mV/step\]"
	scale .calibrate.b.low	\
		-from 0.007	\
		-to 0.004	\
		-variable cal::scaleBLow	\
		-orient vertical	\
		-tickinterval 0	\
		-resolution 0.00001	\
		-length 200	\
		-command {cal::calibrateVertical b low}
	
	grid .calibrate.b.highLabel -row 0 -column 0
	grid .calibrate.b.high -row 1 -column 0
	grid .calibrate.b.lowLabel -row 0 -column 1
	grid .calibrate.b.low -row 1 -column 1
		
	button .calibrate.restoreDefaults	\
		-text "Restore Defaults"	\
		-command {cal::restoreDefaults}
	
	button .calibrate.saveCalibration	\
		-text "Save Calibration Values"	\
		-command {cal::saveCalibration}
	
	grid .calibrate.chanA -row 0 -column 0 -sticky w
	grid .calibrate.a -row 1 -column 0
	grid .calibrate.chanB -row 0 -column 1 -sticky w
	grid .calibrate.b -row 1 -column 1 -sticky w
	grid .calibrate.restoreDefaults -row 2 -column 0 -columnspan 2
	grid .calibrate.saveCalibration -row 3 -column 0 -columnspan 2
}

#Calibrate Vertical Scale
#---------------------------
#This procedure is the service routine called by each of the calibration sliders.
#It performs the necessary conversion calculations and updates the global arrays
#which hold the vertical constants for scaling the vertical axes.
proc cal::calibrateVertical {chan scale sliderValue} {
	
	if {$chan=="a"} {
		if {$scale=="high"} {
			set scope::stepSizeAHigh $cal::scaleAHigh
		} elseif {$scale=="low"} {
			set scope::stepSizeALow $cal::scaleALow
		}
	} elseif {$chan=="b"} {
		if {$scale=="high"} {
			set scope::stepSizeBHigh $cal::scaleBHigh
		} elseif {$scale=="low"} {
			set scope::stepSizeBLow $cal::scaleBLow
		}
	}

	scope::setVertical
	
}

#Restore Defaults
#-------------------
#This procedure restores all of the vertical scale settings to their defaults
proc cal::restoreDefaults {} {
	
	set scope::stepSizeAHigh $scope::stepSizeHighDefault
	set scope::stepSizeALow  $scope::stepSizeLowDefault
	set scope::stepSizeBHigh $scope::stepSizeHighDefault
	set scope::stepSizeBLow $scope::stepSizeLowDefault
	
	set cal::scaleAHigh $scope::stepSizeHighDefault
	set cal::scaleALow $scope::stepSizeLowDefault
	set cal::scaleBHigh $scope::stepSizeHighDefault
	set cal::scaleBLow $scope::stepSizeLowDefault
	
	scope::setVertical
}

#Save Calibration
#-------------------
#This procedure saves the current calibration values into a text configuration
#file that is used to configure the vertical scaling each time the program
#is started.
proc cal::saveCalibration {} {
		
	set configFile [open calibration.cfg w]
	puts $configFile $scope::stepSizeAHigh
	puts $configFile $scope::stepSizeALow
	puts $configFile $scope::stepSizeBHigh
	puts $configFile $scope::stepSizeBLow
	close $configFile
	
	tk_messageBox	\
		-message "Configuration values saved."	\
		-type ok
}

proc cal::readConfig {} {
	#Attempt to read a vertical calibration file on startup
	if [catch {open calibration.cfg r+} configFile] {
		puts "Unable to open custom calibration file, using defaults."
		set cal::scaleAHigh $scope::stepSizeAHigh
		set cal::scaleALow $scope::stepSizeALow
		set cal::scaleBHigh $scope::stepSizeBHigh
		set cal::scaleBLow $scope::stepSizeBLow
	} else {
		#Channel A High Scale Value
		gets $configFile line
		set scope::stepSizeAHigh $line
		set cal::scaleAHigh $line
		#Channel A Low Scale Value
		gets $configFile line
		set scope::stepSizeALow $line
		set cal::scaleALow $line
		#Channel B High Scale Value
		gets $configFile line
		set scope::stepSizeBHigh  $line
		set cal::scaleBHigh $line
		#Channel B Low Scale Value
		gets $configFile line
		set scope::stepSizeBLow $line
		set cal::scaleBLow $line
		
		scope::setVertical
		puts "Calibration constants loaded from file."
	}
}

cal::readConfig