## initial values of parameters
   
   #####################
   ## NETWORK (steady)

     nedges <- n*mean_deg/2 

     deg_seq <- c(56.6, 41.2, 1.8)*n/100   
     #duration <- (562+1260)/2
      
     ## role
     pr_versatile_main <- 1-(pr_insertive_main + pr_receptive_main)
     nodematch.inf.status <- 0.75*nedges 
       #28/35 partnerships match on infection status, 
       #almost equal between susc-susc and inf-inf 

     ## ergm parameters for dissolution 
     d.rate <- 1/((max.age - min.age)*365)
     pg <- (duration - 1)/duration
     ps2 <- (1 - d.rate)^2
     theta.diss <- log(pg/(ps2-pg)) #adjusted for death rate, Run 
   
   #####################
   ## DEMOGRAPHIC
   given.dur.inf.by.age <- 10*365/size.of.timestep #(not age-specific yet)  
   ##daily.entry.rate <- 0.00008
   
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
    
   #####################
   ## PrEP
   prep.daily.stop.prob.lt <- 1/prep.mean.days.usage.lt
   prep.daily.stop.prob.gte <- 1/prep.mean.days.usage.gte

   #####################
   ## Transmission Parameters
   ## relationship between viral load and chronic infectivity (hughes et al.)
   
   #####################
   ## Network (casual)
      ## nedges
         cas_tot_deg <- (0*cas_deg_seq[1])+(1*cas_deg_seq[2])+
                        (2*cas_deg_seq[3])
         cas_n_edges <- cas_tot_deg/2
      ## role
      pr_versatile_casual <- 1 - (pr_insertive_casual + pr_receptive_casual)
      ## serosorting

     ## dissolution parameters for ergm
     pg_cas <- (dur_cas - 1)/dur_cas
     theta.diss_cas <- log(pg_cas/(ps2-pg_cas)) #adjusted for death rate 

    #####################
     ## Testing and diagnosis
     daily.testing.prob <- 1/mean.time.until.next.test #FOR INITIALIZATION ONLY
  
      #####################

    #####################
    ## PrEP
    
#####################
    ## Socioeconomic status
