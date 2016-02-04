## initial values of parameters

   ## NETWORK
   n <- 5000
   nedges <- 2500
   deg_seq <- c(1800)

   duration <- 100

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

   ## ART
   baseline.art.coverage.rate <- 0.60
   art.init.time <- 365 # currently set to one year, make more complex later



   ## TIMESTEP
   size.of.timestep <- 1 #currently set as 1 day
   
