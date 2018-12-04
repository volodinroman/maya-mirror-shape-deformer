[![Maya API](https://img.shields.io/badge/SDK-Maya%20API-blue.svg)]()

# mirrorShape

A Maya deformer that inverts a shape of a topologically symmetrical mesh preserving vertices indices. Practically it was created for getting a blendShapes terget objects mirrored correctly in one click.

The plugin includes:
- mirrorShape command that runs the symmetry map calculation and sets the result to the defomer node
- mirrorShape deformer node that uses the symmetry map for relocating a target mesh vertices

## How it works:

The only rule to get the plugin work correctly is to have a topologically symmetrical mesh located in the center of the scene.

To invert a mesh correctly the plugin needs a "Symmetry Map" data that is calculated using a base mesh vertices positions, that's why before running the command "mirrorShape()" we need to select a base mesh first and then to select a target mesh.

After calculating the "Symmetry Map" the command creates a deformer and transfers the symmetry map data to it.

The command has two attributes:

-"name" or "n" that is used to define the name of the deformer;

-"symmetryPlane" or "sym" that is used to define the plane of the symmetry (XZ, YZ, XY) that visually splits a base mesh into two topologically identical parts.

## Usage:
There are two versions of the plugin available for testing and usage. Download one of them and place it in MAYA_PLUG_IN_PATH folder.

- Make sure the plugin has been loaded via pluginManager

- Create a new scene or open an existing one with a mesh that has a symmetrical topology and located in zero coordinates of the world space.

- Create a copy of this mesh, move it so as you could see it and apply some deformations to the one of the symmetrical sides.

- First select a base mesh (in the center of the scene), then select the second (modified) mesh and run the next script

## MEL
```
mirrorShape -n "deformerName" -symmetryPlane "YZ";
```

## Python
```
maya.cmds.mirrorShape(n = "deformerName", symmetryPlane = "YZ")
```
As soon as you like the invertation of the mesh shape you can delete the constructionHistory from the deformed mesh and use it as a blendShape target.
