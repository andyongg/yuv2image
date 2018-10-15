# C source code
CSRC	= 

# C++ source code
CXXSRC	= main.cpp \
          resize_4x.cpp \
          yuv2rgb.cpp

# all source code
SRC	= $(CSRC) $(CXXSRC)

# C objects
COBJ	= $(CSRC:.c=.o)
# C++ objects
CXXOBJ	= $(CXXSRC:.cpp=.o)
# all objects
OBJ	= $(COBJ) $(CXXOBJ)
# binary target
BIN	= yuv2image

# C optimization flags
COPT	= -O3 -ftree-vectorize -funroll-loops

# C++ optimization flags
CXXOPT	= $(COPT)

# C compilation flags
CFLAGS	= $(COPT) -Wall -Wextra \
	-Wno-write-strings -ansi
# C++ compilation flags
CXXFLAGS	= $(CXXOPT) -Wall -Wextra \
	-Wno-write-strings -Wno-deprecated -ansi
# link flags
LDFLAGS	= -lopencv_core -lopencv_imgcodecs -lopencv_imgproc

# partial compilation of C source code
%.o: %.c %.h
	$(CC) -c -o $@  $< $(CFLAGS)
# partial compilation of C++ source code
%.o: %.cpp %.h
	$(CXX) -c -o $@  $< $(CXXFLAGS)

# link all the object code
$(BIN): $(OBJ) $(LIBDEPS)
	$(CXX) -o $@ $(OBJ) $(LDFLAGS)