# Example format

#global.random.seed=1,stop.at=3650
#global.random.seed <- 1:30

#Default values: 
#--------------

#stop.at=3650  #10y
#is.network.disruption.on = false;
#is.care.disruption.on = false;
#incarceration.prob = 0.0000787
#incarceration.with.cji.prob = 0.0005173
#in.jail.infection.incidence.rate =  0.000091 
#jail.serving.time.mean = 58
#post.release.interference.period.mean = 90
#network.retention.multiplier = 1

#df <- expand.grid(global.random.seed = seq(1, 10, 1), stop.at = 3650)

df <- 
  expand.grid(
    global.random.seed = seq(1, 30, 1),
    #is.network.disruption.on = "true",
    #network.retention.multiplier = 1.5
    #is.care.disruption.on = 'true',
    is.care.disruption.on = "true",
    post.release.interference.period.mean = 30
    #stop.at = 3650,
    #incarceration.prob = 0.0,
    #incarceration.with.cji.prob = 0.0
   
  )

ndf <- names(df)

l1 <- lapply(1:3,function(x) paste0(ndf[x],"=",df[[ndf[x]]]))
res <- do.call(paste,c(l1,c(sep = ",")))
#write(res,file = "baseline.txt")
#write(res,file = "net_disr.txt")
#write(res,file = "net_disr_low.txt")
#write(res,file = "net_disr_high.txt")
#write(res,file = "care_disr.txt")
write(res,file = "care_disr_low.txt")
#write(res,file = "care_disr_high.txt")
#write(res,file = "both_disr.txt")
