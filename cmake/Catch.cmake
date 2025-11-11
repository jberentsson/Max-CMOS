#############################################################
# CATCH2 SETUP
#############################################################

include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.4.0  # or latest stable version
)

FetchContent_MakeAvailable(Catch2)
