/* Cloudwish Kingdom - native handheld demo. SDL 1.2, software renderer. */
#include <SDL/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/stat.h>
#include <unistd.h>
#include <assert.h>

typedef struct { int step,who; float x,y; int stars,owned,outfit[4]; } Save;
static Save g={.step=0,.who=3,.x=320,.y=355};
static SDL_Surface *present,*video,*screen,*bg,*font,*creatures[4];
static int running=1,puzzle=0,selected=0,linked=0,paused=0,menu=0,help=0,save_ok=1;
static int shop=0,item=1;
static const int prices[]={0,10,15,20};
static const char *clothes[]={"Original look","Rose bow","Sunshine crown","Sky scarf"};
static char shopnote[80]="Choose a look for your friend.";
static Uint32 message_until;
static char message[80]="Welcome! Milo can reveal hidden stars.";
static const char *names[]={"Luna","Poppy","Ember","Milo"};
static const char *quests[]={"Milo: reveal the hidden stars","Luna: make a rainbow bridge","Cross the bridge and find Pip","Bring Pip home across the bridge","Ember: light the picnic lantern","Poppy: grow a flower welcome","Milo: brighten the evening sky","You helped Pip! Enjoy the meadow"};
static const char *hints[]={"Go to the glowing star. Use Milo's magic.","Use L/R for Luna. Visit the star again.","Walk over the rainbow to Pip. Press A.","Walk Pip to the HOME circle. Press A.","Pick Ember. Visit the lantern on the right.","Pick Poppy. Visit the flowers on the left.","Pick Milo. Visit the star by the bridge.","Visit Pip at HOME for a happy greeting!"};
static int tx[]={320,320,465,155,480,155,320,155};
static int ty[]={304,304,185,346,346,190,304,346};
static int needed[]={3,0,-1,-1,2,1,3,-1};
static void say(const char *s){snprintf(message,sizeof message,"%s",s);message_until=SDL_GetTicks()+4200;}
static int valid(Save s){
 if(!(s.step>=0&&s.step<=7&&s.who>=0&&s.who<4&&isfinite(s.x)&&isfinite(s.y)&&s.x>=65&&s.x<=575&&s.y>=155&&s.y<=381&&s.stars>=0&&s.stars<=70&&s.owned>=0&&s.owned<=14&&(s.owned&1)==0))return 0;
 for(int i=0;i<4;i++)if(s.outfit[i]<0||s.outfit[i]>3||(s.outfit[i]&&!(s.owned&(1<<s.outfit[i]))))return 0;
 return 1;
}
static int write_save(const char *path,Save s){char tmp[256];FILE *f;int ok;snprintf(tmp,sizeof tmp,"%s.tmp",path);f=fopen(tmp,"w");if(!f)return 0;ok=fprintf(f,"CW2 %d %d %.2f %.2f %d %d %d %d %d %d\n",s.step,s.who,s.x,s.y,s.stars,s.owned,s.outfit[0],s.outfit[1],s.outfit[2],s.outfit[3])>0;if(fflush(f)||fsync(fileno(f)))ok=0;if(fclose(f))ok=0;if(!ok){remove(tmp);return 0;}return rename(tmp,path)==0;}
static int read_save(const char *path,Save *s){
 Save n={0};char tag[8];FILE*f=fopen(path,"r");if(!f)return 0;
 int ok=fscanf(f,"%7s %d %d %f %f",tag,&n.step,&n.who,&n.x,&n.y)==5;
 if(ok&&!strcmp(tag,"CW1")){if(n.step>=0&&n.step<=7)n.stars=n.step*10;else ok=0;}
 else if(ok&&!strcmp(tag,"CW2"))ok=fscanf(f,"%d %d %d %d %d %d",&n.stars,&n.owned,&n.outfit[0],&n.outfit[1],&n.outfit[2],&n.outfit[3])==6;
 else ok=0;
 fclose(f);if(ok&&valid(n)){*s=n;return 1;}return 0;
}
static void save(void){mkdir("saves",0755);save_ok=write_save("saves/slot1.txt",g);if(!save_ok)say("Save failed. Check free space on your SD.");}
static void shop_action(void){
 Save before=g;
 if(item&&!(g.owned&(1<<item))){
  if(g.stars<prices[item]){snprintf(shopnote,sizeof shopnote,"Need %d more stars. Help friends to earn!",prices[item]-g.stars);return;}
  g.stars-=prices[item];g.owned|=1<<item;
 }
 g.outfit[g.who]=item;save();
 if(!save_ok){g=before;snprintf(shopnote,sizeof shopnote,"Could not save. Nothing was purchased.");}
 else snprintf(shopnote,sizeof shopnote,"%s is wearing %s!",names[g.who],clothes[item]);
}
static int can_move(float x,float y){if(x<65||x>575||y<155||y>381)return 0;if(y>240&&y<278)return g.step>=2&&x>275&&x<365;return 1;}
static void finish(void){if(g.step<7){g.step++;g.stars+=10;}puzzle=0;save();if(save_ok)say(g.step==7?"Pip: Hooray! +10 stars! Try the shop with X.":"Wonderful! +10 stars! X opens the shop.");}
static void action(void){if(fabsf(g.x-85)<24&&fabsf(g.y-300)<24){shop=1;return;}if(fabsf(g.x-tx[g.step])>57||fabsf(g.y-ty[g.step])>48){say(hints[g.step]);return;}if(needed[g.step]>=0&&needed[g.step]!=g.who){char s[80];snprintf(s,sizeof s,"This needs %s! Switch friends with L/R.",names[needed[g.step]]);say(s);return;}if(g.step==7){say("Pip: You are my wonderful sky friend!");return;}if(g.step==2||g.step==3){finish();return;}puzzle=1;selected=linked=0;}
static Uint32 color(int r,int b,int c){return SDL_MapRGB(screen->format,r,b,c);}
static void box(int x,int y,int w,int h,int r,int b,int c){SDL_Rect q={x,y,w,h};SDL_FillRect(screen,&q,color(r,b,c));}
static void blit(SDL_Surface*s,int x,int y){SDL_Rect d={x,y,0,0};SDL_BlitSurface(s,NULL,screen,&d);}
static void text(int x,int y,const char*s){for(;*s;s++,x+=13){int c=(unsigned char)*s-32;if(c<0||c>95)continue;SDL_Rect a={(c%16)*14,(c/16)*25,14,25},d={x,y,0,0};SDL_BlitSurface(font,&a,screen,&d);}}
static void circle(int x,int y,int rad,int r,int b,int c){for(int v=-rad;v<=rad;v++){int w=(int)sqrt((double)(rad*rad-v*v));box(x-w,y+v,w*2+1,1,r,b,c);}}
static void line(int x1,int y1,int x2,int y2){int n=abs(x2-x1)+abs(y2-y1);for(int i=0;i<=n;i++){float t=n?(float)i/n:0;circle(x1+(x2-x1)*t,y1+(y2-y1)*t,3,255,224,142);}}
static void pip(int x,int y){circle(x,y-20,18,255,246,210);circle(x-7,y-24,2,83,66,107);circle(x+7,y-24,2,83,66,107);box(x-5,y-13,10,2,198,117,133);}
static void outfit(int kind,int x,int y){
 if(kind==1){circle(x-9,y,9,239,131,180);circle(x+9,y,9,239,131,180);circle(x,y,5,255,221,150);}
 if(kind==2){box(x-18,y-1,36,12,255,213,105);for(int i=-1;i<=1;i++)box(x+i*14-4,y-13,8,17,255,213,105);circle(x,y+4,3,186,125,211);}
 if(kind==3){box(x-16,y,32,9,125,178,242);box(x+7,y+6,10,21,125,178,242);box(x+7,y+20,10,4,246,231,171);}
}
static void friend_sprite(int who,int x,int y,int look){
 blit(creatures[who],x-creatures[who]->w/2,y-creatures[who]->h);
 int offset=look==3?creatures[who]->h/2:who==3?creatures[who]->h*2/3:creatures[who]->h-13;
 outfit(look,x,y-offset);
}
static int nx[]={155,265,375,485},ny[]={300,211,211,300};
static void render(void){
 blit(bg,0,0);box(0,0,640,85,58,51,91);char wallet[70];snprintf(wallet,sizeof wallet,"CLOUDWISH KINGDOM       STARS: %d",g.stars);text(18,7,wallet);text(18,37,quests[g.step]);
 if(g.step>=2){int rainbow[6][3]={{248,137,162},{255,189,122},{253,226,143},{144,211,174},{136,194,239},{196,168,230}};for(int i=0;i<6;i++)box(278+i*14,237,14,44,rainbow[i][0],rainbow[i][1],rainbow[i][2]);}
 box(62,278,47,38,186,139,201);box(57,274,57,10,246,183,204);text(59,311,"SHOP");
 circle(155,346,27,162,186,180);text(127,361,"HOME");
 circle(480,340,13,g.step>=5?255:159,g.step>=5?215:161,110);box(475,348,10,14,123,104,110);
 for(int i=0;i<3;i++){box(143+i*13,185,3,17,93,149,116);circle(144+i*13,183,6,g.step>=6?242:170,g.step>=6?165:190,190);}
 if(g.step<3)pip(465,185);else if(g.step==3)pip(g.x+30,g.y+7);else pip(155,330);
 circle(tx[g.step],ty[g.step],10+(SDL_GetTicks()/350)%3,255,235,164);
 if(g.step>=1)for(int i=0;i<3;i++)circle(298+i*22,292,3,255,250,216);
 friend_sprite(g.who,g.x,g.y,g.outfit[g.who]);
 box(0,408,640,72,58,51,91);char s[80];snprintf(s,sizeof s,"%s  | L/R friend | A magic | START pause",names[g.who]);text(9,411,s);
 if(!shop&&!puzzle&&!paused&&!help&&SDL_GetTicks()<message_until){/* split the message at a word boundary */char first[49],second[80];int len=strlen(message),cut=len>47?47:len;if(len>47)while(cut>0&&message[cut]!=' ')cut--;snprintf(first,sizeof first,"%.*s",cut,message);snprintf(second,sizeof second,"%s",message+cut+(message[cut]==' '));box(5,88,630,58,81,72,117);text(12,90,first);text(12,114,second);}
 text(9,440,"D-pad move    Y hint    X shop / outfits");
 if(puzzle){box(35,100,570,294,65,58,104);text(62,112,g.step==1?"LUNA'S RAINBOW":g.step==5?"POPPY'S FLOWERS":g.step==4?"EMBER'S WARM LIGHT":"MILO'S STAR MAGIC");text(62,145,"Connect 1, 2, 3, 4 to cast your spell.");for(int i=1;i<linked;i++)line(nx[i-1],ny[i-1],nx[i],ny[i]);for(int i=0;i<4;i++){circle(nx[i],ny[i],25,i==selected?255:120,i==selected?229:120,i==selected?159:166);circle(nx[i],ny[i],19,i<linked?99:74,i<linked?177:65,130);char a[2]={(char)('1'+i),0};text(nx[i]-6,ny[i]-13,a);}text(62,342,"D-pad choose   A connect   B undo/back");}
 if(shop){
 box(20,87,600,318,65,58,104);text(42,95,"THE CLOUD CLOSET");
 for(int i=0;i<4;i++){char row[64];snprintf(row,sizeof row,"%c %-14s %s",i==item?'>':' ',clothes[i],i==0||g.owned&(1<<i)?"Owned":i==1?"10 stars":i==2?"15 stars":"20 stars");text(38,137+i*37,row);}
 friend_sprite(g.who,530,258,item);text(492,269,names[g.who]);
 text(38,293,shopnote);text(38,326,"Up/down choose   L/R change friend");
 text(38,356,item==0||g.owned&(1<<item)?"A wear    B back    Free to wear again":"A buy + wear    B back");
 }
 if(paused){box(70,115,500,250,58,51,91);text(105,130,"A LITTLE CLOUD BREAK");text(105,175,menu==0?"> Resume":"  Resume");text(105,212,menu==1?"> Controls":"  Controls");text(105,249,menu==2?"> Save and quit":"  Save and quit");text(105,306,save_ok?"Your missions save automatically.":"SAVE ERROR: check SD free space.");}
 if(help){box(35,100,570,294,58,51,91);text(60,120,"YOUR SKY ADVENTURE");text(60,163,"D-pad: walk / select spell points");text(60,195,"A: talk, help, or connect a point");text(60,227,"B: undo / back     Y: mission hint");text(60,259,"L/R: change magical creature");text(60,291,"START: pause      A or B: close");}
 SDL_Surface *frame=screen;
#ifdef MIYOO_ROTATE_180
 /* The native Miyoo SDL scanout is inverted. Rotate the completed frame,
    keeping logical drawing and input coordinates unchanged. Respect pitch. */
 if(SDL_LockSurface(screen)<0){fprintf(stderr,"Canvas lock: %s\n",SDL_GetError());running=0;return;}
 if(SDL_LockSurface(present)<0){SDL_UnlockSurface(screen);fprintf(stderr,"Present lock: %s\n",SDL_GetError());running=0;return;}
 for(int y=0;y<480;y++){
  const Uint32 *src=(const Uint32*)((const Uint8*)screen->pixels+y*screen->pitch);
  Uint32 *dst=(Uint32*)((Uint8*)present->pixels+(479-y)*present->pitch);
  for(int x=0;x<640;x++)dst[639-x]=src[x];
 }
 SDL_UnlockSurface(present);SDL_UnlockSurface(screen);frame=present;
#endif
 if(SDL_BlitSurface(frame,NULL,video,NULL)<0||SDL_Flip(video)<0){
  fprintf(stderr,"Present failed: %s\n",SDL_GetError());running=0;
 }
}
static void key(SDLKey k){int a=k==SDLK_SPACE,b=k==SDLK_LCTRL;if(help){if(a||b||k==SDLK_RETURN)help=0;return;}if(k==SDLK_RETURN||k==SDLK_ESCAPE){paused=!paused;menu=0;return;}if(paused){if(k==SDLK_UP)menu=(menu+2)%3;if(k==SDLK_DOWN)menu=(menu+1)%3;if(b)paused=0;if(a){if(menu==0)paused=0;else if(menu==1)help=1;else{save();if(save_ok)running=0;}}return;}if(shop){
 if(b||k==SDLK_LSHIFT){shop=0;return;}
 if(k==SDLK_UP)item=(item+3)%4;
 if(k==SDLK_DOWN)item=(item+1)%4;
 if(k==SDLK_e||k==SDLK_TAB)g.who=(g.who+3)%4;
 if(k==SDLK_t||k==SDLK_BACKSPACE)g.who=(g.who+1)%4;
 if(a)shop_action();else snprintf(shopnote,sizeof shopnote,"Preview only. A buys or equips this look.");return;
 }if(puzzle){if(k==SDLK_LALT){selected=linked;return;}if(k==SDLK_LEFT||k==SDLK_UP)selected=(selected+3)%4;if(k==SDLK_RIGHT||k==SDLK_DOWN)selected=(selected+1)%4;if(b){if(linked){linked--;selected=linked;}else puzzle=0;}if(a){if(selected==linked){linked++;if(linked==4)finish();else selected=linked;}else say("Try the next numbered point. You can do it!");}return;}if(k==SDLK_e||k==SDLK_TAB)g.who=(g.who+3)%4;if(k==SDLK_t||k==SDLK_BACKSPACE)g.who=(g.who+1)%4;if(k==SDLK_LSHIFT){shop=1;snprintf(shopnote,sizeof shopnote,"One purchase unlocks it for every friend.");}if(k==SDLK_LALT)say(hints[g.step]);if(a)action();}
static SDL_Surface* load(const char*name,int transparent){char p[160];snprintf(p,sizeof p,"assets/%s.bmp",name);SDL_Surface*s=SDL_LoadBMP(p);if(!s){fprintf(stderr,"Missing asset %s: %s\n",p,SDL_GetError());exit(2);}if(transparent)SDL_SetColorKey(s,SDL_SRCCOLORKEY,SDL_MapRGB(s->format,255,0,255));return s;}
static int tests(void){Save s={.step=6,.who=1,.x=400,.y=355},r={.x=100,.y=155};assert(valid(s));s.step=8;assert(!valid(s));s.step=6;assert(write_save("/tmp/cloudwish-test.txt",s));assert(read_save("/tmp/cloudwish-test.txt",&r));assert(r.step==6&&r.who==1&&r.x==400);FILE*f=fopen("/tmp/cloudwish-test.txt","w");fputs("CW1 99 0 nan 300",f);fclose(f);assert(!read_save("/tmp/cloudwish-test.txt",&r));remove("/tmp/cloudwish-test.txt");g.step=1;assert(!can_move(320,260));g.step=2;assert(can_move(320,260));assert(!can_move(250,260));assert(!can_move(40,300));puts("PASS: save roundtrip, malformed save rejection, bridge and boundary rules");return 0;}
int main(int argc,char**argv){if(argc>1&&!strcmp(argv[1],"--self-test"))return tests();if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER)<0){fprintf(stderr,"SDL: %s\n",SDL_GetError());return 1;}atexit(SDL_Quit);/* Match Onion 4.3.1's 32-bit hardware display and separate canvas. */
video=SDL_SetVideoMode(640,480,32,SDL_HWSURFACE);
if(!video){fprintf(stderr,"Video: %s\n",SDL_GetError());return 1;}
screen=SDL_CreateRGBSurface(SDL_SWSURFACE,640,480,32,0x00ff0000,0x0000ff00,0x000000ff,0);
if(!screen){fprintf(stderr,"Canvas: %s\n",SDL_GetError());return 1;}
present=SDL_CreateRGBSurface(SDL_SWSURFACE,640,480,32,0x00ff0000,0x0000ff00,0x000000ff,0);
if(!present){fprintf(stderr,"Presentation canvas: %s\n",SDL_GetError());return 1;}
#ifdef MIYOO_ROTATE_180
fprintf(stderr,"Presentation rotation: 180 degrees\n");
#endif
char driver[80]={0};SDL_VideoDriverName(driver,sizeof driver);
fprintf(stderr,"Cloudwish 0.2 shop and rewards; SDL driver=%s\n",driver);
fprintf(stderr,"Display: %dx%d bpp=%u pitch=%u flags=0x%lx\n",video->w,video->h,(unsigned)video->format->BitsPerPixel,(unsigned)video->pitch,(unsigned long)video->flags);
fprintf(stderr,"Canvas: %dx%d bpp=%u pitch=%u\n",screen->w,screen->h,(unsigned)screen->format->BitsPerPixel,(unsigned)screen->pitch);
FILE *maps=fopen("/proc/self/maps","r");if(maps){char linebuf[512];while(fgets(linebuf,sizeof linebuf,maps))if(strstr(linebuf,"libSDL"))fputs(linebuf,stderr);fclose(maps);}
fflush(stderr);
if(video->w!=640||video->h!=480||video->format->BitsPerPixel!=32||video->pitch<640*4){fprintf(stderr,"Unsupported display layout; expected 640x480 32-bit.\n");return 1;}
SDL_ShowCursor(SDL_DISABLE);SDL_EnableKeyRepeat(0,0);bg=load("meadow",0);font=load("font",1);for(int i=0;i<4;i++){const char*n[]={"luna","poppy","ember","milo"};creatures[i]=load(n[i],1);}read_save("saves/slot1.txt",&g);if(!can_move(g.x,g.y)){g.x=320;g.y=355;}say("Welcome! Follow the glowing mission marker.");if(argc>2&&!strcmp(argv[1],"--screenshot")){if(argc>3){g.step=atoi(argv[3]);if(g.step<0||g.step>7)g.step=0;}if(argc>4){if(!strcmp(argv[4],"shop"))shop=1;else puzzle=1;}render();SDL_SaveBMP(screen,argv[2]);return 0;}Uint32 last=SDL_GetTicks();while(running){Uint32 start=SDL_GetTicks();float dt=(start-last)/1000.f;if(dt>.05f)dt=.05f;last=start;SDL_Event e;while(SDL_PollEvent(&e)){if(e.type==SDL_QUIT){save();running=0;}else if(e.type==SDL_KEYDOWN)key(e.key.keysym.sym);else if(e.type==SDL_ACTIVEEVENT&&!e.active.gain)save();}if(!paused&&!help&&!puzzle&&!shop){Uint8*k=SDL_GetKeyState(NULL);float dx=k[SDLK_RIGHT]-k[SDLK_LEFT],dy=k[SDLK_DOWN]-k[SDLK_UP];if(dx&&dy){dx*=.7071f;dy*=.7071f;}float x=g.x+dx*125*dt,y=g.y+dy*125*dt;if(can_move(x,g.y))g.x=x;if(can_move(g.x,y))g.y=y;}render();Uint32 elapsed=SDL_GetTicks()-start;if(elapsed<33)SDL_Delay(33-elapsed);}for(int i=0;i<4;i++)SDL_FreeSurface(creatures[i]);SDL_FreeSurface(bg);SDL_FreeSurface(font);SDL_FreeSurface(screen);SDL_FreeSurface(present);return 0;}
