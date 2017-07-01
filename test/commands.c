#include "commands.h"

void Init(Command *entry,FlowEntry TableInfor[5]){
    entry->op = nop;
    entry->id = tb_match_QinQ;
    int i = 0;
    for(i = 0;i<10;i++){
        entry->key[i] = 0x00000000;
        entry->mask[i] = 0x00000000;
        entry->value[i] = 0x00000000;
    }
    entry->key_write_num = 0;
    entry->mask_write_num = 0;
    entry->value_write_num = 0;
    TableInforInit(TableInfor);
}
void TableInforInit(FlowEntry TableInfor[5]){
    TableInfor[0].key_num = 2;
    TableInfor[0].key_len[0] = 16;
    TableInfor[0].key_len[1] = 8;
    TableInfor[0].mask_num = 2;
    TableInfor[0].mask_len[0] = 16;
    TableInfor[0].mask_len[1] = 8;
    TableInfor[0].value_num = 1;
    TableInfor[0].value_len[0] = 2;

    TableInfor[1].key_num = 2;
    TableInfor[1].key_len[0] = 16;
    TableInfor[1].key_len[1] = 16;
    TableInfor[1].mask_num = 2;
    TableInfor[1].mask_len[0] = 16;
    TableInfor[1].mask_len[1] = 16;
    TableInfor[1].value_num = 1;
    TableInfor[1].value_len[0] = 3;

    TableInfor[2].key_num = 1;
    TableInfor[2].key_len[0] = 32;
    TableInfor[2].value_num = 4;
    TableInfor[2].mask_num = 1;
    TableInfor[2].mask_len[0] = 32;
    TableInfor[2].value_len[0] = 4;
    TableInfor[2].value_len[1] = 48;
    TableInfor[2].value_len[2] = 48;
    TableInfor[2].value_len[3] = 16;

    TableInfor[3].key_num = 1;
    TableInfor[3].key_len[0] = 48;
    TableInfor[3].mask_num = 1;
    TableInfor[3].mask_len[0] = 48;
    TableInfor[3].value_num = 4;
    TableInfor[3].value_len[0] = 3;
    TableInfor[3].value_len[1] = 2;
    TableInfor[3].value_len[2] = 12;
    TableInfor[3].value_len[3] = 12;

    TableInfor[4].key_num = 1;
    TableInfor[4].key_len[0] = 48;
    TableInfor[4].mask_num = 1;
    TableInfor[4].mask_len[0] = 48;
    TableInfor[4].value_num = 4;
    TableInfor[4].value_len[0] = 3;
    TableInfor[4].value_len[1] = 2;
    TableInfor[4].value_len[2] = 12;
    TableInfor[4].value_len[3] = 12;
}
uint8_t CharToHex(char ch){
    uint8_t hex;
    if(ch > 96 && ch < 103)
        hex = ch - 87;
    else if(ch >64 && ch < 71)
        hex = ch - 55;
    else if(ch >47 && ch <74)
        hex = ch - 48;
    else
        hex = 0x10;
    return hex;
}
int AddEntry(char * com[10],Command *entry,FlowEntry TableInfor[5]){
    int i;
    for(i = 0;i<3;i++)    
        if(strcmp(com[0],OperationName[i]) == 0){
            switch(i){
                case 0: entry->op = nop; break;
                case 1: entry->op = table_init; break;
                case 2: entry->op = table_add; break;
                case 3: entry->op = table_del; break;
            }
            break;
        }
    for(i = 0;i<5;i++)    
        if(strcmp(com[1],TableName[i]) == 0){
            switch(i){
                case 0 : entry->id = tb_match_QinQ; break;
                case 1 : entry->id = tb_match_Domain; break;
                case 2 : entry->id = tb_match_L3; break;
                case 3 : entry->id = tb_match_pppL2; break;
                case 4 : entry->id = tb_match_extL2; break;
            }
            break;
        }

    int cnt = 0;
    int offset = 0;
    for(i = (int)strlen(com[2])-1;i >= 0;i--) {
        entry->priority += (CharToHex(com[2][i]) << cnt);
        cnt += 4;
    }

    cnt = 0;
    offset = 0;
    int key_start = 3;
    int key_end = 2 + TableInfor[entry->id].key_num;
    int key_id = key_end;

    while(key_id >= key_start){
        for(i = (int)strlen(com[key_id]) - 1;i >= 0;i--) {
            entry->key[offset] += CharToHex(com[key_id][i]) << cnt;
            cnt += 4;
            if(cnt == 32){
                offset += 1;
                cnt = 0;
            }
        }
        key_id --;
    }

    cnt = 0;
    offset = 0;
    int mask_start = key_end + 1;
    int mask_end = mask_start + TableInfor[entry->id].mask_num - 1;
    int mask_id = mask_end;
    while(mask_id >= mask_start){
        for(i = (int)strlen(com[mask_id]) - 1;i >= 0;i--) {
            entry->mask[offset] += CharToHex(com[mask_id][i]) << cnt;
            cnt += 4;
            if(cnt == 32){
                offset += 1;
                cnt = 0;
            }
        }
        mask_id --;
    }

    cnt = 0;
    offset = 0;
    int value_start = mask_end + 1;
    int value_end = value_start + TableInfor[entry->id].value_num - 1;
    int value_id = value_end;
    int value_len = 0;
    while(value_id >= value_start){
        value_len = TableInfor[entry->id].value_len[value_id - value_start];
        for(i = (int)strlen(com[value_id]) - 1;i >= 0;i--) {
            entry->value[offset] += CharToHex(com[value_id][i]) << cnt;
            if(value_len < 4){
                cnt +=value_len;
                value_len = 0;
            }
            else{
                cnt += 4;
                value_len -=4;
            }

            if(cnt >= 32){
                offset += 1;
                cnt = 0;
            }
        }
        value_id --;
    }
    int tmp = 0;
    for(i = 0;i<TableInfor[entry->id].key_num;i++){
        tmp += TableInfor[entry->id].key_len[i];
    }
    if(tmp%32 == 0)
        entry->key_write_num = tmp/32;
    else
        entry->key_write_num = tmp/32 + 1;

    entry->mask_write_num = entry->key_write_num;

    tmp = 0;
    for(i = 0;i<TableInfor[entry->id].value_num;i++){
        tmp += TableInfor[entry->id].value_len[i];
    }
    if(tmp%32 == 0)
        entry->value_write_num = tmp/32;
    else
        entry->value_write_num = tmp/32 + 1;
    printf("%d\t",entry->key_write_num);
    printf("%d\t",entry->mask_write_num);
    printf("%d\n",entry->value_write_num);

    return 0;
}

void show(Command *entry){
    int i;
    printf("op:\n");
    printf("%x\n",entry->op);
    printf("id:\n");
    printf("%x\n",entry->id);
    printf("priority:\n");
    printf("%x\n",entry->priority);
    printf("key:\n");
    for(i = 0;i<10;i++){
        printf("%x\n",entry->key[i]);
    }
    printf("mask:\n");
    for(i = 0;i<10;i++){
        printf("%x\n",entry->mask[i]);
    }
    printf("value:\n");
    for(i = 0;i<10;i++){
        printf("%x\n",entry->value[i]);
    }
}

int ReadCommandFile(char ** const buff, const unsigned int spec, const char * const filename)
{
    FILE *fp = fopen(filename, "r");
    if (fp == NULL)
    {
        printf("Fail to open file %s. \n", filename);
        return 0;
    }

    char line[MAX_LINE_LEN + 2];
    unsigned int cnt = 0;
    while ((cnt < spec) && !feof(fp))
    {
        line[0] = 0;
        if (fgets(line, MAX_LINE_LEN + 2, fp) == NULL)  continue;
        if (line[0] == 0)   continue;
        buff[cnt] = (char *)malloc(MAX_LINE_LEN + 2);
        strncpy(buff[cnt], line, MAX_LINE_LEN + 2 - 1);
        buff[cnt][MAX_LINE_LEN + 1] = 0;
        cnt++;
    }

    printf("Open file %s OK.\n", filename);
    printf("There are %d lines in file %s.\n", cnt, filename);
    fclose(fp);

    return cnt;
}


void ModifyCommands(char * com,int tos){
    int len = (int)strlen(com);
    int i = 0;
    for(i = 0;i<len;i++){
        if(com[tos] == ' ' ||com[tos] == '\n'){
            break;
        }
        else
            com[i] = com[tos++];
    }
    if(i != len )
        com++[i] = '\0';
}

/* int TransferCommands(char *Commands[MAX_DEPTH],int line_num,char *out[10]){ */
    /* int i; */
    /* int k = 0; */
    /* memset(out,0,sizeof(char *)*10); */
    /* for(i = 0;i < line_num;i++){ */
        /* char * buf = Commands[i]; */
        /* printf("%s",buf); */
        /* while((out[k] = strtok(buf,"|")) != NULL){ */
            /* k ++; */
            /* buf = NULL; */
        /* } */
        /* for(i = k;i<10;i++){ */
            /* out[i] = "NULL"; */
        /* } */
        /* for(i = 0;i<k;i++){ */
            /* int cnt = 0; */
            /* while(out[i][cnt] == ' '){ */
                /* cnt ++; */
            /* } */
            /* ModifyCommands(out[i],cnt); */
        /* } */
    /* } */
    /* [> for(i = 0;i<10;i++){ <] */
        /* [> printf("%s\t",out[i]); <] */
        /* [> printf("%d\n",(int)strlen(out[i])); <] */
    /* [> } <] */
    /* return 0; */
/* } */

int TransferCommands(char *Commands[MAX_DEPTH],int line_num,char *out[MAX_DEPTH][10]){
    int i;
    int m;
    int k = 0;
    memset(out,0,sizeof(char *)*10);
    for(i = 0;i < line_num;i++){
        k = 0;
        char * buf = Commands[i];
        printf("%s",buf);
        while((out[i][k] = strtok(buf,"|")) != NULL){
            k ++;
            buf = NULL;
        }
        for(m = k;m<10;m++){
            out[i][m] = "NULL";
        }
        for(m = 0;m<k;m++){
            int cnt = 0;
            while(out[i][m][cnt] == ' '){
                cnt ++;
            }
            ModifyCommands(out[i][m],cnt);
        }
    }
    return 0;
}
