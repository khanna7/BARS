rm(list=ls())
library(dplyr)

##2018Sep27: Increase retentions to 30
##2018July9: PrEP yearly incremement is broken down to `lt` and `gte`. Names of some associated parameters are changed as well.
##2018July5: Yearly PrEP increment should be less than 1/default.prep.years.to.increment, 
## where `default.prep.years.to.increment` is 
##the number of years that the intervention scales over.
##2018June21: Create parameter file for SDC intervention

# Set Parameters ----

# seed
global.random.seed <- 1:30

# prep
prep.uptake <- 'serodiscordant'
serodiscordant.intrv.prep.network.type <- c('main', 'all')
prep.balanced.unbalanced <- "unbalanced"
serodiscordant.base.prep.bl.use.prop.lt <- 12.7/100
serodiscordant.base.prep.bl.use.prop.gte <- 14.7/100

serodiscordant.base.prep.mean.days.usage.lt <- 365 #usage length for everyone in interv phase
serodiscordant.base.prep.mean.days.usage.gte <- 365
serodiscordant.intrv.prep.mean.days.usage.lt <- 365 #usage length for serodiscordants only
serodiscordant.intrv.prep.mean.days.usage.gte <- 365

serodiscordant.intrv.prep.years.to.increment <- 5

prep.scaleup.targets <- seq(20, 60, by=10)/100
serodiscordant.intrv.prep.yearly.increment.lt <- c(0, prep.scaleup.targets - serodiscordant.base.prep.bl.use.prop.lt)/serodiscordant.intrv.prep.years.to.increment
serodiscordant.intrv.prep.yearly.increment.gte <- c(0, prep.scaleup.targets - serodiscordant.base.prep.bl.use.prop.gte)/serodiscordant.intrv.prep.years.to.increment

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
  prep.uptake = prep.uptake,
  serodiscordant.intrv.prep.network.type = serodiscordant.intrv.prep.network.type,
  prep.balanced.unbalanced = prep.balanced.unbalanced,
  serodiscordant.base.prep.bl.use.prop.lt = serodiscordant.base.prep.bl.use.prop.lt,
  serodiscordant.intrv.prep.yearly.increment.lt = serodiscordant.intrv.prep.yearly.increment.lt,
  acute.mult = acute.mult, #fix calibration inputs
  late.mult = late.mult,
  min.chronic.infectivity.unadj = min.chronic.infectivity.unadj,
  prop.steady.sex.acts = prop.steady.sex.acts,
  prop.casual.sex.acts = prop.casual.sex.acts,
  circum.mult = circum.mult
  )

df$serodiscordant.base.prep.bl.use.prop.gte = rep(serodiscordant.base.prep.bl.use.prop.gte, each=length(global.random.seed)*length(serodiscordant.intrv.prep.network.type))
df$serodiscordant.intrv.prep.yearly.increment.gte = rep(serodiscordant.intrv.prep.yearly.increment.gte, each=length(global.random.seed)*length(serodiscordant.intrv.prep.network.type))
  

run.number <- 1:nrow(df)
df <- cbind(run.number, df)

ndf <- names(df)

l1 <- lapply(1:ncol(df), function(x) paste0(ndf[x],"=",df[[ndf[x]]]))
res <- do.call(paste,c(l1,c(sep = ",")))

## write as text
write(res, file = "serodiscordant.txt")

## write df as separate csv to make it easier to search
write.csv(df, "serodiscordant.csv")

