import numpy as np
import matplotlib.pyplot as plt


dir = "../build-NanoPores-Desktop_Qt_5_5_1_clang_64bit-Release/NanoPores/NanoPores.app/Contents/MacOS/"

def load(name):
	with open(dir + name) as f:
	    data = f.read()

	data = data.split('\n')
	while '' in data:
	    data.remove('')

	d = [[],[]]

	d[0] = [row.split(' ')[0] for row in data]
	d[1]= [row.split(' ')[1] for row in data]
	return d

model = load("dta_model.txt")
data = load("dta_data.txt")


fig = plt.figure()

ax1 = fig.add_subplot(111)

ax1.set_title("Plot title...")    
ax1.set_xlabel('your x label..')
ax1.set_ylabel('your y label...')

ax1.plot(data[0],data[1], c='r', label='data')
ax1.plot(model[0],model[1], c='b', label='model')

leg = ax1.legend()

plt.show()