## Simple ergm/ergm-parallel example
   ## Modified from code initially written by Steven M Goodreau
   
  ## libraries
  library(ergm)
  library(snow)
  library(Rmpi)
  library(methods)

  ## Network object
  load("sim_net_small.RData")

  ## Parallelization
  np <- mpi.universe.size()-1
  cluster <- makeCluster(np, type="MPI")

  ## Fit parallel ERGM model 

  t1 <- proc.time()

  respdeg <- sapply(1:n, function(x) sum(sim_net_small[x,1:nresp])) # Calculate degree with respondents
  sim_net_small <- set.vertex.attribute(sim_net_small, 'respdeg', respdeg)  # Make nodal attribute

  sim_fit_par<- ergm(sim_net_small ~ edges+nodecov('respdeg')+gwesp(1, fixed=TRUE),
                     control=control.ergm(parallel=cluster,
                                          parallel.type="MPI",
                                          parallel.version.check=TRUE)
                     )


  print(proc.time()-t1)
  ## Exit MPI                   
  stopCluster(cluster)
  mpi.exit()
