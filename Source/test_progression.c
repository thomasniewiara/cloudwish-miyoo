#define main game_main
#include "cloudwish.c"
#undef main
int main(void){
 char tmp[]="/tmp/cloudwish-flow-XXXXXX";assert(mkdtemp(tmp));assert(chdir(tmp)==0);
 assert(SDL_Init(SDL_INIT_TIMER)==0);
 for(int step=0;step<7;step++){
  g.step=step;g.x=tx[step];g.y=ty[step];puzzle=0;
  if(needed[step]>=0){g.who=(needed[step]+1)%4;action();assert(g.step==step&&!puzzle);g.who=needed[step];}
  action();
  if(needed[step]>=0){
   assert(puzzle&&linked==0);selected=2;key(SDLK_SPACE);assert(linked==0);
   selected=0;key(SDLK_SPACE);assert(linked==1);key(SDLK_LCTRL);assert(linked==0);
   for(int j=0;j<4;j++)key(SDLK_SPACE);
  }
  assert(g.step==step+1&&!puzzle);Save r;assert(read_save("saves/slot1.txt",&r)&&r.step==g.step);
 }
 action();assert(g.step==7);paused=1;menu=2;key(SDLK_SPACE);assert(!running);
 remove("saves/slot1.txt");rmdir("saves");assert(chdir("/tmp")==0);rmdir(tmp);
 SDL_Quit();puts("PASS: all 7 mission steps, creature gating, wrong point, undo, autosave, completion, quit");return 0;
}
