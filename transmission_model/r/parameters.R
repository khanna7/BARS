## initial values of parameters

   ## NETWORK
   n <- 5000
   nedges <- 2500
   deg_seq <- c(1800)

   duration <- 100

   ## TIMESTEP
   size.of.timestep <- 1 #currently set as 1 day

   ## DEMOGRAPHIC
   min.age <- 16
   max.age <- 65

   ## BIOLOGICAL
   circum.rate <- 0.10
   init.hiv.prev <- 0.10
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
  untreated.cd4.daily.decline <- 0.14 # (for men and women)
  untreated.cd4.perstep.decline <- untreated.cd4.daily.decline*
                                   size.of.timestep
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

   ## ART
   baseline.art.coverage.rate <- 0.60
   art.init.time <- 365 # currently set to one year, make more complex later
   per.day.cd4.recovery <- 15/30 ## rate of 15 cells/month
   cd4.rec.per.timestep <- per.day.cd4.recovery*size.of.timestep


   
