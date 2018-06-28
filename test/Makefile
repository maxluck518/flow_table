CC=arm-xilinx-linux-gnueabi-gcc
all : ./lib/common/nf2util.o 
	$(CC) -o download_flow_entry main.c commands.c tcam_flow_entry.c ./lib/common/nf2util.o
clean :
	rm -f *.o cscope.* tags dflow app
