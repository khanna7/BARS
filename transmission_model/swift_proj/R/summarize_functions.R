## summarize functions

summarize_prev <- function(filename="counts.csv"){
  
  counts <- read.csv(filename)
  counts <- counts[-1,]
  prev <- (counts$vertex_count - counts$uninfected)/(counts$vertex_count)
  prev <- prev*100
  prev_last_ten_yrs <- tail(prev, 3650)
  mean_sd <- c(mean(prev_last_ten_yrs), sd(prev_last_ten_yrs))
  
  prev <- (counts$vertex_count_A - counts$uninfected_A)/(counts$vertex_count_A)
  prev <- prev*100
  prev_last_ten_yrs <- tail(prev, 3650)
  mean_sd_A <- c(mean(prev_last_ten_yrs), sd(prev_last_ten_yrs))
  
  prev <- (counts$vertex_count_B - counts$uninfected_B)/(counts$vertex_count_B)
  prev <- prev*100
  prev_last_ten_yrs <- tail(prev, 3650)
  mean_sd_B <- c(mean(prev_last_ten_yrs), sd(prev_last_ten_yrs))
  
  return(c(mean_sd, mean_sd_A, mean_sd_B))
}

inc_func <- function(counts, counts_yr_chunks, numerators, denominator) {
  #print(length(numerators))
  
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
      mean((x[2:nrow(x), numerators[1]] + x[2:nrow(x), numerators[2]] + x[2:nrow(x), numerators[3]]) / x[1:(nrow(x) - 1), denominator])
    )
  }
  
  mean_inc_ten_yrs <- tail(mean_inc, 10)
  mean_inc_ten_yrs <- unlist(mean_inc_ten_yrs)*365*100 
  result <- round(mean_inc_ten_yrs, 3)
  
}

calc_new_inf <- function(counts, counts_yr_chunks, numerators) {
  #print(length(numerators))
  
  if (length(numerators) == 1) {
    sum_new_inf <- lapply(counts_yr_chunks, function (x)
      sum(x[2:nrow(x), numerators])
    )
  } else if (length(numerators) == 2) {
    sum_new_inf <- lapply(counts_yr_chunks, function (x)
      sum((x[2:nrow(x), numerators[1]] + x[2:nrow(x), numerators[2]]))
    )
  } else if (length(numerators) == 3) {
    sum_new_inf <- lapply(counts_yr_chunks, function (x)
      sum((x[2:nrow(x), numerators[1]] + x[2:nrow(x), numerators[2]] + x[2:nrow(x), numerators[3]]))
    )
  }


  mean_new_inf <- mean(unlist(sum_new_inf))
  result <- round(mean_new_inf, 3)
  return(result)
}

summarize_inc <- function(filename="counts.csv"){
  
  counts <- read.csv(filename)
  counts <- counts[-1,]
  counts_yr_chunks <- split(counts,
                            ceiling(seq_along(counts$tick)/365))
  
  full_result <- inc_func(counts, counts_yr_chunks, c("infected_via_transmission"), "uninfected")
  a_result <- inc_func(counts, counts_yr_chunks, c("infected_via_transmission_A"), "uninfected_A")
  b_result <- inc_func(counts, counts_yr_chunks, c("infected_via_transmission_B"), "uninfected_B")
  
  ex_full_result <- inc_func(counts, counts_yr_chunks, c("infected_via_transmission", "infected_externally"), "uninfected")
  ex_A_result <- inc_func(counts, counts_yr_chunks, c("infected_via_transmission_A", "infected_external_A"), "uninfected_A")
  ex_B_result <- inc_func(counts, counts_yr_chunks, c("infected_via_transmission_B", "infected_external_B"), 
                              "uninfected_B")
  
  ee_full_result <- inc_func(counts, counts_yr_chunks, c("infected_via_transmission", "infected_externally", 
                                                         "infected_at_entry"), "uninfected")
  ee_A_result <- inc_func(counts, counts_yr_chunks, c("infected_via_transmission_A", "infected_external_A", 
                                                        "infected_at_entry_A"), "uninfected_A")
  ee_B_result <- inc_func(counts, counts_yr_chunks, c("infected_via_transmission_B", "infected_external_B", 
                                                          "infected_at_entry_B"), "uninfected_B")


  newinf_full_result <- calc_new_inf(counts, counts_yr_chunks, c("infected_via_transmission"))
  newinf_a_result <- calc_new_inf(counts, counts_yr_chunks, c("infected_via_transmission_A"))
  newinf_b_result <- calc_new_inf(counts, counts_yr_chunks, c("infected_via_transmission_B"))

  newinf_ex_full_result <- calc_new_inf(counts, counts_yr_chunks, c("infected_via_transmission", "infected_externally"))
  newinf_ex_a_result <- calc_new_inf(counts, counts_yr_chunks, c("infected_via_transmission_A", "infected_external_A"))
  newinf_ex_b_result <- calc_new_inf(counts, counts_yr_chunks, c("infected_via_transmission_B", "infected_external_B"))

  newinf_ee_full_result <- calc_new_inf(counts, counts_yr_chunks, c("infected_via_transmission", "infected_externally", "infected_at_entry"))
  newinf_ee_a_result <- calc_new_inf(counts, counts_yr_chunks, c("infected_via_transmission_A", "infected_external_A", "infected_at_entry_A"))
  newinf_ee_b_result <- calc_new_inf(counts, counts_yr_chunks, c("infected_via_transmission_B", "infected_external_B", "infected_at_entry_B"))
  
  
   return(c(full_result, a_result, b_result,
            ex_full_result, ex_A_result, ex_B_result,
            ee_full_result, ee_A_result, ee_B_result,
            newinf_full_result, newinf_a_result, newinf_b_result,
            newinf_ex_full_result, newinf_ex_a_result, newinf_ex_b_result,
            newinf_ee_full_result, newinf_ee_a_result, newinf_ee_b_result
            ))
}

summarize_pop_size <- function(filename="counts.csv"){
    counts <- read.csv(filename)
    return(c(tail(counts$vertex_count, 1), tail(counts$vertex_count_A, 1), tail(counts$vertex_count_B, 1)))
}
