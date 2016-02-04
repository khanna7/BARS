## test initialized model

rm(list=ls())

   ## libraries and data
   library(networkDynamic)
   library(network)

   load("initialized-model.RData")

   ## testing code
   net.f #cross-sectional network
   table(net.f %v% "inf.status", exclude=NULL)
   table(net.f %v% "art.status", exclude=NULL)
   summary(net.f %v% "cd4.count.today", exclude=NULL)
   summary(net.f %v% "viral.load.today", exclude=NULL)
   summary(net.f %v% "time.since.art.initiation", exclude=NULL)

   xtabs(~ factor(net.f %v% "inf.status", exclude=NULL) +
             factor(net.f %v% "art.status", exclude=NULL) )


