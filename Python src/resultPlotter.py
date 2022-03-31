from matplotlib import pyplot as plt

ogTimeOV = 0.0
file = open("../measurements/Time_Og.txt", "r")
for line in file.readlines():
    ogTimeOV = ogTimeOV + float(line.strip("\n"))
ogTimeAV = ogTimeOV / 299

PPTimeOV = 0.0
file = open("../measurements/Time_PP.txt", "r")
for line in file.readlines():
    PPTimeOV = PPTimeOV + float(line.strip("\n"))
PPTimeAV = PPTimeOV / 299

PPstlTimeOV = 0.0
file = open("../measurements/Time_PPstl.txt", "r")
for line in file.readlines():
    PPstlTimeOV = PPstlTimeOV + float(line.strip("\n"))
PPstlTimeAV = PPstlTimeOV / 299

exTimeGPU = 0
exTimeCPU = 0
count = 0
file = open("../measurements/example_gpu.txt", "r")
for line in file.readlines():
    if not line.startswith("Timings"):
        exTimeCPU = exTimeCPU + float(line.strip('\n'))
        count = count + 1
exAvTimeCPU = exTimeCPU / count
count = 0

file = open("../measurements/example_gpu_op.txt", "r")
for line in file.readlines():
    if not line.startswith("Timings"):
        exTimeGPU = exTimeGPU + float(line.strip('\n'))
        count = count + 1
exAvTimeGPU = exTimeGPU / count

print(ogTimeAV)
print(PPTimeAV)
print(PPstlTimeAV)
print(exAvTimeCPU)
print(exAvTimeGPU)

names = ['Original', 'Per-Pixel', 'Per-Pixel STL', 'Per-Pixel GPU', 'Per-Pixel GPU optimiert']
heights = [ogTimeAV, PPTimeAV, PPstlTimeAV, exAvTimeCPU, exAvTimeGPU]
names2 = ['CPU', 'GPU', 'GPU optimiert']
heights2 = [PPstlTimeAV, exAvTimeCPU, exAvTimeGPU]
plt.figure(figsize=(9, 5))
plt.bar(names, heights, width=0.5)
plt.ylabel('Zeit in ms')
plt.title("Durchschnittliche Ausführungszeit pro Frame")
plt.savefig('result_plot_bar.svg')
plt.clf()
plt.figure(figsize=(7, 5))
plt.bar(names2, heights2, width=0.5)
plt.ylabel('Zeit in ms')
plt.title("Durchschnittliche Ausführungszeit pro Frame")
plt.savefig('gpugpu_plot_bar.svg')

