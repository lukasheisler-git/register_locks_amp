
#include <assert.h>
#include <iostream>
#include <cstddef>
#include <omp.h>
#include <sstream>
#include <vector>
#include <chrono>  // for high_resolution_clock
#include <string>


#include "locks.hpp"

void testCorrectnes(auto &lock){

    std::size_t control;
    std::size_t id;
    #pragma omp parallel private(id) shared(lock, control) 
    {
        id = omp_get_thread_num();
        for(std::size_t i = 0; i < 500; i++)
        {
            lock.lock(id);
            //critical section
            control = id;
            //simulate computation time in critical section for uneven threads, memory intensive... 
            if(id%2)
            {
                const int max = 100000;
                std::vector<std::size_t> a(max);

                for(std::size_t i = 1; i != a.size(); ++i) {
                    for (std::size_t j = 2 * i; j < a.size(); j += i) {
                        a[j] += i;
                    }
                }
                for (std::size_t i = 1; i != a.size(); ++i) {
                    if(a[i] == i) {
                        //std::cout << i << " is a perfect number!" << std::endl;
                    }
                }
            }
            
            //check control variable after intensive work to see if another thread has joined CS
            if(control != id)
            {
                std::cout << "Attention, not alone in critical section..." << std::endl;
            }
            lock.unlock(id);
        }
        
    }
    std::cout << "___________________________________________________________________" << std::endl;
    std::cout << std::endl;
}


int main(int argc, char *argv[]) {

    //variables
    std::size_t numthreads;
    
    std::cout << "Correctnes benchmark started! Measure if lock works" << std::endl;
    std::cout << "Maximum number of threads for this system: " << omp_get_max_threads() << std::endl;

    //parse command line arguments
    assert(argc == 2);
    {
        std::istringstream tmp(argv[1]);
        tmp >> numthreads;
    }
    
    assert(numthreads >= 1);
    if(numthreads > (std::size_t)omp_get_max_threads())
    {
        std::cout << "Number of threads too high. Reduced to maximum number of threads! " << std::endl;
        numthreads = omp_get_max_threads(); 
    }

    std::cout << "Benchmark started! Number of threads: " << numthreads << std::endl;
    std::cout << std::endl;

    omp_set_dynamic(0);     // Explicitly disable dynamic teams to have full control over amount of threads
	omp_set_num_threads(numthreads); // Fixed amount of threads used for all consecutive parallel regions


    PetersonsFilterLock filterlock(numthreads);
    std::cout << "Testing correctness of Petersons Filter lock..." << std::endl;
    testCorrectnes(filterlock);

    LamportBakeryHerlihyLock herlihylock(numthreads); 
    std::cout << "Testing correctness of Lamport Bakery Herlihy lock..." << std::endl;
    testCorrectnes(herlihylock);

    LamportBakeryOriginalLock lamportlock(numthreads);
    std::cout << "Testing correctness of Lamport Bakery original lock..." << std::endl;
    testCorrectnes(lamportlock);

    PetersonsTree tournamentlock(numthreads);
    std::cout << "Testing correctness of Petersons Tournament tree lock..." << std::endl;
    testCorrectnes(tournamentlock);

    //not working yet
    //BoulangerieLock boulangerielock(numthreads);
    //std::cout << "Testing correctness of Boulangerie lock..." << std::endl;
    //testCorrectnes(boulangerielock);
    
    return EXIT_SUCCESS;
}