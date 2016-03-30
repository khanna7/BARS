# Model Outputs
Note that where the output is written to a file, the model will not overwrite an existing file. It will create a new output file name by appending a number to the file in order to create a file that does not already exist. For example, if counts.csv, counts_1.csv, and counts_2.csv exist, and output is to be written to counts.csv, then the new output will be written to counts_3.csv.

### Partnership Events
Partnership events are recorded in the file defined by *partnership.events.file* in the model properties file. The format is csv with each row recording an event. The columns are:
* tick: the time step at which the event occurred.
* p1: the id of the first person in the partnership
* p2: the id of the second person in the partnership
* type: the type of partnership event.
  * 0: partnership dissolved
  * 1: partnership added
  
### Infection Events
Infection events are recorded in the file defined by *infection.events.file* in the model properties file. The format is csv with each row recording an infection event. An infection event is recorded when one person infects another. An infection event is also recorded when an infected persons enters the model. In that case, only the person 1 attributes are recorded. The columns are:
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
Some number of persons can be selected at the start of the model for biomarker logging. Biomarker logging logs the viral load, cd4 count and art status of each selected person at each time step. The data is recorded in the file defined by the *biomarker.log.file* property in the model.props file. The number of persons to log is defined by the *biomarker.number.of.persons.to.log* property. The persons to log are selected randomly from all persons prior to simulation start. The format is csv with each row logging the data for a particular person. The columns in the output are:
* tick: the time step at which the logging occurred
* p_id: the id of the logged person
* viral_load: the viral load the logged person at time 'tick'
* cd4_cout: the cd4_count of the logged person at time 'tick'
* art_stats: the ART status of the logged person at time 'tick'

### Death Events
Death events are recorded each time a person dies. The event is recorded in the file defined by the *death.events.file* property in the model properties file. The format is csv with each row recording an event. The columns are:
* tick: the time step at which the death occurred
* p_id: the id of the dead person
* age: the age of the dead person
* art_status: the ART status of the dead person
* cause: the cause of death. This is one of:
  * AGE
  * INFECTION
  
### R Network
The network can be saved as a R network image at some specified times. The file into which the network is saved is defined in the *net.save.file* property in the model properties. The file will have the timestep at which the network was saved appended to it. The time steps at which to the save file are defined in the *save.network.at* property. The format for the *save.network.at* property is a comma delimited list of numeric timesteps. Note that 'end' can be used as a timestep value to save the network at the end of the simulation. For example,
```
net.save.file = ./output/network.RDS
save.network.at = 4, end
```
will save the network at timestep 4 and at the end of the simulation, resulting in two files: network_4.RDS and network_N.RDS where N is the last timestep of the simulation. The R function 'saveRDS' is used to save the network. It can be loaded into any variable with 'readRDS'. For example,
```
my.net <- readRDS('../transmission_model/Debug/output/network_4.RDS')
```