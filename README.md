[![Maya API](https://img.shields.io/badge/SDK-Maya%20API-blue.svg)](#)

# mirrorShape

**mirrorShape** is a Maya deformer that inverts the shape of a topologically symmetrical mesh while preserving vertex indices. It was designed to generate correctly mirrored blendShape targets in a single click.

## Features

- **mirrorShape command** – Computes the symmetry map and assigns it to the deformer node.
- **mirrorShape deformer node** – Uses the symmetry map to reposition target mesh vertices.

## How It Works

To ensure correct results, the base mesh must be **topologically symmetrical** and positioned at the **center of the scene**.

1. The plugin calculates a **Symmetry Map** based on the base mesh’s vertex positions.
2. You first **select the base mesh**, then **select the target mesh**.
3. Running `mirrorShape()` generates a deformer and transfers the symmetry data.

### Command Attributes

- **`name` / `n`** – Specifies the deformer’s name.
- **`symmetryPlane` / `sym`** – Defines the symmetry plane (`XZ`, `YZ`, or `XY`), which divides the base mesh into two symmetrical halves.

## Installation & Usage

1. Download and place the plugin in the `MAYA_PLUG_IN_PATH` folder.
2. Load the plugin via **Plugin Manager**.
3. Open a scene with a symmetrical mesh positioned at world origin.
4. Duplicate the mesh, apply deformations to one side, then run the command:

### Using the deformer in Maya

```
// MEL
mirrorShape -n "deformerName" -symmetryPlane "YZ";

# Python
maya.cmds.mirrorShape(n="deformerName", symmetryPlane="YZ")
```

Once satisfied with the shape inversion, delete construction history and use the result as a blendShape target.
