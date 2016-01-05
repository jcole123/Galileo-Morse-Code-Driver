#include <linux/fcntl.h>
#include <linux/unistd.h>
#include <linux/time.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <asm/uaccess.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/fs.h>
#include <asm/segment.h>
#include <linux/unistd.h>
#include <linux/buffer_head.h>
#include <linux/delay.h>


/* Sets the function calls for the proc file that is created. */

struct file *fd;

/*
 * Open a file
 */
struct file* file_open(const char* path, int flags, int rights) {
    struct file* filp = NULL;
    mm_segment_t oldfs;
    int err = 0;

    oldfs = get_fs();
    set_fs(get_ds());
    filp = filp_open(path, flags, rights);
    set_fs(oldfs);
    if(IS_ERR(filp)) {
        err = PTR_ERR(filp);
        return NULL;
    }
    return filp;
}

/*
 * Close a file
 */
void file_close(struct file* file) {
    filp_close(file, NULL);
}

/*
 * Write to a file
 */
int file_write(struct file* file, unsigned long long offset, unsigned char* data, unsigned int size) {
    mm_segment_t oldfs;
    int ret;

    oldfs = get_fs();
    set_fs(get_ds());

    ret = vfs_write(file, data, size, &offset);

    set_fs(oldfs);
    return ret;
}

/*
 * Perform quick blink on LED - "dit"
 */
void performDit(void)
{
	file_write(fd, 0, "1", 1);
	msleep(100);
    
	file_write(fd, 0, "0", 1);
	msleep(100);
}

/*
 * Longer blink on LED - "dah"
 */
void performDah(void)
{
	file_write(fd, 0, "1", 1);
	msleep(300);
    
	file_write(fd, 0, "0", 1);
	msleep(100);
}

/*
 * Switch-case to perform morse code translation and output the Galileo's LED
 */
void switchMorse(char a)
{
	switch(a){
  	case 'a'  :
		performDit();
		performDah();
		break; 
	case 'b'  :
		performDah();
		performDit();
		performDit();
		performDit();
		break; 
	case 'c'  :
		performDah();
		performDit();
		performDah();
		performDit();
		break;
	case 'd'  :
		performDah();
		performDit();
		performDit();
		break;
	case 'e'  :
		performDit();
		break;
	case 'f'  :
		performDit();
		performDit();
		performDah();
		performDit();
		break;
	case 'g'  :
		performDah();
		performDah();
		performDit();
		break;
	case 'h'  :
		performDit();
		performDit();
		performDit();
		performDit();
		break;
	case 'i'  :
		performDit();
		performDit();
		break;
	case 'j'  :
		performDit();
		performDah();
		performDah();
		performDah();
		break;
	case 'k'  :
		performDah();
		performDit();
		performDah();
		break;
	case 'l'  :
		performDit();
		performDah();
		performDit();
		performDit();
		break;
	case 'm'  :
		performDah();
		performDah();
		break;
	case 'n'  :
		performDah();
		performDit();
		break;
	case 'o'  :
		performDah();
		performDah();
		performDah();
		break;
	case 'p'  :
		performDit();
		performDah();
		performDah();
		performDit();
		break;
	case 'q'  :
		performDah();
		performDah();
		performDit();
		performDah();
		break;
	case 'r'  :
		performDit();
		performDah();
		performDit();
		break;
	case 's'  :
		performDit();
		performDit();
		performDit();
		break;
	case 't'  :
		performDah();
		break;
	case 'u'  :
		performDit();
		performDit();
		performDah();
		break;
	case 'v'  :
		performDit();
		performDit();
		performDit();
		performDah();
		break;
	case 'w'  :
		performDit();
		performDah();
		performDah();
		break;
	case 'x'  :
		performDah();
		performDit();
		performDit();
		performDah();
		break;
	case 'y'  :
		performDah();
		performDit();
		performDah();
		performDah();
		break;
	case 'z'  :
		performDah();
		performDah();
		performDit();
		performDit();
		break;
  	case 'A'  :
		performDit();
		performDah();
		break; 
	case 'B'  :
		performDah();
		performDit();
		performDit();
		performDit();
		break; 
	case 'C'  :
		performDah();
		performDit();
		performDah();
		performDit();
		break;
	case 'D'  :
		performDah();
		performDit();
		performDit();
		break;
	case 'E'  :
		performDit();
		break;
	case 'F'  :
		performDit();
		performDit();
		performDah();
		performDit();
		break;
	case 'G'  :
		performDah();
		performDah();
		performDit();
		break;
	case 'H'  :
		performDit();
		performDit();
		performDit();
		performDit();
		break;
	case 'I'  :
		performDit();
		performDit();
		break;
	case 'J'  :
		performDit();
		performDah();
		performDah();
		performDah();
		break;
	case 'K'  :
		performDah();
		performDit();
		performDah();
		break;
	case 'L'  :
		performDit();
		performDah();
		performDit();
		performDit();
		break;
	case 'M'  :
		performDah();
		performDah();
		break;
	case 'N'  :
		performDah();
		performDit();
		break;
	case 'O'  :
		performDah();
		performDah();
		performDah();
		break;
	case 'P'  :
		performDit();
		performDah();
		performDah();
		performDit();
		break;
	case 'Q'  :
		performDah();
		performDah();
		performDit();
		performDah();
		break;
	case 'R'  :
		performDit();
		performDah();
		performDit();
		break;
	case 'S'  :
		performDit();
		performDit();
		performDit();
		break;
	case 'T'  :
		performDah();
		break;
	case 'U'  :
		performDit();
		performDit();
		performDah();
		break;
	case 'V'  :
		performDit();
		performDit();
		performDit();
		performDah();
		break;
	case 'W'  :
		performDit();
		performDah();
		performDah();
		break;
	case 'X'  :
		performDah();
		performDit();
		performDit();
		performDah();
		break;
	case 'Y'  :
		performDah();
		performDit();
		performDah();
		performDah();
		break;
	case 'Z'  :
		performDah();
		performDah();
		performDit();
		performDit();
		break;
	case '1'  :
		performDit();
		performDah();
		performDah();
		performDah();
		performDah();
		break;
	case '2'  :
		performDit();
		performDit();
		performDah();
		performDah();
		performDah();
		break;
	case '3'  :
		performDit();
		performDit();
		performDit();
		performDah();
		performDah();
		break;
	case '4'  :
		performDit();
		performDit();
		performDit();
		performDit();
		performDah();
		break;
	case '5'  :
		performDit();
		performDit();
		performDit();
		performDit();
		performDit();
		break;
	case '6'  :
		performDah();
		performDit();
		performDit();
		performDit();
		performDit();
		break;
	case '7'  :
		performDah();
		performDah();
		performDit();
		performDit(); 
		performDit();
		break;
	case '8'  :
		performDah();
		performDah();
		performDah();
		performDit();
		performDit();
		break;
	case '9'  :
		performDah();
		performDah();
		performDah();
		performDah();
		performDit();
		break;
	case '0'  :
		performDah();
		performDah();
		performDah();
		performDah();
		performDah();
		break;
	default  :
		break;
	}
		/* you can have any number of case statements */
     
}

/*
 * Read in string to be translated and un switchMorse on it
 */
int process_message(struct file *filp,const char *buf,size_t count,loff_t *offp)
{
    struct timespec start, finish;
    getnstimeofday(&start);
	int length = strlen(buf);

	int i;
	//int j = 0;
	
	
	for(i=0;i<length;i++)
	{
		
		switchMorse(buf[i]);
	}
    getnstimeofday(&finish);
    unsigned long timer =  ((finish.tv_sec - start.tv_sec)*1000000000L + finish.tv_nsec) - start.tv_nsec;
    timer = timer/1000;
    printk(KERN_INFO "Time to complete (in microseconds): %ul\n", timer);

    return count;
} 

struct file_operations proc_fops = {
	write: process_message
};

void create_new_proc_entry(void) 
{

	proc_create("kernelToMorse",0,NULL,&proc_fops);

  	fd = kmalloc(GFP_KERNEL, sizeof(struct file));

    //Export pin 3
  	fd = file_open("/sys/class/gpio/export", O_WRONLY, 0);
	file_write(fd, 0, "3", 1);
	file_close(fd);

	//Set Direction
	fd = file_open("/sys/class/gpio/gpio3/direction", O_WRONLY, 0);
	file_write(fd, 0, "out", 3);
	file_close(fd);

    //Update value
	fd = file_open("/sys/class/gpio/gpio3/value", O_WRONLY, 0);
    file_write(fd, 0, "0", 1);
	
	
}



 int proc_init (void) {
	create_new_proc_entry();
	return 0;
}

void proc_cleanup(void) {
	file_close(fd);
	kfree(fd);
	remove_proc_entry("kernelToMorse",NULL);
}

MODULE_LICENSE("GPL"); 
module_init(proc_init);
module_exit(proc_cleanup);