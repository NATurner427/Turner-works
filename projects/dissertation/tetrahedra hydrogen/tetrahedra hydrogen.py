#!/usr/bin/env python
# coding: utf-8

# In[278]:


import math

#constants: the angle to rotate to, its inverse, and the length of the hydrogen
t=2.186
u=-2.186
hlen=.9053

#inintialize lists
bond1=[]
bond2=[]
perp=[]
mid=[]
rot=[]
unity=[]
hydro1=[]
hydro2=[]

#open file that has cartesian coordinates of atoms we will be positioning hydrogen about
#columns are [0] the atom then [1],[2],[3] its x,y,z coordinates, 
#followed by the [4],[5],[6] and [7], [8], [9] x,y,z coordinates of the two atoms this atom is bonded to.
file1=open('PtIhydro.txt') 

#this file collects the calculated hydrogen positions
file2=open('hydro.txt','w')

line= file1.readline()
while line:
    sp=line.split()

    #calculate bond vectors
    bond1.append(float(sp[4])-float(sp[1]))
    bond1.append(float(sp[5])-float(sp[2]))
    bond1.append(float(sp[6])-float(sp[3]))

    bond2.append(float(sp[7])-float(sp[1]))
    bond2.append(float(sp[8])-float(sp[2]))
    bond2.append(float(sp[9])-float(sp[3]))

    #vector that is perpendicular to the 2 bond vectors
    perp.append(bond1[1]*bond2[2]-bond2[1]*bond1[2])
    perp.append(bond2[0]*bond1[2]-bond1[0]*bond2[2])
    perp.append(bond1[0]*bond2[1]-bond2[0]*bond1[1])

    #calculate midvector
    mid.append(bond1[0]+bond2[0])
    mid.append(bond1[1]+bond2[1])
    mid.append(bond1[2]+bond2[2])

    #vector that is perpendicular to the midvector and the perpendicular vector. 
    #This will be the vector that is rotated about
    rot.append(mid[1]*perp[2]-perp[1]*mid[2])
    rot.append(perp[0]*mid[2]-mid[0]*perp[2])
    rot.append(mid[0]*perp[1]-perp[0]*mid[1])

    #find the magnitude of the rotational vector and make its mag unity
    rotmag=(rot[0]*rot[0] + rot[1]*rot[1] + rot[2]*rot[2])**.5
    unity.append(rot[0]/rotmag)
    unity.append(rot[1]/rotmag)
    unity.append(rot[2]/rotmag)

    #check unity is unity!
    #unitymag=(unity[0]*unity[0] + unity[1]*unity[1] + unity[2]*unity[2])**.5
    #print(unitymag)

    #Rotational equation given the normalized rotational vector and an angle to rotate to, get a vector.
    #angle is in radians 0 angle relates to the midvector
    #1st hydrogen
    hydro1.append((math.cos(t) + unity[0]*unity[0]*(1-math.cos(t)))*mid[0]
                  + (unity[0]*unity[1]*(1-math.cos(t))-unity[2]*math.sin(t))*mid[1] 
                  + (unity[0]*unity[2]*(1-math.cos(t))+unity[1]*math.sin(t))*mid[2])

    hydro1.append((unity[1]*unity[0]*(1-math.cos(t)) + unity[2]*math.sin(t))*mid[0]
                  + (math.cos(t) + unity[1]*unity[1]*(1-math.cos(t))*mid[1])
                  + (unity[1]*unity[2]*(1-math.cos(t)) - unity[0]*math.sin(t))*mid[2])

    hydro1.append((unity[2]*unity[0]*(1-math.cos(t)) - unity[1]*math.sin(t))*mid[0] 
                  + (unity[2]*unity[1]*(1-math.cos(t)) - unity[0]*math.sin(t))*mid[1]
                  + (math.cos(t)+unity[2]*unity[2]*(1-math.cos(t)))*mid[2])

    #2nd hydrogen
    hydro2.append((math.cos(u) + unity[0]*unity[0]*(1-math.cos(u)))*mid[0]
                  + (unity[0]*unity[1]*(1-math.cos(u))-unity[2]*math.sin(u))*mid[1] 
                  + (unity[0]*unity[2]*(1-math.cos(u))+unity[1]*math.sin(u))*mid[2])

    hydro2.append((unity[1]*unity[0]*(1-math.cos(u)) + unity[2]*math.sin(u))*mid[0]
                  + (math.cos(u) + unity[1]*unity[1]*(1-math.cos(u))*mid[1])
                  + (unity[1]*unity[2]*(1-math.cos(u)) - unity[0]*math.sin(u))*mid[2])

    hydro2.append((unity[2]*unity[0]*(1-math.cos(u)) - unity[1]*math.sin(u))*mid[0] 
                  + (unity[2]*unity[1]*(1-math.cos(u)) - unity[0]*math.sin(u))*mid[1]
                  + (math.cos(u)+unity[2]*unity[2]*(1-math.cos(u)))*mid[2])

    #readjust magnitude of the hydrogen vectors to match length of known hydrogen bonds in similar materials
    hydro1mag=(hydro1[0]*hydro1[0] + hydro1[1]*hydro1[1] + hydro1[2]*hydro1[2])**.5
    hydro2mag=(hydro2[0]*hydro2[0] + hydro2[1]*hydro2[1] + hydro2[2]*hydro2[2])**.5

    hydro1[0]=(hlen/hydro1mag)*hydro1[0]
    hydro1[1]=(hlen/hydro1mag)*hydro1[1]
    hydro1[2]=(hlen/hydro1mag)*hydro1[2]

    hydro2[0]=(hlen/hydro2mag)*hydro2[0]
    hydro2[1]=(hlen/hydro2mag)*hydro2[1]
    hydro2[2]=(hlen/hydro2mag)*hydro2[2]

    #check that hydrogens are correct length
    #hydro1mag=(hydro1[0]*hydro1[0] + hydro1[1]*hydro1[1] + hydro1[2]*hydro1[2])**.5
    #hydro2mag=(hydro2[0]*hydro2[0] + hydro2[1]*hydro2[1] + hydro2[2]*hydro2[2])**.5
    #print(hydro1mag)
    #print(hydro2mag)

    #last step: these hydrogen bonds are oriented about the origin, need to translate them to where they are in the atomic structure

    hydro1[0]=hydro1[0]+float(sp[1])
    hydro1[1]=hydro1[1]+float(sp[2])
    hydro1[2]=hydro1[2]+float(sp[3])
    hydro2[0]=hydro2[0]+float(sp[1])
    hydro2[1]=hydro2[1]+float(sp[2])
    hydro2[2]=hydro2[2]+float(sp[3])

    strhy1=' '.join(str(x) for x in hydro1)
    strhy2=' '.join(str(x) for x in hydro2)

    file2.write(strhy1)
    file2.write('\r')
    file2.write(strhy2)
    file2.write('\r')

    #reinitialize lists
    bond1=[]
    bond2=[]
    perp=[]
    mid=[]
    rot=[]
    unity=[]
    hydro1=[]
    hydro2=[]
    
    line=file1.readline()
    
file1.close()
file2.close()


# In[ ]:





# In[ ]:




