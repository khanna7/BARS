CXXFLAGS = -g -O0 -fPIC -std=c++11

INCLUDES :=
LIBS :=
RPATHS :=

R_HOME = /Library/Frameworks/R.framework/Versions/3.2/Resources
#R_USER_LIBS = ~/Library/R/3.2/library
R_USER_LIBS = /Library/Frameworks/R.framework/Versions/3.2/Resources/library

INCLUDES += -I $(R_HOME)/include
INCLUDES += -I $(R_USER_LIBS)/RInside/include
INCLUDES += -I $(R_USER_LIBS)/Rcpp/include
INCLUDES += -I ./network_src

LIBS += -L $(PWD)/network_src -l network_access
LIBS += -L $(R_USER_LIBS)/RInside/lib -l RInside
LIBS += -L $(R_HOME)/lib -l R

RPATHS += -Wl,-rpath -Wl,$(R_USER_LIBS)/RInside/lib
RPATHS += -Wl,-rpath -Wl,$(R_HOME)/lib/R/lib
RPATHS += -Wl,-rpath -Wl,$(PWD)/network_src

OBJS = rinside_example.o 

all: rinside_example.x

rinside_example.x: $(OBJS)
	g++ -o $(@)  $(OBJS) $(LIBS) $(RPATHS) 

%.o: %.cpp 
	g++ -c $(INCLUDES) $(CXXFLAGS) $(<)

clean:
	rm -fv *.x *.o
