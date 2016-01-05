#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

struct timespec *dit;
struct timespec *dah;
struct timespec *space;
int fd;

void performDit(void)
{
	write (fd, "1", 1);
	nanosleep(dit,NULL);
    
	write (fd, "0", 1);
	nanosleep(space,NULL);
}

void performDah(void)
{
	write (fd, "1", 1);
	nanosleep(dah,NULL);
		
	write (fd, "0", 1);
	nanosleep(space,NULL);
}

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

int main(int argc, char* argv[])
{
	dit = malloc(sizeof(struct timespec));
	dit->tv_sec = 0;
	dit->tv_nsec = 100000000;
	
	dah = malloc(sizeof(struct timespec));
	dah->tv_sec = 0;
	dah->tv_nsec = 300000000;
	
	space = malloc(sizeof(struct timespec));
	space->tv_sec = 0;
	space->tv_nsec = 100000000;
  
  
  
  fd = open("/sys/class/gpio/export", O_WRONLY);
  write (fd, "3", 1);
  close(fd);
  
  fd = open("/sys/class/gpio/gpio3/direction", O_WRONLY);
  write (fd, "out", 3);
  close(fd);
  
  fd = open("/sys/class/gpio/gpio3/value", O_WRONLY);
  
  write (fd, "0", 1);
  
	int i;
	int j = 0;

    struct timeval start, finish;
    gettimeofday(&start, NULL);
	for(i=1;i<argc;i++)
	{
		char *a = argv[i];
		while(a[j]!='\0')
		{
			
			switchMorse(a[j]);
			j++;
		}
		j=0;
	}
    gettimeofday(&finish, NULL);

    unsigned long timer = ((finish.tv_sec - start.tv_sec)*1000000L + finish.tv_usec) - start.tv_usec;
    printf("Completed in %ul microseconds\n", timer);
	free(dit);
	free(dah);
	free(space);
	close(fd);
 }
