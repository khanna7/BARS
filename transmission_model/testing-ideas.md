
# Transmission Model Testing
  * The tests below can be run after some simulation time; for now, let's take it as 1825 time steps (5 years) 
  * After small fixes, model will have to be rerun. Those maxtimes can be decided based on the fix. 
  
## Calibration Output parameters   
   (this should perhaps be the last step)  
  * HIV Prevalence: At conclusion of burnin should be approximately equal to the empirically known prevalence  
  * HIV includence: At conclusion of burnin, annual incidence should be approximately equal to empirically known incidence.  
  Both prevalence and annual incidence over time should be plotted.

## Infection Transmission
   * For all infected individuals, list times of infection. Summarize mean age at infection (t_inf_susc). C
   * List all transmissitting partners, and their times of infection (t_inf_transm).
   * Check if all t_inf_transm > t_inf_susc. (If not, there is likely a bug.)
   * Note CD4 count, viral load, and ART status of transmitting partners. Check if the three parameters are consistent with t_inf_transm.

## Biomarkers
   * Can we obtain the temporal history of CD4 counts and viral loads?
   * If yes, then we should plot these histories. Time of ART initiation/cessation can be shaded differently. Do these individual trajectories appear to follow the specified rules of evolution? 
