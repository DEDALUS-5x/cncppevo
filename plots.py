# The purpose of this code is to plot easily the feedrate function
# evolution over time. Created for debugging and developing the lookahead algorithm

import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

log = pd.read_csv("log.csv")

n = log['n']
t = log['t_tot']
feedrate = log['feedrate']

plt.figure()
plt.plot(t, feedrate, 'b-', linewidth=1.5, label='Feedrate')

dn = np.concatenate([[True], np.diff(n) != 0])
plt.plot(t[dn], feedrate[dn], 'ro', markerfacecolor='r', label='New Point')

plt.xlabel('Time [s]')
plt.ylabel('Feedrate [mm/min]')
plt.title('Feedrate as a function of time')
plt.grid(True)
plt.legend()

plt.show()
