# Register Locks

## Advanced Multiprocessor Programming - Project
## Tobias Florian (01526529) , Lukas Heisler (12038042)

--------------------------

## Pre-Requisits

- `Make`
- `gcc or g++ C++17 Compiler`
- `python3` (only for benchmark plotting)

--------------------------

### Supported Platforms

Currently, only **Linux** is supported and tested for. 

--------------------------
## Building

To build all benchmark programs:
```
make
```
or build just one specific benchmark program

```
make correctness
make througput
make latency
```
--------------------------

## Implementation

For the lock implementations, please refer to the Report and the attached `locks.hpp` and `locks.cpp` files.

--------------------------

## Cluster SLURM scripts

For benchmarking on the nebula cluster, several slurm scripts were used. Refer to them in the `nebula` folder. 

--------------------------

## Benchmarking results

All benchmarking results from the cluster can be found in the `datacluster` folder, alongside some python scripts for data preparation and to create the plots. 