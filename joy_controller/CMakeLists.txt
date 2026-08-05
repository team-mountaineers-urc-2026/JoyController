cmake_minimum_required(VERSION 3.8)
project(joy_controller)

# [ ] enable optimization on final builds
# [ ] also may wanna make this unconditional, idk what colcon does
# if(NOT CMAKE_BUILD_TYPE)
#     set(CMAKE_BUILD_TYPE Release)
# endif()

# set(CMAKE_CXX_FLAGS_RELEASE "-O3")

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(rclcpp REQUIRED)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/include/joy_controller
)

add_executable(JoyController src/JoyController.cpp)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_copyright_FOUND TRUE)
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_target_dependencies(JoyController
  rclcpp
  sensor_msgs
)

install(TARGETS
  JoyController
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()
