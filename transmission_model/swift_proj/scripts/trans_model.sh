#! /bin/bash

set -eu

model_dir=$1
props_file=$2
param_line=$3

cd $model_dir

./transmission_model-0.0 $props_file $param_line
