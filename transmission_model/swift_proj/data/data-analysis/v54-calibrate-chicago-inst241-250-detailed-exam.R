rm(list=ls())

options(tibble.print_max = 100, tibble.print_min = 100)
options(tibble.width = Inf)

# Libraries ----

library(dplyr)
library(ggplot2)

# Data ----

full_res <- read.csv("../data/v54-calibrate-chicago-inst241-250-detailed-exam-final_results.csv", header=FALSE) #final results
loc <- "../../v54-calibrate-chicago-inst241-250-detailed-exam"

in1 <- read.table(paste0(loc,"/instance_1/output/parameters.txt"))
param_list <- read.csv("../../../data/calibrate-chicago-inst241-250-detailed-exam.csv")
upf <- read.table(paste0(loc, "/calibrate-chicago-inst241-250-detailed-exam.txt"), sep=",")

# Generate Response Dataframe ‚----

all_responses <- full_res[,c(1, 2, 4, 6, seq(17, 97, by=10), 98:111)]
names(all_responses) <- c("instance", 
                          "prevalence", "prev20to30", "prevge30",
                          "inc_transm_full", "inc_transm_20to30", "inc_transm_ge30",
                          "inc_transm_ext_full", "inc_transm_ext_20to30", "inc_transm_ext_ge30",
                          "inc_transm_ext_ent_full", "inc_transm_ext_ent_20to30", "inc_transm_ext_ent_ge30",
                          "newinf_transm_full", "newinf_transm_20to30", "newinf_transm_ge30",
                          "newinf_transm_ext_full", "newinf_transm_ext_20to30", "newinf_transm_ext_ge30",
                          "newinf_transm_ext_ent_full", "newinf_transm_ext_ent_20to30", "newinf_transm_ext_ent_ge30",
                          "popsize_full", "popsize_20to30", "popsize_ge30",
                          "vl_supp_per_pos", "vl_supp_per_diag"
)

n.scen <- 1
n.seed <- 30

all_responses$param_set <- rep(1:n.scen, each=n.seed)
all_responses$seed <- rep(1:n.seed, n.scen)

param_list$param_set <- rep(1:n.scen, each=n.seed)
param_list$seed <- rep(1:n.seed, n.scen)

# Compute means by seeds ----
by_param_set <- group_by(all_responses, param_set)
responses <- summarise(by_param_set,
                       prevalence = mean(prevalence),
                       prev_20to30 = mean(prev20to30),
                       prev_ge30 = mean(prevge30),
                       inc_transm_full = mean(inc_transm_full), #transm. olny
                       inc_transm_20to30 = mean(inc_transm_20to30),
                       inc_transm_ge30 = mean(inc_transm_ge30),
                       inc_transm_ext_full = mean(inc_transm_ext_full), #transm+ext.
                       inc_transm_ext_20to30 = mean(inc_transm_ext_20to30), 
                       inc_transm_ext_ge30 = mean(inc_transm_ext_ge30),
                       inc_transm_ext_ent_full = mean(inc_transm_ext_ent_full),#transm+ext+entry
                       inc_transm_ext_ent_20to30 = mean(inc_transm_ext_ent_20to30),
                       inc_transm_ext_ent_ge30 = mean(inc_transm_ext_ent_ge30),
                       newinf_transm_full = mean(newinf_transm_full), #transm. olny
                       newinf_transm_20to30 = mean(newinf_transm_20to30),
                       newinf_transm_ge30 = mean(newinf_transm_ge30),
                       newinf_transm_ext_full = mean(newinf_transm_ext_full), #transm+ext.
                       newinf_transm_ext_20to30 = mean(newinf_transm_ext_20to30), 
                       newinf_transm_ext_ge30 = mean(newinf_transm_ext_ge30),
                       newinf_transm_ext_ent_full = mean(newinf_transm_ext_ent_full),#transm+ext+entry
                       newinf_transm_ext_ent_20to30 = mean(newinf_transm_ext_ent_20to30),
                       newinf_transm_ext_ent_ge30 = mean(newinf_transm_ext_ent_ge30),
                       popsize_full = mean(popsize_full),
                       popsize_20to30 = mean(popsize_20to30),
                       popsize_ge30 = mean(popsize_ge30),
                       vl_supp_per_pos = mean(vl_supp_per_pos),
                       vl_supp_per_diag = mean(vl_supp_per_diag)
)

dim(responses)

# Reasonable Regions ----

View(all_responses)
View(responses)
apply(responses, 2, summary)

reas_inc <- intersect(which(responses$inc_transm_full >= 4),
                      which(responses$inc_transm_full <= 8))

reas_prev <- intersect(which(responses$prevalence >= 25),
                       which(responses$prevalence <= 40))

reas_region <- intersect(reas_inc, reas_prev)
length(reas_region)

res_dt <-
  filter(responses, param_set %in% reas_region) %>% 
  select(
    param_set,
    prevalence,
    prev_20to30,
    prev_ge30,
    inc_transm_full,
    inc_transm_20to30,
    inc_transm_ge30,
    newinf_transm_full,
    newinf_transm_20to30,
    newinf_transm_ge30
  )

apply(res_dt, 2, summary)
View(res_dt)

## below is needed because `param_list` contains each parameter combination
## repeated 10 times
unique_param_sets <- 
  param_list %>% 
  select(-c(X, global.random.seed, seed))%>% 
  group_by(param_set) %>%
  summarise_all(mean)

## unique parameters that produce reasonable results
reas_param_tbl <- 
  unique_param_sets %>%
  filter(row_number() %in% reas_region)

## obtain instance numbers from parameter sets
   instances_of_interest <- lapply(reas_region, 
                                   function (x)
                                   ((x-1)*10+1):(x*10)
   )
   instances_to_use <- unlist(instances_of_interest[which(
     reas_param_tbl$param_set == 25)])

## Check outcomes on instances of interest      
   View(all_responses[instances_to_use,])
   calib_dt <- round(all_responses[instances_to_use,], 2)
   write.csv(calib_dt, file="out/v54-calibrate-chicago-inst241-250-detailed-exam_dt.csv")
   
## Check parameter sets on instances to use   
   View(param_list[instances_to_use,])
   View(unique_param_sets[which(unique_param_sets$param_set == 25),-1])
   View(unique_param_sets[265,-1])
# # Analyze details in networks generated from selected instance ----
# library(network)
# library(ergm)
#    
#    # read networks from specific instance data
#    main_net <- readRDS("../../v54-calibrate-chicago-inst241-250-detailed-exam/instance_245/output/main_network_36500.RDS")
#    casual_net <- readRDS("../../v54-calibrate-chicago-inst241-250-detailed-exam/instance_245/output/casual_network_36500.RDS")
#    parameters <- read.table("../../v54-calibrate-chicago-inst241-250-detailed-exam/instance_245/output/parameters.txt")
#    
#    # check main and casual networks
#    network.size(main_net)
#    network.edgecount(main_net)
#    degreedist(main_net)/sum(degreedist(main_net))
#      
#    network.size(casual_net)
#    network.edgecount(casual_net)
#    degreedist(casual_net)/sum(degreedist(casual_net))
#    
# Annualized incidences in instance_248 ----
   

  source("../../../R/ph_summarize_functions.R")
  ranges = c(20, 29, 30, 34)
   
  dt <- summarize_directory("/project/khanna7/Projects/midway2/BARS/transmission_model/swift_proj/experiments/v54-calibrate-chicago-inst241-250-detailed-exam", ranges=ranges)
  saveRDS(object = dt, file = "out/v54-calibrate-chicago-inst241-250-detailed-exam.RDS")
  dt <- readRDS("out/v54-calibrate-chicago-inst241-250-detailed-exam.RDS")
  
  prev.dt <- dt[["prevs"]] %>%
    mutate(ycat = as.numeric(ycat))
  inc.dt <- dt[["incs"]] %>%
    mutate(ycat = as.numeric(ycat))
  
  prev.dt <- as.data.table(prev.dt)
  inc.dt <- as.data.table(inc.dt)
  
  n.years <- max(prev.dt$ycat)  
  n.seed <- 30
  n.scen <- 1
  
  # obtain tenth year prevalence 
  #prev.dt <- dt[["prevs"]]
  prev.dt[, param_set := rep(1:n.scen, each=n.seed*n.years)]
  prev.dt[, seed := rep(rep(1:n.seed, each=n.years), n.scen)]
  avg.prev.across.years <- 
    prev.dt[, .(avg_prev_across_years = mean(mean),
                avg_prev_across_year_age1 = mean(mean_1),
                avg_prev_across_year_age2 = mean(mean_2)),
            by=param_set]
  prev.dt.10 <- prev.dt[ycat == n.years]
  avg.prev.10yr <- prev.dt.10[, .(avg = mean(mean), 
                                  avg_1=mean(mean_1), 
                                  avg_2=mean(mean_2)), 
                              by=param_set]
  
  # obtain tenth year incidence
  inc.dt[, param_set := rep(1:n.scen, each=n.seed*n.years)]
  inc.dt[, seed := rep(rep(1:n.seed, each=n.years), n.scen)]
  inc.dt.100 <- inc.dt[ycat == 100]

  avg.inc.across.years <-
    inc.dt[, .(avg_inc_across_years = mean(full_inc_mean),
               avg_inc_across_year_age1 = mean(full_inc_mean_1),
               avg_inc_across_year_age2 = mean(full_inc_mean_2)),
           by=param_set]
  avg.inc.100yr <- inc.dt.100[, .(avg = mean(full_inc_mean), 
                                avg_1=mean(full_inc_mean_1), 
                                avg_2=mean(full_inc_mean_2),
                                se = sd(full_inc_mean)/sqrt(length(full_inc_mean))
  ), 
  by=param_set]
  
  View(avg.inc.100yr)
  
  # compute annualized prevalence and incidence
  summ_prev_dt <- prev.dt %>%
    group_by(param_set, ycat) %>%
    summarize(avg_annual_prev = mean(mean),
              sd_annual_prev = sd(mean)) 
  
  summ_inc_dt <- inc.dt %>%
    group_by(param_set, ycat) %>%
    summarize(avg_annual_inc = mean(full_inc_mean),
              sd_annual_inc = sd(full_inc_mean))
  
  # Analyze details in networks generated from selected instance ----
  library(network)
  library(ergm)
  
  # read networks from specific instance data
  main_net <- readRDS("../../v54-calibrate-chicago-inst241-250-detailed-exam/instance_8/output/main_network_36500.RDS")
  casual_net <- readRDS("../../v54-calibrate-chicago-inst241-250-detailed-exam/instance_8/output/casual_network_36500.RDS")
  parameters <- read.table("../../v54-calibrate-chicago-inst241-250-detailed-exam/instance_8/output/parameters.txt")
  
  # check main and casual networks
  network.size(main_net)
  network.edgecount(main_net)
  (network.edgecount(main_net)*2)/network.size(main_net)
  round(degreedist(main_net)/sum(degreedist(main_net)), 2)
  
  network.size(casual_net)
  network.edgecount(casual_net)
  (network.edgecount(casual_net)*2)/network.size(casual_net)
  degreedist(casual_net)/sum(degreedist(casual_net))
  
  # Analyze partnership data ----
  ## from https://github.com/khanna7/BARS/blob/development/transmission_model/r/testing/compute-partnership-durations.R
  require(data.table)
  loc2 <- "../../v54-calibrate-chicago-inst241-250-detailed-exam/"   
  dt <- fread(paste0(loc2,"instance_8/output/partnership_events.csv"))
  
  dt_main <- dt[network_type == 0]
  dt_casual <- dt[network_type == 1]
  
  ## group by p1, p2
  dt_main[,.(counts = .N),by=.(p1,p2),][,unique(counts)] # 1 2 4 3
  dt_main[,.(counts = .N),by=.(p1,p2),][,.N,by=counts]
  
  res <- dt_main[,.(tick, counts = .N),by=.(p1,p2),][counts == 2,.(dur = abs(diff(tick))), by=.(p1,p2)][,dur]
  summary(res)
  hist(res)
  
  ## casual(group by p1, p2)
  dt_casual[,.(counts = .N),by=.(p1,p2),][,unique(counts)] # 1 2 4 3
  dt_casual[,.(counts = .N),by=.(p1,p2),][,.N,by=counts]
  
  res <- dt_casual[,.(tick, counts = .N),by=.(p1,p2),][counts == 2,.(dur = abs(diff(tick))), by=.(p1,p2)][,dur]
  summary(res)
  hist(res)
  