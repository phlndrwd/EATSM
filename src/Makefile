# Source directories separated by space
# Example ./ src1/ src2/
SRCDIR=./ data/ io/ model/ param/ tools/
# Directory where object files will be placed
OBJDIR=../build/
# Include directories separated by space
# Example: include1/ include2/
INCDIR=./ data/ io/ model/ param/ tools/
# Directory where binary file will be placed
BINDIR=../dist/
# Name of the result file
TARGET=eatsm
# Compiler
CXX=g++

# Retrive list of the source files
SRC=$(wildcard $(addsuffix *.cpp,$(SRCDIR)))
# Generate list of the object files
OBJ=$(addprefix $(OBJDIR), $(patsubst %.cpp, %.o, $(notdir $(SRC))))

VPATH=$(SRCDIR)

# Compilation flags
CXXFLAGS=-Wall -g

$(TARGET) : $(OBJ)
	@echo Linking...
	@mkdir -p $(BINDIR)
	@$(CXX) $(CXXFLAGS) -o $(BINDIR)$@ $(OBJ)
	@echo EATSM compilation complete.

$(OBJDIR)%.o : %.cpp
	@echo Compiling $< in $@...
	@mkdir -p $(OBJDIR)
	@$(CXX) $(CXXFLAGS) $(addprefix -I,$(INCDIR)) -c -o $@ $<

clean :
	@$(RM) -r $(OBJDIR)
	@$(RM) -r $(BINDIR)
