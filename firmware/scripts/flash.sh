#!/bin/bash

if [ -z $1 ]
then
  echo "No binary specified. There's nothing to flash..."
  exit 1
fi

if [ -z $2 ]
then
  probe_id=""
else
  probe_id="-u $2"
fi

pyocd flash -t rp2040 $1 $probe_id
