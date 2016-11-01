rm(list=ls())



suppressMessages(library(ergm))

suppressMessages(library(tergm))

suppressMessages(library(network))

suppressMessages(library(networkDynamic))
suppressMessages(library(purrr))

my.network.update<-function(nw, newmatrix, matrix.type=NULL, output="network", ignore.nattr=c("bipartite","directed","hyper","loops","mnext","multiple","n"), ignore.vattr=c()) {
  unw <- network.initialize(n=network.size(nw), directed = is.directed(nw), hyper = is.hyper(nw), loops = has.loops(nw),
                            multiple = is.multiplex(nw), bipartite = nw %n% "bipartite")
  for(a in setdiff(list.network.attributes(nw),ignore.nattr)) unw <- set.network.attribute(unw, a, get.network.attribute(nw, a, unlist=FALSE))
  # for(a in setdiff(list.vertex.attributes(nw),ignore.vattr)) unw <- set.vertex.attribute(unw, a, get.vertex.attribute(nw, a, unlist=FALSE))
  # new code
  attribs <- setdiff(list.vertex.attributes(nw),ignore.vattr)

  # va is a list of length 28, with each element being a list
  va <- lapply(nw$val, "[", attribs) %>% transpose()
  unw <- set.vertex.attribute(unw, attribs, va)
  ### end

  if(is.null(matrix.type)){
    warning("Don't leave matrix type to chance! Pass matrix.type to network.update!")
    matrix.type <- which.matrix.type(newmatrix)
    if(nrow(newmatrix)==0){matrix.type <- "edgelist"}
  }

  if(matrix.type=="adjacency" && all(newmatrix%in%c(0,1))){
    unw[,] <- newmatrix
  }else if(matrix.type=="edgelist" && !is.null(newmatrix) && nrow(newmatrix)>0){
    add.edges(unw,tail=newmatrix[,1],head=newmatrix[,2])
  }
  if(!is.null(output) && output=="edgelist.compressed")
  {unw <- as.edgelist.compressed(unw)}
  unw
}


unlockBinding("network.update", getNamespace("ergm"))
assign("network.update", my.network.update, getNamespace("ergm"))
lockBinding("network.update",  getNamespace("ergm"))

#suppressMessages(library(parallel))

load(file="../r/network_model/cas_net.RData")

#np <- detectCores()

nw <- fit$network

formation <- fit$formula

dissolution <- dissolution

nw_simulate <-

  function(net_for_sim, time) {

    #print("sim start")

    

    #print(formation)

    #print(dissolution)

    #print(theta.form)

    #print(theta.diss)

    #print(constraints)

    

    class(net_for_sim) <- "network"
    
    #cpp_net <<- net_for_sim
    #save.image(file="./for_profiling.RData")

    changes <- simulate(net_for_sim,

                   formation = formation, 

                   dissolution = dissolution,

                   coef.form = theta.form, 

                   coef.diss = theta.diss,

                   constraints = constraints,

                   output = "changes",

                   time.start = time,

                   control = control.simulate.network(),

                   time.slices = 1

    )
    changes
    #print(network.size(nw))

    #print("sim end")

  }

n_cas <- cas_fit$network

cas_formation <- cas_fit$formula

cas_dissolution <- dissolution_cas

n_cas_simulate <-

  function(cas_net, time) {

    class(cas_net) <- "network"

    changes <- simulate(cas_net,

                        formation = cas_formation, 

                        dissolution = cas_dissolution,

                        coef.form = theta.form_cas, 

                        coef.diss = theta.diss_cas,

                        constraints = constraints_cas,

                        output = "changes",

                        time.start = time,

                        time.slices = 1,

                        control = control.simulate.network()
    )

    changes

  }


nw_save <- function(net, fname, tick) {
	class(net) <- "network"
	set.network.attribute(net, "tick", tick)
	saveRDS(net, file=fname)
}



# holds any code that I want to call from 

# C++ to make sure changes there are being

# reflected here.

nw_debug <- function() {

  #print(nw.active$val[[1]]$age)

  print(nw$val[[1]]$age)

  #nw.active <<- network.extract(nw,at = 2, retain.all.vertices = T)

  age <- nw$val[[1]]$age

  nw <<- set.vertex.attribute(nw, "age", age + 1, v=c(1))

  #print(nw.active$val[[1]]$age)

} 
