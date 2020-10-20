# initial main network estimation for LA

   ## top matter
   rm(list=ls())

   library(ergm)
   library(network)
   library(networkDynamic)
   library(tergm)

   source("../common/la_params_nonderived.R")
   source("../common/la_params_derived.R")
   source("common-functions.R")
   
   #####################
   ## MODEL SETUP
   formation <- ~edges+degree(0:2)#+absdiff("sqrt.age")

   dissolution <- ~offset(edges)

   target.stats <- c(main_nedges, main_deg_seq[1:3])#, absdiff.sqrtage.main*nedges)  
   constraints <- ~.

   formation.n0 <- update.formula(formation, n0~.)

   #####################
   ## CREATE EMPTY NETWORK TO START
   n0 <- network.initialize(n, directed=FALSE, bipartite=FALSE)

   #####################
   ## OTHER INITIALIZATION PARAMETERS

   ## sex
   n0 %v% "sex" <- "male"

   ## age
   age <- runif(n, min.age, max.age)
   n0 %v% "age" <- age
   n0 %v% "sqrt.age" <- sqrt(age)

   ## circumcision status
   circum.status <- rbinom(n, 1, circum.rate)
   n0 %v% "circum.status" <- circum.status

   ## infection status
   init.inf.status <- rbinom(n, 1, init.hiv.prev)
   n0 %v% "inf.status" <- init.inf.status
   init.infected <- which(init.inf.status == 1)

   ## sexual role
   role_main <- sample(0:2, n, c(pr_versatile_main, 
                            pr_insertive_main, 
                            pr_receptive_main), 
                  replace=TRUE)
   
   table(role_main, exclude=NULL)
   n0 %v% "role_main" <- role_main #0=versatile, 1=insertive, 2=receptive
   
   ## time since infection
   ## (draw from uniform distribution)
   time.since.infection <- rep(NA, n)
 
   time.since.infection[init.infected] <-
       sample(c(
           0:floor(duration.of.infection/
                       size.of.timestep)),
              size=length(init.infected),
              replace=TRUE)
                                                   
   set.vertex.attribute(n0, "time.since.infection",
                        time.since.infection)

   ## time of infection
   ## (follows from time.since.infection above)
   time.of.infection <- 0 - (n0%v%"time.since.infection")
   set.vertex.attribute(n0, "time.of.infection", time.of.infection)

   ## age at infection
   n0%v%"age.at.infection" <- rep(NA, n)
   age.at.infection <- age-(time.since.infection*size.of.timestep/365)
   age.at.infection[which(age.at.infection < min.age)] <- min.age #correct?

   set.vertex.attribute(n0, "age.at.infection", age[init.infected],
                        v=init.infected)
   summary(n0%v%"age.at.infection")


   ## dur.inf.by.age (might or might not be needed)

   ## cd4.count.today
   ## (for HIV-uninfected)
   cd4.count.today <- rep(uninfected.cd4.level, n)

   ## (for HIV-infected but ART naive)
      for (i in 1:n){ # for male
          if (i %in% init.infected){ # we only update CD4 of infecteds
              if (age[i] %/% 10 < 3){ ## need quotient operator
                  cd4.count.today[i] <- (b1.ref+b2.african+
                          (time.since.infection[i]/365*size.of.timestep)*
                          (b4.cd4.ref+b5.african+b6.age.15to29))^2
          } else if (age[i] %/% 10 == 3){
            cd4.count.today[i] <- (b1.ref+b2.african+
                          (time.since.infection[i]/365*size.of.timestep)*
                          (b4.cd4.ref+b5.african+b6.age.30to39))^2
          } else if (age[i] %/% 10 == 4){
            cd4.count.today[i] <- (b1.ref+b2.african+
                          (time.since.infection[i]/365*size.of.timestep)*
                          (b4.cd4.ref+b5.african+b6.age.40to49))^2
          } else if (age[i] %/% 10 >= 5){
            cd4.count.today[i] <- (b1.ref+b2.african+
                          (time.since.infection[i]/365*size.of.timestep)*
                          (b4.cd4.ref+b5.african+b6.age.50ormore))^2
          } 
        }
      }

    n0 %v% "cd4.count.today" <- cd4.count.today

   ## non-testers
   age.threshold <- 26

   lt.msm <- which(age < age.threshold) #lt = "less than"
   gte.msm <- which(age >= age.threshold) #gte = "greater than equal to"
   
   non.tester.lt <- rbinom(length(lt.msm), 1, non.testers.prop.lt)
   non.tester.gte <- rbinom(length(gte.msm), 1, non.testers.prop.gte)
   
   set.vertex.attribute(n0, "non.testers", non.tester.lt, v=lt.msm)
   set.vertex.attribute(n0, "non.testers", non.tester.gte, v=gte.msm)
   
   age.lt26 <- age #test if above construction worked
   age.lt26[lt.msm] <- 1
   age.lt26[gte.msm] <- 0
   
   xtabs(~age.lt26 + n0%v%"non.testers") #7.8% (2.3%) of <26y (>=26y) have never testerd
   
   ## ART coverage 
   art.covered <- 1-(n0%v%"non.testers")
   set.vertex.attribute(n0, "art.covered", art.covered)

   ## ART status
   art.status <- rep(NA, n) #everyone
   art.status[init.infected] <- 0

   art.uptaker <- init.infected[which(art.covered == 1)]
   art.inf.long.enough <- which(time.since.infection > art.init.time)
   art.init.elig <- intersect(art.inf.long.enough,
                              art.uptaker)
      # (art.init.time is a constant describing delay between
      #  infection and ART update)
   art.status[art.init.elig] <- 1

   n0 %v% "art.status" <- art.status

   ## time since ART initiation
   on.art <- which(art.status == 1)
   time.since.art.initiation <- rep(NA, n)
   time.since.art.initiation[on.art] <- 0
              #everyone who has ART just went on it
   n0 %v% "time.since.art.initiation" <- time.since.art.initiation

   ## time of ART initiation
   time.of.art.initiation <- 0-time.since.art.initiation
   n0 %v% "time.of.art.initiation" <- time.of.art.initiation

   ## cd4.count.today (for those on ART
       ## (we have list of those on ART
       ## and how long they have been on ART
       ## multiply by CD4 recovery in every time step, and
       ## we get their updated CD4 count.)

    for (i in 1:length(on.art)){
        cd4.count.today[on.art[i]] <- cd4.count.today[on.art[i]]*
                                       cd4.rec.per.timestep*
                                         time.since.art.initiation[on.art[i]]
    }


    ## viral.load.today
       ## (for ART-naive)
    viral.load.today <- rep(0, n)

    for (i in 1:n){

        if(i %in% init.infected){
            
            if ( (!is.na(time.since.infection[i])) && 
                (time.since.infection[i] <= time.infection.to.peak.viral.load)
                ){

                viral.load.today[i] <- mean(c(0, peak.viral.load))

            }  else if ( (time.since.infection[i] %in%
                          (time.infection.to.peak.viral.load+1):
                              (time.infection.to.viral.set.point))
                        )  {
                
                viral.load.today[i] <- peak.viral.load -
                    ((peak.viral.load - set.point.viral.load) *
                         (time.since.infection[i] -
                              time.infection.to.peak.viral.load)/
                                  (time.infection.to.viral.set.point -
                                       time.infection.to.peak.viral.load))*
                                           1/2 # so we arrive at the midpoint
                
            } else if ( (time.since.infection[i] %in%
                         (time.infection.to.viral.set.point+1):
                             (time.infection.to.late.stage))
                       ) {
                viral.load.today[i] <- set.point.viral.load
                
            }  else if ( (!is.na(time.since.infection[i])) &&
                        (time.since.infection[i] > time.infection.to.late.stage)
                        ){
                
                viral.load.today[i] <- set.point.viral.load +
                    ((late.stage.viral.load - set.point.viral.load)*
                         (time.since.infection[i] - time.infection.to.late.stage)/
                             ( (dur.inf-1) - time.infection.to.late.stage ))*1/2
                                        # so we arrive at midpoint

            }
        }
    }

       ## (for ART-initiated)
           ## For those on ART we will first have to assign
           ## "vl.art.traj.slope"

        vl.art.traj.slope <- rep(NA, n)

        if (length(on.art) > 0){ # only update art-naive viral loads
                                 # for those on ART
            for (i in 1:length(on.art)){

                vl.art.traj.slope[on.art[i]] <- 
                    abs((undetectable.vl - viral.load.today[on.art[i]])/time.to.full.supp)

                viral.load.today[on.art[i]] <- 
                    viral.load.today[on.art[i]] -
                        vl.art.traj.slope[on.art[i]]*time.since.art.initiation[on.art[i]]

            }
        }

        n0%v%"viral.load.today" <- viral.load.today
        n0%v%"vl.art.traj.slope" <- vl.art.traj.slope 

        ## stage of infection
       stage.of.infection <- classify.stage.numeric.rewrite(
                               time.since.infection)
       n0%v%"stage.of.infection" <- stage.of.infection

       ## infectivity.today (not needed for initialization)

       ## cd4.at.art.initiation
       n0%v%"cd4.at.art.initiation" <- cd4.count.today -
                                         (time.since.art.initiation*
                                              per.day.cd4.recovery*
                                                  size.of.timestep)

       ## vl.at.art.initiation
       n0%v%"vl.at.art.initiation"  <- viral.load.today+
                                          abs((undetectable.vl -
                                           viral.load.today[on.art[i]])/
                                             time.to.full.supp)*
                                               time.since.art.initiation

       ## cd4.at.art.cessation

       ## vl.at.art.cessation

   ## testing and diagnosis

      ## attribute: diagnosed
      n0%v%"diagnosed" <- 0
      set.vertex.attribute(n0, "diagnosed", 1, on.art)

      ## attribute: tested today
      n0%v%"tested.today" <- 0
      set.vertex.attribute(n0, "tested.today", 1, on.art)

      ## attribute: number.of.tests
      n0%v%"number.of.tests" <- 0
      set.vertex.attribute(n0, "number.of.tests", 1, on.art)

      ## attribute: time until next test
      not.diagnosed <- which(n0 %v% "diagnosed" == 0)
      time.until.next.test <- rep(-1, n)
      time.until.next.test[not.diagnosed] <- rgeom(length(not.diagnosed), p=daily.testing.prob)
      time.until.next.test <- time.until.next.test+1 #because random draws from geom dist will yield sone zeros
      set.vertex.attribute(n0, "time.until.next.test", time.until.next.test)
      
      ## attribute: lag between diagnosis and ART initiation
      #n0 %v% "lag.bet.diagnosis.and.art.init" <- lag.bet.diagnosis.and.art.init

      ## attribute: non-test
         ## defined above"

   ## PrEP
      not.infected <- which(n0 %v% "inf.status" == 0)
      prep.status <- rbinom(length(not.infected), 1, prep.bl.use.prop)
      set.vertex.attribute(n0, "prep.status", prep.status, v=not.infected)

      table(n0%v%"prep.status")
      table(n0%v%"prep.status")/sum(table(n0%v%"prep.status"))

      ## assign time for stoppage
      on.prep <- which(n0%v%"prep.status" == 1)
      set.vertex.attribute(n0, "time.of.prep.cessation",
                           rgeom(length(on.prep), prep.daily.stop.prob),
                           v=on.prep)

      set.vertex.attribute(n0, "time.of.prep.initiation",
                           0,
                           v=on.prep
                           )

   #####################
   ## FIT MODEL
   fit <- ergm(formation.n0, 
               target.stats=target.stats, 
               constraints=constraints,
               eval.loglik=FALSE,
               verbose=TRUE,
               control=control.ergm(MCMLE.maxit=500)
               )

   theta.form <- fit$coef 
   theta.form[1] <- theta.form[1] - theta.diss_main

   #####################
   ## SIMULATE (for testing)
    hetdeg.diag.sim <- simulate(n0,
                                formation=formation.n0,
                                dissolution=dissolution,
                                coef.form=theta.form, 
                                coef.diss=theta.diss_main,
                                time.slices=2e4,
                                #time.slices=1e2,
                                constraints=constraints,
                                monitor=~edges+degree(0:5)
                                )
# 
#    #####################
#    ## TEST
     net.f <- network.collapse(hetdeg.diag.sim, at=20000)
     network.size(net.f)
     network.edgecount(net.f)
     degreedist(net.f)/network.size(net.f)
   
     seq.of.nets <- lapply(1:length(hetdeg.diag.sim),
                           function(x)
                           network.collapse(hetdeg.diag.sim, at=x)
     )
   
# 
#  get.edge.activity(net.f)
   ## To compute partnership metrics from network data, see 
   ## https://github.com/khanna7/sero-discordant-couples/blob/master/Code/Engine/function_sdp_identify_d1.R
   
   #####################
   ## SAVE BINARY
   save.image(file=paste0("LA_initialized-model_n",n,".RData"))
   
