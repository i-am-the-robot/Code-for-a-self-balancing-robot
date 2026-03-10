#Self-balancing-robot-PID
This is a code developed to keep a two wheeled robot vertically stable using the PID control logic. Just not that every system has its own PID value that it responds to. You need to understand your system requirements and tune the PID parameters to meet it.  

Key points.
* Make sure your MPU6050 gyroscope is wheel screwed to the bot chassis, the bot might never be stable if this is violated.
* Make sure you ar using the right coordinate, else you won't get any feedback.
* It won't work once, trust me. It can be fraustrating, just continue.
* You have to keep fine-tuning the P, I, D parameter thier interactive is enough to keep the bot standing on its two wheels.
*You might also need to tune your desireAngle based on the Center of Mass of your bot. 
