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

     deg_seq <- c(61.02, 34.65, 3.54)*n/100 
     mean_deg <- ((0*deg_seq[1])+(1*deg_seq[2])+(2*deg_seq[3]))/n

     # Note: This data doesn't exist for Houston YMAP
     # using estimates from NHBS data
     # Box Sync/BARS/Data-and-Summaries/Houston_Mean_Duration_of_Partnerships.xlsx
     duration <- 970 

     ## role
     pr_insertive_main <- 0.25
     pr_receptive_main <- 0.24 

     ## agemixing
     ## note: in chicago, the age diff was the mean of the diffs, not the sqrts
     absdiff.sqrtage.main <- 0.312
     absdiff.sqrtage.casual <- 0.344
   

   #####################
   ## TIMESTEP
   size.of.timestep <- 1 #currently set as 1 day
 
   #####################
   ## DEMOGRAPHIC
   min.age <- 18
   max.age <- 34
   # TODO: not updated
   # we'll update later during the runs
   daily.entry.rate <- 0.75 # this is to balance out the exits from the population; makes sense to change this after other things
   ## distribution of ages (between min and max)
   ## number of births (n.births: for now take it as 1% per year)
   ## age-specific mortality rates (ASMR), adjusted for HIV/AIDS-related deaths
   
   #####################
   ## BIOLOGICAL
   circum.rate <- 0.64 #TODO: not updated
   # TODO: what's the circumcision rate? Ask Nina/John
   
   # these are general background rates
   init.hiv.prev <- 0.10
   init.hiv.prev.for.entries <- 1/100 # probability that an entering person will have HIV

   acute.length <- 1:90 ## in daily time units
   chronic.length <- 91:3550
   late.length <- 3551:(3551+728)
   duration.of.infection <- max(late.length)

   ## (viral load)
   time.infection.to.peak.viremia <- 45
   time.infection.to.peak.viral.load <- time.infection.to.peak.viremia
   peak.viral.load <- 6.17
   time.infection.to.viral.set.point <- 90
   set.point.viral.load <- 4.2
   time.infection.to.late.stage <- 90+3550+1
   dur.inf <- duration.of.infection
   late.stage.viral.load <- 5.05 ## (max?)

   time.to.full.supp <- 4*30/size.of.timestep ## 4 months
   undetectable.vl <- log(50, base=10)

   uninfected.cd4.level <- 916 #updated value as per nina's suggestion from Mallory Witt (2013), CID.

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
  cd4.at.infection.male <- uninfected.cd4.level #cells/mm3
  untreated.cd4.daily.decline <- 0.14 # (for men and women)

   ## healthy level of CD4: sample from some distribution, or should it be the same for all uninfected men?
   
  ## (viral load)
  undetectable.vl <- log(50, base=10)

   #####################
   ## ART
   #baseline.art.coverage.rate <- 0.60 NOT NEEDED
   art.init.time <- 365 # currently set to one year, make more complex later NEEDED ONLY FOR TIME 0 INITIALIZATION
   per.day.cd4.recovery <- 15/30 ## rate of 15 cells/month
   
   ## ART adherence
   partial.art_adher.window.length <- 1*30 #1 month window over which consistency in behavior is maintained
   # Note: double check these numbers since they seem high
   # TODO: add in the new numbers from Jing, sanity check with John/Kayo
   art.prop.never.adherent <- 0.026 #denominator here is number who initiate ART. We can assign "adherence behavior" as an attribute.
   art.prop.part.neg.adherent <- 0.026 + 0.5*0.11
   art.prop.part.plus.adherent <- 0.24 + 0.5*0.11
   art.prop.always.adherent <- 1 - (art.prop.never.adherent+art.prop.part.plus.adherent+art.prop.part.neg.adherent)
   
 	art.always.adherent.probability <- 0.95
	art.never.adherent.probability <- 0.05
	art.partial.pos.adherent.probability <- 0.66
	art.partial.neg.adherent.probability <- 0.33
   
   #####################
   ## Transmission Parameters
   ## Additional multiplier information: http://www.hiv.va.gov/provider/manual-primary-care/prevention-for-positives-table3.asp
   acute.mult <- 4.98
   late.mult <- 3.49
   circum.mult <- 0.40 ## 40% is the multiplier. Circumcised insertive men are 0.60 times as infectious
   acute.mult.holling <- 26
   late.mult.holling <- 7
   min.chronic.infectivity.unadj <- (0.00898+0.003379)/2 #jenness et al.

   ## relationship between viral load and chronic infectivity (hughes et al.)
      ## each log increment in viral load corresponds to a 2.89-fold increase in infectivity
      inf.inc.log.vl <- 2.89

   ## insertive partner transmission rel. to receptive partner
      inf.part.insertive.mult <- 2

   #####################
   ## Casual (non-main)
      ## duration
      # Note: computed from NHBS data
      dur_cas <- 388
      ## degree 
      cas_deg_seq <- c(59, 25.68, 10.36)*n/100

      ## nedges

      ## role
      pr_insertive_casual <- 21.64/100
      pr_receptive_casual <- 32.09/100

      ## serosorting
   
    #####################
    ## Testing, diagnosis and linkage-to-care
    detection.window <- 22
    mean.time.until.next.test <- 365*1 #FOR INITIALIZATION ONLY
    #lag.bet.diagnosis.and.art.init <- 30
    non.testers.prop.lt <- 0.078 # Note: 17.9% not tested in the last two years but that's very high for never-testers. Does last two mean really never?
    non.testers.prop.gte <- 0.023

    # lag between diagnosis and ART init
    # format is probability, min range val - max range val
    # range is in days
    # Note: don't have any of this for houston
    art.init.lag.lt.1 <- "0.1692857143,0-7"  
    art.init.lag.lt.2 <- "0.314285714,7-30"  
    art.init.lag.lt.3 <- "0.191285714,30-90"  
    art.init.lag.lt.4 <- "0.067285714,90-180"  
    art.init.lag.lt.5 <- "0.123285714,180-365"  
    art.init.lag.lt.6 <- "0.089285714,365-730"
    art.init.lag.lt.7 <- "0.045285714,1825-1825"
    
    art.init.lag.gte.1 <- "0.167125,0-7"  
    art.init.lag.gte.2 <- "0.2621250,7-30"  
    art.init.lag.gte.3 <- "0.095125,30-90"  
    art.init.lag.gte.4 <- "0.143125,90-180"  
    art.init.lag.gte.5 <- "0.143125,180-365"  
    art.init.lag.gte.6 <- "0.071125,365-730"
    art.init.lag.gte.7 <- "0.047125,730-1825"
    art.init.lag.gte.8 <- "0.071125,1825-1825"
   
    #####################
    ## PrEP
 
# days
prep.decision.frequency <- 7

# Note: Not there for Houston
# TODO: follow up with Jing
prep.prop.never.adherent.lt <- 0.211
prep.prop.always.adherent.lt <- 0.619
prep.prop.part.plus.adherent.lt <- 0.10
prep.prop.part.neg.adherent.lt <- 0.07

prep.prop.never.adherent.gte <- 0.211
prep.prop.always.adherent.gte <- 0.619
prep.prop.part.plus.adherent.gte <- 0.10
prep.prop.part.neg.adherent.gte <- 0.07

prep.always.adherent.trans.reduction <- 0.95
prep.never.adherent.trans.reduction <- 0.0
prep.partial.pos.adherent.trans.reduction <- 0.81 
prep.partial.neg.adherent.trans.reduction <- 0.31
    
prep.bl.use.prop.lt <- 12.7/100
prep.bl.use.prop.gte <- 14.7/100
prep.bl.use.prop <- (prep.bl.use.prop.lt + prep.bl.use.prop.gte)/2 #only needed for time 0
prep.mean.days.usage.lt <- 180
prep.mean.days.usage.gte <- 180

#####################
    ## Socioeconomic status
    ##insurance.prop <- 
    ##incarceration.prop <- 

######################
    ## Sexual Behavior
    # num.sex.acts.base <- 2.4
    # Note: from Jing's word doc (on 2018-03-14)
    # Using a weighting of 1/3, 1/7, 1/14, 1/30
    prop.steady.sex.acts <- 0.26 # of steady partnerships on a given day, in how many does a sex act (w or w/o condom) occur?
                                 #same as freq.of.sex parameter in data table
    prop.casual.sex.acts <- 0.23 #same as above, but for casual
    inf.red.w.condom <- 0.80
    
# sd -- sero-discordant
# each partnership falls in one of these buckets with
# specified probability
  # Note: these numbers are there but they're high
    sd.steady.never.use.condoms <- 0
    sd.steady.rarely.use.condoms <- 0.02941176
    sd.steady.sometimes.use.condoms <- 0.17647059
    sd.steady.usually.use.condoms <- 0.20941176
    sd.steady.always.use.condoms <-0.58823529 

    sd.casual.never.use.condoms <- 0.1034483
    sd.casual.rarely.use.condoms <-0.0689655
    sd.casual.sometimes.use.condoms <- 0.1034483
    sd.casual.usually.use.condoms <- 0.137931
    sd.casual.always.use.condoms <- 0.5862069

	### probability that the partnership in the specified bucket
	### will use a condom.
	sd.steady.never.use.condoms.prob <- 0
	sd.steady.rarely.use.condoms.prob <- 0.25
	sd.steady.sometimes.use.condoms.prob <- 0.5
	sd.steady.usually.use.condoms.prob <- 0.75
	sd.steady.always.use.condoms.prob <- 1
	
	sd.casual.never.use.condoms.prob <- 0
	sd.casual.rarely.use.condoms.prob <- 0.25
	sd.casual.sometimes.use.condoms.prob <- 0.5
	sd.casual.usually.use.condoms.prob <- 0.75
	sd.casual.always.use.condoms.prob <- 1
	
	# sc sero-concordant
	sc.steady.never.use.condoms <- 0.399082569
	sc.steady.rarely.use.condoms <- 0.073394495
	sc.steady.sometimes.use.condoms <- 0.133027523
	sc.steady.usually.use.condoms <- 0.055045872
	sc.steady.always.use.condoms <- 0.339449541
	
	sc.casual.never.use.condoms <- 0.401330377
	sc.casual.rarely.use.condoms <- 0.019955654
	sc.casual.sometimes.use.condoms <- 0.053215078
	sc.casual.usually.use.condoms <- 0.033259424
	sc.casual.always.use.condoms <- 0.492239468
	
	### probability that the partnership in the specified bucket
	### will use a condom.
	sc.steady.never.use.condoms.prob <- 0
	sc.steady.rarely.use.condoms.prob <- 0.25
	sc.steady.sometimes.use.condoms.prob <- 0.5
	sc.steady.usually.use.condoms.prob <- 0.75
	sc.steady.always.use.condoms.prob <- 1
	
	sc.casual.never.use.condoms.prob <- 0
	sc.casual.rarely.use.condoms.prob <- 0.25
	sc.casual.sometimes.use.condoms.prob <- 0.5
	sc.casual.usually.use.condoms.prob <- 0.75
	sc.casual.always.use.condoms.prob <- 1
	
	
	### Age Specific Mortality ###
	### We don't strictly need all these age ranges, but 
	### they are here for completeness -- format is [min_max)
	# Note: 
	# - Used CDC Wonder data for Harris Country
	#   Because MSM would tend to congregate in cities
	# - Calculations and query criteria are in
	#   "Houston Mortality Calcs (from CDC Wonder).xslx"
	asm.15_20 <- 0.000966 / (365 * 1)
	asm.20_25 <- 0.001896 / (365 * 1)
	asm.25_30 <- 0.001976 / (365 * 1)
	asm.30_35 <- 0.001976 / (365 * 1)
	asm.35_40 <- 0.003175 / (365 * 1)
	asm.40_45 <- 0.003175 / (365 * 1)
	asm.45_50 <- 0.006803 / (365 * 1)
	asm.50_55 <- 0.006803 / (365 * 1)
	asm.55_60 <- 0.017383 / (365 * 1)
	asm.60_65 <- 0.017383 / (365 * 1)
	asm.65_70 <- 0.033181 / (365 * 1)
	
# external infections per person days
# we mult. this value by the number of uninfected persons
# to get a probability of an infection for that day
# numerator here is 100 person years which we convert into days
# critical information is that: incidence is 5-9 per 100 py
# 28% of transmissions are linked to older BMSM
# 50%-60% of those are likely incident from older to younger BMSM (i.e. 14% to 21%) 
# 14%-21% of infections are incident externally
# 5 * 14% = 0.7 (for min)
# 9*21% = 1.9 (for max)

# Note: Computed as:
# 0.28*.5*5 = (linked inf between young and old black MSM)*(proportion of inf from older to younger)*(lower bound on incidence from YBMSM in Houston)
# 0.28*.8*7
# TODO: ask John/Kayo about the last number
# May need to be updated
external.infections.per.person.day.min <- 0.7 / (100 * 365) 
external.infections.per.person.day.max <- 1.568 / (100 * 365)

# factor used to calculate age related external inf probability --
# [19, 20) year old will be "factor" time more likely to get externally
# infected than [18,19) yearolds. Similarly, (20,21) year olds will 
# be factor times more likely to get infected than the [19, 20) year olds, 
# and factor^2 times more likely to get infected than the [18, 19) so on.
# external.infections.age.factor = 1.2
external.infections.age.factor = 1 # changed to 1 to remove more external infections going to older age groups


# range of number of tests in last two years min-max, fraction of the population
# Note: from Jing's word doc
testing.prob.lt.1 = "1-2,0.50420168"
testing.prob.lt.2 = "3-4,0.22689076"
testing.prob.lt.3 = "5-6,0.15966387"
testing.prob.lt.4 = "7-8,0.06722689"
testing.prob.lt.5 = "9-10,0.00840336"
testing.prob.lt.6 = "11-12,0.00840336"
testing.prob.lt.7 = "13-16,0.02521008"
testing.prob.lt.8 = "17-20,0"
testing.prob.lt.9 = "21-30,0"

testing.prob.gte.1 = "1-2,0.35714286"
testing.prob.gte.2 = "3-4,0.35714286"
testing.prob.gte.3 = "5-6,0.11428571"
testing.prob.gte.4 = "7-8,0.1"
testing.prob.gte.5 = "9-10,0.01428571"
testing.prob.gte.6 = "11-12,0.02857143"
testing.prob.gte.7 = "13-16,0.02857143"
testing.prob.gte.8 = "17-20,0"
testing.prob.gte.9 = "21-30,0"


