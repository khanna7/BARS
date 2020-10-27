#!/bin/bash

#Bryan Brickman Oct 26 2020, BARS project

###Takes one argument, which should be the directory output by the slurm run in /swift_proj/experiments
##example: 
#[USER @midway2-login2]$ bash errorCheck.sh oct22_BARS_model_run_dir

###This script checks for a successfully completed run by checking for the final_results.csv
##and then by checking the err.txt of each instance, outputting the instance number if an error is logged

###Place this script in the directory where your run outputs data
##as of Oct 26, this is {your_directory}/BARS/transmission_model/swift_project/experiments

DIR=$1
NOW=`date +"%Y-%m-%d_%I_%M"`
OUTPUT_FILE="$DIR"_"$NOW"_errCheck_out.txt

#final_results.csv
echo
if [ -s "$DIR/final_results.csv" ]; then
  echo "###---final_results.csv present---###"
  echo
  echo
else
  echo "final_results.csv not found"
  echo
  echo
fi

#Check err.txt and output path if non-empty
for file in "${DIR}"/instance*/e*; do
#echo "${file}"
 if [ -s "${file}" ]; then
    echo error in file: "${file}"
    echo "$file" >> "$OUTPUT_FILE"
    cat "$file" >> "$OUTPUT_FILE"
    echo >> "$OUTPUT_FILE"
 fi
done
