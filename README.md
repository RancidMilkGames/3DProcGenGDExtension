# 3DProcGenGDExtension
A GDExtension for generating 3D procedural worlds

The extension works by making landscape “chunks”. The main node you'll be working with is named Proc. The options for Proc are:

* The row and column settings allow you to choose how many chunks are made.
* Indices distance is the distance between indices in the created mesh.
* Max height is the maximum height of the landscape
* Height exponent sets max height to the power of this value
* Detail is how the noise is scaled to the landscape
* There's an option to generate navigation on the procedural mesh as well
* Terrain material is the material to apply to the landscape
* Noise Texture allows you to set the noise that will be used for generation

![landscape6](https://user-images.githubusercontent.com/115530728/226147429-562b472c-f656-4b66-955f-096977f00f98.png)
