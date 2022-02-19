import matplotlib.pyplot as plt
import numpy as np
import argparse

from numpy import double
PolyOvTime = 0
UpdateOvTime = 0
FlowOvTime = 0
OvTime = 0
count = 0

file = open("example.txt", "r")
for line in file.readlines():
    if line.startswith(" FarnebackP"):
        PolyOvTime = PolyOvTime + double(line.split()[1])
    elif line.startswith(" FarnebackU"):
        UpdateOvTime = UpdateOvTime + double(line.split()[1])
    elif line.startswith(" FarnebackF"):
        FlowOvTime = FlowOvTime + double(line.split()[1])
    elif line.startswith("Overall"):
        OvTime = OvTime + double(line.split()[4])
        count = count + 1

PolyAvTime = PolyOvTime / (count * 8)
UpdateAvTime = UpdateOvTime / (count * 4)
FlowAvTime = FlowOvTime / (count * 12)
AvTime = OvTime / count

PolyD = (PolyAvTime * 8) / AvTime
UpdateD = (UpdateAvTime * 4) / AvTime
FlowD = (FlowAvTime * 12) / AvTime
OtherD = (AvTime - (PolyAvTime*8 + UpdateAvTime*4 + FlowAvTime*12)) / AvTime

names = ['FarnebackPolyExp', 'FarnebackUpdateMatrices', 'FarnebackUpdateFlow_Blur']
heights = [PolyAvTime, UpdateAvTime, FlowAvTime]
labels = ['FarnebackPolyExp', 'FarnebackUpdateMatrices', 'FarnebackUpdateFlow_Blur', 'Other']
sizes = [PolyD, UpdateD, FlowD, OtherD]
plt.figure(figsize=(7, 5))
plt.bar(names, heights, width=0.5)
plt.ylabel('Zeit in ms')
plt.title("Durchschnittliche Ausführungszeit")
plt.savefig('test_plot_bar.svg')
plt.clf()
plt.figure(figsize=(8.5, 5))
plt.pie(sizes, explode=(0.1, 0, 0, 0), labels=labels, autopct='%.2f%%', shadow=True, startangle=90)
plt.axis('equal')
plt.savefig('test_plot_pie.svg')


