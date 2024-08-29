#Set this to @ to keep the makefile quiet
SILENCE = @

#---- Outputs ----#
COMPONENT_NAME = vecField
TARGET_LIB = \
	lib/lib$(COMPONENT_NAME).a
	
TEST_TARGET = \
	$(COMPONENT_NAME)_tests

#--- Inputs ----#
PROJECT_HOME_DIR = .
CPP_PLATFORM = Gcc

SRC_DIRS = \
	$(PROJECT_HOME_DIR)/TDD/src\

TEST_SRC_DIRS = \
	tests\
	tests/*\

INCLUDE_DIRS =\
  .\
  $(CPPUTEST_HOME)/include\
  $(PROJECT_HOME_DIR)/TDD/include\

#CPPUTEST_WARNINGFLAGS += -pedantic-errors -Wconversion -Wshadow  -Wextra
CPPUTEST_WARNINGFLAGS = -Wno-error #-Wall -Werror -Wswitch-default -Wswitch-enum
CPPUTEST_EXE_FLAGS    += -c

# REMOVE '-static' FROM MAKEFILEWORKER!!
CPPUTEST_CXXFLAGS += $$(pkg-config --cflags cairo)
CPPUTEST_CFLAGS   += $$(pkg-config --cflags cairo)
CPPUTEST_LDFLAGS  += $$(pkg-config --libs cairo)

include $(CPPUTEST_HOME)/build/MakefileWorker.mk