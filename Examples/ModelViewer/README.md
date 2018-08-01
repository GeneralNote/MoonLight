# ModelViewer
In this example we will render simple .obj file.
This program is not optimized at all and the helicopter
rotor rotation depends on FPS (which is bad).
You dont have to render the OBJModel this way. So, for
example, you can ignore the material and groups/objects
of the .obj model and just render the whole scene.

*NOTE*: this is just a example, there are various ways to render the OBJModels.

## Code
In this example we ignore the objects in our .obj file
because we know it only consists of groups. So we loop
through all the groups and then we get the materials
used in that group. We loop through all the materials
in that group and then render specific parts of the
model using that material.