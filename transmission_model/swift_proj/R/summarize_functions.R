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

inc_func <- function(counts, counts_yr_chunks, numerators, denominator) {
  print(length(numerators))
  
  if (length(numerators) == 1) {
    mean_inc <- lapply(counts_yr_chunks, function (x)
      mean(x[2:nrow(x), numerators] / x[1:(nrow(x) - 1), denominator])
    )
  } else if (length(numerators) == 2) {
    mean_inc <- lapply(counts_yr_chunks, function (x)
      mean((x[2:nrow(x), numerators[1]] + x[2:nrow(x), numerators[2]]) / x[1:(nrow(x) - 1), denominator])
    )
  } else if (length(numerators) == 3) {
    mean_inc <- lapply(counts_yr_chunks, function (x)
      mean((x[2:nrow(x), numerators[1]] + x[2:nrow(x), numerators[2]] + + x[2:nrow(x), numerators[3]]) / x[1:(nrow(x) - 1), denominator])
    )
  }
  
  mean_inc_ten_yrs <- tail(mean_inc, 10)
  mean_inc_ten_yrs <- unlist(mean_inc_ten_yrs)*365*100 
  result <- round(mean_inc_ten_yrs, 3)
  return(result)
}

summarize_inc <- function(filename="counts.csv"){
  
  counts <- read.csv(filename)
  counts <- counts[-1,]
  counts_yr_chunks <- split(counts,
                            ceiling(seq_along(counts$tick)/365))
  
  full_result <- inc_func(counts, counts_yr_chunks, c("infected_via_transmission"), "uninfected")
  u26_result <- inc_func(counts, counts_yr_chunks, c("infected_via_transmission_u26"), "uninfected_u26")
  gte26_result <- inc_func(counts, counts_yr_chunks, c("infected_via_transmission_gte26"), "uninfected_gte26")
  
  ex_full_result <- inc_func(counts, counts_yr_chunks, c("infected_via_transmission", "infected_externally"), "uninfected")
  ex_u26_result <- inc_func(counts, counts_yr_chunks, c("infected_via_transmission_u26", "infected_external_u26"), "uninfected_u26")
  ex_gte26_result <- inc_func(counts, counts_yr_chunks, c("infected_via_transmission_gte26", "infected_external_gte26"), 
                              "uninfected_gte26")
  
  ee_full_result <- inc_func(counts, counts_yr_chunks, c("infected_via_transmission", "infected_externally", 
                                                         "infected_at_entry"), "uninfected")
  ee_u26_result <- inc_func(counts, counts_yr_chunks, c("infected_via_transmission_u26", "infected_external_u26", 
                                                        "infected_at_entry_u26"), "uninfected_u26")
  ee_gte26_result <- inc_func(counts, counts_yr_chunks, c("infected_via_transmission_gte26", "infected_external_gte26", 
                                                          "infected_at_entry_gte26"), "uninfected_gte26")

   return(c(full_result, u26_result, gte26_result, ex_full_result, ex_u26_result, ex_gte26_result, 
            ee_full_result, ee_u26_result, ee_gte26_result))
}

summarize_pop_size <- function(filename="counts.csv"){
  
  counts <- read.csv(filename)
  return(c(tail(counts$vertex_count, 1), tail(counts$vertex_count_u26, 1), tail(counts$vertex_count_gte26, 1)))
}
