## test properties of network at last time step

   nw #networkDynamic object
   last.time <- 25
   nw.last.time <- network.collapse(nw, at=time) #cross-sectinoal network at last time

   nw.last.time 
   network.edgecount(nw.last.time) #num. of edges at last time
   network.size(nw.last.time) #vertex count at last time

   ## number infected
   length(which(nw%v%"inf.status" == 1)) #in "full" network
   length(which(nw.last.time%v%"inf.status" == 1)) #at last time

   ## number infected through sexual transmission during the simulation
   length((nw%v%"infector")[which(!is.na(nw%v%"infector"))]) #in "full" network 
   length((nw.last.time%v%"infector")[which(!is.na(nw.last.time%v%"infector"))]) #at last time

   ## number infected at start
   length(intersect(which(is.na(nw%v%"infector")), which(nw%v%"inf.status" == 1)))

   ## Number infected during simulation + Number infected at start =? Total number infected
   length((nw%v%"infector")[which(!is.na(nw%v%"infector"))]) + length(intersect(which(is.na(nw%v%"infector")), which(nw%v%"inf.status" == 1))) ==    length(which(nw%v%"inf.status" == 1)) 


   ## cross-tabs of infection status and infector ID
   xtabs(~factor(nw%v%"inf.status", exclude=NULL)+
             factor(nw%v%"infector", exclude=NULL))
  

