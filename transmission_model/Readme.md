# Readme for BARS HIV Transmission Model #

### Requirements ###

* RepastHPC 2.2.0
* R 3.4.1+ 
* network R package v. 1.13.0.1 
* tergm R package v. 3.4.1 
* ergm  R package v. 3.8.0 
* tergmLite R package v. 0.1.2 
* Rcpp R package v. 1.0.0
* RInside R package v. 0.2.14
* networkDynamic R package v. 0.9.0


### Versions Used on Midway2 ###

* RepastHPC 2.2.0
* R 3.4.1
* network R package v. 1.13.0.1 
* tergm R package v. 3.4.1
* ergm  R package v. 3.8.0 
* tergmLite R package v. 0.1.2
* Rcpp R package v. 1.0.0
* RInside R package v. 0.2.14
* networkDynamic R package v. 0.9.0

### Unit Tests ###

Due to some complications with RInside and recreating the R environment within a running
process, the tests need to be run separately. Once compiled, you can run them with:

`./unit_tests X.*`

where X is the name of the test package. For example,

`./unit_tests --gtest_filter=MiscTests.*`

 You can list all the test packages with

`./unit_tests --gtest_list_tests`
