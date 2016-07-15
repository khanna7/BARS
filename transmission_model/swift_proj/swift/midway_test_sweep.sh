export TURBINE_OUTPUT=$PWD/turbine_out
export QUEUE=sandyb
export WALLTIME=00:10:00
export PROCS=8
export PPN=8

export T_PROJECT_ROOT=$PWD/..

swift-t -m slurm -n 1 -p test_sweep.swift -f=$PWD/../data/test_input_params.txt -md=$PWD/../../Release -props=$PWD/../../config/model.props