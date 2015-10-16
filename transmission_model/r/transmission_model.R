rm(list=ls())

suppressMessages(library(ergm))
suppressMessages(library(tergm))
suppressMessages(library(network))
suppressMessages(library(networkDynamic))

set.seed(1)

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

inf.status <- rbinom(N, 1, 0.1)
n0%v%"inf.status" <- inf.status

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

nw.active <- network.extract(nw,
                at = 1,
                retain.all.vertices = T)

# holds any code that I want to call from 
# C++ to make sure changes there are being
# reflected here.
nw_debug <- function() {
  #print(nw.active$val[[1]]$age)
  print(nw$val[[1]]$age)
  #nw.active <<- network.extract(nw,at = 2, retain.all.vertices = T)
  age <- nw$val[[1]]$age
  nw <<- set.vertex.attribute(nw, "age", age + 1, v=c(1))
  #print(nw.active$val[[1]]$age)
  print(nw$val[[1]]$age)
} 

nw_simulate <-
  function() {
    #print("sim start")
    nw <<- simulate(nw,
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
    #print("sim end")
}


