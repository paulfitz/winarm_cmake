INCLUDE(CMakeForceCompiler)

# this one is important
SET(CMAKE_SYSTEM_NAME Generic)
#this one not so much
SET(CMAKE_SYSTEM_VERSION 1)

# specify the cross compiler
CMAKE_FORCE_C_COMPILER(arm-none-eabi-gcc GNU)
CMAKE_FORCE_CXX_COMPILER(arm-none-eabi-gcc GNU)

# CMAKE_FORCE_C_COMPILER(arm-elf-gcc GNU)
# CMAKE_FORCE_CXX_COMPILER(arm-elf-gcc GNU)

