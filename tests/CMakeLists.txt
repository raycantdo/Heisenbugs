enable_testing()

add_executable(test_profile test_profile.cpp)
add_executable(test_user test_user.cpp)

add_test(NAME ProfileLogicTest COMMAND test_profile)
add_test(NAME UserWorkflowTest COMMAND test_user)