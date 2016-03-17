
# Transmission Model Testing
  * The tests below can be run after some simulation time; for now, let's take it as 1825 time steps (5 years) 
  * After small fixes, model will have to be rerun. Those maxtimes can be decided based on the fix. 
  
## Calibration Output parameters   
   (this should perhaps be the last step)  
  * HIV Prevalence: At conclusion of burnin should be approximately equal to the empirically known prevalence  (can be done now)
  * HIV incidence: At conclusion of burnin, annual incidence should be approximately equal to empirically known incidence (can be done now).  
  Both prevalence and annual incidence over time should be plotted.

## Sexual Network parameters
   At conclusion of simulation,  
   * Check mean degree (can be done now)
   * Check degree distribution (need network data at last time step for)
   * Check mean leangth of partnerships (loogging beginning and end of partnerships)
   * Check mixing matrices for specific mixing structures (need network data at last time step for)
   Are simulated values of the above parameters close to the specified input values? 

## Infection Transmission (need logging of individual infection events and "associated" data)
   * For all infected individuals, list times of infection (t_inf_susc). Summarize mean age at infection . 
   * List all transmissitting partners, and their times of infection (t_inf_transm).
   * Check if all t_inf_transm < t_inf_susc. (If not, there is likely a bug.)
   * Note CD4 count, viral load, and ART status of transmitting partners. Check if the three parameters are consistent with t_inf_transm.
   * Classify transmission events by condom use. Do per cent transmissions with/without condoms agree with the specified protection rate of condoms? 
   * Check new infections among PrEP users. (Should be close to zero). 

## Biomarkers (this requires logging vertex attribute status temporally, not necessary for everyone, some subset of agents should be fine)
   * Can we obtain the temporal history of CD4 counts and viral loads?
   * If yes, then we should plot these histories. Time of ART initiation/cessation can be shaded differently. Do these individual trajectories appear to follow the specified rules of evolution? (ART initiation/cessation event logging) 
 
## ART/PrEP parameters (using network data at end)
   * Among infected individuals at conclusion of simulation, how many are on ART? Is this number approximately equal to expected value?
   * Among uninfected individuals, how many are on PrEP? Is this number approximately equal to expected value?
   * Classify ART/PrEP uptake by race. Do these values make sense?
   
## Demographic parameters 
   * Note population size at the conclusion of simulation. Compute annual growth rate. Is it approximately equal to the desired empirical value? (we can do that already)
   * For all deaths over the course of the simulation, how many were due to i) natural mortality, ii) age,  and iii) infection? For infections, how many were on ART? (logging of death events).  

## Other checks
   * Compute the number of new infections caused by each infected person (infector ID's for each individual will be needed). The mean should give a rough estimate for R_0. 
   
