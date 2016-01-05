#include <unistd.h>
#include <sys/inotify.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h> 


void setup();

void translate(char currentChar);
static char switchCase(int v);
char readGPIO(int button);
void checkTimer(unsigned long timer, int button);
void performTest();
void sendLetter(int index);

int messageIndex = 1;
int charIndex = 1;
int letterCount = 0;
int indexMultiplier = 2;
char currentChar;
int done = 0;
int resultIndex = 0;

//size of buffer
int current;
/*
 * Buffer to hold the result of the translation
 */
char *resultBuff;


int file;

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
  char temp;
  int i;
  resultBuff = malloc(sizeof(char)*7);
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
  free(resultBuff);
}

int main(int argc, char* argv[]) {
	//export pin and set direction
	setup();
    current = 0;
	char button_one, button_two;
	struct timeval start, finish;
	int input;
	unsigned long timer;
    //
    if (argc > 1) {
      /* If a command line argument is passed in, perform the test to
         avoid time due to user interacting with button */
      gettimeofday(&start, NULL);
      performTest();
      gettimeofday(&finish, NULL);
        timer = ((finish.tv_sec - start.tv_sec)*1000000L + finish.tv_usec) - start.tv_usec;
      printf("Translating the morse version of TESTING 20 times took %ld microseconds\n", timer);
    } else {
      resultBuff = malloc(sizeof(char) *  1024);
      while(1) {
		button_one = readGPIO(1);
		if(button_one == '0') {
          gettimeofday(&start, NULL);
          while(button_one == '0') {
            button_one = readGPIO(1);
          }
          gettimeofday(&finish, NULL);
          timer = ((finish.tv_sec - start.tv_sec)*1000000L + finish.tv_usec) - start.tv_usec;
          checkTimer(timer, 1);
		}
        else {
          button_two = readGPIO(2);
          if(button_two == '0') {
            gettimeofday(&start, NULL);
            while(button_two == '0') {
              button_two = readGPIO(2);
            }
            gettimeofday(&finish, NULL);
            timer = ((finish.tv_sec - start.tv_sec)*1000000L + finish.tv_usec) - start.tv_usec;
            checkTimer(timer, 2);
          }
        }
		
      }
    }
	
}

/*
 * Export buttons and set their direction to "in"
 */
void setup() {
    /*
     * Button One
     */
	file = open("/sys/class/gpio/export", O_WRONLY);
	//export pin 17
	write(file, "17", 2);
	close(file);

	file = open("/sys/class/gpio/gpio17/direction", O_WRONLY);
	//set pin 17 direction to "in"
	write(file, "in", 2);
	close(file);

    /*
     * Button Two
     */
    file = open("/sys/class/gpio/export", O_WRONLY);
    //export pin
    write(file, "18", 2);
    close(file);

    file = open("/sys/class/gpio/gpio18/direction", O_WRONLY);
    //set pin direction to "in"
    write(file, "in", 2);
    close(file);
}

/*
 * Read button value
 * button: either 1 or 2, depending on which button will be read
 */
char readGPIO(int button) {
    FILE *toRead;
    char contents;
    if(button == 1) {
        toRead = popen("cat /sys/class/gpio/gpio17/value", "r");
        fread(&contents, 1, 1, toRead);
        pclose(toRead);
    }
    else {
        toRead = popen("cat /sys/class/gpio/gpio18/value", "r");
        fread(&contents, 1, 1, toRead);
        pclose(toRead);
    }
    return contents;

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
            //char temp = switchCase(letterCount);
            //resultBuff[current++] =  temp;
            resultBuff[current] =  '\0';
			printf("%s\n", resultBuff);
			free(resultBuff);
			exit(0);
		}
		//end of character
		else if (timer>500000) {
			char temp = switchCase(letterCount);
            resultBuff[current++] =  temp;
	        letterCount = 0;
	        charIndex = 1;
	        indexMultiplier=2;
            resultBuff[current++] =  ' ';
   		 }	
		else {
            char temp = switchCase(letterCount);
            resultBuff[current++] =  temp;
            letterCount = 0;
            charIndex = 1;
            indexMultiplier=2;
			//end of word
			printf("%s\n", resultBuff);
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
 * This translates each dit and dah to modify the hashed value
 * to make sure when the switchCase is called, the proper letter is outputted.
 */
void translate(char currentChar)
{
	/* messageIndex = current position in overall text file
	   charIndex = current position within a character (string of dit-dah's)
	   letterCount = current value of hashing algorithm within a character
	   indexMultipler = used in hashing algorithm
	   resultIndex = position within translated ASCII char buffer */

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







