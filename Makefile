SRC=$(wildcard src/*.cpp)
HDR=$(wildcard src/*.hpp)
OBJECTS=$(patsubst %.cpp,%.o,$(SRC))
EXEC=sat

# unit testing requires UnitTest++ (http://unittest-cpp.sourceforge.net/)
UNITTESTPP_SVN=https://unittest-cpp.svn.sourceforge.net/svnroot/unittest-cpp/UnitTest++
UNITTESTPP_DIR=test/unittest++
UNITTESTPP_H=$(UNITTESTPP_DIR)/src/UnitTest++.h
UNITTESTPP_LIB=$(UNITTESTPP_DIR)/libUnitTest++.a
UNITTESTPP_INC=$(UNITTESTPP_DIR)/src

TEST_SRC=$(wildcard test/*.cpp)
TEST_O=$(patsubst %.cpp,%.o,$(TEST_SRC))
TEST_EXEC=test/runtests

ifeq "$(MAKECMDGOALS)" "test"
CXXFLAGS += -I$(UNITTESTPP_INC)
endif

default: build

test: build $(TEST_EXEC)
	$(TEST_EXEC)

build: $(EXEC)

$(EXEC): $(OBJECTS)
	g++ -o $(EXEC) $(OBJECTS)

clean:
	rm -f $(TEST_EXEC) $(TEST_O)
	rm -f $(EXEC) $(OBJECTS)
	rm -f src/*.d test/*.d

export: clean
	tar -cvzf ../LIF11_PROJET.tar.gz src/

.PHONY: default test build clean

$(UNITTESTPP_H):
	svn co $(UNITTESTPP_SVN) $(UNITTESTPP_DIR)

$(UNITTESTPP_LIB): $(UNITTESTPP_H)
	make -C $(UNITTESTPP_DIR)

$(TEST_O): $(UNITTESTPP_H)

$(TEST_EXEC): $(TEST_O) $(filter-out src/Main.o,$(OBJECTS)) $(UNITTESTPP_LIB)
	g++ -o $(TEST_EXEC) $+

%.d: %.cpp
	g++ -M $(CXXFLAGS) $*.cpp | sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' > $*.d

include $(SRC:.cpp=.d) $(TEST_SRC:.cpp=.d)
