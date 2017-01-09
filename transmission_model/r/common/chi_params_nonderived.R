## non-derived chicago parameters
## initial values of parameters
   
   #####################
   ## NETWORK (steady)
     n <- 5000

   ## empirical edegree information
     ## 0 main partnerships = 56.6% of men
     ## 1 main partnership = 41.2% of men
     ## 2 main partnerships = 1.8 % of men
     ## >=3 main partnerships = 0.3 % of men
     mean_deg <- (0*0.566)+(1*0.412)+(2*0.018)+(3*0.003)

     deg_seq <- c(56.6, 41.2, 1.8)*n/100   
     duration <- (562+1260)/2
      
     ## role
     pr.insertive <- 20.9/100
     pr.receptive <- 24.2/100
       #28/35 partnerships match on infection status, 
       #almost equal between susc-susc and inf-inf 
   

   #####################
   ## TIMESTEP
   size.of.timestep <- 1 #currently set as 1 day
 
   #####################
   ## DEMOGRAPHIC
   min.age <- 16
   max.age <- 65
   daily.entry.rate <- 0.00008
   ## distribution of ages (between min and max)
   ## number of births (n.births: for now take it as 1% per year)
   ## age-specific mortality rates (ASMR), adjusted for HIV/AIDS-related deaths
   
   #####################
   ## BIOLOGICAL
   circum.rate <- 0.10
   init.hiv.prev <- 0.10
   init.hiv.prev.for.entries <- 0.01/100 # probability that an entering person will have HIV
   duration.of.infection <- 3300
   uninfected.cd4.level <- 518 #(might draw uniformly from a range)

   ## (to compute cd4 in infected but ART-naive)
   b1.ref <- 23.53
   b2.african <- -0.76
   b3.female <- 1.11
   b4.cd4.ref <- -1.49
   b5.african <- 0.34
   b6.age.15to29 <- 0
   b6.age.30to39 <- -0.1
   b6.age.40to49 <- -0.34
   b6.age.50ormore <- -0.63

  ## (time of infection)

  ## (cd4 decline)
  cd4.at.infection.male <- 518 #cells/mm3
  untreated.cd4.daily.decline <- 0.14 # (for men and women)

   ## healthy level of CD4: sample from some distribution, or should it be the same for all uninfected men?
   
  ## (viral load)
  undetectable.vl <- log(50, base=10)

   #####################
   ## ART
   baseline.art.coverage.rate <- 0.60
   art.init.time <- 365 # currently set to one year, make more complex later
   per.day.cd4.recovery <- 15/30 ## rate of 15 cells/month
   
   ## ART adherence
   partial.art_adher.window.length <- 3*30 #3 month window over which consistency in behavior is maintained
   prop.never.adherent <- 0.1 #denominator here is number who initiate ART. We can assign "adherence behavior" as an attribute.
   prop.always.adherent <- 0.1
   
   prob.art_adher.for.partial <- 0.5 #probability that a partially adherent individual will take their medication over the next `window.length`
   
   #####################
   ## Transmission Parameters
   ## Additional multiplier information: http://www.hiv.va.gov/provider/manual-primary-care/prevention-for-positives-table3.asp
   acute.mult <- 4.98
   late.mult <- 3.49
   preg.mult <- 2.5 ## check
   circum.mult <- 0.60 ## check
   preg.susc.mult <- 1.7
   acute.mult.holling <- 26
   late.mult.holling <- 7
   min.chronic.infectivity.unadj <- 0.000497/2.89 
   
   ## relationship between viral load and chronic infectivity (hughes et al.)
   
   #####################
   ## Casual (steady)
      ## duration
      dur_cas <- (214 + 456)/2
      ##degree 
        ## 0: 54.1%
        ## 1: 35.4%
        ## 2: 7.6% 
        ## 3: 2.9%
      cas_deg_seq <- c(54.1, 35.4, 7.6, 2.9)*n/100
      ## nedges

      ## role
      pr_insertive_casual <- 25.8/100
      pr_receptive_casual <- 19.4/100

      ## serosorting
   
    #####################
    ## Testing, diagnosis and linkage-to-care
    detection.window <- 22
    mean.time.until.next.test <- 365*2
    #lag.bet.diagnosis.and.art.init <- 30
    non.testers.prop <- 0.25  

    diag.init.2m <- 0.25
    diag.init.2to4m <- 0.1
    diag.init.4to6m <- 0.1
    diag.init.6to8m <- 0.1
    diag.init.8to10m <- 0.1
    diag.init.10to12m <- 0.25
    diag.never.init <- 0.1
    #####################
    ## PrEP
    prep.bl.use.prop <- 10/100
    prep.transm.red <- 96/100
    
#####################
    ## Socioeconomic status
    ##insurance.prop <- 
    ##incarceration.prop <- 

######################
    ## Sexual Behavior
    num.sex.acts.base <- 2.4 #condomless
    num.sex.acts.base.w.condom <- 5
    prop.steady.sex.acts <- 0.10 #of steady parrnteships on a given day, in how many does a sex act (w or w/o condom) occur?
                                 #same as freq.of.sex parameter in data table
    prop.casual.sex.acts <- 0.10 #same as above, but for casual
    prop.steady.sex.acts.w.condom <- 0.2
    prop.casual.sex.acts.w.condom <- 0.8
    inf.red.w.condom <- 0.80
    
