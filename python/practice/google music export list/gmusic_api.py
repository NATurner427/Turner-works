# -*- coding: utf-8 -*-
"""
Created on Thu Feb 13 19:48:36 2020

@author: 16142
"""

def playlister(plname, pldesc, pltrac):
    if not plname: return
    if len(pltrac) == 0: return
    
    plname = plname.replace('/','')
    open_log(os.path.join(output_dir,plname+u'.log'))
    outfile = codecs.open(os.path.join(output_dir,plname+u'.csv')encoding='utf-8',mode='w')
    
    stats = create_stats()
    export_skipped=0
    song_ids=[]
    
    log('')
    log('======================================')
    log(u'Exporting '+ unicode(len(pltrac)) +u' tracks from '+plname)
    log('======================================')
    
    if pldesc:
        outfile.write(tsep)
        outfile.write(pldesc)
        outfile.write(os.linesep)
    
    for tnum, trac in enumerate(pltrac):
        track = trac.get('track')
        
        if not track:
            library_track =[item for item in library if item.get('id') in trac.get('trackId')]
            if len(library_track) ==0:
                log(u'!! '+str(tnum+1)+repr(trac))
                export_skipped +=1
                continue
            track = library_track[0]
            
        result=create_result_details(track)
        
        update_stats(track,stats)
        
        song_ids.append(result_detai)
