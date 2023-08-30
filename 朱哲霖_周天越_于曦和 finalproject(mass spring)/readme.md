# This is a cse project based on mass spring to implement cloth  simulation,we use opengl which includes glfw and glew to implement this method
# You should follow this link to link the glfw and glew https://blog.csdn.net/weixin_38009144/article/details/104588510?utm_medium=distribute.pc_relevant.none-task-blog-baidujs-3 and make all the project as an visual studio project ,the include and lib is provided
## in windows.h/cpp,we implement a window to render the mass spring,the construction function will create a window with parameter width and height
##  in texture.h/cpp,we implement a function to bind the texture
## in rotator.h/cpp,we implement a poll and init function which interacts with the mouse and monitor
## in shaderprogram,we define the function to compile and link the shader,and integrate them as a program
## stb_image.cpp is a lib to load the image
## cloth.vs is the vertex shader which read the vertex attribute and output the position after MVP translation
## cloth.fs is the fragment shader which calculate the color of vertex according to the light and normal information
## environment.vs and environment.fs is the shader which render the environment
## the main part is cloth.cpp,in cloth.h ,the default constructor is to create a cloth with default parameter,and the Cloth(x,y,w,l) make a cloth with has x*y masses spring, and w*l size,render is a function to render the cloth(update ),updatesimulation is the function to update the vertices' attributes such as velocitys and positions,accelartions,it will calculate the total force first and can use different method to update the vertices 
## Initialize_Cloth is to initialize the indices to render and initial physical properties
## calculate_force will calculate the mass spring force which includes shear and structual spring and update the normal as the sum of cross product among the direction between surrounding springs 
## ForwardEulerIntergration is the method to update by forwardEuler method
## Runge_Kutta ,we use K3 runge_kutta method to update
## verletIntergration ,use verletIntergration to update the vertices posistions
## the main function initialize the window and create a rotator class ,create a texture for cloth then use a while loop to update the vertices and render the window,the poll function is to update the mouse and the updateCommonuniforms is to update the rotator


