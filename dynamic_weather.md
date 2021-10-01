**Dynamic weather example**

The dynamic weather example found in (carla/PythonAPI/examples/dynamic_weather.py) creates changing weather conditions with respect to time. The rate at
which the weather changes can be adjusted before running the script. The default value is 1.0

## Rubric Points

### Here I will consider the [rubric points](https://review.udacity.com/#!/rubrics/481/view) individually and describe how I addressed each point in my implementation.  

---
### Writeup / README

#### 1. Provide a Writeup / README that includes all the rubric points and how you addressed each one. You can submit your writeup as markdown or pdf. You can use this template as a guide for writing the report. The submission includes the project code.

You're reading it! and here is a link to my [project code](https://github.com/udacity/CarND-Traffic-Sign-Classifier-Project/blob/master/Traffic_Sign_Classifier.ipynb)

### Data Set Summary & Exploration

#### 1. Following the import of the dataset files, I used the numpy.shape to retrieve the number of examples and the shape of the traffic sign image.

* The size of training set is 34799
* The size of the validation set is 4410
* The size of test set is 12630
* The shape of a traffic sign image is (32, 32, 3)
* The number of unique classes/labels in the data set is 43

#### 2. Include an exploratory visualization of the dataset.

Two different displays were created for the dataset visualization. The first being a sample image to display along with its associated label. Image example shown below.

[image1]: ./examples/traffic_sign_example.png "Traffic Sign Example Wild Animals Crossing"

For the next part, a bar chart was used to visualize the distribution of classes among the three data sets. The y-axis is the number of images and x-axis is the range of classes from 1 - 43.

[image2]: ./examples/data_distribution.png "Data Distribution"

### Design and Test a Model Architecture

#### 1. Describe how you preprocessed the image data. What techniques were chosen and why did you choose these techniques? Consider including images showing the output of each preprocessing technique. Pre-processing refers to techniques such as converting to grayscale, normalization, etc. (OPTIONAL: As described in the "Stand Out Suggestions" part of the rubric, if you generated additional data for training, describe why you decided to generate additional data, how you generated the data, and provide example images of the additional data. Then describe the characteristics of the augmented training set like number of images in the set, number of images for each class, etc.)

As a first step, I shuffled the examples by using shuffle from the sklearn.utils library. The reason for doing this is so the model does not train itself with the same set of data and therefore reduce its accuracy.

I did attempt grayscaling the image, but it did not result in a better accuracy so the only step taken was to normalize by doing 'data / 255 * 0.8 + 0.1' for each value. This resulted in a much improved accuracy.

#### 2. Describe what your final model architecture looks like including model type, layers, layer sizes, connectivity, etc.) Consider including a diagram and/or table describing the final model.

The final model architecture used is the same as the one from the LeNet example in Lesson 8, but with a few tweaks. The depth input was changed from 1 to 3 channels and the number of layers that were used in the lesson, 10, was changed to 43.

My final model consisted of the following layers:

| Layer         		|     Description	        					| 
|:---------------------:|:---------------------------------------------:| 
| Input         		| 32x32x3 RGB image   							| 
| Convolution 1     	| 1x1 stride, valid padding, outputs 28x28x6 	|
| RELU					|												|
| Max pooling	      	| 2x2 stride, valid padding, outputs 14x14x6	|
| Convolution 2 	    | 1x1 stride, valid padding, outputs 10x10x16	|
| RELU          		| 	            								|
| Max pooling			| 2x2 stride, valid padding, outputs 5x5x16		|
| Flatten   			| output = 400 								    |
| Fully connected		| input = 400, output = 120						|
| RELU		            |                       						|
| Fully connected		| input = 120, output = 84						|
| RELU		            |                       						|
| Fully connected		| input = 84, output = 43						|

#### 3. Describe how you trained your model. The discussion can include the type of optimizer, the batch size, number of epochs and any hyperparameters such as learning rate.

To train the model, I used 50 epochs, a batch size of 128, and a learning rate of 0.001.

#### 4. Describe the approach taken for finding a solution and getting the validation set accuracy to be at least 0.93. Include in the discussion the results on the training, validation and test sets and where in the code these were calculated. Your approach may have been an iterative process, in which case, outline the steps you took to get to the final solution and why you chose those steps. Perhaps your solution involved an already well known implementation or architecture. In this case, discuss why you think the architecture is suitable for the current problem.

For my training optimizers, I used softmax_cross_entropy_with_logits to get a tensor representing the mean loss value. From this  the function, tf.reduce_mean, was used to compute the mean of values across the result dimensions. Lastly optimizer.minimize was used on the previous result.

My final model results were:
* training set accuracy of 0.998
* validation set accuracy of 0.98
* test set accuracy of 0.906
 
### Test a Model on New Images

#### 1. Choose five German traffic signs found on the web and provide them in the report. 

Here are ten German traffic signs that I found on the web:

[image3]: ./new_images/image_1.png "Right-of-way at next intersection"
[image4]: ./new_images/image_10.png "Yield"
[image5]: ./new_images/image_2.png "Children crossing"
[image6]: ./new_images/image_3.png "Priority road"
[image7]: ./new_images/image_4.png "Speed liit (60 km/h)"
[image8]: ./new_images/image_5.png "No vehicles"
[image9]: ./new_images/image_6.png "Right-of-way at next intersection"
[image10]: ./new_images/image_7.png "Snow/ice warning"
[image11]: ./new_images/image_8.png "End of speed restriction"
[image12]: ./new_images/image_9.png "No stopping"

#### 2. Discuss the model's predictions on these new traffic signs and compare the results to predicting on the test set. At a minimum, discuss what the predictions were, the accuracy on these new predictions, and compare the accuracy to the accuracy on the test set (OPTIONAL: Discuss the results in more detail as described in the "Stand Out Suggestions" part of the rubric).

Here are the results of the prediction:

| Image			        |     Prediction	        					| 
|:---------------------:|:---------------------------------------------:| 
| Right-of-way  		| Right-of-way    		                    	| 
| Yield     			| Yield 										|
| Children crossing		| Children crossing								|
| Priority road	      	| Priority road					 				|
| Speed limit 60 km/h	| Speed limit 60 km/h      						|
| No vehicles       	| No vehicles            						|
| Snow/ice warning      | Right-of-way          						|
| End speed restriction | End speed restriction        					|
| No stopping           | Keep right            						|

The model was able to correctly guess 8 of the 10 traffic signs, which gives an accuracy of 80%. 

Some of the images in the set may be more difficult to classify then the others. In this set, the classifier predicted image_10 and image_12 incorrectly. In image_10 part of the sign is covered, which might have made it more difficult for the classifier to predict. The bad prediction of the snow/ice warning and no stopping sign could also be a result of the smaller quantity of examples for these kind of images in the training samples.

#### 3. Describe how certain the model is when predicting on each of the five new images by looking at the softmax probabilities for each prediction. Provide the top 5 softmax probabilities for each image along with the sign type of each probability. (OPTIONAL: as described in the "Stand Out Suggestions" part of the rubric, visualizations can also be provided such as bar charts)

For the first image, the model is 100% confident that the sign is a 'Right-of-way', which is correct. The top five soft max probabilities were:

| Probability         	| Prediction	        					    | 
|:---------------------:|:---------------------------------------------:| 
| 1.0        			| Right-of-way at next intersection  		    | 
| 0.0     				| Beware of ice/snow 							|
| 0.0					| Slippery road									|
| 0.0	      			| Dangerous curve to the right	 				|
| 0.0				    | Traffic signals      							|


For the second image, the model is 100% confident again that the sign is a 'No entry' and this is also correct. The top five soft max probabilities were:

| Probability         	| Prediction	        					    | 
|:---------------------:|:---------------------------------------------:| 
| 1.0        			| No entry  		                            | 
| 0.0     				| Double curve      							|
| 0.0					| Speed limit (30 km/h)							|
| 0.0	      			| Beware of ice/snow	 			        	|
| 0.0				    | Right-of-way at next intersection     		|

For the third image, the model is once again 100% confident again that the sign is a 'Yield' and this is correct. The top five soft max probabilities were:

| Probability         	| Prediction	        					    | 
|:---------------------:|:---------------------------------------------:| 
| 1.0        			| Yield     		                            | 
| 0.0     				| Speed limit (60 km/h)      					|
| 0.0					| No passing         							|
| 0.0	      			| No vehicles       	 			        	|
| 0.0				    | Priority road                         		|

There was only one image in the set that the model had a more even split in the softmax probabilities, which was 'image_7'. The model still correctly predicted this image, but it was close to 50:50 between 'Speed limit 60 km/h' and 'Speed limit 80 km/h'.

| Probability         	| Prediction	        					    | 
|:---------------------:|:---------------------------------------------:| 
| 0.53        			| Speed limit (60 km/h)    		                | 
| 0.47    				| Speed limit (80 km/h)      					|
| 0.0					| Speed limit (50 km/h)							|
| 0.0	      			| Speed limit (30 km/h)	 			        	|
| 0.0				    | Speed limit (100 km/h)        
