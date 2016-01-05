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
#include <linux/buffer_head.h>
#include <linux/delay.h>
#include <linux/gpio.h>

int button_one, button_two;
int tempIndex;
struct timespec start, finish;
unsigned long timer;

int charIndex = 1;
int letterCount = 0;
int indexMultiplier = 2;

volatile int done;

//size of buffer
int curr_Index;

/*
 * Buffer to hold the result of the translation
 */
char *resultBuff;

void sendLetter(int index);
void performTest(void);
void switchMorse(char);
int process_message(struct file *filp,const char *buf,size_t count,loff_t *offp);
int print_message(struct file *filp,char *buf,size_t count,loff_t *offp);
void translateMorse(void);
static char switchCase(int);
void checkTimer(unsigned long, int);
void translate(char);
void create_new_proc_entry(void);
int proc_init(void);
void proc_cleanup(void);

/*
 * Helper function for the testing function. Translates a letter and then resets
 * the necessary variables to translate the next letter.
 */
void sendLetter(int index) {
  char temp;
  temp = switchCase(letterCount);
  resultBuff[index] = temp;
  letterCount = 0;
  charIndex = 1;
  indexMultiplier = 2;
}


/*
 * This is the testing function to test the performance of the program.
 * Translates the word TESTING 20 times and outputs how much time it took.
 */
void performTest() {
  // Translate TESTING 20 times
  // TESTING in morse: dah/dit/dit-dit-dit/dah/dit-dit/dah-dit/dah-dah-dit
  int i;
  resultBuff = krealloc(resultBuff, sizeof(char)*7, GFP_KERNEL);
  for (i = 0; i < 20; i++) {
    // Reset the variables to necessary values to restart the translation of TESTING
    // malloc resultBuff to be the exact size of 
    letterCount = 0;
    charIndex = 1;
    indexMultiplier = 2;
    // Translate T
    translate('a');
    sendLetter(0);
    // Translate E
    translate('i');
    sendLetter(1);
    // Translate S
    translate('i');
    translate('i');
    translate('i');
    sendLetter(2);
    // Translate T
    translate('a');
    sendLetter(3);
    // Translate I
    translate('i');
    translate('i');
    sendLetter(4);
    // Translate N
    translate('a');
    translate('i');
    sendLetter(5);
    // Translate G
    translate('a');
    translate('a');
    translate('i');
    sendLetter(6);
    // Set the resultBuff to 0
  }
}

/* Called when writing to the proc file. Reads in the message from the user (via a text file or 
   the echo command) and stores it into a buffer */
int process_message(struct file *filp,const char *buf,size_t count,loff_t *offp)
{
    charIndex = 1;
    letterCount = 0;
    indexMultiplier = 2;
    if (buf[0] == '-') {
        getnstimeofday(&start);
	performTest();
        getnstimeofday(&finish);
        timer =  ((finish.tv_sec - start.tv_sec)*1000000000L + finish.tv_nsec) - start.tv_nsec;
	timer = timer/1000;
	printk(KERN_INFO "Translating the morse version of TESTING 20 times took %ld microseconds\n", timer);
    } else {
        translateMorse();
        tempIndex = curr_Index;
    }
    return count;
}

struct file_operations proc_fops = {
    write: process_message
};

/* Sets up the initial proc entry to be interacted with after
   inserting the module */
void create_new_proc_entry(void)
{
    proc_create("kernelFromMorse",0,NULL,&proc_fops);

    resultBuff = kmalloc(GFP_KERNEL, sizeof(char));

    button_one = gpio_request(17, "input button");
    gpio_direction_input(17);
    button_two = gpio_request(18, "break button");
    gpio_direction_input(18);
   
}

/* Overall translate function that polls the two buttons for input.
   Button one is used to actually send morse signals (dits and dahs are determined
   based on how long the button is held down). Button two is used denote the end of a character, word, or
   the end of translation */
void translateMorse(void) {
    curr_Index = 0;
    done = 0;
    curr_Index = 0;
    resultBuff = krealloc(resultBuff, sizeof(char)*1024, GFP_KERNEL);
    while(!done) {
	/* Check to see if the first button is pressed. If not, check the second button
           for input. */
        button_one = gpio_get_value_cansleep(17);
        if(button_one == 0) {
	    /* Determine how long the button is pressed */
            getnstimeofday(&start);
            while(button_one == 0) {
                button_one = gpio_get_value_cansleep(17);
            }
            getnstimeofday(&finish);
            timer =  ((finish.tv_sec - start.tv_sec)*1000000000L + finish.tv_nsec) - start.tv_nsec;
	    timer = timer/1000;
            checkTimer(timer, 1);
        }
        else {
            button_two = gpio_get_value_cansleep(18);
            if(button_two == 0) {
                getnstimeofday(&start);
                while(button_two == 0) {
                    button_two = gpio_get_value_cansleep(18);
                }
                getnstimeofday(&finish);
                timer =  ((finish.tv_sec - start.tv_sec)*1000000000L + finish.tv_nsec) - start.tv_nsec;
		timer = timer/1000;
                checkTimer(timer, 2);
            }
        }
    }
}


/* This function performs the switch to determine which ASCII character to
   output. Works off a hashing algorithm that creates a unique hashcode
   of each string of dits and dahs. Look at the report for
   algorithm documentation. */
static char switchCase(int v)
{
    switch(v)
    {
        case 2150 :
            return 'A';
            break;
        case 18884 :
            return 'B';
            break;
        case 18452 :
            return 'C';
            break;
        case 7964 :
            return 'D';
            break;
        case 210 :
            return 'E';
            break;
        case 18468 :
            return 'F';
            break;
        case 7804 :
            return 'G';
            break;
        case 18900 :
            return 'H';
            break;
        case 2310 :
            return 'I';
            break;
        case 17476 :
            return 'J';
            break;
        case 7532 :
            return 'K';
            break;
        case 18740 :
            return 'L';
            break;
        case 2134 :
            return 'M';
            break;
        case 2294 :
            return 'N';
            break;
        case 7372 :
            return 'O';
            break;
        case 18308 :
            return 'P';
            break;
        case 17892 :
            return 'Q';
            break;
        case 7820 :
            return 'R';
            break;
        case 7980 :
            return 'S';
            break;
        case 194 :
            return 'T';
            break;
        case 7548 :
            return 'U';
            break;
        case 18068 :
            return 'V';
            break;
        case 7388 :
            return 'W';
            break;
        case 18052 :
            return 'X';
            break;
        case 17620 :
            return 'Y';
            break;
        case 18724 :
            return 'Z';
            break;
        case 33950 :
            return '0';
            break;
        case 33966 :
            return '1';
            break;
        case 34126 :
            return '2';
            break;
        case 34558 :
            return '3';
            break;
        case 35390 :
            return '4';
            break;
        case 36750 :
            return '5';
            break;
        case 36734 :
            return '6';
            break;
        case 36574 :
            return '7';
            break;
        case 36142 :
            return '8';
            break;
        case 35310 :
            return '9';
            break;
        default :
            return '~';
            break;
    }
}

/*
 * Perform translate logic based on which button has been pressed and for how long it was pressed
 * timer: length of time button was held down
 * button: used to denote which button has been pressed - either 1 or 2
 */
void checkTimer(unsigned long timer, int button) {

    if(button==1) {
        if (timer>500000) {
            translate('a');
        }
        else {
            translate('i');
        }
    }
    else {
        //Translation complete
        if (timer >1500000) {
            resultBuff[curr_Index] =  '\0';
            resultBuff = krealloc(resultBuff, 1, GFP_KERNEL);
            done = 1;
            printk(KERN_INFO "Result: %s\n", resultBuff);
        }
        //end of character
        else if (timer>500000) {
            char temp = switchCase(letterCount);
            resultBuff[curr_Index++] =  temp;
            letterCount = 0;
            charIndex = 1;
            indexMultiplier=2;
            resultBuff[curr_Index++] =  ' ';
        }
	//end of word
        else {
            char temp = switchCase(letterCount);
	    letterCount = 0;
            charIndex = 1;
            indexMultiplier=2;
            resultBuff[curr_Index++] =  temp;
        }
    }
}


/*
 * This translates each dit and dah to modify the hashed value
 * to make sure when the switchCase is called, the proper letter is outputted.
 */
void translate(char currentChar)
{
    /* charIndex = current position within a character (string of dit-dah's)
       letterCount = current value of hashing algorithm within a character
       indexMultipler = used in hashing algorithm */

    // Allocate the buffer to hold translated message
    // Loop through the entire text file, translating after every string of dit-dah's.
    if (currentChar=='a')
    {
        letterCount += 97*charIndex*indexMultiplier;
        indexMultiplier+=2;

    }
    else
    {
        letterCount += 105*charIndex*indexMultiplier;
        indexMultiplier+=2;
    }
    charIndex+=4;

}

/* Called when inserting the module. Sets everything up */
int proc_init (void) {
	create_new_proc_entry();
	return 0;
}

/* Called whene removing the module. */
void proc_cleanup(void) {
    gpio_free(17);
    gpio_free(18);
    kfree(resultBuff);
    remove_proc_entry("kernelFromMorse",NULL);
}

MODULE_LICENSE("GPL"); 
module_init(proc_init);
module_exit(proc_cleanup);
