#############################################################
# AUTOMATIC TEST REGISTRATION WITH CTEST
#############################################################

enable_testing()

add_test(
    NAME ${PROJECT_NAME}
    COMMAND $<TARGET_FILE:${PROJECT_NAME}>
)

set_tests_properties(${PROJECT_NAME} 
    PROPERTIES 
        TIMEOUT 30 
        LABELS "unit;shared"
)

message(STATUS "${PROJECT_NAME} test target created and registered with CTest")
