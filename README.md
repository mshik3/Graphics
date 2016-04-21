# Graphics Projects
These are some of my projects from UIUC CS 418 - Interactive Computer Graphics

## Dancing I
This is the first project that we took on in the class, simple vertice and edge mapping to the viewport. The task was to create an "I" using only the "GL_TRIANGLE_STRIP" and "GL_TRIANGLE_FAN" helpers, color it a certain way, and make it dance around in a nice fashion.

### To run
   The Platform I used: OSX 10.11.2  
   The Build Environment: Xcode 7.2 in C++  
   Any External Libraries used: GLUT  

### Keystrokes
   keypress “r” to activate the rotate function  
   keypress “d” to make the “I” dance  
   keypress “h” to show the wireframe  
   keypress “esc” to quit the program  

## FlightSim

This is the most basic flight simulator that we created. We were given a Mountain range background to start our simulation on and were tasked to move the camera in such a way that you felt you were flying a plane. With the use of quaternions, I let the user guide their aircraft using simple keystrokes to roll, pitch, and yaw to avoid mountains and fly around. 

### To run
   The Platform I used: OSX 10.11.2  
   The Build Environment: Xcode 7.2 in C++  
   Any External Libraries used: GLUT  

Simply run the executable titled "FlightSimExec" and it will run through xcode. Happy flying!

### Keystrokes
   'w’ is pitch down  
		’s’ is pitch up  
		‘x’ is roll right  
		‘z’ is roll left  
		‘a’ is yaw left  
		‘d’ is yaw right  
		‘1’ is speed up  
		‘0’ is slow down  
		‘r’ is reset to start  
		
## Teapot

This is a project where we were given a teapot object file (a bunch of vertices and thats it) and we were told to display the 3D teapot, map an image (or texture) onto the teapot, and then make it reflective like a mirror so it would reflect an environment that we provide. 

### To run
The Platform I used: OSX 10.11.2  
The Build Environment: Xcode 7.2 in C++  
Any External Libraries used: GLUT  

### Keystrokes
  keypress “t” to switch between texture mapping and environment mapping.  
	keypress “l” to switch where the light source is  
	keypress “=“ or “-“ to speed up or slow down the rotation  
	keypress “z” to turn on the reflection for an environment  
	
I have created a video that walks through the keystrokes and displays the work I've done: https://www.youtube.com/watch?v=Luhzim-5FnI
