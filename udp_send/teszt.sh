#!/bin/bash
./udp_send 192.168.1.1 1234 192.168.1.2 1400 &
./udp_send 192.168.1.1 1235 192.168.1.3 1400 &
./udp_send 192.168.3.1 1236 192.168.1.2 1400 &
#./udp_send 192.168.3.1 1236 192.168.3.2 1400 &
#./udp_send 192.168.3.1 1237 192.168.3.2 1400 &
#./udp_send 192.168.3.1 1238 192.168.3.2 1400 &

