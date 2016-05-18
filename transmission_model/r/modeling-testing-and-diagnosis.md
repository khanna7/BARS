## Modeling Testing and Diagnosis

To model testing and diagnosis, we need some new constants and attributes. 

| Constants       | Attributes           | 
| ------------- |:-------------:| -----:|
| Detection Window  (`detection.window`)    | Diagnosed (`diagnosed`)| 
| Mean time until next test (`mean.time.until.next.test`)      |  Time until next test (`time.until.next.test`)     |   
| Daily testing probability (`daily.testing.prob`) | Lag between diagnosis and ART initiaion  (`lag.bet.diagnosis.and.art.init`) |      
|Lag between diagnosis and ART initiation (`lag.bet.diagnosis.and.art.init`) | Tested today (`tested.today`)|  
|| Number of Tests (`number.of.tests`)|  

These new constants are in `chicago_parameters.R`, and are used to initialize the new attributes in the main network at time 0 in `chicago-init-net-est.R`.

In the temporal simulation:
For everyone who is not diagnosed (including HIV- and HIV+ individuals), there is a `time.until.next.test` attribute, drawn from a geometric distribution where

```r
time.until.next.text <- rgeom(1, daily.testing.prob) + 1
```
where `daily.testing.prob = 1/mean.time.until.next.test`. An individual will be "diagnosed" if: 1)they are HIV-positive, 2) if they have been infected longer than the detection windo of the test (currently set as 22 days). Initially, we will specify the testing model as being drawn from homogeneous geometric distribution, with daily probability defined as the reciprocal of the mean value of that distribution. For all negative or undiagnosed (i.e. HIV-positive but undiagnosed), we will have a time until next test. For all new entrants, this attribute should be defined at entry.For people who test positive, the attribute is set to 0. For people who test negative, this attribute should be computed again, using the formula above (the `+1` exists because for some individuals, a random draw of `time.until.next.test` will  be 0, due to the geometric distribution). 

For HIV+ diagnosed individuals, when the lag time  between diagnosis and ART initiation has passed (`lag.bet.diagnosis.and.art.initation`), ART will be initiated -- currently set to a constant value of 0, but I will work on parameterizing this quantity soon -- at which point it will only exist as an individual-level attribute. 

At this point, both the testing and lag between diagnosis and ART initiation are simple, which is probably okay for the baseline  burnin. At some point, we could get into more realistic testing recommendations and guidelines --- an interesting research area in it's own right. 

Two other attributes for specified for bookkeeping: an indicator for whether a particular agent tested today, and the number of tests an individual has had over the course of their lifetime. 
