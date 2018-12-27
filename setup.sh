#!/bin/bash

# run as root
if [ "$EUID" -ne 0 ]
then
	sudo ./setup.sh $@
  exit 0
fi

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
	echo -n "Setting up /dev/ttyUSB$var"

	# set up serial connection
	stty -F /dev/ttyUSB$var ispeed 115200 ospeed 115200 -hupcl 2> /dev/null

	# set ID
	sudo ./write.sh "setid $var" "$var"

  if [ $? -ne 0 ]
  then
    echo -n "...ERROR, removing it now"
    rm /dev/ttyUSB$var
  fi

	echo "...OK"

done

exit 0
