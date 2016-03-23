## Example file to retrieve data required for diagnostics and testing

   rm(list=ls())

   ## libraries and data
   library(network)
   library(ergm)
   library(networkDynamic)
   load("/project/khanna7/Projects/sero-discordant-couples/Burnin_Data/burnin.21Feb_UG_phi004_newatts_run1.RData")

   ## extracting information at the last time point:
      ## (example taken from Uganda burnin data in PMTCT/sero-discordant couples projects)

      ## extract networks
      ug.nw <- nw #networkDynamic object
      ug.net <- network.collapse(ug.nw, at=1040) #convert nD object to network object

      ## prevalence
      ug.inf <- (which(ug.net%v%"inf.status" == 1))
      length(ug.inf)/network.size(ug.net)

      ## on ART
      ug.on.art <- (which(ug.net%v%"art.status" == 1))

      length(ug.on.art)/length(ug.inf) # % infected on ART

      ## CD4 <= 350
      ug.cd4.less.350 <- (which(ug.net%v%"cd4.count.today" <= 350))
      length(ug.cd4.less.350)

      ## not on ART
      ug.noton.art <- (which(ug.net%v%"art.status" == 0))
      ug.inf.noton.art <- intersect(ug.inf, ug.noton.art)
      ug.inf.cd4less350 <- (intersect(ug.inf,ug.cd4.less.350))
      ug.inf.art.elig <- intersect(ug.inf.cd4less350,
                                   ug.noton.art)
      length(ug.inf.art.elig)

      ## viral load distribution
      last.viral.load <- ug.net %v% "viral.load.today"
      summary(last.viral.load) # everyone
      summary(last.viral.load[ug.inf]) # infected only

      ## incidence: can be computed from generated csv, by computing the number of new infections/number uninfected at every time step

      ## demographic parameters: i recorded number of births, deaths and associated parameters in a separate csv that was generated at each time step
