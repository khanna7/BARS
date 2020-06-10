# Implementation notes for Model Output 

The general pattern is that data is collected in a struct containing variables representing the data to be collected (e.g. `int infection_count`), and a csv file header for that data.
An individual instance of some struct type can be thought of as a row in a csv file. We
create these structs in the course of recording data and then write them out row by row
into an output file. There are various types of structs for collecting different kinds of output each associated with a different output file. A `Stats` singleton provides global access to the structs or allows a particular struct variable to be incremented via a method.

## Files

The following files are responsible for the collection of model outputs (e.g. infection counts, etc.)

1. Stats.h/cpp
2. StatsWriter.h
3. StatsBuilder.h/cpp

### Stats.h/cpp

The Stats files contain the various structs used for data collection. In general there are two types of structs. Those that record a single particular event (e.g. `ARTEvent` records whenever a person goes on/off ART), and those that record aggregated data such as the 
`Counts` struct. The `Counts` struct is meant to be reset each timestep (a `tick`) back to 0 such that the counts are only for that particular timestep. The various event structs are one off records of a particular event. Some of the structs have additional methods for 
updating their member fields.

The Stats files also contain the `Stats` singleton which provides global access to the
various structs. When we want to record some data produced by the model, we use this `Stats` class. For example, in `Model::runTransmission()`:

```
 if (!person->isInfected()) {
    infectPerson(person, time_stamp);
    stats->currentCounts().incrementInfected(person);
    stats->personDataRecorder()->recordInfection(person, time_stamp, InfectionSource::INTERNAL);
    stats->currentCounts().incrementNewlyInfected();   //infected after burnin     
    ++new_infected_count;
}
```

### StatsWriter.h 

`StatsWriter` is used to write out the struct data instances. Each `StatsWriter` contains a vector of a particular struct type. Additional struct instances (e.g. a `PrepEvent`) are added
to this vector when the event occurs (or on a reset in the case of the `Counts` struct). If the size of the vector is then greater than some threshold, the contents of the vector are written to a file via the `FileOutput` that the `StatsWriter` contains. 

### StatsBuilder.h

`StatsBuilder` is a convience class for instantiating the `Stats` singleton. It provides methods for creating `StatsWriter`s for each data collection struct type, and a method
for creating the `Stats` singleton with some error checking.

