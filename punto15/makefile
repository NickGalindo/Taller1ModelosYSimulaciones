EXE := run 

IDIR := include
LDIR := lib
SDIR := src
ODIR := obj

CXX := g++
CXXFLAGS := -g -Wall -W -pedantic 

CPPFLAGS := -I$(IDIR)
CPPFLAGS += -MMD -MP

CPPONLYFLAGS := -std=c++20

LDFLAGS := -L$(LDIR)

CPPINCSRC := $(wildcard $(LDIR)/*.cpp)
CINCSRC := $(wildcard $(LDIR)/*.c)
CPPSRC := $(wildcard $(SDIR)/*.cpp)
CSRC += $(wildcard $(SDIR)/*.c)

CPPIOBJ := $(patsubst $(LDIR)/%.cpp,$(ODIR)/%.o,$(CPPINCSRC))
CIOBJ :=  $(patsubst $(LDIR)/%.c,$(ODIR)/%.o,$(CINCSRC))
CPPSOBJ := $(patsubst $(SDIR)/%.cpp,$(ODIR)/%.o,$(CPPSRC))
CSOBJ := $(patsubst $(SDIR)/%.c,$(ODIR)/%.o,$(CSRC))
DEP := $(CPPIOBJ:.o=.d) $(CPPSOBJ:.o=.d) $(CIOBJ:.o=.d) $(CSOBJ:.o=.d)


# SET VALUES FOR TEST
TESTEXE := test

TESTDIR := testing
TESTODIR := testing/obj

.PHONY: all clean


printvars:
	@echo -e "CPPINCSRC:\t" $(CPPINCSRC)
	@echo -e "CINCSRC:\t" $(CINCSRC)
	@echo -e "CPPSRC:\t" $(CPPSRC)
	@echo -e "CSRC:\t" $(CSRC)
	@echo -e "CPPIOBJ:\t" $(CPPIOBJ)
	@echo -e "CIOBJ:\t" $(CIOBJ)
	@echo -e "CPPSOBJ:\t" $(CPPSOBJ)
	@echo -e "CSOBJ:\t" $(CSOBJ)
	@echo -e "DEP:\t\t" $(DEP)
	@echo -e "OKS " $(wildcard *.cpp)

all: $(EXE)

$(EXE): $(CPPIOBJ) $(CIOBJ) $(CPPSOBJ) $(CSOBJ)
	$(CXX) $^ -o $@

$(CPPIOBJ): $(ODIR)/%.o: $(LDIR)/%.cpp $(CPPINCSRC) | $(ODIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(CPPONLYFLAGS) -o $@ -c $<

$(CIOBJ): $(ODIR)/%.o: $(LDIR)/%.c $(CINCSRC) | $(ODIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ -c $<

$(CPPSOBJ): $(ODIR)/%.o: $(SDIR)/%.cpp $(CPPSRC) | $(ODIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(CPPONLYFLAGS) -o $@ -c $<

$(CSOBJ): $(ODIR)/%.o: $(SDIR)/%.c $(CSRC) | $(ODIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ -c $<

$(ODIR):
	mkdir $@

testing: $(TESTEXE)

$(TESTEXE): $(CPPIOBJ) $(CIOBJ) 
	cd testing; $(MAKE) all
	$(CXX) $^ $(wildcard $(TESTODIR)/*.o) -o $@ 

clean:
	rm -r $(ODIR) $(EXE)

-include $(DEP)

