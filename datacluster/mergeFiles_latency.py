###### this scipt has to be executed once for merging all the output files
###### do not execute twice

import os
#import pandas as pd
import numpy as np
import csv
import sys
import matplotlib.pyplot as plt


# list to store file lines
def stripAndMerge(filename):
    lines = []
    # read file
    with open(filename, 'r') as fp:
        # read an store all lines into list
        lines = fp.readlines()

    # Write file
    with open(r"bm_result_latency/latency.out", 'a') as fp:
        # iterate each line
        for line in enumerate(lines):
            if (line[1][0] in "1,2,3,4,5,6,7,8,9,0") :
                fp.write(line[1])
            
stripAndMerge("latency2.out")
stripAndMerge("latency4.out")
stripAndMerge("latency8.out")
