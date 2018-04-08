
########################################################
# MANUAL MAKEFILE                                      #
# USE THIS FOR COMPILATION ON A COMMAND LINE INTERFACE #
########################################################

# Source directories separated by space
SRCDIR=./ Input/ Data/ Model/ Output/ Tools/
# Include directories separated by space
INCDIR=./ Input/ Data/ Model/ Output/ Tools/
# Directory where object files will be placed
OBJDIR=../build/
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
CXXFLAGS=-O3 -std=c++11

$(TARGET) : $(OBJ)
	@echo Linking...
	@mkdir -p $(BINDIR)
	@$(CXX) $(CXXFLAGS) -o $(BINDIR)$@ $(OBJ)
	@cp -R ../input $(BINDIR)
	@echo EATSM compilation complete.

$(OBJDIR)%.o : %.cpp
	@echo Compiling $< in $@...
	@mkdir -p $(OBJDIR)
	@$(CXX) $(CXXFLAGS) $(addprefix -I,$(INCDIR)) -c -o $@ $<

clean :
	@echo Cleaning...
	@$(RM) -r $(OBJDIR)
	@$(RM) -r $(BINDIR)
	@echo EATSM cleaning complete.
