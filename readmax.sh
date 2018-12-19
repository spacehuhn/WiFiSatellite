if [ $# -lt 1 ]; then
	echo "missing parameters"
	exit 0
fi

# set serial connection options
stty -F /dev/ttyUSB$1 ispeed 115200 ospeed 115200 -hupcl

# read line
while read -d $'\n' line < /dev/ttyUSB$1
do
	# check that line begins with max
	beginning=$(echo $line | cut -c1-3)
	if [ $beginning == "max" ]
	then
		# get channel
		declare -i ch=$(echo $line | cut -c4-5)
		#echo $ch

		# get data from line
		data=$(echo $line | grep -oP '\[\K[^\]]+')

		# convert data to array
		IFS=',' read -a array <<< $data

		for index in "${!array[@]}"
		do
			declare "maxdata_$ch_$index"="${array[index]}"
		done
	fi
done

exit 0
