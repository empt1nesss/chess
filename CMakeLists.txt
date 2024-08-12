cmake_minimum_required(VERSION 3.22)
project(chess LANGUAGES CXX)

IF (WIN32)
  option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
ENDIF()

set(CMAKE_CXX_STANDARD 17)

IF (WIN32)
  include(FetchContent)
  FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x
  )
  FetchContent_MakeAvailable(SFML)
ELSE()
  find_package(SFML
    COMPONENTS
      graphics
      audio
      window
      system
      network
  REQUIRED)
ENDIF()

file(GLOB_RECURSE BUILD_TARGETS
  source/*.cpp
  source/*.c
)

add_executable(${PROJECT_NAME} ${BUILD_TARGETS})

include_directories(
  include
)

target_link_libraries(${PROJECT_NAME}
  PRIVATE
    sfml-graphics
)

IF(WIN32)
  add_custom_command(
    TARGET ${PROJECT_NAME}
    COMMENT "Copy OpenAL DLL"
    PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:${PROJECT_NAME}>
    VERBATIM
  )
  install(TARGETS ${PROJECT_NAME})
ENDIF()

