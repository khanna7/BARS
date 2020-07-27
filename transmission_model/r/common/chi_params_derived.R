## initial values of parameters
   
   #####################
   ## NETWORK (steady)

     nedges <- n*mean_deg/2 

     #deg_seq <- c(56.6, 41.2, 1.8)*n/100   
     #duration <- (562+1260)/2
      
     ## role
     pr_versatile_main <- 1-(pr_insertive_main + pr_receptive_main)
     nodematch.inf.status <- 0.75*nedges 
       #28/35 partnerships match on infection status, 
       #almost equal between susc-susc and inf-inf 

     ## ergm parameters for dissolution 
     d.rate <- 1/((max.age - min.age)*365)
     pg <- (duration - 1)/duration
     ps2 <- (1 - d.rate)^2
     theta.diss <- log(pg/(ps2-pg)) #adjusted for death rate, Run 
   
   #####################
   ## DEMOGRAPHIC
   ##daily.entry.rate <- 0.00008
   
   #####################
   ## BIOLOGICAL

  ## (cd4 decline)
  untreated.cd4.perstep.decline <- untreated.cd4.daily.decline*
                                   size.of.timestep
   ## healthy level of CD4: sample from some distribution, or should it be the same for all uninfected men?

   #####################
   ## ART
   cd4.rec.per.timestep <- per.day.cd4.recovery*size.of.timestep
   cd4.recovery.time <- 3*365/size.of.timestep ## CD4 recovery for 3 years
   ## parameter for viral decline (add)
   ## ART cessation parameters (what to consider)

   art.meth.to.distribute <- art.prop.always.adherent * meth.decline.art.always.adherent / 3
   art.prop.never.adherent.meth <- art.prop.never.adherent + art.meth.to.distribute
   art.prop.part.plus.adherent.meth <- art.prop.part.plus.adherent + art.meth.to.distribute
   art.prop.part.neg.adherent.meth <- art.prop.part.neg.adherent + art.meth.to.distribute
   art.prop.always.adherent.meth <- 1 - (art.prop.never.adherent.meth + art.prop.part.plus.adherent.meth + art.prop.part.neg.adherent.meth)

   art.crack.to.distribute <- art.prop.always.adherent * crack.decline.art.always.adherent / 3
   art.prop.never.adherent.crack <- art.prop.never.adherent + art.crack.to.distribute
   art.prop.part.plus.adherent.crack <- art.prop.part.plus.adherent + art.crack.to.distribute
   art.prop.part.neg.adherent.crack <- art.prop.part.neg.adherent + art.crack.to.distribute
   art.prop.always.adherent.crack <- 1 - (art.prop.never.adherent.crack + art.prop.part.plus.adherent.crack + art.prop.part.neg.adherent.crack)

   art.ecstasy.to.distribute <- art.prop.always.adherent * ecstasy.decline.art.always.adherent / 3
   art.prop.never.adherent.ecstasy <- art.prop.never.adherent + art.ecstasy.to.distribute
   art.prop.part.plus.adherent.ecstasy <- art.prop.part.plus.adherent + art.ecstasy.to.distribute
   art.prop.part.neg.adherent.ecstasy <- art.prop.part.neg.adherent + art.ecstasy.to.distribute
   art.prop.always.adherent.ecstasy <- 1 - (art.prop.never.adherent.ecstasy + art.prop.part.plus.adherent.ecstasy + art.prop.part.neg.adherent.ecstasy)

   art.meth.crack.to.distribute <- (art.prop.always.adherent * (1 - meth.decline.art.always.adherent) * (1 -crack.decline.art.always.adherent)) / 3
   art.prop.never.adherent.meth.crack <- art.prop.never.adherent + art.meth.crack.to.distribute
   art.prop.part.plus.adherent.meth.crack <- art.prop.part.plus.adherent + art.meth.crack.to.distribute
   art.prop.part.neg.adherent.meth.crack <- art.prop.part.neg.adherent + art.meth.crack.to.distribute
   art.prop.always.adherent.meth.crack <- 1 - (art.prop.never.adherent.meth.crack + art.prop.part.plus.adherent.meth.crack + art.prop.part.neg.adherent.meth.crack)

   art.crack.ecstasy.to.distribute <- (art.prop.always.adherent * (1 - crack.decline.art.always.adherent) * (1 -ecstasy.decline.art.always.adherent)) / 3
   art.prop.never.adherent.crack.ecstasy <- art.prop.never.adherent + art.crack.ecstasy.to.distribute
   art.prop.part.plus.adherent.crack.ecstasy <- art.prop.part.plus.adherent + art.crack.ecstasy.to.distribute
   art.prop.part.neg.adherent.crack.ecstasy <- art.prop.part.neg.adherent + art.crack.ecstasy.to.distribute
   art.prop.always.adherent.crack.ecstasy <- 1 - (art.prop.never.adherent.crack.ecstasy + art.prop.part.plus.adherent.crack.ecstasy + art.prop.part.neg.adherent.crack.ecstasy)

   art.meth.ecstasy.to.distribute <- (art.prop.always.adherent * (1 - meth.decline.art.always.adherent) * (1 -ecstasy.decline.art.always.adherent)) / 3
   art.prop.never.adherent.meth.ecstasy <- art.prop.never.adherent + art.meth.ecstasy.to.distribute
   art.prop.part.plus.adherent.meth.ecstasy <- art.prop.part.plus.adherent + art.meth.ecstasy.to.distribute
   art.prop.part.neg.adherent.meth.ecstasy <- art.prop.part.neg.adherent + art.meth.ecstasy.to.distribute
   art.prop.always.adherent.meth.ecstasy <- 1 - (art.prop.never.adherent.meth.ecstasy + art.prop.part.plus.adherent.meth.ecstasy + art.prop.part.neg.adherent.meth.ecstasy)

   art.meth.crack.ecstasy.to.distribute <- (art.prop.always.adherent * (1 - meth.decline.art.always.adherent) * (1 - crack.decline.art.always.adherent) * (1 -ecstasy.decline.art.always.adherent)) / 3
   art.prop.never.adherent.meth.crack.ecstasy <- art.prop.never.adherent + art.meth.crack.ecstasy.to.distribute
   art.prop.part.plus.adherent.meth.crack.ecstasy <- art.prop.part.plus.adherent + art.meth.crack.ecstasy.to.distribute
   art.prop.part.neg.adherent.meth.crack.ecstasy <- art.prop.part.neg.adherent + art.meth.crack.ecstasy.to.distribute
   art.prop.always.adherent.meth.crack.ecstasy <- 1 - (art.prop.never.adherent.meth.crack.ecstasy + art.prop.part.plus.adherent.meth.crack.ecstasy + art.prop.part.neg.adherent.meth.crack.ecstasy)


   #####################
   ## PrEP

   ### Default PrEP intervention Parameters ###

   default.prep.daily.stop.prob.lt <- 1/default.prep.mean.days.usage.lt
   default.prep.daily.stop.prob.gte <- 1/default.prep.mean.days.usage.gte
   default.prep.daily.stop.prob <- mean(c(default.prep.daily.stop.prob.lt, default.prep.daily.stop.prob.gte))
   prep.daily.stop.prob <- default.prep.daily.stop.prob.lt #needed only for time 0

   ### Substance use PrEP Parameters ###
   default.prep.daily.stop.prob.meth <- 1/default.prep.mean.days.usage.meth
   default.prep.daily.stop.prob.crack <- 1/default.prep.mean.days.usage.crack
   default.prep.daily.stop.prob.ecstasy <- 1/default.prep.mean.days.usage.ecstasy
   default.prep.daily.stop.prob.meth.crack <- 1/default.prep.mean.days.usage.meth.crack
   default.prep.daily.stop.prob.meth.ecstasy <- 1/default.prep.mean.days.usage.meth.ecstasy
   default.prep.daily.stop.prob.crack.ecstasy <- 1/default.prep.mean.days.usage.crack.ecstasy
   default.prep.daily.stop.prob.meth.crack.ecstasy <- 1/default.prep.mean.days.usage.meth.crack.ecstasy

   default.prep.bl.use.prop.meth <- meth.prep.prop.use
   default.prep.bl.use.prop.crack <- crack.prep.prop.use
   default.prep.bl.use.prop.ecstasy <- ecstasy.prep.prop.use
   default.prep.bl.use.prop.meth.crack <- meth.prep.prop.use * crack.prep.prop.use
   default.prep.bl.use.prop.meth.ecstasy <- meth.prep.prop.use * ecstasy.prep.prop.use
   default.prep.bl.use.prop.crack.ecstasy <- crack.prep.prop.use * ecstasy.prep.prop.use
   default.prep.bl.use.prop.meth.crack.ecstasy <- meth.prep.prop.use * crack.prep.prop.use * ecstasy.prep.prop.use


   ### Young Old Ratio PrEP intervention parameters ###

   yor.prep.daily.stop.prob <- 1 / yor.prep.mean.days.usage

   ### Serodiscordant intervention parameters ###

   serodiscordant.base.prep.daily.stop.prob.lt <- 1 / serodiscordant.base.prep.mean.days.usage.lt
   serodiscordant.base.prep.daily.stop.prob.gte <- 1 / serodiscordant.base.prep.mean.days.usage.gte

   serodiscordant.intrv.prep.daily.stop.prob.lt <- 1 / serodiscordant.intrv.prep.mean.days.usage.lt
   serodiscordant.intrv.prep.daily.stop.prob.gte <- 1 / serodiscordant.intrv.prep.mean.days.usage.gte

   ### Eigen intervention parameters ###

   eigen.base.prep.daily.stop.prob.lt <- 1 / eigen.base.prep.mean.days.usage.lt
   eigen.base.prep.daily.stop.prob.gte <- 1 / eigen.base.prep.mean.days.usage.gte

  eigen.intrv.prep.daily.stop.prob.lt <- 1 / eigen.intrv.prep.mean.days.usage.lt
  eigen.intrv.prep.daily.stop.prob.gte <- 1 / eigen.intrv.prep.mean.days.usage.gte

  ### Degree intervention parameters ###
  degree.base.prep.daily.stop.prob.lt <- 1 / degree.base.prep.mean.days.usage.lt
  degree.base.prep.daily.stop.prob.gte <- 1 / degree.base.prep.mean.days.usage.gte

  degree.intrv.prep.daily.stop.prob.lt <- 1 / degree.intrv.prep.mean.days.usage.lt
  degree.intrv.prep.daily.stop.prob.gte <- 1 / degree.intrv.prep.mean.days.usage.gte

  ### Random intervention parameters ###
  random.base.prep.daily.stop.prob.lt <- 1 / random.base.prep.mean.days.usage.lt
  random.base.prep.daily.stop.prob.gte <- 1 / random.base.prep.mean.days.usage.gte

  random.intrv.prep.daily.stop.prob.lt <- 1 / random.intrv.prep.mean.days.usage.lt
  random.intrv.prep.daily.stop.prob.gte <- 1 / random.intrv.prep.mean.days.usage.gte


   #####################
   ## Transmission Parameters
   ## relationship between viral load and chronic infectivity (hughes et al.)
   
   #####################
   ## Network (casual)
      ## nedges
         cas_tot_deg <- (0*cas_deg_seq[1])+(1*cas_deg_seq[2])+
                        (2*cas_deg_seq[3])
         cas_n_edges <- cas_tot_deg/2
      ## role
      pr_versatile_casual <- 1 - (pr_insertive_casual + pr_receptive_casual)
      ## serosorting

     ## dissolution parameters for ergm
     pg_cas <- (dur_cas - 1)/dur_cas
     theta.diss_cas <- log(pg_cas/(ps2-pg_cas)) #adjusted for death rate 

    #####################
     ## Testing and diagnosis
     daily.testing.prob <- 1/mean.time.until.next.test #FOR INITIALIZATION ONLY
  
      #####################

    #####################
    ## PrEP
    
#####################
    ## Socioeconomic status

    ### Incarceration

    incarceration.prob.meth <- incarceration.prob * (1 + meth.increase.incar.rate)
    incarceration.prob.crack <- incarceration.prob * (1 + crack.increase.incar.rate)
    incarceration.prob.ecstasy <- incarceration.prob * (1 + ecstasy.increase.incar.rate)
    incarceration.prob.meth.crack <- incarceration.prob * (1 + meth.increase.incar.rate + crack.increase.incar.rate)
    incarceration.prob.meth.ecstasy <- incarceration.prob * (1 + meth.increase.incar.rate + ecstasy.increase.incar.rate)
    incarceration.prob.crack.ecstasy <- incarceration.prob * (1 + crack.increase.incar.rate + ecstasy.increase.incar.rate)
    incarceration.prob.meth.crack.ecstasy <- incarceration.prob * (1 + meth.increase.incar.rate + crack.increase.incar.rate + ecstasy.increase.incar.rate)

    incarceration.with.cji.prob.meth <- incarceration.with.cji.prob * (1 + meth.increase.incar.rate)
    incarceration.with.cji.prob.crack <- incarceration.with.cji.prob * (1 + crack.increase.incar.rate)
    incarceration.with.cji.prob.ecstasy <- incarceration.with.cji.prob * (1 + ecstasy.increase.incar.rate)
    incarceration.with.cji.prob.meth.crack <- incarceration.with.cji.prob * (1 + meth.increase.incar.rate + crack.increase.incar.rate)
    incarceration.with.cji.prob.meth.ecstasy <- incarceration.with.cji.prob * (1 + meth.increase.incar.rate + ecstasy.increase.incar.rate)
    incarceration.with.cji.prob.crack.ecstasy <- incarceration.with.cji.prob * (1 + crack.increase.incar.rate + ecstasy.increase.incar.rate)
    incarceration.with.cji.prob.meth.crack.ecstasy <- incarceration.with.cji.prob * (1 + meth.increase.incar.rate + crack.increase.incar.rate + ecstasy.increase.incar.rate)

    jail.serving.time.mean.meth <- jail.serving.time.mean * (1 + jail.serving.time.increase.meth)
    jail.serving.time.mean.crack <- jail.serving.time.mean * (1 + jail.serving.time.increase.crack)
    jail.serving.time.mean.ecstasy <- jail.serving.time.mean * (1 + jail.serving.time.increase.ecstasy)
    jail.serving.time.mean.meth.crack <- jail.serving.time.mean * (1 + jail.serving.time.increase.meth + jail.serving.time.increase.crack)
    jail.serving.time.mean.meth.ecstasy <- jail.serving.time.mean * (1 + jail.serving.time.increase.meth + jail.serving.time.increase.ecstasy)
    jail.serving.time.mean.crack.ecstasy <- jail.serving.time.mean * (1 + jail.serving.time.increase.crack + jail.serving.time.increase.ecstasy)
    jail.serving.time.mean.meth.crack.ecstasy <- jail.serving.time.mean * (1 + jail.serving.time.increase.meth + jail.serving.time.increase.crack + jail.serving.time.increase.ecstasy)

