# mirrorShape

A Maya deformer that inverts the shape of a topologically symmetrical mesh preserving vertices indices of a deformed mesh. Practically it was created for getting a blendShapes objects mirrored correctly in one click.

The plugin includes:
- mirrorShape command that runs the symmetry map calculation and sets the result to the defomer node
- mirrorShape deformer node that uses the symmetry map for relocating a target mesh vertices

####How it works:


####Usage:
There are two versions of the plugin available for testing and usage. Download one of them and place it in MAYA_PLUG_IN_PATH folder.

- Make sure the plugin has been loaded via pluginManager

- Create a new scene or open an existing one with a mesh that has a symmetrical topology and located in zero coordinates of the world space.

- Create a copy of this mesh, move it so as you could see it and add some deformations for the one of the symmetrical sides.

- First select a base mesh (in the center of the scene), then select the second (modified) mesh and run the next script

####MEL
```
mirrorShape -n "deformerName" -symmetryPlane "YZ";
```

####Python
```
maya.cmds.mirrorShape(n = "deformerName", symmetryPlane = "YZ")
```
As soon as you like the invertation of the mesh shape you can delete the constructionHistory from the deformed mesh and use it as a blendShape.
