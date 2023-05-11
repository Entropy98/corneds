#!/bin/bash

if [ -z $1 ]
then
  echo "No binary specified. There's nothing to flash..."
  exit 1
fi

pyocd flash -t rp2040 $1 -u E6614103E7632B2F
