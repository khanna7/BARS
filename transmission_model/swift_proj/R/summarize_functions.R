## summarize functions

summarize_prev <- function(filename="counts.csv", ranges){
  
  counts <- read.csv(filename)
  counts <- counts[-1,]
  
  prev <- (counts$vertex_count - counts$uninfected)/(counts$vertex_count)
  prev <- prev*100
  prev_last_ten_yrs <- tail(prev, 3650)
  mean_sd <- c(mean(prev_last_ten_yrs), sd(prev_last_ten_yrs))
  
  means <- sapply(seq(1, length(ranges) - 1, by=2), function(x) {
    v_counts <- sprintf("vertex_count_%s", seq(ranges[x], ranges[x+1]))
    sum_v_counts <- rowSums(counts[v_counts])
   
    uninfs <- sprintf("uninfected_%s", seq(ranges[x], ranges[x+1]))
    sum_unifs <- rowSums(counts[uninfs])
    
    prev <- (sum_v_counts - sum_unifs) / (sum_v_counts)
    prev <- prev*100
    prev_last_ten_yrs <- tail(prev, 3650)
    mean_sd_A <- c(mean(prev_last_ten_yrs), sd(prev_last_ten_yrs))
    
  })
  
  return(unlist(c(mean_sd, means)))
}

calc_denom <- function(x, denominator) {
  if (length(denominator) == 1) {
    denom <- x[1:(nrow(x) - 1), denominator]
  } else {
    denom <- Reduce("+", (x[1:(nrow(x) - 1), denominator]))
  }
}

inc_func <- function(counts_yr_chunks, numerators, denominator) {
  
  if (length(numerators) == 1) {
    mean_inc <- lapply(counts_yr_chunks, function (x) {
      denom <- calc_denom(x, denominator)
      mean(x[2:nrow(x), numerators] / denom)
    })
  } else  {
    mean_inc <- lapply(counts_yr_chunks, function (x) {
      denom <- calc_denom(x, denominator)
      z <- Reduce("+", (x[2:nrow(x), numerators]))
      mean(z / denom)
    })
  } 
  
  mean_inc_ten_yrs <- tail(mean_inc, 10)
  mean_inc_ten_yrs <- unlist(mean_inc_ten_yrs)*365*100 
  result <- round(mean_inc_ten_yrs, 3)
  
}

calc_new_inf <- function(counts_yr_chunks, numerators) {
  #print(length(numerators))
  
  if (length(numerators) == 1) {
    sum_new_inf <- lapply(counts_yr_chunks, function (x)
      sum(x[2:nrow(x), numerators])
    )
  } else {
    sum_new_inf <- lapply(counts_yr_chunks, function (x)
      sum(Reduce("+", (x[2:nrow(x), numerators])))
    )
  }
    
  mean_new_inf <- mean(unlist(sum_new_inf))
  result <- round(mean_new_inf, 3)
  return(result)
}

summarize_inc <- function(filename="counts.csv", ranges){
  
  counts <- read.csv(filename)
  counts <- counts[-1,]
  counts_yr_chunks <- split(counts,
                            ceiling(seq_along(counts$tick)/365))
  
  full_result <- inc_func(counts_yr_chunks, c("infected_via_transmission"), c("uninfected"))
  trans_range_result <- sapply(seq(1, length(ranges) - 1, by=2), function(x) {
    numerators <- sprintf("infected_via_transmission_%s", seq(ranges[x], ranges[x+1]))
    denominator <- sprintf("uninfected_%s", seq(ranges[x], ranges[x+1]))
    inc_func(counts_yr_chunks, numerators, denominator)
  })
  
  ex_full_result <- inc_func(counts_yr_chunks, c("infected_via_transmission", "infected_externally"), c("uninfected"))
  ex_trans_range_result <- sapply(seq(1, length(ranges) - 1, by=2), function(x) {
    # unlist(c(sprintf("vertex_count_%s", seq(1:4)), sprintf("foo_%s", seq(1:4))))
    numerators <- unlist(c(sprintf("infected_via_transmission_%s", seq(ranges[x], ranges[x+1])), 
                           sprintf("infected_external_%s",  seq(ranges[x], ranges[x+1]))))
    denominator <- sprintf("uninfected_%s", seq(ranges[x], ranges[x+1]))
    inc_func(counts_yr_chunks, numerators, denominator)
  })
  
  ee_full_result <- inc_func(counts_yr_chunks, c("infected_via_transmission", "infected_externally", 
                                                         "infected_at_entry"), c("uninfected"))
  ee_range_result <- sapply(seq(1, length(ranges) - 1, by=2), function(x) {
    # unlist(c(sprintf("vertex_count_%s", seq(1:4)), sprintf("foo_%s", seq(1:4))))
    numerators <- unlist(c(sprintf("infected_via_transmission_%s", seq(ranges[x], ranges[x+1])), 
                           sprintf("infected_external_%s",  seq(ranges[x], ranges[x+1])),
                           sprintf("infected_at_entry_%s", seq(ranges[x], ranges[x+1]))))
    denominator <- sprintf("uninfected_%s", seq(ranges[x], ranges[x+1]))
    inc_func(counts_yr_chunks, numerators, denominator)
  })
  
  newinf_full_result <- calc_new_inf(counts_yr_chunks, c("infected_via_transmission"))
  newinf_range_result <-  sapply(seq(1, length(ranges) - 1, by=2), function(x) {
    numerators <- sprintf("infected_via_transmission_%s", seq(ranges[x], ranges[x+1]))
    calc_new_inf(counts_yr_chunks, numerators)
  })
  
  newinf_ex_full_result <- calc_new_inf(counts_yr_chunks, c("infected_via_transmission", "infected_externally"))
  newinf_ex_range_result <-  sapply(seq(1, length(ranges) - 1, by=2), function(x) {
    numerators <- unlist(c(sprintf("infected_via_transmission_%s", seq(ranges[x], ranges[x+1])), 
                           sprintf("infected_external_%s",  seq(ranges[x], ranges[x+1]))))
    calc_new_inf(counts_yr_chunks, numerators)
  })
  
  newinf_ee_full_result <- calc_new_inf(counts_yr_chunks, c("infected_via_transmission", "infected_externally", "infected_at_entry"))
  newinf_ee_range_result <-  sapply(seq(1, length(ranges) - 1, by=2), function(x) {
    numerators <- unlist(c(sprintf("infected_via_transmission_%s", seq(ranges[x], ranges[x+1])), 
                           sprintf("infected_external_%s",  seq(ranges[x], ranges[x+1])),
                           sprintf("infected_at_entry_%s", seq(ranges[x], ranges[x+1]))))
    calc_new_inf(counts_yr_chunks, numerators)
  })
  
  return (unlist(c(full_result, trans_range_result, ex_full_result, ex_trans_range_result, ee_full_result, ee_range_result,
                   newinf_full_result, newinf_range_result, newinf_ex_full_result, newinf_ex_range_result,
                   newinf_ee_full_result, newinf_ee_range_result)))
}

summarize_pop_size <- function(filename="counts.csv", ranges){
    counts <- read.csv(filename)
    
    pop_sizes <- sapply(seq(1, length(ranges) - 1, by=2), function(x) {
      v_counts <- sprintf("vertex_count_%s", seq(ranges[x], ranges[x+1]))
      sum_v_counts <- rowSums(counts[v_counts])
      tail(sum_v_counts, 1)
    })
    
    return(unlist(c(tail(counts$vertex_count, 1), pop_sizes)))
}

summarize_vl_supp <- function(filename="counts.csv"){
  counts <- read.csv(filename)
  vl_supp_per_pos <- tail(counts$vl_supp_per_positives,1)
  vl_supp_diag <- tail(counts$vl_supp_per_diagnosed,1)
  return(c(vl_supp_per_pos, vl_supp_diag))
  
}

