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
       
       count <- 0
           discordant.1pos <- intersect(which(status.el[, 1] == 1),
                                       which(status.el[, 2] == 0))
        
           transmittable.1 <- nw.el[discordant.1pos, 1]
           infectible.2 <- nw.el[discordant.1pos, 2]

           if (length(transmittable.1) != length(infectible.2)){
               stop("serodiscordant partnership matrix doesn't make sense")
           }

           ## simulate transmission in above partnerships
           for (i in 1:length(transmittable.1)){
            
               if (young[transmittable.1[i]]  == young[infectible.2[i]]){
                   transmission.ok <- rbinom(1, 1, like.age.prob)    
               } else {
                   transmission.ok <- rbinom(1, 1, unlike.age.prob)
               }

               if (transmission.ok == 1){
                   inf.status[infectible.2[i]] <- 1
                   nw <- set.vertex.attribute(nw, "inf.status", 1, v=infectible.2[i])
                   nw <- set.vertex.attribute(nw, "inf.time", time, v=infectible.2[i])
                   nw <- set.vertex.attribute(nw, "infector", transmittable.1[i], v=infectible.2[i])
                   count <- count + 1
               }
           }

       ## discordant partnerships with "right" partner of
        ## status matrix above infected
           ## list            
           discordant.2pos <- intersect(which(status.el[, 2] == 1),
                                        which(status.el[, 1] == 0)
                                        )
           transmittable.2 <- nw.el[discordant.2pos, 2]
           infectible.1    <- nw.el[discordant.2pos, 1]
                
           if (length(transmittable.2) != length(infectible.1)){
               stop("serodiscordant partnership matrix doesn't make sense")
           }
                
           ## simulate transmission in above partnerships
           for (i in 1:length(transmittable.2)){
            
               if (young[transmittable.2[i]]  == young[infectible.1[i]]){
                   transmission.ok <- rbinom(1, 1, like.age.prob)    
               } else {
                   transmission.ok <- rbinom(1, 1, unlike.age.prob)
               }

               if (transmission.ok == 1){
                   inf.status[infectible.1[i]] <- 1
                   nw <- set.vertex.attribute(nw, "inf.status", 1, v=infectible.1[i])
                   nw <- set.vertex.attribute(nw, "inf.time", time, v=infectible.1[i])
                   nw <- set.vertex.attribute(nw, "infector", transmittable.2[i], v=infectible.1[i])
                   count <- count + 1
               }
           }

      return(list("network"=nw, "infected_count"=count));
      }
