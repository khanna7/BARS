## summarize functions

summarize_prev <- function(filename="counts.csv"){
  
  counts <- read.csv(filename)
  counts <- counts[-1,]
  prev <- (counts$vertex_count - counts$uninfected)/(counts$vertex_count)
  prev <- prev*100
  prev_last_ten_yrs <- tail(prev, 3650)
  mean_sd <- c(mean(prev_last_ten_yrs), sd(prev_last_ten_yrs))
  return(mean_sd)
}

summarize_inc <- function(filename="counts.csv"){
  
  counts <- read.csv(filename)
  counts <- counts[-1,]
  counts_yr_chunks <- split(counts,
                            ceiling(seq_along(counts$tick)/365))
  
   mean_inc <- lapply(counts_yr_chunks, function (x)
    mean(x[2:nrow(x), "infected_via_transmission"] / x[1:(nrow(x) - 1), "uninfected"])
  )

   mean_inc_ten_yrs <- tail(mean_inc, 10)
   mean_inc_ten_yrs <- unlist(mean_inc_ten_yrs)*365*100 
  
   return(round(mean_inc_ten_yrs, 3))
}

summarize_pop_size <- function(filename="counts.csv"){
  
  counts <- read.csv(filename)

  return(tail(counts$vertex_count, 1))
}
