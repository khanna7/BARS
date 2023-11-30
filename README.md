# BARS
code for collaboration on CJI project
## Jail Circulation Model
Code for _JCM_ [manuscript](https://www.ncbi.nlm.nih.gov/pubmed/38026447)

## Data Generation
 - Can be done using executable v0.72

## Unrolled Parameter Files (UPFs) for Data Generation
 - R file to generate all the needed UPFs for the calibration and the interventions are [here](https://github.com/khanna7/BARS/tree/jcm-paper/transmission_model/swift_proj/data).
 - The code produces output in txt format that is read by swift, and in csv format for easy readability. 

## Data Analysis 
 - Code to produce annualized HIV incidence and prevalence for the calibration and intervention runs as well as the final incidence and prevalence boxplots is [here](https://github.com/khanna7/BARS/tree/jcm-paper/transmission_model/swift_proj/R). 
 - Follow the instructions in the [README](https://github.com/khanna7/BARS/blob/jcm-paper/transmission_model/swift_proj/R/README.md) to reproduce the various plots.
 - Analysis and plotting R files are [here](https://github.com/khanna7/BARS/tree/jcm-paper/transmission_model/swift_proj/R/12sc). This file will produce the plots.
 
## References
 - Reproducing the Jail Circulation Model paper: [here](https://docs.google.com/document/d/1EPRCsISDPPzzIEecDm73_X3vh0W88izykLYFrzLQobQ/).
