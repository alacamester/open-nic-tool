#!/bin/bash
#DEV=$1
DEV=/dev/onicdev0

#FBASE=0x200000
#FCTRL=0
#FIFACE_A=4
#FIFACE_B=8

# Reset filter modules
#./onic_tool $DEV w 0x200000 3
#sleep 0.1
#./onic_tool $DEV w 0x200000 0
#sleep 0.1

# Interface A
# bit 7:0   = FLT number
# bit 15:8  = Filter mask-enabled (IP-src,dst, proto, src-port, dst-port); 0 = pass all
# bit 23:16 = IPv4(/6) protocol
./onic_tool $DEV w 0x200004 00110F00
# bit 31:0  = IPv4 SRC address
./onic_tool $DEV w 0x200004 0101A8C0
# bit 31:0  = IPv4 DST address
./onic_tool $DEV w 0x200004 0201A8C0
# bit 15:0  = L4 SRC port (TCP/UDP for now)
# bit 31:16 = L4 DST port (TCP/UDP for now)
./onic_tool $DEV w 0x200004 0000D204

./onic_tool $DEV w 0x200004 00110F01
./onic_tool $DEV w 0x200004 0101A8C0
./onic_tool $DEV w 0x200004 0201A8C0
./onic_tool $DEV w 0x200004 0000D304

# Interface B
#./onic_tool $DEV w 0x200008 00110F00
#./onic_tool $DEV w 0x200008 0101A8C0
#./onic_tool $DEV w 0x200008 0201A8C0
#./onic_tool $DEV w 0x200008 0000D204
