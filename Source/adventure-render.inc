static void adventure_scenery(void){
 if(g.island<5){disk(540,365,15,255,227,143);star(540,365,9,255,251,216);text(475,383,"ADVENTURE");disk(85,180,13,189,146,231);star(85,180,8,255,242,174);text(52,197,"SECRET");
  if(g.adventures&(1<<g.island)){if(g.island==0){friend_at(205,330,0,1);text(170,347,"SHEEP");}if(g.island==1)for(int i=0;i<4;i++)plant(230+i*40,210,i);if(g.island==2||g.island==4)segment(130,175,490,220,255,240,170);if(g.island==3){furnishing(5,365,210);friend_at(415,215,0,1);text(325,235,"PIP'S PICNIC");wear(1,525,g.progress[3]>=caps[3]?285:170,50);}}
 }else{
  if(g.homebits&1){for(int i=0;i<6;i++)if(g.slots[i]==5){plant(slot_x[i]-20,slot_y[i]-28,0);text(slot_x[i]-45,slot_y[i]+25,"PICNIC");}}
  if(g.display){star(560,165,14,255,223,120);text(270,150,treasure_names[g.display-1]);}
  for(int i=0;i<5;i++)if(g.visitors&(1<<i)){int slot=i%6;friend_at(slot_x[slot]+40,slot_y[slot],i%4,1);if(g.slots[slot])text(slot_x[slot]-35,slot_y[slot]+35,g.slots[slot]==1?"SWEET DREAMS":g.slots[slot]==5?"TEA TIME":"SO COZY!");}
 }
 if(!(g.homebits&2)||g.island==5){int x=(int)g.x-44,y=(int)g.y+5;if(g.homebits&2){for(int i=0;i<6;i++)if(g.slots[i]==1){x=slot_x[i];y=slot_y[i];}text(x+24,y-35,"Zzz");}creature(pet_who[g.companion],x,y,g.petlook,1);}
}
static void arrow(int x,int y,int direction,int gold){const char*labels[]={"UP","RIGHT","DOWN","LEFT"};box(x-34,y-20,72,42,gold?235:123,gold?207:114,160);text(x-30,y-10,labels[direction]);}
static void adventure_panel(void){
 if(mode==ADVENTURES){panel("COMPANIONS & LITTLE ADVENTURES");char line[90];const char*labels[]={pet_names[g.companion],"Island adventure","Explore secret room",looks[g.petlook],g.homebits&1?"Pack away picnic":"Arrange a picnic",g.homebits&2?"Wake companion":"Tuck companion into bed",g.display?treasure_names[g.display-1]:"Display a treasure",g.visitors?"Send visitors home":"Invite rescued friends",g.voice?"Spoken hints: on":"Spoken hints: off"};for(int i=0;i<9;i++){snprintf(line,sizeof line,i==3?"%c Pet look: %s":"%c %s",i==adventure_choice?'>':' ',labels[i]);text(36,125+i*24,line);}text(36,377,"Up/down choose   A use/change   B back");return;}
 if(mode==SECRET){panel(secret_names[g.island]);int found=g.secrets&(1<<g.island);creature(pet_who[g.companion],320,235,g.petlook,1);if(found){star(430,185,23,255,233,149);text(45,260,treasure_names[g.island]);text(45,292,"Your treasure can be displayed at home.");}else{char line[90];snprintf(line,sizeof line,"This doorway needs the %s.",pet_names[secret_power[g.island]]);wrapped(40,258,line,40);text(40,310,"A companion magic   B choose companion");}return;}
 panel(activity_names[g.island]);
 if(activity.type==0){const int route[]={0,1,2,7,12,13,14,19,24};int next=-1;for(int i=0;i<8;i++)if(route[i]==activity.sheep)next=route[i+1];for(int n=0;n<25;n++){int path=0;for(int i=0;i<9;i++)if(route[i]==n)path=1;int x=205+(n%5)*44,y=142+(n/5)*37;box(x,y,39,32,path?236:111,path?219:104,path?180:146);if(activity.demo&&n==next)star(x+20,y+15,12,255,245,145);if(n==activity.sheep){for(int wool=0;wool<5;wool++)disk(x+10+wool*5,y+13+(wool%2)*4,6,255,252,237);friend_at(x+20,y+30,0,0);}if(n==24)text(x+1,y+5,"H");}text(38,335,"D-pad guides sheep to H. X pet helps.");}
 else if(activity.type==3){for(int i=0;i<4;i++){int x=130+i*125,y=225+(activity.demo?0:(int)(sin(clock_time*2+i)*20));if(i==spell.cursor)disk(x,y,39,226,193,139);if(activity.caught&(1<<i))text(x-26,y,"SAFE");else furnishing(i==0?5:i==1?6:i==2?3:2,x,y);}text(38,310,"Left/right choose   A catch   X slow");}
 else{const int rain[]={1,2,1},crystal[]={1,3,2};const int*goal=activity.type==1?rain:crystal;for(int i=0;i<3;i++){int x=165+i*155;arrow(x,163,goal[i],0);arrow(x,239,activity.values[i],activity.values[i]==goal[i]||(activity.demo&&i==spell.cursor));if(i==spell.cursor)text(x-6,275,"^");if(activity.values[i]==goal[i])star(x,307,12,255,236,163);}text(36,335,"Left/right choose   A turn   X pet help");}
 text(38,375,"Y shows how   B back   No timers or lives");
}
