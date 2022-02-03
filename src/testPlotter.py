import matplotlib.pyplot as plt
import numpy as np
import argparse

from numpy import double

'''
x = [0, 1, 1, 2, 2, 3, 3, 4]
y = values
#y2 = [0, 0.2181, 0.2567, 0.2877, 0.4586, 0.6546]
plt.plot(x, y, label='sequenziel', marker='x')  # Plot some data on the (implicit) axes.
#plt.plot(x, y2, label='parallel', marker='x')  # etc.
#plt.plot(x, x**3, label='cubic')
#plt.axis([0, 2.00, 0, 8.00])
plt.grid(True)
plt.xlabel('Schritte der Ausführung')
plt.ylabel('Zeit in ms')
plt.title("Example Plot")
plt.legend()
# save plot as svg
plt.savefig('test_plot.svg')
plt.clf()
'''
seqMS = 0
parMS = 0

names = ['sequenziel',  'std::par']
heights = [seqMS, parMS]
plt.bar(names, heights)
plt.ylabel('Zeit in ms')
plt.title("Execution Time")
plt.savefig('test_plot_bar.svg')


