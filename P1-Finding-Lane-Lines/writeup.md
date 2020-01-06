# **Finding Lane Lines on the Road** 

---

**Finding Lane Lines on the Road**

The goals / steps of this project are the following:
* Make a pipeline that finds lane lines on the road
* Reflect on your work in a written report

---

### Reflections

### 1. Pipeline breakdown and description.

My pipeline consisted of seven steps.

1. First, I converted the images to grayscale using greyscale helper function and assigned it to the variable "grey."

2. Second step consisted of applying a Gaussian smoothing to the uploaded image with predefined kernel size.

3. Used canny helper function with low and high thresholds to find image edges.

4. In this step, dimensions for a four sided polygon were given. Used region_of_interest() function to isolate the desired area.

5. Defined Hough transform parameters for best visibility of lane lines.

6. Run hough_lines() function on edge detected image.

7. Run weighted_image() function before returning final result.

In order to draw a single line on the left and right lanes, I modified the draw_lines() function as follows:
1. Compute the slope of each input segment and separate them into left and right by the sign of the slope value.
2. Ignore the segments that are too close to horizontal. Also ignore the segments that are too close to vertical.
3. Compute the final left and right slope as the average of the slope of all the segments.
4. Compute the endpoints of the lines by taking the min/max of the endpoints of the segments.
5. Extrapolate and draw the low points of the line.


### 2. Identify potential shortcomings with your current pipeline


One shortcoming is the arbitrary epsilon values, which work for the provided test images and videos, but may not be the best values in another case.  

Another shortcoming is the shaking of the drawn lines on video. This likely is the result of running the draw_lines() function on each image in the series that make up a video.


### 3. Suggest possible improvements to your pipeline

A possible improvement would be to assign epsilon values that do not need manual manipulation and guessing.
