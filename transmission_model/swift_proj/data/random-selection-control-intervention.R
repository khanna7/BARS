## 18 APR 2018: UPDATE to fixed params post-art-debug

rm(list=ls())
library(dplyr)

#2018-Sep-25: 30 repetitions
# 2018-July-18: Update using new parameterization scheme
# 2018-Feb-1: But parameters that are run through calibration need to be fixed to those values
# 2018-Jan-31: Generate PrEP retention and scale intervention inputs

# Set Parameters ----

# seed
global.random.seed <- 1:30

# prep
prep.uptake <- 'default'
default.prep.years.to.increment <- 5
default.prep.bl.use.prop.lt <- 12.7/100
default.prep.bl.use.prop.gte <- 14.7/100

prep.scaleup.targets <- seq(20, 60, by=10)/100
default.prep.yearly.increment.lt <- c(0, prep.scaleup.targets - default.prep.bl.use.prop.lt )/default.prep.years.to.increment
default.prep.yearly.increment.gte <- c(0, prep.scaleup.targets - default.prep.bl.use.prop.gte)/default.prep.years.to.increment

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
    default.prep.yearly.increment.lt = default.prep.yearly.increment.lt,
    default.prep.years.to.increment = default.prep.years.to.increment,
    acute.mult = acute.mult, #fix calibration inputs
    late.mult = late.mult,
    min.chronic.infectivity.unadj = min.chronic.infectivity.unadj,
    prop.steady.sex.acts = prop.steady.sex.acts,
    prop.casual.sex.acts = prop.casual.sex.acts,
    circum.mult = circum.mult
  )

df$default.prep.yearly.increment.gte = rep(default.prep.yearly.increment.gte, each=length(global.random.seed))

run.number <- 1:nrow(df)
df <- cbind(run.number, df)

ndf <- names(df)

l1 <- lapply(1:ncol(df), function(x) paste0(ndf[x],"=",df[[ndf[x]]]))
res <- do.call(paste,c(l1,c(sep = ",")))

## write as text
write(res, file = "random-selection.txt")

## write df as separate csv to make it easier to search
write.csv(df, "random-selection.csv")
