## initial values of parameters
   
   #####################
   ## NETWORK (steady)

     nedges <- n*mean_deg/2 

     deg_seq <- c(56.6, 41.2, 1.8)*n/100   
     duration <- (562+1260)/2
      
     ## role
     pr.versatile <- 1-(pr.insertive + pr.receptive)
     nodematch.inf.status <- 0.75*nedges 
       #28/35 partnerships match on infection status, 
       #almost equal between susc-susc and inf-inf 
   
   #####################
   ## TIMESTEP

   #####################
   ## DEMOGRAPHIC
   given.dur.inf.by.age <- 10*365/size.of.timestep #(not age-specific yet)  
   daily.entry.rate <- 0.00008
   
   #####################
   ## BIOLOGICAL

  ## (cd4 decline)
  untreated.cd4.perstep.decline <- untreated.cd4.daily.decline*
                                   size.of.timestep
   ## healthy level of CD4: sample from some distribution, or should it be the same for all uninfected men?

   #####################
   ## ART
   cd4.rec.per.timestep <- per.day.cd4.recovery*size.of.timestep
   cd4.recovery.time <- 3*365/size.of.timestep ## CD4 recovery for 3 years
   ## parameter for viral decline (add)
   ## ART cessation parameters (what to consider)
   
   ## ART Adherence
   prop.partially.adherent <- 1 - (prop.never.adherent+prop.always.adherent) 
   
   #####################
   ## PrEP
   prep.daily.stop.prob <- 1/prep.mean.days.usage
   #####################
   ## Transmission Parameters
   ## relationship between viral load and chronic infectivity (hughes et al.)
   
   #####################
   ## Casual (steady)
      ## nedges
         cas_tot_deg <- (0*cas_deg_seq[1])+(1*cas_deg_seq[2])+
                        (2*cas_deg_seq[3])+(3*cas_deg_seq[4])
         cas_n_edges <- cas_tot_deg/2
      ## role
      pr_versatile_casual <- 1 - (pr_insertive_casual + pr_receptive_casual)
      ## serosorting
   
    #####################
    ## Testing and diagnosis
    daily.testing.prob <- 1/mean.time.until.next.test

    #####################
    ## PrEP
    
#####################
    ## Socioeconomic status
