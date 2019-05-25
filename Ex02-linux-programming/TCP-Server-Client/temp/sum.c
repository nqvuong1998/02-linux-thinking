#include<stdio.h>
#include<stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int calSum(char * filename)
{
    int sum=0;
    FILE *f;
    f=fopen(filename,"r");
    if(!f)
    {
        printf("Can not open %s!\n", filename);
        return;
    }
    // fseek(f, 0L, SEEK_END);
    // int len = ftell(f);
    // fseek(f, 0L, SEEK_SET);
    // if(len <= 0)return 0;
    int value;
    while(fscanf(f,"%d\n",&value) > 0)
    {
        sum+=value;
    }
    fclose(f);
    return sum;
}

bool isNumber(char * s)
{
    int len = strlen(s);
    for (int i = 0; i < len; i++) 
        if (s[i] < '0' || s[i] > '9') 
            return false; 
  
    return true; 
}

char * optimizeNumber(char * value)
{
    char *number = (char*)malloc(100);
    memset(number,'\0',strlen(value));
    int len = strlen(value);
    int j=0;
    for (int i = 0; i < len; i++) 
    {
        if (isdigit(value[i]) == true) 
        {
            number[j++]=value[i];
        }
    } 
    
    printf("len = %d - number %s\n",strlen(number),number);
    return number;
}

void writeFile(char * filename, int num)
{
    FILE *f = fopen(filename,"w+");
    fseek(f, 0L, SEEK_END);
    int size = ftell(f);
    fseek(f, 0L, SEEK_SET);
    if(size <= 0)
    {
        fprintf(f,"%d\n",num);
        return;
    }
    int value;
    bool flag = false;
    while(fscanf(f,"%d\n",&value) > 0)
    {
        if(value>=num && flag == false)
        {
            fprintf(f,"%d\n",num);
            flag = true;
            break;
        }
        //fprintf(f,"%d\n",value);
    }

    fclose(f);
}

int main(int argc, char * argv[])
{
    rename("abc.txt","def.txt");
    FILE * f=fopen("abc.txt","wr+");

    fclose(f);
    // int num;
    // while (1)
    // {
    //     scanf("%d",&num);
    //     if(num==-1)break;
    //     writeFile("abc.txt",num);
    // }
    
    printf("Sum = %d\n", calSum("abc.txt"));
    //printf("Atoi: %d\n",atoi("93\F0N[V808"));
    // char * str = "93\F0N[V808";
    // if(isNumber(str))
    // {
    //     printf("O\n");
    // }
    // else
    // {

    //     char *str2=optimizeNumber(str);
    //     printf("Atoi: %d\n",atoi(str2));
    // }
    

    return 0;
}