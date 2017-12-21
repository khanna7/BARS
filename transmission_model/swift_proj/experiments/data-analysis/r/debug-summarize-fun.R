## debug summarize function

# Clear slate ------

rm(list=ls())

# Set wkg. directory ------

setwd("/project/khanna7/Projects/midway2/BARS/transmission_model/swift_proj/experiments/data-analysis/r")

# Data ------

counts <- read.csv("../../age-20-30-34/instance_1/output/counts.csv")


# Summarize functions ------
source("../../../R/summarize_functions.R")


# Test  ------

summarize_inc("../../age-20-30-34/instance_1/output/counts.csv")

summarize_prev("../../age-20-30-34/instance_1/output/counts.csv")
summarize_pop_size("../../age-20-30-34/instance_1/output/counts.csv")

