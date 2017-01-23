set noiseA 0
set noiseB 0


toplevel .debug

button .debug.test	\
	-text "Test Sine"	\
	-command {fakeData $debugAmp}
	
button .debug.dc	\
	-text "Test DC"	\
	-command {fakeDC}

scale .debug.amp	\
	-label "Amp"	\
	-from 0		\
	-to 511		\
	-variable debugAmp

scale .debug.offset	\
	-label "Offset"	\
	-from -512		\
	-to 511		\
	-variable debugOff


proc fakeData {dummy} {
	global noiseA noiseB
	set data "D"
	
	set f [expr {3/1024.0}]
	
	for {set i 0} {$i < 1024} {incr i} {
	
	#Channel A
	set temp [expr {512+$noiseA*rand()+$::debugAmp*sin(2*3.14*$f*$i)+$::debugOff}]
	set byteHigh [expr {round(floor($temp/pow(2,8)))}]
	set byteLow [expr {round(floor($temp))%round(pow(2,8))}]
	
	lappend data $byteHigh
	lappend data $byteLow
	
	#Channel B
	set temp [expr {512+$noiseB*rand()}]
	set byteHigh [expr {round(floor($temp/pow(2,8)))}]
	set byteLow [expr {round(floor($temp))%round(pow(2,8))}]
	
	lappend data $byteHigh
	lappend data $byteLow
	
	}

	scope::processScopeData $data
	
	#after 750 fakeData $::debugAmp

}



pack .debug.amp .debug.offset
pack .debug.test

