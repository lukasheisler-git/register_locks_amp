#!/bin/bash
#SBATCH -p q_student
#SBATCH --job-name=amp_proj_heisler       # Job name
#SBATCH -c 33
#SBATCH --cpu-freq=High
#SBATCH --time=00:09:50              # Wall time limit (days-hrs:min:sec)
#SBATCH --output=throughput.out

export OMP_NUM_THREADS=33


proc=3
./bm_fairness_throughput $proc 1 Filter
./bm_fairness_throughput $proc 1 Herlihy
./bm_fairness_throughput $proc 1 Lamport
./bm_fairness_throughput $proc 1 Tournament
./bm_fairness_throughput $proc 1 Boulangerie
./bm_fairness_throughput $proc 1 C11
./bm_fairness_throughput $proc 1 OpenMP
./bm_fairness_throughput $proc 1 TestAndSet
./bm_fairness_throughput $proc 1 TestAndTestAndSet

