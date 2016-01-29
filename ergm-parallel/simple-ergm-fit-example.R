## Simple ergm/ergm-parallel example
   ## Modified from code initially written by Steven M Goodreau
   
  ## libraries
  library(ergm)
  library(snow)
  library(Rmpi)
  library(methods)

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


  sim_net <- simulate(myfit.ser, nsim=1)

  ## FIT ERGM (serial)
  t0 <- proc.time()
  sim_fit_ser <- ergm(sim_net ~ edges+gwesp(1, fixed=FALSE)
                      )
  print(proc.time()-t0)

  ## Fit ERGM (parallel)
  np <- mpi.universe.size()-1
  cluster <- makeCluster(np, type="MPI")

  t1 <- proc.time()
  sim_fit_par<- ergm(sim_net ~ edges+gwesp(1, fixed=FALSE),
                               control=control.ergm(parallel=np, parallel.type="MPI")
                     )
  print(proc.time()-t1)

  ## Fit different ERGM model 
  respdeg <- sapply(1:n, function(x) sum(sim_net[x,1:nresp])) # Calculate degree with respondents
  sim_net <- set.vertex.attribute(sim_net, 'respdeg', respdeg)  # Make nodal attribute

  sim_fit_par<- ergm(sim_net ~ edges+nodecov('respdeg')+gwesp(1, fixed=TRUE),
                               control=control.ergm(parallel=np, parallel.type="MPI")
                     )

  ## Exit MPI                   
  stopCluster(cluster)
  mpi.exit()
