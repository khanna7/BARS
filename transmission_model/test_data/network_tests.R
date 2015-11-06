suppressMessages(library(network))

sn <- network.initialize(4) 
#set.network.attribute(triangle, 'vertex.pid', 'tergm_pid')
#add.vertices(triangle, 3)
add.edge(sn,1,2)    
add.edge(sn,2,3)
add.edge(sn,3,1)

set.vertex.attribute(sn, 'age', c(2, 12, 18, 100))

try.net.func <- function() {
  # delete edge between 1 and 4, create between
  # 2 and 4
  # !!! if vertex removed on C++ side, these indices may 
  # not match those above !!!
  data <- c(1, 1, 4, 0, 1, 2, 4, 1)
  m <- matrix(data, nrow=2, byrow=T)
  colnames(m) <- c("time", "tail", "head", "to")
  m
}