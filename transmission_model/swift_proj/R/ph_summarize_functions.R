require(data.table)

create_dt_counts <- function(filename, ranges){
  dt_counts <- fread(filename)
  # remove first row
  dt_counts <- dt_counts[!1]
  # max year periods, last one may not be a full year
  max_periods <- ceiling(dt_counts[,max(tick)]/365)
  # create ycat column for year categories
  cuts <- seq(from = 0, by = 365 , length.out = max_periods+1)
  dt_counts[,ycat := cut(tick,cuts)]
  levels(dt_counts$ycat) = seq(from = 1, to = max_periods)
  dt_counts
}

summarize_yearly_prev <- function(filename = "counts.csv", ranges){
  dt_counts <- create_dt_counts(filename,ranges)

  # Yearly prevalences 
  prev <- dt_counts[,lapply(c(mean,sd), function(x) x(100*(vertex_count - uninfected)/(vertex_count))),by = ycat]
  setnames(prev,c("V1","V2"),c("mean","sd"))
  
  # Yearly prevalences by age range
  prevs = list()
  for(i in 1:(length(ranges)/2)){
    x <- 2*i - 1
    v_counts <- sprintf("vertex_count_%s", seq(ranges[x], ranges[x+1]))
    sum_v_counts <- dt_counts[,rowSums(.SD),.SDcols=v_counts]
    
    uninfs <- sprintf("uninfected_%s", seq(ranges[x], ranges[x+1]))
    sum_unifs <- dt_counts[,rowSums(.SD),.SDcols=uninfs]
    
    dt_res <- dt_counts[,.(tick, ycat, vc = sum_v_counts, un = sum_unifs)][,lapply(c(mean,sd), function(x) x(100*(vc - un)/(vc))),by = ycat]
    setnames(dt_res,c("V1","V2"),c(paste0("mean_",i),paste0("sd_",i)))
    prevs[[i]] <- dt_res
  }
  dt_prevs <- prev
  for(i in seq_along(prevs)){
    dt_prevs <- merge(dt_prevs,prevs[[i]],by = c("ycat"))  
  }
  dt_prevs
}

summarize_yearly_inc <- function(filename="counts.csv", ranges){
  dt_counts <- create_dt_counts(filename,ranges)
  
  # Yearly incidence rates for ("infected_via_transmission")
  full_inc <- dt_counts[,lapply(c(mean), function(x) x(365*100*(infected_via_transmission[2:.N])/(uninfected[1:(.N-1)]))),by = ycat]
  setnames(full_inc,c("V1"),c("full_inc_mean"))
  
  # Yearly incidence rates for ("infected_via_transmission") by age range
  full_incs = list()
  for(i in 1:(length(ranges)/2)){
    x <- 2*i - 1
    numerator_cols <- sprintf("infected_via_transmission_%s", seq(ranges[x], ranges[x+1]))
    sum_nums <- dt_counts[,rowSums(.SD),.SDcols=numerator_cols]
    
    denominator_cols <- sprintf("uninfected_%s", seq(ranges[x], ranges[x+1]))
    sum_denoms <- dt_counts[,rowSums(.SD),.SDcols=denominator_cols]
    
    dt_res <- dt_counts[,.(tick, ycat, nums = sum_nums, denoms = sum_denoms)][,lapply(c(mean), function(x) x(365*100*(nums[2:.N])/(denoms[1:(.N-1)]))),by = ycat]
    setnames(dt_res,c("V1"),c(paste0("full_inc_mean_",i)))
    full_incs[[i]] <- dt_res
  }
  dt_full_incs <- full_inc
  for(i in seq_along(full_incs)){
    dt_full_incs <- merge(dt_full_incs,full_incs[[i]],by = c("ycat"))  
  }
  
  # Yearly incidence rates for ("infected_via_transmission","infected_externally")
  full_ex_inc <- dt_counts[,lapply(c(mean), function(x) x(365*100*((infected_via_transmission + infected_externally)[2:.N])/(uninfected[1:(.N-1)]))),by = ycat]
  setnames(full_ex_inc,c("V1"),c("full_ex_inc_mean"))
  
  # Yearly incidence rates for ("infected_via_transmission_X","infected_external_X") by age range
  full_ex_incs = list()
  for(i in 1:(length(ranges)/2)){
    x <- 2*i - 1
    numerator_cols <- sprintf("infected_via_transmission_%s", seq(ranges[x], ranges[x+1]))
    numerator_cols <- c(numerator_cols,sprintf("infected_external_%s", seq(ranges[x], ranges[x+1])))
    sum_nums <- dt_counts[,rowSums(.SD),.SDcols=numerator_cols]
    
    denominator_cols <- sprintf("uninfected_%s", seq(ranges[x], ranges[x+1]))
    sum_denoms <- dt_counts[,rowSums(.SD),.SDcols=denominator_cols]
    
    dt_res <- dt_counts[,.(tick, ycat, nums = sum_nums, denoms = sum_denoms)][,lapply(c(mean), function(x) x(365*100*(nums[2:.N])/(denoms[1:(.N-1)]))),by = ycat]
    setnames(dt_res,c("V1"),c(paste0("full_ex_inc_mean_",i)))
    full_ex_incs[[i]] <- dt_res
  }
  dt_full_ex_incs <- full_ex_inc
  for(i in seq_along(full_ex_incs)){
    dt_full_ex_incs <- merge(dt_full_ex_incs,full_ex_incs[[i]],by = c("ycat"))  
  }
  
  # Yearly incidence rates for ("infected_via_transmission","infected_externally","infected_at_entry")
  full_ee_inc <- dt_counts[,lapply(c(mean), function(x) x(365*100*((infected_via_transmission + infected_externally + infected_at_entry)[2:.N])/(uninfected[1:(.N-1)]))),by = ycat]
  setnames(full_ee_inc,c("V1"),c("full_ee_inc_mean"))
  
  # Yearly incidence rates for ("infected_via_transmission_X","infected_external_X","infected_at_entry_X") by age range
  full_ee_incs = list()
  for(i in 1:(length(ranges)/2)){
    x <- 2*i - 1
    numerator_cols <- sprintf("infected_via_transmission_%s", seq(ranges[x], ranges[x+1]))
    numerator_cols <- c(numerator_cols,sprintf("infected_external_%s", seq(ranges[x], ranges[x+1])))
    numerator_cols <- c(numerator_cols,sprintf("infected_at_entry_%s", seq(ranges[x], ranges[x+1])))
    sum_nums <- dt_counts[,rowSums(.SD),.SDcols=numerator_cols]
    
    denominator_cols <- sprintf("uninfected_%s", seq(ranges[x], ranges[x+1]))
    sum_denoms <- dt_counts[,rowSums(.SD),.SDcols=denominator_cols]
    
    dt_res <- dt_counts[,.(tick, ycat, nums = sum_nums, denoms = sum_denoms)][,lapply(c(mean), function(x) x(365*100*(nums[2:.N])/(denoms[1:(.N-1)]))),by = ycat]
    setnames(dt_res,c("V1"),c(paste0("full_ee_inc_mean_",i)))
    full_ee_incs[[i]] <- dt_res
  }
  dt_full_ee_incs <- full_ee_inc
  for(i in seq_along(full_ee_incs)){
    dt_full_ee_incs <- merge(dt_full_ee_incs,full_ee_incs[[i]],by = c("ycat"))  
  }
  
  # Yearly incidence for ("infected_via_transmission") 
  # Note: This uses the full year data, including the first day, unlike the rate calculations
  tot_inc <- dt_counts[,lapply(c(sum), function(x) x((infected_via_transmission))),by = ycat]
  setnames(tot_inc,c("V1"),c("tot_inc_sum"))
  
  # Yearly incidence rates for ("infected_via_transmission_X") by age range
  tot_incs = list()
  for(i in 1:(length(ranges)/2)){
    x <- 2*i - 1
    numerator_cols <- sprintf("infected_via_transmission_%s", seq(ranges[x], ranges[x+1]))
    sum_nums <- dt_counts[,rowSums(.SD),.SDcols=numerator_cols]
    
    dt_res <- dt_counts[,.(tick, ycat, nums = sum_nums)][,lapply(c(sum), function(x) x(nums)),by = ycat]
    setnames(dt_res,c("V1"),c(paste0("tot_inc_sum_",i)))
    tot_incs[[i]] <- dt_res
  }
  dt_tot_incs <- tot_inc
  for(i in seq_along(tot_incs)){
    dt_tot_incs <- merge(dt_tot_incs,tot_incs[[i]],by = c("ycat"))  
  } 
  
  # Yearly incidence for ("infected_via_transmission","infected_externally")
  # Note: This uses the full year data, including the first day, unlike the rate calculations
  tot_ex_inc <- dt_counts[,lapply(c(sum), function(x) x((infected_via_transmission + infected_externally))),by = ycat]
  setnames(tot_ex_inc,c("V1"),c("tot_ex_inc_sum"))
  
  # Yearly incidence rates for ("infected_via_transmission_X") by age range
  tot_ex_incs = list()
  for(i in 1:(length(ranges)/2)){
    x <- 2*i - 1
    numerator_cols <- sprintf("infected_via_transmission_%s", seq(ranges[x], ranges[x+1]))
    numerator_cols <- c(numerator_cols,sprintf("infected_external_%s", seq(ranges[x], ranges[x+1])))
    sum_nums <- dt_counts[,rowSums(.SD),.SDcols=numerator_cols]
    
    dt_res <- dt_counts[,.(tick, ycat, nums = sum_nums)][,lapply(c(sum), function(x) x(nums)),by = ycat]
    setnames(dt_res,c("V1"),c(paste0("tot_ex_inc_sum_",i)))
    tot_ex_incs[[i]] <- dt_res
  }
  dt_tot_ex_incs <- tot_ex_inc
  for(i in seq_along(tot_ex_incs)){
    dt_tot_ex_incs <- merge(dt_tot_ex_incs,tot_ex_incs[[i]],by = c("ycat"))  
  }
  
  # Yearly incidence for ("infected_via_transmission","infected_externally","infected_at_entry")
  # Note: This uses the full year data, including the first day, unlike the rate calculations
  tot_ee_inc <- dt_counts[,lapply(c(sum), function(x) x((infected_via_transmission + infected_externally + infected_at_entry))),by = ycat]
  setnames(tot_ee_inc,c("V1"),c("tot_ee_inc_sum"))
  
  # Yearly incidence rates for ("infected_via_transmission_X") by age range
  tot_ee_incs = list()
  for(i in 1:(length(ranges)/2)){
    x <- 2*i - 1
    numerator_cols <- sprintf("infected_via_transmission_%s", seq(ranges[x], ranges[x+1]))
    numerator_cols <- c(numerator_cols,sprintf("infected_external_%s", seq(ranges[x], ranges[x+1])))
    numerator_cols <- c(numerator_cols,sprintf("infected_at_entry_%s", seq(ranges[x], ranges[x+1])))
    sum_nums <- dt_counts[,rowSums(.SD),.SDcols=numerator_cols]
    
    dt_res <- dt_counts[,.(tick, ycat, nums = sum_nums)][,lapply(c(sum), function(x) x(nums)),by = ycat]
    setnames(dt_res,c("V1"),c(paste0("tot_ee_inc_sum_",i)))
    tot_ee_incs[[i]] <- dt_res
  }
  dt_tot_ee_incs <- tot_ee_inc
  for(i in seq_along(tot_ee_incs)){
    dt_tot_ee_incs <- merge(dt_tot_ee_incs,tot_ee_incs[[i]],by = c("ycat"))  
  }
  dt_full_incs[dt_full_ex_incs[dt_full_ee_incs[dt_tot_incs[dt_tot_ex_incs[dt_tot_ee_incs]]]]]
  
}

summarize_directory <- function(base_dir, ranges){
  exp_dirs <- dir(base_dir)
  prevs = list()
  incs = list()
  for(x in exp_dirs){
    filename <- paste0(base_dir,"/",x,"/counts.csv")
    if (file.exists(filename)){
      prevs[[x]] <- summarize_yearly_prev(filename, ranges)[,expid := x]    
      incs[[x]] <- summarize_yearly_inc(filename, ranges)[,expid := x]    
    }
  }
  dt_prevs <- rbindlist(prevs)
  dt_incs <- rbindlist(incs)
  list("prevs" = dt_prevs, "incs" = dt_incs)
}

## Final prevalences
summarize_prev_end <- function(filename="counts.csv", ranges){
  
  counts <- read.csv(filename)
  counts <- counts[-1,]
  
  prev <- (counts$vertex_count - counts$uninfected)/(counts$vertex_count)
  prev <- prev*100
  prev_last <- tail(prev, 1)
  # mean_sd <- c(mean(prev_last), sd(prev_last))
  
  prev_lasts <- sapply(seq(1, length(ranges) - 1, by=2), function(x) {
    v_counts <- sprintf("vertex_count_%s", seq(ranges[x], ranges[x+1]))
    sum_v_counts <- rowSums(counts[v_counts])
    
    uninfs <- sprintf("uninfected_%s", seq(ranges[x], ranges[x+1]))
    sum_unifs <- rowSums(counts[uninfs])
    
    prev <- (sum_v_counts - sum_unifs) / (sum_v_counts)
    prev <- prev*100
    prev_last <- tail(prev, 1)
    # mean_sd_A <- c(mean(prev_last), sd(prev_last))
    
  })
  
  return(unlist(c(prev_last, prev_lasts)))
}