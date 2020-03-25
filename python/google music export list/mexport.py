# -*- coding: utf-8 -*-
"""
Spyder Editor

This is a temporary script file.
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


f2.write(test)
f2.close()

for pl in playlists:
    plname = pl.get('name')
    pltrack = pl.get('tracks')
    n+=1

    print(plname)
    print('\r')
    
    for tr in pltrack:
        track=tr.get('track')
        trackId=tr.get('trackId')
        m+=1 
        if track != None:
            title=track.get('title')
            artist=track.get('artist')
            album=track.get('album')
            year=track.get('year')
            genre=track.get('genre')
            tracknum=track.get('trackNumber')
            duration=track.get('durationMillis')
            
            entry='%s \t %s \t %s \t %s \t %s \t %s \t %s \t %s \t %s \t %s \r' %(title,artist,album,year,genre,tracknum,duration,trackId,plname,n) 
            print(entry)
            f.write(entry)
        else:
            miss+=1
            entry='missing track %s from playlist %s  \r' %(m,n)
            print(entry)
            f.write(entry)
    m=0    
        
        
            
print('\r')
print(miss)
f.close()