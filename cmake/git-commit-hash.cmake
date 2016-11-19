find_package(Git)

if(GIT_FOUND)
  execute_process(
    COMMAND ${GIT_EXECUTABLE} rev-parse HEAD
    OUTPUT_VARIABLE GIT_COMMIT_HASH
    ERROR_QUIET
    OUTPUT_STRIP_TRAILING_WHITESPACE
    )
  message(STATUS "Current comit sha1 is ${GIT_COMMIT_HASH}")
else()
  set(GIT_COMMIT_HASH "unknown")
endif()