#include<stdio.h>
#include<string.h>

void writeFile(char * filename, int num)
{
    char newname[100]="tmp.txt";
    FILE *f,*tmp;
    f=fopen(filename,"a+");
    tmp=fopen(newname,"w");
    if(!f)
    {
        printf("Can not open %s!\n", filename);
        return;
    }
    if(!tmp)
    {
        printf("Can not open %s!\n", newname);
        return;
    }
    fseek(f, 0L, SEEK_END);
    int len = ftell(f);

    fseek(f, 0L, SEEK_SET);
    int count = 0;
    int value;
    if(len>0)
    {
        while(fread(&value,sizeof(int),1,f) == 1)
        {
            if(value>=num)
            {
                break;
            }
            count+=1;
        }
        fseek(f, 0L, SEEK_SET);
        fseek(tmp, 0L, SEEK_SET);
        for(int i=0;i<count;i++)
        {
            fread(&value,sizeof(int),1,f);
            fwrite(&value,sizeof(int),1,tmp);
        }
        fwrite(&num,sizeof(int),1,tmp);
        while(fread(&value,sizeof(int),1,f) == 1)
        {
            fwrite(&value,sizeof(int),1,tmp);
        }
        remove("abc.txt");
        rename("tmp.txt","abc.txt");
    }
    else
    {
        fwrite(&num,sizeof(int),1,f);
    }
    fclose(f);
    fclose(tmp);
}

int calSum(char * filename)
{
    int sum=0;
    FILE *f;
    f=fopen(filename,"rb");
    if(!f)
    {
        printf("Can not open %s!\n", filename);
        return;
    }
    fseek(f, 0L, SEEK_SET); 
    int value;
    while(fread(&value,sizeof(int),1,f) == 1)
    {
        printf("%d\n",value);
        sum+=value;
    }
    fclose(f);
    return sum;
}

void writeFile2(char * filename, int num)
{
    char newname[100]="tmp.txt";
    FILE *f,*tmp;
    f=fopen(filename,"a+");
    tmp=fopen(newname,"w");
    if(!f)
    {
        printf("Can not open %s!\n", filename);
        return;
    }
    if(!tmp)
    {
        printf("Can not open %s!\n", newname);
        return;
    }
    fseek(f, 0L, SEEK_END);
    int len = ftell(f);

    fseek(f, 0L, SEEK_SET);
    int count = 0;
    int value;
    if(len>0)
    {
        while(fscanf(f,"%d-",&value) == 1)
        {
            if(value>=num)
            {
                break;
            }
            count+=1;
        }
        fseek(f, 0L, SEEK_SET);
        fseek(tmp, 0L, SEEK_SET);
        for(int i=0;i<count;i++)
        {
            fscanf(f,"%d-",&value);
            fprintf(tmp,"%d-",value);
        }
        fprintf(tmp,"%d-",num);
        while(fscanf(f,"%d-",&value) == 1)
        {
            fprintf(tmp,"%d-",value);
        }
        remove("abc.txt");
        rename("tmp.txt","abc.txt");
        remove("tmp.txt");
    }
    else
    {
        fprintf(f,"%d-",num);
        remove("tmp.txt");
    }
    fclose(f);
    fclose(tmp);
}

int calSum2(char * filename)
{
    int sum=0;
    FILE *f;
    f=fopen(filename,"rb");
    if(!f)
    {
        printf("Can not open %s!\n", filename);
        return;
    }
    fseek(f, 0L, SEEK_SET); 
    int value;
    while(fscanf(f,"%d-",&value) == 1)
    {
        printf("%d\n",value);
        sum+=value;
    }
    fclose(f);
    return sum;
}

int main()
{
    remove("abc.txt");
    remove("tmp.txt");
    //FILE*f=fopen("abc.txt","wb");
    int num;
    while(1)
    {
        
        scanf("%d",&num);
        if(num==-1)
        {
            printf("\n\n");
            break;
        }
        writeFile2("abc.txt",num);
        
    }
    printf("\n\nNum = %d\n",calSum2("abc.txt"));
    return 0;
}