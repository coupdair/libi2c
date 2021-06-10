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
    I2C_READ_HANDLE i2c_read_handle = i2c_read;
    unsigned int addr=0, iaddr=0x05, num_bytes=2, iaddr_bytes = 1, page_bytes = 16, bus_num = -1;
///Usage
    if (argc < 3) {

        fprintf(stdout, "Usage:%s <bus_num> <dev_addr>\n"
                "Such as:\n"
                "%s 1 0x18\n", argv[0], argv[0]);
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
    device.page_bytes = page_bytes;
    device.iaddr_bytes = iaddr_bytes;

///show device desc.
    /* Print i2c device description */
    fprintf(stdout, "%s\n", i2c_get_device_desc(&device, i2c_dev_desc, sizeof(i2c_dev_desc)));
    fprintf(stdout, "internal register address=0x%02x\n", iaddr);
    fprintf(stdout, "reading %d bytes\n", num_bytes);

    ssize_t ret = 0;
    unsigned char buf[16];
    size_t buf_size = sizeof(buf);

    /* Read */
    memset(buf, 0, buf_size);

///read data
    ret = i2c_read_handle(&device, iaddr, buf, num_bytes);
    if (ret == -1 || (size_t)ret != num_bytes)
    {

        fprintf(stderr, "Read i2c error!\n");
        exit(-5);
    }

    /* Print read result * /
    fprintf(stdout, "Read data:\n");
    print_i2c_data(buf, num_bytes);
*/
    i2c_close(bus);
    
    //print temperature in degree Celcius
    int ub=buf[0];
    int lb=buf[1];
    float temperature=-99.99;
    fprintf(stdout, "Read data: 0x%x 0x%x\n",buf[0],buf[1]);
    ub=ub&0x1F;//Clear flag bits
    if( (ub&0x10) == 0x10)
    {//Ta < 0°C
      ub=ub&0x0F;//Clear SIGN
      temperature=256.0-(ub*16.0+lb/16.0);
    }
    else
    {//Ta ≥ 0°C
      temperature=(ub*16.0+lb/16.0);//Temperature = Ambient Temperature (°C)
    }
    fprintf(stdout, "Read temperature: %06.3f Celcius\n",temperature);
    return 0;
}//main
