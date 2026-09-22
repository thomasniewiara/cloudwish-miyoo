#include "game.h"
static void backup_legacy(void){
 FILE*f=fopen(SAVE_PATH,"r");if(!f)return;char tag[8]={0};if(fscanf(f,"%7s",tag)!=1){fclose(f);return;}
 if(strcmp(tag,"CW1")&&strcmp(tag,"CW2")&&strcmp(tag,"CW3")){fclose(f);return;}rewind(f);
 if(access("saves/pre-0.4-backup.txt",F_OK)==0){fclose(f);return;}
 FILE*out=fopen("saves/pre-0.4-backup.txt","w");if(out){int c;while((c=fgetc(f))!=EOF)fputc(c,out);fclose(out);}fclose(f);
}
int main(int argc,char**argv){
 defaults(&g);int loaded=read_save(SAVE_PATH,&g);
 if(!loaded&&access(SAVE_PATH,F_OK)==0){fprintf(stderr,"Save unreadable. Preserved without changes. Restore a backup to saves/slot1.txt.\n");return 3;}
 if(loaded)backup_legacy();
 if(!unlocked(g.island)){g.island=0;g.x=320;g.y=355;}
 if(!can_move(g.x,g.y)){g.x=320;g.y=355;}
 if(!init_video()){fprintf(stderr,"Video init failed: %s\n",SDL_GetError());SDL_Quit();return 1;}
 if(argc>2&&!strcmp(argv[1],"--screenshot")){
  if(argc>3){g.island=atoi(argv[3]);if(g.island<0||g.island>5)g.island=0;}
  if(argc>4){if(!strcmp(argv[4],"shop"))mode=SHOP;else if(!strcmp(argv[4],"map"))mode=MAP;else if(!strcmp(argv[4],"decor"))mode=DECOR;else if(!strcmp(argv[4],"adventure"))start_activity();else if(!strcmp(argv[4],"companions"))mode=ADVENTURES;else if(!strcmp(argv[4],"secret"))mode=SECRET;else begin_spell(atoi(argv[4])%4);}
  render();SDL_SaveBMP(canvas,argv[2]);close_video();return 0;
 }
 int sound_ok=init_audio();
 say(sound_ok?"Welcome! SELECT explores the sky. X opens shop.":"Audio unavailable. You can still play silently.");
 Uint32 last=SDL_GetTicks();while(running){Uint32 start=SDL_GetTicks();float dt=(start-last)/1000.f;if(dt>.05f)dt=.05f;last=start;
  SDL_Event e;while(SDL_PollEvent(&e)){if(e.type==SDL_QUIT){if(save_game())running=0;}else if(e.type==SDL_KEYDOWN)handle_key(e.key.keysym.sym);else if(e.type==SDL_ACTIVEEVENT&&!e.active.gain){save_game();if(mode!=PAUSE&&mode!=HELP){resume_mode=mode;mode=PAUSE;}}}
  tick(dt,SDL_GetKeyState(NULL));render();Uint32 elapsed=SDL_GetTicks()-start;if(elapsed<33)SDL_Delay(33-elapsed);
 }
 close_audio();close_video();return 0;
}
