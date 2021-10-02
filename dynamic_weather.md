### Dynamic weather example 

The dynamic weather example found in `./carla/PythonAPI/examples/dynamic_weather.py` creates changing weather conditions with respect to time. Changes include the position of the sun along with occasional storms. The rate at which the weather changes can also be adjusted. The default value is 1.0.

To customize the weather conditions, read here about [changing the weather](https://carla.readthedocs.io/en/0.9.12/tuto_G_retrieve_data/#change-the-weather).

#### 1. First step is to launch CARLA

In the terminal:
```
cd ./carla
CarlaUE.exe 
```

The Unreal Simulator will launch and you will see a birds-eye view of the default map, **Town10**, as shown below. Learn more about changing maps [here](https://carla.readthedocs.io/en/0.9.12/tuto_G_retrieve_data/#map-setting).

![CARLA Map](https://github.com/mark-draghicescu/Self-Driving-Car-Engineer-Nanodegree/blob/master/map_view.jpg)

*Bird's eye view of Town10*

#### 2. To run dynamic_weather.py

Open a new terminal and navigate to the directory `./carla/PythonAPI/examples` and run `dynamic_weather.py`. To adjust the ratio for the weather changes, you can include `--speed 1.0` following `dynamic_weather.py` and set the desired value.   

In the terminal:
```
cd ./carla/PythonAPI/examples
python dynamic_weather.py --speed 1.0 
```

Example of dynamic weather change (speed 50.0).

![CARLA Dynamic Weather](https://github.com/mark-draghicescu/Self-Driving-Car-Engineer-Nanodegree/blob/master/dynamic_weather.gif)

*Dynamic weather changing Town10*
