# install

~~~ { .bash }
make
make example
ls *.so objs/
~~~

# use

## i2c_tools

### help

~~~ { .bash }
LD_LIBRARY_PATH=./ ./objs/i2c_tools
Usage:./objs/i2c_tools <bus_num> <dev_addr> <iaddr_bytes> <page_bytes> [ioctl]
Such as:
	i2c_tools 1 0x50 1 8
	i2c_tools 1 0x50 1 16
	i2c_tools 1 0x50 2 32
	i2c_tools 1 0x50 2 ioctl
~~~

## i2c_get

- i2c_get <=> i2cget
      - read short, ok

### help

~~~ { .bash }
LD_LIBRARY_PATH=./ ./objs/i2c_get
Usage:./objs/i2c_get <bus_num> <dev_addr> <iaddr_bytes> <page_bytes> [ioctl]
Such as:
./objs/i2c_get 1 0x18 0x05 2
~~~

### read HRT

~~~ { .bash }
LD_LIBRARY_PATH=./ ./objs/i2c_get 1 0x18 0x05 2

Using i2c_oper r/w data
Device address: 0x18, tenbit: False, internal(word) address: 1 bytes, page max 16 bytes, delay: 1ms
internal register address=0x05
Read data:

c1 89 

#i.e. 24.5625
#e.g. units "(0x1*16)+(0x89/16)" " " -t
~~~

equivalent to

~~~ { .bash }
i2cget -y 1 0x18 0x05 w

0x91c1
~~~

### read many HRT devices

~~~ { .bash }
#raw
for i in 18 19 1a 1b 1c 1d; do LD_LIBRARY_PATH=./ ./objs/i2c_get 1 0x$i 0x05 2 | tail -n 1; done

c1 8b 
c1 8b 
c1 90 
c1 94 
c1 95 
c1 95 
~~~

~~~ { .bash }
#temp. in degree Celcius
./get_temps.sh

24.625 °C
24.625 °C
24.9375 °C
25.1875 °C
25.3125 °C
25.1875 °C
~~~

or

~~~ { .bash }
#temp. in degree Celcius
for i in 18 19 1a 1b 1c 1d
do
  w=`LD_LIBRARY_PATH=../ ../objs/i2c_get 1 0x$i 0x05 2 | tail -n 1`
  ub=0x`echo $w | cut -d' ' -f1`
  lb=0x`echo $w | cut -d' ' -f2`
  #remove flag bits
  ubc=`~/bin/bitset -s 8 -x $ub --and -X 0x1F | tail -n 1`
  #echo 'UpperByte: '$ub' -> '$ubc
  t=`units "($ubc*16)+($lb/16)" " " -t`
  echo $t" °C"
done

24.6875 °C
24.6875 °C
25 °C
25.1875 °C
25.375 °C
25.3125 °C
~~~

### read HRT dev. id and rev.

reg.0x07:

- dev. id =  0x04 (ub)
- rev., e.g. 0x00 (lb)

~~~ { .bash }
LD_LIBRARY_PATH=./ ./objs/i2c_get 1 0x18 0x07 2

Using i2c_oper r/w data
Device address: 0x18, tenbit: False, internal(word) address: 1 bytes, page max 16 bytes, delay: 1ms
internal register address=0x07
reading 2 bytes
Read data:

04 00 
~~~

equivalent to

~~~ { .bash }
i2cget -y 1 0x18 0x07 w

0x0004
~~~

## i2c_get_temperature

i2c_get_temperature on MCP9808

### read single HRT device

~~~ { .bash }
LD_LIBRARY_PATH=./ ./objs/i2c_get_temperature 1 0x18

Device address: 0x18, tenbit: False, internal(word) address: 1 bytes, page max 8 bytes, delay: 1ms
internal register address=0x05
reading 2 bytes
Read data: 0xc1 0x90
Read temperature: 25.000 Celcius
~~~

### read many HRT devices

~~~ { .bash }
for i in 18 19 1a 1b 1c 1d; do LD_LIBRARY_PATH=./ ./objs/i2c_get_temperature 1 0x$i | grep Celcius; done

Read temperature: 25.000 Celcius
Read temperature: 24.938 Celcius
Read temperature: 25.125 Celcius
Read temperature: 25.375 Celcius
Read temperature: 25.438 Celcius
Read temperature: 25.375 Celcius
~~~

