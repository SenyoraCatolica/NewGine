# NewGine
This is an Engine made by Miquel Gironés for my Game Engines subject. 
This engine is supported by SDL, ImGui, MathGeolib, OpenGL, Devil, Glew, Assimp and PhysFS. 

Check out my Web: https://github.com/SenyoraCatolica/NewGine.

Download Last Version: https://github.com/SenyoraCatolica/NewGine/releases.

## Instructions

You are able to drag and drop any FBX model to our engine. If there is a texture separated, you are able to drop it the same way to be applied. 
**The navigation is simple:**
  - W A S D to move on X and Z axis. 
  - RIGHT BUTTON to rotate the camera. 
  - Mouse wheel to zoom.
  - Mouse wheel button to move on X and Z axis.

**There is a menu with some buttons with this functionality:**
 - **Menu:** Displays the Configuration Window and the Console buttons, to turn it on/off.
 - **Help:** Displays buttons that link the user to our github repository and README. And also turn on/off an example window of ImGUI.
 - **About:** Displays a small introduction and a button to this README. 
 
There is an **Attribute Editor** where all the information about the model and texture is displayed. 

There is a **Console** that displays all the internal information that is usefull for the user. 
 - It has a button CLEAR that removes all the drawn logs. 
 
There is a **Configuration Window** where the user will be able to see information about the different modules of our Engine. 

**Features**
 - Game Objects Structure (components & hierarchy).
 - Component options (Select between all meshes, materials, modify transforms, etc.).
 - Game Time Management (Play, Pause & Stop) with a window to select what state you want.
 - Main Camera (Frustum object).
 - You can select game objects that have a bounding box directly from screen (Mousepicking) or you can select them through hierarchy    window.
 - Quadtree to filter collisions with mousepicking.
 - Materials and Meshes components uses Resources.
 
 **Scene Creation**
 - Drag and drop any fbx for models or png / tga for textures file to the application window
 - You will see in the assets window the file names imported
 - To load a scene, go to "file" on the top menu and select load scene. Write the name of the scene without the extention.
 - To load a prefab in the current scene, go to "file" on the top menu and select load prefab. Write the name of the scene without the extention.
 - To save a scene, go to "file" on the top menu and select save scene, the write the name you like.
 - To create an empty scene, go to "file" on the top menu and select create empty scene.
 
 **Innovations**
 
 File System management done with PhysFS library.
