#!/bin/bash
echo "Sum =" `awk '{ sum += $1 } END { print sum }' $1`

#sum=0; for line in `cat $1`; do sum=$((sum + $((10#$line)))); done; echo $sum