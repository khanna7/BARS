# BARS
Code for published _AIDS_ [paper](https://pubmed.ncbi.nlm.nih.gov/31490212/)

## Data Generation
 - Can be done either using executable v0.54 or v0.57.
 - [Changelog](https://github.com/khanna7/BARS/blob/scheduled_intervention/transmission_model/changelog.txt) says "No version 0.57 due to versioning confusion" but Bryan, Francis used it to reproduce the results. 
 - This branch is off of commit for v0.57.

## Unrolled Parameter Files (UPFs) for Data Generation
 - R files to generate the UPFs for the calibration and each of the interventions are [here](https://github.com/khanna7/BARS/tree/AIDS-GTZ-paper/transmission_model/swift_proj/data).
 - The code produces output in txt format that is read by swift, and in csv format for easy readability. 

## Data Analysis 
 - Code to produce annualized HIV incidence and prevalence for the calibration and intervention runs is [here](https://github.com/khanna7/BARS/tree/AIDS-GTZ-paper/transmission_model/swift_proj/data).
 - Code to generate the exact figures that are in the paper are no longer available.
 - But, the annualized prevalences and incidences (along with with the CIs) generated from the above have been plotted by folks who have reproduced the model. (See point 18 under reference item #1 for a description of the data analysis workflow)

## References
 - Reproducing the AIDS paper: [Quick Start Guide](https://docs.google.com/document/d/1XHhmn8uwgCQpVK7l9bDTPfvhAGCgga9TL3oxlQopxsM/edit?usp=sharing)
 - A longer version is [here](https://docs.google.com/document/d/1jfhwJFwRDJGKJOr-iyLYz-Ah5snKnHtYzuAwNPflTqA/edit#bookmark=id.5kxqzyl3xk6c). (The quick start guide links to specific sections in this longer version.)



 
