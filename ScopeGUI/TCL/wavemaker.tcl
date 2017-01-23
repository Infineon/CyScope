#!/usr/bin/wish

# wavemaker - an aid to preparing files for user waveforms
# for the Syscomp WGM-101 waveform generator.

# Usage: first prepare a bitmap file (ppm or gif format is good)
# containing a scan of the waveform. The scan can be from
# a hand-drawn waveform or one scanned from a printed
# source.
# Then use the File menu to load the scanned picture. It'll
# appear in the window.
# Now select the "Adjust Axis" button. You can then drag
# the ends of the horizontal axis line. Set them to cover
# exactly one complete cycle of the waveform. The line is
# not constrained to be horizontal, in case the scan was
# not quite aligned with the horizontal and vertical axes.
# Next, select the "Draw" button and use the mouse to trace
# over the waveform. You can go past the ends of the cycle:
# the horizontal axis line dictates which part of the trace
# is used. Also, you don't need to trace the whole line
# in a single sweep. Use as many strokes as is convenient.
# Select the "Erase" button to use the mouse as an erasor.
# The erasor works like this: you drag the mouse horizontally
# across a part of the waveform, and any drawn lines
# in that region will be removed.
# If any particular x coordinate has more than one line
# running through it, an average will be taken.
# To check accuracy, select the "See Result" button. This
# will show the line that the program will use.

# Special note: if your waveform has vertical edges, don't
# draw them. The result will be better that way. The
# algorithm doesn't mind if there are gaps in the line.
# It fills them by linear interpolation.

namespace eval waveMaker {} {
	set version 0.91
	
	# Variable which will be true if drawing/erasure has been done
	# since the last save.
	set dirty false
	# Variable which will be true if any strokes have been
	# created or erased since the histogram was last created,
	# or if the axis has been moved since then. It's used
	# when deciding whether to re-create the histogram
	# when "See Result" mode is selected.
	set histdirty true
	# Pixel width of the drawn lines. These can be adjusted
	# using the Options menu.
	set strokewidth 1
	set histogramwidth 1 ;# affects the horizontal lines only
	# Available colours. Feel free to extend the list if
	# your favourite colour isn't here.
	set COLOURS [list red green blue yellow cyan magenta \
			 black white]
	# Default colours. These can be altered through
	# the Options menu.
	set strokecolour red
	set axiscolour magenta
	set histogramcolour blue
	
	set drawingmode axis
	set zoomfactor 1
	set prevzoom 1
	set zoombutton ""
}

# From Welch, a scrolled canvas.
proc waveMaker::scrolledCanvas { c args } {
    frame $c
    eval {canvas $c.canvas \
	    -xscrollcommand [list $c.xscroll set] \
	    -yscrollcommand [list $c.yscroll set] \
	    -highlightthickness 0 \
	    -borderwidth 0} $args
    scrollbar $c.xscroll -orient horizontal \
	-command [list $c.canvas xview]
    scrollbar $c.yscroll -orient vertical \
	-command [list $c.canvas yview]
    grid $c.canvas $c.yscroll -sticky news
    grid $c.xscroll -sticky ew
    grid rowconfigure $c 0 -weight 1
    grid columnconfigure $c 0 -weight 1
    return $c.canvas
}

# Let the user specify a file to use as the background picture.
proc waveMaker::loadPicture { } {
    variable c 
    variable drawingmode
    variable picturepath
    variable zoomfactor
    
    set picturepath [tk_getOpenFile -initialdir . -parent .waveMaker]
    if {[string length $picturepath]} {
	waveMaker::watchCursor
	$c delete picture
	foreach s [list 1 2 4 8] {
	    catch {image delete pic$s}
	}
	# The obvious method here would be to import the
	# picture as a bitmap. However, you can't zoom
	# bitmaps but you can zoom photos. And we want
	# to be able to zoom in on the detail, so we
	# use a photo.
	image create photo pic1 -file $picturepath
	foreach s [list 2 4 8] {
	    image create photo pic$s
	    pic$s copy pic1 -zoom $s
	}
	$c create image 0 0 -image pic$zoomfactor \
	    -anchor nw -state disabled -tags picture
	set bbox [$c bbox picture]
	$c configure -scrollregion $bbox
	$c lower picture
	$c lower historect
	waveMaker::endWatch
    }
}

# Draw the horizontal axis.
proc waveMaker::createAxis { } {
    variable c 
    variable drawingmode
    variable axiscolour
    
    set w [$c cget -width]
    set x0 [expr {$w / 3}]
    set x1 [expr {$w * 2 / 3}]
    set y [expr {[$c cget -height] / 2}]
    set axiscoords [list $x0 $y $x1 $y]
    $c create line $axiscoords -fill $axiscolour \
	-state disabled -tags [list axis axisline]
    set r 10
    for {set i 0} {$i <= 1} {incr i} {
	set x [set x$i]
	$c create oval [expr {$x - $r}] [expr {$y - $r}] \
	    [expr {$x + $r}] [expr {$y + $r}] \
	    -width 2 -outline $axiscolour -tags [list axis axishandle$i]
	if {[string equal $drawingmode "axis"]} {
	    $c bind axishandle$i <ButtonPress-1> \
		[list waveMaker::axisHandlePressed $i %X %Y]
	}
    }
}

# User has clicked the middle mouse button on the canvas.
# This is a short cut to select drawing mode, or if already
# in drawing mode to select erase mode.
proc waveMaker::flipDrawingMode { } {
    variable drawingmode
    # We don't set the cursor to a watch here, because
    # modeChanged will do it.
    if {[string equal $drawingmode "draw"]} {
	set drawingmode erase
    } else {
	set drawingmode "draw"
    }
    waveMaker::modeChanged
}

# User has right-clicked on the canvas. Show a menu
# allowing the drawing mode to be changed. This menu
# duplicates the actions of the radio buttons and
# is provided merely for convenience.
proc waveMaker::showDrawingMenu { X_ Y_ x_ y_ } {
    variable drawingmode
    variable zoomfactor
    
    set m .drawmenu
    catch {menu $m -tearoff 0}
    $m delete 0 end
    foreach {text mode} [list "Adjust Axis" axis "Erase" erase \
			     "Draw" draw "See Result" result] {
	$m add radio -label $text -variable waveMaker::drawingmode \
	    -value $mode -command waveMaker::modeChanged
    }
    $m add separator
    set m2 $m.sub
    catch {menu $m2 -tearoff 0}
    $m2 delete 0 end
    foreach z [list 1 2 4 8] {
	$m2 add radio -label $z -variable waveMaker::zoomfactor -value $z \
	    -command [list waveMaker::setZoom $x_ $y_]
    }
    $m add cascade -label "Zoom" -menu $m2
    tk_popup $m $X_ $Y_
}

# User has changed the drawing mode.
proc waveMaker::modeChanged { } {
    variable c
    variable drawingmode
    variable cursor 
    variable histdirty
    
    waveMaker::watchCursor
    for {set i 0} {$i <= 1} {incr i} {
	$c bind axishandle$i <ButtonPress-1> { }
	$c itemconfigure axishandle$i -state disabled
    }
    bind $c <ButtonPress-1> { }
    for {set j 0} {$j < 256} {incr j} {
	$c bind hrect$j <ButtonPress-1> { }
	$c itemconfigure hrect$j -state disabled
    }
    $c configure -background white
    $c configure -closeenough 1
    switch -- $drawingmode {
	axis {
	    for {set i 0} {$i <= 1} {incr i} {
		$c bind axishandle$i <ButtonPress-1> \
		    [list waveMaker::axisHandlePressed $i %X %Y]
		$c itemconfigure axishandle$i -state normal
	    }
	    $c raise axis
	    $c configure -closeenough 4
	    set cursor dot
	}
	erase {
	    bind $c <ButtonPress-1> [list waveMaker::beginErase %x]
	    set cursor sb_up_arrow
	}
	draw {
	    bind $c <ButtonPress-1> \
		[list waveMaker::beginStroke %x %y]
	    $c raise stroke
	    set cursor pencil
	}
	result {
	    if {$histdirty} {
		# We create the histogram only if we need to.
		# This means that any tweaks done on the
		# histogram will be preserved if the user
		# has changed drawing modes but has not taken
		# any actions that would affect the values.
		waveMaker::createHistogram
	    } else {
		$c raise histoline
		for {set j 0} {$j < 256} {incr j} {
		    $c bind hrect$j <ButtonPress-1> [list waveMaker::samplePressed $j %y]
		    $c itemconfigure hrect$j -state normal
		}
	    }
	    set cursor crosshair
	}
	default {
	    puts "Fatal error: Unknown drawing mode '$drawingmode'"
	    destroy .waveMaker
	}
    }
    waveMaker::endWatch
}

# User has chosen an item from the Zoom menu in the menu bar,
# or from the submenu in the drawing window menu. If the
# former, adjust the scroll bars afterwards so that the
# centre of the visible canvas stays in the centre.
# If the latter, the x_ and y_ parameters will give
# the position where the mouse was pressed for the
# popup menu. After the zoom, adjust the scroll bars
# to bring that point to the centre of the screen.
proc waveMaker::setZoom { {x_ ""} {y_ ""} } {
    variable c
    variable zoomfactor
    variable prevzoom
    variable zoombutton
    
    if {$zoomfactor != $prevzoom} {
	waveMaker::watchCursor
	if {[string length $x_]} {
	    set cx [$c canvasx $x_]
	    set cy [$c canvasy $y_]
	    foreach {x0 y0 x1 y1} [$c cget -scrollregion] break
	    set xmid [expr {($cx - $x0) / double($x1 - $x0)}]
	    set ymid [expr {($cy - $y0) / double($y1 - $y0)}]
	} else {
	    foreach {left frac} [$c xview] break
	    set xmid [expr {$left + 0.5 * $frac}]
	    foreach {left frac} [$c yview] break
	    set ymid [expr {$left + 0.5 * $frac}]
	}
	set scale [expr {$zoomfactor / double($prevzoom)}]
	foreach tag [list axis stroke histotag] {
	    $c scale $tag 0 0 $scale $scale
	}
	$c itemconfigure picture -image pic$zoomfactor
	set bbox [$c bbox picture]
	if {[llength $bbox]} {
	    $c configure -scrollregion $bbox
	} else {
	    # A zoom factor has been selected, but there's no
	    # picture loaded. So we base the scroll region
	    # on the canvas size.
	    $c configure -scrollregion \
		[list 0 0 [expr {$zoomfactor * [winfo width $c]}] \
		     [expr {$zoomfactor * [winfo height $c]}]]
	}
	$zoombutton configure -text "Zoom$zoomfactor"
	set prevzoom $zoomfactor
	# Adjust the scroll bars to centre the viewpoint.
	set region [$c cget -scrollregion]
	set frac [expr {[winfo width $c] \
			    / double([lindex $region 2] - [lindex $region 0])}]
	$c xview moveto [expr {$xmid - 0.5 * $frac}]
	set frac [expr {[winfo height $c] \
			    / double([lindex $region 3] - [lindex $region 1])}]
	$c yview moveto [expr {$ymid - 0.5 * $frac}]
	# Keep the histogram background rectangles at constant height.
	waveMaker::adjustHistogramRects
	waveMaker::endWatch
    }
}

# User has changed the stroke width setting.
proc waveMaker::adjustStrokeWidth { } {
    variable c
    variable strokewidth
    waveMaker::watchCursor
    $c itemconfigure stroke -width $strokewidth
    waveMaker::endWatch
}

# User has changed the stroke colour setting.
proc waveMaker::adjustStrokeColour { } {
    variable c
    variable strokecolour
    waveMaker::watchCursor
    $c itemconfigure stroke -fill $strokecolour
    waveMaker::endWatch
}

# User has changed the histogram width setting.
proc waveMaker::adjustHistogramWidth { } {
    variable c
    variable histogramwidth
    waveMaker::watchCursor
    $c itemconfigure histhoriz -width $histogramwidth
    waveMaker::endWatch
}

# User has changed the histogram colour setting.
proc waveMaker::adjustHistogramColour { } {
    variable c
    variable histogramcolour
    waveMaker::watchCursor
    $c itemconfigure histoline -fill $histogramcolour
    waveMaker::endWatch
}

# User has changed the axis colour setting.
proc waveMaker::adjustAxisColour { } {
    variable c
    variable axiscolour
    waveMaker::watchCursor
    $c itemconfigure axisline -fill $axiscolour
    for {set i 0} {$i <= 1} {incr i} {
	$c itemconfigure axishandle$i -outline $axiscolour
    }
    waveMaker::endWatch
}

# The mouse has been pressed on one of the circles
# at the ends of the horizontal axis line.
proc waveMaker::axisHandlePressed { i_ X_ Y_ } {
    variable c
    $c bind axishandle$i_ <B1-Motion> \
	[list waveMaker::axisHandleDragged $i_ $X_ $Y_ %X %Y]
    $c bind axishandle$i_ <ButtonRelease-1> [list waveMaker::axisHandleReleased $i_]
}

# The mouse is being dragged on one of the circles
# at the ends of the horizontal axis line.
proc waveMaker::axisHandleDragged { i_ X0_ Y0_ X_ Y_ } {
    variable c
    variable dirty
    variable histdirty
    set dx [expr {$X_ - $X0_}]
    set dy [expr {$Y_ - $Y0_}]
    $c move axishandle$i_ $dx $dy
    set linecoords [$c coords axisline]
    set px [expr {$i_ * 2}]
    set py [expr {$i_ * 2 + 1}]
    set linecoords [lreplace $linecoords $px $py \
			[expr {[lindex $linecoords $px] + $dx}] \
			[expr {[lindex $linecoords $py] + $dy}]]
    $c coords axisline $linecoords
    $c bind axishandle$i_ <B1-Motion> \
	[list waveMaker::axisHandleDragged $i_ $X_ $Y_ %X %Y]
    set dirty true
    set histdirty true
}

# The drag on an axis circle has been ended by
# the release of the mouse button.
proc waveMaker::axisHandleReleased { i_ } {
    variable c
    $c bind axishandle$i_ <B1-Motion> { }
    $c bind axishandle$i_ <ButtonRelease-1> { }
}

# User has pressed the mouse in drawing mode, which
# may mean the start of a stroke.
proc waveMaker::beginStroke { X_ Y_ } {
    variable c
    set x [$c canvasx $X_]
    set y [$c canvasy $Y_]
    bind $c <B1-Motion> [list waveMaker::extendStroke $x $y %x %y]
    bind $c <ButtonRelease-1> waveMaker::endStroke
}

# User is dragging the mouse to extend a stroke.
proc waveMaker::extendStroke { x0_ y0_ X_ Y_ {t_ ""} } {
    variable c
    variable strokewidth
    variable strokecolour
    set x [$c canvasx $X_]
    set y [$c canvasy $Y_]
    if {[string length $t_]} {
	# Extend the line.
	$c coords $t_ [concat [$c coords $t_] [list $x $y]]
    } else {
	# This is the start of the drag, so create the line.
	set t [$c create line $x0_ $y0_ $x $y \
		   -width $strokewidth \
		   -state disabled \
		   -fill $strokecolour -tags stroke]
	bind $c <B1-Motion> [list waveMaker::extendStroke $x $y %x %y $t]
	bind $c <ButtonRelease-1> [list waveMaker::endStroke $t]
    }
}

# User has released the mouse after perhaps creating a stroke.
proc waveMaker::endStroke { {t_ ""} } {
    variable c
    variable dirty
    variable histdirty
    if {[string length $t_]} {
	set coords [waveMaker::saneCoords [$c coords $t_]]
	if {[llength $coords]} {
	    $c coords $t_ $coords
	    set dirty true
	    set histdirty true
	} else {
	    # After sanitization, the coords were no
	    # longer valid.
	    $c delete $t_
	}
    }
    bind $c <B1-Motion> { }
    bind $c <ButtonRelease-1> { }
}

# User has pressed the mouse to begin an erase operation.
proc waveMaker::beginErase { x_ } {
    variable c
    set x [$c canvasx $x_]
    set box [$c cget -scrollregion]
    set y0 [lindex $box 1]
    set y1 [lindex $box 3]
    $c create line $x $y0 $x $y1 -tags [list erase erase0]
    $c create line $x $y0 $x $y1 -tags [list erase erase1]
    bind $c <B1-Motion> [list waveMaker::extendErase $x %x]
    bind $c <ButtonRelease-1> [list waveMaker::endErase]
}

# User is moving the second erase marker.
proc waveMaker::extendErase { x0_ x_ } {
    variable c
    set x [$c canvasx $x_]
    set dx [expr {$x - $x0_}]
    $c move erase1 $dx 0
    bind $c <B1-Motion> [list waveMaker::extendErase $x %x]
}

# User has released the mouse after positioning
# the erase marker.
proc waveMaker::endErase { } {
    variable c
    variable strokewidth
    variable strokecolour
    variable dirty
    
    waveMaker::watchCursor
    set x0 [lindex [$c coords erase0] 0]
    set x1 [lindex [$c coords erase1] 0]
    if {$x0 > $x1} {
	set temp $x0
	set x0 $x1
	set x1 $temp
    }
    $c delete erase
    # Now adjust the strokes.
    set newstrokes [list]
    foreach stroketag [$c find withtag stroke] {
	set stroke [$c coords $stroketag]
	set newstrokes [concat $newstrokes [waveMaker::cropStroke $stroke $x0 $x1]]
    }
    $c delete stroke
    foreach stroke $newstrokes {
	$c create line $stroke -width $strokewidth \
	    -fill $strokecolour -state disabled -tags stroke
    }
    set dirty true
    waveMaker::endWatch
}

# The user is allowed to draw strokes in any direction,
# and is not prevented from doubling back on a line.
# That presents a problem later if we don't fix it.
# This proc takes a set of line coords, and delivers
# back a line where the coords are monotonically
# increasing in the x direction.
# If the result of the operation is not a valid line,
# return an empty list.
proc waveMaker::saneCoords { coords_ } {
    if {[llength $coords_] < 4} {
	# Not a valid line.
	return [list]
    }
    # Establish the predominant drawing direction of the
    # line, by counting the forward steps (x increasing)
    # versus the reverse steps (x decreasing)
    set forward 0
    set reverse 0
    set same 0
    set px [lindex $coords_ 0]
    foreach {x y} [lrange $coords_ 2 end] {
	if {$x > $px} {
	    incr forward
	} elseif {$x < $px} {
	    incr reverse
	} else {
	    incr same
	}
	set px $x
    }
    if {$reverse > $forward} {
	# Assume that the user drew the line from right
	# to left, so reverse the direction.
	set newcoords [list]
	for {set i [expr {[llength $coords_] - 2}]} {$i >= 0} {incr i -2} {
	    lappend newcoords [lindex $coords_ $i] \
		[lindex $coords_ [expr {$i + 1}]]
	}
	set coords_ $newcoords
    }
    # Remove any points where the x value decreases.
    set newcoords [lrange $coords_ 0 1]
    set px [lindex $coords_ 0]
    foreach {x y} [lrange $coords_ 2 end] {
	if {$x >= $px} {
	    lappend newcoords $x $y
	    set px $x
	}
    }
    # Validity check on what's left.
    if {[waveMaker::validStroke $newcoords]} {
	return $newcoords
    } else {
	return [list]
    }
}

# Given a stroke, remove any part of it that falls within
# the x0-x1 coordinate range. There are n possibilities:
# 1. The stroke is left untouched. Return it, wrapped
# in an outer list.
# 2. The stroke is trimmed at one end. Return it, wrapped
# in an outer list.
# 3. The stroke is erased entirely, or reduced to a single
# point. Return an empty list.
# 4. The stroke is split into two strokes. Return the two
# new strokes as a list.
proc waveMaker::cropStroke { stroke_ x0_ x1_ } {
    variable histdirty
    # Find the first point in the stroke that falls
    # within the range.
    set pos ""
    set i 0
    foreach {x y} $stroke_ {
	if {$x >= $x0_ && $x <= $x1_} {
	    set pos $i
	    break
	}
	incr i
    }
    if { ! [string length $pos]} {
	# The stroke is not affected.
	return [list $stroke_]
    } else {
	# The stroke is affected, and the first affected
	# point is at coord offset 'pos'.
	# This will invalidate the histogram, if it has
	# been created.
	set histdirty true
	# We'll split the stroke in two: coords before pos,
	# and those afterwards.
	set stroke(0) [lrange $stroke_ 0 [expr {$pos * 2 - 1}]]
	set stroke(1) [lrange $stroke_ [expr {$pos * 2}] end]
	for {set i 0} {$i <= 1} {incr i} {
	    set newstroke [list]
	    foreach {x y} $stroke($i) {
		if {$x < $x0_ || $x > $x1_} {
		    # Keep this point.
		    lappend newstroke $x $y
		}
	    }
	    if { ! [waveMaker::validStroke $newstroke]} {
		# Reduced to a point or to nothing at all.
		set newstroke [list]
	    }
	    set stroke($i) $newstroke
	}
	if {[llength $stroke(0)]} {
	    # That stroke survived.
	    if {[llength $stroke(1)]} {
		# So did the other.
		return [list $stroke(0) $stroke(1)]
	    } else {
		# Return just the first.
		return [list $stroke(0)]
	    }
	} else {
	    # The first stroke was reduced to nothing.
	    if {[llength $stroke(1)]} {
		# But the second survived.
		return [list $stroke(1)]
	    } else {
		# Neither stroke survived.
		return [list]
	    }
	}
    }
}

# Check the given stroke coords to see whether they
# constitute a valid stroke. To be valid, a stroke
# must have at least two coordinate pairs. If it
# has only two pairs, they must be different.
proc waveMaker::validStroke { stroke_ } {
    if {[llength $stroke_] > 4} {
	return true
    } elseif {[llength $stroke_] == 4} {
	foreach {x0 y0 x1 y1} $stroke_ break
	# Return true if the end points are not identical.
	return [expr {$x0 != $x1 || $y0 != $y1}]
    } else {
	return false
    }
}

# Create and display the histogram of sample values.
proc waveMaker::createHistogram { } {
    variable c
    variable histogramcolour
    variable histogramwidth
    variable histdirty
    variable extendhist
    variable MIN
    variable MAX
    
    $c delete histotag
    foreach {hx0 hy0 hx1 hy1} [waveMaker::saneCoords [$c coords axisline]] break
    set hyvalues [list]
    set hycounts [list]
    for {set i 0} {$i < 256} {incr i} {
	lappend hyvalues 0
	lappend hycounts 0
    }
    foreach stroketag [$c find withtag stroke] {
	set stroke [$c coords $stroketag]
	foreach {values counts} [waveMaker::stroke2hist $stroke] break
	for {set i 0} {$i < 256} {incr i} {
	    if {[lindex $counts $i]} {
		set hycounts [waveMaker::listincr $hycounts $i]
		set hyvalues [waveMaker::listincr $hyvalues $i [lindex $values $i]]
	    }
	}
    }
    set newvalues [list]
    for {set i 0} {$i < 256} {incr i} {
	if {[lindex $hycounts $i]} {
	    set v [expr {[lindex $hyvalues $i] \
			     / double([lindex $hycounts $i])}]
	    lappend newvalues $v
	} else {
	    lappend newvalues 0
	}
    }
    set hyvalues $newvalues
    # There may be gaps in the histogram values. If so, we'll
    # use interpolation to fix them.
    if {[lindex $hycounts 0] == 0} {
	# There's a gap at the start. Make the first point
	# coincide with the end marker of the horizontal axis.
	set hyvalues [lreplace $hyvalues 0 0 $hy0]
	set hycounts [lreplace $hycounts 0 0 1]
    }
    if {[lindex $hycounts end] == 0} {
	# There's a gap at the end. Make the last point
	# coincide with the end marker of the horizontal axis.
	set hyvalues [lreplace $hyvalues end end $hy1]
	set hycounts [lreplace $hycounts end end 1]
    }
    # Any remaining gaps are now bounded by proper values,
    # which simplifies the interpolation algorithm.
    for {set i 0} {$i < 256} {incr i} {
	if {[lindex $hycounts $i] == 0} {
	    # We've found a gap. Get the index of the point
	    # beyond the end of the gap.
	    for {set k [expr {$i + 1}]} {$k < 256} {incr k} {
		if {[lindex $hycounts $k] != 0} {
		    break
		}
	    }
	    # The point before the gap:
	    set j [expr {$i - 1}]
	    # Fill the gap, using linear interpolation.
	    set y0 [lindex $hyvalues $j]
	    set y1 [lindex $hyvalues $k]
	    for {set m $i} {$m < $k} {incr m} {
		set y [expr {$y0 + ($y1 - $y0) * ($m - $j) / double($k - $j)}]
		set hyvalues [lreplace $hyvalues $m $m $y]
		set hycounts [lreplace $hycounts $m $m 1]
	    }
	    # Skip the part we just filled in.
	    set i $k
	}
    }
    # Draw the sample values. We draw an extra 20 at each
    # end, which are made visible when the "Extend Histogram"
    # option is selected.
    set MIN -20
    set MAX 276
    set htags1 [list histotag histoline histhoriz histmain]
    set htags2 [list histotag histoline histhoriz extension]
    set vtags1 [list histotag histoline]
    set vtags2 [list histotag histoline extension]
    set htags $htags2
    set vtags $vtags2
    for {set i $MIN} {$i < $MAX} {incr i} {
	set j [expr {$i & 255}]
	set x0 [expr {$hx0 + $i * ($hx1 - $hx0) / 256.0}]
	set x1 [expr {$x0 + ($hx1 - $hx0) / 256.0}]
	set y [lindex $hyvalues $j]
	if {$i < 0} {
	    set y [expr {$y + $hy0 - $hy1}]
	} elseif {$i > 255} {
	    set y [expr {$y + $hy1 - $hy0}]
	}
	if {$i == 256} {
	    # Add the extension tag.
	    set htags $htags2
	    set vtags $vtags2
	}
	if {$i > $MIN} {
	    # Draw a vertical line from the previous sample.
	    $c create line $x0 $prevy $x0 $y \
		-fill $histogramcolour \
		-state disabled \
		-tags [concat $vtags [list vert$j]]
	}
	if {$i == 0} {
	    # Drop the extension tag.
	    set htags $htags1
	    set vtags $vtags1
	}
	# Draw the horizontal line.
	set tags $htags
	if {$i >= 0 && $i < 256} {
	    # Add a tag for use by the saveData proc.
	    lappend tags hmain$i
	}
	lappend tags hline$i horiz$j
	$c create line $x0 $y $x1 $y -fill $histogramcolour \
	    -width $histogramwidth \
	    -state disabled \
	    -tags $tags
	set tags [list histotag historect horiz$j hrect$j irect$i]
	if {$i < 0 || $i >= 256} {
	    lappend tags extension
	}
	$c create rect $x0 [expr {$y - 3}] $x1 [expr {$y + 3}] \
	    -fill white -width 0 \
	    -tags $tags
	$c bind hrect$j <ButtonPress-1> [list waveMaker::samplePressed $j %y]
	set prevy $y
    }
    waveMaker::adjustHistogramRects
    $c lower historect
    if {$extendhist} {
	$c itemconfigure extension -state normal
    } else {
	$c itemconfigure extension -state hidden
    }
    set histdirty false
}

# After initial creation or a change of zoom, adjust the
# histogram background rectangles so that they retain
# constant height.
proc waveMaker::adjustHistogramRects { } {
    variable c
    variable MIN
    variable MAX
    if {[info exists MIN]} {
	for {set i $MIN} {$i < $MAX} {incr i} {
	    foreach {x0 y0 x1 y1} [$c coords hline$i] break
	    set x1 [expr {$x1 + 1}]
	    set y0 [expr {$y0 - 3}]
	    set y1 [expr {$y1 + 4}]
	    $c coords irect$i [list $x0 $y0 $x1 $y1]
	}
    }
}

# Turn the given stroke into a histogram and return the
# result as a list of two lists. The first inner list
# has 256 y coord values, and the second has 256 counts.
# Each count is either 0 or 1. If 0, the stroke didn't
# cross that histogram segment and the value is
# irrelevant. If the count is 1, the value is the y coord
# of the stroke at that point.
proc waveMaker::stroke2hist { stroke_ } {
    variable c

    foreach {hx0 hy0 hx1 hy1} [waveMaker::saneCoords [$c coords axisline]] break
    set values [list]
    set counts [list]
    for {set i 0} {$i < 256} {incr i} {
	lappend values 0
	lappend counts 0
    }
    set prevbin ""
    foreach {x y} $stroke_ {
	set bin [expr {int(floor(($x - $hx0) * 256.0 / ($hx1 - $hx0)))}]
	if {$bin >= 0 && $bin < 256} {
	    set counts [waveMaker::listincr $counts $bin]
	    set values [waveMaker::listincr $values $bin $y]
	    if {[string length $prevbin] && $bin > $prevbin + 1} {
		# There's a gap, because adjacent points in
		# the stroke were more than one bin width apart.
		# Fill the gap by linear interpolation.
		for {set b [expr {$prevbin + 1}]} {$b < $bin} {incr b} {
		    set y2 [expr {$prevy + ($b - $prevbin) \
				      / double($bin - $prevbin)}]
		    set counts [waveMaker::listincr $counts $b]
		    set values [waveMaker::listincr $values $b $y2]
		}
	    }
	    set prevbin $bin
	    set prevy $y
	}
    }
    for {set i 0} {$i < 256} {incr i} {
	if {[lindex $counts $i] > 1} {
	    set values [lreplace $values $i $i \
			    [expr {[lindex $values $i] / \
				       double([lindex $counts $i])}]]
	    set counts [lreplace $counts $i $i 1]
	}
    }
    return [list $values $counts]
}

# User has pressed the mouse on a histogram sample.
# Assume it's the start of a drag.
proc waveMaker::samplePressed { j_ y_ } {
    variable c
    set y [$c canvasy $y_]
    $c bind hrect$j_ <B1-Motion> [list waveMaker::sampleDragged $j_ $y %y]
    $c bind hrect$j_ <ButtonRelease-1> [list waveMaker::sampleReleased $j_]
}

# User is dragging a sample vertically.
proc waveMaker::sampleDragged { j_ y0_ y1_ } {
    variable c
    set y1 [$c canvasy $y1_]
    set dy [expr {$y1 - $y0_}]
    $c move horiz$j_ 0 $dy
    $c bind hrect$j_ <B1-Motion> [list waveMaker::sampleDragged $j_ $y1 %y]
    # Adjust the vertical line(s) to the left.
    foreach t [$c find withtag vert$j_] {
	set coords [$c coords $t]
	set v [expr {[lindex $coords 3] + $dy}]
	set coords [lreplace $coords 3 3 $v]
	$c coords $t $coords
    }
    # Adjust the vertical line(s) to the right.
    set j [expr {($j_ + 1) & 255}]
    foreach t [$c find withtag vert$j] {
	set coords [$c coords $t]
	set v [expr {[lindex $coords 1] + $dy}]
	set coords [lreplace $coords 1 1 $v]
	$c coords $t $coords
    }
}

# User has ended the drag of a sample.
proc waveMaker::sampleReleased { j_ } {
    variable c
    variable dirty
    
    waveMaker::watchCursor
    $c bind hrect$j_ <B1-Motion> { }
    $c bind hrect$j_ <ButtonRelease-1> { }
    set dirty true
    waveMaker::endWatch
}

# Increment the specified value in the given list by
# the given amount (default 1) and return the new list.
proc waveMaker::listincr { values_ index_ {increment_ 1} } {
    set v [expr {[lindex $values_ $index_] + $increment_}]
    return [lreplace $values_ $index_ $index_ $v]
}

# User has clicked on the "Hide Picture" button.
proc waveMaker::toggleHidePic { } {
    variable c
    variable hidepic
    waveMaker::watchCursor
    if {$hidepic} {
	$c itemconfigure picture -state hidden
    } else {
	$c itemconfigure picture -state normal
    }
    waveMaker::endWatch
}

# User has toggle the "Extend Histogram" option.
proc waveMaker::extendHistogram { } {
    variable c
    variable extendhist
    if {$extendhist} {
	$c itemconfigure extension -state normal
    } else {
	$c itemconfigure extension -state hidden
    }
}

# User has selected "Save Data" from the File menu.
proc waveMaker::saveData { } {
    variable c
    variable picturepath
    variable dirty 
    variable histdirty
    variable removeDC
    
    set histtags [$c find withtag histmain]
    if {[llength $histtags] != 256} {
	# The histogram has not been created yet, so there's
	# nothing to save.
	set msg "You need to create the histogram before the"
	append msg " data can be saved. Select 'See Result'"
	append msg " to create the histogram. Then you will"
	append msg " be able to save the data."
	tk_messageBox -type ok -message $msg -parent .waveMaker
	return
    } elseif {$histdirty} {
	# Since the histogram was last created, the user has
	# added or erased one or more strokes, or has moved
	# the horizontal axis.
	set msg "The histogram has not been recreated since"
	append msg " your last changes to the strokes and/or axis."
	append msg " To update the histogram, press 'Cancel'"
	append msg " and then select the 'See Result' button."
	append msg " If you press OK, stale data will be saved."
	set reply [tk_messageBox -type okcancel -message $msg -parent .waveMaker]
	if {[string compare $reply "ok"]} {
	    return
	}
    }
    if {[info exists picturepath] && [string length $picturepath]} {
	set path [file rootname $picturepath].wgm
	set dir [file dirname $path]
	set file [file tail $path]
    } else {
	set path ""
	set dir .
	set file ""
    }
    set savepath [tk_getSaveFile -initialdir $dir -initialfile $file -parent .waveMaker]
    if {[string length $savepath]} {
	waveMaker::watchCursor
	# Convert the y coords to offsets from the axis line.
	foreach {hx0 hy0 hx1 hy1} [waveMaker::saneCoords [$c coords axisline]] break
	set offsets [list]
	set min ""
	set max ""
	for {set i 0} {$i < 256} {incr i} {
	    set y [lindex [$c coords hmain$i] 1]
	    set y0 [expr {$hy0 + ($i * ($hy1 - $hy0) / 256.0)}]
	    lappend offsets [set offset [expr {$y - $y0}]]
	    if {[string length $min]} {
		if {$offset < $min} {
		    set min $offset
		} elseif {$offset > $max} {
		    set max $offset
		}
	    } else {
		set min [set max $offset]
	    }
	}
	# Write the sample values to the file.
	set f [open $savepath w]
	set samples [list]
	if {$max == $min} {
	    # If max and min are the same, then all samples
	    # have exactly the same value. This means that
	    # the waveform is a perfect horizontal line.
	    # Scaling is impossible here, so we just write
	    # a zero DC level to the file.
	    for {set i 0} {$i < 256} {incr i} {
		puts $f 127
	    }
	} elseif {$removeDC} {
	    # Convert the offsets to integers in the range 0..255,
	    # but making sure that the average sample value is 127.
	    set average 0
	    foreach offset $offsets {
		set average [expr {$average + $offset}]
	    }
	    set average [expr {$average / 256.0}]
	    # This average offset must map to 127, and the
	    # max and min offsets must be constrained to map
	    # within the 0..255 range.
	    if {($max - $average) / (255.0 - 127.0) > \
		    ($average - $min) / 127.0} {
		# Map max to 255. This will happen if, for
		# instance, the wavform is a negative spike
		# in an otherwise constant value.
		foreach offset $offsets {
		    set sample [expr {int(round(($offset - $average) \
						    / ($max - $average) \
						    * (255 - 127) + 127))}]
		    if {$sample < 0 || $sample > 255} {
			puts "max $max, average $average, min $min"
			puts "offset $offset, Map max to 255, sample $sample"
			error "Sample value $sample is out of range"
		    }
		    puts $f $sample
		}
	    } else {
		# Map min to 0. This will happen if, for
		# instance, the wavform is a positive spike
		# in an otherwise constant value.
		foreach offset $offsets {
		    set sample [expr {int(round(($offset - $min) * 127 \
						    / ($average - $min)))}]
		    if {$sample < 0 || $sample > 255} {
			puts "max $max, average $average, min $min"
			puts "offset $offset, Map max to 255, sample $sample"
			error "Sample value $sample is out of range"
		    }
		    puts $f $sample
		}
	    }
	} else {
	    # Convert the offsets to integers in the range 0..255
	    foreach offset $offsets {
		set sample [expr {int(round(($offset - $min) \
						* 255 / ($max - $min)))}]
		puts $f $sample
	    }
	}
	close $f
	set dirty false
	waveMaker::endWatch
    }
}

# User has selected Quit from the File menu. If there
# are unsaved changes, give the option to cancel the quit.
proc waveMaker::checkQuit { } {
    variable dirty
    if {$dirty} {
	set reply [tk_messageBox -type okcancel -parent .waveMaker -message \
		       "There are unsaved changes. Discard them?"]
	if {[string compare $reply ok]} {
	    return
	}
    }
    destroy .waveMaker
}

# Called at the start of any operation that is initiated
# by a user action. It sets the cursor to a watch.
# The canvas sets its own cursor, so it needs special
# treatment here.
# Actions associated with dragging (mouse press and mouse
# move) shouldn't alter the cursor. If they take enough time
# to need to do so, then they need fixing. However, the
# release of the mouse at the end of a drag may call this proc
# legitimately, because it may be reasonable for the
# consequent actions to take a long time.
proc waveMaker::watchCursor { } {
    variable c
    . configure -cursor watch
    $c configure -cursor watch
    # The following update call is necessary because otherwise
    # the cursor change won't be immediately visible.
    update idletasks
}

# Called at the end of any operation that is initiated
# by a user action. Should correspond exactly to calls
# to watchCursor.
proc waveMaker::endWatch { } {
    variable c
    variable cursor
    . configure -cursor ""
    $c configure -cursor $cursor
}

# The user has selected the manual from the Help menu,
# but the operating system was unrecognized.
proc waveMaker::unknownOS { } {
    global osType
    set msg "The manual can be found as manual.pdf"
    append msg " in the same directory as this program."
    append msg " The program couldn't open it for you"
    append msg " because your operating system"
    append msg " is reported as '$osType', which the"
    append msg " program can't handle. Please report"
    append msg " this as a bug."
    tk_messageBox -type ok -message $msg -parent .waveMaker
}

# The user has selected the manual from the Help menu,
# but we were unable to find it.
proc waveMaker::missingManual { } {
    set msg "There should be a copy of the PDF manual"
    append msg " in the same directory as this program,"
    append msg " but it seems to be missing. The file"
    append msg " name should be manual.pdf."
    tk_messageBox -type ok -message $msg -parent .waveMaker
}

# The user has selected the manual from the Help menu,
# but when we were initializing we didn't find an application
# that could display a PDF file. So instead of displaying
# the file, display a dialog box to tell the user what
# happened.
proc waveMaker::explainManual { } {
    set msg "The PDF manual should be in the same directory"
    append msg " as the program, and you can open it from there."
    append msg " This is a bug, because wavemaker should"
    append msg " have found an application and displayed the manual."
    append msg " When you get the manual open, there's a"
    append msg " description in there of what to do about this."
    tk_messageBox -type ok -message $msg -parent .waveMaker
}

proc waveMaker::showWaveMaker {} {
	variable c
	variable mb
	variable m


	if {![winfo exists .waveMaker]} {
		
		toplevel .waveMaker
		wm title .waveMaker "WaveMaker"
		wm protocol .waveMaker WM_DELETE_WINDOW {waveMaker::checkQuit}

		# GUI setup.
		# Frame for the menu bar.
		set mf [frame .waveMaker.mf]

		# File menu.
		set mb $mf.filemb
		set m $mb.menu
		menubutton $mb -text "File" -menu $m
		menu $m -tearoff 0
		$m add command -label "Load Picture..." -command waveMaker::loadPicture
		# Option to remove DC component when waveform is saved.
		$m add separator
		set removeDC 0
		$m add check -label "Remove DC on Save" -variable waveMaker::removeDC
		$m add command -label "Save Data..." -command waveMaker::saveData
		$m add separator
		$m add command -label "Quit" -command waveMaker::checkQuit
		pack $mb -side left
		# Zoom menu.
		set waveMaker::zoomfactor 1
		set waveMaker::prevzoom 1
		set mb $mf.zoommb
		set m $mb.menu
		menubutton $mb -text "Zoom$waveMaker::zoomfactor" -menu $m
		set waveMaker::zoombutton $mb
		menu $m -tearoff 0
		foreach i [list 1 2 4 8] {
		    $m add radio -label $i -variable waveMaker::zoomfactor -value $i \
			-command [list waveMaker::setZoom]
		}
		pack $mb -side left
		# Options menu.
		set mb $mf.optmb
		set m $mb.menu
		menubutton $mb -text "Options" -menu $m
		menu $m -tearoff 0
		# Submenu for stroke width.
		set m2 $m.sw
		menu $m2 -tearoff 0
		for {set i 1} {$i <= 3} {incr i} {
		    $m2 add radio -label $i -value $i -variable waveMaker::strokewidth \
			-command waveMaker::adjustStrokeWidth
		}
		$m add cascade -label "Stroke Width" -menu $m2
		# Submenu for histogram width.
		set m2 $m.hw
		menu $m2 -tearoff 0
		for {set i 1} {$i <= 3} {incr i} {
		    $m2 add radio -label $i -value $i -variable waveMaker::histogramwidth \
			-command waveMaker::adjustHistogramWidth
		}
		$m add cascade -label "Histogram Width" -menu $m2
		# Submenu for stroke colour.
		set m3 $m.sc
		menu $m3 -tearoff  0
		foreach colour $waveMaker::COLOURS {
		    $m3 add radio -label [string totitle $colour] \
			-variable waveMaker::strokecolour -value $colour -command waveMaker::adjustStrokeColour
		}
		$m add cascade -label "Stroke Colour" -menu $m3
		# Submenu for histogram colour.
		set m3 $m.hc
		menu $m3 -tearoff  0
		foreach colour $waveMaker::COLOURS {
		    $m3 add radio -label [string totitle $colour] \
			-variable waveMaker::histogramcolour -value $colour \
			-command waveMaker::adjustHistogramColour
		}
		$m add cascade -label "Histogram Colour" -menu $m3
		# Submenu for axis colour.
		set m3 $m.ac
		menu $m3 -tearoff  0
		foreach colour $waveMaker::COLOURS {
		    $m3 add radio -label [string totitle $colour] \
			-variable waveMaker::axiscolour -value $colour -command waveMaker::adjustAxisColour
		}
		$m add cascade -label "Axis Colour" -menu $m3
		# Option to show the extended histogram.
		set extendhist 0
		$m add check -label "Extend Histogram" -variable waveMaker::extendhist \
		    -command waveMaker::extendHistogram
		# Pack the menu button.
		pack $mb -side left
		# Help menu.
		set mb $mf.helpmb
		set m $mb.menu
		menubutton $mb -text "Help" -menu $m
		menu $m -tearoff 0
		set spath [file dirname [info script]]
		if {[file exists $spath/waveMakerManual.pdf]} {
		    if { $::osType == "windows" } {
			$m add command \
			    -label "Manual (PDF)"	\
			    -command {eval exec [auto_execok start] \"\" [list "waveMakerManual.pdf"]}
		    } elseif {$::osType == "unix"} {
			# We look for an application that can open and display a pdf file.
			# We'll take the first available one in the list.
			set found false
			foreach app [list xpdf evince acroread] {
			    if { ! [catch {exec which $app}]} {
				$m add command  \
				    -label "Manual (PDF)"   \
				    -command [list exec $app waveMakerManual.pdf &]
				set found true
				break
			    }
			}
			if { ! $found} {
			    # We didn't find a suitable application, so add
			    # an entry which will bring up a dialog box
			    # explaining the situation to the user.
			    $m add command -label "Manual (PDF)" \
				-command waveMaker::explainManual
			}
		    } elseif {$::osType == "Darwin"} {
			$m add command	\
				-label "Manual (PDF)"	\
				-command [list exec open waveMakerManual.pdf]
		    } else {
			$m add command -label "Manual (PDF)" \
			    -command waveMaker::unknownOS
		    }
		} else {
		    # The manual seems to be missing.
		    $m add command -label "Manual (PDF)" \
			-command waveMaker::missingManual
		}
		$m add separator
		$m add command -label "v$waveMaker::version" -state disabled
		pack $mb -side right
		# Pack the menu bar.
		pack $mf -side top -fill x
		# Create and pack the scrolled canvas.
		set c [waveMaker::scrolledCanvas .waveMaker.can -width 600 -height 400 -background white \
			   -scrollregion [list 0 0 600 400]]
		pack .waveMaker.can -side top -fill both -expand true
		bind $c <ButtonPress-2> waveMaker::flipDrawingMode
		bind $c <Shift-ButtonPress-1> waveMaker::flipDrawingMode
		bind $c <ButtonPress-3> [list waveMaker::showDrawingMenu %X %Y %x %y]
		bind $c <Control-ButtonPress-1> [list waveMaker::showDrawingMenu %X %Y]
		$c configure -closeenough 4
		# Frame for the control buttons.
		set bf [frame .waveMaker.bf]
		# Radiobuttons for the controls.
		set waveMaker::drawingmode "axis"
		set b [radiobutton $bf.axis -text "Adjust Axis" \
			   -variable waveMaker::drawingmode -value axis -command waveMaker::modeChanged]
		pack $b -side left
		set b [radiobutton $bf.erase -text "Erase" \
			   -variable waveMaker::drawingmode -value erase -command waveMaker::modeChanged]
		pack $b -side left
		set b [radiobutton $bf.draw -text "Draw" \
			   -variable waveMaker::drawingmode -value draw -command waveMaker::modeChanged]
		pack $b -side left
		set b [radiobutton $bf.result -text "See Result" \
			   -variable waveMaker::drawingmode -value result -command waveMaker::modeChanged]
		pack $b -side left
		# Checkbutton to hide the picture.
		set hidepic 0
		set b [checkbutton $bf.hidepic -text "Hide Picture" -variable waveMaker::hidepic \
			   -command waveMaker::toggleHidePic]
		pack $b -side right
		# Pack the buttons frame.
		pack $bf -side top -fill x
		# Draw the horizontal axis.
		waveMaker::createAxis
		# Adjust for the initial drawing mode.
		waveMaker::modeChanged
		# That's it. Now we wait for the user to do something.
	} else {
		wm deiconify .waveMaker
		raise .waveMaker
		focus .waveMaker
	}
}


