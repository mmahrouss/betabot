# Betabot Obstacle avoider

- [Betabot Obstacle avoider](#betabot-obstacle-avoider)
  - [Project Description](#project-description)
  - [After you implement the pkg state your reflection below](#after-you-implement-the-pkg-state-your-reflection-below)
    - [How did you plan the task?](#how-did-you-plan-the-task)
    - [what is your logic?](#what-is-your-logic)
    - [What ROS REPs did you used?](#what-ros-reps-did-you-used)
    - [How we could increase the overall performance?](#how-we-could-increase-the-overall-performance)
    - [List the most time consuming problems you faced](#list-the-most-time-consuming-problems-you-faced)
    - [Demo](#demo)
    - [Screenshot](#screenshot)
      - [NAME:](#name)
      - [ID:](#id)

## Project Description 

Create a ROS package with custom nodes c++/python to move the
betabot randomly in gazebo, the movement should be triggered then the robot
moves randomly while avoid objects based on laser scans reading based on the laser
scans.


>NOTE: DON'T process one ray of the laser scans array or it will be considered ultrasonic/IR sensor.try to come up with approach thats use the laser full potential. 

>To make you project standout try not to visit any place twice.

---

## After you implement the pkg state your reflection below

### How did you plan the task?

I planned a random movement by first learning about how to use the random generators in the STL. Then deciding on how to introduce randomness to the movement in the callback of a subscriber to the scan topic.
### what is your logic?
When there is no obstacle detected (nothing on a distance less than 0.3), the robot moves forward and turns randomly cw or ccw. In case an obstacle is found the robot moves opposite to the obstacle's direction until it is far enough then continues. 
### What ROS REPs did you used?
The general REP and c++ Rep.

### How we could increase the overall performance?

Intoduce a distribution on the map and assign higher probabilities in directions that are not explored before.

### List the most time consuming problems you faced

Tuning the delays and making sense of c++ vector iterators. 

---

### Demo
Add unlisted youtube/drive video

[Demo](https://youtu.be/zcYE-ajUVS8)

<!-- ### Screenshot

[image](yourscreenshot) -->

---

#### NAME: Mohamed Adham Mahrous
#### ID: 201601078

---
