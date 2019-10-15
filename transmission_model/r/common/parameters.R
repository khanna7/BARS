## initial values of parameters
   
   #####################
   ## NETWORK
   n <- 5000
   nedges <- 2500
   deg_seq <- c(1800)

   duration <- 1000

   #####################
   ## TIMESTEP
   size.of.timestep <- 1 #currently set as 1 day
 
   #####################
   ## DEMOGRAPHIC
   min.age <- 16
   max.age <- 65
   given.dur.inf.by.age <- 10*365/size.of.timestep #(not age-specific yet)  
   daily.entry.rate <- 0.00008
   ## distribution of ages (between min and max)
   ## number of births (n.births: for now take it as 1% per year)
   ## age-specific mortality rates (ASMR), adjusted for HIV/AIDS-related deaths
   
   #####################
   ## BIOLOGICAL
   circum.rate <- 0.10
   init.hiv.prev <- 0.10
   init.hiv.prev.for.entries <- 0.00001 # probability that an entering person will have HIV
   duration.of.infection <- 3300
   uninfected.cd4.level <- 518 #(might draw uniformly from a range)

   ## (to compute cd4 in infected but ART-naive)
   b1.ref <- 23.53
   b2.african <- -0.76
   b3.female <- 1.11
   b4.cd4.ref <- -1.49
   b5.african <- 0.34
   b6.age.15to29 <- 0
   b6.age.30to39 <- -0.1
   b6.age.40to49 <- -0.34
   b6.age.50ormore <- -0.63

  ## (time of infection)
  acute.length <- 1:floor(121/size.of.timestep) ## in daily time units
  chronic.length <- ceiling(121/size.of.timestep):
                    floor(1877/size.of.timestep)
  late.length <- ceiling(1877/size.of.timestep):
                    floor(3300/size.of.timestep)

  ## (cd4 decline)
  cd4.at.infection.male <- 518 #cells/mm3
  untreated.cd4.daily.decline <- 0.14 # (for men and women)
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
   baseline.art.coverage.rate <- 0.60
   art.init.time <- 365 # currently set to one year, make more complex later
   per.day.cd4.recovery <- 15/30 ## rate of 15 cells/month
   cd4.rec.per.timestep <- per.day.cd4.recovery*size.of.timestep
   cd4.recovery.time <- 3*365/size.of.timestep ## CD4 recovery for 3 years
   ## parameter for viral decline (add)
   ## ART cessation parameters (what to consider)
   
   #####################
   ## PrEP
   ##  rate of uptake, reduction in infection transmission (per act), cessation conditions/metrics
   prep.mult <- 1.0
   
   #####################
   ## Transmission Parameters
   num.sex.acts.per.timestep <- 2.4*size.of.timestep/7
   acute.mult <- 4.98
   late.mult <- 3.49
   preg.mult <- 2.5 ## check
   circum.mult <- 0.60 ## check
   preg.susc.mult <- 1.7
   acute.mult.holling <- 26
   late.mult.holling <- 7
   min.chronic.infectivity.unadj <- 0.000497/2.89 
   incarceration.prob.for.entries <- 0.009 # probability that an entering person will be incarcerated

   ## relationship between viral load and chronic infectivity (hughes et al.)

   #####################
   ## Jail related Parameters
   ## 

