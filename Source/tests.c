#include "game.h"
#include <assert.h>
extern int pattern_value(int round,int index);
static Uint8 keys[SDLK_LAST];
static void solve(void){
 int who=spell.type;
 if(who==0){int count=spell.count;spell.cursor=(spell.path[count]+1)%9;spell_key(SDLK_SPACE);assert(spell.count==count);spell.cursor=spell.path[0];spell_key(SDLK_SPACE);spell_key(SDLK_LCTRL);assert(spell.count==0);
  for(int i=0;i<6;i++){spell_key(SDLK_LALT);spell_key(SDLK_SPACE);}
 }else if(who==1){for(int round=0;round<3;round++){spell.cursor=(pattern_value(round,5)+1)%4;spell_key(SDLK_SPACE);assert(spell.round==round);spell_key(SDLK_LALT);spell_key(SDLK_SPACE);}}
 else if(who==2){spell_key(SDLK_SPACE);tick(.05f,keys);assert(spell.count==0);for(int i=0;i<3;i++){spell_key(SDLK_SPACE);keys[SDLK_SPACE]=1;for(int j=0;j<28;j++)tick(.05f,keys);keys[SDLK_SPACE]=0;tick(.01f,keys);}}
 else {spell_key(SDLK_SPACE);assert(spell.count==0);for(int i=0;i<80;i++)tick(.05f,keys);for(int i=0;i<4;i++){spell.cursor=(spell.path[i]+1)%4;spell_key(SDLK_SPACE);assert(spell.count==i);spell.cursor=spell.path[i];spell_key(SDLK_SPACE);}}
 assert(mode==PLAY);
}
static void pixel_test(void){
 assert(SDL_LockSurface(canvas)==0);assert(SDL_LockSurface(video)==0);
 for(int y=0;y<H;y++)for(int x=0;x<W;x++){Uint32 a,b;Uint8 ar,ag,ab,br,bg,bb;
  memcpy(&a,(Uint8*)canvas->pixels+y*canvas->pitch+x*4,4);
#ifdef MIYOO_ROTATE_180
  memcpy(&b,(Uint8*)video->pixels+(H-1-y)*video->pitch+(W-1-x)*4,4);
#else
  memcpy(&b,(Uint8*)video->pixels+y*video->pitch+x*4,4);
#endif
  SDL_GetRGB(a,canvas->format,&ar,&ag,&ab);SDL_GetRGB(b,video->format,&br,&bg,&bb);assert(ar==br&&ag==bg&&ab==bb);
 }
 SDL_UnlockSurface(video);SDL_UnlockSurface(canvas);
}
static void snap(const char*dir,const char*name){render();pixel_test();if(dir){char path[512];snprintf(path,sizeof path,"%s/%s.bmp",dir,name);assert(SDL_SaveBMP(canvas,path)==0);}}
int main(int argc,char**argv){
 defaults(&g);assert(init_video());char temp[]="/tmp/cloudwish-test-XXXXXX";assert(mkdtemp(temp));assert(chdir(temp)==0);
 assert(!can_move(320,260)&&!can_move(20,200));g.progress[0]=2;assert(can_move(320,260)&&!can_move(250,260));g.progress[0]=0;
 assert(!unlocked(1)&&!unlocked(4)&&unlocked(5));assert(!travel(4));assert(g.island==0);
 /* All 23 missions through their creature gate and real puzzle handlers. */
 int total=0;
 for(int island=0;island<5;island++){
  assert(travel(island));for(int stage=0;stage<caps[island];stage++){
   const Quest*q=quest();assert(q);g.x=q->x;g.y=q->y;
   if(q->who>=0){g.who=(q->who+1)%4;interact();assert(mode==PLAY&&g.progress[island]==stage);g.who=q->who;}
   interact();if(q->who>=0){assert(mode==PUZZLE);solve();}
   assert(g.progress[island]==stage+1);total+=island?15:10;assert(g.stars==total);
   Save check;assert(read_save(SAVE_PATH,&check)&&check.progress[island]==stage+1&&check.stars==total);
  }complete_quest();assert(g.stars==total);
 }
 assert(total==310);puts("PASS: 23 missions, 4 distinct puzzles, gates, mistakes/undo, unlocks, rewards and autosaves");
 for(int i=1;i<LOOKS;i++){assert(buy_look(i));int stars=g.stars;assert(buy_look(i)&&g.stars==stars);}
 for(int i=0;i<6;i++){assert(buy_furniture(i));assert(place_furniture(i,i+1));}
 assert(g.stars==35);assert(place_furniture(1,1)&&g.slots[0]==0&&g.slots[1]==1);assert(place_furniture(0,2));
 for(int i=0;i<4;i++){g.who=i;assert(buy_look(i+1));}
 assert(travel(5));Save roundtrip;assert(read_save(SAVE_PATH,&roundtrip));assert(roundtrip.island==5&&roundtrip.stars==35&&roundtrip.look[3]==4&&roundtrip.slots[0]==2);
 g.stars=0;g.owned=0;memset(g.look,0,sizeof g.look);assert(!buy_look(7));g.furniture=0;memset(g.slots,0,sizeof g.slots);assert(!buy_furniture(3)&&!place_furniture(0,3));
 g=roundtrip;
 /* Exact legacy-save import, with no duplicated credit on the new format. */
 FILE*f=fopen("old.txt","w");fputs("CW1 7 3 320 355\n",f);fclose(f);Save old;assert(read_save("old.txt",&old)&&old.stars==70);
 f=fopen("old.txt","w");fputs("CW2 7 2 300 355 25 14 1 2 3 1\n",f);fclose(f);assert(read_save("old.txt",&old)&&old.stars==25&&old.look[1]==2&&old.owned==14);
 assert(write_save("new.txt",&old)&&read_save("new.txt",&old)&&old.stars==25);
 f=fopen("bad.txt","w");fputs("CW3 3 2 300 355",f);fclose(f);assert(!read_save("bad.txt",&old));old=roundtrip;old.slots[2]=7;assert(!valid_save(&old));old=roundtrip;old.x=NAN;assert(!valid_save(&old));
 /* Failed writes leave all transactions unchanged. */
 remove(SAVE_PATH);rmdir("saves");f=fopen("saves","w");fclose(f);
 Save before=g;assert(!buy_look(0)&&g.look[g.who]==before.look[g.who]);assert(!place_furniture(0,0)&&g.slots[0]==before.slots[0]);assert(!travel(0)&&g.island==5);
 g.island=1;g.progress[1]=0;before=g;complete_quest();assert(g.progress[1]==0&&g.stars==before.stars);
 remove("saves");mkdir("saves",0755);g=roundtrip;
 puts("PASS: all purchases affordable, shared outfits, furniture movement, legacy migration, invalid saves, failed-save rollback");
 /* Menu controls don't move characters; cancel does not buy. */
 mode=PLAY;handle_key(SDLK_LSHIFT);assert(mode==SHOP);float x=g.x;keys[SDLK_RIGHT]=1;tick(.05f,keys);assert(g.x==x);keys[SDLK_RIGHT]=0;int coins=g.stars;handle_key(SDLK_LCTRL);assert(mode==PLAY&&g.stars==coins);
 handle_key(SDLK_RCTRL);assert(mode==MAP);handle_key(SDLK_LCTRL);assert(mode==PLAY);handle_key(SDLK_RETURN);assert(mode==PAUSE);pause_choice=2;handle_key(SDLK_SPACE);assert(g.sound==0);handle_key(SDLK_SPACE);assert(g.sound==1);handle_key(SDLK_LCTRL);assert(mode==PLAY);
 const char*dir=argc>1?argv[1]:NULL;
 for(int i=0;i<6;i++){g.island=i;mode=PLAY;toast_time=0;char name[32];snprintf(name,sizeof name,"island-%d",i);snap(dir,name);}
 mode=MAP;snap(dir,"map");mode=SHOP;tab=0;choice=4;snap(dir,"outfits");tab=1;choice=3;snap(dir,"furniture");mode=DECOR;choice=1;snap(dir,"decorate");
 g.island=1;g.progress[1]=0;
 for(int i=0;i<4;i++){begin_spell(i);char name[32];snprintf(name,sizeof name,"spell-%d",i);snap(dir,name);}
 mode=PAUSE;snap(dir,"pause");mode=HELP;snap(dir,"help");mode=PLAY;
 for(int i=0;i<4;i++)for(int n=0;n<8;n++){g.who=i;g.look[i]=n;render();pixel_test();}
 puts("PASS: all islands, menus, spells and 32 outfit combinations render with correct 180-degree presentation");
 Sint16 samples[22050];audio_enabled(0);audio_callback(NULL,(Uint8*)samples,sizeof samples);for(int i=0;i<22050;i++)assert(samples[i]==0);
 audio_enabled(1);chime(2);audio_callback(NULL,(Uint8*)samples,sizeof samples);long energy=0;for(int i=0;i<22050;i++){assert(abs(samples[i])<5000);energy+=abs(samples[i]);}assert(energy>100000);
 defaults(&g);assert(init_audio());SDL_Delay(80);audio_enabled(0);close_audio();puts("PASS: synthesized music/effects, mute, bounded levels and SDL audio callback");
 remove("old.txt");remove("new.txt");remove("bad.txt");remove(SAVE_PATH);rmdir("saves");assert(chdir("/tmp")==0);rmdir(temp);close_video();return 0;
}
