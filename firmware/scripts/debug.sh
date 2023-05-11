#!/bin/bash
pyocd gdb -t rp2040 &
SVR_PID=$!
sleep 1s
gdb-multiarch --data-directory=/usr/share/gdb $1
DBG_PID=$!
wait ${DBG}
kill -9 ${SVR_PID}
