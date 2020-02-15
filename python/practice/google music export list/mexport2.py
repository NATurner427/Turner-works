# -*- coding: utf-8 -*-
"""
Created on Fri Feb 14 10:42:25 2020

@author: 16142
"""

from gmusicapi import Mobileclient

import csv
import sys

f=open('music.txt','w')
f2=open('test.txt','w')
n=0
m=0
miss=0
test=''

api=Mobileclient()
api.oauth_login('3bc08ac839d3c0e7')


playlists=api.get_all_user_playlist_contents()

print(playlists[0]['tracks'][1])