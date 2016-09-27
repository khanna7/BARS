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

  ## (time of infection)
  acute.length <- 1:floor(121/size.of.timestep) ## in daily time units
  chronic.length <- ceiling(121/size.of.timestep):
                    floor(1877/size.of.timestep)
  late.length <- ceiling(1877/size.of.timestep):
                    floor(3300/size.of.timestep)

  ## (cd4 decline)
  untreated.cd4.perstep.decline <- untreated.cd4.daily.decline*
                                   size.of.timestep
   ## healthy level of CD4: sample from some distribution, or should it be the same for all uninfected men?
   
  ## (viral load)
  time.infection.to.peak.viremia <- floor(14/size.of.timestep)
  time.infection.to.peak.viral.load <- time.infection.to.peak.viremia
  peak.viral.load <- 6.17
  time.infection.to.viral.set.point <- floor(121/size.of.timestep)
  set.point.viral.load <- 4.2
  time.infection.to.late.stage <- floor(1877/size.of.timestep)
  dur.inf <- floor(3300/size.of.timestep)
  late.stage.viral.load <- 5.05 ## (max?)
  time.infection.to.peak.viral.load
  time.to.full.supp <- 4*30/size.of.timestep ## 4 months
  undetectable.vl <- log(50, base=10)

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
   
   #####################
   ## Transmission Parameters
   num.sex.acts.per.timestep <- num.sex.acts.base*size.of.timestep/7
   
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
