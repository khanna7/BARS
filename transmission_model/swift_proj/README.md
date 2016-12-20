Swift Sweep Transmission Model Runs
-----------------------

Files under `swift_proj`

 * `data/test_input_params.txt` - sample input parameters to sweep over
 * `scripts/trans_model.sh` - the bash script file that actually runs the model. The model.props file and the executable
 to use can be set in here.
 * `swift/run_trans_model_sweep.sh` - bash script used to launch swift. The number of processes to use, walltime, and
 processes per node (PPN) can be set in here.
 * `swift/trans_model_sweep.swift` - the swift script that peforms the sweep.

To run:

```
cd swift_proj/swift
./run_trans_model_sweep.sh X
```

where X is the name of an "experiment directory". For example `./run_trans_model_sweep.sh test1` will create a `swift_proj/experiments/test1` directory and run the required number of runs from within that directory. Each model run has its own instance directory where its output is generated. For example, `swift_proj/experiments/test1/instance_1`, `swift_proj/experiments/test1/instance_2`, and so on.

Note that `stop.at` is currently set in the model properties file as specified in `scripts/trans_model.sh`.

To run without swift:

```  
mpirun -n 1 ./transmission_model-0.5 ../config/swift_test.props
```  
