#!/bin/bash
insmod ../open-nic-driver_laca/onic.ko
sleep 1
ifconfig enp37s0f0 192.168.2.1
ifconfig enp37s0f1 192.168.3.1

