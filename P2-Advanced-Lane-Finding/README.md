# Advanced Lane Finding Project

---

**Advanced Lane Finding Project**

The goals / steps of this project are the following:

* Compute the camera calibration matrix and distortion coefficients given a set of chessboard images.
* Apply a distortion correction to raw images.
* Use color transforms, gradients, etc., to create a thresholded binary image.
* Apply a perspective transform to rectify binary image ("birds-eye view").
* Detect lane pixels and fit to find the lane boundary.
* Determine the curvature of the lane and vehicle position with respect to center.
* Warp the detected lane boundaries back onto the original image.
* Output visual display of the lane boundaries and numerical estimation of lane curvature and vehicle position.

## Overview

For this project, the objective was to create a pipeline that identifies the lane line boundaries from a front facing camera. Beginning with the camera calibration up until the final video that displays the boundary area. The output examples for each step of the proejct can be found in the links above or in the /output_images folder.

---

## Writeup / README

### Described below are the steps taken for the project and a description of how each rubric point was addressed. 

### Camera Calibration

#### STEP 1 and STEP 2: Computing the camera matrix and distortion coefficients and applying a distortion correction to a test image.

The code for this step is contained in the second code cell of the IPython notebook located in "./Advanced Lane Finding.ipynb"  

I start by defining a 'calibrate_camera' function that reads each chessboard image and eventually outputs the camera matrix and distortion coefficients. First the `imgpoints` and `objpoints` are defined as an empty array. The `objpoints` will be the (x, y, z) coordinates of the chessboard corners in the world. Here I am assuming the chessboard is fixed on the (x, y) plane at z=0, such that the object points are the same for each calibration image.  Thus, `objp` is just a replicated array of coordinates, and `objpoints` will be appended with a copy of it every time I successfully detect all chessboard corners in a test image.  `imgpoints` will be appended with the (x, y) pixel position of each of the corners in the image plane with each successful chessboard detection.  

For each calibration image, the image is read in and converted to grayscale. From there the chessboard corners could be found using cv2.findChessboardCorners. The function then uses the `cv2.calibrateCamera()` to compute the camera calibration and distortion coefficients. I applied the distortion correction to one of the test images using the `cv2.undistort()` function and obtained this result: 

[image1]: ./output_images/Undistorted_Example.png "Undistorted"

### Pipeline (single images)

#### STEP 3. Create a thresholded binary image using color transforms, gradients, and other methods.

In this step, several different functions are defined to calculate different thresholds and gradients to identify particular pixels. These functions are then combined to isolate the desired pixels (left and right lane lines). 

##### 1. Distortion-corrected image example.

Below is an example of a distortion correction being applied to one of the test images. The same image can also be found in the main project notebook.

[image2]: ./output_images/Undistorted_Test_Image.png "Undistorted Test Image"

##### 2. Define sobel function that takes in 'x' and 'y' inputs for the orientation.

The function `abs_sobel_thresh` takes in an image and outputs one of two images depending on orientation input. Below is an example of the image with 'x' orientation compared with the original.

[image3]: ./output_images/X_Orientation_Sobel.png "Sobel X Orientation"

Same function except with 'y' orientation selected and again compared with the original.

[image4]: ./output_images/Y_Orientation_Sobel.png "Sobel Y Orientation"

##### 3. Define function that computes the magnitude of the gradient and applies a threshold.

Another function that applies threshold on the magnitude of the gradient. Takes in a kernel value and threshold range (0, 255). Below is the binary image compared with original, 'sobel_kernel = 15' and 'mag_thresh = (100, 220)'.

[image5]: ./output_images/Magnitude_Threshold.png "Magnitude Threshold"

##### 4. Define a directional threshold function.

For this function, the lane edges can be isolated by orientation. The directional range is from 0 to 'np.pi/2'.

[image6]: ./output_images/Directional_Threshold.png "Directional Threshold"

##### 5. Creating a color threshold function using HLS space.

In here, the input image is converted to HLS colorspace and a threshold is applied to the 'S' channel. This channel seems to work best for finding lane lines. 

[image7]: ./output_images/HLS_Threshold.png "HLS Colorspace Threshold"

##### 6. Creating a color threshold function using HLS space.

I used a combination of color and gradient thresholds to generate a binary image. The sobel 'grad_x', directional threshold, and color threshold functions were put together in a pipeline and the below binary image was produced.

[image8]: ./output_images/Combined_Pipeline_Thresholds.png "Binary Example"

#### STEP 4: Apply a perspective transform to rectify each image to a "birds-eye view".

The code for my perspective transform includes a function called `warp()`, which appears in the 16th code cell of the notebook.  The `warp()` function takes as inputs an image (`img`), as well as source (`src`) and destination (`dst`) points. I chose the source points by drawing picking four coordinates that make up the lane boundary area. The destination points were arbitrary chosen to best represent the "birds-eye" view. 

This resulted in the following source and destination points:

| Source        | Destination   | 
|:-------------:|:-------------:| 
| 280, 700      | 250, 720      | 'bottom_left'
| 600, 460      | 250, 0        | 'top_left'
| 725, 460      | 1065, 0       | 'top_right'
| 1125, 700     | 1065, 720     | 'bottom_right'

I verified that my perspective transform was working as expected by drawing the `src` and `dst` points onto a test image and its warped counterpart to verify that the lines appear parallel in the warped image.

[image9]: ./output_images/Perspective_Transform.png "Warped Image"

#### 4. STEP 5: Identify lane line pixels in the corrected binary image and fit with a curved functional form.

For this step, the lane line pixels had to be identified before a functional form could be applied. This was done by creating a histogram for the bottom half of the warped image. A visualization of the histogram can be found in the 18th code cell of the notebook.

From there, the starting left and right positions of the lanes were selected from the 'x' position of each of the two peaks. The 'Sliding Window' method was used to identify the coordinates of both the left and right lanes. This was done in a function called `find_lane_pixels()` which takes an image as an input and outputs x, y coordinates for both the left and right lanes. The code for this function can be found in the 19th code cell of the notebook.

The last part involved creating a function called `fit_polynomial()`, which calculates a second order polynomial from the previously found coordinates. This was done via the 'np.polyfit' Numpy function. Below is an output example that shows the sliding windows and curved functional form.

[image10]: ./output_images/Sliding_Windows_and_Polynomial.png "Sliding Windows and Polynomial Function"

Once the lines were found, another function was created called `search_around_poly()`, which uses the previously calculated the fitted lines to identify the lines for the next image in line. Below is another output example that shows this function being applied.

[image11]: ./output_images/Detect_Lines_Polynomial.png "Detecting Lines From Prior and Polynomial Function"

#### STEP 6: Calculate the radius of curvature of the lane and the position of the vehicle with respect to center.

I did this in the 25th and 26th code cells of the main notebook. The `measure_curvature_real()` function calculates the curvature of the polynomial functions in meters. In the 27th and 28th code cells, a function called `car_offset()` was created to find the position of the car with respect to the center of the image.

#### STEP 7: Provide an example image of your result plotted back down onto the road such that the lane area is identified clearly.

I implemented this step in code cell #29 and #30 in my notebook with a function called `draw_result()`. Also in the following code cells, another function was created, `add_metrics()`. This function displays the calculated values from `measure_curvature_real()` and `car_offset()` onto the image. Here is an example of my result on a test image:

[image12]: ./output_images/Color_Fit_Lanes.png "Colored Lane Area and Metrics"

---

### Pipeline (video)

#### Step 8: Create a final video where the lanes are identified in each frame and provide a link to the output.

Here's a [link to my video result](./project_video_solution.mp4)

---

### Discussion

#### Briefly discuss any problems / issues you faced in your implementation of this project.  Where will your pipeline likely fail?  What could you do to make it more robust?

This project provided several challenges and obstacles that needed overcoming. Given the time constraints and available knowledge, I took a more simple approach to this project and it is not as robust as it needs to be. There are still some areas that could be improved such as:

- In the image warping function, the corners were chosen arbitrarly and might only work for the test images. A smarter way to have gone about this would have been to automatically detect four source points in an image based with an edge or corner detection algorithm. This will be something to try out in the future.

- In the ProcessImage class, the sliding window function, `find_lane_pixels()` is only used for the first image in the set and `search_around_poly()` for all of the consecutive images. To make this more robust, I would change the function so that if the `search_around_poly()` function fails to identify the lane lines, it would go back to using the sliding windows.

- Finally the combined thresholds do a good job of finding the lane pixels, but it might also detect the edges around other white cars. This could cause an issue if said car is closer in front of the camera or in the adjacent lane.

This project was a challenge, but also very rewarding to get the final video to work. 
