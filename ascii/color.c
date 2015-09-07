#include <stdio.h>

int main(void){
  for(int x=0;x<8;x++){
	for(int y=0;y<8;y++){
	  printf("\033[1;3%d;4%dm1;3%d;4%d\033[0m ",x,y,x,y);

	}
	printf("    ");
	for(int y=0;y<8;y++){
	  printf("\033[0;3%d;4%dm0;3%d;4%d\033[0m ",x,y,x,y);
	}
	printf("\n");
  }

}

