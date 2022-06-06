carebase.rel<-do.call(rbind,list_inc)[1:4*40,]
save(carebase.rel,file="CareEffectRatiosBaseRel.Rdata")
#General
d1<-apply(netbase,1,mean)/apply(netbase,1,mean)[3]

set.seed(1000)
a<-list()
b<-list()

for(i in 1:4){
  a[[i]]<-lapply(1:1000,function(u){sample(carebase.rel[i,],30,replace=TRUE)})
  b[[i]]<-lapply(a,function(j){z<-unlist(j);q<-matrix(z,nrow=30,ncol=1000);q1<-apply(q,2,mean);return(q1)})
}
d<-matrix(NA,nrow=4,ncol=2)
d[1,]<-quantile((b[[4]][[1]])-(b[[4]][[3]]),c(0.025,.975))
d[2,]<-quantile((b[[4]][[2]])-(b[[4]][[3]]),c(0.025,.975))
d[3,]<-quantile((b[[4]][[4]])-(b[[4]][[3]]),c(0.025,.975))
d[4,]<-quantile((b[[4]][[3]])-(b[[4]][[1]]),c(0.025,.975))


