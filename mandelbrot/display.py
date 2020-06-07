# Visualises the output from the OpenCL Mandelbrot code
import numpy as np
import matplotlib.pyplot as plt

#read in the file
f=open("out.dat","rb")

#read the number of points in the x and y direction
nxy = np.fromfile(f,np.int32,2)
nx = nxy[0]
ny = nxy[1]

print("(Nx,Ny) = (%d,%d)"%(nx,ny))

#read in the arrays defining x and y
x = np.fromfile(f,np.float32,nx)
y = np.fromfile(f,np.float32,ny)

#read in the image data and converd to a 2d array
img = np.fromfile(f,np.int32,nx*ny).reshape((nx,ny))

#display
plt.imshow(img,extent=(x[0],x[-1],y[0],y[-1]),origin="lower")
plt.show()
