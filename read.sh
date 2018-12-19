#!/bin/bash

if [ $# -eq 0 ]; then
	echo "missing parameter"
	exit 0
fi

stty -F /dev/ttyUSB$1 ispeed 115200 ospeed 115200 -hupcl
#stty -F /dev/$1 ispeed 115200 ospeed 115200 -ignpar cs8 -cstopb -echo
#stty -F /dev/$1 cread raw sane 115200

while true; do
	while read -d $'\n' line < /dev/ttyUSB$1; do
		#echo $line
		echo $line | grep -oP '\[\K[^\]]+'
	done
done

exit 0
