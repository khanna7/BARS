rm(list=ls())

suppressMessages(library(ergm))
suppressMessages(library(tergm))
suppressMessages(library(network))
suppressMessages(library(networkDynamic))
suppressMessages(library(profvis))
suppressMessages(library(purrr))

#suppressMessages(library(parallel))
#load(file="../network_model/cas_net.RData")
#net <- readRDS("network_for_profiling.rds")
load(file="./for_profiling.RData")
is.new <- F
my.network.update<-function(nw, newmatrix, matrix.type=NULL, output="network", ignore.nattr=c("bipartite","directed","hyper","loops","mnext","multiple","n"), ignore.vattr=c()){
  is.new <<- T
  unw <- network.initialize(n=network.size(nw), directed = is.directed(nw), hyper = is.hyper(nw), loops = has.loops(nw),
                            multiple = is.multiplex(nw), bipartite = nw %n% "bipartite")
  for(a in setdiff(list.network.attributes(nw),ignore.nattr)) unw <- set.network.attribute(unw, a, get.network.attribute(nw, a, unlist=FALSE))
  # for(a in setdiff(list.vertex.attributes(nw),ignore.vattr)) unw <- set.vertex.attribute(unw, a, get.vertex.attribute(nw, a, unlist=FALSE))
  # new code
  #attribs <- setdiff(list.vertex.attributes(nw),ignore.vattr)
  attribs <- list.vertex.attributes(nw)
  #print(toString(attribs))
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

nw_simulate <-
  
  function(net, time) {
    class(net) <- "network"
    
    changes <- simulate(net,
        
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
  }

time1 <- 1
time2 <- time1+0

p <-profvis({
  for (time in time1:time2){
    ch <- nw_simulate(cpp_net, time)
    
  }
})
print(is.new)
print(p)