#!/usr/bin/env python
# coding: utf-8

# ___
# 
# <a href='http://www.pieriandata.com'> <img src='../../Pierian_Data_Logo.png' /></a>
# ___
# # Ecommerce Purchases Exercise
# 
# In this Exercise you will be given some Fake Data about some purchases done through Amazon! Just go ahead and follow the directions and try your best to answer the questions and complete the tasks. Feel free to reference the solutions. Most of the tasks can be solved in different ways. For the most part, the questions get progressively harder.
# 
# Please excuse anything that doesn't make "Real-World" sense in the dataframe, all the data is fake and made-up.
# 
# Also note that all of these questions can be answered with one line of code.
# ____
# ** Import pandas and read in the Ecommerce Purchases csv file and set it to a DataFrame called ecom. **

# In[1]:


import numpy as np
import pandas as pd


# In[5]:


ecom=pd.read_csv('Ecommerce Purchases')


# **Check the head of the DataFrame.**

# In[6]:


ecom.head()


# In[87]:


ecom.head()


# ** How many rows and columns are there? **

# In[10]:


ecom.info()


# In[88]:





# ** What is the average Purchase Price? **

# In[13]:


ecom['Purchase Price'].mean()


# In[90]:





# ** What were the highest and lowest purchase prices? **

# In[16]:


ecom['Purchase Price'].max()


# In[92]:





# In[17]:


ecom['Purchase Price'].min()


# In[93]:





# ** How many people have English 'en' as their Language of choice on the website? **

# In[33]:


ecom['Language'].value_counts()['en']


# In[94]:





# ** How many people have the job title of "Lawyer" ? **
# 

# In[37]:


ecom['Job'].value_counts()['Lawyer']


# In[95]:





# ** How many people made the purchase during the AM and how many people made the purchase during PM ? **
# 
# **(Hint: Check out [value_counts()](http://pandas.pydata.org/pandas-docs/stable/generated/pandas.Series.value_counts.html) ) **

# In[39]:


ecom['AM or PM'].value_counts()


# In[96]:





# ** What are the 5 most common Job Titles? **

# In[40]:


ecom['Job'].value_counts().head(5)


# In[ ]:





# In[97]:





# ** Someone made a purchase that came from Lot: "90 WT" , what was the Purchase Price for this transaction? **

# In[41]:


ecom.head()


# In[46]:


ecom[ecom['Lot']=='90 WT']['Purchase Price']


# In[99]:





# ** What is the email of the person with the following Credit Card Number: 4926535242672853 **

# In[51]:


ecom[ecom['Credit Card'] ==4926535242672853]['Email']


# In[ ]:





# In[100]:





# ** How many people have American Express as their Credit Card Provider *and* made a purchase above $95 ?**

# In[85]:


ecom[(ecom['CC Provider']=='American Express') & (ecom['Purchase Price']>95)].shape[0]


# ** Hard: How many people have a credit card that expires in 2025? **

# In[97]:


def stringsplit(s):
    a=s.split('/')
    if a[1] == '25':
        return True
    else:
        return False


# In[102]:


ecom[ecom['CC Exp Date'].apply(lambda x: stringsplit(x))].shape[0]


# In[ ]:





# In[102]:





# ** Hard: What are the top 5 most popular email providers/hosts (e.g. gmail.com, yahoo.com, etc...) **

# In[103]:


def stringsplit2(s):
    a=s.split('@')
    return a[1]
   


# In[104]:


email='nate.turner.2788@gmail.com'

stringsplit2(email)


# In[110]:


ecom['Email'].apply(lambda x: stringsplit2(x)).value_counts().head(5)


# In[ ]:





# In[ ]:





# # Great Job!
