## summarize functions

summarize_prev <- function(filename="counts.csv"){
  
  counts <- read.csv(filename)
  counts <- counts[-1,]
  prev <- (counts$vertex_count - counts$uninfected)/(counts$vertex_count)
  prev <- prev*100
  prev_last_ten_yrs <- tail(prev, 3650)
  mean_sd <- c(mean(prev_last_ten_yrs), sd(prev_last_ten_yrs))
  
  prev <- (counts$vertex_count_u26 - counts$uninfected_u26)/(counts$vertex_count_u26)
  prev <- prev*100
  prev_last_ten_yrs <- tail(prev, 3650)
  mean_sd_u26 <- c(mean(prev_last_ten_yrs), sd(prev_last_ten_yrs))
  
  prev <- (counts$vertex_count_gte26 - counts$uninfected_gte26)/(counts$vertex_count_gte26)
  prev <- prev*100
  prev_last_ten_yrs <- tail(prev, 3650)
  mean_sd_gte26 <- c(mean(prev_last_ten_yrs), sd(prev_last_ten_yrs))
  
  return(c(mean_sd, mean_sd_u26, mean_sd_gte26))
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
   full_result <- round(mean_inc_ten_yrs, 3)
   
   # u26
   mean_inc <- lapply(counts_yr_chunks, function (x)
     mean(x[2:nrow(x), "infected_via_transmission_u26"] / x[1:(nrow(x) - 1), "uninfected_u26"])
   )
   mean_inc_ten_yrs <- tail(mean_inc, 10)
   mean_inc_ten_yrs <- unlist(mean_inc_ten_yrs)*365*100 
   u26_result <- round(mean_inc_ten_yrs, 3)
   
   # gte26
   mean_inc <- lapply(counts_yr_chunks, function (x)
     mean(x[2:nrow(x), "infected_via_transmission_gte26"] / x[1:(nrow(x) - 1), "uninfected_gte26"])
   )
   mean_inc_ten_yrs <- tail(mean_inc, 10)
   mean_inc_ten_yrs <- unlist(mean_inc_ten_yrs)*365*100 
   gte26_result <- round(mean_inc_ten_yrs, 3)
  
   return(c(full_result, u26_result, gte26_result))
}

summarize_pop_size <- function(filename="counts.csv"){
  
  counts <- read.csv(filename)
  return(c(tail(counts$vertex_count, 1), tail(counts$vertex_count_u26, 1), tail(counts$vertex_count_gte26, 1)))
}
