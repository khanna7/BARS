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
  return(result)
}

summarize_inc_new <- function(filename="counts.csv"){
  
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
  
  num_new_inf_explicit <- lapply(counts_yr_chunks, function (x) #explicit
    sum(x[2:nrow(x), "infected_via_transmission"]) 
  )
  mean_num_new_inf_explicit <- mean(unlist(num_new_inf_explicit)) #mean num. new inf. (per year)
  
  num_new_inf_explicit_ext <- lapply(counts_yr_chunks, function (x) #explicit+ext
    sum(c(x[2:nrow(x), "infected_via_transmission"],
          x[2:nrow(x), "infected_externally"])) 
  )
  mean_num_new_inf_explicit_ext <- mean(unlist(num_new_inf_explicit_ext))
  
  num_new_inf_explicit_ext_ent <- lapply(counts_yr_chunks, function (x) #explicit+ext+entry
    sum(c(x[2:nrow(x), "infected_via_transmission"],
          x[2:nrow(x), "infected_externally"],
          x[2:nrow(x), "infected_at_entry"])) 
  )
  mean_num_new_inf_explicit_ext_ent <- mean(unlist(num_new_inf_explicit_ext_ent))
  
  # u26
  mean_inc <- lapply(counts_yr_chunks, function (x)
    mean(x[2:nrow(x), "infected_via_transmission_A"] / x[1:(nrow(x) - 1), "uninfected_A"])
  )
  mean_inc_ten_yrs <- tail(mean_inc, 10)
  mean_inc_ten_yrs <- unlist(mean_inc_ten_yrs)*365*100 
  u26_result <- round(mean_inc_ten_yrs, 3)
  
  num_new_inf_explicit_u26 <- lapply(counts_yr_chunks, function (x) #explicit
    sum(x[2:nrow(x), "infected_via_transmission_A"]) 
  )
  mean_num_new_inf_explicit_u26 <- mean(unlist(num_new_inf_explicit_u26))
  
  num_new_inf_explicit_ext_u26 <- lapply(counts_yr_chunks, function (x) #explicit+ext
    sum(c(x[2:nrow(x), "infected_via_transmission_A"],
          x[2:nrow(x), "infected_external_A"])) 
  )
  mean_num_new_inf_explicit_ext_u26 <- mean(unlist(num_new_inf_explicit_ext_u26))
  
  num_new_inf_explicit_ext_ent_u26 <- lapply(counts_yr_chunks, function (x) #explicit+ext+entry
    sum(c(x[2:nrow(x), "infected_via_transmission_A"],
          x[2:nrow(x), "infected_external_A"],
          x[2:nrow(x), "infected_at_entry_A"])) 
  )
  mean_num_new_inf_explicit_ext_ent_u26 <- mean(unlist(num_new_inf_explicit_ext_ent_u26))
  
  # gte26
  mean_inc <- lapply(counts_yr_chunks, function (x)
    mean(x[2:nrow(x), "infected_via_transmission_B"] / x[1:(nrow(x) - 1), "uninfected_B"])
  )
  mean_inc_ten_yrs <- tail(mean_inc, 10)
  mean_inc_ten_yrs <- unlist(mean_inc_ten_yrs)*365*100 
  gte26_result <- round(mean_inc_ten_yrs, 3)
  
  num_new_inf_explicit_gte26 <- lapply(counts_yr_chunks, function (x) #explicit
    sum(x[2:nrow(x), "infected_via_transmission_B"]) 
  )
  mean_num_new_inf_explicit_gte26 <- mean(unlist(num_new_inf_explicit_gte26))
  
  num_new_inf_explicit_ext_gte26 <- lapply(counts_yr_chunks, function (x) #explicit+ext
    sum(c(x[2:nrow(x), "infected_via_transmission_B"],
          x[2:nrow(x), "infected_external_B"])) 
  )
  mean_num_new_inf_explicit_ext_gte26 <- mean(unlist(num_new_inf_explicit_ext_gte26))
  
  num_new_inf_explicit_ext_ent_gte26 <- lapply(counts_yr_chunks, function (x) #explicit+ext+entry
    sum(c(x[2:nrow(x), "infected_via_transmission_B"],
          x[2:nrow(x), "infected_external_B"],
          x[2:nrow(x), "infected_at_entry_B"])) 
  )
  mean_num_new_inf_explicit_ext_ent_gte26 <- mean(unlist(num_new_inf_explicit_ext_ent_gte26))
  
  #mean number of infections below is average of the number of new infections per year
  # across the length of the simulation
  
  return(c(full_result, 
           mean_num_new_inf_explicit, #mean number of new infections annually
           mean_num_new_inf_explicit_ext,
           mean_num_new_inf_explicit_ext_ent,
           u26_result, 
           mean_num_new_inf_explicit_u26,
           mean_num_new_inf_explicit_ext_u26,
           mean_num_new_inf_explicit_ext_ent_u26,
           gte26_result,
           mean_num_new_inf_explicit_gte26,
           mean_num_new_inf_explicit_ext_gte26,
           mean_num_new_inf_explicit_ext_ent_gte26
  ))
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

   return(c(full_result, a_result, b_result, ex_full_result, ex_A_result, ex_B_result,
            ee_full_result, ee_A_result, ee_B_result))
}

summarize_pop_size <- function(filename="counts.csv"){
  
  counts <- read.csv(filename)
  return(c(tail(counts$vertex_count, 1), tail(counts$vertex_count_A, 1), tail(counts$vertex_count_B, 1)))
}
