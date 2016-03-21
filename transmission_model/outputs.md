# Model Outputs

### Partnership Events

Events are recorded in the file defined by *partnership.events.file* in the model properties file. The format is csv with each row recording an event. The columns are:
* tick: the time step at which the event occurred.
* p1: the id of the first person in the partnership
* p2: the id of the second person in the partnership
* type: the type of partnership event.
  * 0: partnership dissolved
  * 1: partnership added

