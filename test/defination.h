#ifndef _DEFINATION_H
#define _DEFINATION_H

#include <stdio.h>  
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#define TABLE_DEPTH 511
#define MAX_LINE_LEN 5500
#define MAX_DEPTH 500

enum ofp_exp_tb_match_type {
    tb_match_QinQ       = 0,
    tb_match_Domain     = 1,
    tb_match_L3         = 2,
    tb_match_pppL2      = 3,
    tb_match_extL2      = 4,
};

enum table_operation{
    nop             = 0,
    table_add       = 1,
    table_del       = 2,
};

typedef struct flow_entry_len{
    int key_num;
    int key_len[5];   //bit
    int value_num;
    int value_len[5]; //bit
}FlowEntry;

static char* TableName[5] = {"qinq","domain","L3","pppL2","extL2"};
static char* OperationName[3] = {"nop","table_add","table_del"};
static uint32_t TableAddr[5] = {0x43d40000,0x43d00000,0x43d20000,0x43d30000,0x43d10000};

#endif
