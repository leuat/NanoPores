import numpy as np
import sys
import os
import math
import matplotlib.pyplot as plt


dir = "../build-NanoPores-Desktop_Qt_5_5_1_clang_64bit-Release/NanoPores/NanoPores.app/Contents/MacOS/"

def load(name):
	with open(name) as f:
	    data = f.read()

	data = data.split('\n')
	while '' in data:
	    data.remove('')

	d = [[],[]]

	d[0] = [row.split(' ')[0] for row in data]
	d[0] = [float(i) for i in d[0]]
	d[1]= [row.split(' ')[1] for row in data]
	d[1] = [float(i) for i in d[1]]
	return d

def save(name, data):
	with open(name,"w") as f:
		for i in range(len(data[0])):
			f.write(str(data[0][i]) + " " + str(data[1][i]) + " " + str(data[2][i]) + "\n")

	f.close()

def getAverage(data):
	avg = np.zeros(len(data[0]))
	for d in data:
		avg += d
	avg/=len(data)
	return avg	

def getSigma(data, average):
	sigma = np.zeros(len(data[0]))
	for d in data:
		sigma += np.power(d - average,2)

	sigma = np.sqrt(sigma/len(data))
	return sigma


def addToPlot(path, ax, col, lbl, lw, col2):
	data = []
	x = []
	for filename in os.listdir(path):
		d = load(path + "/" + filename)
		data.append(d[1])
		x = d[0]

	avg = getAverage(data)
	sigma = getSigma(data, avg)

	ax.plot(x,avg, c=col, label=lbl, linewidth=lw)

	fillMin = [xx - y for xx, y in zip(avg, sigma)]
	fillMax = [xx + y for xx, y in zip(avg, sigma)]

	ax1.fill_between(x, fillMin, fillMax, facecolor=col2, edgecolor=col2, interpolate=True, alpha=0.3)



if len(sys.argv) != 2:
	print "usage: python build_model.py [ directory ]"
	sys.exit(1)

path = sys.argv[1]

def loadDirectory(path):
	data = []
	x = []
	for filename in os.listdir(path):
		d = load(path + "/" + filename)
		data.append(d[1])
		x = d[0]

	avg = getAverage(data)
	sigma = getSigma(data, avg)
	data = []
	data.append(x)
	data.append(avg)
	data.append(sigma)
	#print data
	return data


for filename in os.listdir(path):
	fn = path + filename;
	if (os.path.isdir(fn)):
		#print "loading " + filename
		data = loadDirectory(fn)
		#print data
		save(fn + ".txt", data)
	

# then, generate plots

