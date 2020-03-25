# -*- coding: utf-8 -*-
"""
Created on Fri Feb 14 15:03:12 2020

@author: 16142
"""

import wave
import contextlib
from os import path
from pydub import AudioSegment
t1=0
t2=0
t1=t1*10000
t2=t2*10000

newAudio=AudioSegment.from_wav('2_HOU_at_TEX_3.30.18.wav')
newAudio=newAudio[t1:t2]
newAudio.export('segment.wav',format='wav')