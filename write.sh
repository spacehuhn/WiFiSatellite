if [ $# -lt 2 ]; then
	echo "missing parameters"
	exit 0
fi

for var in "${@:2}"
do
	stty -F /dev/ttyUSB$var ispeed 115200 ospeed 115200 -hupcl
	echo $1 > /dev/ttyUSB$var
done

exit 0
