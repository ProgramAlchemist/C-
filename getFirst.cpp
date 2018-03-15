#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include <cstring>
#include <cstdlib>
char *getFirst(char *words){
    char *first;
    char k1[1][4];
    char c = 'n';
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
int main() {
  char *words;
  words = (char *)malloc(sizeof(char)*20);
  scanf("%s",words);
  printf("%s\n",getFirst(words) );
  return 0;
}
