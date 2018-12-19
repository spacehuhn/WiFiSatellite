#!/bin/bash

while true
do
  echo "time $(date +%s)"
  sudo ./write.sh "time $(date +%s)"
  sleep 30
done

exit 0
