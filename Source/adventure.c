#include "game.h"
Activity activity;
int adventure_choice=0;
float idle_help=0;
const int pet_who[]={2,3,1},secret_power[]={0,2,0,1,1},activity_rewards[]={3,7,4,1,2};
const char *pet_names[]={"Baby dragon","Treasure bunny","Garden fairy"};
const char *secret_names[]={"Waterfall hideaway","Garden above the clouds","Singing crystal cave","Sugar-cloud pantry","Moonlit wishing room"};
const char *treasure_names[]={"River pearl","Sky seed","Singing gem","Picnic keepsake","Moon pearl"};
const char *activity_names[]={"Guide the cloud sheep home","Rain for the thirsty garden","Reflect the moonlight","Catch Pip's picnic supplies","Reflect the palace moonlight"};
static const int route[]={0,1,2,7,12,13,14,19,24};
int sheep_step(int at,int direction){int delta[]={-1,1,-5,5};if(direction<0||direction>3)return at;int next=at+delta[direction];if(next<0||next>24||abs(next%5-at%5)+abs(next/5-at/5)!=1)return at;for(int i=0;i<9;i++)if(route[i]==next)return next;return at;}
static int persist(Save before){if(save_game())return 1;g=before;return 0;}
void start_activity(void){if(g.island>=5)return;memset(&activity,0,sizeof activity);activity.type=g.island;spell.cursor=0;toast_time=0;mode=ACTIVITY;idle_help=0;speak_hint(1+g.island);}
void finish_activity(void){int bit=1<<g.island;Save before=g;int first=!(g.adventures&bit);g.adventures|=bit;if(first){g.stars+=20;g.owned|=1<<activity_rewards[g.island];}if(!persist(before))return;mode=PLAY;reaction_time=4;chime(2);say(first?"Thank you! +20 stars and a themed outfit!":"Your friends loved playing again!");}
int adventure_discover(void){if(g.island>=5)return 0;int bit=1<<g.island;if(g.secrets&bit){say("Your special place is always here for you.");return 1;}if(g.companion!=secret_power[g.island]){char line[100];snprintf(line,sizeof line,"Try the %s. B returns to companions.",pet_names[secret_power[g.island]]);say(line);speak_hint(7);return 0;}Save before=g;g.secrets|=bit;g.stars+=10;if(!persist(before))return 0;reaction_time=4;chime(2);say("A secret treasure and 10 stars! Display it at home.");return 1;}
int adventure_change(int action){Save before=g;
 if(action==0){g.companion=(g.companion+1)%3;g.homebits&=~2;}
 else if(action==3){int start=g.petlook;do{g.petlook=(g.petlook+1)%LOOKS;}while(g.petlook&&!(g.owned&(1<<g.petlook))&&g.petlook!=start);}
 else if(action==8)g.voice=!g.voice;
 else{if(g.island!=5){say("Visit Cloud Home using SELECT first.");return 0;}
  if(action==4||action==5){int needed=action==4?5:1,placed=0;for(int i=0;i<6;i++)if(g.slots[i]==needed)placed=1;if(!placed){say(action==4?"Place a tea table at home first.":"Place a cloud bed at home first.");return 0;}g.homebits^=action==4?1:2;}
  if(action==6){if(!g.secrets){say("Explore secret rooms to find treasures.");return 0;}do{g.display=(g.display+1)%6;}while(g.display&&!(g.secrets&(1<<(g.display-1))));}
  if(action==7){int rescued=0;for(int i=0;i<5;i++)if(g.progress[i]==caps[i])rescued|=1<<i;if(!rescued){say("Help island friends before inviting them.");return 0;}g.visitors=g.visitors?0:rescued;}
 }
 if(!persist(before))return 0;
 chime(1);say("Saved! Your friends are ready to play.");if(action==8)speak_hint(0);return 1;
}
void adventure_hint(void){activity.hints++;if(activity.hints>1)activity.demo=1;const char*line=activity.type==0?(activity.hints>1?"Follow the gold square using the D-pad.":"Guide the sheep along the pale path."):activity.type==3?"D-pad choose a supply; A catches it. No rush.":activity.hints>1?"Match each big arrow to the arrow above it.":"Left/right choose. A turns the piece.";say(line);speak_hint(activity.hints>1?6:1+activity.type);}
void adventure_key(SDLKey k){int a=k==SDLK_SPACE,b=k==SDLK_LCTRL;
 if(b||k==SDLK_RETURN||k==SDLK_ESCAPE){mode=mode==ADVENTURES?PLAY:ADVENTURES;return;}
 if(mode==ADVENTURES){if(k==SDLK_UP)adventure_choice=(adventure_choice+8)%9;if(k==SDLK_DOWN)adventure_choice=(adventure_choice+1)%9;if(a){if(adventure_choice==1){if(g.island<5)start_activity();else say("Visit an adventure island using SELECT.");}else if(adventure_choice==2){if(g.island<5)mode=SECRET;else say("Every adventure island has a secret room.");}else adventure_change(adventure_choice);}return;}
 if(mode==SECRET){if(a)adventure_discover();if(k==SDLK_LALT){say("Use the companion named by the doorway.");speak_hint(7);}return;}
 if(k==SDLK_LALT){adventure_hint();return;}
 if(k==SDLK_LSHIFT){activity.demo=1;say(g.companion==0?"Your dragon warms a lantern. The safe way glows!":g.companion==1?"Your bunny finds the next hidden clue!":"Your fairy blooms flowers by the next target!");chime(1);return;}
 if(activity.type==0){int direction=k==SDLK_LEFT?0:k==SDLK_RIGHT?1:k==SDLK_UP?2:k==SDLK_DOWN?3:-1;activity.sheep=sheep_step(activity.sheep,direction);if(activity.sheep==24)finish_activity();return;}
 int total=activity.type==3?4:3;if(k==SDLK_LEFT)spell.cursor=(spell.cursor+total-1)%total;if(k==SDLK_RIGHT)spell.cursor=(spell.cursor+1)%total;spell.cursor%=total;
 if(a){if(activity.type==3){activity.caught|=1<<spell.cursor;chime(1);if(activity.caught==15)finish_activity();}else{activity.values[spell.cursor]=(activity.values[spell.cursor]+1)%4;const int rain[]={1,2,1},crystal[]={1,3,2};const int*goal=activity.type==1?rain:crystal;if(activity.values[0]==goal[0]&&activity.values[1]==goal[1]&&activity.values[2]==goal[2])finish_activity();}}
}
