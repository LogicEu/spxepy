#!/usr/local/bin/python3.10

import spxepy as spxe

pixbuf = spxe.start('spxepy', 800, 600, 80, 60)
width, height = spxe.screenSize()

while spxe.run(pixbuf):
    spxe.clear(pixbuf, 125, 125, 225, 255)
    mx, my = spxe.mousePos()
    b = 255 if spxe.mouseDown(0) else  0
    if mx >= 0 and mx < width and my >= 0 and my < height:
        spxe.set(pixbuf, mx, my, 255 - b, 0, b, 255)
    if spxe.keyPressed(256):
        break

spxe.end()

