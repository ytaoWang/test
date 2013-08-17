/*
 * a simple regext matcher,only support ^,$,*
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
 * match regex expression from text (entry point)
 */
int match(const char *regexp,const char *text);

/*
 * match every regex string from text
 */
int matchhere(const char *regexp,const char *text);

/*
 * match mode: c*regexp from text
 */
int matchstar(int c,const char *regexp,const char *text);

int main(int argc,char *argv[])
{

  if(argc != 3){
    fprintf(stderr,"usage:%s regexp text\n",argv[0]);
    exit(-1);
  }

  printf("regexp:%s,text:%s,match result:%d\n",\
	 argv[1],argv[2],match(argv[1],argv[2]));

  return 0;
}

int match(const char *regexp,const char *text)
{
  if(*regexp == '^') 
    return matchhere(regexp+1,text);

  do {
    if(matchhere(regexp,text))
      return 1;
  } while(*text++ != '\0');

  return 0;
}

int matchhere(const char *regexp,const char *text)
{
  if(*regexp == '\0')
    return 1;
  
  if(regexp[1] == '*')
    return matchstar(regexp[0],regexp+2,text);

  if(regexp[0] =='$' && regexp[1] == '\0')
    return *text == '\0';

#ifdef DEBUG
  fprintf(stderr,"[INFO]:%s,regexp:%s,text:%s\n",__func__,regexp,text);
#endif
  if(*text != '\0' && (regexp[0] == '.' || regexp[0] == *text))
    return matchhere(regexp + 1,text + 1);
  /*
  while(*regexp != '\0') {
    if(*regexp == '*') return matchstar(regexp[0],regexp+2,text);
    if(*text == '\0') return 0;
    if(*regexp++ != *text++) return 0;
  }
  */
#ifdef DEBUG
  fprintf(stderr,"[INFO]:%s,run successful at last.\n",__func__);
#endif

  return 0;
}
/*
 * match string:c*regexp in text
 */
int matchstar(int c,const char *regexp,const char *text)
{
#ifdef DEBUG
  fprintf(stderr,"[INFO]:%s,regexp:%s,text:%s\n",__func__,regexp,text);
#endif
  if(*regexp == '$') return 1;
  do {
    if(matchhere(regexp,text))
      return 1;
  }while(*text != '\0' && (*text++ == c || c == '.'));

  return 0;
}
