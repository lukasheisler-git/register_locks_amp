from ast import Num
import os
import pandas as pd
import numpy as np
import csv
import sys
import matplotlib.pyplot as plt


SMALL_SIZE = 12
MEDIUM_SIZE = 14
BIGGER_SIZE = 16

plt.rc('font', size=SMALL_SIZE)          # controls default text sizes
plt.rc('axes', titlesize=SMALL_SIZE)     # fontsize of the axes title
plt.rc('axes', labelsize=MEDIUM_SIZE)    # fontsize of the x and y labels
plt.rc('xtick', labelsize=SMALL_SIZE)    # fontsize of the tick labels
plt.rc('ytick', labelsize=SMALL_SIZE)    # fontsize of the tick labels
plt.rc('legend', fontsize=SMALL_SIZE)    # legend fontsize
plt.rc('figure', titlesize=BIGGER_SIZE)  # fontsize of the figure title
plt.rcParams["figure.figsize"] = (12,8)





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

# 16 threads .. line 810
start_ = 810
fillPower2(filterLock, herlihy, lamport, tournament, boulangery, c11, openMP, tas, tatas, start_)

# 23 threads .. line 1080
start_ = 1080
fillOther(filterLock, herlihy, lamport, boulangery, c11, openMP, tas, tatas, start_)

# 32 threads ... line 1320
start_ = 1320
fillPower2(filterLock, herlihy, lamport, tournament, boulangery, c11, openMP, tas, tatas, start_)

# 43 threads .. line 1590
start_ = 1590
fillOther(filterLock, herlihy, lamport, boulangery, c11, openMP, tas, tatas, start_)

# 53 threads .. line 1830
start_ = 1830
fillOther(filterLock, herlihy, lamport, boulangery, c11, openMP, tas, tatas, start_)

# 32 threads ... line 2070
start_ = 2070
fillPower2(filterLock, herlihy, lamport, tournament, boulangery, c11, openMP, tas, tatas, start_)


# plotting

def plot_lock(lockList, lockName):
    plt.plot(lockList[0], np.asarray(lockList[1]), label="Latency per lock")
    plt.plot(lockList[0], np.asarray(lockList[2]), label="Maximum latency")
    plt.title(lockName)
    plt.yscale("log")
    plt.legend()
    plt.show()

# this plots one single plot for each lock
# plot_lock(filterLock, "Filter lock")
# plot_lock(herlihy, "Bakery lock - Herlihy/Shavit")
# plot_lock(lamport, "Bakery lock - Original Lamport")
# plot_lock(boulangery, "Boulangerie lock")
# plot_lock(tournament, "Binary tree lock")
# plot_lock(c11, "C11 lock")
# plot_lock(openMP, "OpenMP lock")
# plot_lock(tas, "Test And Set lock")
# plot_lock(tatas, "Test And Test And Set lock")

def plot_lock2(lockList, lockName):
    plt.plot(lockList[0], np.asarray(lockList[1]), label=lockName)
    #plt.plot(lockList[0], np.asarray(lockList[2]), label=lockName)
    plt.yscale("log")

    

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
    plt.xlabel('Number of threads')
    plt.ylabel('Lock acquisition latency')
    plt.title("Mean latency")
    plt.legend()
    # plt.savefig("plots/meanLatency.png")
    plt.show()

# this plots all locks in one plot
plot_totalCounts(filterLock, herlihy, lamport, tournament, boulangery, c11, openMP, tas, tatas)
