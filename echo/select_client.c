#include <sys/select.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <netdb.h>

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <fcntl.h>

#define EVENT_READ (1<<0)
#define EVENT_WRITE (1<<1)
#define EVENT_EXCEPTION (1<<2)

#define MAX 2000
#define LEN 1024

//header
struct header {
    uint32_t cmd;
    uint32_t length;
    uint32_t error;
    uint32_t para1;
    uint32_t para2;
};

    
//select event
struct se {
    int fd;
    int event;
    char rbuf[LEN];
    char wbuf[LEN];
};

static int handle_write(struct se *ev);
static int handle_read(struct se *ev);
static int handle_exception(struct se *ev);

int creat_socket(const char *address,int port)
{
    int fd;
    struct sockaddr_in  addr;
    
    if((fd = socket(AF_INET,SOCK_STREAM,0)) < 0)
        return -1;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(address);
    addr.sin_port = htons(port);
    
    if(connect(fd,(struct sockaddr *)&addr,sizeof(addr)) <0)
    {
        close(fd);
        perror("connect error");
        return -1;
    }

    return fd;
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
    int sockfd,port,max,i,len,curr,s,retval,num;
    fd_set rfd,wfd,exfd,res_rfd,res_wfd,res_exfd;
    struct se ev[MAX];
    struct timeval tv;
    
    if(argc != 4)
    {
        fprintf(stderr,"usage:%s address port num\n",argv[0]);
        return -1;
    }
    
    port = atoi(argv[2]);
    num = atoi(argv[3]);
        
    FD_ZERO(&rfd);
    FD_ZERO(&wfd);
    FD_ZERO(&exfd);
    
    //FD_ZERO(&res_rfd);
    //FD_ZERO(&res_wfd);
    //FD_ZERO(&res_exfd);
    max = 0;
    
    curr = 0;
    len = 1;
    memset(ev,0,sizeof(ev[0]) * MAX);
    
    for(i = 0;i< num;i++)
    {
        if((sockfd = creat_socket(argv[1],port)) <0)
        {
            fprintf(stderr,"fail to creat socket:%s\n",strerror(errno));
            return -1;
        }
        
        //FD_SET(sockfd,&rfd);
        FD_SET(sockfd,&wfd);
        FD_SET(sockfd,&exfd);
        
        ev[curr].fd = sockfd;
        //ev[curr++].event = EVENT_READ|EVENT_WRITE|EVENT_EXCEPTION;
        ev[curr ++].event = EVENT_WRITE| EVENT_EXCEPTION;
        
        max = max < sockfd?sockfd:max;
        setNonblock(sockfd);
    }
    
    len = curr;
    
    printf("socket fd:%d\n",sockfd);
    
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
            
            return -1;
        }
        
        printf("return fd:%d,tv_sec:%lu,tv_usec:%lu\n",retval,tv.tv_sec,tv.tv_usec);
        
        tv.tv_sec = 50;
        tv.tv_usec = 0;

        for(i = 0;i < retval; ++i) {

            if(FD_ISSET(ev[i].fd,&res_exfd))
            {
                FD_CLR(ev[i].fd,&exfd);
                FD_CLR(ev[i].fd,&rfd);
                FD_CLR(ev[i].fd,&wfd);
                handle_exception(&ev[i]);
                memset(&ev[i],0,sizeof(ev[i]));
                continue;
            }

            if(FD_ISSET(ev[i].fd,&res_rfd)) {
                if(handle_read(&ev[i]) < 0)
                {
                    FD_CLR(ev[i].fd,&exfd);
                    FD_CLR(ev[i].fd,&rfd);
                    FD_CLR(ev[i].fd,&wfd);
                    memset(&ev[i],0,sizeof(ev[i]));
                    continue;
                }
                FD_SET(ev[i].fd,&wfd);
                FD_CLR(ev[i].fd,&rfd);
                ev[i].event &= ~EVENT_READ;
                ev[i].event |= EVENT_WRITE;
            }
            if(FD_ISSET(ev[i].fd,&res_wfd)) {
                if(handle_write(&ev[i]) < 0) {
                    FD_CLR(ev[i].fd,&exfd);
                    FD_CLR(ev[i].fd,&rfd);
                    FD_CLR(ev[i].fd,&wfd);
                    memset(&ev[i],0,sizeof(ev[i]));
                    continue;
                }
                
                FD_SET(ev[i].fd,&rfd);
                FD_CLR(ev[i].fd,&wfd);
                ev[i].event &= ~EVENT_WRITE;
                ev[i].event |= EVENT_READ;
            }
        }
        
    }
    
    return 0;
}


static int handle_write(struct se *p)
{
    int len;
    struct header h;
    char q[] = "hello";
    
    memset(&h,0,sizeof(h));
    memset(p->wbuf,0,LEN);
    
    h.length = strlen(q);
    memcpy(p->wbuf,&h,sizeof(h));
    memcpy(p->wbuf+sizeof(h),q,h.length);
 again:
    if((len = write(p->fd,p->wbuf,sizeof(h) + strlen(q)))<0) {
        if(errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK)
            goto again;
        return -1;
    }
    
    printf("f:%s,line:%d,write:%s\n",__FUNCTION__,__LINE__,p->wbuf + sizeof(h));
    return 0;
}

static int handle_read(struct se *p)
{
    int len;
    struct header *h;

 again:
    if((len = read(p->fd,p->rbuf,LEN))<0) {
        if(errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK)
            goto again;
        return -1;
    }
    
    printf("f:%s,line:%d,read:%s\n",__FUNCTION__,__LINE__,p->rbuf + sizeof(*h));
    return 0;
}

static int handle_exception(struct se *p)
{
    perror("handle_error\n");
    close(p->fd);
    return 0;
}
