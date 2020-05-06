#Import needed packages
import csv
import cv2
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.image as mpimg
from scipy import ndimage

#Extract all data from logged simulator csv file
lines = []
with open ('/home/workspace/CarND-Behavioral-Cloning-P3/examples/driving_log.csv') as csvfile:
    reader = csv.reader(csvfile)
    for line in reader:
        lines.append(line)

# Remove the first line, which contains the description of each data column
lines = lines[1:]

#Update path and extract image file name/steering measurements       
images = []
steering_measurements = []

for line in lines:
    for i in range(3):
        source_path = line[i]
        filename = source_path.split('/')[-1]
        current_path = '/home/workspace/CarND-Behavioral-Cloning-P3/examples/IMG/' + filename
        image = cv2.imread(current_path)
        images.append(image)
        measurement_steer = float(line[3])
        if(i==0):
            steering_measurements.append(measurement_steer)
        elif(i==1):
            steering_measurements.append(measurement_steer+0.2)
        elif(i==2):
            steering_measurements.append(measurement_steer-0.2)

augmented_images = []
augmented_measurements = []

for image, measurement in zip(images, steering_measurements):
    augmented_images.append(image)
    augmented_measurements.append(measurement)
    augmented_images.append(cv2.flip(image,1))
    augmented_measurements.append(measurement*-1.0)
    
#Convert 'image' and 'steering measurements' to numpy arrays for Keras
x_train = np.array(augmented_images)
y_train = np.array(augmented_measurements)

#Nvidia neural network
from keras.models import Sequential, Model
from keras.layers.convolutional import Convolution2D
from keras.layers import Flatten, Dense, Lambda, Cropping2D

model = Sequential()
model.add(Lambda(lambda x: x / 255.0 - 0.5, input_shape = (160,320,3)))
model.add(Cropping2D(cropping=((70,25),(0,0))))
model.add(Convolution2D(24,5,5,subsample=(2,2),activation="relu"))
model.add(Convolution2D(36,5,5,subsample=(2,2),activation="relu"))
model.add(Convolution2D(48,5,5,subsample=(2,2),activation="relu"))
model.add(Convolution2D(64,3,3,activation="relu"))
model.add(Convolution2D(64,3,3,activation="relu"))
model.add(Flatten())
model.add(Dense(100))
model.add(Dense(50))
model.add(Dense(10))
model.add(Dense(1))

model.compile(loss='mse', optimizer='adam')
model.fit(x_train, y_train, validation_split=0.2, shuffle=True,
          nb_epoch=3) #Split data into training/validation sets

model.save('model.h5')

print("Model Saved!")