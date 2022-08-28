import numpy
from typing import *
import struct
import os
from PIL import Image

READ_AHEAD_BUFFER_SIZE = 0x00000012
SLIDING_WINDOW_SIZE = 0x00001000

def split8x8(arr: numpy.ndarray):
    h, w = arr.shape
    return arr.reshape(h // 8, 8, -1, 8).swapaxes(1, 2).reshape(-1, 8, 8)

def to_gba(arr: numpy.ndarray):
    arr = arr.astype('<u1')
    buffer = split8x8(arr).flatten()
    buffer = (buffer[1::2] << 4) + buffer[::2]
    return buffer

def palette_to_bytes(palette: Collection[Tuple[int, int, int]]):
    buffer = b""
    f = lambda x: (x >> 3) & 0x1f
    for r, g, b in palette:
        dr, dg, db = f(r), f(g), f(b)
        v = dr + (dg << 5) + (db << 10)
        buffer += struct.pack("<H", v)
    return buffer

def get_palette(image_file, size: int = 16):
    image = Image.open(image_file)
    palette = image.palette.colors
    palette = [i for i in image.palette.colors][:size]
    return palette_to_bytes(palette)

def get_bytes(image_file):
    image = Image.open(image_file)
    w, h = image.size
    arr = numpy.array(image.getdata(), dtype='<u1')
    arr = arr.reshape(h, w)
    return to_gba(arr).tobytes()
