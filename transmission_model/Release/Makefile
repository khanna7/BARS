c_source :=
cpp_source :=

test_src :=

CXX = mpicxx
CXXLD = mpicxx
CC = mpic

CXX_RELEASE_FLAGS = -Wall -O2 -g -std=c++11 -MMD -MP
CXX_DEBUG_FLAGS = -Wall -O0 -g3 -std=c++11 -MMD -MP

CXX_FLAGS = $(CXX_RELEASE_FLAGS)

INCLUDES :=
LIBS :=
RPATHS :=

R_HOME = /software/R-3.4.1-el7-x86_64/lib64/R
#R_USER_LIBS = ~/Library/R/3.2/library
R_USER_LIBS = $(HOME)/R/x86_64-pc-linux-gnu-library/3.4

REPAST_LIB_DIR = /home/ntcollie/midway2/sfw/repast_hpc-2.2.0/lib
REPAST_LIB = repast_hpc-2.2.0
REPAST_INCLUDE = /home/ntcollie/midway2/sfw/repast_hpc-2.2.0/include

BOOST_INCLUDE = /home/ntcollie//midway2/sfw/Boost/Boost_1.61/include
BOOST_LIB_DIR = /home/ntcollie//midway2/sfw/Boost/Boost_1.61/lib
BOOST_LIBS = -lboost_mpi-mt -lboost_system-mt -lboost_filesystem-mt -lboost_serialization-mt -lboost_timer-mt -lboost_chrono-mt

SFW=/home/ntcollie/midway2/sfw
IGRAPH_INCLUDE = $(SFW)/igraph-0.7.1/include
IGRAPH_LIB_DIR = $(SFW)/igraph-0.7.1/lib
IGRAPH_LIB = igraph

NET_CDF_CXX_LIB_DIR = /home/ntcollie/midway2/sfw/NetCDF-cxx/lib/
NET_CDF_CXX_LIB = netcdf_c++

NET_CDF_LIB_DIR = /home/ntcollie/midway2/sfw/NetCDF/lib/
NET_CDF_LIB = netcdf

INCLUDES += -I $(R_HOME)/include
INCLUDES += -I $(R_USER_LIBS)/RInside/include
INCLUDES += -I $(R_USER_LIBS)/Rcpp/include
INCLUDES += -I $(REPAST_INCLUDE)
INCLUDES += -I $(BOOST_INCLUDE)
INCLUDES += -I ../src
INCLUDES += -I $(IGRAPH_INCLUDE)

GTEST_HOME = /Users/nick/bin/googletest
GTEST_LIB = $(GTEST_HOME)/libgtest.a
TEST_INCLUDES = $(INCLUDES) -I $(GTEST_HOME)/include

LIBS += -L $(R_USER_LIBS)/RInside/lib -l RInside
LIBS += -L $(R_HOME)/lib -l R
LIBS += -L $(REPAST_LIB_DIR) -l$(REPAST_LIB)
LIBS += -L $(BOOST_LIB_DIR) $(BOOST_LIBS)
LIBS += -L $(IGRAPH_LIB_DIR) -l$(IGRAPH_LIB)
LIBS += -L $(NET_CDF_LIB_DIR) -l$(NET_CDF_LIB)
LIBS += -L $(NET_CDF_CXX_LIB_DIR) -l$(NET_CDF_CXX_LIB)


RPATHS += -Wl,-rpath -Wl,$(R_USER_LIBS)/RInside/lib
RPATHS += -Wl,-rpath -Wl,$(R_HOME)/lib/R/lib
RPATHS += -Wl,-rpath -Wl,$(BOOST_LIB_DIR)
RPATHS += -Wl,-rpath -Wl,$(REPAST_LIB_DIR)
RPATHS += -Wl,-rpath -Wl,$(NET_CDF_LIB_DIR)
RPATHS += -Wl,-rpath -Wl,$(NET_CDF_CXX_LIB_DIR)
RPATHS += -Wl,-rpath -Wl,$(IGRAPH_LIB_DIR)

SRC_DIR=../src
BUILD_DIR = ./build

include ../src/module.mk
include ../test/module.mk

# objects used by both executable and tests
OBJECTS :=
OBJECTS += $(subst .cpp,.o, $(addprefix $(BUILD_DIR)/, $(cpp_source)))
OBJECTS += $(subst .c,.o, $(addprefix $(BUILD_DIR)/, $(c_source)))

# objects and deps for executable
EXEC_OBJECTS := $(OBJECTS)
EXEC_OBJECTS += $(BUILD_DIR)/main.o

TEST_OBJECTS := $(OBJECTS)
TEST_OBJECTS += $(subst .cpp,.o, $(addprefix $(BUILD_DIR)/, $(test_src)))
TEST_OBJECTS += $(BUILD_DIR)/test_main.o

VPATH = ../src ../test

VERSION = $(shell head ./version.txt)
NAME = transmission_model-$(VERSION)
TEST_NAME = unit_tests

SED := sed
MV := mv -f

-include $(TEST_OBJECTS:.o=.d)
-include $(EXEC_OBJECTS:.o=.d)

.PHONY: all transmission_model tests clean

all: transmission_model tests

transmission_model : $(EXEC_DEPS) $(EXEC_OBJECTS)
	$(CXXLD) $(filter-out %.d, $^)  $(LIBS) $(RPATHS) -o $(NAME)

tests : $(TEST_DEPS) $(TEST_OBJECTS)
	$(CXXLD) $(filter-out %.d, $^) $(LIBS) $(RPATHS) $(GTEST_LIB) -o $(TEST_NAME)

$(BUILD_DIR)/%.o : %.cpp
	@-mkdir -p $(dir $@)
	$(CXX) $(CXX_FLAGS) $(INCLUDES) $(TEST_INCLUDES) -c $< -o $@


clean:
	rm -fv $(NAME) $(TEST_NAME)
	rm -rf $(BUILD_DIR)/*
