C_SOURCE = access.c \
	utils.c 
	

# do not include main here
CPP_SOURCE = Person.cpp \
	RNetwork.cpp \
	Model.cpp \
	Parameters.cpp \
	nd_spell_functions.cpp \
	spell_functions.cpp

c_source += $(C_SOURCE)
cpp_source += $(CPP_SOURCE)