#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "i2c/i2c.h"

void print_i2c_data(const unsigned char *data, size_t len)
{
    size_t i = 0;

    for (i = 0; i < len; i++) {

        if (i % 16 == 0) {

            fprintf(stdout, "\n");
        }

        fprintf(stdout, "%02x ", data[i]);
    }

    fprintf(stdout, "\n");
}


int main(int argc, char **argv)
{
    char i2c_dev_desc[128];
    I2C_WRITE_HANDLE i2c_write_handle = i2c_write;
    unsigned int addr = 0, iaddr=0, num_bytes=1, bus_num = -1;
    unsigned int value=0;
///Usage
    if (argc < 5) {

        fprintf(stdout, "Usage:%s <bus_num> <dev_addr> <value>\n"
                "Such as:\n"
                "%s 1 0x18 0x08 0x3\n", argv[0], argv[0]);
        exit(0);
    }
///CLI option parsing
    /* Get i2c bus number */
    if (sscanf(argv[1], "%u", &bus_num) != 1) {

        fprintf(stderr, "Can't parse i2c 'bus_num' [%s]\n", argv[1]);
        exit(-1);
    }

    /* Get i2c device address */
    if (sscanf(argv[2], "0x%x", &addr) != 1) {

        fprintf(stderr, "Can't parse i2c 'dev_addr' [%s]\n", argv[2]);
        exit(-1);
    }
    
    /* Get i2c internal address bytes */
    if (sscanf(argv[3], "0x%x", &iaddr) != 1) {

        fprintf(stderr, "Can't parse i2c 'iaddr' [%s]\n", argv[3]);
        exit(-2);
    }

    /* Get i2c number of bytes */
    if (sscanf(argv[4], "%x", &value) != 1) {

        fprintf(stderr, "Can't parse i2c 'value' [%s]\n", argv[4]);
        exit(-2);
    }
    /* Open i2c bus */
    int bus;
    char bus_name[32];
    memset(bus_name, 0, sizeof(bus_name));

    if (snprintf(bus_name, sizeof(bus_name), "/dev/i2c-%u", bus_num) < 0) {

        fprintf(stderr, "Format i2c bus name error!\n");
        exit(-3);
    }
///open bus, i.e. fd
    if ((bus = i2c_open(bus_name)) == -1) {

        fprintf(stderr, "Open i2c bus:%s error!\n", bus_name);
        exit(-3);
    }

    /* Init i2c device */
///setup device desc.
    I2CDevice device;
    memset(&device, 0, sizeof(device));
    //default init.
    i2c_init_device(&device);
    //specific init., e.g. from CLI
    device.bus = bus;
    device.addr = addr & 0x3ff;

///show device desc.
    /* Print i2c device description */
    fprintf(stdout, "%s\n", i2c_get_device_desc(&device, i2c_dev_desc, sizeof(i2c_dev_desc)));

    size_t i = 0;
    ssize_t ret = 0;
    unsigned char buf[256];
    size_t buf_size = sizeof(buf);
    memset(buf, 0, buf_size);
///fill data
    buf[0]=value;
    /* Print before write */
    fprintf(stdout, "Write data:\n");
    print_i2c_data(buf, num_bytes);
///write data
    ret = i2c_write_handle(&device, iaddr, buf, num_bytes);
    //if(ret==-1)
    if((size_t)ret != num_bytes)
    {
        fprintf(stderr, "Write i2c error!\n");
        exit(-4);
    }

    i2c_close(bus);
    return 0;
}//main

