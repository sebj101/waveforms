#!/usr/bin/env python
# Reads in a line from a data file and plots the graph of it

from ROOT import TGraph

filename  = 'data.txt'
file = open(filename, "r")
firstline = file.readline()
nWords = 0

dataPoints =[] # list to store data points in 

# Loops over all the words in the first line
for word in firstline.split():
    # Appends data points to list
    dataPoints.append(word)
    print(word)
    nWords += 1

sum = dataPoints[3] + dataPoints[4]
print("Sum of two points = " + str(sum))
