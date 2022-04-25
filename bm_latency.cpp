//Implement:
//• Filter-lock (generalized Peterson)
//• Tournament tree of 2-thread Peterson locks (see exercise)
//• Lamport Bakery, Herlihy-Shavit version (see lecture)
//• Lamport Bakery, Lamport’s original version
//• Boulangerie

#include <assert.h>
#include <iostream>
#include <fstream>
#include <cstddef>
#include <omp.h>
#include <sstream>
#include <vector>
#include <chrono>  // for high_resolution_clock

#include "locks.hpp"


int main(int argc, char *argv[]) {

    //variables
    int numthreads;
    int operations;
    int id;
    
    std::cout << "Latency benchmark started! Measure time per operation." << std::endl;
    std::cout << "Maximum number of threads for this system: " << omp_get_max_threads() << std::endl;
    //parse command line arguments
    assert(argc == 3);
    {
        std::istringstream tmp(argv[1]);
        tmp >> numthreads;
    }
    {
        std::istringstream tmp(argv[2]);
        tmp >> operations;
    }
    assert(numthreads >= 1);
    if(numthreads > omp_get_max_threads())
    {
        std::cout << "Number of threads too high. Reduced to maximum number of threads! " << std::endl;
        numthreads = omp_get_max_threads(); 
    }
    std::cout << "___________________________________________________________________" << std::endl;
    std::cout << "Benchmark started! Number of threads: " << numthreads << " | Operations: " << operations << std::endl;
     

    //possible choices for locks, uncomment only one
    PetersonsFilterLock lock(numthreads);
    //LamportBakeryHerlihyLock lock(numthreads); 
    //
    
    
    omp_set_dynamic(0);     // Explicitly disable dynamic teams to have full control over amount of threads
	omp_set_num_threads(numthreads); // Fixed amount of threads used for all consecutive parallel regions


    
    std::chrono::duration<double, std::micro> maximum;
    std::chrono::duration<double, std::micro> total;
    double maximum_d = 0; 
    double total_d = 0;
    #pragma omp parallel private(id) shared(lock) firstprivate(operations) reduction(+:total_d) reduction(max:maximum_d)
    {
        id = omp_get_thread_num();

        
        for(int i = 0; i < operations; i++)
        {
            auto start = std::chrono::high_resolution_clock::now(); //start time measurement
            lock.lock(id);
            lock.unlock(id);
            auto finish = std::chrono::high_resolution_clock::now(); //stop time measurement
            std::chrono::duration<double, std::micro> elapsed = finish - start;
            if(elapsed > maximum)
            {
                maximum = elapsed;
            }
            total = total + elapsed;
        }
        //average the latency over operations
        total = total / operations;
        //retrieve double value for reduction clause
        total_d = total.count();
        maximum_d = maximum.count();
    }
    //average the latency over the threads
    total_d = total_d / numthreads;

    std::cout << "Maximum Latency: " << maximum_d << " ms" << std::endl;
    std::cout << "Average Latency: " << total_d << " ms" << std::endl;
    std::cout << "___________________________________________________________________" << std::endl;


    //save to file to plot results with python script
    std::string filename("results/latency.csv");
    std::ofstream file_out;
    file_out.open(filename, std::ios_base::app);
    file_out << typeid(lock).name() << ";" << std::to_string(numthreads) << ";" << std::to_string(total_d) << ";" << std::to_string(maximum_d) << std::endl;
    return 0;
}


