## example using statnet: estimation code

## Model description:

   ## All actors between 0 and 50 timesteps of age
   ## Age at recruitment = 0, age at death = 50
   ## additional mortality per time step = 0.1%
   ## Birth rate=0.1%

   ## Population description:
   ## 5000 individuals
   ## 3000 edges
   ## mean partnership duration = 10 timesteps
   ## simulation over 100 timesteps

rm(list=ls())

N <- 5000
duration <- 10
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
target.stats <- c(n.edges, c(1/4*n.edges, 1/2*n.edges)) 
                #50% of partnerships will be btwn "young" individuals 

constraints <- ~.

fit <- ergm(formation.n0,
            target.stats=target.stats,
            constraints=constraints,
            verbose=FALSE)      

theta.form <- fit$coef
theta.form[1] <- theta.form[1]-theta.diss

save.image(file="example-estimation.RData")
