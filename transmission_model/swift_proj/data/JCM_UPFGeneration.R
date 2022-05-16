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
    incarceration.prob=0.0001559,
    incarceration.with.cji.prob=0.0007469,
    jail.serving.time.mean=58.4,
    recently.jailed.time=180,
    not.recently.jailed.time=180,
    post.release.interference.period.mean=90,
    network.retention.multiplier=0.75,
    jail.infection.rate.multiplier=0.5,
    is.network.disruption.on="true",
    is.care.disruption.on=c("true","true","false","false"),
    released.partner.expiration.time=180,
    partner.was.jailed.expiration.time=180,
    release.partner.formation.time=0
  )

is.care.disruption.on.update=rep(c("10950|TRUE","10950|TRUE","10950|FALSE","10950|FALSE"),each=30)
is.post.release.art.on=rep(c("false","false","false","true"),each=30)
is.post.release.art.on.update=rep(c("10950|FALSE","10950|FALSE","10950|FALSE","10950|TRUE"),each=30)
post.release.interference.period.mean.update=rep(c("10950|90","10950|720","10950|1","10950|1"),each=30)

df<-cbind(df,is.care.disruption.on.update,is.post.release.art.on,is.post.release.art.on.update,post.release.interference.period.mean.update)
names(df)[15:18]<-c("is.care.disruption.on.update","is.post.release.art.on","is.post.release.art.on.update","post.release.interference.period.mean.update")
ndf <- names(df)

l1 <- lapply(1:18,function(x) paste0(ndf[x],"=",df[[ndf[x]]]))
res <- do.call(paste,c(l1,c(sep = ",")))
#write(res,file = "baseline.txt")
#write(res,file = "net_disr.txt")
#write(res,file = "net_disr_low.txt")
#write(res,file = "net_disr_high.txt")
#write(res,file = "care_disr.txt")
write(res,file = "CareInterventionsFinal.txt")
#write(res,file = "care_disr_high.txt")
#write(res,file = "both_disr.txt")



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
    incarceration.prob=0.0001559,
    incarceration.with.cji.prob=0.0007469,
    jail.serving.time.mean=58.4,
    recently.jailed.time=180,
    not.recently.jailed.time=180,
    post.release.interference.period.mean=90,
    network.retention.multiplier=c(0.1,0.25,0.5,0.75,1),
    jail.infection.rate.multiplier=0.5,
    is.network.disruption.on="true",
    is.care.disruption.on=c("true"),
    released.partner.expiration.time=180,
    is.care.disruption.on.update=c("10950|TRUE"),
    is.post.release.art.on.update=c("10950|FALSE"),
    is.post.release.art.on=c("false"),
    post.release.interference.period.mean.update=c("10950|90"),
    partner.was.jailed.expiration.time=180,
    release.partner.formation.time=0
  )


ndf <- names(df)

l1 <- lapply(1:18,function(x) paste0(ndf[x],"=",df[[ndf[x]]]))
res <- do.call(paste,c(l1,c(sep = ",")))

write(res,file = "NetInterventionsFinal.txt")

