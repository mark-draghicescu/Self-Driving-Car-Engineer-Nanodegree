### Dynamic weather example 

The dynamic weather example found in `./carla/PythonAPI/examples/dynamic_weather.py` creates changing weather conditions with respect to time. The rate at
which the weather changes can be adjusted before running the script. The default value is 1.0.


It is also possible to customize the weather conditions, read more about [changing the weather](https://carla.readthedocs.io/en/0.9.12/tuto_G_retrieve_data/#change-the-weather).

#### 1. First step is to launch CARLA

In the terminal:
```
cd ./carla
CarlaUE.exe 
```

The Unreal Simulator will launch and you will see a birds-eye view of the default map as shown below. Learn more about changing maps [here](https://carla.readthedocs.io/en/0.9.12/tuto_G_retrieve_data/#map-setting).

![CARLA Map](https://github.com/mark-draghicescu/Self-Driving-Car-Engineer-Nanodegree/blob/master/map_view.jpg)

#### 2. To run dynamic_weather.py

Open a new terminal and navigate to the directory `./carla/PythonAPI/examples` and run `dynamic_weather.py`. It is also possible to adjust the ratio for the weather changes.

In the terminal:
```
cd ./carla/PythonAPI/examples
python dynamic_weather.py --speed 1.0 
```

Example of dynamic weather change (speed 50.0).
![CARLA Dynamic Weather](https://github.com/mark-draghicescu/Self-Driving-Car-Engineer-Nanodegree/blob/master/map_view.jpg)

#### 1. Following the import of the dataset files, I used the numpy.shape to retrieve the number of examples and the shape of the traffic sign image.

