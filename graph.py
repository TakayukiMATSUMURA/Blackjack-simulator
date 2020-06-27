#!/usr/bin/env python
#-*- coding: utf-8 -*-
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import scipy
from scipy.stats import norm
from scipy.optimize import curve_fit

data = pd.read_csv('./data/bankroll_distribution.csv', names=['x', 'y'])
total = sum(data['y'])
#plt.bar(data['x'], data['y'], width=0.9)

# ガウシアンフィッティング

hist = []
for i in range(len(data['y'])):
    for j in range(data['y'][i]):
        hist.append(data['x'][i])
param = norm.fit(hist)
print(param)
x = np.linspace(min(data['x']), max(data['x']),len(data['x']))
pdf_fitted = norm.pdf(x,loc=param[0], scale=param[1])
label = "approx N($\mu$=" + "{0:.2f}".format(param[0]) + ", " + "$\sigma$={0:.2f}".format(param[1]) + ")"
plt.plot(x, pdf_fitted, 'r-', label=label)
plt.hist(hist, weights=np.ones(len(hist))/float(len(hist)), bins=max(data['x']) - min(data['x'])+1)
plt.legend()
plt.show()
