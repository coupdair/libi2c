# install

make
make example
ls *.so objs/

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

Work In Progress i2c_get <=> i2cget

### read HRT

~~~ { .bash }
#LD_LIBRARY_PATH=./ ./objs/i2c_get 1 0x18 0x05 2
LD_LIBRARY_PATH=./ ./objs/i2c_get 1 0x18 1 16

Using i2c_oper r/w data
Device address: 0x18, tenbit: False, internal(word) address: 1 bytes, page max 16 bytes, delay: 1ms
Read data:

c1 86 

#i.e. 24.375
#e.g. units "(0x1*16)+(0x86/16)" " " -t
~~~
