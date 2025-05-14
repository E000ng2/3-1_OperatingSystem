    #include <stdio.h>
    #include <string.h>
    #define MAX_LEN 28

    char *change(char*);

    typedef struct 
    {
        char unix[10];
        char doc[10];
    } ucchange;


    int main(void)
    {
        //파일 열기
        FILE *command_inp, *command_out;
        command_inp = fopen("./command.inp", "r");
        command_out = fopen("./command.out", "w");


        //파일 불러오기
        int n;
        fscanf(command_inp, "%d", &n);

        for(int i = 0; i < n; i++)
        {
            char *changetemp;
            char ctemp[10];
            fscanf(command_inp, "%s", ctemp);
            changetemp = change(ctemp);
            fprintf(command_out, "%s -> %s\n", ctemp, changetemp);
        }
        
        return 0;
    }

    void sum(int b)
    {
        static int a = 1;

        a += b;
    }

    char *change(char *ctemp)
    {
        static char changetemp[10];
        //command list배열
        ucchange commands[] = {
            {"ls", "dir"}, {"mkdir", "md"}, {"rmdir", "rd"}, {"rm", "del"},
            {"cp", "copy"}, {"mv", "rename"}, {"clear", "cls"}, {"pwd", "cd"},
            {"cat", "type"}, {"man", "help"}, {"date", "time"}, {"find", "find"},
            {"grep", "findstr"}, {"more", "more"}, {"diff", "comp"}, {"ed", "edlin"},
            {"sort", "sort"}, {"lsattr", "attrib"}, {"pushd", "pushd"}, {"popd", "popd"},
            {"ps", "taskmgr"}, {"kill", "tskill"}, {"halt", "shutdown"},
            {"ifconfig", "ipconfig"}, {"fsck", "chkdsk"}, {"free", "mem"},
            {"debugfs", "scandisk"}, {"lpr", "print"}
        };

        for(int i = 0; i < MAX_LEN; i++)
        {
            if(strcmp(commands[i].unix, ctemp) == 0)
            {
                strcpy(changetemp, commands[i].doc);
            }
            else if(strcmp(commands[i].doc, ctemp) == 0)
            {
                strcpy(changetemp, commands[i].unix);
            }
        }

        return changetemp;
    }