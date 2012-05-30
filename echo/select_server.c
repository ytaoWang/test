#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/resource.h>

#define EVENT_READ (1<<0)
#define EVENT_WRITE (1<<1)
#define EVENT_EXCEPTION (1<<2)

#define MAX 2000
#define LEN 1024

//select event
struct se {
    int fd;
    int event;
    char rbuf[LEN];
    char wbuf[LEN];
    int init;
};

struct header {
    uint32_t cmd;
    uint32_t length;
    uint32_t error;
    uint32_t para1;
    uint32_t para2;
};

    
    
static int handle_write(struct se *ev);
static int handle_read(struct se *ev);
static int handle_exception(struct se *ev);


static int set_signal_handler(int sig,void (*handler)(int))
{
    struct sigaction act,oact;
    
    memset(&act,0,sizeof(act));
    memset(&oact,0,sizeof(oact));
    
    act.sa_handler = handler;
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);
    
    if(sigaction(sig,&act,NULL) < 0) {   
        perror("set signal handler error");
        return -1;
    }
    
    if(act.sa_handler == SIG_DFL && 
       sigaction(sig,NULL,&oact) == -1) {
        perror("set signal handler error");
        return -1;
    }
    
    return 0;
}


int creat_socket(const char *port)
{
    struct addrinfo hints,*res,*rp;
    int sockfd = -1;
    int err;
    
    memset(&hints,0,sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = 0;
    hints.ai_flags = AI_V4MAPPED | AI_PASSIVE;
    err = getaddrinfo("127.0.0.1",port,&hints,&res);
    if(err) {
        fprintf(stderr,"getaddrinfo: %s\n",gai_strerror(err));
        exit(EXIT_FAILURE);
    }
    
    for(rp = res;rp;rp = rp->ai_next) {
        
        int optval;
        sockfd = socket(rp->ai_family,rp->ai_socktype,rp->ai_protocol);
        if(sockfd == -1) {
            perror("socket");
            continue;
        }
        
        optval = 1;
        err = setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&optval,sizeof(optval));
        if(err == -1) {
            perror("setsocketopt");
            close(sockfd);
            sockfd = -1;
            continue;
        }
        
        err = bind(sockfd,rp->ai_addr,rp->ai_addrlen);
        if(err == -1) {
            perror("bind");
            close(sockfd);
            sockfd = -1;
            continue;
        }
        
        break;
    }
    
    freeaddrinfo(res);
    
    if(sockfd == -1) {
        fprintf(stderr,"can't bind at all\n");
        exit(EXIT_FAILURE);
    }
    
    err = listen(sockfd,5);
    if(err == -1) {
        perror("listen");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    
    return sockfd;
}

void setNonblock(int fd)
{
    int flags;
    
    if((flags = fcntl(fd,F_GETFL,0)) < 0) {
        perror("setNonblock");
        return;
    }
    
    flags |= O_NONBLOCK;
    
    if(fcntl(fd,F_SETFL,flags) < 0) {
        perror("setNonblock");
        return;
    }
    
}

int main(int argc,char *argv[])
{
    int sockfd,retval,max,i,len,curr,s,j;
    
    fd_set rfd,wfd,exfd,res_rfd,res_wfd,res_exfd;
    struct timeval tv;
    struct se ev[MAX];
    struct rlimit res;
    
    set_signal_handler(SIGPIPE,SIG_IGN);

    if(argc != 2) {
        fprintf(stderr,"usage:%s port\n",argv[0]);
        return -1;
    }
    
    sockfd = creat_socket(argv[1]);
    
    setNonblock(sockfd);
    FD_ZERO(&rfd);
    FD_ZERO(&wfd);
    FD_ZERO(&exfd);
    
    FD_SET(sockfd,&rfd);
    FD_SET(sockfd,&exfd);
    
    max = sockfd;
    
    curr = 0;
    len = 1;
    memset(ev,0,sizeof(ev[0]) * MAX);
    ev[curr].fd = sockfd;
    ev[curr].init = 1;
    ev[curr++].event = EVENT_READ|EVENT_EXCEPTION;
    
    tv.tv_sec = 5;
    tv.tv_usec = 0;
    
    
    res.rlim_cur = 10000;
    res.rlim_max = res.rlim_cur * 2;
    
    if(setrlimit(RLIMIT_NOFILE,&res) < 0) 
    {
        perror("set open file size");
        return -1;
    }
    
    while(1) {
        
        res_rfd = rfd;
        res_wfd = wfd;
        res_exfd = exfd;

        //printf("res_rfd:%d,rfd:%d,res_wfd:%d,wfd:%d,res_exfd:%d,exfd:%d\n",res_rfd,rfd,res_wfd,wfd,res_exfd,exfd);
        retval = select(max+1,&res_rfd,&res_wfd,&res_exfd,&tv);
        if(retval < 0) {
            if(errno == EAGAIN || errno == EINTR || errno == EWOULDBLOCK)
                continue;
            return -1;
        }
        printf("ready fd:%d,tv_sec:%lu,tv_usec:%lu,socket fd:%d\n",retval,tv.tv_sec,tv.tv_usec,sockfd);
        
        tv.tv_sec = 5;
        tv.tv_usec = 0;

        for(i = 0;i < retval; ++i) {
            for(j = 0;j<MAX;j++)
            {
                if(!ev[j].fd) continue;
                if((ev[j].fd == sockfd) && (FD_ISSET(ev[j].fd,&res_rfd))) {
                    while ((s= accept(sockfd,NULL,NULL))>0) {
                        if(curr < MAX) {
                            len ++;
                            ev[curr].fd = s;
                            ev[curr++].event = EVENT_READ|EVENT_EXCEPTION;
                            max = max < s?s:max;
                            //FD_CLR(s,&rfd);
                            //FD_CLR(s,&exfd);
                            FD_SET(s,&rfd);
                            FD_SET(s,&exfd);
                            printf("f:%s,line:%d,accept connection fd:%d,curr:%d\n",__FUNCTION__,__LINE__,s,curr);
                        } else {
                            int k;
                            for(k=0;k<MAX;k++) 
                            {
                                if(!ev[k].fd) {
                                    ev[k].fd = s;
                                    ev[k].event = EVENT_READ|EVENT_EXCEPTION;
                                    max = max < s?s:max;
                                    //FD_CLR(s,&rfd);
                                    //FD_CLR(s,&exfd);
                                    FD_SET(s,&rfd);
                                    FD_SET(s,&exfd);
                                    break;
                                }
                            }
                        if(k == MAX) {
                            printf("has reach the maximum connections!\n");
                            close(s);
                        }
                    }
                    printf("f:%s,line:%d,acception connection fd:%d,curr:%d\n",__FUNCTION__,__LINE__,s,curr);
                    setNonblock(s);
                }
                continue;
            } else {
                    printf("f:%s,line:%d,fd:%d\n",__FUNCTION__,__LINE__,ev[j].fd);
                
                    if(FD_ISSET(ev[j].fd,&res_exfd)) 
                    {
                        FD_CLR(ev[j].fd,&exfd);
                        FD_CLR(ev[j].fd,&rfd);
                        FD_CLR(ev[j].fd,&wfd);
                        handle_exception(&ev[j]);
                        memset(&ev[j],0,sizeof(ev[j]));
                        continue;
                    }

                    if(FD_ISSET(ev[j].fd,&res_rfd)) {
                        if(handle_read(&ev[j])<0) {
                            FD_CLR(ev[j].fd,&exfd);
                            FD_CLR(ev[j].fd,&rfd);
                            FD_CLR(ev[j].fd,&wfd);
                            memset(&ev[j],0,sizeof(ev[j]));
                            continue;
                        }
                        FD_SET(ev[j].fd,&wfd);
                        FD_CLR(ev[j].fd,&rfd);
                        ev[j].event &= ~EVENT_READ;
                        ev[j].event |= EVENT_WRITE;
                    }
                    if(FD_ISSET(ev[j].fd,&res_wfd)) {
                        if(handle_write(&ev[j]) < 0) {
                            FD_CLR(ev[j].fd,&exfd);
                            FD_CLR(ev[j].fd,&rfd);
                            FD_CLR(ev[j].fd,&wfd);
                            memset(&ev[j],0,sizeof(ev[i]));
                            continue;
                        }
                        FD_SET(ev[j].fd,&rfd);
                        FD_CLR(ev[j].fd,&wfd);
                        ev[j].event &= ~EVENT_WRITE;
                        ev[j].event |= EVENT_READ;
                    }
                }     
            }
            
            
        }
        
    }
    return 0;
}


static int handle_write(struct se *p)
{
    int len;
 again:
    if((len = write(p->fd,p->rbuf,LEN))<0) {
        if(errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK)
            goto again;
        fprintf(stderr,"send error:%s\n",strerror(errno));
        return -1;
    }
    
    printf("f:%s,line:%d,write:%s\n",__FUNCTION__,__LINE__,p->rbuf + sizeof(struct header));
    return 0;
}

static int handle_read(struct se *p)
{
    int len;
 again:
    if((len = read(p->fd,p->rbuf,LEN))<0) {
        //printf("read error:%s\n",strerror(errno));
        if(errno == EINTR)
            goto again;
        if( errno == EAGAIN || errno == EWOULDBLOCK)
            return 0;

        fprintf(stderr,"recv error:%s,fd:%d",strerror(errno),p->fd);        
        return -1;
    }
    
    printf("f:%s,line:%d,read:%s\n",__FUNCTION__,__LINE__,p->rbuf + sizeof(struct header));
    return 0;
}

static int handle_exception(struct se *p)
{
    perror("handle_error\n");
    return 0;
}
