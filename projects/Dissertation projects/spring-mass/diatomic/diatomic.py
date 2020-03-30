# -*- coding: utf-8 -*-
"""
Created on Thu Feb 20 15:09:58 2020

@author: 16142
"""

import numpy as np
from numpy import linalg as LA
import matplotlib.pyplot as plt
import scipy.constants as sc

#number of atoms in calculation
order=120

i=0
#number of primitive cells
N=order/2

#distance between atom pair in Angstroms
a=5.4275
distance=a/2

#Mass in Kg
Ptmass=3.2394457E-25
MassX=5.8871086E-26
#Force constant in Newtons per meter
K1=166

#Temperature in Kelvin
T=293

#stepsize for k vector
step=((sc.pi)/(order*distance))

#construct reduced mass of pair of atoms
MassA=(MassX)
MassB=(Ptmass)

muAB=(1/((1/MassA)+(1/MassB)))

#construct pre-factor
prefactor=(sc.hbar/(2*N*muAB))
#construct massfactor for eigenvectors
MassA=(muAB/MassA)**.5
MassB=(muAB/MassB)**.5

#Construct mass matrix a order by order identity matrix scaled by mass
Mass1=(1/MassX)**.5
Mass2=(1/Ptmass)**.5
mass=np.identity(order)*Mass1

for index,x in np.ndenumerate(mass):
    if index[0]==index[1] and index[0]%2 != 0:
        mass[index]=Mass2

#Construct force constant matrix. a order by order matrix

force=np.identity(order)*K1*2

for index,x in np.ndenumerate(force):
    if index[0]==index[1]-1 or index[1]==index[0]-1:
        force[index]=K1*-1
    elif index[0]==0 and index[1]==order-1:
        force[index]=K1*-1
    elif index[1]==0 and index[0]==order-1:
         force[index]=K1*-1


#create dynamic matrix
D=np.matmul(force,mass)
D=np.matmul(mass,D)
eigen,vect=LA.eig(D)

wave=np.vstack((abs(eigen)**.5)/((sc.c*100)*(sc.pi)*2))

kvec=np.zeros((order,1),dtype=float)

for index,x in np.ndenumerate(kvec):
    if index[0]==0:
        pass
    elif index[0]%2==1:
        kvec[index]=index[0]*step
    else:
        kvec[index]=(index[0]-1)*step*-1

mode=np.concatenate((kvec,np.sort(wave,axis=0)),axis =1)

omega=abs(eigen)**.5

arg=((omega*(sc.hbar))/(2*(sc.k)*T))
arg=(1/np.tanh(arg))
arg=arg/omega


atomA=vect[0]*MassA

atomB=vect[1]*np.cos((kvec.transpose())*distance)*MassB

pair=((abs(atomB-atomA))**2)*arg

DW=np.sum(pair[0,1:])*prefactor*10E20
print(DW)

mode1,mode2=np.vsplit(mode,2)
mode1=mode1[mode1[:,0].argsort()]
mode2=mode2[mode2[:,0].argsort()]

plt.plot(mode1[:,0],mode1[:,1],'rs',mode2[:,0],mode2[:,1],'bs')
plt.show()

np.savetxt('eigenvalue.txt',eigen)
np.savetxt('mode.txt',mode)