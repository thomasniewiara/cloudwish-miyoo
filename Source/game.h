#ifndef CLOUDWISH_H
#define CLOUDWISH_H
#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/stat.h>
#define W 640
#define H 480
#define ISLANDS 6
#define LOOKS 8
#define FURNITURE 6
#define SAVE_PATH "saves/slot1.txt"
typedef struct {int progress[5],island,who;float x,y;int stars,owned,look[4],furniture,slots[6],sound;} Save;
typedef struct {const char *title,*hint,*thanks;int who,x,y;} Quest;
typedef enum {PLAY,MAP,SHOP,DECOR,PUZZLE,PAUSE,HELP} Mode;
typedef struct {int type,cursor,count,round,phase;float heat,reveal;int path[6],length;char note[80];} Spell;
extern Save g;
extern Mode mode,resume_mode;
extern Spell spell;
extern int running,choice,tab,decor_slot,pause_choice,map_choice,save_ok;
extern float toast_time,reaction_time,clock_time,moving;
extern char toast[100],shop_note[90];
extern const char *island_names[6],*names[4],*looks[8],*furniture_names[6];
extern const int prices[8],caps[5],slot_x[6],slot_y[6];
extern SDL_Surface *canvas,*video,*present,*sprites[4],*font,*background;
void defaults(Save *s);
int valid_save(const Save *s);
int write_save(const char *path,const Save *s);
int read_save(const char *path,Save *s);
int save_game(void);
void say(const char *s);
int unlocked(int island);
const Quest *quest(void);
int can_move(float x,float y);
void complete_quest(void);
void interact(void);
void begin_spell(int who);
void spell_key(SDLKey k);
void tick(float dt,const Uint8 *keys);
void handle_key(SDLKey k);
int buy_look(int item);
int buy_furniture(int item);
int place_furniture(int slot,int item);
int travel(int island);
void render(void);
int init_video(void);
void close_video(void);
int init_audio(void);
void close_audio(void);
void audio_enabled(int on);
void chime(int kind);
void audio_callback(void *unused,Uint8 *stream,int len);
#endif
