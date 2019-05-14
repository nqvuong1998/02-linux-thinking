#!/bin/bash
echo "Sum =" `awk '{ sum += $1 } END { print sum }' $1`
