## initial network estimation

   ## top matter
   rm(list=ls())

   library(ergm)
   library(network)
   library(networkDynamic)
   library(tergm)

   source("parameters.R")

   #####################
   ## Model setup
   formation <- ~edges+degree(1)

   dissolution <- ~offset(edges)
   theta.diss <- log(duration-1)
   target.stats <- c(nedges, deg_seq)

   constraints <- ~.

   formation.n0 <- update.formula(formation, n0~.)

   #####################
   ## Other initialization parameters

   ## sex


   ## age


   ## circumcision status

   ## infection status
   


   #####################
   ## Create empty network to start
   n0 <- network.initialize(n, directed=FALSE, bipartite=FALSE)

   #####################
   ## Fit model
   fit <- ergm(formation.n0, 
               target.stats=target.stats, 
               constraints=constraints
               )

   theta.form <- fit$coef 
   theta.form[1] <- theta.form[1] - theta.diss

   #####################
   ## Simulate 
    hetdeg.diag.sim <- simulate(n0,
                                formation=formation.n0,
                                dissolution=dissolution,
                                coef.form=theta.form, 
                                coef.diss=theta.diss,
                                time.slices=2e4,
                                #time.slices=1e2,
                                constraints=constraints,
                                monitor=~edges+degree(0:5)
                                )

   #####################
   ## Test
   net.f <- network.collapse(hetdeg.diag.sim, at=1000)
   network.size(net.f)
   network.edgecount(net.f)
   degreedist(net.f) 
