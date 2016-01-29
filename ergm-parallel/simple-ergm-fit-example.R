## Simple ERGM example

  ## libraries
  library(ergm)
  library(snow)
  library(Rmpi)
  library(methods)

  ## parameters
  nresp <- 1000
  nalters <- 1000
  mdeg <- 15

  n <- nresp + nalters

  ## Create network
  mynet <- network.initialize(n, directed=F)
  greg <- runif(n) # just to get extra variation in underlying observed degree
  mynet <- set.vertex.attribute(mynet, 'greg', greg)

  ## Network specificaton
  formula <- mynet~edges+nodecov('greg')+degree(0)
  target.stats <- c(mdeg*n/2, 1.1*mdeg*n/2, n/10)

  ## Fit ERGM (serial)
  t0 <- proc.time()

  myfit.ser <- ergm(formula=formula,
                    target.stats=target.stats) 

  print(proc.time()-t0)


  ## Fit ERGM (parallel)


  np <- mpi.universe.size()-1
  cluster <- makeCluster(np, type="MPI")

  t1 <- proc.time()
  myfit.par <- ergm(formula=formula, 
                    target.stats=target.stats,
                    control=control.ergm(parallel=np, parallel.type="MPI"))

  print(proc.time()-t1)

  ## Simulate once to generate a network
  #sim_net <- simulate(myfit.ser)

  ## Exit MPI                   
  stopCluster(cluster)
  mpi.exit()
