cmake_minimum_required(VERSION 3.22)
project(chess LANGUAGES CXX)

IF (WIN32)
  option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
ENDIF()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

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

IF(WIN32)
  add_executable(${PROJECT_NAME} WIN32 ${BUILD_TARGETS})
ELSE()
  add_executable(${PROJECT_NAME} ${BUILD_TARGETS})
ENDIF()


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

file(REMOVE_RECURSE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets)
add_custom_command(
  TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
)

