#!/usr/bin/env python
from ROOT import TGraph

filename  = 'dataWave.txt'
file = open(filename, "r")
firstline = file.readline()
nWords = 0

for word in firstline.split():
    print(word)
    nWords += 1
