# Transmission Model Components #

## Internal Disease Progression ##

Updating viral load and cd4 counts - ```Model::updateVitals```

* class Person - Person.h,cpp
* struct SharedViralLoadParameters - DiseaseParameters.h
* struct InfectionParameters - DiseaseParameters.h
* class ViralLoadCalculator - ViralLoadCalculator.h,cpp
* class ViralLoadSlopeCalculator - ViralLoadSlopeCalculator.h,cpp
* class CD4Calculator - CD4Calculator.h,cpp


## Transmission ##

Transmitting the disease through the network - ```Model::runTransmission```

* class Person - Person.h,cpp
* class TransmissionRunner - TransmissionRunner.h,cpp


## Testing and Diagnosis ##

Setting testing schedule and PrEP/ART adherence during initialization - ```Model::Model```

* initialize_network - network_utils.h
* class PersonCreator - PersonCreator.h,cpp
* class TestingConfigurator - TestingConfigurator.h,cpp
* class PREPAdherenceConfigurator - PREPAdherenceConfigurator.h,cpp
* class ARTLagCalculator - ARTLagCalculator.h,cpp

Testing persons for HIV and diagnosis (including starting ART) - ```Model::updateVitals``` - 

* class Person - Person.h,cpp
* class Diagnoser - Diagnoser.h,cpp
  

## PrEP Interventions ##

Initializing Interventions - ```Model::Model()```

* initialize_prep_interventions - Model.cpp

Running Interventions - ```Model::updateVitals```

* class Person - Person.h,cpp
* class IPrepIntervention - PrepInterventionManager.h,cpp
* class PrepIntervention - PrepInterventionManager.h,cpp
* class PrepInterventionManager - PrepInterventionManager.h,cpp
* class RandomSelectionPrepIntervention - RandomSelectionPrepIntervention.h,cpp
* class SerodiscordantPrepIntervention - SerodiscordantPrepIntervention.h,cpp
* class NetStatPrepIntervention - NetStatPrepIntervention.h,cpp
* class CompositeNetStatPrepIntervention - NetStatPrepIntervention.h,cpp
* class NetworkStats - NetworkStats.h













