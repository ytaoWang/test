#ifndef __SCULL_H_
#define __SCULL_H_

#include <linux/list.h>
#include <linux/semaphore.h>
#include <asm/atomic.h>
#include <linux/wait.h>

#ifndef SCULL_MAJOR
#define SCULL_MAJOR 0 /* dynamic major by default */
#endif

#ifndef SCULL_NR_DEVS
#define SCULL_NR_DEVS 1 /* one scull device */
#endif

#define SCULL_SIZE 32
#define SCULL_MAX 12

struct scull_qset {
    void *data;		//data pointer
    struct list_head list;	//in scull device list
    size_t len;	// data length
    size_t curr; //current position
};

/*
struct scull_entity { //reader/writer
    int len;
    unsigned long size;
    struct list_head list;
    struct semaphore sem;
    struct scull_dev *dev;
};
*/  

struct scull_dev {
    atomic_t len; //list length
    unsigned long size; //data total size
    struct list_head list; //data list
    struct semaphore sem; 
    wait_queue_head_t inq,outq;//reader queue and write queue
    struct fasync_struct *async_queue;
    struct cdev cdev;// char device structure
};


extern int scull_major;
extern int scull_nr_devs;


#endif    
