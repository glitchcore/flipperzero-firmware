# Compiller 
ifeq ($(TOOLCHAIN), arm)
PREFIX = arm-none-eabi-
ifdef GCC_PATH
PREFIX = $(GCC_PATH)/$(PREFIX)
endif
endif

CC	= $(PREFIX)gcc -std=gnu17
CPP	= $(PREFIX)g++ -std=gnu++17
LD	= $(PREFIX)g++
AS	= $(PREFIX)gcc -x assembler-with-cpp
CP	= $(PREFIX)objcopy
DUMP = $(PREFIX)objdump -d
SZ	= $(PREFIX)size
AR  = $(PREFIX)ar -rcs
HEX	= $(CP) -O ihex
BIN	= $(CP) -O binary -S

DEBUG ?= 0
COMPACT ?= 1
ifeq ($(DEBUG), 1)
CFLAGS += -DFURI_DEBUG -DNDEBUG -Og -g
else ifeq ($(COMPACT), 1)
CFLAGS += -DFURI_DEBUG -DNDEBUG -Os -g0
else
CFLAGS += -DFURI_DEBUG -DNDEBUG -Og -g0
endif

CFLAGS		+= -fdata-sections -ffunction-sections -fno-math-errno -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)"
CPPFLAGS	+= -fno-threadsafe-statics -fno-use-cxa-atexit -fno-exceptions -fno-rtti
LDFLAGS		+= -Wl,--gc-sections -Wl,--undefined=uxTopUsedPriority -u _printf_float
LDMAP		+= -Wl,-Map=$(OBJ_DIR)/$(PROJECT).map,--cref