/*
  * given a string,check whether it's a palindrome string or not
  *
  * for google --> goog
  *     goddog123 -->goddog
  *		ggab --> gg
  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int is_palindrome(const char *str,int size,int *s,int *e)
{
    int i,prev,len,start,end;
    
    prev = 0;
    start = 0;
    len = 0;
    *s = 0;
    *e = 0;
    end = 0;
    /*
     * symmtery with even
     */
    for(i = 1;i < size;++i) {

        if(prev < 0 || str[i] != str[prev]) {
            if(prev < 0) prev = 0;
            if(len < (i - prev)) {
                end = i;
                len = i - start;
            }
#ifdef DEBUG
            printf("new echo i:%d,char:%c,len:%d\n",i,str[i],len);
#endif
            prev = i;
            continue;
        }
        
        if(str[i] == str[prev]) {
#ifdef DEBUG
            printf("equal i:%d,prev:%d,char:%c\n",i,prev,str[prev]);
#endif
            prev--;
        }
    }
#ifdef DEBUG
    printf("prev:%d,i:%d\n",prev,i);
#endif    

    if(prev < 0 && len < i) {
        *s = 0;
        *e = i;
    } else {
        *s = start;
        *e = end;
    }

    prev = 0;
    start = 0;
    len = 0;
    end = 0;
    
    /*
     * symmtery with odd
     */
    for(i = 2; i < size;++i) {
        
        if(prev < 0 || str[i] != str[prev]) {
            if(prev < 0) prev = 0;
            if(len < (i - prev)) {
                end = i + 1;
                len = i - start;
            }
#ifdef DEBUG
            printf("new echo i:%d,char:%c,len:%d\n",i,str[i],len);
#endif

            prev = i-1;
            continue;
            
            if(str[i] == str[prev]) {
#ifdef DEBUG
                printf("equal i:%d,prev:%d,char:%c\n",
                       i,prev,str[prev]);
#endif
                prev--;
            }
            
        }
        
    }
    

    if(prev < 0 && *e < i) {
        *s = 0;
        *e = i;
    }
 
    if(*e < end){
        *s = start;
        *e = end;
    }

    return len != 0;
}

int main(int argc,char *argv[])
{

    const char *p = "cbabcbabb";
    int s,e;
    
    if(is_palindrome(p,strlen(p),&s,&e)) {
        printf("yes start:%d,end:%d,string:",s,e);
        while(s < e) {
            printf("%c",p[s]);
            s++;
        }
        printf("\n");
    } else
        printf("%s isn't palindrome.\n",p);
    
    return 0;
}
