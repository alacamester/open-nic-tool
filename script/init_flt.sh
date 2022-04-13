#!/bin/bash
DEV=$1
#DEV=/dev/onicdev0
FFILE=$2

# Reset filter modules
#./onic_tool $DEV w 0x200000 3
#sleep 0.1
#./onic_tool $DEV w 0x200000 0
#sleep 0.1

# 1. Clear previous filters
./onic_tool $DEV c

# 2. read filter-file
./onic_tool $DEV f $FFILE
