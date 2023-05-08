SYSTEM  ?= $(shell uname -s)
SILICON ?= $(shell uname -m)

# # Provide actual OpenCilk Tapir/LLVM Clang
# # clang version 14.0.6 (https://github.com/OpenCilk/opencilk-project fc90ded2b090672f84c58d12d8d85cd999eb6c1a)
# CXX := ~/.env/custom_compilers/OpenCilk-2.0.0-arm64-Apple-Darwin/bin/clang++
CXXV := $(shell $(CXX) --version | head -n 1)

CXXFLAGS := -I . -std=c++17
#  -O3 -ffast-math
# CXXFLAGS_DEBUG := -Rpass=vector
# # -Rpass-analysis=loop-vectorize -fsave-optimization-record -mllvm -force-vector-width=8 -mllvm -force-vector-interleave=4

# # OpenCilk Clang
# CXXFLAGS += -fopencilk -Wno-backend-plugin

# ifeq ($(SYSTEM), Darwin)
# 	ifeq ($(SILICON), arm64)
# 		CXXFLAGS += -mcpu=apple-m1
# 	else
# 		CXXFLAGS += -march=native
# 	endif
# 	CXXFLAGS += -framework Accelerate
# else
# 	error := $(error Supposed to be run under macOS)
# endif

OBJ := fft_sample

$(info Matrix multiply build info: )
$(info SYSTEM:   $(SYSTEM))
$(info SILICON:  $(SILICON))
$(info CXX:      $(CXX))
$(info CXXV:     $(CXXV))
$(info CXXFLAGS: $(CXXFLAGS))
$(info )

main: fft_sample.cc
	xcrun $(CXX) $(CXXFLAGS) fft_sample.cc -o $(OBJ)

clean:
	rm -f $(OBJ)

default: main
