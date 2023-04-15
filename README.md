# 3DProcGenGDExtension
A GDExtension for generating 3D procedural worlds. For more info on GDExtensions, visit https://docs.godotengine.org/en/stable/tutorials/scripting/gdextension/index.html

## To Build
Use `scons` + `your build options` to build the extension.

## Example:
* Run `scons platform=linux` to build the debug extension for linux.
* Run `scons platform=windows` to build the debug extension for windows.
* Run `scons target=template_release` to build the release extension for your current platform.

## To Use
Copy the `project/addons/proc` folder to the `res://addons` folder of any project you want to add it to. Remember to run `scons` for any code changes you made to take effect. You can make a godot project in the "project" folder of this repo for quick testing.

### Example Code:
```
extends Node3D

# You can replace this cam with your player if you want
@export var cam: Camera3D
@export var proc: Proc

func _ready():
    # Generate the landscape
    proc.generate()
    # Wait for the landscape to be generated
    await proc.generated
    # Set the camera to a random point on the landscape
    cam.global_position = proc.get_random_point() + Vector3(0, 20, 0)
```

## Notes
The extension works by making landscape “chunks”. The main node you'll be working with is named Proc. The options for Proc are:

* The row and column settings allow you to choose how many chunks are made.
* Indices distance is the distance between indices in the created mesh.
* Max height is the maximum height of the landscape
* Height exponent sets max height to the power of this value
* Detail is how the noise is scaled to the landscape
* There's an option to generate navigation on the procedural mesh as well
* Terrain material is the material to apply to the landscape
* Noise Texture allows you to set the noise that will be used for generation

The icons for the nodes only work in 4.x versions of Godot. Currently, 4.0 versions don't support it.

![landscape6](https://user-images.githubusercontent.com/115530728/226147429-562b472c-f656-4b66-955f-096977f00f98.png)

