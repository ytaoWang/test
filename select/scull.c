/*
 * chardev reference by http://linux.die.net/lkmpg/x569.html 
 * Create a read-only char device that says how many times,you've 
 * read from the dev file
 */

#include <linux/kernel.h> //printk
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/types.h>
#include <linux/kdev_t.h>    
#include <linux/slab.h> //kmalloc
#include <linux/uaccess.h>
#include <linux/errno.h>
#include <linux/poll.h>
#include <linux/fs.h>
#include <linux/sched.h>

#include "scull.h"


MODULE_LICENSE("Dual BSD/GPL");

int scull_major = SCULL_MAJOR;
int scull_minor = 0;
int scull_nr_devs = SCULL_NR_DEVS;

static struct scull_dev *scull_devices;
static struct kmem_cache *scullc_cache;

static void scull_wait_reader(struct scull_dev *);
static void scull_wait_writer(struct scull_dev *);
static void scull_qset_destroy(struct scull_qset *);

static int scull_trim(struct scull_dev *dev)
{
    struct scull_qset *n,*pos;
    
    list_for_each_entry_safe(pos,n,&dev->list,list) {
        scull_qset_destroy(pos);
    }

    return 0;
}

static void scull_qset_destroy(struct scull_qset *q) {
    list_del(&q->list);
    kfree(q->data);
    kmem_cache_free(scullc_cache,q);
}


static loff_t scull_llseek(struct file *filp ,loff_t off,int whence)
{
    return -ENOSYS;
}


ssize_t scull_read(struct file *filp,char __user *buf,size_t count,loff_t *offp)
{
    struct scull_dev *dev = filp->private_data;
    struct scull_qset *pos;
    
    if(count != SCULL_SIZE) 
        return -EINVAL;
    
    while(!atomic_read(&dev->len)) {
        if(filp->f_flags & O_NONBLOCK) 
            return -EAGAIN;

        if(signal_pending(current)) 
            return -ERESTARTSYS;
        
        #ifdef SCULL_DEBUG
        PDEBUG("\"%s\" reading: going to sleep\n",current->comm);
        #endif
        
        // if(down_interruptible(&dev->sem))
        //    return -ERESTARTSYS;
        scull_wait_reader(dev);
    }
    
    if(down_interruptible(&dev->sem))
        return -ERESTARTSYS;

    pos = list_entry(dev->list.next,struct scull_qset,list);
    printk(KERN_NOTICE "\"%s\" read data:%s\n",current->comm,(char *)pos->data);
    if(copy_to_user(buf,pos->data,count)) {
        up(&dev->sem);
        return -EFAULT;
    }

    scull_qset_destroy(pos);

    if(atomic_read(&dev->len) <= SCULL_MAX) {
        atomic_dec(&dev->len);
        wake_up_interruptible(&dev->outq);
        if(dev->async_queue)
           kill_fasync(&dev->async_queue,SIGIO,POLL_OUT);
    }
    
    up(&dev->sem);
    
    printk(KERN_NOTICE "scull_read is called in here.\n");

    return count;
}

static ssize_t scull_write(struct file *filp,const char __user *buf,size_t count,loff_t *offp)
{
    struct scull_dev *dev = filp->private_data;
    struct scull_qset *pos;
    
    if(count != SCULL_SIZE)
        return -EINVAL;

    while(SCULL_MAX == atomic_read(&dev->len)) {
        if(filp->f_flags & O_NONBLOCK)
            return -EAGAIN;
        
        if(signal_pending(current))
            return -ERESTARTSYS;
		#ifdef SCULL_DEBUG
        PDEBUG("\"%s\" writing: going to sleep\n",current->comm);
        #endif        
        
        //if(down_interruptible(&dev->sem))
        //    return -ERESTARTSYS;
        scull_wait_writer(dev);
        //up(&dev->sem);
    }

    if(!(pos = kmem_cache_alloc(scullc_cache,GFP_KERNEL)))
        return -ENOMEM;
    
    memset(pos,0,sizeof(struct scull_qset));
    
    if(!(pos->data = kmalloc(SCULL_SIZE,GFP_KERNEL))) {
        kmem_cache_free(scullc_cache,pos);
        return -ENOMEM;
    }
    
    INIT_LIST_HEAD(&pos->list);
    
    if(copy_from_user(pos->data,buf,count)) {
        kfree(pos->data);
        kmem_cache_free(scullc_cache,pos);
        return -EFAULT;
    }
    
    if(down_interruptible(&dev->sem))
        return -ERESTARTSYS;

    list_add_tail(&pos->list,&dev->list);
    
    up(&dev->sem);
    
    if(atomic_add_return(1,&dev->len) >= 1) {
        wake_up_interruptible(&dev->inq);
        if(dev->async_queue)
            kill_fasync(&dev->async_queue,SIGIO,POLL_IN);
    }
    
    printk(KERN_NOTICE "scull_write is called in here.\n");
    
    return count;
}

static int scull_open(struct inode *inode,struct file *filp)
{
    struct scull_dev *dev;
    
    dev = container_of(inode->i_cdev,struct scull_dev,cdev);
    
    filp->private_data = dev;
    
    /*
    if((filp->f_flags & O_ACCMODE) == O_WRONLY) {
        scull_trim(dev);
    }
    */

    return 0;
}

static int scull_release(struct inode *inode,struct file *filep)
{
    return 0;
}

static int scull_fasync(int fd,struct file *filp,int mode)
{
    struct scull_dev *dev = filp->private_data;
        //return -ENOSYS;
    return fasync_helper(fd,filp,mode,&dev->async_queue);
}


static unsigned int scull_poll(struct file *filp,struct poll_table_struct *wait)
{
    struct scull_dev *dev = filp->private_data;
    unsigned int mask = 0;
    
    printk(KERN_NOTICE "Select into call\n");
    
    poll_wait(filp,&dev->inq,wait);
    poll_wait(filp,&dev->outq,wait);
    
    if(atomic_read(&dev->len) != 0)
        mask |= POLLIN | POLLRDNORM;
    if(atomic_read(&dev->len) != SCULL_MAX)
        mask |= POLLOUT | POLLWRNORM;
    
    return mask;
}


struct file_operations scull_fops = {
    .owner = THIS_MODULE,
    .llseek = scull_llseek,
    .read = scull_read,
    .write = scull_write,
    .open = scull_open,
    .release = scull_release,
    .poll = scull_poll,
    .fasync = scull_fasync,
};


static void scull_wait_reader(struct scull_dev *dev)
{
    DEFINE_WAIT(wait);
    //up(&dev->sem);
    prepare_to_wait(&dev->inq,&wait,TASK_INTERRUPTIBLE);
    schedule();
    finish_wait(&dev->inq,&wait);
}

static void scull_wait_writer(struct scull_dev *dev)
{
    DEFINE_WAIT(wait);
    //up(&dev->sem);
    prepare_to_wait(&dev->outq,&wait,TASK_INTERRUPTIBLE);
    schedule();
    finish_wait(&dev->outq,&wait);
}


static void scull_setup_cdev(struct scull_dev *dev,int index)
{
    int err,devno = MKDEV(scull_major,scull_minor + index);
    
    cdev_init(&dev->cdev,&scull_fops);
    dev->cdev.owner = THIS_MODULE;
    dev->cdev.ops = &scull_fops;
    dev->async_queue = NULL;
    err = cdev_add(&dev->cdev,devno,1);
    
    if(err)
        printk(KERN_NOTICE "Error %d adding scull%d",err,index);
}

void scull_cleanup_module(void)
{
    int i ;
    dev_t devno = MKDEV(scull_major,scull_minor);
    
    /* Get rid of our char dev entries */
    if(scull_devices) {
        for(i = 0;i < scull_nr_devs; i++) {
            scull_trim(scull_devices + i);
            cdev_del(&scull_devices[i].cdev);   
        }
        kfree(scull_devices);
    }

    kmem_cache_destroy(scullc_cache);
    unregister_chrdev_region(devno,scull_nr_devs);
}


int scull_init_module(void)
{
    dev_t dev = 0;
    int result,i;
    
    scullc_cache = kmem_cache_create("scullc",sizeof(struct scull_qset),0,0,NULL);
    if(!scullc_cache) {
        printk(KERN_WARNING "scull: cant't alloc kmem for scullc\n");
        return -ENOMEM;
    }
    
    if(scull_major) {
        dev = MKDEV(scull_major,scull_minor);
        result = register_chrdev_region(dev,scull_nr_devs,"select");
    } else {
        result = alloc_chrdev_region(&dev,scull_minor,\
                                     scull_nr_devs,"select");
        scull_major = MAJOR(dev);
    }
    
    if(result < 0) {
        printk(KERN_WARNING "scull: can't get major %d\n",scull_major);
        return result;
    }
    
    /*
     * allocate the devices -- we can't have them static,
     * as the number can be specified at load time.
     */

    scull_devices  = kmalloc(scull_nr_devs * sizeof(struct scull_dev),GFP_KERNEL);
    if(!scull_devices) {
        result = -ENOMEM;
        goto fail;
    }
    
    memset(scull_devices,0,scull_nr_devs * sizeof(struct scull_dev));

    for(i = 0;i < scull_nr_devs;++i) {
        scull_devices[i].size = 0;
        INIT_LIST_HEAD(&scull_devices[i].list);
        init_waitqueue_head(&scull_devices[i].inq);
        init_waitqueue_head(&scull_devices[i].outq);
        sema_init(&scull_devices[i].sem,1);
        atomic_set(&scull_devices[i].len,0);
        scull_setup_cdev(&scull_devices[i],i);
    }
    return 0;
 fail:
    scull_cleanup_module();
    return result;
}


module_init(scull_init_module);
module_exit(scull_cleanup_module);
