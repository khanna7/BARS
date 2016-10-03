# BARS
Code for collaborative project on the modeling of incarceration on HIV among Black men who have sex with men (MSM) in Chicago, Houston and Los Angeles (PI Kayo Fujimoto, Nina Harawa, John Schneider). 

The latest version of this repository can be found in the `Development` branch, [here] (https://github.com/khanna7/BARS/tree/development/transmission_model). You can contact the authors via
e-mail at <akhanna@medicine.bsd.uchicago.edu>. 

## Transmission model
   Data and code are in the `transmission_model` directory. 
   The code is divided into two parts: 
   
   * The agent-based modeling suite, derived from the [Repast HPC toolkit](https://repast.github.io/repast_hpc.html), included in `transmission_model/src/`, and,
   * The the dynamic network modeling tools, derived from the [statnet](http://www.statnet.org/) project, included in `transmission_model/r/`.  
     
The population is initialized within `r/`, and dynamically simulated through time using the Repast toolkit. The R package [Rcpp](https://cran.r-project.org/web/packages/Rcpp/index.html) is used to integrate the two frameworks. Automated parameter sweeping procedures are created using the  [Swift/T](http://swift-lang.org/Swift-T/) parallel scripting language and are implemented to enable efficient model exploration and calibration.

## Selected References   

   * Collier N, North M. Parallel agent-based simulation with Repast for High Performance Computing. SIMULATION: Transactions of The Society for Modeling and Simulation International. 2012. [LINK](http://sim.sagepub.com/content/89/10/1215)  

   * Khanna AS, Roberts ST, Cassels S, et al. Estimating PMTCT's Impact on Heterosexual HIV Transmission: A Mathematical Modeling Analysis. PLoS One. 2015. [LINK](https://www.ncbi.nlm.nih.gov/pubmed/26262889)
   
   * Khanna A, Goodreau SM, Wohlfeiler D, et al. Individualized diagnosis interventions can add significant effectiveness in reducing human immunodeficiency virus incidence among men who have sex with men: insights from Southern California. Ann Epidemiol. 2015. [LINK](https://www.ncbi.nlm.nih.gov/pubmed/25453725)   
   
   * Macal CM, North MJ, Collier N, et al. Modeling the transmission of community-associated methicillin-resistant Staphylococcus aureus: a dynamic agent-based simulation. J Transl Med. 2014. [LINK](https://www.ncbi.nlm.nih.gov/pubmed/24886400)
   
   * Schneider JA, Kozloski M, Michaels S, et al. Criminal justice involvement history is associated with better HIV care continuum metrics among a population-based sample of young black men who have sex with men. AIDS. 2016. [LINK](https://www.ncbi.nlm.nih.gov/pubmed/27662544) 

## Lead Authors        

   [Aditya Khanna](https://github.com/khanna7), University of Chicago    
   [Nicholson Collier](https://github.com/ncollier), Argonne National Laboratory    
   [Jonathan Ozik](https://github.com/jozik), Argonne National Laboratory      
   
   for the BARS Study Team       

## Funding
This work is supported by NIH R01 DA 039 934.
