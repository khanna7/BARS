# Analyze PrEP usage data in v0.47

rm(list=ls())

# Load libraries ---------------------------

library(dplyr)
library(ggplot2)


# Set location of data ---------------------------

loc <- "/project/khanna7/Projects/midway2/BARS/transmission_model/swift_proj/experiments/v49-random-selection/"


# Read input upf ---------------------------

upf <- read.table(paste0(loc, "increase-prep-use.txt"), sep=",")


# Set instance numbers ---------------------------

instances <- 1:60 #base=1:10, 20%=11:20,...,60%=51:60
counts_data <- as.list(instances)

for (i in instances){
  counts_data[[i]] <- read.csv(paste0(loc, "instance_", instances[i], "/output/counts.csv"))  
  counts_data[[i]] <- counts_data[[i]][-1,] #first row needs to be dropped
  }

# Merge data frames ---------------------------

counts_df <- do.call("rbind", counts_data)
counts_df$instance <- rep(1:10, each=nrow(counts_data[[1]]))


# Partition cases ---------------------------

n_ticks <- nrow(counts_data[[1]])
base_instances <- 1:10
pct_20_instances <- 11:20
pct_30_instances <- 21:30
pct_40_instances <- 31:40
pct_50_instances <- 41:50
pct_60_instances <- 51:60

base_rows <- ((min(base_instances) - 1)*n_ticks) : (max(base_instances)*n_ticks) 
pct_20_rows <- ((min(pct_20_instances) - 1)*n_ticks) : (max(pct_20_instances)*n_ticks) 
pct_30_rows <- ((min(pct_30_instances) - 1)*n_ticks) : (max(pct_30_instances)*n_ticks) 
pct_40_rows <- ((min(pct_40_instances) - 1)*n_ticks) : (max(pct_40_instances)*n_ticks)
pct_50_rows <- ((min(pct_50_instances) - 1)*n_ticks) : (max(pct_50_instances)*n_ticks)
pct_60_rows <- ((min(pct_60_instances) - 1)*n_ticks) : (max(pct_60_instances)*n_ticks)


# Outcome 1 of interest: % of negatives on PrEP ---------------------------

ggplot(counts_df[pct_30_rows,])+ #Change specific rows to be plotted here, based on defs above
  geom_line(aes(x=tick/365, y=on_prep/uninfected, col=as.factor(instance)))+
  xlab("time step")+ylab("mean degree")+
  scale_y_continuous(limits=c(0, 1))+
  ylab("% of negatives on PrEP (Base Case)")+
  xlab("Time (years)")

ggsave(filename = "out/v49-random-selection-30pct-target.png")

## Check PrEP usage at the end

tail(counts_df[base_rows,]$on_prep/counts_df[base_rows,]$uninfected)
tail(counts_df[pct_20_rows,]$on_prep/counts_df[pct_20_rows,]$uninfected)
tail(counts_df[pct_30_rows,]$on_prep/counts_df[pct_30_rows,]$uninfected)
tail(counts_df[pct_40_rows,]$on_prep/counts_df[pct_40_rows,]$uninfected)
tail(counts_df[pct_50_rows,]$on_prep/counts_df[pct_50_rows,]$uninfected)
tail(counts_df[pct_60_rows,]$on_prep/counts_df[pct_60_rows,]$uninfected)
