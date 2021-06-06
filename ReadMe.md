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

TODO i2c_get <=> i2cget

### read HRT

~~~ { .bash }
LD_LIBRARY_PATH=./ ./objs/i2c_get 1 0x18 0x05 2
~~~
