> network.initialize
function (n, directed = TRUE, hyper = FALSE, loops = FALSE, multiple = FALSE, 
    bipartite = FALSE) 
{
    n <- round(n)
    if (n < 0) 
        stop("Network objects cannot be of negative order.")
    g <- list()
    g$mel <- list()
    g$gal <- list()
    g$gal$n <- n
    g$gal$mnext <- 1
    g$gal$directed <- directed
    g$gal$hyper <- hyper
    g$gal$loops <- loops
    g$gal$multiple <- multiple
    g$gal$bipartite <- bipartite
    if (n > 0) {
        g$val <- replicate(n, list())
        g$iel <- replicate(n, vector(mode = "integer"))
        g$oel <- replicate(n, vector(mode = "integer"))
    }
    else {
        g$val <- vector(length = 0, mode = "list")
        g$iel <- vector(length = 0, mode = "list")
        g$oel <- vector(length = 0, mode = "list")
    }
    class(g) <- "network"
    if (n > 0) 
        g <- set.vertex.attribute(g, "na", rep(FALSE, n), 1:n)
    if (n > 0) 
        network.vertex.names(g) <- 1:n
    g
}
<environment: namespace:network>
