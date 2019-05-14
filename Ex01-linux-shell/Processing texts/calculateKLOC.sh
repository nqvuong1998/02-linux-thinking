#!/bin/bash
sum=0
for file in $(find -regex '.*/.*\.\(c\|cpp\|h\)$' -print0 | xargs -0)
do
    #Loriginal=`wc -l $file`
    Lnon_blank=`sed '/^\s*$/d' $file > $file.out | wc -l`
    Lcode=$(sed '/^\s*\/\/.*/d;/\/\*/{ N; s/\/\*.*\*\/// }' $file.out | wc -l)
    sum=$(($sum+$Lcode))
    rm $file.out
done
echo $sum


