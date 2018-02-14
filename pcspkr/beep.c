#include <stdio.h>
#include <stdlib.h> // exit()
#include <unistd.h> //close()
#include <fcntl.h>  // open()
#include <sys/ioctl.h> //ioctl()
#include <linux/kd.h>  // KIOCSOUND
/*
 * the number comes from beep.c on http://www.johnath.com/beep/beep.c
 */
#define CLOCK_TICK_RATE 1193180
int scale_high[]={880,988,1109,1174,1318,1480,1661};
int scale[]={440,494,554,587,658,739,830};
int scale_low[]={220,247,277,293,329,370,415};


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
  for (int i=0;i<7;i++){
	play_beep(scale_low[i],100);
	usleep(500000);
  }
  for (int i=0;i<7;i++){
	play_beep(scale[i],100);
	usleep(500000);
  }
  for (int i=0;i<7;i++){
	play_beep(scale_high[i],100);
	usleep(500000);
  }
  
}
