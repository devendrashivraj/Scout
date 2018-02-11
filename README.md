# Scout
A Robotic Pet that looks for people. Its brains are a Raspberry Pi, Arduino Uno, and a TB6612FNG motor driver. Later on in the project, we added an Adafruit Bluetooth friend to allow RC driving from a phone.

<img src="https://user-images.githubusercontent.com/6588879/36074524-e816b15c-0f0e-11e8-8e77-f9169f4f8b18.jpeg" width="440" height="420"> <img src="https://user-images.githubusercontent.com/6588879/36074525-e8320c2c-0f0e-11e8-9f09-e105df131e34.jpeg" width="440" height="420">

# The Pi Side
Using the Pi Camera and the PIL library, images taken of the environment in front of Scout to be analyzed in the AWS cloud. This was done by splitting the image into thirds and checking for humans in any of the sectors. The robot then turns towards the sector that the person was in, to move towards them. The Amazon Rekognition platform was used to return labels about each of the sections in the image.

# The Arduino Side
The Arduino Uno received commands from the Raspberry Pi through I2C and used the TB6612FNG motor driver to control the robot's motion, and the range of view of the ultrasonic sensor (could not be used at the same time though, because it also communicated on the I2C protocol). The robot used two optical sensors to prevent falling off edges, and colliding with walls. 


