import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
import numpy as np
import pandas as pd
import seaborn as sns

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



data = pd.read_csv('bm_result_throughput/throughput.out', sep=";")
data2 = pd.read_csv('bm_result_latency/latency.out', sep=";")

data.columns=["lockname", "numThreads", "time", "totalCount", "maxCount", "minCount"]
data2.columns=["lockname", "numThreads", "meanLatency", "maxLatency"]

# data.head()

# remove digits from locknames
def func(string):
    new_string = ''.join((x for x in string if not x.isdigit()))
    return new_string
data['lockname'] = data['lockname'].apply(func)
data2['lockname'] = data2['lockname'].apply(func)


# plotting throughput
def plotLock(lockname):
    # filter data by lock name
    filterData = data[data['lockname'] == lockname]
    # print(filterData)
    
    sns.lineplot(data = filterData, x="numThreads", y="totalCount", ci='sd', label='total lock counts')
    sns.lineplot(data = filterData, x="numThreads", y="maxCount", ci='sd', label='max. lock counts')
    sns.lineplot(data = filterData, x="numThreads", y="minCount", ci='sd', label='min. lock counts')

    nameDict = {
        'PetersonsFilterLock':'Filter Lock',
        'LamportBakeryHerlihyLock': 'Bakery Lock - Herlihy/Shavit',
        'LamportBakeryOriginalLock': 'Bakery Lock - Orignal',
        'PetersonsTree' : 'Binary Tree Lock',
        'BoulangerieLock' : 'Boulangerie Lock',
        'CLock' : 'C11 - Lock',
        'OpenMPLock' : 'OpenMP - Lock',
        'TestAndSetLock' : 'Test And Set - Lock',
        'TestAndTestAndSetLock' : 'Test And Test And Set Lock'
    }
    
    plt.title(nameDict[lockname])
    plt.xlabel('Number of threads')
    plt.ylabel('Lock acquisition per second')
    plt.yscale('log')
    plt.legend()
    # plt.savefig("plots/" + lockname + "_throughput.png")
    plt.show()

plotLock('PetersonsFilterLock')
plotLock('LamportBakeryHerlihyLock')
plotLock('LamportBakeryOriginalLock')
plotLock('PetersonsTree')
plotLock('BoulangerieLock')
plotLock('CLock')
plotLock('OpenMPLock')
plotLock('TestAndSetLock')
plotLock('TestAndTestAndSetLock')



#plotting latency
def plotLock2(lockname):
    # filter data by lock name
    filterData2 = data2[data2['lockname'] == lockname]
    # print(filterData)
    
    sns.lineplot(data = filterData2, x="numThreads", y="meanLatency", ci='sd', label='mean latency')
    sns.lineplot(data = filterData2, x="numThreads", y="maxLatency", ci='sd', label='max latency')

    nameDict = {
        'PetersonsFilterLock':'Filter Lock',
        'LamportBakeryHerlihyLock': 'Bakery Lock - Herlihy/Shavit',
        'LamportBakeryOriginalLock': 'Bakery Lock - Orignal',
        'PetersonsTree' : 'Binary Tree Lock',
        'BoulangerieLock' : 'Boulangerie Lock',
        'CLock' : 'C11 - Lock',
        'OpenMPLock' : 'OpenMP - Lock',
        'TestAndSetLock' : 'Test And Set - Lock',
        'TestAndTestAndSetLock' : 'Test And Test And Set Lock'
    }
    
    plt.title(nameDict[lockname])
    plt.xlabel('Number of threads')
    plt.ylabel('Lock acquisition latency')
    plt.yscale('log')
    plt.legend()
    # plt.savefig("plots/" + lockname + "_latency.png")
    plt.show()

plotLock2('PetersonsFilterLock')
plotLock2('LamportBakeryHerlihyLock')
plotLock2('LamportBakeryOriginalLock')
plotLock2('PetersonsTree')
plotLock2('BoulangerieLock')
plotLock2('CLock')
plotLock2('OpenMPLock')
plotLock2('TestAndSetLock')
plotLock2('TestAndTestAndSetLock')

