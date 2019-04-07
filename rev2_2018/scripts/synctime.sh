#!/bin/bash

# run as root
if [ "$EUID" -ne 0 ]
then
	sudo ./synctime.sh $@
  exit 0
fi

while true
do
  echo "time $(date +%s)"
  sudo ./write.sh "time $(date +%s)"
  sleep 30
done

exit 0
