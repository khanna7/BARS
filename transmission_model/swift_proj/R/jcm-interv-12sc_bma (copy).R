rm(list=ls())

global.random.seed <- 1:30
post.release.interference.period.mean.p1 <- 90 #T1
#released.partner.expiration.time.pl <- c(0, 30, 60, 90)
intervention.time <- 10950
#net.multiple<- c(0.75,0.8125,0.875,0.9375,1)

net.multiple<- c(0.1,0.25,0.5,0.75,1)



T1<-c(90,720)#c(90,720)#c(90,180,270,360,720)
T2<-c(180)
T3<-c(180)
#df<-df[c(1:30,10231:10260,20461:20490,30691:30720),]

dur.low<-19.1	
dur.med<-58.4	
dur.high<-97.7


with.low<-0.0000398	
with.med<-0.0000787
with.high<-0.0001559
  	
cji.low<-0.0003583	
cji.med<-0.0005173	
cji.high<-0.0007469





df <- expand.grid(global.random.seed=global.random.seed,
                  incarceration.prob=0.0001559, #0.0001366,
                  incarceration.with.cji.prob=0.0007469, #0.0008196,
                  jail.serving.time.mean=58.4,
                  recently.jailed.time=180,
                  not.recently.jailed.time=180,
   	              post.release.interference.period.mean=90,
		         #network.retention.multiplier=net.multiple,
		          jail.infection.rate.multiplier=.5,
		          is.network.disruption.on = "true",
		          is.care.disruption.on = "true",
		          is.post.release.art.on = "false",
				  #post.release.interference.period.mean.update=paste0(intervention.time,"|",T1),
				  released.partner.expiration.time=T3,
				  partner.was.jailed.expiration.time=180,
				  release.partner.formation.time=T2
)

net.multiplier1 <- expand.grid(global.random.seed=global.random.seed,
                  incarceration.prob=0.0001559, #0.0001366,
                  incarceration.with.cji.prob=0.0007469, #0.0008196,
                  jail.serving.time.mean=58.4,
                  recently.jailed.time=180,
                  not.recently.jailed.time=180,
                   post.release.interference.period.mean=90,
                  network.retention.multiplier=.75,
                  jail.infection.rate.multiplier=.5,
                  is.network.disruption.on = "true",
                  network.retention.multiplier.update=paste0(intervention.time,"|",net.multiple),
                  is.care.disruption.on = "true",
                  is.post.release.art.on = "false",
                  post.release.interference.period.mean.update=paste0(intervention.time,"|",90),
                  released.partner.expiration.time=T3,
                  partner.was.jailed.expiration.time=180,
                  released.partner.formation.time=180
)

df <- expand.grid(global.random.seed=global.random.seed,
                       incarceration.prob=0.0001559, #0.0001366,
                       incarceration.with.cji.prob=0.0007469, #0.0008196,
                       jail.serving.time.mean=58.4,
                       recently.jailed.time=180,
                       not.recently.jailed.time=180,
                       post.release.interference.period.mean=90,
                       network.retention.multiplier=.75,
                       jail.infection.rate.multiplier=.5,
                       is.network.disruption.on = "true",
                       is.care.disruption.on = "true",
                       is.care.disruption.on.update = "10950|TRUE",
                       is.post.release.art.on = "false",
                       is.post.release.art.on.update = "10950|FALSE",
                       post.release.interference.period.mean.update=paste0(intervention.time,"|",T1),
                       released.partner.expiration.time=T3,
                       partner.was.jailed.expiration.time=180,
                       release.partner.formation.time=T2
)






nochaos <- expand.grid(global.random.seed=global.random.seed,
                            incarceration.prob=0.0001559, #0.0001366,
                            incarceration.with.cji.prob=0.0007469, #0.0008196,
                            jail.serving.time.mean=58.4,
                            recently.jailed.time=180,
                            not.recently.jailed.time=180,
                            post.release.interference.period.mean=90,
                            network.retention.multiplier=.75,
                            jail.infection.rate.multiplier=.5,
                            is.network.disruption.on = "true",
                            is.care.disruption.on = "true",
                            is.care.disruption.on.update = "10950|FALSE",
                            is.post.release.art.on = "false",
                            is.post.release.art.on.update = "10950|FALSE",
                            post.release.interference.period.mean.update=paste0(intervention.time,"|",1),
                            released.partner.expiration.time=T3,
                            partner.was.jailed.expiration.time=180,
                            release.partner.formation.time=T2
)

targetedcare <- expand.grid(global.random.seed=global.random.seed,
                       incarceration.prob=0.0001559, #0.0001366,
                       incarceration.with.cji.prob=0.0007469, #0.0008196,
                       jail.serving.time.mean=58.4,
                       recently.jailed.time=180,
                       not.recently.jailed.time=180,
                       post.release.interference.period.mean=90,
                       network.retention.multiplier=.75,
                       jail.infection.rate.multiplier=.5,
                       is.network.disruption.on = "true",
                       is.care.disruption.on = "true",
                       is.care.disruption.on.update = "10950|FALSE",
                       is.post.release.art.on = "false",
                       is.post.release.art.on.update = "10950|TRUE",
                       post.release.interference.period.mean.update=paste0(intervention.time,"|",1),
                       released.partner.expiration.time=T3,
                       partner.was.jailed.expiration.time=180,
                       release.partner.formation.time=T2
)

# 
# 
# targetedcare <- expand.grid(global.random.seed=global.random.seed,
#                        incarceration.prob=0.0001559, #0.0001366,
#                        incarceration.with.cji.prob=0.0007469, #0.0008196,
#                        jail.serving.time.mean=58.4,
#                        recently.jailed.time=180,
#                        not.recently.jailed.time=180,
#                         post.release.interference.period.mean=90,
#                           network.retention.multiplier=.75,
#                        jail.infection.rate.multiplier=.5,
#                        is.network.disruption.on = "true",
#                        is.care.disruption.on = "false",
#                        is.post.release.art.on = "true",
#                        post.release.interference.period.mean.update=paste0(intervention.time,"|",1),
#                        released.partner.expiration.time=T3,
#                        partner.was.jailed.expiration.time=180,
#                        release.partner.formation.time=T2
# )


df<-rbind(df,nochaos,targetedcare)



# 
# - Fix withOUT CJI probability to 0.000156.
# - Fix with CJI probability to 0.000747. 
# - HIV incidence ratio to 0.5 
# - Mean duration of incarceration to 58.4, 


# df<-expand.grid(global.random.seed=global.random.seed,
#                 incarceration.prob=with.high,
#                 incarceration.with.cji.prob=cji.high,
#                 jail.serving.time.mean=58.4,
#                 jail.infection.rate.multiplier=.5
# )

#df<-df[c(1:30,2551:2580,5101:5130,7651:7680),]
df<-net.multiplier1
run.number <- 1:nrow(df)
df <- cbind(run.number, df)
#T2 is supposed to be set equal to whatever T1 is, so rather than expand it by all of it's combinations
#We should append release.partner.formation.time
#df$release.partner.formation.time<-df$post.release.interference.mean
ndf <- names(df)

l1 <- lapply(1:ncol(df), function(x) paste0(ndf[x],"=",df[[ndf[x]]]))
upf <- do.call(paste,c(l1,c(sep = ",")))

write(upf, file = "/project2/khanna7/francis/sch_inter/BARS/transmission_model/swift_proj/data/NetworkMultiplier.txt")

