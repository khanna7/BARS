# BARS
Code for collaborative project on the modeling of incarceration on HIV among Black men who have sex with men (MSM) in Chicago, Houston and Los Angeles (PI Kayo Fujimoto, Nina Harawa, John Schneider). 

The latest version of this repository can be found in the `Development` branch, [here] (https://github.com/khanna7/BARS/tree/development/transmission_model). You can contact the authors via
e-mail at <akhanna@medicine.bsd.uchicago.edu> 

## Transmission model
   Data and code are in the `transmission_model` directory. 
   The code is divided into two parts: 
   
   * The agent-based modeling suite, derived from the [Repast HPC toolkit](https://repast.github.io/repast_hpc.html), included in `transmission_model/src/`, and,
   * The the dynamic network modeling tools, derived from the [statnet](http://www.statnet.org/) project, included in `transmission_model/r/`.  
     
The population is initialized within `r/`, and dynamically simulated through time using the Repast toolkit. The Rpackage [Rcpp](https://cran.r-project.org/web/packages/Rcpp/index.html) is used to integrate the two frameworks. Automated parameter sweeping procedures, derived from [swift-t](http://swift-lang.org/Swift-T/). are implemented to enable efficient model calibration. 
   
## Lead Authors        

   [Aditya Khanna](https://github.com/khanna7), University of Chicago    
   [Nicholson Collier](https://github.com/ncollier), Argonne National Laboratory    
   [Jonathan Ozik](https://github.com/jozik), Argonne National Laboratory      
   
   for the BARS Study Team       

## Funding
This work is supported by NIH R01 DA 039 934.
