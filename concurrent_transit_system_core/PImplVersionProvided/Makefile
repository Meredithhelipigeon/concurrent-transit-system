CXX = u++					# compiler
#CXXFLAGS = -std=c++17 -g -multi -Wall -Wextra -MMD 
CXXFLAGS = -std=c++17 -multi -Wall -Wextra -MMD -O2 
MAKEFLAGS = --no-print-directory
MAKEFILE_NAME = ${firstword ${MAKEFILE_LIST}}	# makefile name

OBJECTS = watcard.o printer.o timer.o nameserver.o trainstop.o train.o conductor.o student.o parent.o groupoff.o cardoffice.o bank.o config.o main.o
EXEC = lrt

DEPENDS = ${OBJECTS:.o=.d}			# substitute ".o" with ".d"

.PHONY : clean

${EXEC} : ${OBJECTS}
	${CXX} ${CXXFLAGS} $^ -o $@

${OBJECTS} : ${MAKEFILE_NAME}			# OPTIONAL : changes to this file => recompile

-include ${DEPENDS}

clean :						# remove files that can be regenerated
	rm -f ${DEPENDS} ${OBJECTS} ${EXEC} ${EXEC}.out

test :
	${MAKE} ${EXEC}
	rm -f ${EXEC}.out
	${EXEC}.mark ${EXEC}
