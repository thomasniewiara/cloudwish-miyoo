#define main game_main
#include "cloudwish.c"
#undef main
int main(void){
 char tmp[]="/tmp/cloudwish-shop-XXXXXX";assert(mkdtemp(tmp));assert(chdir(tmp)==0);SDL_Init(SDL_INIT_TIMER);
 FILE*f=fopen("old.txt","w");fputs("CW1 7 3 320 355\n",f);fclose(f);
 assert(read_save("old.txt",&g)&&g.stars==70&&g.owned==0);
 item=1;shop_action();assert(g.stars==60&&g.outfit[3]==1);
 shop_action();assert(g.stars==60);g.who=0;shop_action();assert(g.stars==60&&g.outfit[0]==1);
 item=2;shop_action();assert(g.stars==45&&g.outfit[0]==2);
 item=3;shop_action();assert(g.stars==25&&g.outfit[0]==3);
 Save loaded;assert(read_save("saves/slot1.txt",&loaded));assert(loaded.stars==25&&loaded.outfit[3]==1&&loaded.outfit[0]==3&&loaded.owned==14);
 finish();assert(g.stars==25);item=0;shop_action();assert(g.outfit[0]==0&&g.stars==25);
 g.owned=0;g.stars=0;g.outfit[3]=0;item=3;shop_action();assert(g.stars==0&&g.owned==0&&g.outfit[0]==0);
 /* A failed disk write must roll back currency, ownership and equipment. */
 remove("saves/slot1.txt");rmdir("saves");f=fopen("saves","w");fclose(f);g.stars=30;shop_action();assert(g.stars==30&&g.owned==0&&g.outfit[0]==0);
 remove("saves");remove("old.txt");assert(chdir("/tmp")==0);rmdir(tmp);SDL_Quit();puts("PASS: legacy credit, buying, shared ownership, no repeat charges/rewards, equipment persistence, affordability and failed-save rollback");return 0;
}
