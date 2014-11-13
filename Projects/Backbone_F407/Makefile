#=============================================================================#
# ARM makefile
#
# author: Freddie Chopin, http://www.freddiechopin.info/
# last change: 2012-01-07
#
# this makefile is based strongly on many examples found in the network
#=============================================================================#

#=============================================================================#
# toolchain configuration
#=============================================================================#

TOOLCHAIN_PATH = C:/GCCARM/bin

TOOLCHAIN = $(TOOLCHAIN_PATH)/arm-none-eabi-

CXX = $(TOOLCHAIN)g++
CC = $(TOOLCHAIN)gcc
AS = $(TOOLCHAIN)gcc -x assembler-with-cpp
OBJCOPY = $(TOOLCHAIN)objcopy
OBJDUMP = $(TOOLCHAIN)objdump
SIZE = $(TOOLCHAIN)size
RM = $(TOOLCHAIN_PATH)/rm -f
CP = $(TOOLCHAIN_PATH)/cp
GREP = $(TOOLCHAIN_PATH)/grep
AWK = $(TOOLCHAIN_PATH)/awk
MKDIR = $(TOOLCHAIN_PATH)/mkdir -p
TC = $(TOOLCHAIN_PATH)/TemplateCompiler
ZIP = zip -q
NODE = node

#ifeq (AMD64, $(PROCESSOR_ARCHITEW6432))
#  SUBWCREV = tools/SubWCRev64.exe
#else
#  SUBWCREV = tools/SubWCRev.exe
#endif
SUBWCREV = SubWCRev
OPENOCD = openocd-0.8.0

#=============================================================================#
# project configuration
#=============================================================================#

# project name
PROJECT = FIRMWARE

# core type
CORE = cortex-m4

# linker script
#LD_SCRIPT = src/stm32f429_flash.ld
LD_SCRIPT = src/stm32f407_flash.ld

# output folder (absolute or relative path, leave empty for in-tree compilation)
OUT_DIR = out

# C++ definitions (e.g. "-Dsymbol_with_value=0xDEAD -Dsymbol_without_value")
CXX_DEFS = -DDEBUG

# C definitions
C_DEFS = -DDEBUG

# ASM definitions
AS_DEFS =

# include directories (absolute or relative paths to additional folders with
# headers, current folder is always included)
INC_DIRS  = libs/STM32F4xx_HAL_Driver/Inc
INC_DIRS += libs/STM32F4xx_HAL_Driver/Inc
INC_DIRS += libs/CMSIS/Include
INC_DIRS += libs/CMSIS/Device/ST/STM32F4xx/Include
INC_DIRS += libs/STM32_USB_Device_Library/Class/CDC/Inc
INC_DIRS += libs/STM32_USB_Device_Library/Core/Inc

INC_DIRS += libs/LwIP/src/include/ipv4/
INC_DIRS += libs/LwIP/src/include
INC_DIRS += libs/LwIP/system

INC_DIRS += libs/fatfs

INC_DIRS += inc


# library directories (absolute or relative paths to additional folders with
# libraries)
LIB_DIRS = libs .

# libraries (additional libraries for linking, e.g. "-lm -lsome_name" to link
# math library libm.a and libsome_name.a)
LIBS = -lm


# additional directories with source files (absolute or relative paths to
# folders with source files, current folder is always included)
SRCS_DIRS = src
SRCS_DIRS += libs/STM32F4xx_HAL_Driver/Src
SRCS_DIRS += libs/STM32_USB_Device_Library/Class/CDC/Src
SRCS_DIRS += libs/STM32_USB_Device_Library/Core/Src

SRCS_DIRS += libs/LwIP/src/core
SRCS_DIRS += libs/LwIP/src/core/ipv4
SRCS_DIRS += libs/LwIP/src/core/snmp
SRCS_DIRS += libs/LwIP/src/netif
SRCS_DIRS += libs/LwIP/src/api

SRCS_DIRS += libs/fatfs

# exclude some files from compiling
EXCLUDES = src/fsdata.c

# extension of C++ files
CXX_EXT = cpp

# wildcard for C++ source files (all files with CXX_EXT extension found in
# current folder and SRCS_DIRS folders will be compiled and linked)
CXX_SRCS = $(wildcard $(patsubst %, %/*.$(CXX_EXT), . $(SRCS_DIRS)))

# extension of C files
C_EXT = c

# wildcard for C source files (all files with C_EXT extension found in current
# folder and SRCS_DIRS folders will be compiled and linked)
C_SRCS_ALL = $(wildcard $(patsubst %, %/*.$(C_EXT), . $(SRCS_DIRS)))
C_SRCS = $(filter-out $(EXCLUDES), $(C_SRCS_ALL))

# extension of ASM files
AS_EXT = asm

# wildcard for ASM source files (all files with AS_EXT extension found in
# current folder and SRCS_DIRS folders will be compiled and linked)
AS_SRCS = $(wildcard $(patsubst %, %/*.$(AS_EXT), . $(SRCS_DIRS)))

# optimization flags ("-O0" - no optimization, "-O1" - optimize, "-O2" -
# optimize even more, "-Os" - optimize for size or "-O3" - optimize yet more) 
OPTIMIZATION = -O3

# set to 1 to optimize size by removing unused code and data during link phase
REMOVE_UNUSED = 1

# set to 1 to compile and link additional code required for C++
USES_CXX = 0

# define warning options here
CXX_WARNINGS = -Wall -Wextra
C_WARNINGS = -Wall -Wextra -fno-strict-aliasing

# C++ language standard ("c++98", "gnu++98" - default, "c++0x", "gnu++0x")
CXX_STD = gnu++98

# C language standard ("c89" / "iso9899:1990", "iso9899:199409",
# "c99" / "iso9899:1999", "gnu89" - default, "gnu99")
C_STD = gnu99

#=============================================================================#
# set the VPATH according to SRCS_DIRS
#=============================================================================#

VPATH = $(SRCS_DIRS)

#=============================================================================#
# when using output folder, append trailing slash to its name
#=============================================================================#

ifeq ($(strip $(OUT_DIR)), )
	OUT_DIR_F = 
else
	OUT_DIR_F = $(strip $(OUT_DIR))/
endif

#=============================================================================#
# various compilation flags
#=============================================================================#

# core flags
CORE_FLAGS = -mcpu=$(CORE) -mthumb -mfloat-abi=soft
#CORE_FLAGS = -mcpu=$(CORE) -mthumb -mfloat-abi=softfp -mfpu=fpv4-sp-d16

# flags for C++ compiler
CXX_FLAGS = -std=$(CXX_STD) -g -ggdb3 -fno-rtti -fno-exceptions -fverbose-asm -Wa,-adhlns=$(OUT_DIR_F)$(notdir $(<:.$(CXX_EXT)=.lst))

# flags for C compiler
#C_FLAGS = -std=$(C_STD) -fmessage-length=0 -fno-strict-aliasing -flto -g3 -gdwarf-2 -Wa,-adhlns=$(OUT_DIR_F)$(notdir $(<:.$(C_EXT)=.lst))
C_FLAGS  = -O$(OPT)
C_FLAGS += -std=gnu99
C_FLAGS += -gdwarf-2
C_FLAGS += -ffunction-sections
C_FLAGS += -fdata-sections
#C_FLAGS += -Wall


# flags for assembler
AS_FLAGS = -g -ggdb3 -Wa,-amhls=$(OUT_DIR_F)$(notdir $(<:.$(AS_EXT)=.lst))

# flags for linker
LD_FLAGS = -T$(LD_SCRIPT) -g -Wl,-Map=$(OUT_DIR_F)$(PROJECT).map,--cref,--no-warn-mismatch

# process option for removing unused code
ifeq ($(REMOVE_UNUSED), 1)
	LD_FLAGS += -Wl,--gc-sections
	OPTIMIZATION += -ffunction-sections -fdata-sections
endif

# if __USES_CXX is defined for ASM then code for global/static constructors /
# destructors is compiled; if -nostartfiles option for linker is added then C++
# initialization / finalization code is not linked
ifeq ($(USES_CXX), 1)
	AS_DEFS += -D__USES_CXX
else
	LD_FLAGS += -nostartfiles
endif

#=============================================================================#
# do some formatting
#=============================================================================#

CXX_OBJS = $(addprefix $(OUT_DIR_F), $(notdir $(CXX_SRCS:.$(CXX_EXT)=.o)))
C_OBJS = $(addprefix $(OUT_DIR_F), $(notdir $(C_SRCS:.$(C_EXT)=.o)))
AS_OBJS = $(addprefix $(OUT_DIR_F), $(notdir $(AS_SRCS:.$(AS_EXT)=.o)))
OBJS = $(AS_OBJS) $(C_OBJS) $(CXX_OBJS) $(USER_OBJS)
DEPS = $(OBJS:.o=.d)
INC_DIRS_F = -I. $(patsubst %, -I%, $(INC_DIRS))
LIB_DIRS_F = $(patsubst %, -L%, $(LIB_DIRS))

ELF = $(OUT_DIR_F)$(PROJECT).elf
HEX = $(OUT_DIR_F)$(PROJECT).hex
COMPOSITEHEX = $(OUT_DIR_F)$(PROJECT)-Composite.hex
BIN = $(OUT_DIR_F)$(PROJECT).bin
UPDATEBIN = $(OUT_DIR_F)$(PROJECT)-Update.bin
COMPOSITEBIN = $(OUT_DIR_F)$(PROJECT)-Composite.bin
LSS = $(OUT_DIR_F)$(PROJECT).lss
DMP = $(OUT_DIR_F)$(PROJECT).dmp

# format final flags for tools, request dependancies for C++, C and asm
CXX_FLAGS_F = $(CORE_FLAGS) $(OPTIMIZATION) $(CXX_WARNINGS) $(CXX_FLAGS)  $(CXX_DEFS) -MD -MP -MF $(OUT_DIR_F)$(@F:.o=.d) $(INC_DIRS_F)
C_FLAGS_F = $(CORE_FLAGS) $(OPTIMIZATION) $(C_WARNINGS) $(C_FLAGS) $(C_DEFS) -MD -MP -MF $(OUT_DIR_F)$(@F:.o=.d) $(INC_DIRS_F)
AS_FLAGS_F = $(CORE_FLAGS) $(AS_FLAGS) $(AS_DEFS) -MD -MP -MF $(OUT_DIR_F)$(@F:.o=.d) $(INC_DIRS_F)
LD_FLAGS_F = $(CORE_FLAGS) $(LD_FLAGS) $(LIB_DIRS_F)

#contents of output directory
GENERATED = $(wildcard $(patsubst %, $(OUT_DIR_F)*.%, bin d dmp elf hex lss lst map o))

#=============================================================================#
# make all
#=============================================================================#



all: build showsize

build: elf bin lss

	
elf: $(ELF)
  

bin: $(BIN)

lss: $(LSS)

hex: $(HEX)

dump: $(DMP)

# make object files dependent on Makefile and create output dir order-only
$(OBJS) : Makefile | $(OUT_DIR)

# create output directory if necessary
$(OUT_DIR):
	$(MKDIR) $(OUT_DIR)
	
# make .elf file dependent on linker script
$(ELF) : $(LD_SCRIPT)

#-----------------------------------------------------------------------------#
# linking - objects -> elf
#-----------------------------------------------------------------------------#

.SECONDARY: $(ELF)
.PRECIOUS:  $(OBJS)
$(ELF) : $(OBJS)
	@echo Linking target: $(ELF)
	$(CXX) $(LD_FLAGS_F) $(OBJS) $(LIBS) -o $@
	@printf ""

#-----------------------------------------------------------------------------#
# compiling - C++ source -> objects
#-----------------------------------------------------------------------------#

$(OUT_DIR_F)%.o : %.$(CXX_EXT)
	@echo Compiling file: $<
	@$(MKDIR) $(addprefix $(OUT_DIR)/,$(basename $<))
	$(CXX) -c $(CXX_FLAGS_F) $< -o $@
	@printf ""

#-----------------------------------------------------------------------------#
# compiling - C source -> objects
#-----------------------------------------------------------------------------#

$(OUT_DIR_F)%.o : %.$(C_EXT)
	@echo Compiling file: $<
	$(CC) -c $(C_FLAGS_F) $< -o $@
	@printf ""

#-----------------------------------------------------------------------------#
# assembling - ASM source -> objects
#-----------------------------------------------------------------------------#

$(OUT_DIR_F)%.o : %.$(AS_EXT)
	@echo Assembling file: $<
	$(AS) -c $(AS_FLAGS_F) $< -o $@
	@printf ""

#-----------------------------------------------------------------------------#
# memory images - elf -> hex, elf -> bin
#-----------------------------------------------------------------------------#

$(HEX) : $(ELF)
	@echo Creating IHEX image: $(HEX)
	$(OBJCOPY) -O ihex $< $@
	@printf ""

$(BIN) : $(ELF)
	@echo Creating binary image: $(BIN)
	$(OBJCOPY) -O binary $< $@
	@printf ""

#-----------------------------------------------------------------------------#
# memory dump - elf -> dmp
#-----------------------------------------------------------------------------#

$(DMP) : $(ELF)
	@echo Creating memory dump: $(DMP)
	$(OBJDUMP) -x --syms $< > $@
	@printf ""

#-----------------------------------------------------------------------------#
# extended listing - elf -> lss
#-----------------------------------------------------------------------------#

$(LSS) : $(ELF)
	@echo Creating extended listing: $(LSS)
	$(OBJDUMP) -S $< > $@
	@printf ""

#-----------------------------------------------------------------------------#
# print the size of the objects and the .elf file
#-----------------------------------------------------------------------------#

showsize : $(ELF)
#       @echo 'Size of modules:'
#       $(SIZE) -B -t --common $(OBJS) $(USER_OBJS)
#       @echo ' '
	@echo Size of target .elf file:
	$(SIZE) -B $(ELF)
	@printf ""

# SVN Info
svninfo:
	$(SUBWCREV) .. subwcrev.h.in subwcrev.h

openocd:
	$(OPENOCD)
	
# Flash the device using ELF file  
flash: $(ELF)
	$(OPENOCD) -f "openocd.cfg" -c "flash_image $(ELF); shutdown"

# Flash the device using composite bin file (bootloader+application)
flash-composite: composite
	$(OPENOCD) -f "openocd.cfg" -c "flash_bin_image $(COMPOSITEBIN); shutdown"

#=============================================================================#
# make clean
#=============================================================================#

clean:
ifeq ($(strip $(OUT_DIR_F)), )
	@echo  Removing all generated output files 
else
	@echo  Removing all generated output files from output directory: $(OUT_DIR_F) 
endif
ifneq ($(strip $(GENERATED)), )
	$(RM) $(GENERATED)
else
	@echo  Nothing to remove... 
endif

#=============================================================================#
# global exports
#=============================================================================#

.PHONY: all build flash clean dependents showsize elf lss

# include dependancy files
-include $(DEPS)


