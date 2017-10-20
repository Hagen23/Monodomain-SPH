######################################
#                                    #
# Display results with matplotlib    # 
# Reads an output file into a numpy  # 
# array and displays scalar field.   #
# Uses python3.		                 # 
#				                     #
######################################

import numpy as np
import matplotlib.pyplot as plt
import sys

default_fileName = "Vm.out.txt"
fileName = ""

if len(sys.argv) < 2:
    fileName = default_fileName
else:
    fileName = str(sys.argv[1])

print("Processing file " + fileName)

#read file into numpy array
a=np.genfromtxt(fileName, dtype='double')
max = np.amax(a)
min = np.amin(a)
print(max)
print(min)
print(a.shape)
fig=plt.figure()
ims=[]

#step through each time-step and display image
for i in range(a.shape[0]):
    b=a[i,:].reshape((100,100)) 
    #get correct orientation with b.T (i.e. transpose)
    im=plt.imshow(b.T, interpolation='bicubic', extent=[0,1,0, 1], vmin=min, vmax=max)
    ims.append([im])
    plt.pause(0.1)
#plt.show()
