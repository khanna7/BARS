## example using statnet

## Packages and libraries
library(network)# 3 Sep 2013
library(ergm)#3 Sep 2013
library(tergm)
library(networkDynamic)
      
N <- 5000
duration <- 25
theta.diss <- log(duration-1)
n.edges <- 3000

n0 <- network.initialize(N, directed=FALSE)
n0%v%"age" <- sample(0:30, N, replace=TRUE)
age <- n0%v%"age"
age.less.than.15 <- which(age <= 15)
n0%v%"young" <- rep(0, length=N)
set.vertex.attribute(n0, "young", 1, age.less.than.15)

formation <- ~edges+nodemix("young", base=1)
formation.n0 <- update.formula(formation, n0~.)
dissolution <- ~offset(edges)
target.stats <- c(n.edges, c(1/5*n.edges, 1/2*n.edges)) 
#50% of partnerships will be btwn "young" individuals 

constraints <- ~.

fit <- ergm(formation.n0,
            target.stats=target.stats,
            constraints=constraints,
            verbose=FALSE)      

theta.form <- fit$coef
theta.form[1] <- theta.form[1]-theta.diss

nw <- fit$network
formation <- fit$formula
dissolution <- ~offset(edges)
popsize <- network.size(nw)

## Start with all vertices and edges active
nw <- activate.vertices(nw,onset=1,terminus=Inf)
nw <- activate.edges(nw,onset=1,terminus=Inf)

nw <- simulate(nw,
               formation = formation, 
               dissolution = dissolution,
               coef.form = theta.form, 
               coef.diss = theta.diss,
               constraints = constraints,
               #time.start = ts,
               time.start = 2,
               #start.time = time,
               time.slices = 1#,
               #monitor = stats.form,
               #control = control.simulate.network(MCMC.burnin=10000)
)
