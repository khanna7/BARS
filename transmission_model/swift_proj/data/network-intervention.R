rm(list=ls())
library(dplyr)

##018Sep27: 30 repetitions, reduce range of topn
##2018July9: Adapted from serodiscordant.R

# Set Common Parameters ----

# seed
global.random.seed <- 1:30

# prep
#prep.scaleup.targets <- seq(20, 60, by=10)/100
prep.scaleup.targets <- c(20, 30, 40, 50, 60)/100

# Set Eigen-Specific Parameters ----

#prep.uptake <- 'eigen' #set in function call
eigen.base.prep.bl.use.prop.lt <- 12.7/100
eigen.base.prep.bl.use.prop.gte <- 14.7/100
eigen.intrv.prep.years.to.increment <- 5
eigen.intrv.prep.yearly.increment.lt <- c(0, prep.scaleup.targets - eigen.base.prep.bl.use.prop.lt)/eigen.intrv.prep.years.to.increment
eigen.intrv.prep.yearly.increment.gte <- c(0, prep.scaleup.targets - eigen.base.prep.bl.use.prop.gte)/eigen.intrv.prep.years.to.increment
#eigen.intrv.prep.topn <- c(0, 1:4, seq(5, 50, by=5))/100
#eigen.intrv.prep.topn <- c(0, 1, seq(10, 50, by=10))/100
eigen.intrv.prep.topn <- c(0.1, 1, 10, 25, 50)/100

# Fix calibration values to "instance_242" #updated on 2018-05-25 ----

acute.mult=5
late.mult=1
min.chronic.infectivity.unadj=0.000922913
prop.steady.sex.acts=0.1893571  
prop.casual.sex.acts=0.053
circum.mult=1
   
## make parameter grid

df <- 
  expand.grid(
    global.random.seed = global.random.seed, #seed
    prep.uptake = 'eigen',
    eigen.base.prep.bl.use.prop.lt = eigen.base.prep.bl.use.prop.lt,
    eigen.intrv.prep.yearly.increment.lt = eigen.intrv.prep.yearly.increment.lt,
    eigen.intrv.prep.topn = eigen.intrv.prep.topn,
    acute.mult = acute.mult, #fix calibration inputs
    late.mult = late.mult,
    min.chronic.infectivity.unadj = min.chronic.infectivity.unadj,
    prop.steady.sex.acts = prop.steady.sex.acts,
    prop.casual.sex.acts = prop.casual.sex.acts,
    circum.mult = circum.mult
  )

df$eigen.base.prep.bl.use.prop.gte = rep(eigen.base.prep.bl.use.prop.gte, each=length(global.random.seed))
df$eigen.intrv.prep.yearly.increment.gte = rep(eigen.intrv.prep.yearly.increment.gte, each=length(global.random.seed))

run.number <- 1:nrow(df)
df <- cbind(run.number, df)
ndf <- names(df)

l1 <- lapply(1:ncol(df), function(x) paste0(ndf[x],"=",df[[ndf[x]]]))
res <- do.call(paste,c(l1,c(sep = ",")))

## write as text
write(res, file = "eigen.txt")

## write df as separate csv to make it easier to search
write.csv(df, "eigen.csv")


# Set Degree-Specific Parameters ----

## make parameter grid
df <- 
  expand.grid(
    global.random.seed = global.random.seed, #seed
    prep.uptake = 'degree',
    degree.base.prep.bl.use.prop.lt = eigen.base.prep.bl.use.prop.lt,
    degree.intrv.prep.yearly.increment.lt = eigen.intrv.prep.yearly.increment.lt,
    degree.intrv.prep.topn = eigen.intrv.prep.topn,
    acute.mult = acute.mult, #fix calibration inputs
    late.mult = late.mult,
    min.chronic.infectivity.unadj = min.chronic.infectivity.unadj,
    prop.steady.sex.acts = prop.steady.sex.acts,
    prop.casual.sex.acts = prop.casual.sex.acts,
    circum.mult = circum.mult
  )

df$degree.base.prep.bl.use.prop.gte = rep(eigen.base.prep.bl.use.prop.gte, each=length(global.random.seed))
df$degree.intrv.prep.yearly.increment.gte = rep(eigen.intrv.prep.yearly.increment.gte, each=length(global.random.seed))

run.number <- 1:nrow(df)
df <- cbind(run.number, df)
ndf <- names(df)

l1 <- lapply(1:ncol(df), function(x) paste0(ndf[x],"=",df[[ndf[x]]]))
res <- do.call(paste,c(l1,c(sep = ",")))

## write as text
write(res, file = "degree.txt")

## write df as separate csv to make it easier to search
write.csv(df, "degree.csv")
