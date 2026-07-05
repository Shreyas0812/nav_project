cmake_minimum_required(VERSION 3.8)
project(nav_costmap_plugin)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(nav2_costmap_2d REQUIRED)
find_package(pluginlib REQUIRED)

# Build plugin as a Shared Library -- compile the source files into a shared library that can be loaded at runtime by the navigation stack.
add_library(${PROJECT_NAME} SHARED
  src/simple_layer.cpp
)

# Headers
target_include_directories(${PROJECT_NAME} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

# Link against ROS dependencies -- links libraries against the ROS dependencies
ament_target_dependencies(${PROJECT_NAME} 
  rclcpp
  nav2_costmap_2d
  pluginlib
)

# Register this plugin with pluginlib -- tells pluginlib about the plugin and its class name, so that it can be loaded at runtime by the navigation stack.
pluginlib_export_plugin_description_file(nav2_costmap_2d simple_layer.xml) 

# Install the library and header files
install(TARGETS ${PROJECT_NAME}
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)

install(DIRECTORY include/
  DESTINATION include
)

# Export so other packages/pluginlib can find this package
ament_export_include_directories(include)
ament_export_libraries(${PROJECT_NAME})

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
