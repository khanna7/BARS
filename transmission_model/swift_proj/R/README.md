## General
 - R files to analyze the data are provided in BaselinePlots.R, NetworkInterventionIncAnalysis.R and CareInterventionIncAnalysis.R which calls summarize_functions_ext6.R
 - The key workhorse functions are summarize_yearly_inc (general), summarize_yearly_inc_jailed_partners (pre-incarceration), and summarize_yearly_inc_released_partners (post-incarceration).
 - Code to make the bar and boxplots are available in the respectively labeled files. You should be able to hit source on all these files once you have the relevant experiment directory specified and have the code compile and it'll automatically save the objects you need to plot all the figures.

## Figure 1
- The code to generate this is from BaselinePlots.R. Source the code to generate the baseline (90 Day disruption scenario) subpopulation incidences for ever jailed, never jailed, momentary degree of at least 1 but not CJI, 

## Figure 2 and 3
 - Code to produce the incidence of the general vs the pre-incarceration incidences for the barplots are available in NetworkBoxPlot.Rdata, NetworkBarPlot.Rdata, and CareDisruptionBoxPlot.Rdata, CareDisruptionBarPlot.Rdata for Figures 2 and 3 respectively. The code to generate this is in BoxplotCode.R which is run AFTER NetworkInterventionIncAnalysis.R and CareInterventionIncAnalysis.R are run.

## Figure 4
Code to generate Figure 4 (in the appendix) is generated from BiomarkerAppendixAnalysis1.R. You need to pull the branch that records every single person's HIV status (ask Daniel about what commit this is), and you also need to turn on the biomarker_log.csv file recording functionality turned on in the config. 

## Figure 5
Code to generate Figure 5 is in the BARSEmpiricalSurvivorshipCurve.R and ActualPartnershipRunsforJCM.R, in order to analyze this data you need the partnership_events.csv file recording functionality turned on in the config.
