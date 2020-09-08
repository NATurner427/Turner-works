# Collected Projects

Here I've collected my various programming projects from my time at WSU and after. 

## Dissertation Projects

My dissertation projects were written originally in C and I rewrote them in Python recently. Both are available here

-a 1-d mass spring model that calculates eigenmodes and eigenvectors

-a routine that given two legs of a tetrahedra, finds the orientation of the the other 2 legs in cartesian coordinates

-a routine that takes raw beamline data collected at SSRL and processes it to create a 2-d Resonant Inelastic X-ray Scattering intensity map

## Data science projects

Since my graudation I've learned and practiced projects in python3 with the focus on modeling, data visualization, and machine learning

Many of the techniques here use clean data from online sources like haggle or provided by python packages like seaborn or scikit-learn.
Techniques practiced were linear regressions, logistic regressions, K nearest neighbors, K means clustering, Support vector machines as well as trees and random forests.

I've also practiced working with 'dirty' real world data in various projects.

I did time series analysis using The Prophet on my brothers stock portfolio to predict the future performance of his choices (before the pandemic...)

I webscraped Carvanna's Columbus Ohio listings and performed linear regression to predict the pricing of a car and attempted some K means clustering on pricing and milage to label the year.

I collected US airline flights and performed exploratory data analysis and predicted delay times using machine learning.

I want to use Natural Language Processing to analyze my music taste. I've been using Google Play Music for close to 10 years and have all my songs, playlists, and listening info on the app. I've extracted that info using an unoffical API and saved it to a csv file. I'm currently pulling lyrics from the 18k+ songs in my library using Selenium and beautiful soup to webscrape from Genius.com. Once this is done I'm looking forward to making my own website where I will create a dashboard analyzing the text of these songs.

There is a suspicion that The Houston Astros cheated using whistles to alert batters of the pitch being thrown in previous years of Major League Baseball. Every game of the 2018 season is available on Youtube. I'll pull the pitch count and audio from these games and then using a routine similar to previously published work on birdcalls, find correlation between whistles, pitches, and outcomes. I used webscraping, computer vision, data visualization, Fourier transforms and tkinter to do this. 

## Anxious Modeling

Since the outbreak of the Coronavirus, I've found myself stressfully working on modeling the outbreak of Ohio. When our understanding was very preliminary, I learned and implemented the Sucesptible, Infectious, Recovered, (SIR) model to forecast the outbreak. 

I also, after seeing it done in the New York Times, wanted to model the interaction of people in a 2-d box getting each other infected. As a stepping stone, I modelled a gas in 2-d.

## Current and Future projects

Currently considering my next project. 

I've finished analyzing the astros home games for the 2018 season for the whistling project and it was tiring. I think as a palette cleanser I want to do something more mainstream and practical before I return to the Astros and see about training an AI to find whistles for me. I'm considering credit card fraud or ad clicking or customer churn. Unlike my last 3 projects where I culled my own data from the internet this project will likely be using a prepared data set from kaggle or elsewhere.

A second project percalating in my mind is simply a fun challenge focusing on baseball physics. It would be fun to create a pitch modeler; set the spin rate, spin axis, speed and see the flight path. Use a 3-d modeling package to visualize it. Make it as complex as I want with atmospheric and weather effects.

Finally, more long term, two areas I want to focus on learning more that seem highly saught after are hadoop/spark and tableau/power BI. I'm going to want to do a project that incorporates these or use an old project and add these skills to them.
