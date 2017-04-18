> network.update
function (nw, newmatrix, matrix.type = NULL, output = "network", 
    ignore.nattr = c("bipartite", "directed", "hyper", "loops", 
        "mnext", "multiple", "n"), ignore.vattr = c()) 
{
    unw <- network.initialize(n = network.size(nw), directed = is.directed(nw), 
        hyper = is.hyper(nw), loops = has.loops(nw), multiple = is.multiplex(nw), 
        bipartite = nw %n% "bipartite")
    for (a in setdiff(list.network.attributes(nw), ignore.nattr)) unw <- set.network.attribute(unw, 
        a, get.network.attribute(nw, a, unlist = FALSE))
    for (a in setdiff(list.vertex.attributes(nw), ignore.vattr)) unw <- set.vertex.attribute(unw, 
        a, get.vertex.attribute(nw, a, unlist = FALSE))
    if (is.null(matrix.type)) {
        warning("Don't leave matrix type to chance! Pass matrix.type to network.update!")
        matrix.type <- which.matrix.type(newmatrix)
        if (nrow(newmatrix) == 0) {
            matrix.type <- "edgelist"
        }
    }
    if (matrix.type == "adjacency" && all(newmatrix %in% c(0, 
        1))) {
        unw[, ] <- newmatrix
    }
    else if (matrix.type == "edgelist" && !is.null(newmatrix) && 
        nrow(newmatrix) > 0) {
        add.edges(unw, tail = newmatrix[, 1], head = newmatrix[, 
            2])
    }
    if (!is.null(output) && output == "edgelist.compressed") {
        unw <- as.edgelist.compressed(unw)
    }
    unw
}
<environment: namespace:ergm>

