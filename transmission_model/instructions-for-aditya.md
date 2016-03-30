Aditya,

I’ve compiled the transmission model on midway. The easiest way to get this running for you is to:

1. Pull the development branch of the BARS repo into your midway account.

2. Create a Release directory under the transmission_model directory (i.e. BARS/transmission_model/Release)

3. Copy the transmission model from my Release directory to your Release directory. The transmission model is:

/home/ntcollie/src/BARS/transmission_model/Release/transmission_model-0.0 

4. The following modules need to be loaded in order for it to work

module load R
module load netcdf/4.2
module load gcc/4.8

5. I had to install some R packages (ergm etc.) but I imagine you’ve got those already installed. I did have to export R_LIBS though:

export R_LIBS=/home/ntcollie/R/x86_64-unknown-linux-gnu-library/3.2

6. Run the model from within the Release directory with 

./transmission_model-0.0 ../config/config.props ../config/model.props

Jonathan and I tested this procedure on his account and the model ran.

A few of the model properties (such as the output file) are specified in the config/model.props file. The others are all in transmission_model/r/common/parameters.R

At the moment the output file records the number of entries, old age and infection deaths, and number iof infected that occured during a timestep as well as the number of edges and vertices at the end of each timestep. 

I haven’t tried running this on the cluster itself via the queue, but that should be straight forward. We could also talk (next week?) about strategies for doing multiple parallel runs on the cluster to calibrate parameters, if that is necessary.

Nick
