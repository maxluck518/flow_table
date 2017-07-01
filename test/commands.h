#ifndef _COMMANDS_H
#define _COMMANDS_H
#include "defination.h"

typedef struct Command{
    enum table_operation op;
    enum ofp_exp_tb_match_type id;
    uint32_t key[10];
    uint32_t mask[10];
    uint32_t value[10];
    uint16_t priority;
    int key_write_num;
    int mask_write_num;
    int value_write_num;
    char *split[10];
}Command;

void Init(Command *entry,FlowEntry TableInfor[5]);
void TableInforInit(FlowEntry TableInfor[5]);
uint8_t CharToHex(char ch);
int AddEntry(char * com[10],Command *entry,FlowEntry TableInfor[5]);
void show(Command *entry);
void display(Command *entry,FlowEntry TableInfor[5]);

int ReadCommandFile(char ** const buff, const unsigned int spec, const char * const filename);


void ModifyCommands(char * com,int tos);

int TransferCommands(char *Commands[MAX_DEPTH],int line_num,char *out[MAX_DEPTH][10]);

#endif
