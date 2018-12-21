#!/bin/bash

# run as root
if [ "$EUID" -ne 0 ]
then
	sudo ./syncmax.sh $@
  exit 0
fi

echo "Starting..."

# check arguments
if [ $# -lt 1 ]; then
	echo -n "Auto detecing UART devices..."

	ports=$(ls /dev/ttyUSB[0-9]* 2> /dev/null | grep -oP [0-9]*$)

	if [ ${#ports} -gt 0 ]
	then
		echo "OK"
		/bin/bash $PWD/syncmax.sh $ports
	else
		echo "No UART interfaces found"
	fi

	exit 0
fi

echo "Started"

# close background tasks from last time
#pkill -f readmax.sh

# remove old files
rm -rf "tmp_ch_values"

# create folder again
mkdir "tmp_ch_values"

for var in "$@"
do
	# set up serial connection
	stty -F /dev/ttyUSB$var ispeed 115200 ospeed 115200 -hupcl

	# stop on error
	if [ $? -ne 0 ]
	then
		echo "Error at ttyUSB$var"
		exit 0
	fi

	# start reader
	#echo "$var"
	nohup ./readmax.sh "$var" > /dev/null 2> /dev/null &
done

# declare max data array
declare -a maxdata=(0 0 0 0 0)

# every second
while sleep 1
do
	# reset all values
	maxdata=(0 0 0 0 0)
	
	#echo "update"
	# for all files in folder
	FILES=./tmp_ch_values/*
	for file in $FILES
	do
		#echo "$file"
		if [ -f $file ]
		then

			data=$(cat $file | tail -n 1)

			#echo -n "$(echo $file | tail -c 3): "
			#echo "$data"

			#cat $file

			# convert data to array
			IFS=',' read -a array <<< $data

			for index in "${!array[@]}"
			do
				declare -i tmp_element=${array[index]}
				declare -i max_element=${maxdata[index]}
				#echo "tmp: $tmp_element, max: $max_element"

				if [ $tmp_element -gt $max_element ]
				then
					maxdata[index]=$tmp_element
				fi
			done
		fi
	done

	max_arr_str="setmax [${maxdata[0]},${maxdata[1]},${maxdata[2]},${maxdata[3]},${maxdata[4]}]"
	#echo $max_arr_str

	./write.sh "$max_arr_str" $@
	echo "$max_arr_str"
	#echo "write.sh $max_arr_str $@"
done

exit 0
