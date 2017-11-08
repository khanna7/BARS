## debug summarize function

# Clear slate ------

rm(list=ls())

# Set wkg. directory ------

setwd("/project/khanna7/Projects/midway2/BARS/transmission_model/swift_proj/experiments/data-analysis/r")


# Data ------

counts <- read.csv("../../agg-incidence-at-30/instance_1/output/counts.csv")


# Summarize functions (old version)

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



# Test old functions ------

summarize_prev("../../agg-incidence-at-30/instance_1/output/counts.csv")
summarize_inc("../../agg-incidence-at-30/instance_1/output/counts.csv")
summarize_pop_size("../../agg-incidence-at-30/instance_1/output/counts.csv")


# Write new incidence function to include numerators ------

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
    mean(x[2:nrow(x), "infected_via_transmission_u26"] / x[1:(nrow(x) - 1), "uninfected_u26"])
  )
  mean_inc_ten_yrs <- tail(mean_inc, 10)
  mean_inc_ten_yrs <- unlist(mean_inc_ten_yrs)*365*100 
  u26_result <- round(mean_inc_ten_yrs, 3)
  
  num_new_inf_explicit_u26 <- lapply(counts_yr_chunks, function (x) #explicit
    sum(x[2:nrow(x), "infected_via_transmission_u26"]) 
  )
  mean_num_new_inf_explicit_u26 <- mean(unlist(num_new_inf_explicit_u26))
  
  num_new_inf_explicit_ext_u26 <- lapply(counts_yr_chunks, function (x) #explicit+ext
    sum(c(x[2:nrow(x), "infected_via_transmission_u26"],
          x[2:nrow(x), "infected_external_u26"])) 
  )
  mean_num_new_inf_explicit_ext_u26 <- mean(unlist(num_new_inf_explicit_ext_u26))
  
  num_new_inf_explicit_ext_ent_u26 <- lapply(counts_yr_chunks, function (x) #explicit+ext+entry
    sum(c(x[2:nrow(x), "infected_via_transmission_u26"],
          x[2:nrow(x), "infected_external_u26"],
          x[2:nrow(x), "infected_at_entry_u26"])) 
  )
  mean_num_new_inf_explicit_ext_ent_u26 <- mean(unlist(num_new_inf_explicit_ext_ent_u26))
  
  # gte26
  mean_inc <- lapply(counts_yr_chunks, function (x)
    mean(x[2:nrow(x), "infected_via_transmission_gte26"] / x[1:(nrow(x) - 1), "uninfected_gte26"])
  )
  mean_inc_ten_yrs <- tail(mean_inc, 10)
  mean_inc_ten_yrs <- unlist(mean_inc_ten_yrs)*365*100 
  gte26_result <- round(mean_inc_ten_yrs, 3)
  
  num_new_inf_explicit_gte26 <- lapply(counts_yr_chunks, function (x) #explicit
    sum(x[2:nrow(x), "infected_via_transmission_gte26"]) 
  )
  mean_num_new_inf_explicit_gte26 <- mean(unlist(num_new_inf_explicit_gte26))
  
  num_new_inf_explicit_ext_gte26 <- lapply(counts_yr_chunks, function (x) #explicit+ext
    sum(c(x[2:nrow(x), "infected_via_transmission_gte26"],
          x[2:nrow(x), "infected_external_gte26"])) 
  )
  mean_num_new_inf_explicit_ext_gte26 <- mean(unlist(num_new_inf_explicit_ext_gte26))
  
  num_new_inf_explicit_ext_ent_gte26 <- lapply(counts_yr_chunks, function (x) #explicit+ext+entry
    sum(c(x[2:nrow(x), "infected_via_transmission_gte26"],
          x[2:nrow(x), "infected_external_gte26"],
          x[2:nrow(x), "infected_at_entry_gte26"])) 
  )
  mean_num_new_inf_explicit_ext_ent_gte26 <- mean(unlist(num_new_inf_explicit_ext_ent_gte26))
  
  
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

summarize_inc_new("../../agg-incidence-at-30/instance_1/output/counts.csv")

