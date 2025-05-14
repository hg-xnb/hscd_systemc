gtkwave::loadFile "gcd_sw_sw_trace.vcd"
set num_sigs [gtkwave::getNumFacs]
for {set i 0} {$i < $num_sigs} {incr i} {
    set sig_name [gtkwave::getFacName $i]
    gtkwave::addSignalsFromList $sig_name
}
gtkwave::setZoomFactor -2
