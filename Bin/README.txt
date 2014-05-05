
======================================
	  Christopher Howlett
	Cloth Simulation README
======================================

=========      Controls      =========
W A S D: 		Move camera
Left Mouse Button: 	Cloth Interaction
Right Mouse Button: 	Yaw Camera
Left Mouse Button: 	Cloth Interaction
CTRL & Space: 		Elevate Camera
Shift: 			Boost camera speed
Arrow Keys: 		Move Sphere

TILDE Key:	Draw Controls to screen

1: 		Drag Cloth
2: 		Tear Cloth
3: 		Force at mouse
R: 		Reset Cloth to starting state
E: 		Extend rods in mouse direction
3: 		Force at mouse
NUMPAD + - : 	Increase / Decrease Wind



Notes_______________________________________

Verlet integration and all collision detection is found in CWireCloth class
Constraint solving is processed in CSpringLink class
Gravity and Wind forces are defined in COpenGLRenderer class

Running this software in Debug results in a significant framerate loss.

I apologise for the really bright text colour, could not figure out how to change it and ran out of time.