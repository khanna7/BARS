suppressMessages(library(network))
suppressMessages(library(networkDynamic))

triangle <- network.initialize(3) 
add.edge(triangle,1,2)    
add.edge(triangle,2,3)
add.edge(triangle,3,1)

set.vertex.attribute(triangle, 'age', c(2, 12, 18))

# all active at 1
activate.edges(triangle,at=1)

# edge(1,2) from 2 up to 3
activate.edges(triangle, onset=2, terminus=3, 
                e=get.edgeIDs(triangle, v=1, alter=2))
