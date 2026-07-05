#include "nav_costmap_plugin/simple_layer.hpp"
#include "nav2_costmap_2d/cost_values.hpp"
#include "rclcpp/rclcpp.hpp"
#include <algorithm> // for std::min and std::max

namespace nav_costmap_plugin
{

void SimpleLayer::onInitialize()
{
  // Hardcode a rectangle in world coordinates (metres) for Stage 1.
  // We'll make these parameters later.
  rect_min_x_ = 1.0;
  rect_min_y_ = 1.0;
  rect_max_x_ = 2.0;
  rect_max_y_ = 2.0;

  need_recalculation_ = false;
  current_ = true;      // inherited from Layer: marks layer as up-to-date
  enabled_ = true;      // inherited from Layer: layer is active
}

void SimpleLayer::updateBounds(
  double /*robot_x*/, double /*robot_y*/, double /*robot_yaw*/,
  double * min_x, double * min_y, double * max_x, double * max_y)
{
  // Expand the update window to include our rectangle, so Nav2 knows
  // to recompute this region. We take the min/max of the existing
  // window and our rectangle's extents.
  *min_x = std::min(*min_x, rect_min_x_);
  *min_y = std::min(*min_y, rect_min_y_);
  *max_x = std::max(*max_x, rect_max_x_);
  *max_y = std::max(*max_y, rect_max_y_);
}

void SimpleLayer::updateCosts(
  nav2_costmap_2d::Costmap2D & master_grid,
  int min_i, int min_j, int max_i, int max_j)
{
  if (!enabled_) {
    return;
  }
  
  for (int i = min_i; i < max_i; ++i) {
    for (int j = min_j; j < max_j; ++j) {
      // Convert cell indices to world coordinates
      double wx, wy;
      master_grid.mapToWorld(i, j, wx, wy); // Convert cell indices to world coordinates

      // Check if the world coordinates are within our rectangle
      if (wx >= rect_min_x_ && wx <= rect_max_x_ &&
          wy >= rect_min_y_ && wy <= rect_max_y_) {
        // Set the cost of this cell to LETHAL_OBSTACLE
        master_grid.setCost(i, j, nav2_costmap_2d::LETHAL_OBSTACLE);
      }
    }
  }

}

}  // namespace nav_costmap_plugin

// Register this plugin with pluginlib

#include "pluginlib/class_list_macros.hpp"
PLUGINLIB_EXPORT_CLASS(nav_costmap_plugin::SimpleLayer, nav2_costmap_2d::Layer) 