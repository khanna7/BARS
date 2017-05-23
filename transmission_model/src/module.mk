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
	CondomUseAssigner.cpp \
	debug_utils.cpp
	
#	EventWriter.cpp \
	

c_source += $(C_SOURCE)
cpp_source += $(CPP_SOURCE)