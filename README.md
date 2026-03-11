# Self-Balancing Robot PID Control

This is a project about developing a system that keeps a two-wheeled robot vertically stable using the PID control logic. Just note that every system has its own PID value that it responds to. You need to understand your system requirements and tune the PID parameters to meet them.

---

## Hardware Required

- Arduino Board
- MPU6050 Sensor
- L298N Motor Driver
- 2 x DC TT Gear Motors
- 2 x 3.7V Li ion Battery

---

## PID Values Used

```
Kp = 30  |  Ki = 0  |  Kd = 6  |  Loop interval = 10ms
```

These values worked for my system. Every system is different. You will need to tune yours.

---

## Before You Run It

- Screw the MPU6050 firmly to the chassis or the bot will never stabilise.
- Make sure you are reading from the correct MPU6050 coordinate axis.
- Adjust the desiredAngle value to match your bot's Centre of Mass.
- It won't work on the first try. Keep tuning.
