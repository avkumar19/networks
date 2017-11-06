set val(chan) Channel/WirelessChannel;
set val(prop) Propagation/TwoRayGround;
set val(netif) Phy/WirelessPhy;
set val(mac) Mac/802_11;
set val(ifq) Queue/DropTail/PriQueue;
set val(ll) LL;
set val(ant) Antenna/OmniAntenna;
set val(X) 1000;
set val(Y) 1000;
set val(ifqlen) 50;
set val(nn) 6;
set val(rp) DSDV;
set ns [new Simulator]
set f [open out.tr w]
$ns trace-all $f
set nf [open out.nam w]
$ns namtrace-all-wireless $nf $val(X) $val(Y)
set topo [new Topography]
$topo load_flatgrid $val(X) $val(Y)
set god_ [create-god $val(nn)]
$ns node-config -adhocRouting $val(rp) \
-llType $val(ll) \
-macType $val(mac) \
-ifqType $val(ifq) \
-ifqLen $val(ifqlen) \
-antType $val(ant) \
-propType $val(prop) \
-phyType $val(netif) \
-channelType $val(chan) \
-topoInstance $topo \
-agentTrace ON \
-routerTrace ON \
-macTrace OFF \
-movementTrace OFF
for { set i 0 } { $i< $val(nn) } { incr i } {
	set n($i) [$ns node]
$ns initial_node_pos $n($i) 40
$n($i) random-motion 0
}
$n(0) set x_ 100.0
$n(0) set y_ 100.0
$n(0) set z_ 000.0
$n(1) set x_ 200.0
$n(1) set y_ 200.0
$n(1) set z_ 000.0
$n(2) set x_ 300.0
$n(2) set y_ 200.0
$n(2) set z_ 000.0
$n(3) set x_ 400.0
$n(3) set y_ 300.0
$n(3) set z_ 000.0
$n(4) set x_ 500.0
$n(4) set y_ 300.0
$n(4) set z_ 000.0
$n(5) set x_ 600.0
$n(5) set y_ 400.0
$n(5) set z_ 000.0
$ns at 0.0 "$n(0) setdest 100.0 100.0 000.0"
$ns at 0.0 "$n(1) setdest 200.0 200.0 000.0"
$ns at 0.0 "$n(2) setdest 300.0 200.0 000.0"
$ns at 0.0 "$n(3) setdest 400.0 300.0 000.0"
$ns at 0.0 "$n(4) setdest 500.0 300.0 000.0"
$ns at 0.0 "$n(5) setdest 600.0 400.0 000.0"
set udp0 [new Agent/UDP]
$ns attach-agent $n(0) $udp0
set cbr0 [new Application/Traffic/CBR]
$cbr0 set packetSize_ 500
$cbr0 set interval_ 0.005
$cbr0 attach-agent $udp0
set null0 [new Agent/Null]
$ns attach-agent $n(5) $null0
$ns connect $udp0 $null0
proc finish {} {
global ns nf f
$ns flush-trace
close $f
close $nf
exec nam -r 5m out.nam &
exit 0
}
$ns at 0.5 "$cbr0 start"
$ns at 3.0 "$cbr0 stop"
$ns at 5.0 "finish"
$ns run