# **Udacity Self-Driving Nanodegree Behavioral Cloning Project** 

---

**Behavioral Cloning Project**

The goals / steps of this project are the following:
* Use the simulator to collect data of good driving behavior
* Build, a convolution neural network in Keras that predicts steering angles from images
* Train and validate the model with a training and validation set
* Test that the model successfully drives around track one without leaving the road
* Summarize the results with a written report

## Rubric Points
### Here I will consider the [rubric points](https://review.udacity.com/#!/rubrics/432/view) individually and describe how I addressed each point in my implementation.  

---
### Files Submitted & Code Quality

#### 1. Submission includes all required files and can be used to run the simulator in autonomous mode

My project includes the following files:
* model.py containing the script to create and train the model
* drive.py for driving the car in autonomous mode
* model.h5 containing a trained convolution neural network 
* run1.mp4 video of recorded lap in autonomous mode around first track
* project_writeup.md summarizing the results and steps taken for the project

#### 2. Submission includes functional code
Using the Udacity provided simulator and my drive.py file, the car can be driven autonomously around the track by executing 
```sh
python drive.py model.h5
```

#### 3. Submission code is usable and readable

The model.py file contains the code for training and saving the convolution neural network. The file shows the pipeline I used for training and validating the model, and it contains comments to explain how the code works.

### Model Architecture and Training Strategy

#### 1. Pre-processing steps

1 - The CSV file created from recording the simulator run is loaded and then split into images and steering measurements.
2 - To account for the left and right cameras being offset, a steering correction of 0.2 was added/subtracted from the measured value and then appended to the 'steering_measurements' list.
3 - The data was then augmented and images were flipped.
4 - The images were cropped using keras.layers Cropping2D so that background distractions could be avoided.

#### 2. An appropriate model architecture has been employed

The chosen model architecture for this project was the reccommended NVIDIA model. The model consists of 9 layers, 5 of which are convolutional layers, 3 connected layers, and one normalization layer. The first 3 convolutional layers use strided convolutions of size 2x2 and a 5x5 kernel. The remaining two layers use a 3x3 kernel size.

The model includes RELU layers to introduce nonlinearity (code line 59), and the data is normalized in the model using a Keras lambda layer (code line 57). 

#### 3. Model parameter tuning

The model used an adam optimizer, so the learning rate was not tuned manually (model.py line 70).

#### 4. Splitting validation and training sets

Within 'model.fit()', the training and validation sets were split. I deceided to keep 20% of the data in the validation set and the rest in the training set.

### Model Architecture and Training Strategy

#### 1. Solution Design Approach

The overall strategy for deriving a model architecture was to not reinvent the wheel and start with a model that has proved to work hence why the decision was made to use the NVIDIA architecture. From there I wanted to challenge myself and test if the model could work with not only the provided data, but from the measured data from the manually driven runs. A combination of maneuvers were made to help the model understand what the appropriate response is.

Then I built the model and architecture and in order to gauge how well the model was working.

The final step was to run the simulator to see how well the car was driving around track one. There were a few spots where the vehicle fell off the track, such as just before the bridge and in the middle of the first corner. To improve the driving behavior in these cases, I implemented the steering corrections for the left and right images and this immediately improved the model. I also adjusted the speed within 'drive.py' to see if this would have an effect on how well the model would drive

At the end of the process, the vehicle is able to drive autonomously around the track without leaving the road.

#### 2. Final Model Architecture

The final model architecture (model.py lines 57-68) consisted of a convolution neural network that follows an architecture similar to the NVIDIA model. 

#### 3. Creation of the Training Set & Training Process

To capture good driving behavior, I first recorded two laps on track one, the first lap was center lane driving and the following lap focused on recovery from lane deviation. This was then repeated in the reverse direction. One lap was also recorded on the second track. Here is an example image of center lane driving:

[image1]: /home/workspace/CarND-Behavioral-Cloning-P3/examples/center_2019_08_19_23_21_04_148.jpg

To augment the data sat, I also flipped images and angles by multiplying the steering angle by a factor of -1. This resulted in 6 images for one set in the csv file. 

The number of epochs was 3 and I used an adam optimizer so that manually training the learning rate wasn't necessary.