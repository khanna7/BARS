rm(list=ls())

options(tibble.print_max = 100, tibble.print_min = 100)
options(tibble.width = Inf)

# Libraries ----

library(dplyr)
library(ggplot2)

# Data ----

full_res <- read.csv("../data/v51-retention-final_results.csv", header=FALSE) #final results
loc <- "../../v51-retention"

in1 <- read.table(paste0(loc,"/instance_1/output/parameters.txt"))
upf <- read.table(paste0(loc, "/prep-unbalanced.txt"), sep=",")
#upf_csv <- read.csv("../../../data/serodiscordant.csv")

# Generate Response Dataframe ----

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
n.scen <- 7
n.seed <- 10
all_responses$param_set <- rep(1:n.scen, each=n.seed)
all_responses$seed <- rep(1:n.seed, 1)

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

View(all_responses)
View(responses)
apply(responses, 2, summary)

# Compute annual incidence, prevalence, and number of infections ----
source("../../../R/ph_summarize_functions.R")
ranges = c(20, 29, 30, 34)

#dt <- summarize_directory(loc, ranges=ranges)
#saveRDS(object = dt, file = "out/v51-retention.RDS")
dt <- readRDS("out/v51-retention.RDS")

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

# obtain tenth year incidence
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
                              avg_2=mean(full_inc_mean_2),
                              se = sd(full_inc_mean)/sqrt(length(full_inc_mean))
                              ), 
                          by=param_set]

View(avg.inc.10yr)

# obtain annualized number of new infections
inct.dt.by.param.ycat <- inc.dt %>% 
  group_by(param_set, ycat)
tot_inc_sum_dt <- (dt[["incs"]][["tot_inc_sum"]])

tot.newinf <-
  inc.dt[, .(sum_newinf_across_years = sum(tot_inc_sum),
             sum_newinf_across_year_age1 = sum(tot_inc_sum_1),
             sum_newinf_across_year_age2 = sum(tot_inc_sum_2)),
         by=param_set]

tot.newinf.across.years <-
  inc.dt %>% 
  group_by(param_set, ycat) %>%
  summarise(sum_newinf_across_years = sum(tot_inc_sum)/n.years,
            sum_newinf_across_year_age1 = sum(tot_inc_sum_1)/n.years,
            sum_newinf_across_year_age2 = sum(tot_inc_sum_2)/n.years
  )


View(
  tot.newinf.across.years
)

retention <- c("Base (1 year)", "540 days", "730 days", 
                  "900 days", "1095 days", "1275 days", "1440 days")
# retention <- c("180d-control", "270d", "365d", 
#                "450d", "540d", "630d", "730d")

summ_prev_dt <- prev.dt %>%
  group_by(param_set, ycat) %>%
  summarize(avg_annual_prev = mean(mean),
            sd_annual_prev = sd(mean)) 
summ_prev_dt$retention = rep(retention, each = 10) 
#summ_prev_dt$retention <- rep(retention, each=420/7)

summ_inc_dt <- inc.dt %>%
  group_by(param_set, ycat) %>%
  summarize(avg_annual_inc = mean(full_inc_mean),
            sd_annual_inc = sd(full_inc_mean))
summ_inc_dt$retention = rep(retention, each = 10)
#summ_inc_dt$retention <- rep(retention, each=420/7)

p <- 
  ggplot(filter(summ_prev_dt, factor(param_set) %in% as.factor(1:6)), 
         aes(x=ycat, y=avg_annual_prev, color=final_uptake))+
  geom_line()+geom_point()+
  ggtitle("Average PrEP Retention = 6 months")+
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
  ggplot(filter(summ_inc_dt, factor(param_set) %in% as.factor(1:6)), 
         aes(x=ycat, y=avg_annual_inc, color=retention))+
  geom_line()+geom_point()+
  #ggtitle("Average PrEP Retention = 1 year")+
  xlab("year of intervention")+
  ylab("incidence (per 100 py)")+
  scale_y_continuous(limit=c(0,8))+
  scale_x_continuous(breaks = 1:10)

p1+
  geom_errorbar(data=p1[["data"]],
                aes(x=ycat, ymin=(avg_annual_inc-sd_annual_inc), 
                    ymax=avg_annual_inc+sd_annual_inc),
                width = 0.15)

ggsave("out/v51-retention.png")

