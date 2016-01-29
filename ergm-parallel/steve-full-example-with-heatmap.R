# Demonstration of imputation of alter-alter ties using degree with
#      with respondents as one predictor of the missing ties

##########################
# params to set
nresp <- 1000
nalters <- 1000
mdeg <- 25

##########################
# Process
library(ergm)
n <- nresp + nalters

###########################
# Create init net

mynet <- network.initialize(n, directed=F)
greg <- runif(n) # just to get extra variation in underlying observed degree
mynet <- set.vertex.attribute(mynet, 'greg', greg)
myfit <- ergm(mynet~edges+nodecov('greg'),
              target.stats=c(mdeg*n/2,1.1*mdeg*n/2)) # The 1.1 means that those with higher 'gregariousness' will form more ties.
mynet <- simulate(myfit)
mynet[(nresp+1):n,(nresp+1):n] <- NA    # Make the missing missing (quick way to set edges in corresponding adj. matrix = NA)
summary(mynet~edges)

# Check that NA's are registering in
#aaa <- ergm(mynet~edges)$coef
#density <- exp(aaa)/(1+exp(aaa))
#density * n *(n-1) / 2
#density * (((nresp)*(nresp-1)/2) + (nresp*nalters))

# Calculate
respdeg <- sapply(1:n, function(x) sum(mynet[x,1:nresp])) # Calculate degree with respondents
#plot(respdeg, greg) # Confirm that those with higher "greg" have higher degree
mynet <- set.vertex.attribute(mynet, 'respdeg', respdeg)  # Make nodal attribute

# Fit ergm with nodecov(respdeg), simulate leaving obs ties as is, compile into matrix, calc freqs that each tie is present across 100 nets
newfit <- ergm(mynet~edges+nodecov('respdeg'))
output <- simulate(newfit, constraints=~observed, nsim=100)
nets <- array(NA, dim=c(n,n,100)) ## 100 adjacency matrices
for (i in 1:100) nets[,,i] <- output[[i]][,] #adjacency matrix for each output
freq <- apply(nets,c(1,2),sum)
image(freq, xlim=c(0,1), ylim=c(1,0), col=gray(32:0/32))
## It worked!! observed dyads are present either 0% or 100% of the time;
##     unobserved are present stoachstically, with those involving folks
##     with higher observed degree having higher probability

## # try it again, this time adding a gwesp term
## newfit2 <- ergm(mynet~edges+nodecov('respdeg')+gwesp(1, fixed=T))
## output2 <- simulate(newfit2, constraints=~observed, nsim=100)
## nets2 <- array(NA, dim=c(n,n,100))
## for (i in 1:100) nets2[,,i] <- output2[[i]][,]
## freq2 <- apply(nets2,c(1,2),sum)
## image(freq2, xlim=c(0,1), ylim=c(1,0), col=gray(32:0/32))
## # Works again!! Of course we there is no
## #      shared partner clustering in the network given the model we built it from,
## #      so the gwesp term is not significant - but at least we see the mechanics work

## # Then, calculate all sorts of centrality scores across the 100 networks saved in output2
