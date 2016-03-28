# Model Outputs
### Partnership Events
Events are recorded in the file defined by *partnership.events.file* in the model properties file. The format is csv with each row recording an event. The columns are:
* tick: the time step at which the event occurred.
* p1: the id of the first person in the partnership
* p2: the id of the second person in the partnership
* type: the type of partnership event.
  * 0: partnership dissolved
  * 1: partnership added
  
### Infection Events
Events are recorded in the file defined by *infection.events.file* in the model properties file. The format is csv with each row recording an infection event. The columns are:
* tick: the time step at which the event occurred.
* p1: the id of the infector (the infected person)
* p1_age: the age of p1 at the time 'tick'
* p1_viral_load: the viral load of p1 at time 'tick'
* p1_cd4: the cd4 count of p1 at time 'tick'
* p1_art_status: the ART status of p1 at time 'tick'. 0 - not on ART; 1 - on ART.
* p1_on_prep: whether or not p2 was on PreP at time 'tick'
* p1_infectivity: the infectivity of p1 at time 'tick'. This does not include PreP or circumcision multipliers
* condom_used: whether or not a condom was used. 0 - no condom was used; 1 - a condom was used.
* p2: the id of the infectee (the uninfected persons who has just been infected)
* p2_age: the age of p2 at time 'tick'
* p2_viral_load: the viral load of p2 at time 'tick'
* p2_cd4: the cd4 count of p2 at time 'tick'
* p2_on_prep: whether or not p2 was on PreP at time 'tick'

### Biomarker logging
Some number of persons can be selected at the start of the model for biomarker logging. Biomarker logging logs the viral load, cd4 count and art status of each selected person at each time step. The data is recorded in the file defined by the *biomarker.log.file* property in the model.props file. The number of persons to log is defined by the *biomarker.number.of.persons.to.log* property. The persons to log are selected randomly from all persons prior to simulation start. The columns in the output are:
* tick: the time step at which the logging occurred
* p_id: the id of the logged person
* viral_load: the viral load the logged person at time 'tick'
* cd4_cout: the cd4_count of the logged person at time 'tick'
* art_stats: the ART status of the logged person at time 'tick'
