# Autonomous Claw Project
This is a collection of the programming work that went into the autonomous claw project for UBC's APSC 101, Introduction to Engineering II. The goal of the project was to semi-autonomously pick up objects of different sizes and weights and transport them to a dropoff location accurately and efficiently.

## Programs
[`studio_sonar_servo.ino`](studio_sonar_servo.ino) - An initial implementation of controlling a servo motor based on sonar distances

[`sonar_servo.ino`](sonar_servo.ino) - A more refined sonar and servo program using the [switch...case](https://www.arduino.cc/reference/tr/language/structure/control-structure/switchcase/) framework, later used in the final program

[`claw.ino`](claw.ino) - The final claw code that incorporates all of my learning and hard work

## Using My Code
Please refer to the [`LICENSE`](LICENSE) if you are using any of my code for your own project

## The Claws
Our group made two claws to use in the competiton. The first, namde Huggey Jr., was a combination of early prototypes and it performed quite well. For this reason, our second claw was just a refined version, named Huggey Sr., that was built using more precise measurements and a longer sonar arm so that there would be less interference. Here are some photos of the claws side by side:

<p align="center">
  <img alt="Huggeys Standing" src="https://github.com/ElioDiNino/Autonomous-Claw/blob/main/Photos%20%26%20Videos/huggeys_standing.jpg" width="47%">
&nbsp; &nbsp; &nbsp;
  <img alt="Huggeys" src="https://github.com/ElioDiNino/Autonomous-Claw/blob/main/Photos%20%26%20Videos/huggeys.jpg" width="47%">
</p>


## Demonstrations
This first video shows me testing the [`studio_sonar_servo.ino`](studio_sonar_servo.ino) code with my hand. As you can see it works quite well, but there are a few stutters which I addressed with a distance array in [`sonar_servo.ino`](sonar_servo.ino).

https://user-images.githubusercontent.com/52972357/169155096-76cfd707-e288-4bb8-bfdb-0ae722be0e96.mp4

This video demonstrates Huggey Jr. in action. It faced a few distance detection issues part-way through which is why we chose to extend the sonar arm for Huggey Sr.

https://user-images.githubusercontent.com/52972357/169155158-440fe4ac-b5d0-45da-b8c0-2793f19534d4.mp4

Our final result, a demonstration of Huggey Sr. picking up objects flawlessly.

https://user-images.githubusercontent.com/52972357/169155189-9e62381a-04bd-4658-af0e-c98851ac07ae.mp4
