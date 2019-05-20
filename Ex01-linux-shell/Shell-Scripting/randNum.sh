#!/bin/bash
fileName="$1"
ten=10
hundred=100
zero='0'
for i in {1..500000}
do
    tmp=$((1 + RANDOM % 999))
    if [ ${#tmp} -lt 2 ]
    then
        tmp="$zero$tmp"
    fi
    if [ ${#tmp} -lt 3 ]
    then
        tmp="$zero$tmp"
    fi
    echo "$tmp"
done >> $fileName