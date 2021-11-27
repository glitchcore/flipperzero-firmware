CORE_DIR		= $(PROJECT_ROOT)/core

CFLAGS			+= -D_GNU_SOURCE
INCLUDES		+= -I$(CORE_DIR) 
ASM_SOURCES		+= $(wildcard $(CORE_DIR)/*.s)
C_SOURCES		+= $(wildcard $(CORE_DIR)/*.c)
C_SOURCES		+= $(wildcard $(CORE_DIR)/furi/*.c)
C_SOURCES		+= $(wildcard $(CORE_DIR)/furi-hal/*.c)
CPP_SOURCES		+= $(wildcard $(CORE_DIR)/*.cpp)
