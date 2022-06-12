#!/bin/bash
#SBATCH -p q_student
#SBATCH --job-name=amp_proj_heisler       # Job name
#SBATCH -c 64
#SBATCH --cpu-freq=High
#SBATCH --time=00:09:50              # Wall time limit (days-hrs:min:sec)
#SBATCH --output=latency2.out

export OMP_NUM_THREADS=64


proc=2
./bm_latency $proc Filter
./bm_latency $proc Herlihy
./bm_latency $proc Lamport
./bm_latency $proc Tournament
./bm_latency $proc Boulangerie
./bm_latency $proc C11
./bm_latency $proc OpenMP
./bm_latency $proc TestAndSet
./bm_latency $proc TestAndTestAndSet
