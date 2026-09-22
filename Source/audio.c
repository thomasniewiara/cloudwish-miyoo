#include "game.h"
/* Original synthesized music; no external audio assets. All shared audio
   state is changed under SDL's audio lock after the device is opened. */
static Uint8 *voice_data=NULL;
static Uint32 voice_length=0,voice_position=0;
static int ready=0,enabled=1,effect=0;
static unsigned long sample_clock=0;
static float tone_phase=0,effect_phase=0;
static int effect_left=0;
static const float melody[]={261.63f,329.63f,392.f,523.25f,440.f,392.f,329.63f,293.66f,261.63f,392.f,440.f,523.25f,587.33f,523.25f,392.f,329.63f};
void audio_callback(void*unused,Uint8*stream,int len){
 (void)unused;Sint16*out=(Sint16*)stream;int samples=len/sizeof(Sint16);memset(stream,0,len);
 for(int i=0;i<samples;i++){
  if(voice_position+1<voice_length){Sint16 sample;memcpy(&sample,voice_data+voice_position,2);voice_position+=2;out[i]=sample;continue;}
  if(!enabled)continue;
  int note=(sample_clock/11025)%16;float env=1.f-(sample_clock%11025)/11025.f;
  tone_phase+=melody[note]/22050.f;if(tone_phase>=1)tone_phase-=1;
  float tri=4*fabsf(tone_phase-.5f)-1;
  float value=tri*env*900;
  if(effect_left>0){float hz=effect==2?(effect_left>4400?523.25f:783.99f):effect==1?659.25f:293.66f;effect_phase+=hz/22050.f;if(effect_phase>=1)effect_phase-=1;value+=(4*fabsf(effect_phase-.5f)-1)*(effect_left/8800.f)*1600;effect_left--;}
  out[i]=(Sint16)value;sample_clock++;
 }
}
int init_audio(void){
 if(SDL_InitSubSystem(SDL_INIT_AUDIO)<0){fprintf(stderr,"Audio unavailable: %s\n",SDL_GetError());return 0;}
 SDL_AudioSpec want;memset(&want,0,sizeof want);want.freq=22050;want.format=AUDIO_S16SYS;want.channels=1;want.samples=512;want.callback=audio_callback;
 if(SDL_OpenAudio(&want,NULL)<0){fprintf(stderr,"Audio unavailable: %s\n",SDL_GetError());return 0;}
 ready=1;enabled=g.sound;SDL_PauseAudio(0);fprintf(stderr,"Audio: 22050 Hz, mono, signed 16-bit\n");return 1;
}
void audio_enabled(int on){if(ready)SDL_LockAudio();enabled=on;if(ready)SDL_UnlockAudio();}
void chime(int kind){if(ready)SDL_LockAudio();effect=kind;effect_left=8800;effect_phase=0;if(ready)SDL_UnlockAudio();}
void speak_hint(int id){
 if(!ready)return;
 if(!g.voice){SDL_LockAudio();voice_position=voice_length;SDL_UnlockAudio();return;}
 if(id<0||id>7)return;
 char path[80];snprintf(path,sizeof path,"assets/hint%d.wav",id);SDL_AudioSpec spec;Uint8*data=NULL;Uint32 length=0;if(!SDL_LoadWAV(path,&spec,&data,&length))return;
 if(spec.freq!=22050||spec.format!=AUDIO_S16SYS||spec.channels!=1){SDL_FreeWAV(data);return;}
 SDL_LockAudio();if(voice_data)SDL_FreeWAV(voice_data);voice_data=data;voice_length=length;voice_position=0;SDL_UnlockAudio();
}
void close_audio(void){if(ready){SDL_CloseAudio();ready=0;if(voice_data)SDL_FreeWAV(voice_data);voice_data=NULL;voice_length=voice_position=0;}}
