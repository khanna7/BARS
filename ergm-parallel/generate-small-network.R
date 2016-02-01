## Simple ergm/ergm-parallel example
   ## Modified from code initially written by Steven M Goodreau
   
  ## libraries
  library(ergm)

  ## parameters
  nresp <- 25
  nalters <- 25
  mdeg <- 15

  n <- nresp + nalters

  ## Create network
  mynet <- network.initialize(n, directed=F)
  greg <- runif(n) # just to get extra variation in underlying observed degree
  mynet <- set.vertex.attribute(mynet, 'greg', greg)

  ## Network specificaton
  formula <- mynet~edges+nodecov('greg')
  target.stats <- c(mdeg*n/2, 1.1*mdeg*n/2)

  ## Generate starting network

  myfit.ser <- ergm(formula=formula,
                    target.stats=target.stats,
                    constraints = ~.) 


  sim_net_small <- simulate(myfit.ser, nsim=1)

  ## Save
  save.image(file="sim_net_small.RData")
