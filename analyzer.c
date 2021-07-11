#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define ARRAY_SIZE(array) (sizeof(array)/sizeof(array[0]))
#define false (0)
#define true  (1)
typedef unsigned char bool;

typedef struct DataTypes_T{
    char name[64];
    char size;
    struct DataTypes_T *next;
}DataTypes;

/* 関数 */
void func(FILE **,int );
void addnode(DataTypes **,char *,char );
void freelist(DataTypes **);
void savelist(FILE *,DataTypes **);

typedef enum FILE_NAME_T{
    FPI,
    FPO,
    FPD,
}FILE_NAME;


/*メイン関数*/
int main(void){
    FILE *fpi = fopen("target.c","r");
    FILE *fpo = fopen("output.txt","w");
    FILE *fpd = fopen("datalist.txt","w");
    FILE *file_pointer_list[] = {fpi,fpo,fpd};
    int index;

    /* initialize */
    for( index = 0 ; ARRAY_SIZE(file_pointer_list) < index ; index++ ){
        if(file_pointer_list[index] == NULL){
            printf("file opne error\n");
            return 1;
        }
    }

    /* 処理 */
    func(file_pointer_list,ARRAY_SIZE(file_pointer_list));
    
    /* finalize */
    for( index = 0 ; ARRAY_SIZE(file_pointer_list) < index ; index++ ){
        fclose(file_pointer_list[index]);
    }
    printf("finish\n");
    return 0;
}


void func(FILE **fpl,int size){
    int  kakko_counter = 0;
    int  i;
    char buff[128];
    char mozi;
    bool flag = false;
    DataTypes *datatype = NULL;

    /*コピー*/
    i = 0;
    while( (mozi=fgetc(fpl[FPI])) != EOF ){
        if( mozi != ' ' && mozi != '\t' && mozi != '\n'){
            buff[i] = mozi;
            i++;
            continue;
        }else{
            if(i == 0){
                
            }else{
                buff[i] = '\0';
                i = 0;
                /*typedefから;までを表示*/
                if(strcmp(buff,"typedef") == 0){
                    flag = true;
                    continue;
                }

                /*{カウント*/
                if(strstr(buff,"{")!=NULL){
                    kakko_counter++;
                }
                /*}カウント*/
                if(strstr(buff,"}")!=NULL){
                    kakko_counter--;
                }

                /*typedefに対する;があれば書き込み終了*/
                if(strcmp(buff,";") == 0){
                    flag = false;
                    fprintf(fpl[FPO],"\n");
                    continue;
                }
                
                /*unsignedはサイズ関係ないので無視*/
                if(strcmp(buff,"unsigned") == 0){
                    continue;
                }

                /*サイズ判定*/
                if(strcmp(buff,"char") == 0){
                    fprintf(fpl[FPO],"1 : ");
                    continue;
                }
                if(strcmp(buff,"short") == 0){
                    fprintf(fpl[FPO],"2 : ");
                    continue;
                }
                if(strcmp(buff,"int") == 0){
                    fprintf(fpl[FPO],"4 : ");
                    continue;
                }
                if(strcmp(buff,"long") == 0){
                    fprintf(fpl[FPO],"4 : ");
                    continue;
                }
                if(strcmp(buff,"float") == 0){
                    fprintf(fpl[FPO],"4 : ");
                    continue;
                }
                if(strcmp(buff,"double") == 0){
                    fprintf(fpl[FPO],"8 : ");
                    continue;
                }

                if(flag){
                    fprintf(fpl[FPO],"%s\n",buff);
                }
            }
            
        }
    }
}

/*追加*/
void addnode(DataTypes **dt,char *name,char size){
    DataTypes *temp;
    temp = malloc(sizeof(DataTypes));

    strcpy(temp->name,name);
    temp->size = size;
    temp->next = *dt;

    *dt = temp;
}

/*保存*/
void savelist(FILE *fp,DataTypes **dt){
    DataTypes *temp;
    fprintf(fp,"[------name------] : [size]\n"); 
    while( *dt != NULL ){
        printf("[%16s] : %2d\n",(*dt)->name,(*dt)->size);
        fprintf(fp,"[%16s] : %2d\n",(*dt)->name,(*dt)->size);     
        temp = *dt;
        *dt = (*dt)->next;

    }
}

/*解放*/
void freelist(DataTypes **dt){
    DataTypes *temp;
    while( *dt != NULL ){
        temp = *dt;
        *dt = (*dt)->next;
        free(temp);
    }
}