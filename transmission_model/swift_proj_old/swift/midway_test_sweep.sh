export TURBINE_OUTPUT=$PWD/turbine_out
export QUEUE=sandyb
export WALLTIME=00:10:00
export PROCS=8
export PPN=8
export EXP_NAME="expID"
export TURBINE_JOBNAME="tm_"$EXP_NAME

THIS=$( dirname $0 )
export T_PROJECT_ROOT=$( cd $THIS/.. ; /bin/pwd )

swift-t -m slurm -p test_sweep.swift -exp=$EXP_NAME -f=$T_PROJECT_ROOT/data/test_input_params.txt -md=$PWD/../../Release -props=$PWD/../../config/model.props
