Swift Sweep Transmission Model Runs
-----------------------

Files under `swift_proj`

 * `data/test_input_params.txt` - sample input parameters to sweep over
 * `scripts/trans_model.sh` - the bash script file that actually runs the model. The model.props file and the executable
 to use can be set in here.
 * `swift/run_trans_model_sweep.sh` - bash script used to launch swift. The file containing the matrix of parameters that need to be swept over, the number of processes to use, walltime, and
 processes per node (PPN) can be set in here.
 * `swift/trans_model_sweep.swift` - the swift script that peforms the sweep.
 * Remember to change the version number in `../Release/verstion.txt`    
 
To run:

```
cd swift_proj/swift
./run_trans_model_sweep.sh X
```

where X is the name of an "experiment directory". For example `./run_trans_model_sweep.sh test1` will create a `swift_proj/experiments/test1` directory and run the required number of runs from within that directory. Each model run has its own instance directory where its output is generated. For example, `swift_proj/experiments/test1/instance_1`, `swift_proj/experiments/test1/instance_2`, and so on.

Note that `stop.at` is currently set in the model properties file as specified in `scripts/trans_model.sh`. (Though it will be overwritten if specified under in the matrix of parameters to be varied.)

If an indexing error appears, you may need to recompile the main and casual networks.


## To run without swift:

```  
mpirun -n 1 ./transmission_model-0.5 ../config/model.props
```  
Copy excutable from `/home/ntcollie/src/BARS/transmission_model/Release/transmission_model-*` to `../Release` directory

## To run without swift, with a particular instance of parameters, from the command line
Change the contents of the transmission submit script to this:
#!/bin/bash
sbatch <<EOT
#!/bin/sh

module load R
module load netcdf/4.2
module load gcc/4.8

mpirun -n 1 ./transmission_model-0.8 ../config/model.props $1
EOT

Then:
chmod +x run-transmission-model.sh

Then call using:
./run-transmission-model.sh output.directory=./myoutput,global.random.seed=1,acute.mult=10,late.mult=5,prop.steady.sex.acts=0.1428
57142857143,prop.casual.sex.acts=0.142857142857143,min.chronic.infectivity
.unadj=8e-04,daily.entry.rate=8e-04
