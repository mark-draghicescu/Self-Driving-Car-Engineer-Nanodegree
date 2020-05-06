# Udacity Path Planning Project

#### The goal of this project was to build a path planner that creates smooth and safe trajectories for the car to follow on a highway.

---

## Overview

The project uses a simulator to replicate a highway driving scenario. The road has other vehicles moving at different speeds, ~50mph, in different lanes. The objectives that the path planner needed to meet include:
   
1. Stay as close to 50mph speed limit without going over it.
2. Not exceed 10 m/s^2 in jerk and total acceleration.
3. Drive inside the lane lines, except when changing lanes.
4. Avoid all collisions with traffic.
5. Safely change lanes if slower car in same lane.
  
The car transmits its location, along with its sensor fusion data, which estimates the location of all the vehicles on the same side of the road. Also given are a list of waypoints around the highway. From this information, it was possible to create a planner that allows the car to drive around the track and meet the above conditions. The project must meet all points in the [rubric](https://review.udacity.com/#!/rubrics/1971/view).

## Implementation

The main.cpp file contains most of the code for the path planner. Some helper functions were used along with the spline.h file. A couple of different methods were tried for the path generation. To begin with, a decision was made to try to use a quintic polynomial solver to generate an optimal trajectory. After several trials, the car was staying within the lane at the correct speed, but there was still the problem of high jerk accelerations at each waypoint. Because of this, I chose to go with the cublic spline method as shown in Udacity's walkthough [video](https://www.youtube.com/watch?v=7sI3VHFPP0w).

The implementation is divided into two sections:

1. Prediction and decision based on sensor fusion data
2. Generation of the vehicle's trajectory from waypoint and previous path information.

Below is the code that uses the given sensor fusion information to help the ego vehicle decide where it needs to place itself. By using several booleans, it is possible to create the conditions for when the car can overtake or change lanes. First several lane variables were created based on the 'd' value of the car in question. Using these variables, three booleans were generated which are set to 'true' if the car in the right or left next lane are too close for a lane change maneuver. In this case, the gap was set to 30m. Also added was a boolean that determined if the ego vehicle got too close to the vehicle in front. If it does, then the speed is reduced until there is big enough gap for when it can get back up to the speed limit.

### Prediction and Decision

'''prediction_and_decision (lines 107-172 of main.cpp)
//main.cpp 

    bool too_close = false;
    bool car_left = false;
    bool car_right = false;
     
    for (int i = 0; i < sensor_fusion.size(); i++) {
     
        float d = sensor_fusion[i][6];
       
        //Identify which lane each car is in.
    
        int car_lane;
     
        if (d >= 0 && d < 4) {
            car_lane = 0;
        }
        else if (d >= 4 && d < 8) {
            car_lane = 1;
        }
        else if (d >= 8 && d < 12) {
            car_lane = 2;
        }
        else {
            continue;
        }
      
        double vx = sensor_fusion[i][3];
        double vy = sensor_fusion[i][4];
        double check_speed = sqrt(vx*vx+vy*vy);
        double check_car_s = sensor_fusion[i][5];
         
        check_car_s += ((double)prev_size*.02*check_speed);

        // Identify whether the car is ahead, to the left, or to the right
    
        if (car_lane == lane) {
        // Another car is ahead
            too_close |= (check_car_s > car_s) && ((check_car_s - car_s) < 30);
        } 
        else if (car_lane - lane == 1) {
        // Another car is to the right
            car_right |= ((car_s - 30) < check_car_s) && ((car_s + 30) > check_car_s);
        } 
        else if (lane - car_lane == 1) {
        // Another car is to the left
            car_left |= ((car_s - 30) < check_car_s) && ((car_s + 30) > check_car_s);
        }
     }
     
     if (too_close) {
         // Decrease velocity if car ahead in same lane is too close in front.
         ref_vel -= 0.224;
         // Change lane if next lanes are clear of any other cars.
         if (!car_right && lane < 2) {
             lane += 1;
         }
         else if (!car_left && lane > 0) {
             lane -= 1;
         }
     }
    
     //Get back up to speed limit if velocity is too low.
     else if (ref_vel < 49.5) {
       ref_vel += 0.224;
     }
     
     //Move back to the middle lane if clear of cars.
     else {
       if (lane != 1) {
         if ((lane == 2 && !car_left) || (lane == 0 && !car_right)) {
           lane = 1;
         }
       }
     }

'''

### Trajectory Generation Using Splines

Lines 174-252 of main.cpp creates the spline and correctly distanced points along that spline so that the vehicle can travel at the correct speed. Using the spline.h file, the vehicle's position, the waypoints in the map, and previous path information, it was possible to create a smooth trajectory that allows the vehicle to stay within acceleration and jerk limits.

'''trajectory_generation (lines 174-252 of main.cpp)
//main.cpp 

    vector<double> next_x_vals;
    vector<double> next_y_vals;
    vector<double> ptsx, ptsy;
    double ref_x = car_x, ref_y = car_y, ref_yaw = deg2rad(car_yaw);
    
    //Initialized step, no previous path yet.
    if (prev_size < 2) {
    
        double prev_car_x = car_x - cos(car_yaw);
        double prev_car_y = car_y - sin(car_yaw);

        //Use two points to make path tangent to the car.
        ptsx.push_back(prev_car_x);
        ptsx.push_back(car_x);

        ptsy.push_back(prev_car_y);
        ptsy.push_back(car_y);
    }
    
    //Using previous path's endpoint as starting point for new path.
    else {
        ref_x = previous_path_x[prev_size - 1];
        ref_y = previous_path_y[prev_size - 1];

        double ref_x_prev = previous_path_x[prev_size - 2], ref_y_prev = previous_path_y[prev_size - 2];
        ref_yaw = atan2(ref_y - ref_y_prev, ref_x - ref_x_prev);

        ptsx.push_back(ref_x_prev);
        ptsx.push_back(ref_x);

        ptsy.push_back(ref_y_prev);
        ptsy.push_back(ref_y);
    }

    //Using Frenet, add 30m evenly spaced points ahead of the starting reference
    vector<double> next_wp0 = getXY(car_s + 30, (2 + 4 * lane), map_waypoints_s, map_waypoints_x, map_waypoints_y);
    vector<double> next_wp1 = getXY(car_s + 60, (2 + 4 * lane), map_waypoints_s, map_waypoints_x, map_waypoints_y);
    vector<double> next_wp2 = getXY(car_s + 90, (2 + 4 * lane), map_waypoints_s, map_waypoints_x, map_waypoints_y);

    ptsx.push_back(next_wp0[0]);
    ptsx.push_back(next_wp1[0]);
    ptsx.push_back(next_wp2[0]);

    ptsy.push_back(next_wp0[1]);
    ptsy.push_back(next_wp1[1]);
    ptsy.push_back(next_wp2[1]);

    //Shift car refernece angle to 0 degrees.
    for (int i = 0; i < ptsx.size(); ++i) {
        double shift_x = ptsx[i] - ref_x, shift_y = ptsy[i] - ref_y;

        ptsx[i] = (shift_x * cos(0 - ref_yaw) - shift_y * sin(0 - ref_yaw));
        ptsy[i] = (shift_x * sin(0 - ref_yaw) + shift_y * cos(0 - ref_yaw));
    }

    //Create a spline and set x, y coordinates
    tk::spline s;
    s.set_points(ptsx, ptsy);

    //Start with the previous path points
    for (int i = 0; i < previous_path_x.size(); ++i) {
        next_x_vals.push_back(previous_path_x[i]);
        next_y_vals.push_back(previous_path_y[i]);
    }

    //Compute the distance between spline points so car travels at correct speed.
    double target_x = 30.0;
    target_y = s(target_x); 
    target_dist = sqrt(pow(target_x, 2) + pow(target_y, 2));
    double x_add_on = 0.0;

    //Output 50 points for path planner.
    for (int i = 1; i <= 50 - previous_path_x.size(); ++i) {
        double N = (target_dist / (.02 * ref_vel / 2.24));
        double x_point = x_add_on + target_x / N;
        double y_point = s(x_point);

        x_add_on = x_point;

        double x_ref = x_point, y_ref = y_point;

        x_point = (x_ref * cos(ref_yaw) - y_ref * sin(ref_yaw));
        y_point = (x_ref * sin(ref_yaw) + y_ref * cos(ref_yaw));

        x_point += ref_x;
        y_point += ref_y;

        next_x_vals.push_back(x_point);
        next_y_vals.push_back(y_point);
    }
'''

The first part of the code block defines the conditions of whether the car has traveled along the first generated path or if it is just starting out. If the vehicle has not yet moved, the car's current position is used instead of previous waypoints. The helper getXY function is used to generate three points spaced evenly at 30m, 60m, and 90m in front of the car's position. Before the spline could be generated, the coordinates needed to be shifted to local car coordinates. After the spline is generated and the correct spacing between points has been calculcated, 50 waypoints are created. If the car needs to change lanes, the spline will automatically change its form so that the car can smoothly go to the middle of the next lane over.