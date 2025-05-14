#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void sort(int*, char[][31], char[][31], int);
int* last(char[][31], int);
int* dupprint(char[][31], int*, int);
int longname(char[][31], int);

int main(void)
{
    FILE *fp_in, *fp_out;
    int N; // 입력 받을 사람의 수

    //파일 열기기
    fp_in = fopen("./test.inp", "r");
    fp_out = fopen("./test.out", "w");

    //사람 수 입력
    fscanf(fp_in, "%d", &N);

    //배열 선언
    int num[N];
    char firstname[N][31];
    char lastname[N][31];
    int *dupary;

    // fp_in 읽기
    for(int i = 0; i < N; i++)
    {
        fscanf(fp_in, "%d %s %s", &num[i], firstname[i], lastname[i]);
    }

    //정렬!
    sort(num, firstname, lastname, N);

    //firstname의 최댓값
    int max = longname(firstname, N);

    //결과 출력

    // fp_out 쓰기
    for(int i = 0; i < N; i++)
    {
        fprintf(fp_out, "%05d %-*s %s\n", num[i], max, firstname[i], lastname[i]);
    }
    
    dupary = last(lastname, N);

    fprintf(fp_out, "\n");
    
    for(int i = 0; i < N; i++)
    {
        if(dupary[i] <= 1) continue;
        fprintf(fp_out, "%s %d\n", lastname[i], dupary[i]);
    }
    
    //파일 닫기
    fclose(fp_in);
    fclose(fp_out);

    return 0;
}

// 숫자 정렬 함수
void sort(int *num, char firstname[][31], char lastname[][31], int N)
{
    int temp;
    char ctemp[31];

    for( int i = 0; i < N - 1; i ++)
    {
        for(int j = i + 1; j < N; j ++)
        {
            if(num[i] > num[j])
            {
                temp = num[i];
                num[i] = num[j];
                num[j] = temp;
                
                strcpy(ctemp, firstname[i]);
                strcpy(firstname[i], firstname[j]);
                strcpy(firstname[j], ctemp);

                strcpy(ctemp, lastname[i]);
                strcpy(lastname[i], lastname[j]);
                strcpy(lastname[j], ctemp);
            }
        }
    }
}

//lastname 중복 검사 함수
int* last(char lastname[][31], int N)
{
    int flag[N]; //중복 표시
    int cnt; //개수 카운트
    int *dupary = (int*)malloc(N * sizeof(int)); //각 이름 개수 

    //flag 초기화
    for(int i = 0; i < N; i++)
    {
        flag[i] = 0;
    }

    //중복 확인
    for(int i = 0; i < N ; i++)
    {
        cnt = 1;
        if(flag[i] != 1)
        {
            for(int j = i + 1; j < N; j++)
            { 
                    if(strcmp(lastname[i], lastname[j]) == 0)
                    {
                        cnt += 1;
                        flag[j] = 1;
                        flag[i] = 1;
                    }
            }
        }

        dupary[i] = cnt;
        // //중복일 경우 lastname과 개수 출력
        // if (cnt > 1)
        // {
        //     printf("%s %d\n", lastname[i], cnt);
        // }
    }

    return dupprint(lastname, dupary, N);
}

//중복 결과 출력 함수
int* dupprint(char lastname[][31], int *dupary, int N)
{
    int temp;
    char ctemp[31];

    for( int i = 0; i < N - 1; i ++)
    {
        for(int j = i + 1; j < N; j ++)
        {
            if(strcmp(lastname[i], lastname[j]) > 0)
            {
                strcpy(ctemp, lastname[i]);
                strcpy(lastname[i], lastname[j]);
                strcpy(lastname[j], ctemp);

                temp = dupary[i];
                dupary[i] = dupary[j];
                dupary[j] = temp;
            }
        }
    }
    return dupary;
}

//가장 긴 이름 찾기
int longname(char firstname[][31], int N)
{
    int max =0;

    for(int i = 0; i < N; i++)
    {
            if(strlen(firstname[i]) > max)
            {
                max = strlen(firstname[i]);
            }
    }

    return max;
}