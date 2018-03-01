/*
 * manipulate pc speaker
 */
#include <stdio.h>
#include <stdlib.h> // exit()
#include <unistd.h> //close()
#include <fcntl.h>  // open()
#include <sys/ioctl.h> //ioctl()
#include <linux/kd.h>  // KIOCSOUND

#define BEAT 500
#define HALF_BEAT 500 / 2

/*
 * the number comes from beep.c on http://www.johnath.com/beep/beep.c
 */
#define CLOCK_TICK_RATE 1193180
int scale[3][7]={
  {220,247,277 ,293 ,329 ,370 ,415},
  {440,494,554 ,587 ,658 ,739 ,830},
  {880,988,1109,1174,1318,1480,1661}
};

int birthday[]={
  0,4,HALF_BEAT,  0,4,HALF_BEAT, 0,5,BEAT, 0,4,BEAT, 1,0,BEAT,0,6,BEAT * 2,
  0,4,HALF_BEAT,  0,4,HALF_BEAT, 0,5,BEAT, 0,4,BEAT, 1,1,BEAT,1,0,BEAT * 2,
  0,4,HALF_BEAT,  0,4,HALF_BEAT, 1,4,BEAT, 1,3,BEAT, 1,0,BEAT,0,6,BEAT,0,5,BEAT,
  1,3,BEAT * 3 / 4,1,3,BEAT / 4, 1,2, BEAT,1,0,BEAT, 1,1,BEAT,1,0,BEAT * 2,
  0,0,0
};

void play_beep(int feq,int length)
{
  int fd;
  if((fd=open("/dev/console", O_WRONLY)) == -1){
	fprintf(stderr,"Could not open /dev/console for writing\n");
	exit(1);
  }
  ioctl(fd,KIOCSOUND, CLOCK_TICK_RATE/feq);
  usleep(1000*length);
  ioctl(fd,KIOCSOUND, 0);
  close(fd);
}
int main(int argc,char **argv)
{
  printf("PC speaker beep\n");
  int idx=0;
  while(1){
	int s;
	if( birthday[idx + 2] == 0) break;
	s=scale[birthday[idx]][birthday[idx+1]];
	play_beep(s,birthday[idx+2]);
	usleep(1000 * HALF_BEAT / 4);
	idx += 3;
  }
}
