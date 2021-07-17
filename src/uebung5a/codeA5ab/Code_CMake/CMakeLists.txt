# Copyright (c) 2021, Tim A. Werthmann.

cmake_minimum_required(VERSION 3.13.3 FATAL_ERROR)
cmake_policy(VERSION 3.13.3)
# ##############################################################################
# Build type (needs to be handled before project command below)
# ##############################################################################
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE
      "Release"
      CACHE
        STRING
        "Configuration type (one of Debug, Release, RelWithDebInfo, MinSizeRel)"
  )
endif()
message(STATUS "The build type is ${CMAKE_BUILD_TYPE}")
#
# ##############################################################################
# Project metadata
# ##############################################################################
project(astern LANGUAGES CXX VERSION 1.0.0)

# ##############################################################################
# Find or fetch SFML
# ##############################################################################

# Dies laedt die SFML herunter
include(${astern_SOURCE_DIR}/cmake/dependencies.cmake)

# ##############################################################################
# Copy data to target
# ##############################################################################

# Dies kopiert die Daten an den richtigen Ort
include(${astern_SOURCE_DIR}/cmake/data_copy.cmake)

# ##############################################################################
# Handle Unit
# ##############################################################################

set(a_stern_unit)
if(APPLE)
  list(APPEND a_stern_unit lib/unit_mac.cpp.o)
elseif(UNIX AND NOT APPLE)
  list(APPEND a_stern_unit lib/unit.cpp.o)
endif()

# ##############################################################################
# Add Binaries
# ##############################################################################

# hier muesst ihr eure weiteren Dateien also Abhaenigkeiten eintragen
add_executable(a_stern src/a_stern.cpp ${a_stern_unit})

# Setzt diese Variable auf true, wenn ihr Teil b bearbeitet
option(teil_b "Teil b on" ON)

# ##############################################################################

set_target_properties(
  a_stern PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON CXX_EXTENSIONS
                                                              OFF
)
target_compile_options(
  a_stern
  PRIVATE
    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:-Wall
    -Wextra
    -Wpedantic>
    $<$<CXX_COMPILER_ID:MSVC>:
    /W4>
)
target_include_directories(a_stern PRIVATE "${astern_SOURCE_DIR}/include/")

if(${teil_b})
  if((UNIX AND NOT APPLE) AND CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_include_directories(a_stern PRIVATE "${sfml_SOURCE_DIR}/include/")
  endif()

  if((UNIX AND NOT APPLE) AND CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_link_libraries(
      a_stern
      PRIVATE "${sfml_SOURCE_DIR}/lib/libsfml-graphics.so"
              "${sfml_SOURCE_DIR}/lib/libsfml-window.so"
              "${sfml_SOURCE_DIR}/lib/libsfml-system.so"
    )
  else()
    target_link_libraries(a_stern PRIVATE sfml-graphics sfml-window sfml-system)
  endif()

  # ############################################################################

  add_executable(sfml_test "src/sfml_test.cpp")

  set_target_properties(
    sfml_test PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON CXX_EXTENSIONS
                                                                  OFF
  )
  target_compile_options(
    sfml_test
    PRIVATE
      $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:-Wall
      -Wextra
      -Wpedantic>
      $<$<CXX_COMPILER_ID:MSVC>:
      /W4>
  )
  target_include_directories(sfml_test PRIVATE "${astern_SOURCE_DIR}/include/")

  if((UNIX AND NOT APPLE) AND CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_include_directories(sfml_test PRIVATE "${sfml_SOURCE_DIR}/include/")
  endif()

  if((UNIX AND NOT APPLE) AND CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_link_libraries(
      sfml_test
      PRIVATE "${sfml_SOURCE_DIR}/lib/libsfml-graphics.so"
              "${sfml_SOURCE_DIR}/lib/libsfml-window.so"
              "${sfml_SOURCE_DIR}/lib/libsfml-system.so"
    )
  else()
    target_link_libraries(
      sfml_test PRIVATE sfml-graphics sfml-window sfml-system
    )
  endif()

endif()
