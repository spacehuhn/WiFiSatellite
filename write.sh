#!/bin/bash

# run as root
if [ "$EUID" -ne 0 ]
then
	sudo ./write.sh $@
  exit 0
fi

if [ $# -eq 0 ]
then

	echo "Type your command my lord:"
	while read CMD
	do
			./write.sh "$CMD"
	done

elif [  $# -lt 2  ]
then
		ports=$(ls /dev/ttyUSB[0-9]* 2> /dev/null | grep -oP [0-9]*$)

		if [ ${#ports} -gt 0 ]
		then
			./write.sh "$1" $ports
		else
			echo "No UART interfaces found"
		fi

		exit 0
fi

for var in "${@:2}"
do
	#stty -F /dev/ttyUSB$var ispeed 115200 ospeed 115200 -hupcl
	echo $1 > /dev/ttyUSB$var
done

exit 0
