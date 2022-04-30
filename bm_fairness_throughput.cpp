#include <assert.h>
#include <iostream>
#include <fstream>
#include <cstddef>
#include <omp.h>
#include <sstream>
#include <vector>
#include <chrono>  // for high_resolution_clock
#include <algorithm>
#include <bits/stdc++.h>


#include "locks.hpp"

const int REPS = 10;

std::vector<int> testFairnessThroughput(auto &lock, int timeframe){

    std::chrono::seconds sec(timeframe); //convert to nanoseconds
    int id;
    int counter = 0, max = 0, min = 0;
    bool _continue = true;  //variable to indicate when one second is over

    #pragma omp parallel private(id) shared(lock, _continue) reduction(+:counter) reduction(max:max) reduction(min:min)
    {
        id = omp_get_thread_num();
        
        if(id == 0)
        {
            auto start = std::chrono::high_resolution_clock::now(); //start time measurement
            while(_continue)
            {
                auto finish = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double, std::micro> elapsed = finish - start;
                if(elapsed.count() > std::chrono::microseconds(sec).count())
                {
                    _continue = false;
                }
            }
        }
        else
        {
            while(_continue) //during timeframe, try to obtain lock as often as possible
            {
                lock.lock(id);
                lock.unlock(id);
                counter++;
            }
        }
        max = counter;
        //set min of thread 0 to large value to avoid reduction
        min = (id == 0) ? INT_MAX : counter; 
    }

    std::vector<int> result = {counter, max, min};
    return result;
}

void benchmark(auto &lock, int timeframe, int numthreads){

    std::cout << "Benchmark started for " << typeid(lock).name() << " ..." << std::endl; 
    numthreads -= 1; //Because thread 0 is just counting the time
    std::string filename("results/fairness.csv");
    std::ofstream file_out;

    for(int i = 0; i < REPS; i++){
        auto tmp = testFairnessThroughput(lock, timeframe);
        
        //save to file to plot results with python script
        file_out.open(filename, std::ios_base::app);
        file_out << typeid(lock).name() << ";" << std::to_string(numthreads) << ";" << std::to_string(timeframe) << ";" << std::to_string(tmp[0]) << ";" << std::to_string(tmp[1]) << ";" << std::to_string(tmp[2]) << std::endl;
    }
}

int main(int argc, char *argv[]) {

    //variables
    int numthreads;
    int timeframe;
    
    
    std::cout << "Throughput benchmark started! Measure number of operations in given time slot." << std::endl;
    std::cout << "Maximum number of threads for this system: " << omp_get_max_threads() << std::endl;

    //parse command line arguments
    assert(argc == 3);
    {
        std::istringstream tmp(argv[1]);
        tmp >> numthreads;
    }
    {
        std::istringstream tmp(argv[2]);
        tmp >> timeframe;
    }
    
    assert(numthreads >= 1);
    if(numthreads > omp_get_max_threads())
    {
        std::cout << "Number of threads too high. Reduced to maximum number of threads! " << std::endl;
        numthreads = omp_get_max_threads(); 
    }
    
    //possible choices for locks, uncomment only one
    //PetersonsFilterLock lock(numthreads);
    LamportBakeryHerlihyLock lock(numthreads);
    //Bakery lock(numthreads); //not working yet...
    //Filter lock(numthreads); //not working yet...

    std::cout << "Benchmark started! Number of threads: " << numthreads << " | Time frame: " << timeframe << " s" << std::endl;
     
    omp_set_dynamic(0);     // Explicitly disable dynamic teams to have full control over amount of threads
	omp_set_num_threads(numthreads); // Fixed amount of threads used for all consecutive parallel regions

    benchmark(lock, timeframe, numthreads);
    
    return EXIT_SUCCESS;
}