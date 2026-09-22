import ctypes,wave,audioop
from pathlib import Path
out=Path(__file__).resolve().parent.parent/'Roms/PORTS/Games/Cloudwish/assets'
lib=ctypes.CDLL('libflite.so.1',mode=ctypes.RTLD_GLOBAL) if Path('/usr/lib/x86_64-linux-gnu/libflite.so.1').exists() else ctypes.CDLL('/usr/lib/x86_64-linux-gnu/libflite.so.2.2',mode=ctypes.RTLD_GLOBAL)
lib.flite_init()
voice=ctypes.CDLL('/usr/lib/x86_64-linux-gnu/libflite_cmu_us_slt.so.2.2')
voice.register_cmu_us_slt.argtypes=[ctypes.c_char_p];voice.register_cmu_us_slt.restype=ctypes.c_void_p
v=voice.register_cmu_us_slt(None)
lib.flite_text_to_speech.argtypes=[ctypes.c_char_p,ctypes.c_void_p,ctypes.c_char_p];lib.flite_text_to_speech.restype=ctypes.c_float
lines=['Follow the glowing target. Press B to choose a companion or an adventure.','Use the direction buttons to guide the sheep along the pale path to its home.','Turn the rain channels. Match each direction to the little direction above it.','Turn the crystals to reflect moonlight. Match each direction to the direction above it.','Choose a floating picnic supply with left and right. Press A to catch it. Take your time.','Turn the crystals to fill the room with moonlight. Match the directions.','Look at the glowing target. Follow the next gold square, or match the directions above the pieces.','Try another companion. The baby dragon warms things, the fairy grows flowers, and the bunny finds hidden treasures.']
for i,line in enumerate(lines):
 tmp=Path('/tmp/hint-source.wav');lib.flite_text_to_speech(line.encode(),v,str(tmp).encode())
 with wave.open(str(tmp),'rb') as w:rate=w.getframerate();data=w.readframes(w.getnframes());channels=w.getnchannels();width=w.getsampwidth()
 if channels==2:data=audioop.tomono(data,width,.5,.5)
 if width!=2:data=audioop.lin2lin(data,width,2)
 data=audioop.ratecv(data,2,1,rate,22050,None)[0]
 with wave.open(str(out/f'hint{i}.wav'),'wb') as w:w.setnchannels(1);w.setsampwidth(2);w.setframerate(22050);w.writeframes(data)
 print(i,len(data))
