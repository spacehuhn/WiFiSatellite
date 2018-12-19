#!/bin/bash

# check arguments
if [ $# -lt 1 ]; then
	echo "missing parameters"
	exit 0
fi

# create folder if not exists
if [ ! -d "tmp_ch_values" ]
then
	mkdir "tmp_ch_values"
fi

# variable to save last channel
declare -i ch=0
declare -i lastch=0

# read line
while read -d $'\n' line < /dev/ttyUSB$1
do
	# check that line begins with max
	beginning=$(echo $line | cut -c1-3)
	if [ "$beginning" == "max" ]
	then
		# get channel
		ch=$(echo $line | cut -c4-5)

		# delete old file on channel change
		if [ ! "$ch" = "$lastch" ]
		then
			if [ -f "tmp_ch_values/0$lastch" ]
			then
				rm "tmp_ch_values/0$lastch"
			fi

			lastch=$ch
			touch "tmp_ch_values/0$ch"
			echo "Changed channel to $ch"
		fi

		# get data from line
		data=$(echo $line | grep -oP '\[\K[^\]]+')

		# write data into file
		echo "$data" > "tmp_ch_values/0$ch"
	fi
done

# delete old file
if [ ! "$lastch" == "0" ]
then
	rm "tmp_ch_values/0$lastch"
fi

exit 0
