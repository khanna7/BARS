# PrEP retention and scale intervention for uptake = 5K

rm(list=ls())

options(tibble.print_max = Inf, tibble.print_min = 50, dplyr.width=Inf)
options(tibble.width = Inf)

# Libraries ----

library(dplyr)
library(ggplot2)

# Data ----

full_res <- read.csv("../data/post-art-debug-prep-retention-scale-final_results.csv", header=FALSE) #final results
loc <- "../../post-art-debug-prep-retention-scale"

in1 <- read.table(paste0(loc,"/instance_1/output/parameters.txt"))
counts_dt <- read.csv(paste0(loc, "/instance_1/output/counts.csv"))

param_list <- read.csv("../../../data/prep-retention-scale-interv.csv")
param_list_txt <- read.table("../../../data/prep-retention-scale-interv.txt")

n.scen <- 42
n.seed <- 10

# Generate Response Dataframe ----

all_responses <- full_res[,c(1, 2, 4, 6, seq(17, 97, by=10), 98:109)]
names(all_responses) <- c("instance", 
                          "prevalence", "prev20to30", "prevge30",
                          "inc_transm_full", "inc_transm_20to30", "inc_transm_ge30",
                          "inc_transm_ext_full", "inc_transm_ext_20to30", "inc_transm_ext_ge30",
                          "inc_transm_ext_ent_full", "inc_transm_ext_ent_20to30", "inc_transm_ext_ent_ge30",
                          "newinf_transm_full", "newinf_transm_20to30", "newinf_transm_ge30",
                          "newinf_transm_ext_full", "newinf_transm_ext_20to30", "newinf_transm_ext_ge30",
                          "newinf_transm_ext_ent_full", "newinf_transm_ext_ent_20to30", "newinf_transm_ext_ent_ge30",
                          "popsize_full", "popsize_20to30", "popsize_ge30"
                          )
all_responses$param_set <- rep(1:n.scen, each=10)
all_responses$seed <- rep(1:n.seed, n.scen)
dim(all_responses)

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
  popsize_ge30 = mean(popsize_ge30)
  )

dim(responses)

# All Responses ----

responses %>% 
  select(prevalence, prev_20to30, prev_ge30,
         inc_transm_ext_ent_full, inc_transm_ext_20to30, inc_transm_ext_ent_ge30,
         newinf_transm_full, newinf_transm_20to30, newinf_transm_ge30
         )

#write.csv(responses, file="out/post-art-debug-prep-retention-scale.csv")

# Select Unique Parameters From List ----

seq_of_ones <- seq(1, nrow(param_list), by=10)
write.csv(param_list[seq_of_ones,], file="out/prep-retention-cale-interv-2-param-list.csv")

# Summary of Results Regions ----

apply(responses, 2, summary)


# Annual prevalence and incidence ----

# source function
source("../../../R/ph_summarize_functions.R")

# set input
ranges = c(20, 29, 30, 34)

# call function
#dt <- summarize_directory(loc, ranges=ranges)
#saveRDS(object = dt, file = "out/post-art-debug-prep-retention-scale.RDS")
dt <- readRDS("out/post-art-debug-prep-retention-scale.RDS")

prev.dt <- dt[["prevs"]] %>%
  mutate(ycat = as.numeric(ycat))
inc.dt <- dt[["incs"]] %>%
  mutate(ycat = as.numeric(ycat))

prev.dt <- as.data.table(prev.dt)
inc.dt <- as.data.table(inc.dt)
n.years <- max(prev.dt$ycat)

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

#write.csv(avg.prev.10yr, file="out/prep-reten-scale-interv-2-tenyearprev.csv")

# obtain tenth year incidence
#inc.dt <- dt[["incs"]]
inc.dt[, param_set := rep(1:n.scen, each=n.seed*n.years)]
inc.dt[, seed := rep(rep(1:n.seed, each=n.years), n.scen)]
inc.dt.10 <- inc.dt[ycat == 10]
avg.inc.across.years <-
  inc.dt[, .(avg_inc_across_years = mean(full_inc_mean),
             avg_inc_across_year_age1 = mean(full_inc_mean_1),
             avg_inc_across_year_age2 = mean(full_inc_mean_2)),
         by=param_set]
avg.inc.10yr <- inc.dt.10[, .(avg = mean(full_inc_mean), 
                              avg_1=mean(full_inc_mean_1), 
                              avg_2=mean(full_inc_mean_2)), 
                          by=param_set]

final_uptake <- c("13.7%-control", "20%", 
                  "30%", "40%", "50%", "60%")
retention <- c("180d-control", "270d", "365d", 
               "450d", "540d", "630d", "730d")

#write.csv(avg.inc.10yr, file="out/prep-reten-scale-interv-2-tenyearinc.csv")

tenthyr.outcomes <- round(left_join(avg.prev.10yr, avg.inc.10yr, by ="param_set"), 2)
avg.across.years <- round(left_join(avg.prev.across.years, avg.inc.across.years, 
                                    by ="param_set"), 2)

head(tenthyr.outcomes)
head(avg.across.years)
head(responses)

tenthyr.outcomes <- apply(tenthyr.outcomes, c(1,2), function (x) round(x, 2))

summ_prev_dt <- prev.dt %>%
  group_by(param_set, ycat) %>%
  summarize(avg_annual_prev = mean(mean),
            sd_annual_prev = sd(mean)) 

summ_prev_dt$final_uptake = rep(final_uptake, each = n.years) 
summ_prev_dt$retention <- rep(retention, each=length(final_uptake)*n.years)

summ_inc_dt <- inc.dt %>%
  group_by(param_set, ycat) %>%
  summarize(avg_annual_inc = mean(full_inc_mean),
            sd_annual_inc = sd(full_inc_mean))

summ_inc_dt$final_uptake = rep(final_uptake, each = n.years) 
summ_inc_dt$retention <- rep(retention, each=length(final_uptake)*n.years)

inc.dt[, c("avg_inc_each_year", 
           "avg_inc_each_age1", 
           "avg_inc_year_age2") := list(
             mean(full_inc_mean),
             mean(full_inc_mean_1),
             mean(full_inc_mean_2)), 
       by=param_set]

summ_newinf_dt <- inc.dt %>%
  group_by(param_set, ycat) %>%
  summarize(avg_newinc = mean(tot_ee_inc_sum),
            sd_annual_inc = sd(tot_ee_inc_sum))

summ_newinf_dt$final_uptake = rep(final_uptake, each = n.years) 
summ_newinf_dt$retention <- rep(retention, each=length(final_uptake)*n.years)


control.uptake.prev.dt <- 
  summ_prev_dt %>% 
  filter(final_uptake == "13.7%-control") 

control.uptake.inc.dt <- 
  summ_inc_dt %>% 
  filter(final_uptake == "13.7%-control") 

control.retention.prev.dt <- 
  summ_prev_dt %>% 
  filter(retention == "180d-control") 

control.retention.inc.dt <- 
  summ_inc_dt %>% 
  filter(retention == "180d-control")

control.retention.newinf.dt <- 
  summ_newinf_dt %>% 
  filter(retention == "180d-control")

p <- 
  ggplot(control.uptake.prev.dt, 
         aes(x=ycat, y=avg_annual_prev, color=retention))+
  geom_line()+geom_point()+
  ggtitle("Control PrEP Uptake, Alpha = 1")+
  xlab("year of intervention")+
  ylab("prevalence (%)")+
  scale_y_continuous(limit=c(10,35))+
  scale_x_continuous(breaks = 1:10)

p+
  geom_errorbar(data=p[["data"]],
                aes(x=ycat, ymin=(avg_annual_prev-sd_annual_prev), 
                    ymax=avg_annual_prev+sd_annual_prev),
                width = 0.15)

p1 <-
  ggplot(control.uptake.inc.dt, 
         aes(x=ycat, y=avg_annual_inc, color=retention))+
  geom_line()+geom_point()+
  ggtitle("Control PrEP Uptake, Alpha = 1")+
  xlab("year of intervention")+
  ylab("incidence (per 100 py)")+
  scale_y_continuous(limit=c(0,8))+
  scale_x_continuous(breaks = 1:10)

p1+
  geom_errorbar(data=p1[["data"]],
                aes(x=ycat, ymin=(avg_annual_inc-sd_annual_inc), 
                    ymax=avg_annual_inc+sd_annual_inc),
                width = 0.15)

p2 <- 
  ggplot(control.retention.prev.dt, 
         aes(x=ycat, y=avg_annual_prev, color=final_uptake))+
  geom_line()+geom_point()+
  ggtitle("Control PrEP Retention, Alpha = 1")+
  xlab("year of intervention")+
  ylab("prevalence (%)")+
  scale_y_continuous(limit=c(10,35))+
  scale_x_continuous(breaks = 1:10)

p2+
  geom_errorbar(data=p2[["data"]],
                aes(x=ycat, ymin=(avg_annual_prev-sd_annual_prev), 
                    ymax=avg_annual_prev+sd_annual_prev),
                width = 0.15)


p3 <- 
  ggplot(control.retention.inc.dt, 
         aes(x=ycat, y=avg_annual_inc, color=final_uptake))+
  geom_line()+geom_point()+
  ggtitle("Control PrEP Retention, Alpha = 1")+
  xlab("year of intervention")+
  ylab("incalence (%)")+
  scale_y_continuous(limit=c(0,10))+
  scale_x_continuous(breaks = 1:10)

p3+
  geom_errorbar(data=p3[["data"]],
                aes(x=ycat, ymin=(avg_annual_inc-sd_annual_inc), 
                    ymax=avg_annual_inc+sd_annual_inc),
                width = 0.15)
ggsave("out/control.retention.inc.png")
p3[["data"]] %>% filter(ycat==10) #10th year summary for various uptake levels

p4 <- 
  ggplot(control.retention.newinf.dt, 
         aes(x=ycat, y=avg_newinc, color=final_uptake))+
  geom_line()+geom_point()+
  ggtitle("Control PrEP Retention")+
  xlab("year of intervention")+
  ylab("number of new infections")+
  scale_y_continuous(limit=c(0,300))+
  scale_x_continuous(breaks = 1:10)

p4+
  geom_errorbar(data=p4[["data"]],
                aes(x=ycat, ymin=(avg_newinc-sd_annual_inc), 
                    ymax=avg_newinc+sd_annual_inc),
                width = 0.15)

p4[["data"]]

p4[["data"]] %>% filter(final_uptake == "30%")
# Population size summary ----

responses_popsize <- 
  responses %>% 
  select(popsize_full, popsize_20to30, popsize_ge30)

# Compute annual growth rate
starting.popsize <- counts_dt$vertex_count[2]
#starting.popsize.20to30 <- counts_dt$vertex_count_18[2]
#starting.popsize.ge30 <- counts_dt$vertex_count_B[2]  
  
n.ts <- nrow(counts_dt) - 1
n.yrs <- n.ts/365

growth.rate <- ((5600/5000)^(1/n.yrs) - 1)*100 #example

responses_popsize %>% 
  mutate(growthrate_allages = ((popsize_full/starting.popsize)^(1/n.yrs) - 1)*100
         #growthrate_20to30 = ((popsize_20to30/starting.popsize.20to30)^(1/n.yrs) - 1)*100,
         #growthrate_ge30 = ((popsize_ge30/starting.popsize.ge30)^(1/n.yrs) - 1)*100
         )


         
