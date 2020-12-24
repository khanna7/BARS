rm(list=ls())

options(tibble.print_max = 100, tibble.print_min = 100)
options(tibble.width = Inf)

# Libraries ----

library(dplyr)
library(ggplot2)

# Data ----

full_res <- read.csv("../data/vl-supp-1m-inst25-prep-retention-unbalanced-final_results.csv", header=FALSE) #final results
loc <- "../../vl-supp-1m-inst25-prep-retention-unbalanced"

in1 <- read.table(paste0(loc,"/instance_1/output/parameters.txt"))
param_list <- read.csv("../../../data/prep-unbalanced.csv")

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

n.scen <- 252
n.seed <- 10

all_responses$param_set <- rep(1:n.scen, each=n.seed)
all_responses$seed <- rep(1:n.seed, n.scen)

#param_list$param_set <- rep(1:n.scen, each=n.seed)
#param_list$seed <- rep(1:n.seed, n.scen)

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

# Consider regions where final scale-up target is 30% ----

# inputs
# param_list_30pct <- 
#   param_list %>% filter(prep.yearly.increment.lt == 0.0346)
# 
# param_set_30pct <- 
#   param_list_30pct %>% 
#   group_by(param_set) %>%
#   summarise_all(mean)
# 
# param_list_30pct_run_num <- 
#   param_list_30pct %>% 
#   select(run.number) %>%
#   pull()

# Consider annual prevalences and incidences ----
source("../../../R/ph_summarize_functions.R")

# set input
ranges = c(20, 29, 30, 34)

# call function
##dt <- summarize_directory(loc, ranges=ranges)
##saveRDS(object = dt, file = "out/vl-supp-1m-inst25-prep-retention-unbalanced.RDS")
dt <- readRDS("out/vl-supp-1m-inst25-prep-retention-unbalanced.RDS")

prev.dt <- dt[["prevs"]] %>%
  mutate(ycat = as.numeric(ycat))
inc.dt <- dt[["incs"]] %>%
  mutate(ycat = as.numeric(ycat))

prev.dt <- as.data.table(prev.dt)
inc.dt <- as.data.table(inc.dt)
n.years <- max(prev.dt$ycat)

# obtain tenth year prevalence 
#prev.dt <- as.data.table(arrange(prev.dt, instance))
prev.dt[, param_set := rep(1:n.scen, each=100)]
prev.dt[, seed := rep(rep(1:n.seed, each=10), n.scen)]
avg.prev.across.years <- 
  prev.dt[, .(avg_prev_across_years = mean(mean),
              avg_prev_across_year_age1 = mean(mean_1),
              avg_prev_across_year_age2 = mean(mean_2)),
          by=param_set]
prev.dt.10 <- prev.dt[ycat == 10]
avg.prev.10yr <- prev.dt.10[, .(avg_prev = mean(mean), 
                                avg_prev_1=mean(mean_1), 
                                avg_prev_2=mean(mean_2)), 
                            by=param_set]

# obtain tenth year incidence
#inc.dt <- as.data.table(arrange(inc.dt, instance))
inc.dt[, param_set := rep(1:n.scen, each=100)]
inc.dt[, seed := rep(rep(1:n.seed, each=10), n.scen)]
avg.inc.across.years <-
  inc.dt[, .(avg_inc_across_years = mean(full_inc_mean),
             avg_inc_across_year_age1 = mean(full_inc_mean_1),
             avg_inc_across_year_age2 = mean(full_inc_mean_2)),
         by=param_set]

inc.dt.10 <- inc.dt[ycat == 10]
avg.inc.10yr <- inc.dt.10[, .(avg_inc = mean(full_inc_mean), 
                              avg_inc_age1=mean(full_inc_mean_1), 
                              avg_inc_age2=mean(full_inc_mean_2)), 
                          by=param_set]

tenthyr.outcomes <- round(left_join(avg.prev.10yr, avg.inc.10yr, by ="param_set"), 2)
avg.across.years <- round(left_join(avg.prev.across.years, avg.inc.across.years, 
                                    by ="param_set"), 2)

head(tenthyr.outcomes)
head(avg.across.years)
head(responses)

tenthyr.outcomes <- apply(tenthyr.outcomes, c(1,2), function (x) round(x, 2))
tenthyr.outcomes

# 10th year outcomes summary  ----
View(tenthyr.outcomes[seq(1, 252, by=60),]) #base usage
View(tenthyr.outcomes[seq(3, 252, by=6),]) #target usage 30%

## PrEP summary
View(responses[seq(1, 252, by=60),]) #base usage
View(responses[seq(3, 252, by=60),]) #target usage 30%

# Plot data ----

final_uptake <- c("13.7%-control", "20%", 
                  "30%", "40%", "50%", "60%")
retention <- c("6m", "9m", "12m", 
                "15m", "18m", "21m", "24m")

summ_prev_dt <- prev.dt %>%
  group_by(param_set, ycat) %>%
  summarize(avg_annual_prev = mean(mean),
            sd_annual_prev = sd(mean)) 

summ_prev_dt$final_uptake = rep(final_uptake, each = 10) 
summ_prev_dt$retention <- rep(retention, each=2520/7)

summ_inc_dt <- inc.dt %>%
  group_by(param_set, ycat) %>%
  summarize(avg_annual_inc = mean(full_inc_mean),
            sd_annual_inc = sd(full_inc_mean))

summ_inc_dt$final_uptake = rep(final_uptake, each = 10) 
summ_inc_dt$retention <- rep(retention, each=2520/7)

summ_prev_dt <- 
summ_prev_dt %>% 
  filter(final_uptake == "13.7%-control")

summ_inc_dt <- 
summ_inc_dt %>%
  filter(final_uptake == "13.7%-control")

p <- 
  ggplot(summ_prev_dt, 
         aes(x=ycat, y=avg_annual_prev, color=retention))+
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
  ggplot(summ_inc_dt, 
         aes(x=ycat, y=avg_annual_inc, color=retention))+
  geom_line()+geom_point()+
  ggtitle("")+
  xlab("year of intervention")+
  ylab("incidence (per 100 py)")+
  scale_y_continuous(limit=c(2,6))+
  scale_x_continuous(breaks = 1:10)

p1+
  geom_errorbar(data=p1[["data"]],
                aes(x=ycat, ymin=(avg_annual_inc-sd_annual_inc), 
                    ymax=avg_annual_inc+sd_annual_inc),
                width = 0.15)

ggsave("out/vl-supp-1m-inst25-prep-retention-unbalanced-inc-data.png")

