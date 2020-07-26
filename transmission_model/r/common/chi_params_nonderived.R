## non-derived chicago parameters
## initial values of parameters

   #####################
   ## NETWORK (steady)
     n <- 10000

   ## empirical edegree information
     ## 0 main partnerships = 56.6% of men
     ## 1 main partnership = 41.2% of men
     ## 2 main partnerships = 1.8 % of men
     ## >=3 main partnerships = 0.3 % of men
     #deg_seq <- c(63.71, 34.03, 1.89)*n/100

     deg_seq <- c(56.60, 41.20, 1.80)*n/100

     mean_deg <- ((0*deg_seq[1])+(1*deg_seq[2])+(2*deg_seq[3]))/n

     duration <- 512
     #duration <- 160

     ## role
     pr_insertive_main <- 15/100
     pr_receptive_main <- 20/100
       #28/35 partnerships match on infection status,
       #almost equal between susc-susc and inf-inf

     ## agemixing
     absdiff.sqrtage.main <- 0.38 #not used
     absdiff.sqrtage.casual <- 0.43 #not used
     absdiff.main <- 2.9
     absdiff.casual <- 3.1


   #####################
   ## TIMESTEP
   size.of.timestep <- 1 #currently set as 1 day

   #####################
   ## DEMOGRAPHIC
   min.age <- 18
   max.age <- 34
   #daily.entry.rate <- 1.0 #rate for n=5000
   daily.entry.rate <- 2.0 #rate for n=10000

   ## distribution of ages (between min and max)
   ## number of births (n.births: for now take it as 1% per year)
   ## age-specific mortality rates (ASMR), adjusted for HIV/AIDS-related deaths

   #####################
   ## BIOLOGICAL
   circum.rate <- 0.64 #in accordance with CDC circumcision data sent by Kiana (2016)
   init.hiv.prev <- 0.10
   init.hiv.prev.for.entries <- 1/100 # probability that an entering person will have HIV
   # value prior to 10/19/2017: 0.001/100

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

   #time.to.full.supp <- 4*30/size.of.timestep ## old: 4 months
   time.to.full.supp <- 1*30/size.of.timestep ## from DHHS (refer to John)
   undetectable.vl <- log(200, base=10) #updated to 200 from 50 on 17may2018

   #uninfected.cd4.level <- 518 #(might draw uniformly from a range)
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
  cd4.at.infection.male <- uninfected.cd4.level  #needs to be the same as `uninfected.cd4.level`
  untreated.cd4.daily.decline <- 0.14 # (for men and women)

   ## healthy level of CD4: sample from some distribution, or should it be the same for all uninfected men?

  ## (viral load)
  ## undetectable.vl <- log(50, base=10) ##see above needs to set to 200

   #####################
   ## ART
   #baseline.art.coverage.rate <- 0.60 NOT NEEDED
   art.init.time <- 365 # currently set to one year, make more complex later NEEDED ONLY FOR TIME 0 INITIALIZATION
   per.day.cd4.recovery <- 15/30 ## rate of 15 cells/month

   ## ART adherence
   partial.art_adher.window.length <- 1*30 #1 month window over which consistency in behavior is maintained

   art.prop.always.adherent <- 0.32
   art.prop.never.adherent <- 0.1
   art.prop.part.plus.adherent <- 0.28
   art.prop.part.neg.adherent <- 0.30

   art.always.adherent.probability <- 0.95
   art.never.adherent.probability <- 0.05
   art.partial.pos.adherent.probability <- 0.66
   art.partial.neg.adherent.probability <- 0.33

   #####################
   ## Transmission Parameters
   ## Additional multiplier information: http://www.hiv.va.gov/provider/manual-primary-care/prevention-for-positives-table3.asp
   #acute.mult <- 4.98
   acute.mult <- 5
   #late.mult <- 3.49
   late.mult <- 1
   #circum.mult <- 0.40 ## 40% is the multiplier. Circumcised insertive men are 0.60 times as infectious
    circum.mult <- 1

   acute.mult.holling <- 26
   late.mult.holling <- 7
   #min.chronic.infectivity.unadj <- 0.000497/2.89
   #min.chronic.infectivity.unadj <- (0.00898+0.003379)/2 #jenness et al.
   min.chronic.infectivity.unadj <- 0.000922913

   ## relationship between viral load and chronic infectivity (hughes et al.)
      ## each log increment in viral load corresponds to a 2.89-fold increase in infectivity
      inf.inc.log.vl <- 2.89

   ## insertive partner transmission rel. to receptive partner
      inf.part.insertive.mult <- 2

   #####################
   ## Casual (non-main)
      ## duration
      dur_cas <- 160
      ##degree
        ## 0: 54.1%
        ## 1: 35.4%
        ## 2: 7.6%
        ## 3: 2.9%
      cas_deg_seq <- c(58.9, 32.2, 6.8)*n/100

      ## nedges

      ## role
      pr_insertive_casual <- 25.8/100
      pr_receptive_casual <- 19.4/100

      ## serosorting

    #####################
    ## Testing, diagnosis and linkage-to-care
    detection.window <- 22
    mean.time.until.next.test <- 365*1 #FOR INITIALIZATION ONLY
    #lag.bet.diagnosis.and.art.init <- 30
    non.testers.prop.lt <- 0.078
    non.testers.prop.gte <- 0.023

     # lag between diagnosis and ART init
    # format is probability, min range val - max range val
    # range is in days
    art.init.lag.lt.1 <- "0.1692857143|0-7"
    art.init.lag.lt.2 <- "0.314285714|7-30"
    art.init.lag.lt.3 <- "0.191285714|30-90"
    art.init.lag.lt.4 <- "0.067285714|90-180"
    art.init.lag.lt.5 <- "0.123285714|180-365"
    art.init.lag.lt.6 <- "0.089285714|365-730"
    art.init.lag.lt.7 <- "0.045285714|1825-1825"

    art.init.lag.gte.1 <- "0.167125|0-7"
    art.init.lag.gte.2 <- "0.2621250|7-30"
    art.init.lag.gte.3 <- "0.095125|30-90"
    art.init.lag.gte.4 <- "0.143125|90-180"
    art.init.lag.gte.5 <- "0.143125|180-365"
    art.init.lag.gte.6 <- "0.071125|365-730"
    art.init.lag.gte.7 <- "0.047125|730-1825"
    art.init.lag.gte.8 <- "0.071125|1825-1825"

#####################
## PrEP

## PrEP Uptake Scheme ##

# one of default, young_old_ratio, serodiscordant, eigen, or degree
prep.uptake <- 'default'
prep.intervention.at <- 0
prep.intervention <- 'random'

## Default PrEP  parameters ###

default.prep.bl.use.prop.lt <- 12.7/100
default.prep.bl.use.prop.gte <- 14.7/100

prep.bl.use.prop <- (default.prep.bl.use.prop.lt + default.prep.bl.use.prop.gte)/2 #only needed for time 0

default.prep.mean.days.usage.lt <- 365 #updated 23may2018
default.prep.mean.days.usage.gte <- 365

default.prep.yearly.increment.lt <- 0
default.prep.yearly.increment.gte <- 0
default.prep.years.to.increment <- 0
#default.prep.years.to.increment <- 14

default.prep.unbalanced.starting.prob.lt <- 1/365 #these params are in the nonderived file so we can decouple them from the 
default.prep.unbalanced.starting.prob.gte <- 1/365 #stopping probabilities for the retention intervention
default.prep.unbalanced.starting.prob.meth <- 1/365 #these params are in the nonderived file so we can decouple them from the 
default.prep.unbalanced.starting.prob.crack <- 1/365 #stopping probabilities for the retention intervention
default.prep.unbalanced.starting.prob.ecstasy <- 1/365 #these params are in the nonderived file so we can decouple them from the 
default.prep.unbalanced.starting.prob.meth.crack <- 1/365 #stopping probabilities for the retention intervention
default.prep.unbalanced.starting.prob.meth.ecstasy <- 1/365 #these params are in the nonderived file so we can decouple them from the 
default.prep.unbalanced.starting.prob.crack.ecstasy <- 1/365 #stopping probabilities for the retention intervention
default.prep.unbalanced.starting.prob.meth.crack.ecstasy <- 1/365 #these params are in the nonderived file so we can decouple them from the 

# "balanced" - use prep uptake algorithm that "balances" uptake and cessation
# "unbalanced" - use prep uptake algorithm that does not "balance" uptake and cessation
default.prep.balanced.unbalanced <- 'balanced'

## End Default Parameters ##

## Random Selection Intervention 
random.base.prep.bl.use.prop.lt <- 12.7/100
random.base.prep.bl.use.prop.gte <- 14.7/100

random.base.prep.mean.days.usage.lt <- 365
random.base.prep.mean.days.usage.gte <- 365

random.intrv.prep.mean.days.usage.lt <- 365
random.intrv.prep.mean.days.usage.gte <- 365

random.intrv.prep.yearly.increment.lt <- 0
random.intrv.prep.yearly.increment.gte <- 0

random.intrv.prep.years.to.increment <- 0

## End random selection params

## Young Old Ratio PrEP Uptake Parameters ##

yor.prep.bl.use.prop <- 13.7/100
yor.prep.mean.days.usage <- 365

# probability of picking a young (y) vs old (o) person by a factor of alpha
yor.prep.alpha <- 0.3

yor.prep.yearly.increment <- 0.2
yor.prep.years.to.increment <- 5

# prep.addtional.* specifies how many more lt, gte persons to put on prep
# the value is the fractional increment over the base rate. For example,
# an lt value of 0.5 will add an additional amount of person on prep equal to
# 0.5 the amount the base added.
yor.prep.additional.lt <- 0.5
yor.prep.additional.gte <- 0.4

## End Young Old Ratio Parameters ##


## Serodiscordant PrEP Uptake Parameters ###

serodiscordant.base.prep.bl.use.prop.lt <- 12.7/100
serodiscordant.base.prep.bl.use.prop.gte <- 14.7/100
serodiscordant.base.prep.mean.days.usage.lt <- 365 #updated 23may2018
serodiscordant.base.prep.mean.days.usage.gte <- 365

serodiscordant.intrv.prep.mean.days.usage.lt <- 365
serodiscordant.intrv.prep.mean.days.usage.gte <- 365

# one of main, casual, or all
serodiscordant.intrv.prep.network.type <- 'main'

serodiscordant.intrv.prep.yearly.increment.lt <- 0.04
serodiscordant.intrv.prep.yearly.increment.gte <- 0.04

serodiscordant.intrv.prep.years.to.increment <- 5

## End Serodiscordant Parameters ##

## Eigen PrEP Uptake Parameters ###

eigen.base.prep.bl.use.prop.lt <- 12.7/100
eigen.base.prep.bl.use.prop.gte <- 14.7/100

eigen.base.prep.mean.days.usage.lt <- 365
eigen.base.prep.mean.days.usage.gte <- 365

eigen.intrv.prep.mean.days.usage.lt <- 365
eigen.intrv.prep.mean.days.usage.gte <- 365

eigen.intrv.prep.yearly.increment.lt <- 0.04
eigen.intrv.prep.yearly.increment.gte <- 0.04

eigen.intrv.prep.years.to.increment <- 5
# apply the intervention to the top N fraction of
# persons ranked by eigen centrality
eigen.intrv.prep.topn <- 0.10
## End Eigen Parameters ##

## Degree PrEP Uptake Parameters ###

degree.base.prep.bl.use.prop.lt <- 12.7/100
degree.base.prep.bl.use.prop.gte <- 14.7/100

degree.base.prep.mean.days.usage.lt <- 365
degree.base.prep.mean.days.usage.gte <- 365

degree.intrv.prep.mean.days.usage.lt <- 365
degree.intrv.prep.mean.days.usage.gte <- 365

degree.intrv.prep.yearly.increment.lt <- 0.04
degree.intrv.prep.yearly.increment.gte <- 0.04

degree.intrv.prep.years.to.increment <- 5
# apply the intervention to the top N fraction of
# persons ranked by degree
degree.intrv.prep.topn <- 0.10

## End Degree Parameters ##

# days
prep.decision.frequency <- 7

prep.prop.never.adherent.lt <- 0.211
prep.prop.always.adherent.lt <- 0.619
prep.prop.part.plus.adherent.lt <- 0.10
prep.prop.part.neg.adherent.lt <- 0.07

prep.prop.never.adherent.gte <- 0.211
prep.prop.always.adherent.gte <- 0.619
prep.prop.part.plus.adherent.gte <- 0.10
prep.prop.part.neg.adherent.gte <- 0.07

prep.prop.never.adherent.meth <- 0.211
prep.prop.always.adherent.meth <- 0.619
prep.prop.part.plus.adherent.meth <- 0.10
prep.prop.part.neg.adherent.meth <- 0.07

prep.prop.never.adherent.crack <- 0.211
prep.prop.always.adherent.crack <- 0.619
prep.prop.part.plus.adherent.crack <- 0.10
prep.prop.part.neg.adherent.crack <- 0.07

prep.prop.never.adherent.ecstasy <- 0.211
prep.prop.always.adherent.ecstasy <- 0.619
prep.prop.part.plus.adherent.ecstasy <- 0.10
prep.prop.part.neg.adherent.ecstasy <- 0.07

prep.prop.never.adherent.meth.crack <- 0.211
prep.prop.always.adherent.meth.crack <- 0.619
prep.prop.part.plus.adherent.meth.crack <- 0.10
prep.prop.part.neg.adherent.meth.crack <- 0.07

prep.prop.never.adherent.meth.ecstasy <- 0.211
prep.prop.always.adherent.meth.ecstasy <- 0.619
prep.prop.part.plus.adherent.meth.ecstasy <- 0.10
prep.prop.part.neg.adherent.meth.ecstasy <- 0.07

prep.prop.never.adherent.crack.ecstasy <- 0.211
prep.prop.always.adherent.crack.ecstasy <- 0.619
prep.prop.part.plus.adherent.crack.ecstasy <- 0.10
prep.prop.part.neg.adherent.crack.ecstasy <- 0.07

prep.prop.never.adherent.meth.crack.ecstasy <- 0.211
prep.prop.always.adherent.meth.crack.ecstasy <- 0.619
prep.prop.part.plus.adherent.meth.crack.ecstasy <- 0.10
prep.prop.part.neg.adherent.meth.crack.ecstasy <- 0.07

prep.always.adherent.trans.reduction <- 0.95
prep.never.adherent.trans.reduction <- 0.0
prep.partial.pos.adherent.trans.reduction <- 0.81
prep.partial.neg.adherent.trans.reduction <- 0.31


#####################
    ## Socioeconomic status
    ##insurance.prop <-
    ##incarceration.prop <-

######################
    ## Sexual Behavior
    #num.sex.acts.base <- 2.4
    #prop.steady.sex.acts <- 0.093 #of steady parrnteships on a given day, in how many does a sex act (w or w/o condom) occur?
                                 #same as freq.of.sex parameter in data table
    prop.steady.sex.acts <- 0.1893571
    prop.casual.sex.acts <- 0.053 #same as above, but for casual
    inf.red.w.condom <- 0.80

# sd -- sero-discordant
# each partnership falls in one of these buckets with
# specified probability
    sd.steady.never.use.condoms <- 0.261627907
    sd.steady.rarely.use.condoms <- 0.046511628
    sd.steady.sometimes.use.condoms <- 0.081395349
    sd.steady.usually.use.condoms <- 0.087209302
    sd.steady.always.use.condoms <- 0.523255814

    sd.casual.never.use.condoms <- 0.281420765
    sd.casual.rarely.use.condoms <- 0.013661202
    sd.casual.sometimes.use.condoms <- 0.043715847
    sd.casual.usually.use.condoms <- 0.054644809
    sd.casual.always.use.condoms <- 0.606557377

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

     #####################
   ## Jail related Parameters
  ## network.retention.multiplier <- 1


	### Age Specific Mortality ###
	### We don't strictly need all these age ranges, but
	### they are here for completeness -- format is [min_max)

	asm.15_20 <- 0.002333 / (365 * 1)
	asm.20_25 <- 0.002711  / (365 * 1)
	asm.25_30 <- 0.0028 / (365 * 1)
	asm.30_35 <- 0.003451 / (365 * 1)
	asm.35_40 <- 0.003477 / (365 * 1)
	asm.40_45 <- 0.004394 / (365 * 1)
	asm.45_50 <- 0.006315 / (365 * 1)
	asm.50_55 <- 0.010228 / (365 * 1)
	asm.55_60 <- 0.01605 / (365 * 1)
	asm.60_65 <- 0.022538 / (365 * 1)
	asm.65_70 <- 0.030628 / (365 * 1)

### CD4 Mortality ###
### Format is [min_max)
# for treated persons within the specified range
# ASM is increased by the specified amount
cd4m_treated.0_50	<- 51 / 100
cd4m_treated.50_100 <- 37 / 100
cd4m_treated.100_200 <- 26 / 100
cd4m_treated.200_1000 <- 0

# external infections per person days
# we mult. this value by the number of uninfected persons
# to get a probability of an infection for that day
# numerator here is 100 person years which we convert into days
# critical information is that| incidence is 5-9 per 100 py
# 28% of transmissions are linked to older BMSM
# 50%-80% of those are likely incident from older to younger BMSM (i.e. 14% to 21%)
# 14%-21% of infections are incident externally
# 5 * 28% * 50% = 0.7 (for min)
# 7 * 28% * 80% = 1.568 (for max)

external.infections.per.person.day.min <- 0.7 / (100 * 365) #revised as per write up from 0.8 and 1.6 for lower and upper bounds on 09.07.17
external.infections.per.person.day.max <- 1.568 / (100 * 365)

# factor used to calculate age related external inf probability --
# [19, 20) year old will be "factor" time more likely to get externally
# infected than [18,19) yearolds. Similarly, (20,21) year olds will
# be factor times more likely to get infected than the [19, 20) year olds,
# and factor^2 times more likely to get infected than the [18, 19) so on.
# external.infections.age.factor = 1.2
external.infections.age.factor = 1 # changed to 1 to remove more external infections going to older age groups


# range of number of tests in last two years min-max, fraction of the population
testing.prob.lt.1 <- "1-2|0.457377778"
testing.prob.lt.2 <- "3-4|0.299377778"
testing.prob.lt.3 <- "5-6|0.109377778"
testing.prob.lt.4 <- "7-8|0.055377778"
testing.prob.lt.5 <- "9-10|0.039377778"
testing.prob.lt.6 <- "11-12|0.012377778"
testing.prob.lt.7 <- "13-16|0.008877778"
testing.prob.lt.8 <- "17-20|0.010677778"
testing.prob.lt.9 <- "21-30|0.007177778"

testing.prob.gte.1 <- "1-2|0.457377778"
testing.prob.gte.2 <- "3-4|0.299377778"
testing.prob.gte.3 <- "5-6|0.109377778"
testing.prob.gte.4 <- "7-8|0.055377778"
testing.prob.gte.5 <- "9-10|0.039377778"
testing.prob.gte.6 <- "11-12|0.012377778"
testing.prob.gte.7 <- "13-16|0.008877778"
testing.prob.gte.8 <- "17-20|0.010677778"
testing.prob.gte.9 <- "21-30|0.007177778"

# jail
is.network.disruption.on <- FALSE
is.care.disruption.on <- FALSE

#incarceration.prob <- 0.0
incarceration.prob <- 0.0000787
incarceration.prob.meth <- 0.0000787
incarceration.prob.crack<- 0.0000787
incarceration.prob.ecstasy <- 0.0000787
#incarceration.prob <- 0.002

#incarceration.with.cji.prob <- 0.0
incarceration.with.cji.prob <- 0.0005173
incarceration.with.cji.prob.meth <- 0.0005173
incarceration.with.cji.prob.crack <- 0.0005173
incarceration.with.cji.prob.ecstasy <- 0.0005173
#incarceration.with.cji.prob <- 0.002

jail.infection.rate.window.size <- 90
jail.infection.rate.multiplier <- 0.5
jail.infection.rate.default <- 0.000091 

#jail.serving.time.mean <- 58
jail.serving.time.mean <- 58.4

post.release.interference.period.mean <- 90
#network.retention.multiplier <- 1
network.retention.multiplier <- 0.75

# format is "at_timestep|new_val"
# jail.infection.rate.multiplier.update <- "10.0|20.0"
# incarceration.prob.update <- "10.0|0.5"
# incarceration.with.cji.prob.update <- "10|0.2"
# jail.serving.time.mean.update <- "10|20"
# network.retention.multiplier.update <- "100|0.434"
# post.release.interference.period.mean.update <- "100|32"
# is.network.disruption.on.update <- "30|TRUE"
# is.care.disruption.on.update <- "30|TRUE"

######################
# substance use
meth.prop <- 9.21/100 
ecstasy.prop <- 4.02/100
crack.prop <- 17.42/100

# ART and substance use
meth.decline.art.always.adherent <- 50/100
ecstasy.decline.art.always.adherent <- 42/100
crack.decline.art.always.adherent <- 39/100

# PrEP and substance use
meth.prep.prop.use <- 7.1/100
ecstasy.prep.prop.use <- 5.4/100
crack.prep.prop.use <- 4.6/100

default.prep.mean.days.usage.meth <- 365 #updated 23may2018
default.prep.mean.days.usage.crack <- 365 #updated 23may2018
default.prep.mean.days.usage.ecstasy <- 365 #updated 23may2018
default.prep.mean.days.usage.meth.crack <- 365 #updated 23may2018
default.prep.mean.days.usage.meth.ecstasy <- 365 #updated 23may2018
default.prep.mean.days.usage.crack.ecstasy <- 365 #updated 23may2018
default.prep.mean.days.usage.meth.crack.ecstasy <- 365 #updated 23may2018

# Talk to John regarding PrEP and retention use

# incarceration and substance use
#meth.increase.incar.rate <-
#ecstasy.increase.incar.rate <-
#crack.increase.incar.rate <- 

