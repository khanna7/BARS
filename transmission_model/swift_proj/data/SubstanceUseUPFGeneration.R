remove(list=ls())




df.bc <- 
  expand.grid(
    global.random.seed = seq(1, 30, 1),
    acute.mult=5,
    late.mult=1,
    min.chronic.infectivity.unadj=0.000922913,
    prop.steady.sex.acts=0.1893571,
    prop.casual.sex.acts=0.053,
    circum.mult=1,
    counseling.and.behavioral.treatment.use.prop=c(0.1,0.15,0.2,0.25),
    counseling.and.behavioral.treatment.length=c(60,90),
    substance.use.intervention="bc"
  )
run.number <- 1:nrow(df.bc)
df.bc <- cbind(run.number, df.bc)

ndf <- names(df.bc)



l1 <- lapply(1:10,function(x) paste0(ndf[x],"=",df.bc[[ndf[x]]]))
res <- do.call(paste,c(l1,c(sep = ",")))
write(res,file = "drug-bc.txt")

df.bm <- 
  expand.grid(
    global.random.seed = seq(1, 30, 1),
    acute.mult=5,
    late.mult=1,
    min.chronic.infectivity.unadj=0.000922913,
    prop.steady.sex.acts=0.1893571,
    prop.casual.sex.acts=0.053,
    circum.mult=1,
    mirtazapine.treatment.use.prop=c(0.2,0.4,0.6,0.8,1),
    mirtazapine.treatment.adherence.prop=0.485,
    mirtazapine.treatment.length=c(90,365),
    substance.use.intervention="mirtazapine"
    
  )
run.number <- 1:nrow(df.bm)
df.bm <- cbind(run.number, df.bm)
ndf <- names(df.bm)

l1 <- lapply(1:10,function(x) paste0(ndf[x],"=",df.bm[[ndf[x]]]))
res <- do.call(paste,c(l1,c(sep = ",")))
write(res,file = "drug-bm.txt")

df.bc20 <- 
  expand.grid(
    global.random.seed = seq(1, 30, 1),
    acute.mult=5,
    late.mult=1,
    min.chronic.infectivity.unadj=0.000922913,
    prop.steady.sex.acts=0.1893571,
    prop.casual.sex.acts=0.053,
    circum.mult=1,
    counseling.and.behavioral.treatment.use.prop=c(0.1,0.15,0.2,0.25),
    counseling.and.behavioral.treatment.length=c(60,90),
    substance.use.intervention="bc",
    default.prep.yearly.increment.lt=0.02,
    default.prep.yearly.increment.gte=0.02,
    default.prep.years.to.increment=10,
    art.prop.always.adherent=1,
    art.prop.never.adherent=0,
    art.prop.part.plus.adherent=0,
    art.prop.part.neg.adherent=0
  )
run.number <- 1:nrow(df.bc20)
df.bc20 <- cbind(run.number, df.bc20)

ndf <- names(df.bc20)

l1 <- lapply(1:18,function(x) paste0(ndf[x],"=",df.bc20[[ndf[x]]]))
res <- do.call(paste,c(l1,c(sep = ",")))
write(res,file = "drug-bcplusprepandart20.txt")


df.bc30 <- 
  expand.grid(
    global.random.seed = seq(1, 30, 1),
    acute.mult=5,
    late.mult=1,
    min.chronic.infectivity.unadj=0.000922913,
    prop.steady.sex.acts=0.1893571,
    prop.casual.sex.acts=0.053,
    circum.mult=1,
    counseling.and.behavioral.treatment.use.prop=c(0.1,0.15,0.2,0.25),
    counseling.and.behavioral.treatment.length=c(60,90),
    substance.use.intervention="bc",
    default.prep.yearly.increment.lt=0.03,
    default.prep.yearly.increment.gte=0.03,
    default.prep.years.to.increment=10,
    art.init.lag.lt.1="0.9|0-7",
    art.init.lag.lt.2="0.1|7-30",
    art.init.lag.lt.3="0|30-90",
    art.init.lag.lt.4="0|90-180",
    art.init.lag.lt.5="0|180-365",
    art.init.lag.lt.6="0|365-730",
    art.init.lag.lt.7="0|1825-1825",
    art.init.lag.gte.1="0.9|0-7",
    art.init.lag.gte.2="0.1|7-30",
    art.init.lag.gte.3="0|30-90",
    art.init.lag.gte.4="0|90-180",
    art.init.lag.gte.5="0|180-365",
    art.init.lag.gte.6="0|365-730",
    art.init.lag.gte.7="0|730-1825",
    default.prep.years.to.increment=10,
    art.prop.always.adherent=.82,
    art.prop.never.adherent=0.05,
    art.prop.part.plus.adherent=0.08,
    art.prop.part.neg.adherent=0.05
  )
run.number <- 1:nrow(df.bc30)
df.bc30 <- cbind(run.number, df.bc30)
ndf <- names(df.bc30)

l1 <- lapply(1:33,function(x) paste0(ndf[x],"=",df.bc30[[ndf[x]]]))
res <- do.call(paste,c(l1,c(sep = ",")))
write(res,file = "drug-bcplusprepandart30.txt")

