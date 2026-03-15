# Self-Balancing Robot PID Control

A two-wheeled robot is naturally unstable. Without active correction, it falls. This project implements a PID control system on an Arduino to keep the robot upright in real time by continuously reading the tilt angle from an MPU6050 sensor and adjusting motor speed accordingly. The results were validated using a digital twin built with serial data logged directly from the Arduino.

Read the full write up here: [What Building a Self-Balancing Robot Taught Me After Many Days of Tuning](https://www.linkedin.com/pulse/what-building-self-balancing-robot-taught-me-after-many-titilola-xlhef)

Want to see the system behaviour visualised in real time? Check out the [Self-Balancing Robot Digital Twin](https://github.com/i-am-the-robot/Self-Balancing-Robot-Digital-Twin) repository.

---

## Hardware Required

| Component | Quantity |
|---|---|
| Arduino Uno | 1 |
| MPU6050 Sensor | 1 |
| L298N Motor Driver | 1 |
| DC TT Gear Motors | 2 |
| 3.7V Li ion Battery | 2 |
| Jumper Wires | As needed |

![Arduino](Arduino_Uno.jpeg)  
Arduino Uno

![MPU](MPU6050.jpg)
MPU6050 Sensor

---

## Building and Arranging the Robot. Getting the Centre of Gravity Right

Before you wire anything or write a line of code, you need to think carefully about how you arrange the components on your robot. This is one of the most overlooked steps and it will save you a lot of tuning time later.

The goal is to get the centre of gravity as close to the centre of the robot as possible, both front to back and left arto right. Here is how to do that:

**Place heavy components in the middle.** Your battery is likely your heaviest component. Mount it as close to the central axis of the robot as you can, not at the top or bottom of the chassis. The higher the heavy components are placed, the harder the robot will be to balance.

**Mount the MPU6050 as close to the centre of the chassis as possible.** The sensor needs to be at or near the point where the robot rotates. If it is too far from the centre, the readings will not accurately represent the tilt of the whole robot.

**Keep the left and right sides symmetrical.** If one side is heavier than the other, the robot will always drift in that direction. Try to distribute your components evenly across both sides.

**Screw everything down firmly.** Nothing should move once you have decided where it goes. Any loose component will shift the centre of gravity during operation and make the system unpredictable.

**Test the balance before powering on.** Once everything is assembled, hold the robot upright and let go carefully. If it falls immediately and consistently to one side, your centre of gravity is off. Rearrange your components until the robot feels naturally balanced when standing upright.

The closer your physical build is to a balanced centre of gravity, the less you will need to compensate in software and the easier your tuning will be.

---

## Wiring

| MPU6050 Pin | Arduino Pin |
|---|---|
| VCC | 3.3V or 5V |
| GND | GND |
| SDA | A4 |
| SCL | A5 |

| L298N Pin | Arduino Pin |
|---|---|
| IN1 | Pin 4 |
| IN2 | Pin 5 |
| IN3 | Pin 6 |
| IN4 | Pin 7 |
| ENA | Pin 10 (PWM) |
| ENB | Pin 11 (PWM) |
| VCC (12V) | Battery positive |
| VS (5V) | Arduino Vin |
| GND | Battery negative and Arduino GND |

![Circuit Diagram](Self_Balacing_Robot.png)

Note: Your pin assignments may be different depending on how you wired your board. Check your code and adjust accordingly.

---

## Setting Up Arduino IDE

If you have never used Arduino IDE before, follow these steps:

1. Download and install Arduino IDE from arduino.cc
2. Connect your Arduino Uno to your computer using a USB cable
3. Open Arduino IDE
4. Go to Tools, then Board, and select Arduino Uno
5. Go to Tools, then Port, and select the port your Arduino is connected to. It will usually show as COM3 or COM4 on Windows, or /dev/ttyUSB0 on Linux

---

## Installing the Required Library

Before uploading the code, you need to install one library:

1. Open Arduino IDE
2. Go to Sketch, then Include Library, then Manage Libraries
3. In the search box, type MPU6050_light
4. Find the one by rfetick and click Install
5. Close the Library Manager once it is installed

---

## Uploading the Code

1. Download or copy the code from this repository
2. Open the code file in Arduino IDE
3. Make sure your board and port are correctly selected
4. Click the Upload button — the arrow pointing to the right at the top of the screen
5. Wait for it to finish. You will see Done uploading at the bottom when it is complete

---

## Setting the Centre of Gravity Before Turning On

This step is critical. Before you turn the robot on, physically stand it upright and find the position where it feels naturally balanced without falling to either side. Hold it gently, find that position, and then turn it on.

When the robot powers on, the MPU6050 reads that initial position as 0°. This becomes the reference angle the controller will always try to return to. If you turn the robot on while it is tilted, it will use that wrong position as its target and it will never balance properly.

If the robot keeps drifting in one direction even after tuning, adjust the desiredAngle value in the code slightly until the robot holds its position. This compensates for any remaining imbalance in the physical build.

---

## PID Values Used

```
Kp = 30  |  Ki = 0  |  Kd = 6  |  Sampling time T = 0.01s (10ms)
```

These values worked for my system. Every system is different. You will need to tune yours.

---

## Tuning Sequence

This is the sequence that worked for me. Follow this order and it will reduce your tuning time significantly.

1. Start with Kp. Tune it until the bot responds fast enough to catch a fall without oscillating.
2. Tune Kd next. This dampens the oscillation introduced by Kp and prevents overshoot. Tune until the system is smooth.
3. Tune Ki last. Only add it if your system has a persistent drift that Kp and Kd cannot fix. Be careful — too much Ki will cause error windup and make the system unstable. Mine worked without it.

---

## Serial Output

The code prints the following to Serial Monitor at 9600 baud:

```
Angle: X    Error: Y    U: Z
```

To open the Serial Monitor in Arduino IDE, go to Tools and click Serial Monitor. Make sure the baud rate is set to 9600.

You can use this output to monitor your system in real time or connect it to the digital twin visualiser.

---

## Key Things to Note

Make sure your MPU6050 is firmly screwed to the chassis. If it moves even slightly, the bot will never stabilise.

Make sure you are reading from the correct coordinate axis on the MPU6050. The wrong axis means no useful feedback.

It will not work on the first try. It can be frustrating. Just keep tuning.

---

## Results

| Test | Result |
|---|---|
| Flat surface | Stable |
| Slightly inclined surface | Took longer to stabilise |
| 3° disturbance | Recovered in approximately 3 seconds |
| Maximum recoverable disturbance | 4° |
| Beyond 4° | Falls, 45° cutoff stops the motors |

![Step Response Graph](graph.png)

---

## Future Work

- Kalman filter for smoother MPU6050 angle feedback
- Cascaded PID with separate loops for angle and velocity
- Wireless communication for remote monitoring and control

---

*Every system has its own parameter values. You must understand your system to know what will work for it.*
