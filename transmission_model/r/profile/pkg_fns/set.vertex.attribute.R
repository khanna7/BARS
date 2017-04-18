> set.vertex.attribute
function (x, attrname, value, v = seq_len(network.size(x))) 
{
    if (!is.network(x)) 
        stop("set.vertex.attribute requires an argument of class network.")
    if (any((v > network.size(x)) | (v < 1))) 
        stop("Vertex ID does not correspond to actual vertex in set.vertex.attribute.\n")
    xn <- substitute(x)
    if (length(attrname) == 1) {
        if (!is.list(value)) {
            if (!is.vector(value)) {
                stop("Inappropriate value given in set.vertex.attribute.\n")
            }
            else {
                value <- as.list(rep(value, length = length(v)))
            }
        }
        else {
            if (length(value) != length(v)) {
                value <- rep(value, length = length(v))
            }
        }
        x <- .Call(setVertexAttribute_R, x, attrname, value, 
            v)
    }
    else {
        if (length(value) != length(attrname)) {
            stop("the 'value' attribute must have an element corresponding to each attribute name in 'attrnames' in set.vertex.attribute")
        }
        if (!is.list(value)) {
            if (!is.vector(value)) {
                stop("Inappropriate value given in set.vertex.attribute.\n")
            }
            else {
                value <- lapply(1:length(value), function(n) {
                  if (length(value[n]) < length(v)) {
                    return(as.list(rep(value[n], length = length(v))))
                  }
                  else {
                    return(as.list(value[n]))
                  }
                })
            }
        }
        else {
            value <- lapply(1:length(value), function(n) {
                if (length(value[[n]]) < length(v)) {
                  return(as.list(rep(value[[n]], length = length(v))))
                }
                else {
                  return(as.list(value[[n]]))
                }
            })
        }
        x <- .Call(setVertexAttributes_R, x, attrname, value, 
            v)
    }
    if (.validLHS(xn, parent.frame())) {
        on.exit(eval.parent(call("<-", xn, x)))
    }
    invisible(x)
}
<environment: namespace:network>
