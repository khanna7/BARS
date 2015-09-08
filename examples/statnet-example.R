## example using statnet

## Packages and libraries
library(network)# 3 Sep 2013
library(ergm)#3 Sep 2013
library(tergm)
library(networkDynamic)

   rm(list=ls())
   ## Example 1
      ## n=5000
      ## 3000 edges
      ## mean partnership duration = 10 timesteps
      ## simulation over 50 timesteps
      
      N <- 5000
      duration <- 10
      theta.diss <- log(duration-1)
      n.edges <- 3000
      target.stats <- c(n.edges)
      constraints <- ~.
      
      n0 <- network.initialize(N, directed=FALSE)
      
      formation <- ~edges
      formation.n0 <- update.formula(formation, n0~.)
      dissolution <- ~offset(edges)
      
      fit <- ergm(formation.n0,
                  target.stats=target.stats,
                  constraints=constraints,
                  maxit=500,
                  verbose=FALSE)      

       theta.form <- fit$coef
       theta.form[1] <- theta.form[1]-theta.diss

      ## testing estimation procedure
     ##  hetdeg.diag.sim <- simulate(n0,
     ##                              formation=formation,
     ##                              dissolution=dissolution,
     ##                              coef.form=theta.form,
     ##                              coef.diss=theta.diss,
     ##                              time.slices=1e2,
     ##                              constraints=constraints,
     ##                              monitor=~edges
     ##                              )

     ## sim.stats <- attributes(hetdeg.diag.sim)$stats
     ## stats.means <- colMeans(sim.stats)##mean(sim.stats)
     ## stats.sd <- apply(sim.stats,2,sd)
     ## sim.df <- as.data.frame(hetdeg.diag.sim)
     ## partnership.duration.mean <- mean(sim.df$duration)
     ## partnership.duration.sd <- sd(sim.df$duration)
     ## partnership.duration.expected <- exp(theta.diss)+1

     ## Dynamic simulation
      nw <- fit$network
      formation <- fit$formula
      dissolution <- ~offset(edges)
      popsize <- network.size(nw)

      timesteps <- 100
      start.time <- 1

  ## Start with all vertices and edges active
      nw <- activate.vertices(nw,onset=1,terminus=Inf)
      nw <- activate.edges(nw,onset=1,terminus=Inf)

    for (time in 2:timesteps) {

        set.seed(Sys.time())
             # sam has no problem getting stochastically different results 
             # without this line

      ## Relational Change
      cat("Completing time step", time,
          "of", timesteps, "\n")
        
        nw <- simulate(nw,
                 formation = formation, 
                 dissolution = dissolution,
                 coef.form = theta.form, 
                 coef.diss = theta.diss,
                 constraints = constraints,
                 #time.start = ts,
                 time.start = time,
                 #start.time = time,
                 time.slices = 1#,
                 #monitor = stats.form,
                 #control = control.simulate.network(MCMC.burnin=10000)
                       )

      cat("Total number of edges is ",
          network.edgecount(nw), "\n")
      cat("Number of alive edges is ",
          network.edgecount(network.extract(nw, at=time)),
          "\n\n") 
        
    }
