# SimpleExample
This example render simple triangle on the screen.
You can achieve this by:
	- creating ml::Window with ml::Window::Create function
	- create input layout class that you can use to tell HLSL vertex shader the layout of input elements
	- load pixel shader ( ml::PixelShader )
	- load vertex shader + bind the input layout class to it so that it also creates the input layout (this is not necessary if you already have input layout created with some other shader)
	- create vertex buffer
	- create constant buffer that we will use to send data to shaders
	- clear the screen and depth stencil buffers
	- set topology, bind vertex buffer, bind const buffers, bind pixel shader
	- call the draw member of Window class
	- render everything