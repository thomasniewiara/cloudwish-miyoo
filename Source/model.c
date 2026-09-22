#include "game.h"
Save g;
Mode mode=PLAY,resume_mode=PLAY;
Spell spell;
int running=1,choice=0,tab=0,decor_slot=0,pause_choice=0,map_choice=0,save_ok=1;
float toast_time=0,reaction_time=0,clock_time=0,moving=0;
char toast[100]="",shop_note[90]="Choose a look. A buys or wears.";
const char *names[]={"Luna","Poppy","Ember","Milo"};
const char *island_names[]={"Rainbow Meadow","Fairy Garden","Crystal Peaks","Candy Clouds","Moon Meadow","Cloud Home"};
const char *looks[]={"Original look","Rose bow","Sunshine crown","Sky scarf","Royal cape","Party hat","Star wand","Petal skirt"};
const int prices[]={0,10,15,20,25,25,30,30};
const char *furniture_names[]={"Cloud bed","Star lamp","Flower pot","Cozy sofa","Tea table","Rainbow rug"};
const int caps[]={7,4,4,4,4};
const int slot_x[]={145,320,490,145,320,490},slot_y[]={210,210,210,340,340,340};
static const Quest quests[5][7]={
 {{"Milo: reveal the hidden stars","Use Milo by the bridge marker.","Milo: The stars were here all along!",3,320,304},
 {"Luna: make a rainbow bridge","Use Luna at the bridge marker.","Luna: A rainbow for our friends!",0,320,304},
 {"Cross the bridge and find Pip","Cross at the center. Press A near Pip.","Pip: You found me! Let's go home!",-1,465,185},
 {"Bring Pip home across the bridge","Walk Pip to HOME and press A.","Pip: Thank you! Fairy Garden is open!",-1,155,346},
 {"Ember: light the picnic lantern","Use Ember at the right lantern.","Ember: A cozy light for Pip!",2,480,346},
 {"Poppy: grow a flower welcome","Use Poppy at the flowers up left.","Poppy: Every flower says welcome!",1,155,190},
 {"Milo: brighten the evening sky","Use Milo by the bridge marker.","Pip: Our meadow is full of magic!",3,320,304}},
 {{"Poppy: wake the sleepy blossoms","Help the flowers at the left marker.","Poppy: Look! They're dancing!",1,150,195},
 {"Luna: mend the butterfly path","Make a rainbow near the pond.","Luna: The butterflies can come home!",0,450,310},
 {"Ember: warm the tiny seed nest","Warm the seed nest at the top right.","Ember: Tiny seeds, big dreams!",2,475,185},
 {"Milo: find the lost firefly","Remember the stars by the willow.","Twinkle: You found me! Peaks are open!",3,180,335}},
 {{"Milo: reveal the crystal trail","Follow the hidden stars at the left.","Milo: A secret trail through the gems!",3,155,190},
 {"Ember: thaw Pebble's cozy cave","Warm the cave at the top right.","Pebble: My paws are warm again!",2,475,185},
 {"Luna: guide Pebble to the valley","Connect a safe rainbow at the bottom.","Luna: Come along, little Pebble!",0,320,345},
 {"Poppy: plant the valley flowers","Grow flowers at the lower left.","Pebble: Candy Clouds is ready to visit!",1,160,330}},
 {{"Luna: bridge the candy stream","Make a rainbow at the stream marker.","Luna: A sweet way across!",0,320,315},
 {"Poppy: bloom the sugar orchard","Finish the flower pattern up left.","Poppy: Blossoms as sweet as a smile!",1,150,185},
 {"Milo: find missing little Sprinkle","Remember the stars at the top right.","Sprinkle: I knew you would find me!",3,475,190},
 {"Ember: warm the welcome cocoa","Warm three cups by the picnic spot.","Sprinkle: Let's visit Moon Meadow!",2,460,345}},
 {{"Ember: light the moon beacon","Warm the beacon at the top right.","Ember: A little warmth in the night!",2,475,180},
 {"Luna: build the starlight walkway","Connect a rainbow at the lower marker.","Luna: A path made of wishes!",0,320,345},
 {"Poppy: open the moonflowers","Complete the flower pattern up left.","Poppy: Moonflowers love kind friends!",1,150,195},
 {"Milo: start the sky celebration","Remember the stars at the final marker.","Everyone: You made the whole sky shine!",3,320,190}}
};
void defaults(Save *s){memset(s,0,sizeof *s);s->who=3;s->x=320;s->y=355;s->sound=1;}
int valid_save(const Save *s){
 if(s->island<0||s->island>=6||s->who<0||s->who>3||!isfinite(s->x)||!isfinite(s->y)||s->x<65||s->x>575||s->y<155||s->y>381||s->stars<0||s->stars>460||s->owned<0||s->owned>254||(s->owned&1)||s->furniture<0||s->furniture>63||s->sound<0||s->sound>1)return 0;
 if(s->companion<0||s->companion>2||s->petlook<0||s->petlook>=LOOKS||(s->petlook&&!(s->owned&(1<<s->petlook)))||s->adventures<0||s->adventures>31||s->secrets<0||s->secrets>31||s->homebits<0||s->homebits>3||s->display<0||s->display>5||(s->display&&!(s->secrets&(1<<(s->display-1))))||s->visitors<0||s->visitors>31||s->voice<0||s->voice>1)return 0;
 for(int i=0;i<5;i++)if(s->progress[i]<0||s->progress[i]>caps[i])return 0;
 for(int i=0;i<4;i++)if(s->look[i]<0||s->look[i]>=LOOKS||(s->look[i]&&!(s->owned&(1<<s->look[i]))))return 0;
 int used=0;for(int i=0;i<6;i++){int n=s->slots[i];if(n<0||n>6)return 0;if(n){if(!(s->furniture&(1<<(n-1)))||(used&(1<<n)))return 0;used|=1<<n;}}
 return 1;
}
int write_save(const char *path,const Save *s){
 if(!valid_save(s))return 0;
 char tmp[512];if(snprintf(tmp,sizeof tmp,"%s.tmp",path)>=(int)sizeof tmp)return 0;
 FILE*f=fopen(tmp,"w");if(!f)return 0;
 int ok=fprintf(f,"CW4 %d %d %.2f %.2f %d %d %d %d %d %d %d %d",s->island,s->who,s->x,s->y,s->stars,s->owned,s->look[0],s->look[1],s->look[2],s->look[3],s->furniture,s->sound)>0;
 for(int i=0;i<5;i++)if(fprintf(f," %d",s->progress[i])<0)ok=0;
 for(int i=0;i<6;i++)if(fprintf(f," %d",s->slots[i])<0)ok=0;
 if(fprintf(f," %d %d %d %d %d %d %d %d",s->companion,s->petlook,s->adventures,s->secrets,s->homebits,s->display,s->visitors,s->voice)<0)ok=0;
 if(fputc('\n',f)==EOF||fflush(f)||fsync(fileno(f)))ok=0;
 if(fclose(f))ok=0;
 if(!ok){remove(tmp);return 0;}return rename(tmp,path)==0;
}
int read_save(const char *path,Save *s){
 Save n;defaults(&n);char tag[8];FILE*f=fopen(path,"r");if(!f)return 0;
 int ok=fscanf(f,"%7s",tag)==1;
 if(ok&&(!strcmp(tag,"CW3")||!strcmp(tag,"CW4"))){
  ok=fscanf(f,"%d %d %f %f %d %d %d %d %d %d %d %d",&n.island,&n.who,&n.x,&n.y,&n.stars,&n.owned,&n.look[0],&n.look[1],&n.look[2],&n.look[3],&n.furniture,&n.sound)==12;
  for(int i=0;i<5;i++)if(fscanf(f,"%d",&n.progress[i])!=1)ok=0;
  for(int i=0;i<6;i++)if(fscanf(f,"%d",&n.slots[i])!=1)ok=0;
  if(!strcmp(tag,"CW4")&&fscanf(f,"%d %d %d %d %d %d %d %d",&n.companion,&n.petlook,&n.adventures,&n.secrets,&n.homebits,&n.display,&n.visitors,&n.voice)!=8)ok=0;
 }else if(ok&&(!strcmp(tag,"CW1")||!strcmp(tag,"CW2"))){
  ok=fscanf(f,"%d %d %f %f",&n.progress[0],&n.who,&n.x,&n.y)==4;
  if(!strcmp(tag,"CW1")){if(n.progress[0]>=0&&n.progress[0]<=7)n.stars=n.progress[0]*10;else ok=0;}
  else if(fscanf(f,"%d %d %d %d %d %d",&n.stars,&n.owned,&n.look[0],&n.look[1],&n.look[2],&n.look[3])!=6)ok=0;
 }else ok=0;
 fclose(f);if(ok&&valid_save(&n)){*s=n;return 1;}return 0;
}
void say(const char *s){snprintf(toast,sizeof toast,"%s",s);toast_time=4.5f;}
int save_game(void){mkdir("saves",0755);save_ok=write_save(SAVE_PATH,&g);if(!save_ok)say("Save failed. Check the SD card's free space.");return save_ok;}
int unlocked(int island){if(island==0||island==5)return 1;if(island==1)return g.progress[0]>=4;if(island>1&&island<5)return g.progress[island-1]==4;return 0;}
const Quest*quest(void){if(g.island>=5||g.progress[g.island]>=caps[g.island])return NULL;return &quests[g.island][g.progress[g.island]];}
int can_move(float x,float y){if(x<65||x>575||y<155||y>381)return 0;if(g.island==0&&y>240&&y<278)return g.progress[0]>=2&&x>275&&x<365;return 1;}
void complete_quest(void){
 const Quest*q=quest();if(!q)return;
 Save before=g;g.progress[g.island]++;int reward=g.island?15:10;g.stars+=reward;
 if(!save_game()){g=before;mode=PLAY;return;}
 mode=PLAY;reaction_time=3;chime(2);char text[100];snprintf(text,sizeof text,"%s +%d stars!",q->thanks,reward);say(text);
}
static int transaction(Save before){if(save_game()){chime(1);return 1;}g=before;snprintf(shop_note,sizeof shop_note,"Save failed. No stars were spent.");return 0;}
int buy_look(int item){
 if(item<0||item>=LOOKS)return 0;
 Save before=g;
 if(item&&!(g.owned&(1<<item))){if(g.stars<prices[item]){snprintf(shop_note,sizeof shop_note,"Need %d more stars. Help friends!",prices[item]-g.stars);chime(0);return 0;}g.stars-=prices[item];g.owned|=1<<item;}
 g.look[g.who]=item;if(!transaction(before))return 0;
 snprintf(shop_note,sizeof shop_note,"%s loves the %s!",names[g.who],looks[item]);reaction_time=2;return 1;
}
int buy_furniture(int item){
 if(item<0||item>=6)return 0;
 if(g.furniture&(1<<item)){snprintf(shop_note,sizeof shop_note,"Owned! Place it at Cloud Home.");return 1;}
 if(g.stars<20){snprintf(shop_note,sizeof shop_note,"Need %d more stars. Help friends!",20-g.stars);return 0;}
 Save before=g;g.stars-=20;g.furniture|=1<<item;
 if(!transaction(before))return 0;
 snprintf(shop_note,sizeof shop_note,"Yours! SELECT map takes you home.");return 1;
}
int place_furniture(int slot,int item){
 if(slot<0||slot>5||item<0||item>6||(item&&!(g.furniture&(1<<(item-1)))))return 0;
 Save before=g;
 if(item)for(int i=0;i<6;i++)if(g.slots[i]==item)g.slots[i]=0;
 g.slots[slot]=item;if(!transaction(before))return 0;
 mode=PLAY;say(item?"A cozy new spot! Your home is saved.":"Room for something new!");return 1;
}
int travel(int island){
 if(!unlocked(island)){say("Help the friends on the previous island.");return 0;}
 Save before=g;g.island=island;g.x=320;g.y=375;
 if(!transaction(before))return 0;
 mode=PLAY;reaction_time=1;if(island<5&&(g.adventures&(1<<island))){say(island==0?"Pip: Welcome back! Our cloud sheep is happy.":island==3?"Bonbon: I love my bow! Pip saved you a picnic.":island==1?"Your flowers wave hello! Thank you for the rain.":"Welcome back! Our moonlight still shines.");return 1;}say(island==5?"A near a circle decorates. X opens the shop.":"Follow the glowing marker. Y gives a hint.");return 1;
}
void begin_spell(int who){
 memset(&spell,0,sizeof spell);spell.type=who;mode=PUZZLE;
 int stage=g.island==5?0:g.progress[g.island];
 static const int routes[3][6]={{6,3,4,1,2,5},{0,1,4,3,6,7},{8,5,4,1,0,3}};
 spell.length=who==0?6:4;
 for(int i=0;i<spell.length;i++)spell.path[i]=who==0?routes[(stage+g.island)%3][i]:(i*3+stage+g.island)%4;
 spell.cursor=who==0?spell.path[0]:0;spell.reveal=who==3?3.6f:0;
 snprintf(spell.note,sizeof spell.note,"Take your time. Magic grows with practice.");
}
void interact(void){
 if(g.island<5&&hypotf(g.x-540,g.y-365)<42){start_activity();return;}
 if(g.island<5&&hypotf(g.x-85,g.y-180)<38){mode=SECRET;return;}
 if(g.island==5){int best=-1;float distance=65;for(int i=0;i<6;i++){float d=hypotf(g.x-slot_x[i],g.y-slot_y[i]);if(d<distance){distance=d;best=i;}}
  if(best>=0){decor_slot=best;choice=g.slots[best];mode=DECOR;}else say("Stand near a circle and press A to decorate.");return;}
 if(fabsf(g.x-85)<24&&fabsf(g.y-300)<24){mode=SHOP;return;}
 const Quest*q=quest();if(!q){reaction_time=2;say(g.island==4?"Everyone: Thank you for our magical sky!":"Your friends are happy! SELECT visits islands.");chime(1);return;}
 if(fabsf(g.x-q->x)>52||fabsf(g.y-q->y)>45){say(q->hint);return;}
 if(q->who>=0&&q->who!=g.who){char s[90];snprintf(s,sizeof s,"%s can help! Switch friends with L/R.",names[q->who]);say(s);return;}
 if(q->who<0)complete_quest();else begin_spell(q->who);
}
/* Poppy grows three repeating patterns; shapes and names also identify colors. */
static const int patterns[3][6]={{0,1,0,1,0,1},{2,2,3,2,2,3},{0,2,1,0,2,1}};
int pattern_value(int round,int index){return patterns[round%3][index%6];}
void spell_key(SDLKey k){
 int a=k==SDLK_SPACE,b=k==SDLK_LCTRL;
 if(b){if(spell.type==0&&spell.count){spell.count--;spell.cursor=spell.path[spell.count];}else mode=PLAY;return;}
 if(k==SDLK_LALT){if(spell.type==3){spell.reveal=3.6f;spell.count=0;}else if(spell.type==0)spell.cursor=spell.path[spell.count];else if(spell.type==1)spell.cursor=patterns[spell.round][5];return;}
 if(spell.type==2){if(a){spell.phase=1;spell.heat=0;}return;}
 if(spell.type==3&&spell.reveal>0)return;
 if(spell.type==0){int x=spell.cursor%3,y=spell.cursor/3;if(k==SDLK_LEFT&&x>0)spell.cursor--;if(k==SDLK_RIGHT&&x<2)spell.cursor++;if(k==SDLK_UP&&y>0)spell.cursor-=3;if(k==SDLK_DOWN&&y<2)spell.cursor+=3;}
 else {if(k==SDLK_LEFT||k==SDLK_UP)spell.cursor=(spell.cursor+3)%4;if(k==SDLK_RIGHT||k==SDLK_DOWN)spell.cursor=(spell.cursor+1)%4;}
 if(!a)return;
 int correct=spell.type==1?patterns[spell.round][5]:spell.path[spell.count];
 if(spell.cursor==correct){chime(1);spell.count++;snprintf(spell.note,sizeof spell.note,"Lovely! Keep going.");
  if(spell.type==1){spell.round++;spell.count=0;if(spell.round==3){complete_quest();return;}}
  else if(spell.count==spell.length){complete_quest();return;}
 }else {chime(0);snprintf(spell.note,sizeof spell.note,"Try another. Y gives a gentle hint.");}
}
void handle_key(SDLKey k){
 idle_help=0;
 if(mode==ADVENTURES||mode==ACTIVITY||mode==SECRET){adventure_key(k);return;}
 int a=k==SDLK_SPACE,b=k==SDLK_LCTRL;
 if(mode==HELP){if(a||b||k==SDLK_RETURN)mode=PAUSE;return;}
 if(k==SDLK_RETURN||k==SDLK_ESCAPE){if(mode==PAUSE)mode=resume_mode;else{resume_mode=mode;mode=PAUSE;pause_choice=0;}return;}
 if(mode==PAUSE){if(k==SDLK_UP)pause_choice=(pause_choice+3)%4;if(k==SDLK_DOWN)pause_choice=(pause_choice+1)%4;if(b)mode=resume_mode;if(a){if(pause_choice==0)mode=resume_mode;else if(pause_choice==1)mode=HELP;else if(pause_choice==2){Save before=g;g.sound=!g.sound;if(!save_game())g=before;audio_enabled(g.sound);}else if(save_game())running=0;}return;}
 if(mode==PUZZLE){spell_key(k);return;}
 if(mode==MAP){if(b||k==SDLK_RCTRL){mode=PLAY;return;}if(k==SDLK_UP)map_choice=(map_choice+5)%6;if(k==SDLK_DOWN)map_choice=(map_choice+1)%6;if(a&&!travel(map_choice))snprintf(shop_note,sizeof shop_note,"Finish the previous island to unlock.");return;}
 if(mode==DECOR){if(b){mode=PLAY;return;}if(k==SDLK_UP)choice=(choice+6)%7;if(k==SDLK_DOWN)choice=(choice+1)%7;if(a&&!place_furniture(decor_slot,choice))say("Buy this decoration in the shop first.");return;}
 if(mode==SHOP){if(b||k==SDLK_LSHIFT){mode=PLAY;return;}
  if(k==SDLK_LEFT||k==SDLK_RIGHT){tab=1-tab;choice=0;snprintf(shop_note,sizeof shop_note,tab?"Furniture costs 20 stars. Place it at home.":"One purchase unlocks a look for all friends.");}
  int total=tab?6:8;if(k==SDLK_UP)choice=(choice+total-1)%total;if(k==SDLK_DOWN)choice=(choice+1)%total;
  if(k==SDLK_e||k==SDLK_TAB)g.who=(g.who+3)%4;
 if(k==SDLK_t||k==SDLK_BACKSPACE)g.who=(g.who+1)%4;
  if(a){if(tab)buy_furniture(choice);else buy_look(choice);}return;
 }
 if(b){mode=ADVENTURES;adventure_choice=0;return;}
 if(k==SDLK_RCTRL){mode=MAP;map_choice=g.island;shop_note[0]=0;return;}
 if(k==SDLK_LSHIFT){mode=SHOP;choice=0;tab=0;snprintf(shop_note,sizeof shop_note,"One purchase unlocks a look for all friends.");return;}
 if(k==SDLK_e||k==SDLK_TAB)g.who=(g.who+3)%4;
 if(k==SDLK_t||k==SDLK_BACKSPACE)g.who=(g.who+1)%4;
 if(k==SDLK_LALT){speak_hint(0);const Quest*q=quest();say(q?q->hint:g.island==5?"A decorates a nearby circle. X buys furniture.":"SELECT opens the map. There is more to explore!");}
 if(a)interact();
}
void tick(float dt,const Uint8 *keys){
 clock_time+=dt;
 if(mode==ACTIVITY){idle_help+=dt;if(idle_help>20){idle_help=0;adventure_hint();}}if(toast_time>0)toast_time-=dt;if(reaction_time>0)reaction_time-=dt;moving=0;
 if(mode==PUZZLE){if(spell.reveal>0)spell.reveal-=dt;
  if(spell.type==2&&spell.phase){spell.heat+=dt*.42f;if(!keys[SDLK_SPACE]){
   spell.phase=0;if(spell.heat>=.48f&&spell.heat<=.78f){spell.count++;chime(1);snprintf(spell.note,sizeof spell.note,"Just right! A cozy little glow.");if(spell.count==3)complete_quest();}
   else{chime(0);snprintf(spell.note,sizeof spell.note,"Try again: release A inside the gold band.");}
  }else if(spell.heat>1.05f){spell.phase=0;spell.heat=0;snprintf(spell.note,sizeof spell.note,"A little cooler! Try another gentle glow.");}
  }return;
 }
 if(mode!=PLAY)return;
 float dx=keys[SDLK_RIGHT]-keys[SDLK_LEFT],dy=keys[SDLK_DOWN]-keys[SDLK_UP];moving=dx||dy;
 if(dx&&dy){dx*=.7071f;dy*=.7071f;}float x=g.x+dx*125*dt,y=g.y+dy*125*dt;if(can_move(x,g.y))g.x=x;if(can_move(g.x,y))g.y=y;
}
