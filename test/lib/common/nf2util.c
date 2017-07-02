/*
 * Copyright (c) 2006-2011 The Board of Trustees of The Leland Stanford Junior
 * University
 *
 * Author: Glen Gibb <grg@stanford.edu>
 *
 * We are making the NetFPGA tools and associated documentation (Software)
 * available for public use and benefit with the expectation that others will
 * use, modify and enhance the Software and contribute those enhancements back
 * to the community. However, since we would like to make the Software
 * available for broadest use, with as few restrictions as possible permission
 * is hereby granted, free of charge, to any person obtaining a copy of this
 * Software) to deal in the Software under the copyrights without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to the
 * following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * The name and trademarks of copyright holder(s) may NOT be used in
 * advertising or publicity pertaining to the Software or any derivatives
 * without specific, written prior permission.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>  
#include <fcntl.h>
#include <stdint.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/ioctl.h>

#include <net/if.h>

#include <arpa/inet.h>
#include "nf2.h"
#include "nf2util.h"

#include "reg_defines.h"
int readReg(struct nf2device *nf2, unsigned reg, unsigned *val)
{

	int fd;
	int i, linenum, count=0;
	unsigned char * map_base;
	unsigned long PHYADDR, length;
   	unsigned char val_str[4];
   	unsigned temp_val;
	/* get pagesize on the specific device */
	int  pagesize = getpagesize();
	/* turn a strig into a unsigned long integer */
	/* PHYADDR = reg + DEVICE_ADDRESS; */
    PHYADDR = reg;
	// printf("READ PHYADDR = 0x%08x\n", PHYADDR);
	length = 1;
	//printf("length = %d words\n", length);

	/* open a descriptor to /dev/mem */
	fd = open("/dev/mem", O_RDWR|O_SYNC);
	if(fd == -1)  
	{  
		printf("open /dev/mem failed\n");
		return -1;  
	}
	// printf("BREAK ONE\n");
	/* mmap() maps the physical address into virtual address, including IO and hardware registers
	* the start address is NULL, so kernel will locate it at any possible place.
	* with read and write permission
	* the offset is aligned with a memory page
	*/
	map_base = mmap(NULL, pagesize, PROT_READ|PROT_WRITE, MAP_SHARED, fd, PHYADDR & 0xfffff000);
	if(map_base == MAP_FAILED) {
		perror("mmap");
	};/*

	else
		printf("map_base = 0x%08X\n", map_base);
	printf("\n");*/
	// printf("BREAK TWO\n");

	/* get the within-page offset */
	int pgoffset = PHYADDR & 0x00000fff;
	
	/* print 4 registers in a line */
	while(length>4) {
		linenum=4;
		length-=4;
		printf("%08X: ", (PHYADDR + count));
		for(i=0;i<4*linenum;i=i+4) {
			printf("%02X%02X%02X%02X    ",  (unsigned int)map_base[pgoffset + count + i+3],
							(unsigned int)map_base[pgoffset + count + i+2],
							(unsigned int)map_base[pgoffset + count + i+1],
							(unsigned int)map_base[pgoffset + count + i]
							);
		}
		printf("\n");
		count+=16;
	}
	// printf("%08X: ", (PHYADDR + count));
	// printf("BREAK THREE , length is %x\n" , length);
	// for(i=0;i<4*length;i=i+4) {
	// 	sprintf(val_str, "%02X%02X%02X%02X    ",  (unsigned int)map_base[pgoffset + count + i+3],
	// 					(unsigned int)map_base[pgoffset + count + i+2],
	// 					(unsigned int)map_base[pgoffset + count + i+1],
	// 					(unsigned int)map_base[pgoffset + count + i]
	// 					);
	// 	printf("BREAK FOUR\n");
	// 	printf("The val_str is %s\n",val_str);
 //     	// *val = strtoul(val_str,NULL,16);
 //     	printf("BREAK FIVE\n");
 //      	printf("VAL in readReg is %08X \n", *val);
	// }
	unsigned int c0,c1,c2,c3;
	printf("IN READ : Value at 0x%08X: 0x%02X%02X%02X%02X\n", PHYADDR,
					(unsigned int)map_base[pgoffset + 3],
					(unsigned int)map_base[pgoffset + 2],
					(unsigned int)map_base[pgoffset + 1],
					(unsigned int)map_base[pgoffset]
					);
	c0 = (unsigned int)map_base[pgoffset];
	c1 = (unsigned int)map_base[pgoffset + 1];
	c2 = (unsigned int)map_base[pgoffset + 2];
	c3 = (unsigned int)map_base[pgoffset + 3];
	/* *val = c3*0x1000000 + c2*0x10000 + c1*0x100 + c0; */
	*val = (c3 << 6) + (c2 << 4) +(c1 << 2) + c0;
	printf("c0 is %x , c1 is %x , c2 is %x , c3 is %x \n",c0,c1,c2,c3);
	printf("val is %x\n", *val);
	//printf("\n");
	// printf("BREAK SIX\n");

	/* close file descriptor */
	close(fd);
	/* release map */
	// printf("BREAK SEVEN\n");
	munmap(map_base, pagesize);
	// printf("BREAK EIGHT\n");
	return 0;
}
int check_iface(struct nf2device *nf2)
{
	struct stat buf;
	char filename[PATHLEN];

	/* See if we can find the interface name as a network device */

	/* Test the length first of all */
	if (strlen(nf2->device_name) > IFNAMSIZ)
	{
		fprintf(stderr, "Interface name is too long: %s\n", nf2->device_name);
		return -1;
	}

	/* Check for /sys/class/net/iface_name */
	strcpy(filename, "/sys/class/net/");
	strcat(filename, nf2->device_name);
	if (stat(filename, &buf) == 0)
	{
		fprintf(stderr, "Found net device: %s\n", nf2->device_name);
		nf2->net_iface = 1;
		return 0;
	}

	/* Check for /dev/iface_name */
	strcpy(filename, "/dev/");
	strcat(filename, nf2->device_name);
	if (stat(filename, &buf) == 0)
	{
		fprintf(stderr, "Found dev device: %s\n", nf2->device_name);
		nf2->net_iface = 0;
		return 0;
	}

	fprintf(stderr, "Can't find device: %s\n", nf2->device_name);
	return -1;
}

/*
 * Open the descriptor associated with the device name
 */
int openDescriptor(struct nf2device *nf2)
{
        struct ifreq ifreq;
	char filename[PATHLEN];
	struct sockaddr_in address;
	int i;
	struct sockaddr_in *sin = (struct sockaddr_in *) &ifreq.ifr_addr;
	int found = 0;

	if (nf2->net_iface)
	{
		/* Open a network socket */
		nf2->fd = socket(AF_INET, SOCK_DGRAM, 0);
		if (nf2->fd == -1)
		{
                	perror("socket: creating socket");
                	return -1;
		}
		else
		{
			/* Root can bind to a network interface.
			   Non-root has to bind to a network address. */
			if (geteuid() == 0)
			{
				strncpy(ifreq.ifr_ifrn.ifrn_name, nf2->device_name, IFNAMSIZ);
				if (setsockopt(nf2->fd, SOL_SOCKET, SO_BINDTODEVICE,
					(char *)&ifreq, sizeof(ifreq)) < 0) {
					perror("setsockopt: setting SO_BINDTODEVICE");
					return -1;
				}

			}
		}
	}
	else
	{
		strcpy(filename, "/dev/");
		strcat(filename, nf2->device_name);
		nf2->fd = fileno(fopen(filename, "w+"));
		if (nf2->fd == -1)
		{
                	perror("fileno: creating descriptor");
                	return -1;
		}
	}

	return 0;
}

int writeReg(struct nf2device *nf2, unsigned reg, unsigned val)
{
	int fd;
	// unsigned char * map_base;
	unsigned char char_len;
	unsigned int * map_base;
	unsigned long PHYADDR;
	unsigned int  newval;
	int  pagesize = getpagesize();

	/* PHYADDR = reg +0x43d30000; */
    /* PHYADDR = reg + DEVICE_ADDRESS; */
    PHYADDR = reg;
	printf("WRITE PHYADDR = 0x%08X\n",PHYADDR);
	newval = val;
	//printf("WRITE VAL IS %x and NEWVAL IS %x sizeof char_len is %d\n",val , newval , sizeof(char_len));
	fd = open("/dev/mem", O_RDWR|O_SYNC);
	if(fd == -1)  
	{  
		printf("open /dev/mem failed\n");
		return -1;  
	}

	map_base = mmap(NULL, pagesize, PROT_READ|PROT_WRITE, MAP_SHARED, fd, PHYADDR & 0xfffff000);
	if(map_base == MAP_FAILED) {
		perror("mmap");
	};
	/*else
		printf("mmap = 0x%08X\n", map_base);*/

	int pgoffset = PHYADDR & 0x00000fff;
	unsigned int c0,c1,c2,c3;
	/* seperate 4 bytes of a 32-bit register value */
	c0 = newval & 0x000000ff;
	c1 = (newval & 0x0000ff00) / 0x100;
	c2 = (newval & 0x00ff0000) / 0x10000;
	c3 = (newval & 0xff000000) / 0x1000000;
	printf("c0 is %x , c1 is %x , c2 is %x , c3 is %x \n",c0,c1,c2,c3);
	/* write into registers */
	// map_base[pgoffset] = (unsigned char)c0;
	// map_base[pgoffset + 1] = (unsigned char)c1;
	// map_base[pgoffset + 2] = (unsigned char)c2;
	// map_base[pgoffset + 3] = (unsigned char)c3;
	map_base[pgoffset] = newval;   
	// printf("IN WRITE : Value at 0x%08X: 0x%02X%02X%02X%02X\n", PHYADDR,
	// 				(unsigned int)map_base[pgoffset + 3],
	// 				(unsigned int)map_base[pgoffset + 2],
	// 				(unsigned int)map_base[pgoffset + 1],
	// 				(unsigned int)map_base[pgoffset]
	// 				);
	printf("IN WRITE : Value at 0x%08X: 0x%08X\n", PHYADDR,
					(unsigned int)map_base[pgoffset]
					);
	close(fd);
	munmap(map_base, pagesize);
	return 0;
}


/*
 * Close the descriptor associated with the device name
 */
int closeDescriptor(struct nf2device *nf2)
{
        struct ifreq ifreq;
	char filename[PATHLEN];

	if (nf2->net_iface)
	{
		close(nf2->fd);
	}
	else
	{
		close(nf2->fd);
	}

	return 0;
}

