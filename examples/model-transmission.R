## model transmission
   ## if both +ve and -ve partner are of same age classification, P(transmission) = x%,
   ## else, probability of transmission = 2x%


  transmission <-
      function(nw, verbose,
               like.age.prob,
               unlike.age.prob,
               ...
               ){

        nw.el <- as.edgelist(network.extract(nw,
                                             at = time,
                                             retain.all.vertices = T)) #8Oct13

       ## 3Jun13: only pulling out active network

       status.el <- matrix((nw %v% "inf.status")[nw.el], ncol = 2)
       inf.time <- nw %v% "inf.time"
       time.since.infection <- nw %v% "time.since.infection" # ASK
       young <- nw%v%"young" 

       ## discordant partnerships with "left" partner of
        ## status matrix above infected
           ## list    
           discordant.lpos <- intersect(which(status.el[, 1] == 1),
                                       which(status.el[, 2] == 0))
        
           transmittable.l <- nw.el[discordant.lpos, 1]
           infectible.r <- nw.el[discordant.lpos, 2]

           if (length(transmittable.l) != length(infectible.r)){
               stop("serodiscordant partnership matrix doesn't make sense")
           }

           ## compute infectivity in above partnerships
           for (i in 1:length(transmittable.l)){
            
               if (young[transmittable.l[i]]  == young[infectible.r[i]]){
                   transmission <- rbinom(1, 1, like.age.prob)    
               } else {
                   transmission <- rbinom(1, 1, unlike.age.prob)
               }

               if (transmission == 1){
                   inf.status[infectible.r[i]] <- 1
                   nw <- set.vertex.attribute(nw, "inf.status", 1, v=infectible.r[i])
                   nw <- set.vertex.attribute(nw, "inf.time", time, v=infectible.r[i])
               }
           }
                                    
       ## discordant partnerships with "right" partner of
        ## status matrix above infected
           ## list            
           discordant.rpos <- intersect(which(status.el[, 2] == 1),
                                        which(status.el[, 1] == 0)
                                        )
           transmittable.r <- nw.el[discordant.rpos, 2]
           infectible.l    <- nw.el[discordant.rpos, 1]
                
           if (length(transmittable.r) != length(infectible.l)){
               stop("serodiscordant partnership matrix doesn't make sense")
           }
                
           ## compute infectivity in above partnerships




     

      return(nw);
      }
