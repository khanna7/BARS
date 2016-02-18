C_SOURCE =  
	

# do not include main here
CPP_SOURCE = Person.cpp \
	Model.cpp \
	Parameters.cpp \
	Stats.cpp \
	DiseaseParameters.cpp \
	CD4Calculator.cpp \
	ViralLoadCalculator.cpp \
	ViralLoadSlopeCalculator.cpp \
	Stage.cpp \
	TransmissionRunner.cpp \
	PersonCreator.cpp \
	ARTScheduler.cpp \
	utils.cpp
	

c_source += $(C_SOURCE)
cpp_source += $(CPP_SOURCE)