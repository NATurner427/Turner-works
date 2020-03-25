# -*- coding: utf-8 -*-
"""
Created on Fri Oct 18 13:46:32 2019

@author: 16142
"""

wave=2

import math
import numpy as np
import matplotlib.pyplot as plt
import random
from matplotlib import gridspec

#setting up figure for display purposes
fig = plt.figure(figsize=(6,8.1))
#x = plt.axes(xlim=(-np.pi, np.pi), ylim=((-1.1), (1.1)))

#readjust axes

def do_axes():
    axe=plt.gca()
    axe.spines['right'].set_color('none')
    axe.spines['top'].set_color('none')
    axe.xaxis.set_ticks_position('bottom')
    axe.spines['bottom'].set_position(('data',0))
    axe.yaxis.set_ticks_position('left')
    axe.spines['left'].set_position(('data',0))
    
superpos=0
sine=0

# time points
t = np.linspace(-np.pi,np.pi,200, endpoint = True)

# sine wave variables: amplitude, wave number, frequency,phase 
amp=1
wavenum=1
freq=1
phase=0

# how many terms?
n=0
terms = 20

gs =gridspec.GridSpec(terms+1, 1)

     
sl= np.zeros((terms,4))

# Sine wave series, can set _0 terms and in the loop can create regressions to build constants
if wave == 0:
    while n < terms:
        sl[n]= [amp,wavenum,freq,phase]
        ind=n
        # y points
        sine=(sl[ind,0] * np.sin( -(sl[ind,2] * t + sl[ind,3])))
        superpos+=sine
        
        ax = fig.add_subplot(gs[n])
        ax.plot(t,sine)
        
        do_axes()
        
        n +=1
        amp +=1
        wavenum +=1
        freq +=1
        phase +=1
        
    if terms >=2:    
        ax = fig.add_subplot(gs[ind+1])
        ax.plot(t,superpos,color='r')
        do_axes()
        fig.tight_layout()
    

#random generated sine waves
elif wave == 1:
    amp=random.random()
    wavenum=random.randrange(0,20)
    freq=random.randrange(0,20)
    phase=random.random()*2*np.pi
       
    while n < terms:
        sl[n]= [amp,wavenum,freq,phase]
        ind=n
        
        # y points
        sine=sl[ind,0] * (np.sin( -(sl[ind,2] * t + sl[ind,3])))
        superpos+=sine
        #plt.plot(t,sine,linewidth=2)
        
        ax = fig.add_subplot(gs[n])
        ax.plot(t,sine)
        
        do_axes()
        
        n +=1
        wavenum=random.randrange(0,100)
        freq=random.randrange(0,100)
        phase=random.random()*2*np.pi
        
    if terms >=2:    
        ax = fig.add_subplot(gs[ind+1])
        ax.plot(t,superpos,color='r')
        do_axes()
        fig.tight_layout()
        
# square wave to n terms
elif wave == 2:
    #Fourier expansion of square wave:
    while n < terms:
        sl[n]=[amp,wavenum,freq,phase]
        ind=n
        k=n+1
        square=((4/np.pi)*((np.sin((2*k-1)*freq*t))/(2*k-1)))
        superpos+=square
        
        # = fig.add_subplot(gs[ind])
      #  ax.plot(t,square)
        
        do_axes()
        
        n +=1
    
    if terms >= 2:
        #ax = fig.add_subplot(gs[ind+1])
        #ax.plot(t,superpos,color='r')
        do_axes()
        plt.plot(t,superpos,linewidth=2)
        fig.tight_layout()
    
    
    
    
    
    
    
    
    