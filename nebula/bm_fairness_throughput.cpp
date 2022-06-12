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

const int REPS = 30;

std::vector<int> testFairnessThroughput(auto &lock, int timeframe){

    std::chrono::seconds sec(timeframe); //convert to nanoseconds
    int id;
    int counter = 0, maxV = 0, minV = INT_MAX;
    bool _continue = true;  //variable to indicate when one second is over

    #pragma omp parallel private(id) shared(lock, _continue) reduction(min:minV) reduction(+:counter) reduction(max:maxV)
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
            minV = counter;
            maxV = counter;
        }
        
    }
    std::vector<int> result = {counter, maxV, minV};
    return result;
}

void benchmark(auto &lock, int timeframe, int numthreads){

    std::cout << "Benchmark started for " << typeid(lock).name() << " ..." << std::endl; 

    for(int i = 0; i < REPS; i++){
        auto tmp = testFairnessThroughput(lock, timeframe);
        
        std::cout << typeid(lock).name() << ";" ;
        std::cout << std::to_string(numthreads) << ";"; 
        std::cout << std::to_string(timeframe) << ";" ;
        std::cout << std::to_string(tmp[0]) << ";" ;
        std::cout << std::to_string(tmp[1]) << ";" ;
        std::cout << std::to_string(tmp[2]) << std::endl;
    }
}

int main(int argc, char *argv[]) {

    //variables
    int numthreads;
    int timeframe;
    std::string lockname;
    
    std::cout << "Throughput benchmark started! Measure number of operations in given time slot." << std::endl;
    std::cout << "Maximum number of threads for this system: " << omp_get_max_threads() << std::endl;

    //parse command line arguments
    assert(argc == 4);
    {
        std::istringstream tmp(argv[1]);
        tmp >> numthreads;
    }
    {
        std::istringstream tmp(argv[2]);
        tmp >> timeframe;
    }
    {
        std::istringstream tmp(argv[3]);
        tmp >> lockname;
    }
    
    assert(numthreads >= 1);
    if(numthreads > omp_get_max_threads())
    {
        std::cout << "Number of threads too high. Reduced to maximum number of threads! " << std::endl;
        numthreads = omp_get_max_threads(); 
    }

    std::cout << "Benchmark started! Number of threads: " << numthreads << " | Time frame: " << timeframe << " s" << std::endl;
     
    omp_set_dynamic(0);     // Explicitly disable dynamic teams to have full control over amount of threads
	omp_set_num_threads(numthreads); // Fixed amount of threads used for all consecutive parallel regions

    //assign lock
    if(lockname == "Filter"){
        PetersonsFilterLock lock(numthreads-1);
        benchmark(lock, timeframe, numthreads);
    }
    else if(lockname == "Herlihy"){
        LamportBakeryHerlihyLock lock(numthreads-1);
        benchmark(lock, timeframe, numthreads);
    }
    else if(lockname == "Lamport"){
        LamportBakeryOriginalLock lock(numthreads-1);
        benchmark(lock, timeframe, numthreads);
    }
    else if(lockname == "Tournament"){
        PetersonsTree lock(numthreads-1);
        benchmark(lock, timeframe, numthreads);
    }
    else if(lockname == "Boulangerie"){
        BoulangerieLock lock(numthreads-1);
        benchmark(lock, timeframe, numthreads);
    }
    else if(lockname == "C11"){
        C11Lock lock(numthreads-1);
        benchmark(lock, timeframe, numthreads);
    }
    else if(lockname == "OpenMP"){
        OpenMPLock lock(numthreads-1);
        benchmark(lock, timeframe, numthreads);
    }
    else if(lockname == "TestAndSet"){
        TestAndSetLock lock(numthreads-1);
        benchmark(lock, timeframe, numthreads);
    }
    else if(lockname == "TestAndTestAndSet"){
        TestAndTestAndSetLock lock(numthreads-1);
        benchmark(lock, timeframe, numthreads);
    }
    else{
        std::cout << "Wrong command line argument: lock type not available!" << std::endl;
        std::cout << "Choose from: [Filter | Lamport | Herlihy | Tournament | Boulangerie | C11 | OpenMP | TestAndSet | TestAndTestAndSet]" << std::endl;
        return EXIT_FAILURE;
    }



    
    
    return EXIT_SUCCESS;
}