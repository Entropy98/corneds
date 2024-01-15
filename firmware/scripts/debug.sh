#!/bin/bash

if [ -z $1 ]
then
  echo "No binary specified. This debugger won't know much..."
  exit 1
fi

gdb-multiarch --data-directory=/usr/share/gdb $1
