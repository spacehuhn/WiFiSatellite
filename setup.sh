#!/bin/bash

if [ $# -eq 0 ]; then
		ports=$(ls /dev/ttyUSB[0-9]* 2> /dev/null | grep -oP [0-9]*$)

		if [ ${#ports} -gt 0 ]
		then
				sudo ./setup.sh $ports
		else
			echo "No UART interfaces found"
		fi

		exit 0
fi

for var in "$@"
do
	# set up serial connection
	stty -F /dev/ttyUSB$var ispeed 115200 ospeed 115200 -hupcl 2> /dev/null

  if [ $? -ne 0 ]
  then
    echo "Error on port ttyUSB$var, removing it now"
    rm /dev/ttyUSB$var
  fi

done

exit 0
