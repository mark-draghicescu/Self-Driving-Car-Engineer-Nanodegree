# Udacity PID Control Project

#### The goal of this project was to build a PID controller to compute the steering angle so that the car can navigate around the track.

---

## Overview

The objective of this project was to build a PID (proportional/integral/differential) controller and tune the parameters so that the car can navigate around the track smoothly, while staying within the boundaries. The simulator provides the CTE (cross track error) and speed information. The objectives that the PID controler needed to meet include:
   
1. Stay within the edges of the road.
2. Complete at least one lap of the lake circuit.
3. Not exceed 100mph speed limit.
  
The PID controller must output with at least steering commands to drive the car reliably around the simulator track. Here is the [link](https://review.udacity.com/#!/rubrics/1972/view) to the rubric for reference.

## Reflection

The 'proportional' element of the controller gets the car to steer towards the center of the road, reducing the cross track error. The problem with only using this for the controller is that the vehicle overshoots the center and can leave the edge of the road. The car has a tendency to swerve from left to right with an increasing CTE margin until it finally leaves the track (./CarND-PID-Control-Project/videos/Only_Proportional.mov). The 'p' was set to 0.2 in this case.

The 'integral' is a total sum of the CTE's. By looking at the history of the error's, the integral term resets the bias value to eliminate offset. If the parameter is too high for I, the car tends to have quicker oscillations, and does not tend to get up to a higher speed. A low setting for I will cause the car to tend to drift to one side of the lane or the other for longer periods of time. Using only this control will cause the vehicle to steer in a circle.

The 'derivative' is the difference between the instantaneous CTE and the previous error value. This parameter helps counteract the trend to overshoot the center line by smoothing the car's approach to it, the derivative value is negative. Too high of a coefficient will also cause the car to oscillate at a high rate with large changes in steering angles (./CarND-PID-Control-Project/videos/High_Derivative.mov) A coefficient that is too low will also make the car oscillate, but it will overshoot more often.

The tuning of the hyperparamters were manually adjusted so that the car can run around the lake circuit for at least one lap. Although a Twiddle algorithm was created to try to automate the parameter optimization, it was taking a lot of time even with the simulator set to reset after a pre-determined step amount. The Twiddle algorith can be found in main.cpp and only runs if the boolean in PID.cpp is set to 'true'. The final PID controller was set to the following values, P: 0.13, I: 0.0002, D: 3.0).

The throttle was not changed although a PID controller could have also been created for this to maximize the vehicle's speed around the track.

Final video: (./CarND-PID-Control-Project/videos/Final_Movie.mov)