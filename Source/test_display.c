#define main game_main
#include "cloudwish.c"
#undef main
/* Run from the game's asset directory with SDL_VIDEODRIVER=dummy. */
int main(void){
 char *args[]={"cloudwish","--screenshot","/tmp/cloudwish-display-test.bmp",NULL};
 assert(game_main(3,args)==0);
 assert(screen!=video&&screen->format->BitsPerPixel==32&&video->format->BitsPerPixel==32);
 for(int phase=0;phase<5;phase++){
  g.step=phase==0?0:phase==1?3:1;puzzle=phase==2;paused=phase==3;shop=phase==4;render();
  assert(SDL_LockSurface(screen)==0);assert(SDL_LockSurface(video)==0);
  for(int y=0;y<480;y++)for(int x=0;x<640;x++){
   Uint32 a,b;Uint8 ar,ag,ab,br,bg,bb;
   memcpy(&a,(Uint8*)screen->pixels+y*screen->pitch+x*4,4);
   #ifdef MIYOO_ROTATE_180
   memcpy(&b,(Uint8*)video->pixels+(479-y)*video->pitch+(639-x)*4,4);
#else
   memcpy(&b,(Uint8*)video->pixels+y*video->pitch+x*4,4);
#endif
   SDL_GetRGB(a,screen->format,&ar,&ag,&ab);SDL_GetRGB(b,video->format,&br,&bg,&bb);
   assert(ar==br&&ag==bg&&ab==bb);
  }
  SDL_UnlockSurface(video);SDL_UnlockSurface(screen);
 }
 remove("/tmp/cloudwish-display-test.bmp");
 puts("PASS: separate 32-bit canvas, every presented pixel (including configured rotation) for meadow, bridge, puzzle and pause");
 return 0;
}
