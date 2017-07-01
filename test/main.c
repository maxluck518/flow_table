#include "commands.h"
#include "tcam_flow_entry.h"
/* int AddTcamFlowEntry(uint32_t key[10],uint32_t mask[10],uint32_t value[10],uint32_t addr){ */
/* } */

/* int main(){ */
int main(int argc,int * argv[]){
    /* char *file = "flow.txt"; */
    char *file = argv[1];
    char *com[MAX_DEPTH];
    char *out[MAX_DEPTH][10];
    int line_num;
    FlowEntry TableInfor[5];

    line_num = ReadCommandFile(com,MAX_DEPTH,file);
    TransferCommands(com,line_num,out);
    struct Command * entry[line_num];
    int i = 0;
    int entry_num = 0;
    int init_flag = 1;
    /* InitAllTcamTable(); */
    TableInforInit(TableInfor);
    for(i = 0;i<line_num;i++){
        entry[entry_num] = (Command *)malloc(sizeof(Command));
        Init(entry[entry_num],TableInfor);
        if(AddEntry(out[i],entry[entry_num],TableInfor)){
            /* show(entry[entry_num]); */
            entry_num ++;
        }
        else
            free(entry[entry_num]);
    }
    printf("There are %d table entries ! \n",entry_num);
    for(i = 0;i<entry_num;i++)
        display(entry[i],TableInfor);
    for(i = 0;i<entry_num;i++){
        ActivateOperation(entry[i]);
    }

    printf("Activate %d flow entries ! \n",entry_num);

    return 0;

}
