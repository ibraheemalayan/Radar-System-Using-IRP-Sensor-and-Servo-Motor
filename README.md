# Radar System Using IRP Sensor and Servo Motor

## Task
```
Use an IR sensor (keyes IR) with a servo motor, the sensor is attached to the motor so that it works as a rotating IR radar, the whole circuit will be inside a 20*20 cm cell and have an opening in one of its walls, your system should have two modes (automatic and manual) and a control bit to choose between the two modes, in automatic mode the system should check and report where is the opening (front, back , left, right ) through serial port to the user every 20 seconds, in manual mode your system should receive commands from the user to check a specific direction and report if its open or close. In both modes the system should display the results on an led module also.
```

## Components

* Arduino Uno
* IR Proximity Sensor
* 16x2 LCD Screen ( I2C )
* Breadboard
* Jumper Wires
* Resistors
* Joystick
* LED
* Push Button

## Circuit Diagram

![Circuit Diagram](./assets/schematics.png) # TODO

## Assembled Circuit

<p float="left">
  <img src="./assets/circuit_1.jpeg" width="300" /> # TODO
  <img src="./assets/circuit_2.jpeg" width="300" /> # TODO 
  <img src="./assets/circuit_3.jpeg" width="300" /> # TODO
</p>

          

## Project Demo

![Project Demo](https://cdn.loom.com/sessions/thumbnails/c36953c4ccf54f7eb345afdf09b8bf03-with-play.gif)    
[Watch Video](https://www.loom.com/share/c36953c4ccf54f7eb345afdf09b8bf03)

## Code

[hw3.ino](hw3.ino)