PROGRAM_NAME := $(shell basename `pwd`)

# compiler
CXX := g++
CXXFLAGS := -Wextra -std=c++14 -fpermissive -g3 -lSDL2 -lSDL2_image -lboost_thread-mt -lboost_system
LIBS :=

# directory
OUT_DIR := out
SOURCE_DIR := src
HEADER_DIR := src

SOURCES := $(wildcard $(SOURCE_DIR)/*.cpp)
HEADERS := $(wildcard $(HEADER_DIR)/*.h)

PROGRAM_DIR := $(OUT_DIR)/bin
OBJ_DIR := $(OUT_DIR)/obj
DEPEND_DIR := $(OUT_DIR)/depend

PROGRAM := $(PROGRAM_DIR)/$(PROGRAM_NAME)
SOURCE_NAMES = $(notdir $(SOURCES))
OBJS := $(addprefix $(OBJ_DIR)/,$(SOURCE_NAMES:.cpp=.o))
DEPENDS := $(addprefix $(DEPEND_DIR)/,$(SOURCE_NAMES:.cpp=.depend))

.PHONY: all
all: $(DEPENDS) $(PROGRAM)
$(PROGRAM): $(OBJS)
	@mkdir -p $(PROGRAM_DIR)
	$(CXX) $(CXXFLAGS) $(LIBS) $^ -o $(PROGRAM)

$(DEPEND_DIR)/%.depend: $(SOURCE_DIR)/%.cpp $(HEADERS)
	@echo "generating $@"
	@mkdir -p $(DEPEND_DIR)
	@$(CXX) $(CXXFLAGS) $(LIBS) -I$(HEADER_DIR) -MM $< > $@

$(OBJ_DIR)/%.o: $(SOURCE_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(LIBS) -I$(HEADER_DIR) -c $^ -o $@


ifneq "$(MAKECMDGOALS)" "clean"
-include $(DEPENDS)
endif

.PHONY : clean
clean:
	$(RM) -r $(OUT_DIR)
