import numpy as np
import matplotlib.pyplot as plt

setPoint=[]
t=[]
measurement=[]
out=[]

for line in open('output.txt', 'r'):
	values = [float(s) for s in line.split(',')]
	t.append(values[0])
	measurement.append(values[1])
	out.append(values[2])

#print(len(t))

for x in range(1,len(t)+1):
	setPoint.append(500)


plt.plot(t,measurement)
plt.plot(t,out) 
plt.plot(t,setPoint)
plt.legend(["Measurement", "PID output", "Set Point"]) 
plt.show()