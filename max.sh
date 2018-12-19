if [ $# -lt 1 ]; then
	echo "missing parameters"
	exit 0
fi

for var in "$@"
do
	/bin/bash ~/Documents/readmax.sh $var &
done

while sleep 1
do
	for var in "$@"
	do
		varname="maxdata_$var_0"
        	echo "${!varname}"
	done
done

exit 0
