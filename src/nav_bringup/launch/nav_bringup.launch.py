import os
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch_ros.actions import Node

def generate_launch_description():
    nav_bringup_dir = get_package_share_directory('nav_bringup')
    tb3_gazebo_dir = get_package_share_directory('turtlebot3_gazebo')
    nav2_bringup_dir = get_package_share_directory('nav2_bringup')

    # paths
    map_file = os.path.join(nav_bringup_dir, 'maps', 'map.yaml')
    ekf_config = os.path.join(nav_bringup_dir, 'config', 'ekf.yaml')

    # Gazebo + Turtlebot3 Gazebo launch
    gazebo = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(tb3_gazebo_dir, 'launch', 'turtlebot3_world.launch.py')
        )
    )

    # nav2 bringup launch
    tb3_nav2_dir = get_package_share_directory('turtlebot3_navigation2')
    nav2_params = os.path.join(tb3_nav2_dir, 'param', 'waffle.yaml')

    nav2 = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(nav2_bringup_dir, 'launch', 'bringup_launch.py')
        ),
        launch_arguments={
            'map': map_file,
            'use_sim_time': 'true',
            'params_file': nav2_params,
        }.items()
    )

    # ekf node
    ekf = Node(
        package='robot_localization',
        executable='ekf_node',
        name='ekf_filter_node',
        output='screen',
        parameters=[ekf_config, {'use_sim_time': True}]
    )

    return LaunchDescription([gazebo, nav2, ekf])