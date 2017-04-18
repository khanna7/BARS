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

      ## list discordant partnerships
      ug.nw.el <- as.edgelist(network.extract(ug.nw,
                                              at = time,
                                              retain.all.vertices = T)) 

      status.el <- matrix((ug.nw %v% "inf.status")[ug.nw.el], ncol = 2)
      inf.time <- ug.nw %v% "inf.time"
      time.since.infection <- ug.nw %v% "time.since.infection" # ASK


      discordant.mpos <- intersect(which(status.el[, 1] == 1),
                                    which(status.el[, 2] == 0)) #male positive, female negative

      transmittable.m <- ug.nw.el[discordant.mpos, 1]
      infectible.f <- ug.nw.el[discordant.mpos, 2]

      discordant.fpos <- intersect(which(status.el[, 1] == 0),
                                    which(status.el[, 2] == 1)) #female positive, male negative

      infectible.m <- ug.nw.el[discordant.fpos, 1]
      transmittable.f <- ug.nw.el[discordant.fpos, 2]

      ## incidence: can be computed from generated csv, by computing the number of new infections/number uninfected at every time step

      ## demographic parameters: i recorded number of births, deaths and associated parameters in a separate csv that was generated at each time step
