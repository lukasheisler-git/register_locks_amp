
#include <assert.h>
#include <iostream>
#include <fstream>
#include <cstddef>
#include <omp.h>
#include <sstream>
#include <vector>
#include <chrono>  // for high_resolution_clock

#include "locks.hpp"

const int REPS = 30;
const int OPERATIONS = 1000;

std::vector<double> testLatency(auto &lock){
    
    double maximum = 0; 
    double total = 0;
    int id;

    #pragma omp parallel private(id) shared(lock) reduction(+:total) reduction(max:maximum)
    {
        id = omp_get_thread_num();
        
        for(int i = 0; i < OPERATIONS; i++)
        {
            auto start = std::chrono::high_resolution_clock::now(); //start time measurement
            lock.lock(id);
            lock.unlock(id);
            auto finish = std::chrono::high_resolution_clock::now(); //stop time measurement

            double elapsed = std::chrono::duration_cast<std::chrono::duration<double> >(finish - start).count();
            maximum = std::max(maximum, elapsed);
            total = total + elapsed;
        }
        //average the latency over operations
        total = total / OPERATIONS;
    }
    std::vector<double> result = {total, maximum};
    return result;
}

void benchmark(auto &lock, int numthreads){

    std::cout << "Benchmark started for " << typeid(lock).name() << " ..." << std::endl; 
    for(int i = 0; i < REPS; i++){
        auto tmp = testLatency(lock);
        
        //save to file to plot results with python script
        std::cout << typeid(lock).name() << ";" << std::to_string(numthreads) << ";" << std::to_string(tmp[0]/numthreads) << ";" << std::to_string(tmp[1]) << std::endl;
    }
}

int main(int argc, char *argv[]) {

    //variables
    int numthreads;
    std::string lockname;
    
    std::cout << "Latency benchmark started! Measure time for lock operation." << std::endl;
    std::cout << "Maximum number of threads for this system: " << omp_get_max_threads() << std::endl;

    //parse command line arguments
    assert(argc == 3);
    {
        std::istringstream tmp(argv[1]);
        tmp >> numthreads;
    }
    {
        std::istringstream tmp(argv[2]);
        tmp >> lockname;
    }
    
    assert(numthreads >= 1);
    if(numthreads > omp_get_max_threads())
    {
        std::cout << "Number of threads too high. Reduced to maximum number of threads! " << std::endl;
        numthreads = omp_get_max_threads(); 
    }

    std::cout << "Benchmark started! Number of threads: " << numthreads << std::endl;
     
    omp_set_dynamic(0);     // Explicitly disable dynamic teams to have full control over amount of threads
	omp_set_num_threads(numthreads); // Fixed amount of threads used for all consecutive parallel regions

    //assign lock
    if(lockname == "Filter"){
        PetersonsFilterLock lock(numthreads);
        benchmark(lock, numthreads);
    }
    else if(lockname == "Herlihy"){
        LamportBakeryHerlihyLock lock(numthreads);
        benchmark(lock, numthreads);
    }
    else if(lockname == "Lamport"){
        LamportBakeryOriginalLock lock(numthreads);
        benchmark(lock, numthreads);
    }
    else if(lockname == "Tournament"){
        PetersonsTree lock(numthreads);
        benchmark(lock, numthreads);
    }
    else if(lockname == "Boulangerie"){
        BoulangerieLock lock(numthreads);
        benchmark(lock, numthreads);
    }
    else if(lockname == "C11"){
        C11Lock lock(numthreads);
        benchmark(lock, numthreads);
    }
    else if(lockname == "OpenMP"){
        OpenMPLock lock(numthreads);
        benchmark(lock, numthreads);
    }
    else if(lockname == "TestAndSet"){
        TestAndSetLock lock(numthreads);
        benchmark(lock, numthreads);
    }
    else if(lockname == "TestAndTestAndSet"){
        TestAndTestAndSetLock lock(numthreads);
        benchmark(lock, numthreads);
    }
    else{
        std::cout << "Wrong command line argument: lock type not available!" << std::endl;
        std::cout << "Choose from: [Filter | Lamport | Herlihy | Tournament | Boulangerie | C11 | OpenMP | TestAndSet | TestAndTestAndSet]" << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}


