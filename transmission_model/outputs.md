# Model Outputs

The location of the model output (csv files etc.) is determined by the *output.directory* property in the model properties file. The individual output described below will be written into this directory using the file names specifed for each file. For example,

```
output.directory = /home/nick/Documents/BARS/output
partnership.events.file = partnership_events.csv
```
will write the partnership event output data (see below) to a /home/nick/Documents/BARS/output/partnership_events.csv file.

If the model input parameters include a *run* parameter then the run number will be appended to the *output.directory*. For example,

```
run = 1
output.directory = /home/nick/Documents/BARS/output
partnership.events.file = partnership_events.csv
```
will write the partnership event output data to a /home/nick/Documents/BARS/output_1/partnership_events.csv file. The expectation is that the mechanism that performs the model parameter exploration will pass a *run* parameter that identifies each model run and that the output of that run will be written into the appropriately named output directory.

Note that where the output is written to a file, the model will not overwrite an existing file. It will create a new output file name by appending a number to the file in order to create a file that does not already exist. For example, if counts.csv, counts_1.csv, and counts_2.csv exist, and output is to be written to counts.csv, then the new output will be written to counts_3.csv.

### parameters.txt
Parameters.txt contains the parameter values for a model run. It is written out immediately after all the [model inputs](inputs.md) have been loaded. 

### Per timestep aggregate data
The aggregate data consist of various aggregate per time step stats (e.g. the total number of persons infected during a timestep). These are recorded in the file defined by *per.tick.counts.output.file* in the model properties file. The format is csv with each row recording the stats for that timestep. The columns are:
* tick: the timestep at which the stats were generated
* entries: the number of persons who entered the simulation at that timestep
* old_age_deaths: the number of persons who died from old age (person's age > max age) at that timestep
* infection_deaths: the number of persons who died from infection at that timestep
* infected_via_transmission: the number of persons infected via transmission at that timestep
* infected_at_entry: the number of persons who were infected when entering the model
* uninfected: the number of uninfected persons at that timestep. This includes uninfected entering persons and uninfected persons who have died during this timestep
* main_edge_count: the number of edges in the main network in the model at the end of the timestep
* casual_edge_count: the number of edges in the casual network in the model at the end of the timestep
* vertex_count: the total number of vertices at the end of the timestep. This takes into account adding entries and subtracting deaths.
* overlaps: the number of edges that occur in both the main network and the casual network. This is determined by iterating through the edges in the network with the smallest edge count, and incrementing the count if the same edge exists in the other network. Edges are non-directed so the count is incremented if v1 -> v2 or v2 -> v1 exists. The iteration is a relatively expensive operation and it can be turned off by setting the model property *count.overlaps* to false. 

### Partnership Events
Partnership events are recorded in the file defined by *partnership.events.file* in the model properties file. The format is csv with each row recording an event. The columns are:
* tick: the time step at which the event occurred.
* p1: the id of the first person in the partnership
* p2: the id of the second person in the partnership
* type: the type of partnership event.
  * 0: partnership dissolved
  * 1: partnership added
* network_type: the type of network in which the event occurred
  * 0: the main network
  * 1: the casual network
  
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
* network_type: the type of network in which the event occurred
  * 0: the main network
  * 1: the casual network

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

### Person Data
Person data recording collects the following attributes for each person in the model. The file in which the data is saved is defined by the *person.data.file* model property. The format is csv with one row per person.
* id: the id of the person
* time of entry: the timestamp at which the person entred the model. This is 0 for persons entering during intitialization.
* time of death: the timestamp at which the person died. If the person has not died by the end of the simulation this will be -1.
* infection status: the infection status of the person at death or simulation end.
  * 0: uninfected
  * 1: infected
* time of infection: the timestamp at which the person was infected. If never infected, then -1.
* art status: The ART status of the person at death or simulation end.
  * 0: not on ART
  * 1: on ART
* time of art initiation: the timestamp when the person when on ART. If never on ART then -1.
* time of art cessation: the timestamp when the person went off of ART. If never went off of ART then -1.
* prep status: The PrEP status of the person at death or simulation end
  * 0: not on PrEP
  * 1: not on PrEP after being on PrEP and then diagnosed as positive
  * 2: on PrEP
* time of prep initiation: the timestep when the person went on PrEP. If never on PrEP, then -1.
* time of prep cessation: the timestep when the person went off of PrEP. If never went off PrEP then -1.
* number of tests: the number of diagnostic tests the person underwent
* time since last test: the time since the person was last tested. If never tested, then -1.
* diagnosis status: whether or not the person has been diagnosed.
  * 0: undiagnosed
  * 1: diagnosed

### Testing Events
Testing events are recorded in the file defined by the *testing.events.file* property. Each time a 
person is tested an event is recorded. The format is csv with one row per event.
* tick: the time at which the testing occurred
* p_id: the id of the person tested
* result: the result of the test.
  * 0: negative test
  * 1: positive test

