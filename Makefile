################################################
# Two changes are needed.
# (1) Add your player's .o file to CONTESTOBJECTS
# (2) Add your player's dependencies right after DumbPlayerV2's.
################################################

CXXFLAGS = -g -Wall -Og -std=c++11
CXX = g++


################################################
# Change 1:
# Add your player on the line after GamblerPlayer 
################################################
CONTESTOBJECTS = AIContest.o BoardV3.o Message.o PlayerV2.o conio.o contest.o \
	DumbPlayerV2.o CleanPlayerV2.o GamblerPlayerV2.o LearningGambler.o SemiSmartPlayerV2.o myPlayer.o HitAll.o

TESTEROBJECTS = AIContest.o BoardV3.o Message.o PlayerV2.o conio.o tester.o \
	DumbPlayerV2.o CleanPlayerV2.o GamblerPlayerV2.o LearningGambler.o SemiSmartPlayerV2.o myPlayer.o HitAll.o

HOST_NAME := $(shell hostname)
HOST_OK := no

$(info Checking that you are on matt or john ...)
ifeq ($(HOST_NAME),matt.cse.taylor.edu)
    $(info You are on matt. Good, carry on.)
    HOST_OK = yes
endif

ifeq ($(HOST_NAME),john)
    $(info You are on john. Good, carry on.)
    HOST_OK = yes
endif
HOST_OK = yes
ifneq ($(HOST_OK),yes)
    $(error Compile code on matt or john, or else hack this Makefile if you know what you are doing.)
endif

instructions:
	@echo "Make options: contest, testAI, clean, tar"

contest: $(CONTESTOBJECTS)
	g++ -o contest $(CONTESTOBJECTS)
	@echo "Contest binary is in 'contest'. Run as './contest'"

testAI: $(TESTEROBJECTS) 
	g++ -o testAI $(TESTEROBJECTS)
	@echo "Tester is in 'testAI'. Run as './testAI'"

clean:
	rm -f contest $(CONTESTOBJECTS) $(TESTEROBJECTS)

tar:
	g++ -c -Wall -O2 CleanPlayerV2.cpp
	g++ -c -Wall -O2 SemiSmartPlayerV2.cpp
	g++ -c -Wall -O2 GamblerPlayerV2.cpp
	g++ -c -Wall -O2 BoardV3.cpp
	tar -cvf binaries.tar CleanPlayerV2.o GamblerPlayerV2.o BoardV3.o SemiSmartPlayerV2.o

contest.o: contest.cpp
contest.cpp: defines.h Message.cpp

tester.o: tester.cpp
tester.cpp: defines.h Message.cpp

Message.o: Message.cpp defines.h
Message.cpp: Message.h defines.h

conio.o: conio.cpp
conio.cpp: conio.h

AIContest.o: AIContest.cpp
AIContest.cpp: AIContest.h Message.h defines.h

# Players here
DumbPlayerV2.o: DumbPlayerV2.cpp Message.cpp
DumbPlayerV2.cpp: DumbPlayerV2.h defines.h PlayerV2.h conio.cpp

HitAll.o: HitAll.cpp Message.cpp
HitAll.cpp: HitAll.h defines.h PlayerV2.h conio.cpp

myPlayer.o: myPlayer.cpp Message.cpp
myPlayer.cpp: myPlayer.h defines.h PlayerV2.h conio.cpp
################################################
# Change 2:
# Add your player dependency information below
################################################


# CleanPlayerV2.o and other provided binaries are only available as a linkable Linux binary, not as source code.
CleanPlayerV2.o: 
	tar -xvf binaries.tar CleanPlayerV2.o
GamblerPlayerV2.o:
	tar -xvf binaries.tar GamblerPlayerV2.o
LearningGambler.o: 
	tar -xvf binaries.tar LearningGambler.o
SemiSmartPlayerV2.o: 
	tar -xvf binaries.tar SemiSmartPlayerV2.o
BoardV3.o:
	tar -xvf binaries.tar BoardV3.o
