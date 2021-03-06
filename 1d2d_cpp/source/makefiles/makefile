#!/bin/sh
#
# Makefile for OSHUN
# This should work with gcc-6, open-mpi compiled with gcc-6, and HDF5 compiled with c++11 standards

COMPILER = g++-8

# GNU compilers#
DBGFLAGS = -v -O0 -Wall -Wuninitialized -Wextra -Wpedantic
OPTFLAGS3 = -O3 -ffast-math -ftree-loop-distribution -fno-signed-zeros -ftree-loop-ivcanon -ftree-vect-loop-version
# -O3 -ffast-math -ftree-loop-distribution -fno-signed-zeros -ftree-loop-ivcanon -ftree-vect-loop-version
#-fopt-info-vec-optimized  
# INTEL compilers
#OPTFLAGS3 = -O3
#OPTFLAGS3 = -g -O0 -Wall

# HDF5 libraries
H5LIB = -lhdf5_hl -lhdf5
MPILIB = -lmpi
LDFLAGS = 
CPPFLAGS = -fopenmp -DOSHUN_VERSION=\"$(GIT_VERSION)\"
INCLUDE_DIRS = 

EXEC_DIR = ../bin
TMP_DIR = ../bin/tmp

OBJECTS = input.o clock.o state.o stepper.o formulary.o parser.o setup.o vlasov.o nmethods.o functors.o collisions.o fluid.o implicitE.o parallel.o export.o main.o

EXEC = oshun.e

GIT_VERSION := $(shell git describe --abbrev=4 --dirty --always --tags)

BUILDOBJECTS = ${addprefix ${TMP_DIR}/,${OBJECTS}}

BUILDEXEC = ${addprefix ${EXEC_DIR}/,${EXEC}}

${BUILDEXEC} : ${BUILDOBJECTS}
	-${COMPILER} ${CPPFLAGS} ${OPTFLAGS2} ${INCLUDE_DIRS} ${LDFLAGS} $(H5LIB) $(MPILIB) -o ${BUILDEXEC}  \
	${BUILDOBJECTS} 

clean ::
	rm -f ${BUILDOBJECTS}
	rm -f ${BUILDEXEC}
	rm -f ${TMP_DIR}/${BUILDOBJECTS}

debug :
	${COMPILER} ${DBGFLAGS} -o ${BUILDEXEC} \
	%.cpp

${TMP_DIR}/parser.o: parser.cpp
	$(COMPILER) ${CPPFLAGS} $(DBGFLAGS) $(INCLUDE_DIRS) -c -o $@ $<		

${TMP_DIR}/input.o: input.cpp
	$(COMPILER) ${CPPFLAGS} $(DBGFLAGS) $(INCLUDE_DIRS) -c -o $@ $<

${TMP_DIR}/stepper.o: stepper.cpp
	$(COMPILER) ${CPPFLAGS} $(DBGFLAGS) $(INCLUDE_DIRS) -c -o $@ $<

${TMP_DIR}/clock.o: clock.cpp
	$(COMPILER) ${CPPFLAGS} $(DBGFLAGS) $(INCLUDE_DIRS) -c -o $@ $<

${TMP_DIR}/state.o: state.cpp
	$(COMPILER) ${CPPFLAGS} $(DBGFLAGS) $(INCLUDE_DIRS) -c -o $@ $<	

${TMP_DIR}/formulary.o: formulary.cpp
	$(COMPILER) ${CPPFLAGS} $(DBGFLAGS) $(INCLUDE_DIRS) -c -o $@ $<	

${TMP_DIR}/setup.o: setup.cpp
	$(COMPILER) ${CPPFLAGS} $(DBGFLAGS) $(INCLUDE_DIRS) -c -o $@ $<	

${TMP_DIR}/vlasov.o: vlasov.cpp
	$(COMPILER) ${CPPFLAGS} $(DBGFLAGS) $(INCLUDE_DIRS) -c -o $@ $<	

${TMP_DIR}/nmethods.o: nmethods.cpp
	$(COMPILER) ${CPPFLAGS} $(OPTFLAGS2) $(INCLUDE_DIRS) -c -o $@ $<

${TMP_DIR}/functors.o: functors.cpp
	$(COMPILER) ${CPPFLAGS} $(DBGFLAGS) $(INCLUDE_DIRS) -c -o $@ $<

${TMP_DIR}/collisions.o: collisions.cpp
	$(COMPILER) ${CPPFLAGS} $(DBGFLAGS) $(INCLUDE_DIRS) -c -o $@ $<

${TMP_DIR}/fluid.o: fluid.cpp
	$(COMPILER) ${CPPFLAGS} $(DBGFLAGS) $(INCLUDE_DIRS) -c -o $@ $<

${TMP_DIR}/implicitE.o: implicitE.cpp
	$(COMPILER) ${CPPFLAGS} $(DBGFLAGS) $(INCLUDE_DIRS) -c -o $@ $<

${TMP_DIR}/parallel.o: parallel.cpp
	$(COMPILER) ${CPPFLAGS} $(DBGFLAGS) $(INCLUDE_DIRS) -c -o $@ $<

${TMP_DIR}/export.o: export.cpp
	$(COMPILER) ${CPPFLAGS} $(DBGFLAGS) $(INCLUDE_DIRS) -c -o $@ $<

${TMP_DIR}/main.o: main.cpp
	$(COMPILER) ${CPPFLAGS} $(DBGFLAGS) $(INCLUDE_DIRS) -c -o $@ $<		
	#
