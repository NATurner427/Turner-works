#!/usr/bin/env python
# coding: utf-8

# ___
# 
# <a href='http://www.pieriandata.com'> <img src='../../Pierian_Data_Logo.png' /></a>
# ___

# # SF Salaries Exercise 
# 
# Welcome to a quick exercise for you to practice your pandas skills! We will be using the [SF Salaries Dataset](https://www.kaggle.com/kaggle/sf-salaries) from Kaggle! Just follow along and complete the tasks outlined in bold below. The tasks will get harder and harder as you go along.

# ** Import pandas as pd.**

# In[1]:


import pandas as pd


# ** Read Salaries.csv as a dataframe called sal.**

# In[2]:


sal=pd.read_csv('Salaries.csv')


# ** Check the head of the DataFrame. **

# In[5]:


sal.head()


# In[8]:





# ** Use the .info() method to find out how many entries there are.**

# In[7]:


sal.info()


# In[9]:





# **What is the average BasePay ?**

# In[9]:


sal


# In[15]:


sal['BasePay'].mean()


# ** What is the highest amount of OvertimePay in the dataset ? **

# In[17]:


sal['OvertimePay'].max()


# ** What is the job title of  JOSEPH DRISCOLL ? Note: Use all caps, otherwise you may get an answer that doesn't match up (there is also a lowercase Joseph Driscoll). **

# In[29]:


sal[sal['EmployeeName'] == 'JOSEPH DRISCOLL']['JobTitle']


# ** How much does JOSEPH DRISCOLL make (including benefits)? **

# In[13]:





# In[31]:


sal[sal['EmployeeName'] == 'JOSEPH DRISCOLL']['TotalPayBenefits']


# ** What is the name of highest paid person (including benefits)?**

# In[36]:


sal[sal['TotalPayBenefits']==sal['TotalPayBenefits'].max()]['EmployeeName']


# In[14]:





# ** What is the name of lowest paid person (including benefits)? Do you notice something strange about how much he or she is paid?**

# In[41]:


sal[sal['TotalPayBenefits']==sal['TotalPayBenefits'].min()]['EmployeeName']


# In[15]:





# ** What was the average (mean) BasePay of all employees per year? (2011-2014) ? **

# In[12]:


sal.groupby('Year').mean()['BasePay']


# In[16]:





# ** How many unique job titles are there? **

# In[13]:


sal


# In[18]:


sal['JobTitle'].nunique()


# In[17]:





# ** What are the top 5 most common jobs? **

# In[25]:


sal['JobTitle'].value_counts().head(5)


# ** How many Job Titles were represented by only one person in 2013? (e.g. Job Titles with only one occurence in 2013?) **

# In[36]:


sum(sal[sal['Year']==2013] ['JobTitle'].value_counts()==1)


# ** How many people have the word Chief in their job title? (This is pretty tricky) **

# In[39]:


def chief(name):
    if 'chief' in name.lower():
        return True
    else:
        return False


# In[41]:


sum(sal['JobTitle'].apply(lambda x: chief(x)))


# In[21]:





# ** Bonus: Is there a correlation between length of the Job Title string and Salary? **

# In[22]:





# In[23]:





# # Great Job!
