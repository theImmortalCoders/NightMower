# NightMower - 3D Game Project
## Overview
NightMower is a 3D game developed in C++ using OpenGL. The project was created as part of the Computer Graphics course at Rzeszow University of Technology.

## Developers:

Marcin Bator,
Bogdan Bosak

## Used libraries:
- tinyobjloader
- stb_image

## 1. Project Goals
The goal of the project was to create a 3D game on the Windows platform using C++ and OpenGL. The game should implement basic physical phenomena such as vehicle control, collisions with objects, and the motion of a moving object. Other requirements included object texturing, camera movement, and adding a simple storyline. All project goals outlined in the lab instructions were successfully implemented and extended with additional features.

## 2. Project Description
NightMower is a 3D computer game that allows the user to control a lawnmower. The objective is to collect all potatoes randomly placed on the grass. After collecting all potatoes, the level increases, and new potatoes are randomly generated. Collecting a potato earns the player 30 points. Trees and a wall in the middle of the field act as obstacles, and colliding with them deducts 50 points. The best score is saved to a .txt file and displayed on the screen.


The player can control the lawnmower using the keyboard. They can also adjust its speed, control the light height, pause or restart the game, and return to the initial position. Camera control is possible in two planes: distance from the lawnmower (within certain limits) and azimuth.

## 3. Game Rules
The lawnmower is always created at the point (0,0) on the horizontal coordinate axis.
The map boundaries and the wall are fixed in the same position.
Tree positions are randomly generated at program startup.
Potato positions are randomly generated at program startup and after advancing to the next level.
The initial number of potatoes is 5 and increases with each level.
Collecting a potato earns the player 30 points, while colliding with an object deducts 50 points.
The best score is saved to a .txt file.
## 4. Game Controls
W, A, S, D: Control lawnmower movement (forward, left, backward, right).
Shift: Increase lawnmower speed.
Mouse Right Button + Movement: Control camera azimuth.
Mouse Scroll: Adjust camera distance from the lawnmower.
Up/Down Arrow Keys: Change light height.
ESC: Pause the game.
R: Restart the game.
Space: Return lawnmower to the initial position.

## 5. Development Stages
### 5.1. Creating the Mower Object
The first stage involved transforming the existing C program into an object-oriented C++ project. The lawnmower object was created using a combination of manually coded elements in OpenGL and imported elements from Blender in .obj format.

### 5.2. Environment Creation and .obj File Loading
This stage focused on designing the environment in Blender, exporting it to the .obj format, and loading it into the game using the TinyObjLoader library. Objects such as trees, the wall, and potatoes were created and textured.

### 5.3. Texturing Objects and Camera Control
Texturing was implemented using .jpg files, loaded into the game with the STB_IMAGE library. Camera control was also introduced, allowing the user to orbit the lawnmower at a fixed height using the mouse.

### 5.4. Vehicle Movement
A vehicle movement model was developed, including gradual acceleration, deceleration, turning, and animation of rotating wheels and steering. User controls were provided for moving forward, backward, turning left, and turning right, with the option to increase speed using the Shift key.

### 5.5. Collision Detection
Collision detection was implemented using a circular model around the lawnmower and different collision zones for objects. The game checks for collisions in each frame and updates collision areas accordingly.

### 5.5.1. Terrain Class
A class was created to represent the terrain and handle collision-related functions. The checkCollision function determines the collision zone of the lawnmower and checks distances between the lawnmower and collision points. If the distance is less than the sum of radii, a collision is detected.

### 5.5.2. Lazik Class
The updateCollision function in the Lazik class determines the collision zone, updates the collision area, and checks collisions for objects in that area.

### 5.6. Conclusion
The collision implementation proved to be one of the more complex stages, but it significantly enhanced gameplay realism. The use of a circular collision model allowed for accurate representation of real-world collisions. Dividing the map into collision areas improved program efficiency, especially for large maps with numerous collision elements. Ultimately, the collision implementation positively impacted gameplay smoothness and made it more realistic.
