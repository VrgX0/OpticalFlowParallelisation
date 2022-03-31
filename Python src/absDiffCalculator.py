
file = open("../measurements/dst_original.txt", "r")
line = file.readline()
originalValues = line.split(",")

file = open("../measurements/dst_FarnebackPPstl2.txt", "r")
line = file.readline()
newValues = line.split(",")
diff = 0.0
origin = []
perPixel = []
for x in range(len(originalValues)):
    if originalValues[x] != "":
        PK = originalValues[x].strip("[]").split()
        for y in range(len(PK)):
            origin.append(float(PK[y]))

for x in range(len(newValues)):
    sumOfPK = 0.0
    if newValues[x] != "":
        PK = newValues[x].strip("[]").split()
        for y in range(len(PK)):
            perPixel.append(float(PK[y]))
count = 0

for x in range(len(origin)):
    diff = diff + abs(origin[x] - perPixel[x])
    count = count + 1
print('\n')
print(count)
print(diff / count)
