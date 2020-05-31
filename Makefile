# (GNU) Makefile for Ghost Mansion
#
CONFIGURATIONS = Debug Release

ifeq (,$(filter $(notdir $(CURDIR)),$(CONFIGURATIONS)))
	# If not in a "Config" directory, go there now!
	include target.mk
else
	VPATH = $(SRCDIR)

# -------------- End of boilerplate. Rules follow. --------------
	
ifeq '$(findstring ;,$(PATH))' ';'

	UNAME := Windows
else
	UNAME := $(shell uname 2>/dev/null || echo Unknown)
	UNAME := $(patsubst CYGWIN%,Cygwin,$(UNAME))
	UNAME := $(patsubst MSYS%,MSYS,$(UNAME))
	UNAME := $(patsubst MINGW%,MSYS,$(UNAME))
endif

ifeq '$(UNAME)' 'Windows'
	exe := .exe
	RM_FILES := del /Q
else
	exe := 
	RM_FILES := rm -f 
endif

ifeq '' '$(CONFIG)'
	Config := $(notdir $(CURDIR))
	CONFIG := $(shell echo '$(Config)' | tr '[:lower:]' '[:upper:]')
endif

# Clear the suffixes list
.SUFFIXES:
# Set the suffix list to what we care about
.SUFFIXES: .c .o

# Base directory of project
BASEDIR ?= .

# Source directory
SRCDIR ?= $(BASEDIR)/src

# C compiler settings
ifeq ('$(UNAME)','Linux')
	CC := gcc 
	CFLAGS :=  -std=c11 -Wall -Werror -Wextra -D$(CONFIG)

	LD := gcc
	LDFLAGS := -g
else ifeq '$(UNAME)','Windows'
	CC := cl.exe
	CFLAGS := /Za /Wall /WX /D$(CONFIG)

	LD := cl.exe
	LDFLAGS :=	# ???
else
	$(error UNAME is $(UNAME), not recognized!)
endif

ifeq ('$(Config)','Debug')
	CFLAGS += -O0 -g
else ifeq '$(Config)','Release'
	CFLAGS += -O3 -DNDEBUG
else
	$(error Config is '$(Config)', which is not recognized!)
endif

MAKE_TAGS = etags$(exe)

SRCS_C = main.c
SRCS = $(SRCS_C)
OBJS = $(SRCS_C:.c=.o)
EXE = gmanse$(exe)

# Make runs the first target encountered by default:
all: $(EXE)

$(EXE): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)


# --------------------- Resume boilerplate. ---------------------
endif

