#!/bin/bash

# run as root
if [ "$EUID" -ne 0 ]
then
	sudo ./clear.sh $@
  exit 0
fi

echo "Killing old background jobs"
pkill -f readmax.sh
pkill -f max.sh

echo "Deleting tmp files"

rm -rf "tmp_ch_values"

if [ -f nohop.out ]
then
  echo "Deleting output file"
  rm nohup.out
fi

echo "Done!"

exit 0
