###########################################################
## 16 Aug 2013: "preg.mult" in the vector that is populated by 2.54 for
## currently pregnant women and 1 for everyone else should be called
## "preg.mult.vec" to avoid confusion with the constant which is also called
## preg.mult

## 15 Aug 2013: Change how adjustment for pregnancy occurs.
## Assign a pregnancy multiplier of "preg.mult" for all women who are currently
## pregnant, and a multiplier of 1 for everyone else. Instead of adjusting
## infectivities of pregnant women at the bottom.

## 11 Aug 2013: Adjust for progression of disease.
## Late stage multiplier should only apply to those who are not on ART.

## 1 Aug 2013: Got error below again.
## May be happening because the "vl.art.traj.slope" attribute is set to NA
## for all infecteds, and that needs to change at time of art initiation.
## Error ocured again at time step 190. "vl.art.traj.slope" is NA for ART
## initiators. Error occured at the same time step in two separate runs,
## and, in the first ART initiators. Therefore, it seems that the change in
## viral load for art initiators was staying at NA. To fix this, I made some
## changes in "update.treatment_d4.R", dated 1 Aug 2013. 

## 31 Jul 2013: Got error
## ``Error in if (viral.load.today[i] < 2) { : 
##   missing value where TRUE/FALSE needed''
## at time step 190 of a 300-step simulation:
## most likely in assign.infectivity function

## 30Jul2013: Rewrote assignment of infection stage using min and max
## time for each stage, instead of the "%in% operator."
## Add arguments for duration of infection, lengths of stages for
## assigning stage of infection.

## 30 Jul 2013: Move assignment of infection stage here

## 30 Jul 2013: Moved "assign.infectivity" to its own
## file from "common.functions_d8.R."
###########################################################

### Assign infectivity based on viral load

    assign.infectivity <-
  # 10Jun13: Account for effect of pregnancy
  function(nw, verbose,
           ## function describes extrapolated 
           ## infectivity at lowest viral load of log 2
           ## Hughes et al. give minimum infectivity at viral load of log 3.
           ## infection stages coded numerically
           min.chronic.infectivity.unadj, # changed in "common.functions_d3"
           num.sex.acts.per.timestep,
           acute.mult,
           late.mult,
           preg.mult, #10Jun13
           acute.length, #30Jul13
           chronic.length,
           late.length,
##           dur.inf,
           ...) {

  ## Top Matter
    ## browser()
     inf.status <- nw%v%"inf.status"
     ## art.status <- nw%v%"art.status"#30Jul13: Not needed
     viral.load.today <- nw%v%"viral.load.today"
     stage.of.infection <- nw%v%"stage.of.infection"
     time.since.infection <- nw%v%"time.since.infection"
     infectivity.today <- nw%v%"infectivity.today"
     curr.pregnancy.status <- nw%v%"curr.pregnancy.status"
     stage.of.infection <- nw%v%"stage.of.infection"
     art.status <- nw%v%"art.status"# 11Aug13: Added to account for disease progression,
                                    # i.e. late-stage multiplier should only apply
                                    # to those not on ART.
      
     preg.mult.vec <- rep(1, network.size(nw)) #15Aug13:to change adjustm. for pregnancy
     curr.pregnant <- which(curr.pregnancy.status == 1)
     preg.mult.vec[curr.pregnant] <- preg.mult #16Aug13

     for (i in 1:length(time.since.infection)){
       if (inf.status[i] == 1){ #30Jul13: added to skip over uninfected individuals
         ##if (time.since.infection[i] %in% acute.length){
           if (time.since.infection[i] <= max(acute.length)){
           stage.of.infection[i] <- 0
         } else if (#time.since.infection[i] %in% chronic.length){
                    time.since.infection[i] >= min(chronic.length) &&
                  time.since.infection[i] < min(late.length)
                    ){
           stage.of.infection[i] <- 1
         } else if (## time.since.infection[i] %in% late.length){
                    time.since.infection[i] >= min(late.length)
                    ){
           stage.of.infection[i] <- 2
         }
       }
         }

      nw %v% "stage.of.infection" <- stage.of.infection      
      stage.of.infection  <- nw %v% "stage.of.infection" # 31Jul13

     ## browser()     

     for (i in 1:network.size(nw)) {

       if (inf.status[i] == 1){
         ## 24 May '13: Add conditional for stage.of.infection not be ing NA
          if (!is.na(stage.of.infection[i])){
            ## browser()
            if (stage.of.infection[i] == 1) {
                                        # for chronically infected
             ## if (viral.load.today[i] == 3) {
             if (viral.load.today[i] < 2) {
                           ## browser()
               infectivity.today[i] <- 0
               infectivity.today[i] <- infectivity.today[i]*preg.mult.vec[i] #16Aug13
               infectivity.today[i] <- 1-(1-infectivity.today[i])^num.sex.acts.per.timestep
             }
           
           else if (viral.load.today[i] == 2) {
             infectivity.today[i] <- min.chronic.infectivity.unadj
             infectivity.today[i] <- infectivity.today[i]*preg.mult.vec[i] #15Aug13
             infectivity.today[i] <- 1-(1-infectivity.today[i])^num.sex.acts.per.timestep
           } else if (viral.load.today[i] > 2) {
             infectivity.today[i] <- min.chronic.infectivity.unadj*2.89^
             (viral.load.today[i] - 2)
             infectivity.today[i] <- infectivity.today[i]*preg.mult.vec[i] #15Aug13
             infectivity.today[i] <- 1-(1-infectivity.today[i])^num.sex.acts.per.timestep
           }

           if (verbose){
             cat("Chronically Infected are ", i,
                 "with viral load", viral.load.today[i],                 
                 "with infectivity", infectivity.today[i],
                 "\n")
           }

           } else if (stage.of.infection[i] == 0) {
                                        # for acutely infected
             ## browser()
             if (viral.load.today[i] < 2) {
               infectivity.today[i] <- 0
               infectivity.today[i] <- infectivity.today[i]*preg.mult.vec[i]#16Aug13
               infectivity.today[i] <- 1-(1-infectivity.today[i])^num.sex.acts.per.timestep
             }  else if (viral.load.today[i] == 2) {

               infectivity.today[i] <- min.chronic.infectivity.unadj*acute.mult
               infectivity.today[i] <- infectivity.today[i]*preg.mult.vec[i] #16Aug13
               infectivity.today[i] <- 1-(1-infectivity.today[i])^num.sex.acts.per.timestep
                                  ## set as 0.00497
           } else if (viral.load.today[i] > 2) {
             infectivity.today[i] <- min.chronic.infectivity.unadj*2.89^
             (viral.load.today[i] - 2)*acute.mult
             infectivity.today[i] <- infectivity.today[i]*preg.mult.vec[i] #16Aug13
             infectivity.today[i] <- 1-(1-infectivity.today[i])^num.sex.acts.per.timestep
           }

           if (verbose){
             cat("Acutely Infected are ", i,
                 "with viral load", viral.load.today[i],
                 "with infectivity", infectivity.today[i],
                 "\n")
           }
         } else if (stage.of.infection[i] == 2){
                                        # for late infected
           if (viral.load.today[i] < 2) {
             infectivity.today[i] <- 0
             infectivity.today[i] <- infectivity.today[i]*preg.mult.vec[i] #16Aug13
             infectivity.today[i] <- 1-(1-infectivity.today[i])^num.sex.acts.per.timestep
           }

           else if (viral.load.today[i] == 2) {
             if (art.status[i] != 1){ #11Aug13: Conditional on ART status added
               infectivity.today[i] <- min.chronic.infectivity.unadj*late.mult
                                  ## set as 0.00497
               infectivity.today[i] <- infectivity.today[i]*preg.mult.vec[i] #16Aug13
               infectivity.today[i] <- 1-(1-infectivity.today[i])^num.sex.acts.per.timestep
             } else if (art.status[i] == 1){ #11Aug13: Conditional on ART status added
               infectivity.today[i] <- min.chronic.infectivity.unadj
                                  ## set as 0.00497
               infectivity.today[i] <- infectivity.today[i]*preg.mult.vec[i] #16Aug13
               infectivity.today[i] <- 1-(1-infectivity.today[i])^num.sex.acts.per.timestep
             }
           } else if (viral.load.today[i] > 2) {
             if (art.status[i] != 1){ #11Aug13: Conditional on ART status added

               infectivity.today[i] <- min.chronic.infectivity.unadj*2.89^
               (viral.load.today[i]-2)*late.mult
               infectivity.today[i] <- infectivity.today[i]*preg.mult.vec[i] #16Aug13
               infectivity.today[i] <- 1-(1-infectivity.today[i])^num.sex.acts.per.timestep
             } else if (art.status[i] == 1){ #11Aug13: Conditional on ART status added

               infectivity.today[i] <- min.chronic.infectivity.unadj*2.89^
               (viral.load.today[i]-2)
               infectivity.today[i] <- infectivity.today[i]*preg.mult.vec[i] #16Aug13
               infectivity.today[i] <- 1-(1-infectivity.today[i])^num.sex.acts.per.timestep
             }
             
           }

           if (verbose){
             cat("Late-Infected (based on time since infection) are ", i,
                 # 11Aug13: Parenthetical time since infection based defn added, because
                 # if these ind. are on ART, their infectivity will not be the
                 # same as those late-infecteds who are not on ART.
                 "with viral load", viral.load.today[i],                 
                 "with infectivity", infectivity.today[i],
                 "\n")
           }

         }
           
         }
       }
     }
     ## 10Jun13: Adjust infectivity of pregnant women
     ## browser()
     ## curr.pregnant <- which(curr.pregnancy.status == 1) ## 15 Aug 13
   ##   if (length(curr.pregnant) > 0){
   ##     infectivity.today[curr.pregnant] <-
   ##     # infectivity.today[curr.pregnant]*preg.mult
   ## }
     if (verbose){
       cat("Currently pregnant women are ", curr.pregnant, "\n",
           "Adjusted infectivities of pregnant women: ",
           infectivity.today[curr.pregnant], 
           "\n")
           }
     
     ## Update Infectivity
        nw%v%"infectivity.today" <- infectivity.today


     
     ## Return Network Object
        return(nw)

   }
