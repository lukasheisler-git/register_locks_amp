SOURCES = locks.cpp
CXX=g++
CXXFLAGS := $(CXXFLAGS) -std=c++11 -Wall -pedantic -O3 -ffast-math     -march=native   -fopenmp

all: correctness

#throughput:
#	$(CXX) $(CXXFLAGS) -o bm_throughput bm_throughput.cpp $(SOURCES) 

correctness:
	$(CXX) $(CXXFLAGS) -o bm_correctness bm_correctness.cpp $(SOURCES)
	
#latency:
#	$(CXX) $(CXXFLAGS) -o bm_latency bm_latency.cpp $(SOURCES)

.PHONY: clean
clean:
	rm -f bm_throughput
	rm -f bm_correctness
	rm -f bm_latency

