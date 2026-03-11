# Self-Balancing Robot PID Control

This is a project about developing a system that keeps a two-wheeled robot vertically stable using the PID control logic. Just note that every system has its own PID value that it responds to. You need to understand your system requirements and tune the PID parameters to meet it.

---

## The Problem

Building a two-wheeled robot is something that can be really unstable. Without an active corrective system, it will fall. This brings about the challenge of making it stand by correcting its own tilt in real time.

To solve this, I built a self-balancing robot using an Arduino board, an MPU6050 sensor, and an L298N motor driver, along with two DC motors. The controller runs every 10 milliseconds. This was done for a reason. At a slower interval, the controller was not fast enough to catch a small tilt before it became too large to recover from. Likewise, at faster intervals, there was a leakage of noise introduced by the MPU6050 to the system. But at 10 milliseconds, that stability was achieved.

---

## Hardware Components

| Component | Role |
|---|---|
| Arduino Board | The microcontroller, the brain of the system that runs the control loop logic |
| MPU6050 Sensor | Measures the tilt angle using an accelerometer and a gyroscope |
| L298N Motor Driver | Converts the PWM signals from the Arduino into what drives the motors |
| 2 x DC TT Gear Motors | Rotates the wheels to gain balance |
| 2 x 3.7V Li ion Battery | Powers the system |
| Jumper Wires | For connecting the hardware components |

---

## The Control Equation

The PID equation in continuous time domain is:

```
U(t) = Kp * e(t) + Ki * integral(e(t)dt) + Kd * de(t)/dt
```

But I had to convert it to a discrete value for the Arduino microcontroller to be able to do the mathematics with it:

```
U[n] = Kp * e[n] + Ki * sum(e[n]) + Kd * (e[n] - e[n-1])
U[n] = 30 * e[n] + 0 * sum(e[n]) + 6 * (e[n] - e[n-1])
```

Where:

Proportional term (Kp) = 30, Integral term (Ki) = 0, Derivative term (Kd) = 6

U[n] = Output PWM signal to the motors, e[n] = error, e[n-1] = previous error, sum(e[n]) = error sum

The target angle is 0°, which means I want the robot to be perfectly upright. The Kp term is responsible for keeping the robot from falling. The more the robot tilts, the more this term pushes the wheel to catch up with it. The Kd term acts as the brake of the system. When the proportional term catches up with the tilt and the velocity of the robot suddenly changes, it is the derivative term that prevents the bot from falling backward.

I realized that my controller doesn't require the integral parameter to work. When I added it, it was winding up the error and making the system more unstable, so I had to remove it. It is not always important, except when your system requires it. Mine didn't. Yours might.

---

## Tuning Sequence

This was absolutely one of the most difficult aspects of building this kind of system. You can keep tuning for days without getting the PID parameters that would work perfectly with your system. There is a sequence that worked for me:

1. Start with Kp first and tune it until it is enough to catch up quickly with the fall.
2. The second to tune is Kd, which balances any oscillation that might have been introduced by Kp and also acts as a damper for overshoot prevention. Tune until there are no more oscillations or overshoots.
3. Ki should be the last to tune. It is required to fix any persistent drift by accumulating previous errors over time. Be careful while introducing this term. You must define constraints to keep the error sum within a limit to avoid error windup.

---

## Test Results

| Test Condition | Result |
|---|---|
| Flat surface balancing | Successful |
| Steep surface balancing | Took longer to stabilise |
| 3° disturbance applied | Recovered in approximately 3 seconds |
| Maximum recoverable disturbance | 4° |
| Beyond 4° disturbance | Falls, 45° cutoff triggers motor stop |

The maximum disturbance the robot can recover from is between -4° and 4°. The reason for this is that when the tilt is steeper, gravity accelerates the fall faster than the speed at which the PID controller can calculate and apply the corrective response required within the 10 milliseconds of the loop interval. Another factor is the torque limit of TT motors, which cannot produce enough driving force to keep up with the falling robot above that angle.

---

## Runaway Protection

The solution to this came to mind when the robot kept falling and trying to speed out of reach. Sometimes, the wheel continued spinning, especially when motion was prevented by obstacles. I realized that this could result in hardware damage. I came up with a logic that once the robot has fallen beyond 45°, it has fallen beyond recovery, and then the robot should stop moving. This also automatically resets the error, the previous error, and the error sum.

```cpp
if (abs(angle) > 45) {
  move(0, 0);
  errorSum = 0;
  prevError = 0;
  error = 0;
  return;
}
```

---

## Step Response

![PID Step Response Graph](graph.png)

The graph shows the angle response after a 10° disturbance is applied. The system converges toward 0° as the PID controller applies corrective PWM signals to the motors. It is also worth noting that the 3 seconds recovery time is based on physical observation. The calculated settling time shown in the graph is shorter because it is the time it takes the system to settle within the tolerance band of ±0.2°, not physically settled.

---

## Key Things to Note

Make sure your MPU6050 is firmly screwed to the bot chassis. The bot might never be stable if this is violated.

Make sure you are using the right coordinate axis on the MPU6050. If you pick the wrong one, you won't get any useful feedback from the sensor.

It won't work on the first try and that is completely normal. It can be frustrating, just continue.

You have to keep fine tuning the P, I, D parameters. Their interaction is enough to keep the bot standing on its two wheels.

You might also need to tune your desiredAngle based on the Centre of Mass of your bot. If your battery or components are not evenly distributed, the robot's natural balance point will not be exactly 0°.

---

## Future Design

I recently read about the Kalman filter and how it can help to reduce noise to provide smoother feedback through the MPU6050. Instead of using the raw angle calculation in the future project, I'll implement this.

Using a different loop for both angle and velocity would eventually help to reduce latency. I'll be using cascaded PID in the next version.

I will also need to add a remote set point so that I can monitor the system's stability in real time.

---

---

*Every system has its own parameter values. You must understand your system to know what will work for it.*
