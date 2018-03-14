#include <cstring>
#include <cstdlib>
#include "stdio.h"
#define MAXSIZE 1024
#define FAIL 0
#define OK  1
struct Words{
    char * elem;                 // 汉字词语
    Words * next;                // 链表下一个结点
};
typedef struct HashNode{
    char *  key;                 //  键 汉字词语首字
    Words * link;                //  值 一个链表指针
    struct HashNode * next;
}Node;
Node bucket[MAXSIZE];
Words * pWords;
Words * qWords;
Node * pNodes;

int HashTableSize = 0;
int flag = 0;
FILE *fp;
FILE *fpo;

int Find(Node *p,char *str);
int Find_link(Words* p,char *str);
int Insert(char *);
//创建链表
int init_linktable(Node *x){
    Words * newWords;
    newWords = (Words *)malloc(sizeof(Words));
    if (x->link==NULL) {
        x->link = newWords;
        newWords->elem = NULL;
        newWords->next = NULL;
        return OK;
    }
    else {
        return FAIL;
    }
}
//向链表中插入字符串
int insert_linktable(Node *x, char *str){

    Words * newWords;
    Words * oldWords;
    oldWords = x->link;
    newWords = oldWords->next;
    if (x==NULL)
        return FAIL;
    else {
    while (newWords!=NULL)
    {
        oldWords = oldWords->next;
        newWords = oldWords->next;

    }
        newWords = (Words *)malloc(sizeof(Words));
        oldWords->next = newWords;
    newWords->elem = str;
    newWords->next = NULL;
        return OK;
    }

}
//根据首字判断地址
int keyToIndex(char * key){
    int index;
    index = ((unsigned char)key[0]-129)*190 +
            ((unsigned char)key[1]-64) - (unsigned char)key[1]/128;

    if (index > 0)
    return index%MAXSIZE;
    else return 0;
}
//取汉字字符串的首个汉字，返回字符串类型
char *getFirst(char *words){
    char *first;
    char k1[1][4];
    char c;
    first = (char *)malloc(sizeof(char)*4);
    c = words[0];
    if( 0<=c && c<=127)
    {
        first[0] = c;
        first[1] = '\0';
    }
    else{

        k1[0][0] = words[0];
        k1[0][1] = words[1];
        k1[0][2] = words[2];
        k1[0][3] = '\0';
        strncpy(first,k1[0],sizeof(k1));
    }
    return first;
}
//向哈希表中插入数据
int insertValue(Node *p, char * str, char * value)
{
    if (Find(p,value)) {
        return FAIL;
    }
    if (HashTableSize>=MAXSIZE){
        printf("Out of memory\n");
        return FAIL;
    }

    if(p->key==NULL){                   //首字不存在

        p->key = str;
        if(init_linktable(p)==1){
            p->link->elem = value;
            p->link->next = NULL;
            HashTableSize++;
            return OK;
        }
        else {
            printf("链表创建失败\n");
            return FAIL;
        }
    }
    else {                                      //首字存在

        if (!strcmp(p->key,str)){             //首字相同 在这个结点的链表里插入value

            if(insert_linktable(p,value)==1){
            } else printf("插入失败!\n");
            return 2;
        }
        else {                                  //首字不同
            if(p->next==NULL)
            {
                Node * newNode;
                Node * oldNode;
                oldNode = p;
                newNode = (Node*)malloc(sizeof(Node));

                newNode->key = str;
                newNode->link = NULL;
                oldNode->next = newNode;
                if(init_linktable(newNode)==1){
                    newNode->link->elem = value;
                    newNode->link->next = NULL;
                    return 3;
                }
                else {
                    printf("链表创建失败");
                    return FAIL;}
            }
            else {
                insertValue(p->next,str,value);

            }
        }

    }
    return OK;
}
//输入
int Input(){
    char * value;
    char * k;
    value = (char *)malloc(sizeof(char)*60);
    scanf("%s",value);
    k = getFirst(value);
    int index = keyToIndex(k);
    printf("%d\n",insertValue(&bucket[index],k,value));
    printf("key = %s\n",k);
    return OK;
}
//  删除词语
int Delete(){
    char * dvalue;
    char * k;
    dvalue = (char *)malloc(sizeof(char)*60);
    scanf("%s",dvalue);
    k = getFirst(dvalue);
    int index = keyToIndex(k);
    if (Find(&bucket[index],dvalue)) {
        if (pNodes->link==pWords) {
            if(pWords->next==NULL){
                pNodes->link=NULL;
                pNodes->key = NULL;
                pNodes = NULL;
                free(pWords);
                return 1;
            }
            else {
                pNodes->link = pWords->next;
                free(pWords);
                return 4;
            }

        }
        else if (pWords->next==NULL){
            qWords->next=NULL;
            free(pWords);
            return 2;
        }
        else {
            qWords->next = pWords->next;
            free(pWords);
            return 3;
        }

    }
    else {
        printf("没有这个词！\n");
        return 0;
    }
}
//  修改词语
int Change(){
    char * cvalue;
    char * kvalue;
    char * k;
    cvalue = (char *)malloc(sizeof(char)*60);
    kvalue = (char *)malloc(sizeof(char)*60);
    scanf("%s",cvalue);
    k = getFirst(cvalue);
    int index = keyToIndex(k);
    if (Find(&bucket[index],cvalue)){
        printf("请输入要改成的词：(请输入首字相同的词语)\n");
        scanf("%s",kvalue);
        if(Find(&bucket[index],kvalue)!=1){
            Find(&bucket[index],cvalue);
            strcpy(pWords->elem,kvalue);

        }

        return 1;
    }
    else {
        printf("没有这个词！\n");
        return 0;
    }

}
//  插入
int Insert(char * value){
    char * k;
    char * str;
    str = (char *)malloc(sizeof(char)*80);
    k = (char *)malloc(sizeof(char)*20);
    strcpy(str,value);
    k = getFirst(str);
    int index = keyToIndex(k);
    insertValue(&bucket[index],k,str);

    if(Find(&bucket[index],k)==0)insertValue(&bucket[index],k,k);

}
//  判断空
int isEmpty (Node *p){
    if (p==NULL||p->link==NULL) return 1;
    else return 0;
}
//在链表中查找字符串是否存在
int Find_link(Words* p,char *str){
    pWords = p;
    qWords = pWords;
    while(p!=NULL){
        if(!strcmp(p->elem,str)) {
            pWords = p;
            return OK;}
        qWords = p;
        p = p->next;
    }
    return FAIL;
}
//在哈希结点中查找字符串是否存在
int Find(Node *p,char *str){

    char *str1;
    str1 = getFirst(str);
    if (p->key==NULL||p==NULL)                          //首字不存在
    {
        return FAIL;
    }
    else if (!strcmp(p->key,str1)){                     //首字存在且相同
        if(Find_link(p->link,str)==1){
            pNodes = p;
            return OK;
        }
        else {
            return FAIL;
        }
    }
    else                                               //首字存在但不相同
        {
        if(p->next!=NULL)
            {
                p = p->next;
            if(Find(p,str))return OK;
                else return FAIL;
            }
        else
            {
             return FAIL;
            }
        }
}
// 遍历链表
int travelLink(Words *y){
    if(y==NULL)return 1;
    else {
        if(y->next!=NULL) travelLink(y->next);
        printf("%s\n",y->elem);
    }
}
// 遍历结点
int travelNode(Node *x){
    if(x==NULL)return 1;
    else {
        if(x->next!=NULL) travelNode(x->next);
        printf("首字%s:\n",x->key);
        travelLink(x->link);
        printf("------------*-----------\n\n");
    }
}
// 分词
int divideWords(char *key){
    long int x = 0,i = 0,j = 1,k = 0,index;
    char * value;
    value = (char *)malloc(sizeof(char)*6000);

    while(key[i]!='\0'){
        i++;
    }

    while (x<i)
    {   if (j == 0||x ==i)
        {
            printf("（词典中未录入该词）\n");
            break;}

        for (j = i;j > 0 ; j--)
        {
            for (k = 0; k < j;k++)
            {
                value[k] = key[k+x];
            }
            value[k] = '\0';
            index = keyToIndex(value);
            if(Find(&bucket[index],value)==1)
            {
                printf("%s|",value);
                if(flag)fprintf(fpo, "%s|", value);
                x = x+k;
                break;
            }
            else if ((k)==0) {
                x = i;
                break;
            }
        }
    }
    if(flag)fprintf(fpo, "%c", '\n');
    printf("\n");
    if(flag)fprintf(fpo, "%c", '\n');
    printf("\n");
}
int main(){
    int i,op = 1;
    int index;
    char *key;
    char *filevalue;
    char *filename;
    while (op)
    {
        filename = (char *)malloc(sizeof(char)*60);
        key = (char *)malloc(sizeof(char)*6000);
        filevalue = (char *)malloc(sizeof(char)*6000);
        system("clear");	printf("\n\n");
        printf("                      中文分词系统菜单                               \n");
        printf(" ----------------------------------------------------------\n");
        printf("|    	  1. 手动录入词语         2. 手动删除词语         |        \n");
        printf("|    	  3. 查找指定词语         4. 修改指定词语         |           \n");
        printf("|    	  5. 文件文章分词         6. 输入句子分词         |           \n");
        printf("|    	  7. 文件导入词典         8. 文件打印词典         |           \n");
        printf(" ----------------------------------------------------------\n");
        printf("                   请选择你的操作[0~8]:                        \n");
        printf("                      %d/%d           \n",HashTableSize,MAXSIZE);
        scanf("%d",&op);
        switch(op){
            case 1:     {
                            printf("请输入词语或单词:\n");
                            Input();
                            getchar();getchar();
                            break;
                        }
            case 2:     {
                            printf("请输入要删除的词语:\n");
                            printf("%d",Delete());
                            getchar();getchar();
                            break;
            }


            case 3:     {
                            printf("请输入要查找的词语\n");
                            scanf("%s",key);
                            index = keyToIndex(key);
                            if (!isEmpty(&bucket[index])) {
                               if(Find(&bucket[index],key)==OK)printf("查找成功！");
                               else printf("查找失败！\n");
                            }
                            else printf("当前key值无内容\n");
                            getchar();getchar();
                            break;

                        }
            case 4:     {
                            printf("请输入要修改的词语:\n");
                            if (Change()==1)printf("修改成功！");
                            getchar();getchar();
                            break;
            }

            case 5:     {
                            printf("请输入文件名\n");
                            scanf("%s",filename);
                            fp = fopen(filename,"r+");
                            fpo = fopen("Out.txt","w");
                            if (fp==NULL){
                                printf("不存在！\n");
                                getchar();getchar();
                                break;
                            }
                            flag = 1;
                            while (strcmp(filevalue,"END"))
                            {
                                fscanf (fp,"%s",filevalue);
                                divideWords(filevalue);

                            }

                            if(fclose(fp)==0)
                                printf("文件读取成功\n");
                            else printf("ERROR!\n");
                            fclose(fpo);
                            fclose(fp);
                            flag = 0;

                            getchar();getchar();
                            break;
                        }
            case 6:     {
                            printf("请输入语句\n");
                            scanf("%s",key);
                            divideWords(key);

                            getchar();getchar();
                            break;
                        }
            case 7:     {
                            printf("请输入文件名\n");
                            scanf("%s",filename);
                            fp = fopen(filename,"r+");
                            if (fp==NULL){
                                printf("不存在！\n");
                                getchar();getchar();
                                break;
                            }
                            while (strcmp(filevalue,"END"))
                            {
                                fscanf (fp,"%s",filevalue);
                                Insert(filevalue);
                            }
                            filevalue = "0";
                            if(fclose(fp)==0)
                                printf("文件读取成功\n");
                            else printf("ERROR!\n");
                            fclose(fp);
                            getchar();getchar();
                            break;

                        }
            case 8:     {

                printf("请输入文件名\n");
                scanf("%s",filename);
                fp = fopen(filename,"w");
                for(i=0;i<1024;i++) {
                  if(bucket[i].key!=NULL) {
                      pNodes = &bucket[i];
                      while (1) {
                          pWords = pNodes->link;
                          while (1) {

                              if(pWords->elem!=NULL){
                                  strcpy(key,pWords->elem);
                                  fprintf(fp, "%s\n", key);
                              }
                              if (pWords->next!= NULL) pWords = pWords->next;
                              else break;
                          }
                          if (pNodes->next != NULL) pNodes = pNodes->next;
                          else break;
                      }
                  }
                }
                fprintf(fp, "%s","END");
                fclose(fp);
                printf("文件生成成功\n");
                getchar();getchar();
                break;
            }
            case 9:     {
                printf("请输入你想要遍历位置的首字\n");
                scanf("%s",key);
                index = keyToIndex(key);
                printf("当前key有以下内容\n**************************\n\n");
                if (!isEmpty(&bucket[index])) travelNode(&bucket[index]);
                else printf("当前key值无内容\n");
                getchar();getchar();
                break;

            }
            case 0 :    break;
        }
    }
    return 0;
}
