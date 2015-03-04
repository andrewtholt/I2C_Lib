#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <stdlib.h>

// Helper library for usv-i2c device.


struct termios  options;

int 
i2c_open_port(char *port)
{
	int             fd;
	// File descriptor for the port
    
    printf("The port is %s\n",port);
    fd = open(port, O_RDWR | O_NOCTTY | O_NONBLOCK);
	if (fd == -1)
	{
		perror("open_port: Unable to open port - ");
		// Could not open the port.
	} else
	{
		fcntl(fd, F_SETFL, 0);
        
		//Get the current options for the port...
        tcgetattr(fd, &options);
        
		//Set the baud rates to 19200...
        cfsetispeed(&options, B19200);
        cfsetospeed(&options, B19200);
        
		//Enable the receiver and set local mode...
        options.c_cflag |= (CLOCAL | CREAD);
        
		//Set no parity bit
        options.c_cflag &= ~PARENB;
        
		//Set 2 stop bits
        options.c_cflag &= ~CSTOPB;
        
		//Set the character size
        options.c_cflag &= ~CSIZE;
		options.c_cflag |= CS8;
        
		//Set the new options for the port...
        tcsetattr(fd, TCSANOW, &options);
        
		fcntl(fd, F_SETFL, FNDELAY);
	}
	return (fd);
}

/*
    Data passed consistes of:
*/
int i2c_write( int fd, char addr, char reg,char *ptr, int len) {
    char sbuf[255];
    int r=-1;
    int i=0;

    sbuf[0]=0x55;           // r/w to 1 byte addressed devices
    sbuf[1]=addr & 0xfe;    // Address (Write)
    sbuf[2]=reg & 0xff;     // register
    sbuf[3]= len  & 0xff;
    
    memcpy( &sbuf[4], ptr, len);
    r=write(fd,sbuf,len + 4 );
//    usleep(750000);
    return(r);
}

int i2c_read( int fd, char addr, char reg,char *ptr, int len) {
    char sbuf[255];
    int r=-1;
    int i=0;

    sbuf[0]=0x55;           // r/w to 1 byte addressed devices
    sbuf[1]=(addr & 0xff) | 0x01 ;    // Address (read)
    sbuf[2]=reg & 0xff;     // register
    sbuf[3]= len  & 0xff;
    
//    memcpy( &sbuf[4], ptr, len);
    r=write(fd,sbuf,4 );
    usleep(750000);
    bzero(sbuf, len+1);

    r = read(fd, ptr, len+1);
    return(r);
}

void i2c_close(int fd) {
    close(fd);
}

/*
int main(int argc,char *argv[])
{
	int             fd;
	unsigned char   sbuf[64];
	int             x = 0;
	int             output = 0;
	int             r = 0;
    int i=0;
    
    printf("argc=%d\n",argc);
    if(argc !=2 ) {
        printf("Requires port name as an argument.\n");
        exit(0);
    }
    
    fd = open_port(argv[1]);
    if(fd > 0) {
        sbuf[0] = 0x55;
        sbuf[1] = 0x70;
        sbuf[2] = 0x01;
        sbuf[3] = 0x01;
        sbuf[4] = 0x00;
        
        r=write(fd, sbuf, 5 );
        
        printf("Wrote %d bytes\n",r);
        usleep(750000);
        
        sbuf[0] = 0x55;
        sbuf[1] = 0x71;
        sbuf[2] = 0x01;
        sbuf[3] = 0x01;
        r=write(fd, sbuf, 4 );
        usleep(750000);
        
        r = read(fd, sbuf, 3);
        if (r < 0) {
            perror("read() failed!!");
        } else {
            printf("Read %d bytes\n",r);
        }
        
        for(i=0; i < r; i++) {
            printf("\nOutput[%d]: %02x\n\n", i,sbuf[i]);
        }

        close(fd);
    } else {
        exit(-1);
    }
}
*/
