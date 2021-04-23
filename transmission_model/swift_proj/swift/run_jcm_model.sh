#! /usr/bin/env bash
set -eu

if [ "$#" -ne 1 ]; then
  script_name=$(basename $0)
  echo "Usage: ${script_name} EXPERIMENT_ID SCENARIO_PARAMS_FILE (e.g. ${script_name} experiment_1 5K_params.sh)"
  exit 1
fi

PATH=/home/ntcollie/midway2/sfw/swift-t-838176b/stc/bin:$PATH

# uncomment to turn on swift/t logging. Can also set TURBINE_LOG,
# TURBINE_DEBUG, and ADLB_DEBUG to 0 to turn off logging
# export TURBINE_LOG=1 TURBINE_DEBUG=1 ADLB_DEBUG=1
THIS=$( cd $( dirname $0 ) ; /bin/pwd )
export EMEWS_PROJECT_ROOT=$( cd $( dirname $0 )/.. ; /bin/pwd )
# source some utility functions used by EMEWS in this script
source "${EMEWS_PROJECT_ROOT}/etc/emews_utils.sh"

#export TURBINE_SBATCH_ARGS="--account=pi-jozik"
export EXPID=$1
export TURBINE_OUTPUT=$EMEWS_PROJECT_ROOT/experiments/$EXPID
check_directory_exists

# TODO edit the number of processes as required.
#export PROCS=644
#export PROCS=32
export PROCS=242
#export PROCS=4
# TODO edit QUEUE, WALLTIME, PPN, AND TURNBINE_JOBNAME
# as required. Note that QUEUE, WALLTIME, PPN, AND TURNBINE_JOBNAME will
# be ignored if the MACHINE variable (see below) is not set.
export QUEUE=broadwl
export WALLTIME=04:00:00
#export WALLTIME=0:50:00
export PPN=14
export TURBINE_JOBNAME="${EXPID}_job"
export TURBINE_DIRECTIVE="#SBATCH --mem-per-cpu=4000\n#SBATCH --mail-type=END,FAIL"
#export TURBINE_SBATCH_ARGS="--exclude=midway2-[0054-0060]"

# if R cannot be found, then these will need to be
# uncommented and set correctly.
# export R_HOME=/path/to/R
# export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$R_HOME/lib
# if python packages can't be found, then uncommited and set this
# export PYTHONPATH=/path/to/python/packages


# TODO edit command line arguments as appropriate
# for your run. Note that the default $* will pass all of this script's
# command line arguments to the swift script.
CMD_LINE_ARGS="$*"
#MODEL_SH=$EMEWS_PROJECT_ROOT/scripts/trans_model_n5000.sh
#MODEL_SH=$EMEWS_PROJECT_ROOT/scripts/trans_model.sh
MODEL_SH=$EMEWS_PROJECT_ROOT/scripts/jcm_model.sh

# set machine to your schedule type (e.g. pbs, slurm, cobalt etc.),
# or empty for an immediate non-queued unscheduled run
MACHINE="slurm"

if [ -n "$MACHINE" ]; then
  MACHINE="-m $MACHINE"
fi

# source the params file
# SCENARIO_PARAMS_FILE=$THIS/$2
# source $SCENARIO_PARAMS_FILE

PARAM_ARGS="-model_sh=$MODEL_SH "
# PARAM_ARGS="-daily.entry.rate=$DAILY_ENTRY_RATE -prep.mean.days.usage.lt=$PREP_MEAN_DAYS_USAGE_LT "
# PARAM_ARGS+="-prep.mean.days.usage.gte=$PREP_MEAN_DAYS_USAGE_GTE -prep.bl.use.prop.lt=$PREP_BL_USE_PROP_LT "
# PARAM_ARGS+="-prep.bl.use.prop.gte=$PREP_BL_USE_PROP_GTE -model_sh=$MODEL_SH"

# Add any script variables that you want to log as
# part of the experiment meta data to the USER_VARS array,
# for example, USER_VARS=("VAR_1" "VAR_2")
# USER_VARS=("DAILY_ENTRY_RATE" "PREP_MEAN_DAYS_USAGE_LT" "PREP_MEAN_DAYS_USAGE_GTE"
#   "PREP_BL_USE_PROP_LT" "PREP_BL_USE_PROP_GTE" "SCENARIO_PARAMS_FILE")

# log variables and script to to TURBINE_OUTPUT directory
log_script
#UPF_DATA_FILE=$EMEWS_PROJECT_ROOT/data/combination-rs-retention.txt 
#UPF_DATA_FILE=$EMEWS_PROJECT_ROOT/data/baseline.txt 
#UPF_DATA_FILE=$EMEWS_PROJECT_ROOT/data/jail.txt 
#UPF_DATA_FILE=$EMEWS_PROJECT_ROOT/data/net_disr.txt 
#UPF_DATA_FILE=$EMEWS_PROJECT_ROOT/data/care_disr.txt 
#UPF_DATA_FILE=$EMEWS_PROJECT_ROOT/data/both_disr.txt 
#UPF_DATA_FILE=$EMEWS_PROJECT_ROOT/data/net_disr_low.txt 
#UPF_DATA_FILE=$EMEWS_PROJECT_ROOT/data/net_disr_high.txt 
#UPF_DATA_FILE=$EMEWS_PROJECT_ROOT/data/care_disr_low.txt 
#UPF_DATA_FILE=$EMEWS_PROJECT_ROOT/data/care_disr_high.txt 
#UPF_DATA_FILE=$EMEWS_PROJECT_ROOT/data/baseline_one.txt 
#UPF_DATA_FILE=$EMEWS_PROJECT_ROOT/data/care_disr_low_one.txt
#UPF_DATA_FILE=$EMEWS_PROJECT_ROOT/data/net_disr_one.txt 
UPF_DATA_FILE=$EMEWS_PROJECT_ROOT/data/drug-bc.txt 
#UPF_DATA_FILE=$EMEWS_PROJECT_ROOT/data/drug-base10yr.txt 


UPF_NAME=$( basename $UPF_DATA_FILE )
UPF_FILE=$TURBINE_OUTPUT/$UPF_NAME

echo $UPF_FILE

#cp $EMEWS_PROJECT_ROOT/../config/model_n10000.props $TURBINE_OUTPUT/model.props
#cp $EMEWS_PROJECT_ROOT/../config/baseline_aids_model.props $TURBINE_OUTPUT/model.props
cp $EMEWS_PROJECT_ROOT/../config/baseline_aids_model.props $TURBINE_OUTPUT/baseline_aids_model.props
#cp $EMEWS_PROJECT_ROOT/../config/jcm_model.props $TURBINE_OUTPUT/jcm_model.props
cp $UPF_DATA_FILE $TURBINE_OUTPUT/

# echo's anything following this standard out
set -x

swift-t -n $PROCS $MACHINE -p $EMEWS_PROJECT_ROOT/swift/trans_model_sweep.swift -f="$UPF_FILE" $PARAM_ARGS $CMD_LINE_ARGS

