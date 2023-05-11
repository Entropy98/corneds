#!/bin/bash

if [ -z $1 ]
then
  echo "No binary specified. This debugger won't know much..."
  exit 1
fi

pyocd gdb -t rp2040 &
SVR_PID=$!
sleep 1s
gdb-multiarch --data-directory=/usr/share/gdb $1
DBG_PID=$!
wait ${DBG}
kill -9 ${SVR_PID}
