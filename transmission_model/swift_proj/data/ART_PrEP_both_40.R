# Abby's code to generate upf for ART, PrEP interventions

rm(list=ls())
library(dplyr)

## seed
global.random.seed <- 1:30

prep.bl.use.prop.lt.init <- 12.7/100 # actual values 12.7 and 14.7, took average here
prep.bl.use.prop.gte.init <- 14.7/100

prep.years.to.increment <- 14
uptake.values.to.achieve <- c(13.7, 40)/100 #20, 30, 40, 50, 60)/100

prep.uptake <- 'default'
prep.yearly.increment.lt <- c(0, (uptake.values.to.achieve[-1] - prep.bl.use.prop.lt.init)/prep.years.to.increment)
prep.yearly.increment.gte <- c(0, (uptake.values.to.achieve[-1] - prep.bl.use.prop.gte.init)/prep.years.to.increment)

## Fix calibration values to "instance_25"
acute.mult=5
late.mult=1
min.chronic.infectivity.unadj=0.0009229133
prop.steady.sex.acts=0.1893571
prop.casual.sex.acts=0.053
circum.mult=1

#ART Adherence parameters
art.prop.never.adherent <- .1        #.05 month to month
art.prop.part.plus.adherent <- .28   #.66 month to month 
art.prop.part.neg.adherent <- .3     #.33 month to month
art.prop.always.adherent <- .32     #.95 month to month

## make parameter grid
df_full <- expand.grid(
  global.random.seed = global.random.seed, #seed
   acute.mult = acute.mult, #fix calibration inputs
   late.mult = late.mult,
   min.chronic.infectivity.unadj = min.chronic.infectivity.unadj,
   prop.steady.sex.acts = prop.steady.sex.acts,
   prop.casual.sex.acts = prop.casual.sex.acts,
   circum.mult = circum.mult,
   default.prep.yearly.increment.lt = prep.yearly.increment.lt, # prep inputs
   default.prep.years.to.increment = prep.years.to.increment,
   art.prop.always.adherent = c(.32,1),
   art.prop.part.plus.adherent = c(.28,0),
   art.prop.part.neg.adherent = c(.3,0),
   art.prop.never.adherent = c(.1,0)
   
 )

df_full$default.prep.yearly.increment.gte = rep(prep.yearly.increment.gte, each=length(global.random.seed))
df_full <- filter (df_full, (art.prop.always.adherent==.32 & art.prop.part.plus.adherent==.28 & art.prop.part.neg.adherent==.3 & art.prop.never.adherent== .1) | (art.prop.always.adherent==1 & art.prop.part.plus.adherent==0 & art.prop.part.neg.adherent==0 & art.prop.never.adherent== 0))


run.number <- 1:nrow(df_full)
df_full <- cbind(run.number, df_full)

ndf <- names(df_full)
numcols <- ncol(df_full)

l1 <- lapply(1:numcols, function(x) paste0(ndf[x],"=",df_full[[ndf[x]]]))
res <- do.call(paste,c(l1,sep = ","))

## write as text
write(res, file = "abby-ART_ad_prep.txt")

## write df as separate csv to make it easier to search
write.csv(df_full, "abby-ART_ad_prep.csv")


res1_to_60 <- res[1:60]
#df1_to_60 <- df[1:60,]

