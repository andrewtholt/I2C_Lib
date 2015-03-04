#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <stdlib.h>

#define PORT "/dev/ttyUSB0"

struct termios options;

int open_port(char *name) {

    int fd; // File descriptor for the port
    fd = open(name, O_RDWR | O_NOCTTY);

    if (fd == -1) {
        perror("open_port: Unable to open /dev/ttyUSB0 - "); // Could not open the port.
        exit(-1);
    } else {
        fcntl(fd, F_SETFL, 0);
        
        // Get the current options for the port...
        tcgetattr(fd, &options);
        
        // Set the baud rates to 19200...
        cfsetispeed(&options, B19200);
        
        // Enable the receiver and set local mode...
        options.c_cflag |= (CLOCAL | CREAD);
        
        // Set no parity bit
        options.c_cflag &= ~PARENB;
        
        // Set 2 stop bits
        options.c_cflag &= ~CSTOPB;
        
        // Set the character size
        options.c_cflag &= ~CSIZE;
        options.c_cflag |= CS8;
        
        // Set the new options for the port...
        tcsetattr(fd, TCSANOW, &options);
        
        fcntl(fd, F_SETFL, FNDELAY);
    }
    return (fd);
}

int main(int argc, char *argv[]) {
    int fd;
    unsigned char sbuf[64];
    int x = 0;
    
    printf("argc - %d\n",argc);

    if( argc == 1 ) {
        fd = open_port(PORT);
    } else {
        fd = open_port(argv[1]);
    }

    while(x < 4) {
        // set ranging mode in cm
        
        sbuf[0] = 0x55;
        sbuf[1] = 0xc6; // i2c address
        sbuf[2] = 0x00; // register
        sbuf[3] = 0x01; // Number of bytes
        sbuf[4] = 0x06;   // Values
        
        write(fd, sbuf, 5);
        usleep(750000);

        printf("Written\n");
        
        sbuf[0] = 0x55;
        sbuf[1] = 0xC6; // i2c address
        sbuf[2] = 0x03; // Register address
        sbuf[3] = 0x01; // Count
        
        write(fd, sbuf, 4);
        usleep(750000);
        
        int r = read(fd, sbuf, 1);
        if(r < 0)
            printf("read() failed!!");
        
        
        int output = sbuf[0];
        printf("output: %02x\n\n", output);
        
        x++;
    }
    
    close(fd);
}

