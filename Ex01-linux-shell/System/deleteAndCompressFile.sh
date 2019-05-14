#!/bin/bash
IFS=$'\t\n'
fileArr=(`find $HOME -type f -size +100k`)
unset $IFS
for file in ${fileArr[*]}
do
	echo "File name:" $file
	echo "Delete or Compress or None this file?"
	echo "1. Delete"
	echo "2. Compress"
	echo "3. None"
	echo "Choose: "

	read choose
	
	case $choose in
	1)
	rm -f $file
	dt=$(date '+%d/%m/%Y %H:%M:%S')
	printf "File name: %s - Time: %s\n" $file $dt >> log.txt
	;;
	2)
	tar -cvzf $file.tar.gz $file
	;;
	3)
	continue
	;;
	*)
	echo "Exit"
	break
	esac
done

