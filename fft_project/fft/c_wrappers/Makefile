SYSTEM  ?= $(shell uname -s)
SILICON ?= $(shell uname -m)


CXXV := $(shell $(CXX) --version | head -n 1)

CXXFLAGS := -I . -std=c++17 -O3 -ffast-math -Rpass=vector -DCXX_MEASURE_TIME -lpthread

OBJ := fft.o

$(info Matrix multiply build info: )
$(info SYSTEM:   $(SYSTEM))
$(info SILICON:  $(SILICON))
$(info CXX:      $(CXX))
$(info CXXV:     $(CXXV))
$(info CXXFLAGS: $(CXXFLAGS))
$(info )

main: fft.cpp
	xcrun $(CXX) $(CXXFLAGS) fft.cpp -o $(OBJ)

clean:
	rm -f $(OBJ)

default: main
