# Collected Projects

Here I've collected my various programming projects from my time at WSU and after. 

My dissertation projects were written originally in C and I rewrote them in Python recently. Both are available here
Those routines are

## Dissertation Projects
-a 1-d mass spring model that calculates eigenmodes and eigenvectors

-a routine that given two legs of a tetrahedra, finds the orientation of the the other 2 legs in cartesian coordinates

-a routine that takes raw beamline data collected at SSRL and processes to create a 2-d Resonant Inelastic X-ray Scattering intensity map

## Data science projects

Since my graudation I've learned and practiced projects in python3 with the focus on modeling and machine learning

Many of the techniques here use clean data from online sources like haggle or provided by python packages like seaborn or scikit-learn.
Techniques practiced were linear regressions, logistic regressions, K nearest neighbors, K means clustering, Support vector machines as well as trees and random forests.

I've also practiced working with 'dirty' real world data in various projects.

I did time series analysis using The Prophet on my brothers stock portfolio to predict the future performance of his choices (before the pandemic...)

I webscraped Carvanna's Columbus Ohio listings and performed linear regression to predict the pricing of a car and plan to perform K means clustering to predict the model of the car.

I collected US airline flights and performed exploratory data analysis and predicted delay times using machine learning.

## Anxious Modeling

Since the outbreak of the Coronavirus, I've found myself stressfully working on modeling the outbreak of Ohio. When our understanding was very preliminary, I learned and implement a program that uses the Sucesptible, Infectious, Recovered, (SIR) model to forecast the outbreak. 

I also after seeing it done in the New York Times, wanted to model the interaction of people in a 2-d box getting each other infected. As a stepping stone, I modelled a gas in 2-d.

## Current and Future projects

I have 2 projects I am currently working on.

I want to use Natural Language Processing to analyze my music playlist. I've been using Google Play Music for close to 10 years and have all my songs playlists and listening info on the app. I've extracted that info using an unoffical API and saved it to a csv file.

The other project, which I consider my personal capstone, is to use Convolutional Neural Networks to parse whistles from crowd noise. There is a suspition that The Houston Astros cheated using whistles to alert batters of the pitch being thrown in previous years of Major League Baseball. Every game of the 2018 season is available on Youtube. I'll pull the pitch count and audio from these games and then using a routine similar to previously published work on birdcalls, find correlation between whistles, pitches, and outcomes. I will also use the Cleveland Indians, a team not accused of cheating, as a baseline.