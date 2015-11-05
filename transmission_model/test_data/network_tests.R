suppressMessages(library(network))

sn <- network.initialize(4) 
#set.network.attribute(triangle, 'vertex.pid', 'tergm_pid')
#add.vertices(triangle, 3)
add.edge(sn,1,2)    
add.edge(sn,2,3)
add.edge(sn,3,1)

set.vertex.attribute(sn, 'age', c(2, 12, 18, 100))

try.net.func <- function(nw) {
  # not sure how to achieve the effect of this in
  # c++
  class(nw) <- "network"
  nw <- add.edge(nw, 3, 4)
  nw
}