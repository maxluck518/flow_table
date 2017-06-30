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
#include "./lib/common/nf2util.h"
#include "TCAM.h"

#define DEFAULT_IFACE	"mem"
#define READ_TEST       0x0000040

/* int AddTcamFlowEntry(uint32_t key[10],uint32_t mask[10],uint32_t value[10],uint32_t addr){ */
/* } */

int main()
{

  
  struct nf2device nf2;

  nf2.device_name = DEFAULT_IFACE;

  if (check_iface(&nf2))
  {
    exit(1);
  }
  if (openDescriptor(&nf2))
  {
    exit(1);
  }

  int log_write(const char * format)
  {
    return 0;
  }

  int (*log_message)(const char * format);
  int (*register_write)(struct nf2device *nf2, uint32_t addr, uint32_t data);
  int (*register_read)(struct nf2device *nf2, uint32_t addr,uint32_t *val);
  register_write = writeReg;
  register_read = readReg;
  log_message = log_write;

  TCAM_CONTEXT cx;
  uint32_t base=0x00;
  uint32_t size=256;
  uint32_t max_depth=1024;
  uint32_t key_width=240;
  uint32_t value_width=11;
  uint32_t num_ranges=0;
  uint32_t range_width=0;
  uint32_t range_offset=0;
  uint32_t log_level=0;
  uint32_t addr = 0;
  unsigned read_test;
 
  const char* data="010101020202000001010102020200000101010202020000010101020202";
  const char* mask="000000000000000000000000000000000000000000000000000000000000";
  const char* value="FFF";

  /* init */
  addr = 0x10;
  read_test = 0x20000000;
  writeReg(&nf2, addr, read_test);
  /* flow_entry_1 */
  addr = 0x13;
  read_test = 0x0;
  writeReg(&nf2, addr, read_test);

  addr = 0x14;
  read_test = 0xffffffff;
  writeReg(&nf2, addr, read_test);

  addr = 0x15;
  read_test = 0x0000ffff;
  writeReg(&nf2, addr, read_test);

  addr = 0x24;
  read_test = 0xffffffff;
  writeReg(&nf2, addr, read_test);

  addr = 0x25;
  read_test = 0x0000ffff;
  writeReg(&nf2, addr, read_test);

  addr = 0x34;
  read_test = 0x11000000;
  writeReg(&nf2, addr, read_test);

  addr = 0x13;
  read_test = 0x1;
  writeReg(&nf2, addr, read_test);

  addr = 0x10;
  read_test = 0x10000000;
  writeReg(&nf2, addr, read_test);

  addr = 0x11;
  read_test = 0x00000001;
  writeReg(&nf2, addr, read_test);
	
  /* flow_entry_2 */
  addr = 0x13;
  read_test = 0x0;
  writeReg(&nf2, addr, read_test);

  addr = 0x14;
  read_test = 0xccddeeff;
  writeReg(&nf2, addr, read_test);

  addr = 0x15;
  read_test = 0x0000aabb;
  writeReg(&nf2, addr, read_test);

  addr = 0x24;
  read_test = 0xffffffff;
  writeReg(&nf2, addr, read_test);

  addr = 0x25;
  read_test = 0x0000ffff;
  writeReg(&nf2, addr, read_test);

  addr = 0x34;
  read_test = 0x16007008;
  writeReg(&nf2, addr, read_test);

  addr = 0x13;
  read_test = 0x1;
  writeReg(&nf2, addr, read_test);

  addr = 0x10;
  read_test = 0x10000001;
  writeReg(&nf2, addr, read_test);

  addr = 0x11;
  read_test = 0x00000001;
  writeReg(&nf2, addr, read_test);

  /* flow_entry_3 */
  addr = 0x13;
  read_test = 0x0;
  writeReg(&nf2, addr, read_test);

  addr = 0x14;
  read_test = 0x33445566;
  writeReg(&nf2, addr, read_test);

  addr = 0x15;
  read_test = 0x00001122;
  writeReg(&nf2, addr, read_test);

  addr = 0x24;
  read_test = 0xffffffff;
  writeReg(&nf2, addr, read_test);

  addr = 0x25;
  read_test = 0x0000ffff;
  writeReg(&nf2, addr, read_test);

  addr = 0x34;
  read_test = 0x14003004;
  writeReg(&nf2, addr, read_test);

  addr = 0x13;
  read_test = 0x1;
  writeReg(&nf2, addr, read_test);

  addr = 0x10;
  read_test = 0x10000002;
  writeReg(&nf2, addr, read_test);

  addr = 0x11;
  read_test = 0x00000001;
  writeReg(&nf2, addr, read_test);
  closeDescriptor(&nf2);

  return 0;
   
}

