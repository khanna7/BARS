## Add sdp parameters
## for Uganda, from https://github.com/khanna7/sero-discordant-couples/blob/master/Code/Data/params_uganda_d3.R

## 10 July 2014: Add 40% 

## 2 July 2014: Vary PMTCT coverage: add 20%

## 1 Jul 2014: Update mean CD4 count for initiation
## to 174 (currently it is 145)

## 26 June 2014: Additions to model ART eligibility at 500:-
## a. Coverage: does not change
## b. Mean CD4 at initiation: 145

## 24 June 2014: Vary PMTCT coverage: 50%, 60%, 70% 80%
## and compute how many would be virally suppressed in
## those cases

## 28 April 2014: Add "OptB.thres" argument --
## see "notes.txt" with this date

## 23 April 2014:
## a. for a below, "eligible.cd4" also needs to be
## changed to 500.

## 22 April 2014:
## a. UG start at CD4 of 500.
## b. hemodilution start for pregnant women at CD4 of 450.

## 14 Mar 2014: Add ZA fertility rates

## 21 Feb 2014:
## a. Set sc.art.postcess.ret.bl= sc.art.postcess.ret.bl.chung
## to avoid confusion later.
## b. Already had Add optb.vl.postcess.ret.bl=2*30/14 ##21Feb14
## c. Differentiate
## optA.sc.art.vl.perstep.dec and optA.sc.art.cd4.perstep.rec with
## optA.sc.art.vl.perstep.dec.idealized and optA.sc.art.cd4.perstep.rec.idealized
## (these idealized "dec" and "ret" params to not have to be used -- see
## email from Sarah dated 21 Feb 2014)

## Also change 23 weeks to 22 weeks in calculations above

## 19 Feb 2014: Add argument in function to 
## optb.vl.postcess.ret.bl=2*30/14

## 12 Feb 2014: Add 1 month return post-ART cessation

## 14 Jan 2013: Use Hollingsworth's multiplier

## 22 Dec 2013:
## Add parameter for cessation of Option B ART:
## optB.cess.time = round(12*30/14)

##18Dec13: Added
## idealized.cd4.at.art.initiation.men <- 350 
## idealized.cd4.at.art.initiation.women <- 350 
## for interventions

## 4 Dec 2013:
## a. Differentiate prevalence at recruitment for men and women -- 1.9% for men,
## 4.9% for women; earlier, I was using an average recruit prevalence of 3% for everyone

## b. Decided on 75% adherence for PMTCT under idealized intervention, consistent
## with adherence for realistics PMTCT intervention. For regular ART, adherence estimate
## under realistic and idealized interventions are also the same. The realistic
## coverage rate for PMTCT already accounts for adherence. 

## 1 Dec 2013: Also adjust "idealized.art.coverage.rate" for adherence using
## multiplicative model -- similar to adjustment for
## "baseline.art.coverage.rate"

## 25 Nov 2013:
## Adjust "baseline.art.coverage.rate" for adherence. We will do this by using
## a multiplicative model for coverage and adherence. See email dated 22 Nov 2013
## from Sarah

## 18 Nov 2013:
## a. Adjust age distribution here now that individuals enter at age 18.
## b. Make sure "preg.susc.mnult" in transmission function and "preg.mult"
## in 

## 16 Nov 2013: need to make change to burnin and intervention simulation,
## make sure "assign.infectivity" function has argument "preg.mult" and
## "transmission" function has argument "preg.susc.mult."

## 8 Nov 2013: Change name of data "dur.inf.by.age" to "given.dur.inf.by.age"
## to avoid confusion with the attribute which appears later

## 7 Nov 2013: Change min.age to 18.

## 28 Oct 2013: Add idealized intervention parameters here, so they
## don't have to be called separately. 

## 23 Oct 2013:
## a. add the pregnancy parameters: "full.term", "optA.vl.reduction" and
## "sc.art.postcess.rest.bl" here
## b. Add "preg.susc.mult" to differentiate multiplier for susceptible women

## 1 Oct 2013: add partnership duration restricted to 15 to 55.

## 26 Sep 2013: Increase number of births per 1000 women by
## 15%

## 3 Sep 2013: Revise fertility rates (per 1000) based on Sarah's
## email dated 3 Sep 2013.

## 2 Sep 2013: Add attributes for stratifying pregnancy by age
## and infection status.

## 26 Aug 2013: Max Survival for everyone -- add that NOW!!!!

## 22 Aug 2013:
## a. Add age-based expected life expectancy at time of infection
## b. Realistic CD4 initiation 131 (100 for South Africa)

## 20 Aug 2013: Change initial prevalence to 10%.
## 15 Aug 2013: I think min.chronic.infectivity is off by an order of magnitude.

### File to maintain comprehensive list of parameters

  ## Basic Population Set UP
  num.male <- 2500
  num.female <- 2500 
  N <- num.male+num.female

  ## BEHAVIOR
  ## Network Related Statistics
  ## Degree Distribution, Partnership Duration
  ## and Number of Partnerships
  male.deg.dist <- c(34.8, 52.3, 9.8, 3.1)/100 ## Phase 2 data
  female.deg.dist <- c(42.0, 55.7, 2.3, 0)/100 ## Phase 2 data

  size.of.timestep <- 14 ## each time step is 14 days
  duration <- 4303/size.of.timestep
  duration.15to55 <- 4139/size.of.timestep #1Oct2013 
  diagnostics <- T

  female.deg.counts <- female.deg.dist*num.female
  male.deg.counts <- male.deg.dist*num.male
  female.deg.tot <- (0*female.deg.counts[1] + 1*female.deg.counts[2] +
                     2*female.deg.counts[3] + 3*female.deg.counts[4])
  male.deg.tot <- (0*male.deg.counts[1] + 1*male.deg.counts[2] +
                   2*male.deg.counts[3] + 3*male.deg.counts[4])

  ## to match the degree totals for men and women, reduce the number of
  ## isolates in the women, and increase the number of women with 1 partner
  female.deg.dist.matched <- c(22.0, 75.7, 2.3, 0)/100
  female.deg.counts.matched <- female.deg.dist.matched*num.female
  female.deg.tot.matched <- (0*female.deg.counts.matched[1] +
                             1*female.deg.counts.matched[2] +
                             2*female.deg.counts.matched[3] +
                             3*female.deg.counts.matched[4])




  ## DEMOGRAPHIC ATTRIBUTES 
 
      max.survival <- 55 # 26 Aug 2013 -- max age (in years)
      ## Sex, age, circumcision status, 
      ## pregnancy status, 
      ## other pregnany related parameters


      ## Age (in accordance with proportions from census data)
      ##min.age <- 15
      min.age <- 18
      max.age <- 54

      ## age.distribution <- c(0.25468095,
      ##                       0.20238352,
      ##                       0.155598745,
      ##                       0.119586275,
      ##                       0.095660855,
      ##                       0.07367249,
      ##                       0.058677315,
      ##                       0.04014263)

     age.distribution <- c(0.12,
                           0.24,
                           0.18,
                           0.14,
                           0.11,
                           0.09,
                           0.07,
                           0.05)


      age.classes <- c(#seq(15, 19, by=1),
                       seq(18, 19, by=1), #7Nov13
                       seq(20, 24, by=1),
                       seq(25, 29, by=1),
                       seq(30, 34, by=1),
                       seq(35, 39, by=1),
                       seq(40, 44, by=1),
                       seq(45, 49, by=1),
                       seq(50, 54, by=1)
                       ) # create a vector of all ages of interest

      ## circumcision status
      circum.rate <- 96/(851+96) ## No 851; Yes 96; Refusal 1; NA 0,
                               ## see file "data_report.pdf"



  ## num.births.per1k.byage =
  ## round(c(83.2206172368,
  ##         154.3021435787,
  ##         167.5508974253,
  ##         122.3312442802,
  ##         76.2926740922,
  ##         29.3483793413,
  ##         9.424565988), 1) #2Sep13: CHECK!!!!!!!!!


      num.births.per1k.byage = round(c(
        174.967,
        345.9442,
        320.1459,
        266.055,	
        184.28,	
        79.91913,
        36.50134),
        1) # for 1 decimal place

      ## prop.stillbirth = 0.10 #2Sep13
         prop.stillbirth = 24.8/1000 #2Sep13
         inf.preg.red = 0.53 #2Sep13

      num.births.per1k.byage.15pcinc <- num.births.per1k.byage+
  (0.15*num.births.per1k.byage)

   ## 14Mar14: South Africa fertility rates for
   ## counterfactual analysis

      num.births.per1k.byage.za = round(c(
        c(80.6,
          139,
          141.8,
          105.6,
          67.4,
          27.1,
          8.8)),
        1) # for 1 decimal place

         prop.stillbirth.za = 20.4/1000 #2Sep13

      num.births.per1k.byage.15pcinc.za <- num.births.per1k.byage.za+
  (0.15*num.births.per1k.byage.za)

  
  ## BIOLOGICAL ATTRIBUTES 

      ## Infection Status 
      ## init.hiv.prev <- 0.06 # set to about 6% for Uganda
      ## init.hiv.prev <- 0.10 # set to about 6% for Uganda
      ## init.hiv.prev <- 0.15 # 22Aug2013
         init.hiv.prev <- 0.10 # 23 Aug 2013
         init.hiv.prev.6 <- 0.06 # 23 Aug 2013
         init.hiv.prev.15 <- 0.15
         init.hiv.prev.10 <- 0.10 # 3Sep13
         recruit.inf.prop.male <- 1.9/100 #4Dec13
         recruit.inf.prop.female <- 4.9/100 #4Dec13

      ## Time Since Infection 
      duration.of.infection <- 3300 ## in days, modify later

      ## infectivity for infected individuals
      # min.chronic.infectivity <- 0.00497/2.89
        #min.chronic.infectivity.unadj <- 0.00497/2.89
                                        # changed to include infection at log 2

     min.chronic.infectivity.unadj <- 0.000497/2.89 # 15 Aug 2013: was off by order. of
                                        # magnitude
  ## Time of Infection 
      acute.length <- 1:floor(121/size.of.timestep) ## in daily time units
      chronic.length <- ceiling(121/size.of.timestep):floor(1877/size.of.timestep)
      late.length <- ceiling(1877/size.of.timestep):floor(3300/size.of.timestep)

      ## CD4 Counts
             ## Set to 518 for men and 570 for women
             ## For positives, this will change as we step through time loop.
             ## Relevant parameters
      cd4.at.infection.male <- 518 #cells/mm3
      cd4.at.infection.female <- 570 #cells/mm3
      untreated.cd4.daily.decline <- 0.14 # (for men and women)
      untreated.cd4.perstep.decline <- untreated.cd4.daily.decline*size.of.timestep
      untreated.cd4.time.to.350.men <- 3.3*365/size.of.timestep # changed due to timestep
      untreated.cd4.time.to.350.men <- 4.2*365/size.of.timestep # changed due to timestep

      ## Viral Load Today
      ## List viral load parameters, adjusted for size of timestep
      time.infection.to.peak.viremia <-   floor(14/size.of.timestep)
      time.infection.to.peak.viral.load <- time.infection.to.peak.viremia
      peak.viral.load <- 6.17
      time.infection.to.viral.set.point <- floor(121/size.of.timestep)
      set.point.viral.load <- 4.2
      time.infection.to.late.stage <- floor(1877/size.of.timestep)
      dur.inf <- floor(3300/size.of.timestep)
      late.stage.viral.load <- 5.05 ## (max?)
      time.infection.to.peak.viral.load
      time.to.full.supp <- 4*30/size.of.timestep ## 4 months
      undetectable.vl <- log(50, base=10)

      ## dur.inf.by.age <- round(c(12.8*365/size.of.timestep,
      ##                           10.6*365/size.of.timestep,
      ##                           7.5*365/size.of.timestep,
      ##                           5.6*365/size.of.timestep))

      given.dur.inf.by.age <- round(c(12.8*365/size.of.timestep,
                                      10.6*365/size.of.timestep,
                                      7.5*365/size.of.timestep,
                                      5.6*365/size.of.timestep))

  ## ART
      #baseline.art.coverage.rate <- 0.40 # coverage
      baseline.art.coverage.rate <- 0.487 #25Nov13
      art.adherence.rate <- 0.885 #25Nov13
      baseline.art.coverage.rate <- baseline.art.coverage.rate*art.adherence.rate #25Nov13

      baseline.preg.art.coverage.rate <- baseline.preg.coverage.rate <- 0.43 #14Oct13
                                        # coverage # CHECK THIS

      idealized.art.coverage.rate <- 0.90 # 28Oct13
      idealized.art.coverage.rate <- idealized.art.coverage.rate*art.adherence.rate
                                        # 1 Dec 2013
      idealized.preg.coverage.rate <- 0.90 # 28Oct13
      preg.adherence.rate          <- 0.75 #4Dec13
      idealized.preg.coverage.rate <- idealized.preg.coverage.rate*preg.adherence.rate
                                        #4Dec13

      idealized.preg.coverage.rate.50 <- 0.50 #24Jun14
      idealized.preg.coverage.rate.60 <- 0.60 #24Jun14
      idealized.preg.coverage.rate.70 <- 0.70 #24Jun14
      idealized.preg.coverage.rate.80 <- 0.80 #24Jun14
      idealized.preg.coverage.rate.20 <- 0.20 #2Jul14
      idealized.preg.coverage.rate.40 <- 0.40 #2Jul14

      cd4.recovery.time <- 3*365/size.of.timestep ## CD4 recovery for 3 years
      per.day.cd4.recovery <- 15/30 ## rate of 15 cells/month
      eligible.cd4 <- 350

      eligible.cd4.500 <- 500 #23Apr14

      baseline.cd4.at.art.initiation.men <- 131 # 22Aug13: customized for UG
      baseline.cd4.at.art.initiation.women <- 131 # 22Aug13: customized for UG

      baseline.cd4.at.art.initiation.men.artelig500 <- 174 #1Jul14. Previously 145 (#26Jun14)
      baseline.cd4.at.art.initiation.women.artelig500 <- 174 #1Jul14. previously 145 (#26Jun14)

      idealized.cd4.at.art.initiation.men <- 350 
      idealized.cd4.at.art.initiation.women <- 350 
                                        #18Dec13:added for interventions

      cd4.at.art.initiation.men.500 <- 500 #22Apr2014
      cd4.at.art.initiation.women.500 <- 500 #22Apr2014
      cd4.at.art.initiation.women.450 <- 450 #22Apr2014


      bl.min.art.init.timestep.male <- (cd4.at.infection.male -
                                        baseline.cd4.at.art.initiation.men)/
                                        untreated.cd4.perstep.decline
      bl.min.art.init.timestep.female <- (cd4.at.infection.female -
                                          baseline.cd4.at.art.initiation.women)/
                                          untreated.cd4.perstep.decline


      idealized.cd4.at.art.initiation.men <- 350
      idealized.cd4.at.art.initiation.women <- 350


  ## Option A
      optA.sc.art.vl.perstep.dec <- 1.1/((40-22)*7)*size.of.timestep
                ## decline is 1.1 log over 17 weeks(from first visit to delivery)
                ## Per day decline, therefore, is 1.1/((40-23)*7)
                ## Per time step decline, therefore, is given by expr. above
      optA.sc.art.cd4.perstep.rec <- 50/((40-22)*7)*size.of.timestep
                ## recovery is 50 cells/mm3 over 17 weeks
                ## (from first visit to delivery)
                ## same logic as above applies
      optA.sc.art.vl.perstep.dec.idealized <- 1.1/((40-14)*7)*size.of.timestep #21Feb14
      optA.sc.art.cd4.perstep.rec.idealized <- 50/((40-14)*7)*size.of.timestep #21Feb14

      optA.thres=350
      optB.thres=350#28Apr14

  ## Option B
     optB.cess.time = round(12*30/14)

  ## Demographic Parameters

      ## Mortality
      ## 7Jul13: Adjusted to realistic values
      asmr.perperson.perday <- c(6.87671232876712E-006,
                                 1.31232876712329E-005,
                                 1.93424657534247E-005,
                                 2.66027397260274E-005,
                                 3.7013698630137E-005,
                                 4.59452054794521E-005,
                                 5.29315068493151E-005,
                                 5.68493150684932E-005
                                 )

      asmr.perperson.pertimestep <- asmr.perperson.perday*size.of.timestep
      asmr.male <- asmr.perperson.pertimestep
      asmr.female <- asmr.perperson.pertimestep

      ## Births
      phi <- 0.001*5 ## mean parameter for poisson process
      phi <- (phi/5)*3 #29Aug13, 2Sep13
      phi.std <- 0.001*1
      phi.std2 <- 0.001*2
      phi.zero <- 0 #22 Aug 2013
      phi.std5 <- 0.001*5
      phi.std4 <- 0.001*4
      phi.std45 <- 0.001*4.5
      phi.std35 <- 0.001*3.5

      ## Pregnancy
      full.term=40/14*7
      min.preg.interval=15*30/14
      optA.vl.reduction=1.1
      ## sc.art.postcess.ret.bl=6*30/14 ## return in 6 months = 180/14 timesteps
      baseline.f.ges.visit=23*7/14
      idealized.f.ges.visit=14*7/14
      sc.art.postcess.ret.bl.chung=1*30/14 ## return in 1 month = 30/14 timesteps
      sc.art.postcess.ret.bl <- sc.art.postcess.ret.bl.chung #21Feb14
      optB.vl.postcess.ret.bl=2*30/14 ##19Feb14

  ## Transmission Parameters
  ## Frequency of Sex
     num.sex.acts.per.timestep <- 2.4*size.of.timestep/7
     acute.mult <- 4.98
     late.mult <- 3.49
     preg.mult <- 2.5 ## check
     circum.mult <- 0.60 ## check
     preg.susc.mult <- 1.7
     acute.mult.holling <- 26
     late.mult.holling <- 7

  ## New parameters for SDP
     hbhtc.testing.coverage=0.80
     known.sdp.art.coverage=58.4/100
     known.sdp.art.at.cd4=350

     not.known.sdp.art.coverage=58.4/100
     not.known.sdp.art.at.cd4=350

     decline.ui=0.63
     
     known.sdp.art.coverage.high=0.90
     known.sdp.art.at.cd4.high=1e3
