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



if len(sys.argv) != 3:
	print "usage: python plot_dist.py dir1 dir2"
	sys.exit(1)

path1 = sys.argv[1]
path2 = sys.argv[2]


fig = plt.figure()

ax1 = fig.add_subplot(111)

ax1.set_title("Model comparison")    
ax1.set_xlabel('Scale')
ax1.set_ylabel('Value')

lw = 2.0

fc = '#B0B0B0'


addToPlot(path1, ax1, '#F08030', 'test1', 2.0,'#F0B090')
addToPlot(path2, ax1, '#3080F0', 'test2', 2.0,'#90B0F0')

#ax1.set_xlim([0,12])

leg = ax1.legend()
plt.savefig('compare.eps', format='eps', dpi=1000)
plt.savefig('compare.png', format='png', dpi=1000)

plt.show()