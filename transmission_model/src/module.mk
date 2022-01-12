C_SOURCE =  
    

# do not include main here
CPP_SOURCE = Person.cpp \
    Model.cpp \
    Parameters.cpp \
    Stats.cpp \
    StatsBuilder.cpp \
    DiseaseParameters.cpp \
    CD4Calculator.cpp \
    ViralLoadCalculator.cpp \
    ViralLoadSlopeCalculator.cpp \
    Stage.cpp \
    TransmissionRunner.cpp \
    PersonCreator.cpp \
    ARTScheduler.cpp \
    FileOutput.cpp \
    utils.cpp \
    file_utils.cpp \
    PersonDataRecorder.cpp \
    GeometricDistribution.cpp \
    DayRangeCalculator.cpp \
    RangeWithProbability.cpp \
    AdherenceCheckScheduler.cpp \
    adherence_functions.cpp \
    PrepParameters.cpp \
    PrepCessationEvent.cpp \
    CondomUseAssigner.cpp \
    debug_utils.cpp \
    Diagnoser.cpp \
    ARTLagCalculator.cpp \
    TestingConfigurator.cpp \
    PREPAdherenceConfigurator.cpp \
    Range.cpp \
    PrepInterventionManager.cpp \
    BasePrepIntervention.cpp \
    SerodiscordantPrepIntervention.cpp \
    NetStatPrepIntervention.cpp \
    RandomSelectionPrepIntervention.cpp \
    PrepFilter.cpp \
    Logger.cpp \
    Jail.cpp \
    JailEvents.cpp \
    CSVReader.cpp \
    JailInfectionRateCalc.cpp \
    Serializer.cpp \
    PersonToVAL.cpp \
    JailIntervention.cpp \
    MethUse.cpp \
    MethUseCessationEvent.cpp
    

c_source += $(C_SOURCE)
cpp_source += $(CPP_SOURCE)

# IncrementingPrepUptakeManager.cpp \
# PrepUptakeManager.cpp \
# ProportionalPrepUptakeManager.cpp \
# SerodiscordantPrepUptakeManager.cpp \
# NetStatPUManager.cpp \
# PUBase.cpp \
# PUExtra.cpp \
