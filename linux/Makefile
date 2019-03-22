# 
# To build on RaspberryPi:
# 1. Install WiringPi: http://wiringpi.com/download-and-install/
# 2. Run:
#    make target=raspberry
#    sudo blynk --token=YourAuthToken
#

CC ?= gcc
CXX ?= g++
CFLAGS += -I ./
CXXFLAGS += -I ../src/ -I ./ -DLINUX
LDFLAGS += -lrt -lpthread

ifeq ($(build),debug)
	CXXFLAGS += -c -g2 -O0 \
		-Wall -Weffc++ \
		-Wextra -Wcast-align \
		-Wchar-subscripts  -Wcomment -Wconversion \
		-Wdisabled-optimization \
		-Wfloat-equal  -Wformat  -Wformat=2 \
		-Wformat-nonliteral -Wformat-security  \
		-Wformat-y2k \
		-Wimplicit  -Wimport  -Winit-self  -Winline \
		-Winvalid-pch   \
		-Wunsafe-loop-optimizations  -Wlong-long -Wmissing-braces \
		-Wmissing-field-initializers -Wmissing-format-attribute   \
		-Wmissing-include-dirs -Wmissing-noreturn \
		-Wpacked  -Wparentheses  -Wpointer-arith \
		-Wredundant-decls -Wreturn-type \
		-Wsequence-point  -Wshadow -Wsign-compare  -Wstack-protector \
		-Wstrict-aliasing -Wstrict-aliasing=2 -Wswitch  -Wswitch-default \
		-Wswitch-enum -Wtrigraphs  -Wuninitialized \
		-Wunknown-pragmas  -Wunreachable-code -Wunused \
		-Wunused-function  -Wunused-label  \
		-Wunused-value  -Wunused-variable \
		-Wvolatile-register-var  -Wwrite-strings
	
	# Disable some warnings
	CXXFLAGS += -Wno-variadic-macros -Wno-unused-parameter -Wno-vla
	
	# Uncomment this to get pedantic warnings:
	#CXXFLAGS += -pedantic -Wvariadic-macros -Wunused-parameter -Waggregate-return -Wcast-qual -Wpadded
else
	CXXFLAGS += -c -O3 -w
	LDFLAGS += -s
endif

ifeq ($(target),raspberry)
	CXXFLAGS += -DRASPBERRY
	LDFLAGS += -lwiringPi
endif

CXX_SOURCES=main.cpp \
	../src/utility/BlynkDebug.cpp \
	../src/utility/BlynkHandlers.cpp \
	../src/utility/BlynkTimer.cpp

CXX_OBJECTS=$(CXX_SOURCES:.cpp=.o)

C_SOURCES=roof_client.c
C_OBJECTS=$(C_SOURCES:.c=.o)

EXECUTABLE=blynkClient

all: $(CXX_SOURCES) $(C_SOURCES) $(EXECUTABLE)

clean:
	-rm -f $(CXX_OBJECTS) $(C_OBJECTS) $(EXECUTABLE)

$(EXECUTABLE): $(CXX_OBJECTS) $(C_OBJECTS)
	$(CXX) $(CXX_OBJECTS) $(C_OBJECTS) $(LDFLAGS) -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
