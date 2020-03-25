#!/usr/bin/env python
# coding: utf-8

# In[113]:


import math
import numpy as np
from sklearn.preprocessing import normalize

hlen=.9053

#function to find bond vectors from atomic positions
def bond(x,y,z):
    n=0
    while n<3:
        y[n]=x[i,n+z]-x[i,n]
        n+=1
    return y

#function that builds rotation matrix to find hydrogen positions
def rotate(theta,V,vec):
    M=np.zeros((3,3))
    V2=np.square(V) 
    M[0,0] = (V2[0]+(V2[1]+V2[2])*math.cos(theta))
    M[1,1] = (V2[1]+(V2[0]+V2[2])*math.cos(theta))
    M[2,2] = (V2[2]+(V2[0]+V2[1])*math.cos(theta))
    
    M[0,1] = (V[0]*V[1]*(1-math.cos(theta))-V[2]*math.sin(theta))
    M[0,2] = (V[0]*V[2]*(1-math.cos(theta))+V[1]*math.sin(theta))
    M[1,2] = (V[1]*V[2]*(1-math.cos(theta))-V[0]*math.sin(theta))
    M[1,0] = (V[1]*V[0]*(1-math.cos(theta))+V[2]*math.sin(theta))
    M[2,0] = (V[2]*V[0]*(1-math.cos(theta))-V[1]*math.sin(theta))
    M[2,1] = (V[2]*V[1]*(1-math.cos(theta))+V[0]*math.sin(theta))
    
    A=np.matmul(M,vec)
    return A


#access data
i=0
atoms=np.genfromtxt('PtIhydro.txt',usecols=0,dtype=str)
cart=np.genfromtxt('PtIhydro.txt',dtype=float)[:,1:]

#find number of rows for interation purposes
count=np.size(cart,0)
#initiate array to collect results
Hs=np.zeros((count*2,3))

while i<count:

    #calculate bond vectors
    bond1=bond(cart,np.zeros((3,1)),3)
    bond2=bond(cart,np.zeros((3,1)),6)

    #calculate perpendicular vector by taking cross product
    perp=np.cross(bond1,bond2,axis=0)

    #calculate midvector
    mid=bond1+bond2

    # caclculate vector that is perpendicular to perp and mid vectors and normalize it
    #This will be the vector rotated about
    rot=np.cross(mid,perp,axis=0)
    rot=normalize(rot, norm='l2',axis=0)
    rs=np.square(rot)
    
    #rotate midvector about the rotation vector to find hydrogen bond vectors 
    #then adjust them to be length of typical h bonds in similar materials
    
    Hydro1=rotate(2.186,rot,mid)
    Hydro1=normalize(Hydro1, norm='l2',axis=0)*hlen
    Hydro1=Hydro1.transpose()
    Hydro2=rotate(-2.186,rot,mid)
    Hydro2=normalize(Hydro2, norm='l2',axis=0)*hlen
    Hydro2=Hydro2.transpose()
    Hydro1=Hydro1+cart[i,:3]
    Hydro2=Hydro2+cart[i,:3]

    Hs[i*2]=Hydro1
    Hs[(i*2)+1]=Hydro2
    i+=1
    
np.savetxt('np_hydro.txt',Hs)


# In[ ]:




