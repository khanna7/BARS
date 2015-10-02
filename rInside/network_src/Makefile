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
INCLUDES += -I .

LIBS += -L $(R_USER_LIBS)/RInside/lib -l RInside 
LIBS += -L $(R_HOME)/lib -l R

RPATHS += -Wl,-rpath -Wl,$(R_USER_LIBS)/RInside/lib
RPATHS += -Wl,-rpath -Wl,$(R_HOME)/lib/R/lib

OBJS =  ./access.o ./utils.o

all: libnetwork_access.dylib

libnetwork_access.dylib: $(OBJS)
	clang -dynamiclib -o $(@) $(OBJS) $(LIBS) $(RPATHS)

%.o: %.c 
	clang -c $(INCLUDES) -g -O0 -fPIC $(<)

clean:
	rm -fv *.x *.o
