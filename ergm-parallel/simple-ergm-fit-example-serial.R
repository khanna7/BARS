## Simple ergm/ergm-serial example
   ## Due to Steven M Goodreau
   
  ## libraries
  library(ergm)

  ## Network object
  load("sim_net_small.RData")

  ## Fit different ERGM model 

  t1 <- proc.time()

  respdeg <- sapply(1:n, function(x) sum(sim_net_small[x,1:nresp])) # Calculate degree with respondents
  sim_net_small <- set.vertex.attribute(sim_net_small, 'respdeg', respdeg)  # Make nodal attribute

  sim_fit_ser <- ergm(sim_net_small ~ edges+nodecov('respdeg')+gwesp(1, fixed=TRUE),
                     )

  print(proc.time()-t1)

  ## Exit MPI                   
  stopCluster(cluster)
  mpi.exit()
