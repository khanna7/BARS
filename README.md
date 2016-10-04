# BARS
## Building Agent-based Models for Racialized Justice Systems

This is a repository of code for modeling the impact of incarceration on HIV among Black men who have sex with men (MSM) in Chicago, Houston and Los Angeles (PI Kayo Fujimoto, Nina Harawa, John Schneider). 

Detainees and former detainees are a transient population. Incarceration and recidivism have the potential to continually disrupt social and social networks of detainees, with associated increases in high-risk sexual behavior, and turnover in relationships that provide social support. Detainees have a higher prevalence of HIV than the general population. However, correctional facilities may also provide an opportunity for sustained engagement in clinical care for HIV-infected individuals, and preexposure prophylaxis (PrEP) for persons at risk. 

We are utilizing computational modeling tools, parameterized with empirical data, to examine innovative models to reduce new HIV infections in detainees and their communities, inside and outside of custody. The behavioral heterogeneities, combined with the transience in network structures, necessitates the use of modeling frameworks that explicitly allows us to consider both. 

### Transmission model

The latest version of the code for the project can be found in the `Development` branch, [here] (https://github.com/khanna7/BARS/tree/development/transmission_model). 

   Data and code are in the `transmission_model` directory. 
   The code is divided into two parts: 
   
   * The agent-based modeling suite, written in C++ using the [Repast HPC toolkit](https://repast.github.io/repast_hpc.html), included in `transmission_model/src/`, and,
   * The dynamic network modeling tools, using the [statnet](http://www.statnet.org/) toolkit, included in `transmission_model/r/`.  
     
The population is initialized in the R code in the `r/` directory, and dynamically simulated through time in the C++ code. The R package [Rcpp](https://cran.r-project.org/web/packages/Rcpp/index.html) is used to integrate the two frameworks. Automated parameter sweeping procedures are created using the  [Swift/T](http://swift-lang.org/Swift-T/) parallel scripting language and are implemented to enable efficient model exploration and calibration.

### Selected References   

   * Collier N, North M. Parallel agent-based simulation with Repast for High Performance Computing. SIMULATION: Transactions of The Society for Modeling and Simulation International. 2012. [LINK](http://sim.sagepub.com/content/89/10/1215)  

   * Khanna AS, Roberts ST, Cassels S, et al. Estimating PMTCT's Impact on Heterosexual HIV Transmission: A Mathematical Modeling Analysis. PLoS One. 2015. [LINK](https://www.ncbi.nlm.nih.gov/pubmed/26262889)
   
   * Khanna A, Goodreau SM, Wohlfeiler D, et al. Individualized diagnosis interventions can add significant effectiveness in reducing human immunodeficiency virus incidence among men who have sex with men: insights from Southern California. Ann Epidemiol. 2015. [LINK](https://www.ncbi.nlm.nih.gov/pubmed/25453725)   
   
   * Macal CM, North MJ, Collier N, et al. Modeling the transmission of community-associated methicillin-resistant Staphylococcus aureus: a dynamic agent-based simulation. J Transl Med. 2014. [LINK](https://www.ncbi.nlm.nih.gov/pubmed/24886400)
   
   * Schneider JA, Kozloski M, Michaels S, et al. Criminal justice involvement history is associated with better HIV care continuum metrics among a population-based sample of young black men who have sex with men. AIDS. 2016. [LINK](https://www.ncbi.nlm.nih.gov/pubmed/27662544) 

### Lead Authors        

   [Aditya Khanna](https://github.com/khanna7), University of Chicago    
   [Nicholson Collier](https://github.com/ncollier), Argonne National Laboratory    
   [Jonathan Ozik](https://github.com/jozik), Argonne National Laboratory   
   
   You can contact the authors via
e-mail at <akhanna@medicine.bsd.uchicago.edu>. 
   
   for the BARS Study Team       

### Funding
This work is supported by NIH R01 DA 039 934.
