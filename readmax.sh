#!/bin/bash

# run as root
if [ "$EUID" -ne 0 ]
then
	sudo ./readmax.sh $@
  exit 0
fi

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
	echo "$line"
	if [ "$beginning" == "max" ]
	then
		# get channel
		ch=$(echo $line | cut -c4-6)

		#echo "channel=$ch"

		# delete old file on channel change
		if [ ! "$ch" = "$lastch" ]
		then
			if [ -f "tmp_ch_values/$lastch" ]
			then
				rm "tmp_ch_values/$lastch"
			fi

			lastch=$ch
			touch "tmp_ch_values/$ch"
			echo "Changed channel to $ch"
		fi

		# get data from line
		data=$(echo $line | grep -oP '\[\K[^\]]+')

		# write data into file
		echo "$data" > "tmp_ch_values/$ch"
	fi
done

# delete old file
if [ ! "$lastch" == "0" ]
then
	rm "tmp_ch_values/$lastch"
fi

exit 0
