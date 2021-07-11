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
    char buff[128];
    bool flag = false;
    DataTypes *datatype = NULL;

    /*コピー*/
    while( fgets(buff,128,fpl[FPI])!=NULL ){
        if( strstr(buff,"struct") != NULL || strstr(buff,"union") != NULL ){
            flag = true;
        }

        if( strstr(buff,"{") != NULL ){
            kakko_counter++;
        }

        if(flag && kakko_counter>0 ){
            fprintf(fpl[FPO],"%s",buff);
        }
        
        if(kakko_counter == 0){
            flag = false;
        }

        if( strstr(buff,"}") != NULL ){
            kakko_counter--;
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