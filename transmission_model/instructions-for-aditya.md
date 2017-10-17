## Compiling BARS Repast code

* Pull the development branch of the BARS repo into your midway account.

* Create a Release directory under the transmission_model directory (i.e. BARS/transmission_model/Release)

* Copy the transmission model from my Release directory to your Release directory. The transmission model is:

     `/home/ntcollie/src/BARS/transmission_model/Release/transmission_model-0.0`

* The following modules need to be loaded in order for it to work

   `module load mvapich2/2.0` 
   `module load R`  
   `module load netcdf/4.2`  
   `module load gcc/4.8`

* I had to install some R packages (ergm etc.) but I imagine you’ve got those already installed. I did have to export R_LIBS though:

   `export R_LIBS=/home/ntcollie/R/x86_64-unknown-linux-gnu-library/3.2`

* Run the model from within the Release directory with 

   `./transmission_model-0.0 ../config/config.props ../config/model.props`    
   
* Midway 2 executable: `/home/ntcollie/midway2/repos/BARS/transmission_model/Release/transmission_model-0.18`

* To exclude bad nodes (nodes 54-60, for example) from Midway, do the following:

     in run_trans_model_sweep.sh, add `export TURBINE_SBATCH_ARGS="--exclude=midway2-[0054–0060]”`, after the line that reads: `export TURBINE_JOBNAME="${EXPID}_job”`    

## previewing html docs from github

Link: http://htmlpreview.github.io/?[--insert-github-html-link]

see example [here](http://htmlpreview.github.io/?https://github.com/khanna7/BARS/blob/development/transmission_model/r/testing/testing-apr06-2016.html).
