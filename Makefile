CC=arm-xilinx-linux-gnueabi-gcc
all : ./lib/common/nf2util.o 
	$(CC) -o pppL2 pppL2_tb.c ./lib/common/nf2util.o ./TCAM.c
clean :
	rm -f *.o
