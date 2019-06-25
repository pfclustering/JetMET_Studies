all: my_DrawStuff my_analyzer
CC = g++
CFLAGS = -Wall -c -g -fpermissive 

ROOFIT_INCLUDE := $(shell cd $(CMSSW_BASE); scram tool info roofitcore | grep INCLUDE= | sed 's|INCLUDE=||')
ROOFIT_LIBDIR := $(shell cd $(CMSSW_BASE); scram tool info roofitcore | grep LIBDIR= | sed 's|LIBDIR=||')

INCLUDES = -I. -I.. -I$(ROOTSYS)/include -I$(ROOFIT_INCLUDE)/ -I$(CMSSW_BASE)/src -I$(CMSSW_BASE)/src/CommonTools  

ROOTSYS ?= ERROR_RootSysIsNotDefined

ROOTFLAG = `${ROOTSYS}/bin/root-config --cflags --libs`

EXTRALIBS := -L$(ROOTSYS)/lib -L$(ROOFIT_LIBDIR)/ -L$(CMSSW_BASE)/lib/slc6_amd64_gcc491/ -L/cvmfs/cms.cern.ch/slc6_amd64_gcc491/cms/cmssw/CMSSW_7_4_7/lib/slc6_amd64_gcc491/ -lHtml -lMathCore -lGenVector -lMinuit -lEG  -lPhysics -L/cvmfs/cms.cern.ch/slc6_amd64_gcc491/external/zlib/1.2.8-cms/lib -lz -lTreePlayer -lRooFitCore -lRooFit


my_DrawStuff: my_DrawStuff.cpp jseDataset.o jseCommon.o
	$(CC) -Wall $(INCLUDES) -o my_DrawStuff my_DrawStuff.cpp jseDataset.o jseCommon.o 	 $(ROOTFLAG) $(EXTRALIBS)

my_analyzer: my_analyzer.cpp
	$(CC) -Wall $(INCLUDES) -o my_analyzer my_analyzer.cpp 	 $(ROOTFLAG) $(EXTRALIBS)

jseCommon.o: ../interface/jseCommon.h ../src/jseCommon.cc
	$(CC) -Wall $(INCLUDES) -c ../src/jseCommon.cc $(ROOTFLAG) $(EXTRALIBS)

jseDataset.o: ../interface/jseDataset.h ../src/jseDataset.cc
	$(CC) -Wall $(INCLUDES) -c ../src/jseDataset.cc $(ROOTFLAG) $(EXTRALIBS)

