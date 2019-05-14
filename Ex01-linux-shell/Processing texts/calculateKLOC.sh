#!/bin/bash
count=$(find $1 -regex '.*/.*\.\(c\|cpp\|h\)$' -print0 | xargs -0 cat | sed '$a\\n' | sed ':a;N;$!ba;s/\/\*.*\*\//\n/' | sed '/^\s*\/\/.*/d;/^\s*$/d' | wc -l)
echo "$count / 1000" | bc -l
