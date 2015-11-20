
## 20 Feb 2014: Differentiate post-cessation trajectories for Option A and B

## 20 Dec 2013:
## Option B: Add condition for ART type 4. 

## 3 Dec 2013:
## a. Cap viral load for women who have stopped ART --
## that is where the problem was. Correction from 2 Dec 2013 was in wrong place.

## b. Cap is only necessary for people in chronic stage, or later. Currently
## it was applied to people in peak stage going to late stage. Change cap to only
## apply to chronic stage and later.

## Apply cap to all people 

## 2 Dec 2013: For some women after stopping ART, viral load rises above the permitted
## level. We need to cap that at the maximum permissible level. 

## 22 Aug 2013: Add age-based duration of infection at time of infection --
## to calculate change in viral load during late stage

## 26 Jul 2013: For women who have ceased to receive scART, compute viral
## load in terms of slope of the 6-parameter viral load curve, than
## simply using the same code for treatment naive individuals

## Phase I: infection to peak viremia (0 to 14 days)
##       II: peak viremia to start of late stage stage (14 to 121 days)
##       III: start of chronic stage to start of late stage (122 to 1877 days)
##       IV: start of late stage to death (1878 to 3301 days)

## 26 Jul 2013: Add modification in viral load increasae for women post-ART
## who have crossed their pre-ART level

## 11 Jul 2013: Algorithm that is currently implemented:
## If treatment-naive, according to six parameter curve.
## If a) regular ART, then undetectable by "time.to.full.suppression"
##       Attribute "vl.art.traj.slope" needed.
##    b) scART for pregnant women, then decline of 1.1 log between initiation
##       and delivery. Attribute "vl.art.traj.slope" needed.
##    c) after cessation of short-course ART, the 1.1 log is regained in
##       "sc.art.postcess.ret.bl."
##  NB: Keep absolute values of entries in "vl.art.traj.slope" vector.
## Add or subtract these entries from current viral load depending on whether
## the person is on or off ART.

## 11 Jul 2013: Commented out condition
## if (viral.load.today[i] >= undetectable.vl){
## under "art.status == 1" and "art.type==1" because
## it was not complementary to the situation given by the else if below it
## (which decides if someone is at undetectable viral load -- needed to
## establish when treated individuals become undetectable)
## and hence was causing problems with the exection of (a) correctly above.

## 11 July 2013: For previous versions of this function,
## see "common.functions_d7.R."

## 10 July 2013: Function in separate file

compute.vl.mp.art4 <- function(nw, verbose, 
                               time.infection.to.peak.viral.load,
                               peak.viral.load,
                               time.infection.to.viral.set.point,
                               set.point.viral.load,
                               time.infection.to.late.stage,
                               late.stage.viral.load,
                               # dur.inf, #22Aug13
                               time.to.full.supp,
                               undetectable.vl,
                               sc.art.postcess.ret.bl,
                               size.of.timestep, #28May13
                               optB.vl.postcess.ret.bl, #20Feb14
                               ...
                               ) {

     ## 11Jul13: Reduction in viral voad should not be the same at every time
     ## step for everyone. The decrease should be 1.1 log ("optA.vl.reduction")
     ## between initiation and
     ## delivery. Similarly, the increase in viral load after cessation
     ## is also added by the "vl.art.traj.slope" attribute,
     ## with the "optA.vl.reduction" amount gained in "sc.art.postcess.ret.bl."
  
     ## 5 Jul 2013: make corrections for option A viral load reduction
  
     ## 12June13: Rewrite now to compute individual slopes for viral load
     ## trajectories in treated individuals.

     ## 10Jun13: Modify to include decline in CD4 count and increase in
     ## viral load after cessation of treatment in pregnant women (Option A)
     ## Also model CD4 and viral load trajectories for these individuals
     ## while on treatment

     ## Loop to assign viral load based on time sice infection
     ## this piece of code does not have to be changed because
     ## we have changed the length of the relevant parameters here
     ## on account of size of the timestep --
     ## a.time.infection.to.peark.viral.load;
     ## b. time.infection.to.viral.set.point
     ## c. time.infection.to.late.stage
     ## d. dur.inf
     ## e. take midpoints instead of right endpoints to compute viral loads over
     ## 14-day intervals

  ## Top Matter  
     inf.status <- nw%v%"inf.status"
     art.status <- nw%v%"art.status"
     art.type <- nw%v%"art.type"
     time.since.infection <- nw%v%"time.since.infection" 
     time.since.art.initiation <- nw%v%"time.since.art.initiation" 
     viral.load.today <- nw%v%"viral.load.today"
     vl.art.traj.slope <- nw%v%"vl.art.traj.slope"
     time.since.art.cessation <- nw%v%"time.since.art.cessation" #25Jul13
     dur.inf.by.age <- nw%v%"dur.inf.by.age" #22Aug13
     
  ## Male and Female IDs    
     male.id.curr <- nwmodes(nw, 1)
     female.id.curr <- nwmodes(nw, 2)
     
     ##browser()         
     for (i in 1:length(time.since.infection)){
      ##if(viral.load.today[i] >= 0){%
       if ((is.na(art.status[i]) && is.na(art.type[i])) ||
           (art.status[i] == 0 && is.na(art.type[i]))
           ## if both art.status and art.type are NA,
           ## or if art.status is 0 and art.type is NA
           ){ 
                                        # untreated individuals --
                                        # men and women are not considered separately 

           if ( (!is.na(time.since.infection[i])) && 
                (time.since.infection[i] <= time.infection.to.peak.viral.load)
                ){

             viral.load.today[i] <- mean(c(0, peak.viral.load))

             if (verbose){
               cat("Untreated actors going to peak viral load are: ", i, "\n")
             }
             
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

               
             if (verbose){
               cat("Untreated actors going to set-point viral load are: ", i, "\n")
             }
             
           } else if ( (time.since.infection[i] %in%
                        (time.infection.to.viral.set.point+1):
                        (time.infection.to.late.stage))
                      ) {
             viral.load.today[i] <- set.point.viral.load

             if (verbose){
               cat("Untreated actors at set-point viral load are: ", i, "\n")
             }
             
           }  else if ( (!is.na(time.since.infection[i])) &&
                      (time.since.infection[i] > time.infection.to.late.stage)
                    ){

             if (viral.load.today[i] < late.stage.viral.load){#3Dec13 
             viral.load.today[i] <- set.point.viral.load + #22Aug13
               ((late.stage.viral.load - set.point.viral.load)*
                 (time.since.infection[i] - time.infection.to.late.stage)/
                   ( (dur.inf.by.age[i]-1) - time.infection.to.late.stage ))*1/2
                                        # so we arrive at midpoint
           } else if (viral.load.today[i] >= late.stage.viral.load){#3Dec13
             viral.load.today[i] <- late.stage.viral.load
           }
             
             if (verbose){
               cat("Untreated actors at final stage are: ", i, "\n")
             }


           } ## check this one what happens to viral load in the last stage


         } else if (art.status[i] == 1 &&
                    (art.type[i] == 1 || art.type[i] == 4) ## 20Dec13: Option B
                    ) {
                                      # individuals on regular ART -- 
                                      # men and women are not considered separately

           ##if (viral.load.today[i] >= undetectable.vl){
            ## 29May13: should this go to midpoint too??
             if ( !is.na(time.since.art.initiation[i]) &&
                 (time.since.art.initiation[i] < time.to.full.supp)){
               viral.load.current <- viral.load.today[i]
               viral.load.today[i] <- viral.load.today[i] - vl.art.traj.slope[i]
             
               if (verbose){
                 cat("Treated actors on regular ART,
                      not yet suppressed, are: ", i, "\n")
               }
             ##}
             } else if ((!is.na(time.since.art.initiation[i])) &&
                    (time.since.art.initiation[i] >= time.to.full.supp)){
               viral.load.today[i] <- undetectable.vl

               if (verbose){
               cat("Treated actors, fully suppressed, are: ", i, "\n")
             }
         }
       } else if (art.status[i] == 1 && art.type[i] == 2) {
                                      # individuals on regular ART -- 
                                      # men and women are not considered separately

           if (viral.load.today[i] >= undetectable.vl){
            ## 29May13: should this go to midpoint too??

               viral.load.current <- viral.load.today[i]
               viral.load.today[i] <- viral.load.today[i] - vl.art.traj.slope[i]
             
               if (verbose){
                 cat("Treated actors on scART,
                      not yet suppressed, are: ", i, "\n")
               }
             }
         } else if (art.status[i] == 0 && art.type[i] == 3) {
                                      # individuals after cessation of Option A

            ## 29May13: should this go to midpoint too??

               viral.load.current <- viral.load.today[i]

               if (time.since.art.cessation[i] <= sc.art.postcess.ret.bl) {
                 ##20Feb14: 1 month return to pre-ART level
                 viral.load.today[i] <- viral.load.today[i]+vl.art.traj.slope[i]

               } else if (time.since.art.cessation[i] > sc.art.postcess.ret.bl){
                 ##20Feb14: conditional, after 1 month of cessation,
                 ## viral load trajectory will change as it does in ART-naive individuals
                 if(!is.na(time.since.art.cessation[i]) &&
                         time.since.art.cessation[i] < sc.art.postcess.ret.bl){
                  ## for post-scART women who have not yet reached pre-ART viral load 
                 viral.load.today[i] <- viral.load.today[i] + vl.art.traj.slope[i]
               } else if (!is.na(time.since.art.cessation[i]) &&
                          time.since.art.cessation[i] >= sc.art.postcess.ret.bl){
                 ##  25Jul13: for post-scART women who have crossed pre-ART viral load 

                     ## code-chunk below taken from slope computations in
                     ## viral load trajectories
                     #########################################################

                 ## no need to multiply slopes by the size of timestep,
                 ## because durations of all infection stages are adjusted for
                 ## the size of timestep.
                 
                 if ( (!is.na(time.since.infection[i])) && 
                     (time.since.infection[i] <= time.infection.to.peak.viral.load)
                     ){

                   viral.load.today[i] <- mean(c(0, peak.viral.load))
                      ## this case should really not be possible, because
                      ## these women have already been pregnant, delivered,
                      ## and stopped ART. So time.since.infection is most likely
                      ## not <= 1.

                 }  else if ( (time.since.infection[i] %in%
                               (time.infection.to.peak.viral.load+1):
                               (time.infection.to.viral.set.point))
                             )  {

                     viral.load.today[i] <- viral.load.today[i]+
                       (set.point.viral.load - peak.viral.load)/
                         (time.infection.to.viral.set.point-
                          time.infection.to.peak.viral.load)
                      ## slope for phase II

                 } else if ( (time.since.infection[i] %in%
                              (time.infection.to.viral.set.point+1):
                              (time.infection.to.late.stage))
                            ) {

                   if (viral.load.today[i] < late.stage.viral.load){#3Dec13: cap
                     viral.load.today[i] <- viral.load.today[i]
                    ## this line has slope=0 # Phase III

                   } else if (viral.load.today[i] >= late.stage.viral.load){#3Dec13: cap
                     viral.load.today[i] = late.stage.viral.load
                   }
                   
                 }  else if ( (!is.na(time.since.infection[i])) &&
                             (time.since.infection[i] > time.infection.to.late.stage)
                             ){
           
                   if (viral.load.today[i] < late.stage.viral.load){#3Dec13:b
                     viral.load.today[i] <- viral.load.today[i] + #22Aug13
                       (late.stage.viral.load - set.point.viral.load)/
                         (dur.inf.by.age[i] - time.infection.to.late.stage)

                   } else if (viral.load.today[i] >= late.stage.viral.load){#3Dec13:b
                     viral.load.today[i]=late.stage.viral.load
                   }
                   
                        # Slope for Phase IV
                   
                   if (verbose){
                     cat("Untreated actors at final stage are: ", i, "\n")
                   }

                   #########################################################

           } ## check this one what happens to viral load in the last stage

                 if (verbose){
                   cat("Women off scART,
                      who have crossed pre-ART levels, are: ", i, "\n")
                 }

               }
               
               }
             } else if (art.status[i] == 0 && art.type[i] == 5) {
                                      # individuals after cessation of Option B 

            ## 29May13: should this go to midpoint too??

               viral.load.current <- viral.load.today[i]

               if (time.since.art.cessation[i] <= sc.art.postcess.ret.bl) {
                 ##20Feb14: 1 month return to pre-ART level
                 viral.load.today[i] <- viral.load.today[i]+vl.art.traj.slope[i]

               } else if (time.since.art.cessation[i] > sc.art.postcess.ret.bl){
                 ##20Feb14: conditional, after 1 month of cessation,
                 ## viral load trajectory will change as it does in ART-naive individuals
                 if(!is.na(time.since.art.cessation[i]) &&
                         time.since.art.cessation[i] < sc.art.postcess.ret.bl){
                  ## for post-scART women who have not yet reached pre-ART viral load 
                 viral.load.today[i] <- viral.load.today[i] + vl.art.traj.slope[i]
               } else if (!is.na(time.since.art.cessation[i]) &&
                          time.since.art.cessation[i] >= sc.art.postcess.ret.bl){
                 ##  25Jul13: for post-scART women who have crossed pre-ART viral load 

                     ## code-chunk below taken from slope computations in
                     ## viral load trajectories
                     #########################################################

                 ## no need to multiply slopes by the size of timestep,
                 ## because durations of all infection stages are adjusted for
                 ## the size of timestep.
                 
                 if ( (!is.na(time.since.infection[i])) && 
                     (time.since.infection[i] <= time.infection.to.peak.viral.load)
                     ){

                   viral.load.today[i] <- mean(c(0, peak.viral.load))
                      ## this case should really not be possible, because
                      ## these women have already been pregnant, delivered,
                      ## and stopped ART. So time.since.infection is most likely
                      ## not <= 1.

                 }  else if ( (time.since.infection[i] %in%
                               (time.infection.to.peak.viral.load+1):
                               (time.infection.to.viral.set.point))
                             )  {

                     viral.load.today[i] <- viral.load.today[i]+
                       (set.point.viral.load - peak.viral.load)/
                         (time.infection.to.viral.set.point-
                          time.infection.to.peak.viral.load)
                      ## slope for phase II

                 } else if ( (time.since.infection[i] %in%
                              (time.infection.to.viral.set.point+1):
                              (time.infection.to.late.stage))
                            ) {

                   if (viral.load.today[i] < late.stage.viral.load){#3Dec13: cap
                     viral.load.today[i] <- viral.load.today[i]
                    ## this line has slope=0 # Phase III

                   } else if (viral.load.today[i] >= late.stage.viral.load){#3Dec13: cap
                     viral.load.today[i] = late.stage.viral.load
                   }
                   
                 }  else if ( (!is.na(time.since.infection[i])) &&
                             (time.since.infection[i] > time.infection.to.late.stage)
                             ){
           
                   if (viral.load.today[i] < late.stage.viral.load){#3Dec13:b
                     viral.load.today[i] <- viral.load.today[i] + #22Aug13
                       (late.stage.viral.load - set.point.viral.load)/
                         (dur.inf.by.age[i] - time.infection.to.late.stage)

                   } else if (viral.load.today[i] >= late.stage.viral.load){#3Dec13:b
                     viral.load.today[i]=late.stage.viral.load
                   }
                   
                        # Slope for Phase IV
                   
                   if (verbose){
                     cat("Untreated actors at final stage are: ", i, "\n")
                   }

                   #########################################################

           } ## check this one what happens to viral load in the last stage

                 if (verbose){
                   cat("Women off scART,
                      who have crossed pre-ART levels, are: ", i, "\n")
                 }

               }
               
               }
             }  else if (art.status[i] == 0 && art.type[i] == 5) {
                                      # individuals on regular ART -- 
                                      # men and women are not considered separately


            ## 29May13: should this go to midpoint too??

               viral.load.current <- viral.load.today[i]

               if (time.since.art.cessation[i] <= optB.vl.postcess.ret.bl) {
                 ##20Feb14:  2 month return to pre-ART level for post-cess Opt B
                 viral.load.today[i] <- viral.load.today[i]+vl.art.traj.slope[i]

               } else if (time.since.art.cessation[i] > optB.vl.postcess.ret.bl){
                 ##20Feb14: conditional, 2 months after cessation Option B,
                 ## viral load trajectory will change as it does in ART-naive individuals
                 if(!is.na(time.since.art.cessation[i]) &&
                         time.since.art.cessation[i] < sc.art.postcess.ret.bl){
                  ## for post-scART women who have not yet reached pre-ART viral load 
                 viral.load.today[i] <- viral.load.today[i] + vl.art.traj.slope[i]
               } else if (!is.na(time.since.art.cessation[i]) &&
                          time.since.art.cessation[i] >= sc.art.postcess.ret.bl){
                 ##  25Jul13: for post-scART women who have crossed pre-ART viral load 

                     ## code-chunk below taken from slope computations in
                     ## viral load trajectories
                     #########################################################

                 ## no need to multiply slopes by the size of timestep,
                 ## because durations of all infection stages are adjusted for
                 ## the size of timestep.
                 
                 if ( (!is.na(time.since.infection[i])) && 
                     (time.since.infection[i] <= time.infection.to.peak.viral.load)
                     ){

                   viral.load.today[i] <- mean(c(0, peak.viral.load))
                      ## this case should really not be possible, because
                      ## these women have already been pregnant, delivered,
                      ## and stopped ART. So time.since.infection is most likely
                      ## not <= 1.

                 }  else if ( (time.since.infection[i] %in%
                               (time.infection.to.peak.viral.load+1):
                               (time.infection.to.viral.set.point))
                             )  {

                     viral.load.today[i] <- viral.load.today[i]+
                       (set.point.viral.load - peak.viral.load)/
                         (time.infection.to.viral.set.point-
                          time.infection.to.peak.viral.load)
                      ## slope for phase II

                 } else if ( (time.since.infection[i] %in%
                              (time.infection.to.viral.set.point+1):
                              (time.infection.to.late.stage))
                            ) {

                   if (viral.load.today[i] < late.stage.viral.load){#3Dec13: cap
                     viral.load.today[i] <- viral.load.today[i]
                    ## this line has slope=0 # Phase III

                   } else if (viral.load.today[i] >= late.stage.viral.load){#3Dec13: cap
                     viral.load.today[i] = late.stage.viral.load
                   }
                   
                 }  else if ( (!is.na(time.since.infection[i])) &&
                             (time.since.infection[i] > time.infection.to.late.stage)
                             ){
           
                   if (viral.load.today[i] < late.stage.viral.load){#3Dec13:b
                     viral.load.today[i] <- viral.load.today[i] + #22Aug13
                       (late.stage.viral.load - set.point.viral.load)/
                         (dur.inf.by.age[i] - time.infection.to.late.stage)

                   } else if (viral.load.today[i] >= late.stage.viral.load){#3Dec13:b
                     viral.load.today[i]=late.stage.viral.load
                   }
                   
                        # Slope for Phase IV
                   
                   if (verbose){
                     cat("Untreated actors at final stage are: ", i, "\n")
                   }

                   #########################################################

           } ## check this one what happens to viral load in the last stage

                 if (verbose){
                   cat("Women off scART,
                      who have crossed pre-ART levels, are: ", i, "\n")
                 }

               }
               
               }
             }
     }
     ## Update viral load
        nw %v% "viral.load.today" <- viral.load.today

       if(verbose){
         cat("Viral loads are ", nw%v%"viral.load.today", "\n")
       }
         
     
     ## Return network object
        return(nw)
   }
