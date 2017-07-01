#ifndef _TCAM_FLOW_ENTRY_
#define _TCAM_FLOW_ENTRY_

#include <stdio.h>  
#include <unistd.h>  
#include <sys/mman.h>
#include <sys/types.h>  
#include <sys/stat.h>  
#include <fcntl.h>
#include <stdlib.h>
#include <stdint.h>

#include <string.h>
#include <net/if.h>
#include "./commands.h"
#include "./lib/common/nf2util.h"

#define DEFAULT_IFACE	"mem"
#define READ_TEST       0x0000040

int InitTcamFlowEntry(Command * entry);
int AddTcamFlowEntry(Command * entry);

#endif
