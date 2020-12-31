import numpy as np
import matplotlib.pyplot as plt

targetRPM=[]
t=[]
measuredRPM=[]
out=[]
pwmWidth=[]

for line in open('output.txt', 'r'):
	values = [float(s) for s in line.split(',')]
	t.append(values[0])
	measuredRPM.append(values[1])
	out.append(values[2])
	targetRPM.append(values[3])
	pwmWidth.append(values[4])


plt.plot(t,measuredRPM)
plt.plot(t,out) 
plt.plot(t,targetRPM)
plt.plot(t,pwmWidth)
plt.legend(["measuredRPM", "PID output", "targetRPM","pwmWidth"]) 
plt.show()