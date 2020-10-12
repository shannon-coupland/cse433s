#!/usr/bin/python3

from scapy.all import *

a = IP()
a.dst = '10.0.2.4'
a.src = '10.10.10.10'
b = ICMP()
p = a/b
send(p)
print("Packet sent:")
p.show()

