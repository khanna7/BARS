# BARS
code for collaboration on Houston Medicaid Expansion project
## Medicaid Expansion Model in Houston
Code for unpublished _Medicaid Expansion Paper_ [paper](https://uchicago.app.box.com/file/951786888664)

## Data Generation
 - Can be done using executable v0.72

## Unrolled Parameter Files (UPFs) for Data Generation
 - R files to generate the UPFs for the calibration and each of the interventions are [here](https://github.com/khanna7/BARS/blob/houston-paper/transmission_model/swift_proj/data/MedicaidUPF.R).
 - The code produces output in txt format that is read by swift, and in csv format for easy readability. Source it in order to produce the appropriate upf.

## Data Analysis 
 - When the calibration is run, use the code from [here](https://github.com/khanna7/BARS/blob/houston-paper/transmission_model/swift_proj/R/MakeCalibrationPlots.R) to reproduce the calibration plot.
 - When the experiments are run, use the code from [here](https://github.com/khanna7/BARS/blob/houston-paper/transmission_model/swift_proj/R/inc_sp_bma-Medicaid.R) to reproduce all the plots.
 - Source the function and it should produce all the objects required to replicate the analysis. (Incidence, Prevalence, Cases, PrEP and ART Prevalence)
