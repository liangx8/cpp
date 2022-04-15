#include <SDL2/SDL.h>
#include <stdio.h>

int main(int argc,char **argv)
{
  if(SDL_Init(SDL_INIT_VIDEO)){
    printf("SDL init error %s\n",SDL_GetError());
    return 1;
  } 
  SDL_Window *win = SDL_CreateWindow("SDL学习", 100, 100, 640, 480, SDL_WINDOW_SHOWN);
  if (win == NULL){
    printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
    SDL_Quit();
    return 1;
  }
  SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (ren == NULL){
    SDL_DestroyWindow(win);
    printf("SDL_CreateRenderer Error: %s\n",SDL_GetError());
    SDL_Quit();
    return 1;
  }
  SDL_Surface *surf;
  surf=SDL_CreateRGBSurface(0,96,96,32,0,0,0,0);
  SDL_FillRect(surf,NULL,SDL_MapRGB(surf->format,255,0,0));
  SDL_Texture *tex = SDL_CreateTextureFromSurface(ren,surf);
  SDL_FreeSurface(surf);
  if(tex == NULL){
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    printf("SDL_CreateTextureFromSurface error:%s",SDL_GetError());
    return 1;
  }
  for(int i=0;i<3;++i){
    SDL_RenderClear(ren);
    SDL_RenderCopy(ren,tex,NULL,NULL);
    SDL_RenderPresent(ren);
    SDL_Delay(1000);
    printf("second %d\n",i);
  }
  SDL_DestroyTexture(tex);
  SDL_DestroyRenderer(ren);
  SDL_DestroyWindow(win);
  SDL_Quit();
  return 0;
}
