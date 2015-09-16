update.vital.dynamics <-
  function(nw, verbose=TRUE,
           max.survival,
           daily.death.prob,
           daily.birth.rate,
           ...
  ){
    
  
    
    ## Update temporal attributes
       ## age
       nw%v%"age" <- nw%v%"age"+1

    ##########################################################################   
    ## mortality
       ## by age
       dying.of.age <- which(floor(age) == max.survival &
                              is.active(nw, v=1:network.size(nw), at=time))#26Aug13:VERYIMP
 
       if(length(dying.of.age)>0) {
        nw <- deactivate.vertices(nw, onset=time, terminus=Inf, v=dying.of.age)
        dying.of.age.edges <- get.edgeIDs.active(nw, dying.of.age[1], at=time)
        ## In theory an sapply with an unlist
        ## could handle this; in practice, there are issues.
        for (i in dying.of.age[-1]) {
          dying.of.age.edges <- c(dying.of.age.edges,
                                  get.edgeIDs.active(nw, i, at=time))
        }
        if (length(dying.of.age.edges)>0) {
            nw <- deactivate.edges(nw,onset=time, terminus=Inf, e=dying.of.age.edges)
           }
      
           }
    
        if (verbose) cat("Deaths due to age", length(dying.of.age), "\n")
    
        # Activation of network that is left
        node.active <- is.active(nw, v=1:network.size(nw), at=time)
        # update the list of still-alive nodes
        active.nodes <- which(node.active)
        popsize.temp <- sum(node.active)
        if(popsize.temp==0) break
    
     ## grim reaper deaths
        nw.new.popsize <- network.size(nw)
        dying.of.grim.reaper.index <- which(rbinom(popsize.temp, 1, daily.death.prob) == 1)
        dying.of.grim.reaper <- active.nodes[dying.of.grim.reaper.index]
    
        if (verbose) cat("Grim reaper deaths", length(dying.of.grim.reaper), "\n")
    
        if(length(dying.of.grim.reaper)>0) {
        nw <- set.vertex.attribute(nw, "time.of.death",
                                   time, v=dying.of.grim.reaper)
      
        nw <- deactivate.vertices(nw, onset=time, terminus=Inf, v=dying.of.grim.reaper)
        dying.of.grim.reaper.edges <- get.edgeIDs.active(nw, dying.of.grim.reaper[1], at=time)
      
        for (i in dying.of.grim.reaper[-1]) {
            dying.of.grim.reaper.edges <- c(dying.of.grim.reaper.edges,
                                           get.edgeIDs.active(nw, i, at=time))
      }
      
        if (length(dying.of.grim.reaper.edges)>0) {
          nw <- deactivate.edges(nw,onset=time, terminus=Inf, e=dying.of.grim.reaper.edges)
          }
      
          }
    
    # 26 Aug 2013: Update active nodes
          node.active <- is.active(nw, v=1:network.size(nw), at=time)
          # update the list of still-alive nodes
          active.nodes <- which(node.active)
          popsize.temp <- sum(node.active)
          if(popsize.temp==0) break
    
    ####################################################################
    ### births
    
    node.active <- is.active(nw, v=1:network.size(nw), at=time)
    # update the list of still-alive nodes
    active.nodes <- which(node.active)
    popsize.temp <- sum(node.active)
    
    if(popsize.temp==0) break
    
    nintros <- rpois(1, daily.birth.rate*popsize.temp)
    
    if (verbose){ 
      cat("Number of Intros is ", nintros, "\n")                         
    }
    
 
    #     if (nintros>0) {  
    #       for (zzz in 1:nintros) nw <- add.vertices(nw, 1, last.mode=FALSE)
    #     } # This loop approach is temp to get around bug in network
    #     
    if(nintros > 0){  
     nw <- add.vertices(nw, nintros)
    }
    
    ########################################################################
    ## ASK: Lines below commented out because we don't
    ## have an attribute called status
    ## important for assigning status to new entries
    ## nodes.to.activate <- which(is.na(nw%v%'status'))
    ## if (verbose) cat("Births",length(nodes.to.activate),"\n")
    ## if (length(nodes.to.activate)>0) {
    ##   nw <- activate.vertices(nw,onset=time, terminus=Inf, v=nodes.to.activate)
    ##   nw <- set.vertex.attribute(nw,'status', 0, nodes.to.activate)
    ## }
    
    ## browser()
    
      nodes.to.activate <- which(is.na(nw%v%'age'))
    
      if (length(nodes.to.activate)>0) { ## if new nodes enter the network
      
      
      length.new.nodes <- length(nodes.to.activate)

      ## update relevant attributes here 
      nw <- activate.vertices(nw, onset=time,
                              terminus=Inf, v=nodes.to.activate)
      nw <- set.vertex.attribute(nw, "time.of.birth", time, nodes.to.activate)
      nw <- set.vertex.attribute(nw, "age", 0, nodes.to.activate)
      nw <- set.vertex.attribute(nw, "young", 1, nodes.to.activate)
      }
    ########################################################################
    ## Compile results 
      node.active <- is.active(nw, v=1:network.size(nw), at=time)
      nw.curr.wo.dead.nodes <- network.extract(nw, at=time)
    
      if (verbose) {
    
        cat("Number Alive (Total): ", network.size(nw.curr.wo.dead.nodes),
            "\n"
            )
      
        }
    
    
      popsize[time]  <- network.size(nw.curr.wo.dead.nodes) 

    
      ## Update edges coef, give feedback
      theta.form[1] <-  theta.form[1] + log(popsize[time-1]) - log(popsize[time])
      ## cat("Finished timestep ",time," of ",timesteps,".\n",sep="")
    
    return(nw)
        
      }