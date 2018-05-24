## Compiling BARS Repast code

* Pull the development branch of the BARS repo into your midway account.

* Create a Release directory under the transmission_model directory (i.e. BARS/transmission_model/Release)

* Copy the transmission model from my Release directory to your Release directory. The transmission model is:

     `/home/ntcollie/midway2/repos/BARS/transmission_model/Release/transmission_model-*`

* Update the `version.txt` in your `Release` directory.

* Source settings from [here](https://github.com/khanna7/BARS/blob/development/transmission_model/swift_proj/swift/settings.sh).

* Make sure you have all underlying software packages from [here](https://github.com/khanna7/BARS/blob/development/transmission_model/Readme.md).  

* To run one instance of the model from within the Release directory, use: 

   `./transmission_model-0.0 ../config/config.props ../config/model.props`    
   
 * To run with swift, see instructions [here](https://github.com/khanna7/BARS/blob/development/transmission_model/swift_proj/README.md).  

### Other Notes

* To exclude bad nodes (nodes 54-60, for example) from Midway, do the following:

     in run_trans_model_sweep.sh, add `export TURBINE_SBATCH_ARGS="--exclude=midway2-[0054–0060]”`, after the line that reads: `export TURBINE_JOBNAME="${EXPID}_job”`    

### previewing html docs from github

Link: http://htmlpreview.github.io/?[--insert-github-html-link]

see example [here](http://htmlpreview.github.io/?https://github.com/khanna7/BARS/blob/development/transmission_model/r/testing/testing-apr06-2016.html).
