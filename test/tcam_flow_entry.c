#include "tcam_flow_entry.h"

int InitAllTcamTable(){
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
    int i = 0;
    uint32_t ph_addr;
    uint32_t addr = 0x10;
    uint32_t init_req = 0x20000000;
    for(i = 0;i<5;i++){
        ph_addr = TableAddr[i];
        writeReg(&nf2, ph_addr + addr, init_req);
    }
    return 0;
}
int ActivateOperation(Command * entry){
    switch(entry->op){
        case nop        : break;
        case table_init : InitTcamFlowEntry(entry);   break;
        case table_add  : AddTcamFlowEntry(entry);    break;
        case table_del  : DelTcamFlowEntry(entry);    break;
    }
    return 0;
}
int InitTcamFlowEntry(Command * entry){
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
    uint32_t ph_addr = TableAddr[entry->id];
    uint32_t addr = 0x10;
    uint32_t init_req = 0x20000000;
    writeReg(&nf2, ph_addr + addr, init_req);
    return 0;
}

int AddTcamFlowEntry(Command * entry){
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
    uint32_t ph_addr = TableAddr[entry->id];
    uint32_t ack_addr = 0x10;
    uint32_t ack_resp_addr = 0x11;
    uint32_t vld_addr = 0x13;
    uint32_t key_addr = 0x14;
    uint32_t mask_addr = 0x24;
    uint32_t value_addr = 0x34;

    uint32_t vld_req = 0x00000000;
    uint32_t ack_req = 0x10000000 + entry->priority;
    uint32_t ack_resp = 0x00000001;

    /* writeReg(&nf2, ph_addr + vld_addr, vld_req); */
    int i = 0;
    /* write key */
    for(i = 0;i<entry->key_write_num;i++){
        writeReg(&nf2, ph_addr + key_addr, entry->key[i]);
        key_addr ++;
    }
    /* write mask */
    for(i = 0;i<entry->mask_write_num;i++){
        writeReg(&nf2, ph_addr + mask_addr, entry->mask[i]);
        mask_addr ++;
    }
    /* write value */
    for(i = 0;i<entry->value_write_num;i++){
        writeReg(&nf2, ph_addr + value_addr, entry->value[i]);
        value_addr ++;
    }
    vld_req = 0x00000001;
    writeReg(&nf2, ph_addr + vld_addr, vld_req);
    writeReg(&nf2, ph_addr + ack_addr, ack_req);
    /* writeReg(&nf2, ph_addr + ack_resp_addr, ack_resp); */

    return 0;


}

int DelTcamFlowEntry(Command * entry){
    struct nf2device nf2;

    nf2.device_name = DEFAULT_IFACE;
    printf("delete!\n");

    if (check_iface(&nf2))
    {
        exit(1);
    }
    if (openDescriptor(&nf2))
    {
        exit(1);
    }
    uint32_t ph_addr = TableAddr[entry->id];
    uint32_t ack_addr = 0x10;
    uint32_t ack_req = 0x10000000 + entry->priority;
    uint32_t vld_addr = 0x13;
    uint32_t vld_req = 0x00000000;

    writeReg(&nf2, ph_addr + vld_addr, vld_req);
    writeReg(&nf2, ph_addr + ack_addr, ack_req);

    return 0;
}

