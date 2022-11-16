import pandas as pd 
import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit
import os

fileDirectory = os.path.dirname(__file__)

#rename str_cor_long.txt to Gt_qX_YYf.txt and put the file here
inputFile = os.path.join(fileDirectory, "Gt_q1_25f.txt")

data = pd.read_csv(inputFile, sep="\t", header = None)

expFit, expCov = curve_fit(lambda t, C, k: C * np.exp(k * t), data[0] * 0.005 , data[1], p0=(40, -1))

visc = -expFit[0] /expFit[1]

print("Viscosity = ", visc)

#plt.scatter(data[0] * 0.005, data[1])
#plt.plot(data[0] * 0.005, expFit[0] * np.exp(expFit[1] * data[0] * 0.005))
#plt.show()