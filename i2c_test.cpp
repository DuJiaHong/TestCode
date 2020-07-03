
#include<stdio.h>
#include<linux/types.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/ioctl.h>
#include<string.h>
#include<linux/i2c.h>
#include<linux/i2c-dev.h>
 
#define MAX_BYTES            2 
#define DEFAULT_I2C_BUS      "/dev/i2c-1"
 
int
i2c_write(int fd,
		 unsigned int addr,
		 unsigned int offset,
		 unsigned char *buf,
		 unsigned int len)
{
	struct i2c_rdwr_ioctl_data msg_rdwr;
	struct i2c_msg i2cmsg;
	unsigned int i;
	unsigned char _buf[MAX_BYTES+1];
 
	if(len>MAX_BYTES)
	{
		return -1;
	}
 
	if(offset+len>256)
	{
		return -1;
	}
 
	_buf[0]=offset;
	for(i=0;i<len;i++)
	{
	    _buf[1+i]=buf[i];
	    printf("----_writedata:%x------\n",_buf[1+i]);
	}
 
	msg_rdwr.msgs = &i2cmsg;
	msg_rdwr.nmsgs = 1;
 
	i2cmsg.addr  = addr;
	i2cmsg.flags = 0;
	i2cmsg.len   = 1+len;
	i2cmsg.buf   = _buf;
 
	if((i=ioctl(fd,I2C_RDWR,&msg_rdwr))<0){
		perror("ioctl()");
		fprintf(stderr,"ioctl returned %d\n",i);
		return -1;
	}
 
	return 0;
}
 
int
i2c_read(int fd,
			unsigned int addr,
			unsigned int offset,
			unsigned char *buf,
			unsigned int len)
{
	struct i2c_rdwr_ioctl_data msg_rdwr;
	struct i2c_msg i2cmsg;
	int i;
 
	if(len>MAX_BYTES)
	{
		return -1;
	}
 
	if(i2c_write(fd,addr,offset,NULL,0)<0)
	{
	    return -1;
	}
	
	msg_rdwr.msgs = &i2cmsg;
	msg_rdwr.nmsgs = 1;
	
	i2cmsg.addr  = addr;
	i2cmsg.flags = I2C_M_RD;
	i2cmsg.len   = len;
	i2cmsg.buf   = buf;
 
 
	if((i=ioctl(fd,I2C_RDWR,&msg_rdwr))<0){
	    perror("ioctl()");
	    fprintf(stderr,"ioctl returned %d\n",i);
	    return -1;
	}
 
	return 0;
}
 
 
 
int main()
{
	printf("----start---------\n");
 
	
	int fd =open(DEFAULT_I2C_BUS, O_RDWR);
 
	if (fd< 0) 
	{
		printf("open failed/n");
		return -1;
	}
 
	unsigned int addr = 0x20;
	unsigned int offset = 0x04;
	unsigned char writebuf[2]={0x26,0x52};
	unsigned char readbuf[2];
	unsigned int len = 2;
	
	i2c_read(fd,addr,offset,readbuf,len);
	printf("----i2c_read--write before--buff:%x-----\n",readbuf[0]);
	printf("----i2c_read--write before--buff:%x-----\n",readbuf[1]);
	
	memset(readbuf,0,sizeof(readbuf));
 
	//i2c_write(fd,addr,offset,writebuf,len);
	
//	i2c_read(fd,addr,offset,readbuf,len);
	printf("----i2c_read--write after--buff:%x-----\n",readbuf[0]);
	printf("----i2c_read--write after--buff:%x-----\n",readbuf[1]);
	printf("----end---------\n");
	close(fd);
	return 0;
}