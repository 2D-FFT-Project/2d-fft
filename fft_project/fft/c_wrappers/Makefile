SYSTEM  ?= $(shell uname -s)
SILICON ?= $(shell uname -m)

CXXFLAGS := -I . -std=c++17 -ffast-math -Rpass=vector -DCXX_MEASURE_TIME 

ifdef USE_CILK
	CXXFLAGS += -fopencilk -DUSE_CILK -O3
	CXX := /Users/gyzavyalov/.env/custom_compilers/OpenCilk-2.0.0-arm64-Apple-Darwin/bin/clang++
else
	ifdef USE_THREAD
		ifdef P_THRESHOLD
			CXXFLAGS += -DP_THRESHOLD=$(P_THRESHOLD)
		endif
		CXXFLAGS += -DUSE_THREAD -lpthread
	endif
	CXXFLAGS += -O3
endif

OBJ := fft.o

CXXV := $(shell $(CXX) --version | head -n 1)
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
