#!/bin/bash

#device address list
list="18 19 1a 1b 1c 1d"

#I2C get program
get="../objs/i2c_get"
#bitset
bit="/home/$USER/bin/bitset"

#temp. in degree Celcius
for i in $list
do
  w=`LD_LIBRARY_PATH=../ $get 1 0x$i 0x05 2 | tail -n 1`
  ub=0x`echo $w | cut -d' ' -f1`
  lb=0x`echo $w | cut -d' ' -f2`
  #remove flag bits
  ubc=`$bit -s 8 -x $ub --and -X 0x1F | tail -n 1`
  #echo 'UpperByte: '$ub' -> '$ubc
  t=`units "($ubc*16)+($lb/16)" " " -t`
  echo $t" °C"
done

