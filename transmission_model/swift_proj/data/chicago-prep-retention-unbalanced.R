rm(list=ls())
library(dplyr)

##2018Sep27: increase repetitions to 30
##2018JUly18: Update parameterization as per v0.42.
##2018June11: Make sure parameters here are only non-derived parameters. 
  ## derived params are updated automatically. for prep retention, mean days of usage.
##2018June6: Fix stopping and starting parameters here for unbalanced PrEP uptake
##2018May25: Run with updated calibration model (PrEP usage duration = 1yr)
##2018May20: Run with updated calibration model (time to vl supp = 1m)
##2018Apr18: Update fixed parameters to calibrated values derived post-ART-debug
## 20180Apr-6: In the unbalanced model, alpha cannot be varied -- commented it out here.
## 2018-Apr-2: Unbalanced init/cess/total users of PrEP, consider fewer age-based ratios than previous analysis
## 2018-Feb-28: Add age-based picking 
# 2018-Feb-1: But parameters that are run through calibration need to be fixed to those values
# 2018-Jan-31: Generate PrEP retention and scale intervention inputs

# Set Parameters ----

# seed
global.random.seed <- 1:30

# prep
prep.uptake <- 'default'
default.prep.balanced.unbalanced <- 'unbalanced'
default.prep.mean.days.usage.lt <- c(365, 540, 730, 900, 1095, 1275, 1440)
default.prep.mean.days.usage.gte <- default.prep.mean.days.usage.lt
  
# Fix calibration values to "instance_242" #updated on 2018-05-25 -----

acute.mult=5
late.mult=1
min.chronic.infectivity.unadj=0.000922913
prop.steady.sex.acts=0.1893571  
prop.casual.sex.acts=0.053
circum.mult=1
   
# Make parameter grid ----

df <- 
  expand.grid(
    global.random.seed = global.random.seed, 
    default.prep.balanced.unbalanced = default.prep.balanced.unbalanced,
    default.prep.mean.days.usage.lt = default.prep.mean.days.usage.lt,
    acute.mult = acute.mult, #fix calibration inputs
    late.mult = late.mult,
    min.chronic.infectivity.unadj = min.chronic.infectivity.unadj,
    prop.steady.sex.acts = prop.steady.sex.acts,
    prop.casual.sex.acts = prop.casual.sex.acts,
    circum.mult = circum.mult
  )

df$default.prep.mean.days.usage.gte = rep(default.prep.mean.days.usage.gte, each=length(global.random.seed))

run.number <- 1:nrow(df)
df <- cbind(run.number, df)

ndf <- names(df)

l1 <- lapply(1:ncol(df), function(x) paste0(ndf[x],"=",df[[ndf[x]]]))
res <- do.call(paste,c(l1,c(sep = ",")))

# Write upfs -----   

# as text
write(res, file = "prep-unbalanced.txt")

# as csv 
write.csv(df, "prep-unbalanced.csv")

