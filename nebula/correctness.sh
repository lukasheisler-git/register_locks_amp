#!/bin/bash
#SBATCH -p q_student
#SBATCH --job-name=amp_proj_heisler       # Job name
#SBATCH -N 1
#SBATCH -c 16
#SBATCH --cpu-freq=High
#SBATCH --time=00:04:50              # Wall time limit (days-hrs:min:sec)
#SBATCH --output=test_job.out

export OMP_NUM_THREADS=16
procs=( 2 4 8 16 )

for proc in "${procs[@]"
do
    ./bm_correctness $proc 
done
