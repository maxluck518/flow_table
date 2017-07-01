#include "commands.h"
/* int AddTcamFlowEntry(uint32_t key[10],uint32_t mask[10],uint32_t value[10],uint32_t addr){ */
/* } */

/* int main(){ */
int main(int argc,int * argv[]){
    /* char *file = "flow.txt"; */
    // char *file = "flow.txt.old";
    char *file = argv[1];
    char *com[MAX_DEPTH];
    char *out[MAX_DEPTH][10];
    int line_num;
    FlowEntry TableInfor[5];

    line_num = ReadCommandFile(com,MAX_DEPTH,file);
    TransferCommands(com,line_num,out);
    struct Command * entry[line_num];
    int i = 0;
    /* InitTcamFlowEntry(); */
    for(i = 0;i<line_num;i++){
        entry[i] = (Command *)malloc(sizeof(Command));
        Init(entry[i],TableInfor);
        printf("entry %d :\t",i);
        AddEntry(out[i],entry[i],TableInfor);
        show(entry[i]);
        /* writeReg(); */
	//        InitTcamFlowEntry(entry[i]);
        /* AddTcamFlowEntry(entry[i]); */
    }


    return 0;

}
