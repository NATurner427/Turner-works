# -*- coding: utf-8 -*-
"""
Created on Sat Oct 26 15:01:19 2019

@author: 16142
"""

# sieve of Eratostehenses: Find primes up to integer n

# create a list of consecutive integers 2 through n
#choose n
x=0
p=2
n=200 #set this to your limit
rangelist=list(range(p,n+1))
# prime variable switches to true when a prime is found;
primeflag=False
nextprimeflag=False
nextprime=0
primelist=[]
#primer
while p <= n:
    for x in rangelist:
        if x%p==0:
            rangelist.remove(x)
            primeflag=True
        elif x%p != 0 and nextprimeflag==False:
            nextprime=p
            print(p)
            nextprimeflag=True
        else:
            pass
    if primeflag == True:
        primelist.append(p)
    primeflag=False
    x=nextprime
    nextprimeflag=False
    p+=1

print(primelist)
