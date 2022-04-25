#include <assert.h>
#include <iostream>
#include <fstream>
#include <cstddef>
#include <omp.h>
#include <sstream>
#include <vector>
#include <chrono>  // for high_resolution_clock
#include <string>

#include "locks.hpp"



int main(int argc, char *argv[]) {

    //variables
    int numthreads;
    int timeframe;
    int id;
    int counter;
    bool _continue = true;  //variable to indicate when one second is over
    
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

    std::cout << "___________________________________________________________________" << std::endl;
    std::cout << "Benchmark started! Number of threads: " << numthreads << " | Time frame: " << timeframe << " s" << std::endl;
     
    std::chrono::seconds sec(timeframe); //convert to nanoseconds

    omp_set_dynamic(0);     // Explicitly disable dynamic teams to have full control over amount of threads
	omp_set_num_threads(numthreads); // Fixed amount of threads used for all consecutive parallel regions

    
    std::vector<int> global_counter(numthreads, 0);
    

    #pragma omp parallel private(id,counter) shared(lock, _continue, global_counter) 
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
                    std::cout << "Elapsed time: " << elapsed.count() << " ms" << std::endl;;
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
        global_counter[id] = counter;
    }
    std::cout << "Number of lock acquiritions in " << timeframe << " seconds: " << counter << std::endl;
    std::cout << "Number of lock acquiritions per second: " << int(counter/timeframe) << std::endl;

    //save to file to plot results with python script
    std::string filename("results/fairness.csv");
    std::ofstream file_out;
    file_out.open(filename, std::ios_base::app);
    numthreads -= 1; //Because thread 0 is just counting the time
    file_out << typeid(lock).name() << ";" << std::to_string(numthreads) << ";" << std::to_string(int(counter/timeframe)) << std::endl;
    return 0;
}