#ifndef NAV_COSTMAP_PLUGIN__SIMPLE_LAYER_HPP_  // prevents double inclusion of this header file
#define NAV_COSTMAP_PLUGIN__SIMPLE_LAYER_HPP_

#include "rclcpp/rclcpp.hpp"
#include "nav2_costmap_2d/layer.hpp"
#include "nav2_costmap_2d/costmap_layer.hpp"   // Gives CostmapLayer base class
#include "nav2_costmap_2d/layered_costmap.hpp" // Gives access to the parent costmap stack

namespace nav_costmap_plugin // namespace must match the package name for pluginlib to find the plugin 
{
class SimpleLayer : public nav2_costmap_2d::CostmapLayer
{
public:
    SimpleLayer() = default;
    
    // Called once when layer loaded. It reads params and init state.
    virtual void onInitialize() override;

    // Called on every cycle of the costmap update:
    // It updates the bounds of the costmap that need to be updated.
    virtual void updateBounds(
        double robot_x, double robot_y, double robot_yaw,
        double * min_x, double * min_y, double * max_x, double * max_y) override;
    
    // Called on every cycle of the costmap update. 
    // It updates the cost values for the claimed cells.
    virtual void updateCosts(
        nav2_costmap_2d::Costmap2D & master_grid,
        int min_i, int min_j, int max_i, int max_j) override;
    
    // Lifecycle 
    virtual void reset() override;
    virtual bool isClearable() override { return false; }

private:
    // World coordinates of the area to be updated in meters
    double rect_min_x_, rect_min_y_, rect_max_x_, rect_max_y_;
    bool need_recalculation_;
};
} // namespace nav_costmap_plugin

#endif // NAV_COSTMAP_PLUGIN__SIMPLE_LAYER_HPP_