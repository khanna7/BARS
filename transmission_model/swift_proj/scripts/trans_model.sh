#! /bin/bash

set -eu

model_dir=$1
props_file=$2
param_line=$3

echo "cding to $model_dir"
cd $model_dir

#app_name=transmission-model-0.0
app_name=transmission_model-testing_diagnosis-0.2

mpirun -n 1 ./$app_name $props_file $param_line
