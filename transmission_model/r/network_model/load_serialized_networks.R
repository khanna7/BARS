load.networks <- function(main.network.file, cas.network.file) {
  if (exists("nw") && exists("n_cas")) {
    nw <<- readRDS(main.network.file)
    n_cas <<- readRDS(cas.network.file)
  } else {
    stop("Error while replacing nw and n_cas with serialized networks: nw and n_cas do not already exist")
  }
}

