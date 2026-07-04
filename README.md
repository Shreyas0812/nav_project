# nav_project

Autonomous navigation stack built on ROS2 Humble + Nav2 + SLAM Toolbox,
simulated with TurtleBot3 (Waffle) in Gazebo Classic.

**Status:** In Progress.

## Environment
Ubuntu 22.04 · ROS2 Humble · Gazebo Classic 11 · TurtleBot3 Waffle

## Setup
```bash
source env.sh
```

## Building a map (SLAM Toolbox)
Each command runs in its own sourced terminal.

1. Launch the simulated robot:
```bash
   ros2 launch turtlebot3_gazebo turtlebot3_world.launch.py
```
2. Start SLAM Toolbox. `use_sim_time:=true` to use Gazebo's simulated clock
```bash
   ros2 launch slam_toolbox online_async_launch.py use_sim_time:=true
```
3. Visualized in RViz (Fixed Frame = `map`, add `/map` and `/scan`):
```bash
   rviz2
```
4. Teleop to Drive, builds maps, revisiting areas to trigger loop closure:
```bash
   ros2 run turtlebot3_teleop teleop_keyboard
```
5. Save the map:
```bash
   ros2 run nav2_map_server map_saver_cli -f maps/turtlebot3_world
```

Saved map: `maps/turtlebot3_world.{pgm,yaml}`

## Autonomous navigation (Nav2)
With Gazebo still running and the robot spawned:

1. Launch Nav2 against the saved map:
```bash
   ros2 launch turtlebot3_navigation2 navigation2.launch.py \
     use_sim_time:=true \
     map:=/absolute/path/to/turtlebot3_world.yaml
```
2. In RViz, **2D Pose Estimate** to set robot's actual
   location to set its starting pose and heading.

3. In RViz, **Nav2 Goal** and click/drag a reachable destination. Nav2 plans a
   global path and the local controller (DWB) drives the robot there while
   avoiding obstacles.

## Sensor Fusion - EKF (in progress)
Extended Kalman Filter (`robot_localization`) fuses wheel odometry (`/odom`)
and IMU (`/imu`) into a single estimate published on `/odometry/filtered`.

Config: `config/ekf.yaml`. Run:
```bash
ros2 run robot_localization ekf_node --ros-args --params-file config/ekf.yaml
```

**Known issue (not yet resolved):** the TurtleBot3 Gazebo diff-drive plugin
also publishes `odom -> base_footprint`.

## Scope 
Simulation-only (Gazebo). Not yet validated on physical hardware.