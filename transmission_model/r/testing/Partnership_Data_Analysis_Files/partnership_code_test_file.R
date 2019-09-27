rm(list = ls())

setwd("~/Desktop")
library(tidyr)
library(statnet)
library(stringr)
library(devtools)
library(testthat)

dt<-read.csv("partnership_events.csv")
main_matrix<-readRDS("v54-calibrate-chicago-instance248_main_el.RDS")

#Before sourcing, be sure to comment out the file writing components of the code you're sourcing.
source('partnership_code_final_draft.R')
#If you choose to not comment out:
#once you start seeing percentages (white text) in the console, cancel the run. 
#This is generating data. The functions have been sourced.

#synthentic data generation
#Need the following synthetic partnership data:
     #1) dt$p1-->dt_filter_t_p1
     #2) dt$p2-->dt_filter_t_p2
     #3) dt$type-->dt_filter_t_type

#The following is used to filter the above:
     #4) dt$network_type
     #5) dt$tick

#We also need an initial registry.

#######~~~Test preparation step 1: creating a registry that we will use over and over. 
#######~~~It is the partnership data we want to vary. 
#######~~~The registry can only be a source of error if the partnership data caused it to have one.

# synthetic registry
test_registry<-list()
for (i in 1:10){test_registry[[i]]<-c(i+1,i+11)} #extra +1 for c++ difference

#first function to test: main_relationship_registry(interval,mr)

###################################~~~~~~TEST 1 of 5~~~~~~~#######################################

# synthetic partnership data
tick<-c(1,1,1,1,1,
        2,2,2,2,2)
p1<-1:10
p2<-11:20
type<-c(0,2,3,4,0,
        2,0,0,3,0)
network_type<-c(0,0,0,0,0,
                1,1,1,1,1)
test_df<-data.frame(tick,p1,p2,type,network_type)
dt<-test_df

#expected resultant registry:
result_registry<-list()
for (i in 1:5){
  result_registry[[i]]<-test_registry[[i+5]]
}

test_that("relationship_registry test 1", {
  expect_equal(relationship_registry(1:2,test_registry,0)[[1]],
               result_registry)
  expect_equal(relationship_registry(1:10,test_registry,0)[[1]],
               result_registry)
  expect_equal(relationship_registry(1:1,test_registry,0)[[1]],
               result_registry)
})

###################################~~~~~~TEST 2 of 5~~~~~~~#######################################

#synthetic partnership data
tick<-c(1,2,3,4,5,
        6,7,8,9,10)
p1<-1:10
p2<-c(11,12,13,14,15,
      2,3,4,5,6)
type<-c(0,2,3,4,0,
        1,1,1,1,1)
network_type<-c(0,0,0,0,0,
                0,0,0,0,0)
test_df<-data.frame(tick,p1,p2,type,network_type)
dt<-test_df

#expected resultant registry:
result_registry<-list()
for (i in 1:5){
  result_registry[[i]]<-test_registry[[i+5]]
  result_registry[[i+5]]<-c(p1[i+5],p2[i+5])+1 #account for the c++ addition in the function.
  }

test_that("relationship_registry test 2", {
  expect_equal(relationship_registry(1:50,test_registry,0)[[1]],
               result_registry)
  expect_equal(relationship_registry(1:10,test_registry,0)[[1]],
               result_registry)
  expect_equal(relationship_registry(1:15,test_registry,0)[[1]],
               result_registry)
})

###################################~~~~~~TEST 3 of 5~~~~~~~#######################################

#synthetic partnership data
tick<-c(1,2,3,4,5,
        6,7,8,9,20)
p1<-1:10
p2<-c(11,12,13,14,15,
      11,12,13,14,15)
type<-c(0,2,3,4,0,
        1,1,1,1,1)
network_type<-c(0,0,0,0,0,
                0,0,0,0,0)
test_df<-data.frame(tick,p1,p2,type,network_type)
dt<-test_df

#expected resultant registry:
result_registry<-list()
for (i in 1:5){
  result_registry[[i]]<-test_registry[[i+5]]
  result_registry[[i+5]]<-c(p1[i+5],p2[i+5])+1 #account for the c++ addition in the function.
}

test_that("relationship_registry test 3", {
  expect_equal(relationship_registry(1:20,test_registry,0)[[1]],
               result_registry)
  expect_equal(relationship_registry(1:30,test_registry,0)[[1]],
               result_registry)
  expect_equal(relationship_registry(1:40,test_registry,0)[[1]],
               result_registry)
})

###################################~~~~~~TEST 4 of 5~~~~~~~#######################################
tick<-c(1,2,3,4,5,
        6,7,8,9,10)
p1<-1:10
p2<-c(11,12,13,14,15,
      2,3,4,5,6)
type<-c(0,0,0,0,0,
        0,1,0,1,0)
network_type<-c(0,0,0,0,0,
                1,1,1,1,1)
test_df<-data.frame(tick,p1,p2,type,network_type)
dt<-test_df

#expected resultant registry:
result_registry<-list()
for (i in 1:10){
  result_registry[[i]]<-test_registry[[i]]
}
result_registry[[11]]<-c(8,4) #account for the c++ addition in the function.
result_registry[[12]]<-c(10,6)

test_that("relationship_registry test 4", {
  expect_equal(relationship_registry(1:10,test_registry,1)[[1]],
               result_registry)
})
###################################~~~~~~TEST 5 of 5~~~~~~~#######################################
tick<-c(1,2,3,4,5,
        6,7,8,9,10)
p1<-1:10
p2<-c(11,12,13,14,15,
      2,3,4,5,6)
type<-c(1,1,1,1,1,
        0,2,3,4,0)
network_type<-c(1,1,1,1,1,
                1,1,1,1,1)
test_df<-data.frame(tick,p1,p2,type,network_type)
dt<-test_df

#expected resultant registry:
result_registry<-list()
for (i in 1:5){
  result_registry[[i]]<-test_registry[[i]]
  result_registry[[i+5]]<-test_registry[[i+5]]
  result_registry[[i+10]]<-test_registry[[i]]
}

test_that("relationship_registry test 5", {
  expect_equal(relationship_registry(1:10,test_registry,1)[[1]],
               result_registry)
})






