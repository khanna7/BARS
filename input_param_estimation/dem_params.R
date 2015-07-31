  ## file to estimate demographic parameters for three counties
     ## estimate the following parameters:
     ## 1. Distribution of ages within the three counties
     ## 2. Race composition of the three counties
     ## 3. Net population growth rate (births+in migrations ? (deaths+out migrations))
     ## 4. Growth parameters(Births, inmigrations)
     ## 5. Male circumcision rates (overall or by race)


  ## Cook county

     ## age, sex, race
     cook.gq.ppl <- read.csv("../input_data/cook_2010_ver1_17031/2010_ver1_17031_synth_gq_people.txt")
     dim(cook.gq.ppl)
     colnames(cook.gq.ppl)

     cook.gq.ppl.age <- cook.gq.ppl$age
     summary(cook.gq.ppl.age)

     cook.gq.ppl.sex <- cook.gq.ppl$sex
     table(cook.gq.ppl.sex)/(length(cook.gq.ppl.sex)) #1=male, 2=female

     ## race
     cook.ppl <- read.csv("../input_data/cook_2010_ver1_17031/2010_ver1_17031_synth_people.txt")
     dim(cook.ppl)
     colnames(cook.ppl)

     cook.ppl.age <- cook.ppl$age
     summary(cook.ppl.age)

     cook.ppl.sex <- cook.ppl$sex
     table(cook.ppl.sex)/(length(cook.ppl.sex)) #1=male, 2=female

     cook.ppl.race <- cook.ppl$race
     table(cook.ppl.race)/(length(cook.ppl.race)) 
             ## Race coding:
             ## 1 White alone
             ## 2 Black or African American alone
             ## 3 American Indian alone
             ## 4 Alaska Native alone
             ## 5 American Indian and Alaska Native tribes specified; or American 
             ##   Indian or Alaska native, not specified and no other races
             ## 6 Asian alone
             ## 7 Native Hawaiian or Other Pacific Islander alone
             ## 8 Some other race alone
             ## 9 Two or more major race groups

     ## Another source: http://quickfacts.census.gov/qfd/states/17/17031.html
             ## includes growth rate information
