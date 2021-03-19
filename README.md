# Image Saver

## Package Summary

ROS2 package to capture and save frames from rgb image topic (sensor_msgs/msg/Image).

* **Mainteiner status:** Mainteined
* **Mainteiner:** Fernando González <fergonzaramos@yahoo.es>
* **Author:** Fernando González <fergonzaramos@yahoo.es>
* **License:** BSD

## Usage

```
$ ros2 run image_saver image_saver_node --ros-args -p image_topic:=[string] -p freq:=[double] -p prefix_name:=[string]
```

For example, to save images whose prefix is 'trial', the camera topic is */camera/rgb/image_raw* and you want to save the frames to a frequency of 2 Hz:

```
$ ros2 run image_saver image_saver_node --ros-args -p image_topic:=/camera/rgb/image_raw -p freq:=2.0 -p prefix_name:=trial
```

It will save images like that:

```
trial_1616182640643867236.jpg  trial_1616182643641073523.jpg
trial_1616182641141531320.jpg  trial_1616182644140983536.jpg
trial_1616182641641003475.jpg  trial_1616182644641078351.jpg
trial_1616182642141174749.jpg  trial_1616182645141019454.jpg
trial_1616182642641633054.jpg  trial_1616182645641050103.jpg
trial_1616182643141471573.jpg
```

## Nodes

### image_saver_node

It is subscribed to the camera image topic (sensor_msg/msg/Image) and it save frames with .jpg format.

#### Parameters

* **image_topic:** RGB image topic. By default: */xtion/rgb/image_raw*
* **freq:** Image saving frequency. It must be a **double**. By default, 5.0 Hz
* **prefix_name:** prefix for de output file name of the image. For example, if prefix is *foo*, the output image will be named something like *foo_1616182643641073523.jpg*, where *1616182643641073523* is de system time in nanoseconds. By default, an empty prefix name is used