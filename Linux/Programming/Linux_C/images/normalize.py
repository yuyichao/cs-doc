#!/usr/bin/env python
import Image, glob

filelist = []
filelist += glob.glob("*.png")
filelist += glob.glob("*.PNG")
filelist += glob.glob("*.jpg")
filelist += glob.glob("*.JPG")
filelist += glob.glob("*.gif")
filelist += glob.glob("*.GIF")

for f in filelist:    
    im = Image.open(f)
    width, height = im.size
    if width > 600:
        im.resize((600, height*600/width), Image.BICUBIC).save(f)

