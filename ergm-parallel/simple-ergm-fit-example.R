## Simple ERGM example

  ## libraries
  library(ergm)
  library(snow)
  library(Rmpi)
  library(methods)

  ## parameters
  nresp <- 1e3
  nalters <- 2e3
  mdeg <- 50

  n <- nresp + nalters

  ## Create network
  mynet <- network.initialize(n, directed=F)
  greg <- runif(n) # just to get extra variation in underlying observed degree
  mynet <- set.vertex.attribute(mynet, 'greg', greg)

  ## Network specificaton
  formula <- mynet~edges+nodecov('greg')
  target.stats <- target.stats

  ## Fit ERGM (serial)
  t0 <- proc.time()

  myfit.ser <- ergm(formula=formula,
                    target.stats=target.stats) 

  proc.time()-t0


  ## Fit ERGM (parallel)
  np <- mpi.universe.size()-1
  cluster <- makeCLuster(np, type="MPI")

  t1 <- proc.time()
 
  myfit.par <- ergm(formula=formula, 
                    target.stats=target.stats,
                    control=control.ergm(parallel=np, parallel.type="MPI")

  proc.time()-t1

  ## Simulate once to generate a network
  sim_net <- simulate(myfit)

  ## Exit MPI                   
  stopCluster(cluster)
  mpi.exit()
