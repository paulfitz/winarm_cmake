cmake_minimum_required(VERSION 2.8)

set(WinArm_FOUND TRUE)

set(WINARM_BASE ${CMAKE_CURRENT_LIST_DIR})

macro(set_winarm_flags_base SIMULATOR)
  if (NOT ${SIMULATOR})
    set(CMAKE_TOOLCHAIN_FILE ${WINARM_BASE}/toolchain.cmake)
    message(STATUS "Set toolchain")
    enable_language(C ASM)
  endif()

  set(WinArm_SRCS)
  set(WinArm_INCLUDE_DIRS)
  set(WinArm_LIBRARIES)
  set(WinArm_SIMULATED FALSE)

  set(WINARM ${WINARM_BASE}/extern/Common_WinARM)
  if (NOT ${SIMULATOR})
    set(CMAKE_C_FLAGS "-mcpu=arm7tdmi-s -mthumb-interwork -I. -gdwarf-2 -DROM_RUN -D__WinARM__  -Os -Wall -Wcast-align -Wimplicit  -Wpointer-arith -Wswitch -Wredundant-decls -Wreturn-type -Wshadow -Wunused -Wa,-adhlns=\"$@.lst\"  -I../Common_WinARM/inc -Wcast-qual -MD -MP -MF ignore.txt -Wnested-externs  -std=gnu99 -Wmissing-prototypes  -Wstrict-prototypes -Wmissing-declarations") 
    set(CMAKE_ASM_FLAGS "-mcpu=arm7tdmi-s -mthumb-interwork -I. -x assembler-with-cpp -DROM_RUN -D__WinARM__ -Wa,-adhlns=\"$@.lst\",-gdwarf-2")
    set(CMAKE_EXE_LINKER_FLAGS "-nostartfiles -Wl,-Map=main.map,--cref -lc  -lm -lc -lgcc -T${WINARM}/LPC2138-ROM.ld")
    set(WinArm_SRCS
      ${WINARM}/src/Startup.S
      ${WINARM}/src/swi_handler.S
      ${WINARM}/src/irq.c
      ${WINARM}/src/target.c
      ${WINARM}/src/timer.c)
    set(WinArm_HEXIFY arm-none-eabi-objcopy -Oihex)
  else ()
    set(CMAKE_C_COMPILER "${CMAKE_CXX_COMPILER}")
    set(CMAKE_C_COMPILE_OBJECT "${CMAKE_CXX_COMPILE_OBJECT}") 
    enable_language(CXX)
    set(WINARM ${WINARM_BASE}/sim)
    set (WinArm_SRCS
      ${WINARM}/src/platform_sim.cpp)
    find_package(YARP REQUIRED)
    set(WinArm_INCLUDE_DIRS ${WinArm_INCLUDE_DIRS} ${YARP_INCLUDE_DIRS})
    set(WinArm_LIBRARIES ${WinArm_LIBRARIES} ${YARP_LIBRARIES})
  set(WinArm_SIMULATED TRUE)
  endif ()

  set(WinArm_INCLUDE_DIRS ${WinArm_INCLUDE_DIRS} ${WINARM}/inc)
endmacro ()


macro(winarm_flags)
  if (SIMULATE)
    set_winarm_flags_base(TRUE)
  else ()
    set_winarm_flags_base(FALSE)
  endif()
endmacro ()

macro(winarm_tweak)
  foreach(fname ${ARGN})
    if (WinArm_SIMULATED)
      set_source_files_properties(${fname} PROPERTIES LANGUAGE "CXX")
    endif()
  endforeach ()
endmacro ()
