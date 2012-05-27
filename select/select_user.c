#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/time.h>
#include <stdint.h>
#include <errno.h>

#define SELECT "/dev/select"

#define SELECT_MAX 12

int main(int argc, char *argv[])
{
    int fd1,fd2,max,retval,i,j;
    fd_set rfd,wfd,exfd,res_rfd,res_wfd,res_exfd;
    struct timeval tv;
    int ev[2];
    uint64_t t;
    char rbuf[33],wbuf[33];
    int rcnt,wcnt;

    if((fd1 = open(SELECT,O_RDONLY|O_NONBLOCK)) < 0) {
        fprintf(stderr,"open %s error:%s",SELECT,strerror(errno));
        return -1;
    }
    
    if((fd2 = open(SELECT,O_WRONLY|O_NONBLOCK)) < 0) {
        fprintf(stderr,"open %s error:%s",SELECT,strerror(errno));
        close(fd1);
        return -1;
    }
    
    FD_ZERO(&rfd);
    FD_ZERO(&wfd);
    FD_ZERO(&exfd);
    
    FD_SET(fd1,&rfd);
    FD_SET(fd1,&exfd);
    
    FD_SET(fd2,&wfd);
    FD_SET(fd2,&exfd);
    
    
    max = fd1;
    max = max < fd2?fd2:max;
    ev[0] = fd1;
    ev[1] = fd2;
    rcnt = 0;
    wcnt = 0;
    
    t = time(NULL);
    memset(rbuf,0,33);
    memset(wbuf,0,33);

    //snprintf(wbuf,32,"time:%llu\n",t);
    printf("rfd:%d,wfd:%d.max:%d\n",fd1,fd2,max);
    memset(wbuf,0,33);
    snprintf(wbuf,33,"hello,world!!!!,time:%llu\n",t);
    printf("wbuf:%s,len:%d\n",wbuf,strlen(wbuf));
    
    tv.tv_sec = 5;
    tv.tv_usec = 0;
    
    while(1) 
    {
        res_rfd = rfd;
        res_wfd = wfd;
        res_exfd = exfd;
        
        retval = select(max+1,&res_rfd,&res_wfd,&res_exfd,&tv);
        //retval = select(max+1,&rfd,&wfd,&exfd,&tv);
        if(retval < 0)
        {
            if(errno == EAGAIN || errno == EINTR || errno == EWOULDBLOCK)
                continue;

            fprintf(stderr,"select error:%s\n",strerror(errno));
            
            break;
        }
        
        printf("return fd:%d,tv_sec:%lu,tv_usec:%lu\n",retval,tv.tv_sec,tv.tv_usec);
        
        tv.tv_sec = 50;
        tv.tv_usec = 0;

        for(j = 0;j < retval; ++j) 
            for(i = 0;i < 2;++i){
                if(FD_ISSET(ev[i],&res_exfd)) {
                    FD_CLR(ev[i],&exfd);
                    FD_CLR(ev[i],&rfd);
                    FD_CLR(ev[i],&wfd);
                    close(ev[i]);
                    fprintf(stderr,"res_exfd fd:%d\n",ev[i]);
                    ev[i] = 0;
                    continue;
                }
            
                if(FD_ISSET(ev[i],&res_rfd)) {
                    memset(rbuf,0,32);
                    if(read(ev[i],rbuf,32) < 0) {
                        FD_CLR(ev[i],&exfd);
                        FD_CLR(ev[i],&rfd);
                        FD_CLR(ev[i],&wfd);
                        close(ev[i]);
                        ev[i] = 0;
                    }
                    rcnt ++;
                    fprintf(stderr,"ready to read:%s,read cnt:%d\n",rbuf,rcnt);
                }
            
                if(FD_ISSET(ev[i],&res_wfd)) {
                    t = time(NULL);
                    //memset(wbuf,0,32);
                    //snprintf(wbuf,32,"hello,world!!!!,time:%llu\n",t);
                    memset(wbuf,0,33);
                    snprintf(wbuf,33,"hello,world!!!!,time:%llu\n",t);
                    //printf("wbuf:%s,len:%d\n",wbuf,strlen(wbuf));
                    if(write(ev[i],wbuf,32) < 0) {
                        FD_CLR(ev[i],&exfd);
                        FD_CLR(ev[i],&rfd);
                        FD_CLR(ev[i],&wfd);
                        close(ev[i]);
                        ev[i] = 0;
                    }
                    wcnt++;
                    if(wcnt == SELECT_MAX)
                        continue;
                    fprintf(stderr,"ready to write,wcnt:%d\n",wcnt);
                }

            }
    }
    
    close(fd1);
    close(fd2);
    
    return 0;
}
