#include "game.h"
SDL_Surface *canvas,*video,*present,*sprites[4],*font,*background;
extern int pattern_value(int round,int index);
static void box(int x,int y,int w,int h,int r,int g,int b){SDL_Rect q={x,y,w,h};SDL_FillRect(canvas,&q,SDL_MapRGB(canvas->format,r,g,b));}
static void disk(int x,int y,int radius,int r,int g,int b){for(int v=-radius;v<=radius;v++){int w=(int)sqrt(radius*radius-v*v);box(x-w,y+v,w*2+1,1,r,g,b);}}
static void blit(SDL_Surface*s,int x,int y){SDL_Rect d={x,y,0,0};SDL_BlitSurface(s,NULL,canvas,&d);}
static void text(int x,int y,const char*s){for(;*s;s++,x+=13){int c=(unsigned char)*s-32;if(c<0||c>95)continue;SDL_Rect a={(c%16)*14,(c/16)*25,14,25},d={x,y,0,0};SDL_BlitSurface(font,&a,canvas,&d);}}
static void wrapped(int x,int y,const char*s,int width){char line[80];while(*s){int n=strlen(s);if(n>width){n=width;while(n>0&&s[n]!=' ')n--;if(!n)n=width;}snprintf(line,sizeof line,"%.*s",n,s);text(x,y,line);s+=n;while(*s==' ')s++;y+=24;}}
static void segment(int x1,int y1,int x2,int y2,int r,int g,int b){int n=abs(x2-x1)+abs(y2-y1);for(int i=0;i<=n;i++){float t=n?(float)i/n:0;disk(x1+(x2-x1)*t,y1+(y2-y1)*t,2,r,g,b);}}
static void heart(int x,int y){disk(x-4,y,5,255,153,197);disk(x+4,y,5,255,153,197);for(int i=0;i<8;i++)box(x-8+i,y+i,17-2*i,1,255,153,197);}
static void star(int x,int y,int size,int r,int g,int b){for(int j=-size;j<=size;j++){int span=(size-abs(j))/3+1;box(x-span,y+j,span*2+1,1,r,g,b);}box(x-size,y-1,size*2+1,3,r,g,b);}
static void plant(int x,int y,int kind){
 static const int colors[4][3]={{248,147,180},{253,221,132},{152,198,252},{195,164,235}};
 int r=colors[kind][0],g=colors[kind][1],b=colors[kind][2];box(x-1,y,3,22,89,158,124);
 if(kind==0){for(int i=-1;i<=1;i++)disk(x+i*7,y,7,r,g,b);disk(x,y-7,7,r,g,b);disk(x,y+7,7,r,g,b);}
 if(kind==1)star(x,y,15,r,g,b);
 if(kind==2){disk(x,y,12,r,g,b);disk(x+4,y-3,8,74,65,104);}
 if(kind==3){for(int i=-12;i<=12;i++)box(x-(12-abs(i)),y+i,(12-abs(i))*2+1,1,r,g,b);}
 disk(x,y,3,255,247,204);
}
static void wear(int kind,int x,int y,int h){
 if(kind==1){disk(x-8,y,8,239,131,180);disk(x+8,y,8,239,131,180);disk(x,y,4,255,221,150);}
 if(kind==2){box(x-17,y-1,34,11,255,213,105);for(int i=-1;i<=1;i++)box(x+i*13-3,y-12,7,16,255,213,105);disk(x,y+4,3,186,125,211);}
 if(kind==3){box(x-16,y,32,8,125,178,242);box(x+7,y+6,10,20,125,178,242);box(x+7,y+19,10,4,246,231,171);}
 if(kind==4){for(int i=0;i<h/2;i++)box(x-10-i/3,y+i,20+2*i/3,1,160,115,212);disk(x,y,4,255,220,113);}
 if(kind==5){for(int i=0;i<26;i++)box(x-i/2,y-26+i,i+1,1,134,214,215);disk(x,y-26,4,255,220,130);}
 if(kind==6){segment(x+22,y+25,x+34,y,246,216,139);star(x+35,y-3,11,255,236,143);}
 if(kind==7){for(int i=0;i<16;i++)box(x-13-i/2,y+i,26+i,1,234,159,207);for(int i=-1;i<=1;i++)disk(x+i*12,y+13,6,245,181,217);}
}
static void creature(int who,int x,int y,int outfit,int animate){
 int h=sprites[who]->h;int bob=animate?(int)(sinf(clock_time*(moving?10:3))*2):0;
 if(animate&&reaction_time>0)bob-=(int)(fabsf(sinf(clock_time*7))*6);
 int oy=(outfit==3||outfit==4||outfit==6)?h/2:outfit==7?h/3:who==3?h*2/3:h-13;
 if(outfit==4)wear(outfit,x,y+bob-oy,h);
 blit(sprites[who],x-sprites[who]->w/2,y+bob-h);
 if(outfit!=4)wear(outfit,x,y+bob-oy,h);
 if(animate&&reaction_time>0){heart(x-25,y-h-5);heart(x+25,y-h-10);}
}
static void friend_at(int x,int y,int kind,int happy){
 int bob=(int)(sinf(clock_time*3)*2);y+=bob;
 disk(x,y-20,18,kind==1?237:255,kind==2?205:240,kind==3?239:205);
 if(kind==1){disk(x-20,y-24,10,230,230,255);disk(x+20,y-24,10,230,230,255);}
 if(kind==2){disk(x-11,y-37,7,180,174,202);disk(x+11,y-37,7,180,174,202);}
 disk(x-6,y-24,2,83,66,107);disk(x+6,y-24,2,83,66,107);
 box(x-5,y-13,10,2,198,117,133);if(happy)heart(x+23,y-42);
}
static void furnishing(int item,int x,int y){
 if(item==1){box(x-37,y-18,74,30,159,133,198);box(x-32,y-27,64,20,231,224,246);box(x-27,y-24,20,14,255,248,230);box(x-35,y+12,7,8,125,107,160);box(x+28,y+12,7,8,125,107,160);}
 if(item==2){box(x-3,y-35,6,46,186,152,124);box(x-19,y+10,38,5,186,152,124);for(int i=0;i<23;i++)box(x-12-i/3,y-52+i,24+2*i/3,1,255,223,149);star(x,y-45,7,255,249,204);}
 if(item==3){box(x-15,y-8,30,23,204,138,137);plant(x,y-26,0);}
 if(item==4){box(x-35,y-26,70,35,179,150,221);box(x-29,y-12,58,19,208,181,237);box(x-42,y-15,13,28,153,123,194);box(x+29,y-15,13,28,153,123,194);}
 if(item==5){box(x-35,y-12,70,10,212,172,137);box(x-28,y-2,7,22,167,131,113);box(x+21,y-2,7,22,167,131,113);disk(x,y-18,10,244,241,220);box(x-20,y-25,10,11,181,216,219);}
 if(item==6){for(int i=4;i>=0;i--)box(x-44+i*5,y-20+i*4,88-i*10,40-i*8,218-i*15,166+i*12,212+i*5);star(x,y,9,255,235,164);}
}
static void scenery(void){
 int island=g.island;
 if(island==0)blit(background,0,0);
 else{
  static const int sky[5][3]={{155,203,231},{157,190,227},{244,192,217},{76,82,137},{170,193,220}};
  static const int land[5][3]={{190,224,180},{194,218,234},{247,220,215},{144,154,201},{236,212,196}};
  const int *s=sky[island-1],*p=land[island-1];box(0,0,640,480,s[0],s[1],s[2]);
  for(int i=0;i<4;i++){int x=20+i*180+(int)(sinf(clock_time*.15f+i)*8);disk(x,135,28,229,236,247);box(x-40,136,80,20,229,236,247);}
  box(48,146,544,251,p[0],p[1],p[2]);
  if(island==1){for(int i=0;i<8;i++)plant(78+i*67,170+(i%2)*200,i%4);disk(320,245,42,141,190,213);disk(320,245,27,161,211,226);}
  if(island==2){for(int i=0;i<7;i++){int x=90+i*76,y=160+(i%2)*200;for(int j=0;j<30;j++)box(x-j/2,y-30+j,j+1,1,173+i*8,157+i*9,220);}}
  if(island==3){for(int i=0;i<5;i++){int x=110+i*105;box(x-3,178,6,24,218,173,145);disk(x,165,20,245,165+i*8,199);disk(x-5,160,8,255,221,218);}box(60,253,520,15,195,161,222);}
  if(island==4){disk(550,105,25,255,237,180);disk(560,99,21,s[0],s[1],s[2]);for(int i=0;i<15;i++)star(50+i*39,100+(i*31)%270,4,246,235,191);for(int i=0;i<4;i++)plant(90+i*150,367,2);}
  if(island==5){box(60,130,520,267,231,210,200);for(int y=155;y<390;y+=30)box(60,y,520,2,214,190,183);box(240,118,160,45,145,184,218);box(318,118,4,45,236,224,210);for(int i=0;i<6;i++){disk(slot_x[i],slot_y[i],31,214,191,189);if(g.slots[i])furnishing(g.slots[i],slot_x[i],slot_y[i]-5);else{char n[2]={(char)('1'+i),0};text(slot_x[i]-6,slot_y[i]-14,n);}}}
 }
 if(island==0){
  if(g.progress[0]>=2)for(int i=0;i<6;i++)box(278+i*14,237,14,44,248-i*13,137+i*13,162+i*10);
  disk(155,346,27,162,186,180);text(127,363,"HOME");
  disk(480,340,13,g.progress[0]>=5?255:159,g.progress[0]>=5?215:161,110);box(475,348,10,14,123,104,110);
  for(int i=0;i<3;i++)plant(135+i*20,182,g.progress[0]>=6?i:2);
  if(g.progress[0]<3)friend_at(465,185,0,0);else if(g.progress[0]==3)friend_at(g.x+30,g.y+7,0,1);else friend_at(155,330,0,1);
 }else if(island<5){int p=g.progress[island];friend_at(525,p>=caps[island]?330:215,island,p>=caps[island]);for(int i=0;i<p;i++)star(260+i*36,360,8,254,220,137);}
 if(island<5){box(62,278,47,34,186,139,201);box(57,274,57,10,246,183,204);text(59,310,"SHOP");}
 const Quest*q=quest();if(q){disk(q->x,q->y,12+(int)(sinf(clock_time*4)*2),253,225,149);star(q->x,q->y,7,255,252,215);}
 creature(g.who,g.x,g.y,g.look[g.who],1);
 if(reaction_time>0)for(int i=0;i<9;i++)star(g.x+(i-4)*15,g.y-40-(int)((3-reaction_time)*25)-(i%3)*10,3,255,226,159);
}
static void panel(const char *title){box(20,87,600,318,65,58,104);text(40,94,title);}
static void shop_panel(void){
 panel(tab?"CLOUD CLOSET  /  HOME DECOR":"CLOUD CLOSET  /  OUTFITS");int total=tab?6:8;
 for(int i=0;i<total;i++){char row[64],price[20];int own=tab?(g.furniture&(1<<i)):i==0||(g.owned&(1<<i));snprintf(price,sizeof price,own?"Owned":"%d stars",tab?20:prices[i]);snprintf(row,sizeof row,"%c %-14s %s",choice==i?'>':' ',tab?furniture_names[i]:looks[i],price);text(36,125+i*25,row);}
 if(tab)furnishing(choice+1,530,230);else creature(g.who,530,245,choice,1);
 if(!tab)text(492,259,names[g.who]);
 text(38,329,shop_note);text(38,354,"Left/right tab   Up/down choose   B back");text(38,379,tab?"A buy    SELECT map after leaving shop":"A buy/wear    L/R preview another friend");
}
static void map_panel(void){panel("THE SKY MAP  /  A TRAVEL   B BACK");for(int i=0;i<6;i++){char row[80];snprintf(row,sizeof row,"%c %-16s %s",map_choice==i?'>':' ',island_names[i],unlocked(i)?i==g.island?"You are here":"Open":"Locked");text(42,132+i*32,row);}if(!unlocked(map_choice))wrapped(42,332,"Help every friend on the previous island. Fairy Garden opens after Pip's rescue.",40);else text(42,348,"Up/down choose    A visit this island");}
static void decor_panel(void){panel("DECORATE YOUR CLOUD HOME");for(int i=0;i<7;i++){char row[70];snprintf(row,sizeof row,"%c %-14s %s",choice==i?'>':' ',i?furniture_names[i-1]:"Empty space",i&&!(g.furniture&(1<<(i-1)))?"Buy in shop":"Ready");text(40,132+i*28,row);}if(choice)furnishing(choice,530,230);text(40,340,"A place/move    B back    X shop outside");text(40,370,"Each item can live in one spot at a time.");}
static const int node_x[]={185,320,455},node_y[]={200,259,318};
static void spell_panel(void){
 char title[80];snprintf(title,sizeof title,"%s'S MAGIC",names[spell.type]);panel(title);
 if(spell.type==0){
  text(42,125,"Connect the rainbow numbers in order.");
  for(int i=1;i<spell.count;i++){int a=spell.path[i-1],b=spell.path[i];segment(node_x[a%3],node_y[a/3],node_x[b%3],node_y[b/3],255,215,150);}
  for(int i=0;i<9;i++){disk(node_x[i%3],node_y[i/3],19,i==spell.cursor?254:123,i==spell.cursor?221:115,170);char n[2]=".";for(int j=0;j<spell.length;j++)if(spell.path[j]==i)n[0]='1'+j;text(node_x[i%3]-6,node_y[i/3]-13,n);}
  text(40,345,"D-pad move   A connect   B undo/back");
 }else if(spell.type==1){
  text(42,125,"Which flower completes this pattern?");
  for(int i=0;i<5;i++)plant(140+i*65,205,pattern_value(spell.round,i));
  text(458,190,"?");
  for(int i=0;i<4;i++){int x=155+i*110;disk(x,286,25,i==spell.cursor?237:104,i==spell.cursor?211:97,149);plant(x,278,i);}
  char n[60];snprintf(n,sizeof n,"Pattern %d/3   D-pad choose   A grow",spell.round+1);text(40,337,n);
 }else if(spell.type==2){
  text(42,125,"Hold A, then release in the gold band.");
  for(int i=0;i<3;i++){int x=220+i*100;disk(x,208,22,i<spell.count?255:119,i<spell.count?225:112,154);box(x-12,230,24,6,178,146,144);}
  box(115,271,410,35,107,102,145);box(115+196,271,123,35,229,199,115);
  float heat=spell.heat;if(heat>1)heat=1;box(115+(int)(heat*400),265,7,47,255,248,223);
  text(42,334,"3 cozy lights. No rush. B goes back.");
 }else{
  text(42,125,spell.reveal>0?"Watch the stars glow, then repeat them.":"Repeat the glowing star order. Y replays.");
  int glow=spell.reveal>0?(int)((3.6f-spell.reveal)/.9f): -1;if(glow>3)glow=3;
  for(int i=0;i<4;i++){int x=155+i*110;int lit=glow>=0&&spell.path[glow]==i;disk(x,245,30,i==spell.cursor?170:100,lit?213:107,lit?150:161);star(x,245,lit?22:12,255,236,170);char n[2]={(char)('1'+i),0};text(x-6,290,n);}
  char n[70];snprintf(n,sizeof n,"%d/4 stars    D-pad choose    A sparkle",spell.count);text(42,334,n);
 }
 text(40,375,spell.note);
}
void render(void){
 scenery();box(0,0,640,85,58,51,91);char head[80];snprintf(head,sizeof head,"%-18s  STARS: %d",island_names[g.island],g.stars);text(18,7,head);const Quest*q=quest();text(18,37,q?q->title:g.island==5?"Your own cozy corner of the sky":"All friends helped! SELECT explores the sky");
 box(0,408,640,72,58,51,91);snprintf(head,sizeof head,"%s  L/R friend  A magic  START pause",names[g.who]);text(9,411,head);text(9,440,"D-pad move   Y hint   X shop   SELECT map");
 if(mode==PLAY&&toast_time>0){box(5,87,630,59,81,72,117);wrapped(12,90,toast,47);}
 if(mode==SHOP)shop_panel();
 if(mode==MAP)map_panel();
 if(mode==DECOR)decor_panel();
 if(mode==PUZZLE)spell_panel();
 if(mode==PAUSE){panel("A LITTLE CLOUD BREAK");const char*items[]={"Resume","Controls",g.sound?"Sound: on":"Sound: off","Save and quit"};for(int i=0;i<4;i++){char row[70];snprintf(row,sizeof row,"%c %s",pause_choice==i?'>':' ',items[i]);text(75,153+i*43,row);}text(45,356,save_ok?"Your missions and purchases save right away.":"SAVE ERROR: check the SD card.");}
 if(mode==HELP){panel("YOUR SKY ADVENTURE");text(42,135,"D-pad walk     L/R switch magical friend");text(42,175,"A help/cast    B undo or leave a menu");text(42,215,"Y hint/replay  X clothing and furniture");text(42,255,"SELECT map     START pause / sound");text(42,295,"At home: A near a circle to decorate");text(42,355,"A or B returns to your cloud break");}
 SDL_Surface*frame=canvas;
#ifdef MIYOO_ROTATE_180
 if(SDL_LockSurface(canvas)<0)return;
 if(SDL_LockSurface(present)<0){SDL_UnlockSurface(canvas);return;}
 for(int y=0;y<H;y++){const Uint32*src=(const Uint32*)((const Uint8*)canvas->pixels+y*canvas->pitch);Uint32*dst=(Uint32*)((Uint8*)present->pixels+(H-1-y)*present->pitch);for(int x=0;x<W;x++)dst[W-1-x]=src[x];}
 SDL_UnlockSurface(present);SDL_UnlockSurface(canvas);frame=present;
#endif
 if(SDL_BlitSurface(frame,NULL,video,NULL)<0||SDL_Flip(video)<0){fprintf(stderr,"Present failed: %s\n",SDL_GetError());running=0;}
}
static SDL_Surface*load(const char*name,int transparent){char path[180];snprintf(path,sizeof path,"assets/%s.bmp",name);SDL_Surface*s=SDL_LoadBMP(path);if(!s){fprintf(stderr,"Missing %s: %s\n",path,SDL_GetError());return NULL;}if(transparent)SDL_SetColorKey(s,SDL_SRCCOLORKEY,SDL_MapRGB(s->format,255,0,255));return s;}
int init_video(void){
 if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER)<0)return 0;
 video=SDL_SetVideoMode(W,H,32,SDL_HWSURFACE);
 if(!video||video->w!=W||video->h!=H||video->format->BitsPerPixel!=32||video->pitch<W*4)return 0;
 canvas=SDL_CreateRGBSurface(SDL_SWSURFACE,W,H,32,0x00ff0000,0x0000ff00,0x000000ff,0);
 present=SDL_CreateRGBSurface(SDL_SWSURFACE,W,H,32,0x00ff0000,0x0000ff00,0x000000ff,0);
 if(!canvas||!present)return 0;
 const char*files[]={"luna","poppy","ember","milo"};for(int i=0;i<4;i++){sprites[i]=load(files[i],1);if(!sprites[i])return 0;}
 font=load("font",1);background=load("meadow",0);if(!font||!background)return 0;
 SDL_ShowCursor(SDL_DISABLE);SDL_EnableKeyRepeat(0,0);char driver[80]={0};SDL_VideoDriverName(driver,sizeof driver);
 fprintf(stderr,"Cloudwish 0.3; video=%s %dx%d bpp=%u pitch=%u\n",driver,video->w,video->h,video->format->BitsPerPixel,video->pitch);
#ifdef MIYOO_ROTATE_180
 fprintf(stderr,"Presentation rotation: 180 degrees\n");
#endif
 return 1;
}
void close_video(void){for(int i=0;i<4;i++)if(sprites[i])SDL_FreeSurface(sprites[i]);if(font)SDL_FreeSurface(font);if(background)SDL_FreeSurface(background);if(canvas)SDL_FreeSurface(canvas);if(present)SDL_FreeSurface(present);SDL_Quit();}
