from PIL import Image,ImageDraw,ImageFont
from pathlib import Path
import sys
out=Path(__file__).resolve().parents[1]/'Roms/PORTS/Games/Cloudwish/assets'
src=Path(sys.argv[1])
out.mkdir(parents=True,exist_ok=True)
# Assets are pre-sized, so the handheld only needs software blits.
bg=Image.new('RGB',(640,480));d=ImageDraw.Draw(bg)
for y in range(480):d.line((0,y,639,y),fill=(119+y//6,186+y//9,235))
for x,y in [(35,105),(490,90),(5,355),(505,335)]:
 d.ellipse((x,y,x+130,y+44),fill='#edf8ff');d.ellipse((x+28,y-20,x+91,y+40),fill='#edf8ff')
# Wide, readable walkable meadow, with a clearly visible crossing.
d.rounded_rectangle((45,133,595,408),radius=85,fill='#9acbbb')
d.rounded_rectangle((45,121,595,395),radius=80,fill='#d0edcb')
for x in range(75,590,38):
 for y in (145,368):d.ellipse((x,y,x+4,y+4),fill='#fcf8b7')
d.rectangle((47,246,593,270),fill='#80c2eb')
bg.save(out/'meadow.bmp')
for name in ['luna','poppy','ember','milo']:
 im=Image.open(src/(name+'.png')).convert('RGBA');im.thumbnail((76,93))
 canvas=Image.new('RGB',im.size,(255,0,255));canvas.paste(im,mask=im.getchannel('A').point(lambda a:255 if a>=128 else 0));canvas.save(out/(name+'.bmp'))
font=ImageFont.truetype('/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf',18)
atlas=Image.new('RGB',(16*14,6*25),(255,0,255));d=ImageDraw.Draw(atlas)
for i in range(96):d.text(((i%16)*14,(i//16)*25),chr(i+32),font=font,fill='#fff9ee')
# Crisp colorkey glyphs: no magenta antialiasing fringe.
for y in range(atlas.height):
 for x in range(atlas.width):
  r,g,b=atlas.getpixel((x,y));atlas.putpixel((x,y),(255,249,238) if g>110 else (255,0,255))
atlas.save(out/'font.bmp')
