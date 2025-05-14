#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{   
    //파일 열기
    FILE *batch_inp, *batch_out;

    batch_inp = fopen("./batch.inp", "r");
    batch_out = fopen("./batch.out", "w");

    //파일 읽기
    int n;
    fscanf(batch_inp, "%d", &n);

    int max_line = 1000;
    int num;
    char end_line[4] = "-1";
    int idletime = 0;
    int programtime = 0;
    
    char *line = (char*)malloc(sizeof(char) * max_line);

    while(fgets(line, max_line, batch_inp))
    {
        int cnt = 0;

        char *token = strtok(line, " \t\n");
        while(token && strcmp(token, end_line) != 0)
        {
            num = atoi(token);
            if(cnt % 2 == 1)
            {
                idletime += num;
            }
            programtime += num;
            cnt++;

            token = strtok(NULL, " \t\n");
        } 
    }

    fprintf(batch_out, "%d %d \n",idletime, programtime);
    return 0;
}