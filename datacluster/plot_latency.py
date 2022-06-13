from ast import Num
import os
import pandas as pd
import numpy as np
import csv
import sys
import matplotlib.pyplot as plt


data = pd.read_csv('bm_result_latency/latency.out', sep=";", header=None)
#print(data[0][1])  # data[col][row]


filterLock = [[],[],[],[]]
herlihy = [[],[],[],[]]
lamport = [[],[],[],[]]
tournament = [[],[],[],[]]
boulangery = [[],[],[],[]]
c11 = [[],[],[],[]]
openMP = [[],[],[],[]]
tas = [[],[],[],[]]
tatas = [[],[],[],[]]




# numberOfThreads = [2 ,3 ,4 ,8, 13, 16, 21, 35, 48, 53, 63]

def average(values):
    tmp_sum = 0
    counter = 0
    for v in values:
        tmp_sum += v
        counter += 1
    return tmp_sum / counter

#print(data.shape[0])
# start = 0
# filterLock[0].append(average(data[3][start:start+30]))
# filterLock[1].append(average(data[4][start:start+30]))
# filterLock[2].append(average(data[5][start:start+30]))
# filterLock[3].append(average(data[1][start:start+30])-1)

def fillList(listName, start):
    listName[0].append(average(data[1][start:start+30])) #numthreads
    listName[1].append(average(data[2][start:start+30])) #total
    listName[2].append(average(data[3][start:start+30])) #max

def fillPower2(filterLock, herlihy, lamport, tournament, boulangery, c11, openMP, tas, tatas, start_):
    fillList(filterLock, start_)
    start_ += 30
    fillList(herlihy, start_)
    start_ += 30
    fillList(lamport, start_)
    start_ += 30
    fillList(tournament, start_)
    start_ += 30
    fillList(boulangery, start_)
    start_ += 30
    fillList(c11, start_)
    start_ += 30
    fillList(openMP, start_)
    start_ += 30
    fillList(tas, start_)
    start_ += 30
    fillList(tatas, start_)
    start_ += 30

def fillOther(filterLock, herlihy, lamport, boulangery, c11, openMP, tas, tatas, start_):
    fillList(filterLock, start_)
    start_ += 30
    fillList(herlihy, start_)
    start_ += 30
    fillList(lamport, start_)
    start_ += 30
    # fillList(tournament, start_)
    # start_ += 30
    fillList(boulangery, start_)
    start_ += 30
    fillList(c11, start_)
    start_ += 30
    fillList(openMP, start_)
    start_ += 30
    fillList(tas, start_)
    start_ += 30
    fillList(tatas, start_)
    start_ += 30

# 2 threads
start_ = 0
fillPower2(filterLock, herlihy, lamport, tournament, boulangery, c11, openMP, tas, tatas, start_)

start_ = 270
fillPower2(filterLock, herlihy, lamport, tournament, boulangery, c11, openMP, tas, tatas, start_)

start_ = 540
fillPower2(filterLock, herlihy, lamport, tournament, boulangery, c11, openMP, tas, tatas, start_)







# 3 threads ... line 270
# start_ = 270
# fillOther(filterLock, herlihy, lamport, boulangery, c11, openMP, tas, tatas, start_)

# # 4 threads ... line 510
# start_ = 510
# fillPower2(filterLock, herlihy, lamport, tournament, boulangery, c11, openMP, tas, tatas, start_)

# # 8 threads ... line 780
# start_ = 780
# fillPower2(filterLock, herlihy, lamport, tournament, boulangery, c11, openMP, tas, tatas, start_)

# # 13 threads ... line 1050
# start_ = 1050
# fillOther(filterLock, herlihy, lamport, boulangery, c11, openMP, tas, tatas, start_)

# # 16 threads ... line 1290
# start_ = 1290
# fillPower2(filterLock, herlihy, lamport, tournament, boulangery, c11, openMP, tas, tatas, start_)

# # 21 threads ... line 1560
# start_ = 1560
# fillOther(filterLock, herlihy, lamport, boulangery, c11, openMP, tas, tatas, start_)

# # 32 threads ... line 1800
# start_ = 1800
# fillPower2(filterLock, herlihy, lamport, tournament, boulangery, c11, openMP, tas, tatas, start_)

# # 35 threads ... line 2070
# start_ = 2070
# fillOther(filterLock, herlihy, lamport, boulangery, c11, openMP, tas, tatas, start_)

# # 48 threads .. line 2310
# start_ = 2310
# fillOther(filterLock, herlihy, lamport, boulangery, c11, openMP, tas, tatas, start_)

# # 53 threads ... line 2550
# start_ = 2550
# fillOther(filterLock, herlihy, lamport, boulangery, c11, openMP, tas, tatas, start_)

# # 63 threads ... line 2790
# start_ = 2790
# fillOther(filterLock, herlihy, lamport, boulangery, c11, openMP, tas, tatas, start_)

# # end 3030...




#print(filterLock)
#print(data[0][30:60])


# plotting

def plot_lock(lockList, lockName):
    plt.plot(lockList[0], np.asarray(lockList[1]), label="Latency per lock")
    plt.plot(lockList[0], np.asarray(lockList[2]), label="Maximum latency")
    plt.title(lockName)
    plt.legend()
    plt.show()

# this plots one single plot for each lock
plot_lock(filterLock, "Filter lock")
plot_lock(herlihy, "Bakery lock - Herlihy/Shavit")
plot_lock(lamport, "Bakery lock - Original Lamport")
plot_lock(boulangery, "Boulangerie lock")
plot_lock(tournament, "Binary tree lock")
plot_lock(c11, "C11 lock")
plot_lock(openMP, "OpenMP lock")
plot_lock(tas, "Test And Set lock")
plot_lock(tatas, "Test And Test And Set lock")

def plot_lock2(lockList, lockName):
    plt.plot(lockList[0], np.asarray(lockList[2]), label=lockName)
    #plt.plot(lockList[0], np.asarray(lockList[2]), label=lockName)

    

def plot_totalCounts(filterLock, herlihy, lamport, tournament, boulangery, c11, openMP, tas, tatas):
    # here you can comment/uncomment what you want to have plotted
    plot_lock2(filterLock, "Filter lock")
    plot_lock2(herlihy, "Bakery lock - Herlihy/Shavit")
    plot_lock2(lamport, "Bakery lock - Original Lamport")
    plot_lock2(boulangery, "Boulangerie lock")
    plot_lock2(tournament, "Binary tree lock")
    plot_lock2(c11, "C11 lock")
    plot_lock2(openMP, "OpenMP lock")
    plot_lock2(tas, "Test And Set lock")
    plot_lock2(tatas, "Test And Test And Set lock")
    plt.title("Maximum latency")
    plt.legend()
    plt.show()

# this plots all locks in one plot
plot_totalCounts(filterLock, herlihy, lamport, tournament, boulangery, c11, openMP, tas, tatas)
