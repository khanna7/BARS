## General
 - R files to analyze the data are provided in DataAnalysis.R which calls summarize_functions_ext6.R
 - The key workhorse functions are summarize_yearly_inc (general), summarize_yearly_inc_jailed_partners (pre-incarceration), and summarize_yearly_inc_released_partners (post-incarceration).
 - Code to make the bar and boxplots are available in the respectively labeled files. The key thing is to re-run the DataAnalysis.R folder multiple times

## Figure 1
- The subpopulation incidences are available in Figure1A.Rdata and Figure1B.Rdata. The code to generate this is from BaselinePlots.R, but the appropriate subpopulation needs to be specified running the function each time.

## Figure 2 and 3
 - Code to produce the incidence of the general vs the pre-incarceration incidences for the barplots are available in NetworkBoxPlot.Rdata, NetworkBarPlot.Rdata, and CareDisruptionBoxPlot.Rdata, CareDisruptionBarPlot.Rdata for Figures 2 and 3 respectively. The code to generate this is in BoxplotCode.R

## Figure 4
Code to generate Figure 4 (in the appendix) is generated from BiomarkerAppendixAnalysis1.R. You need to pull the branch that records every single person's HIV status (ask Daniel about what commit this is), and you also need to turn on the biomarker_log.csv file recording functionality turned on in the config. 

## Figure 5
Code to generate Figure 5 is in the BARSEmpiricalSurvivorshipCurve.R and ActualPartnershipRunsforJCM.R, in order to analyze this data you need the partnership_events.csv file recording functionality turned on in the config.
