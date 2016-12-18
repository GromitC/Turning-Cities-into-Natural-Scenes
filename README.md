# Graduate Computer Graphics CSCI-GA 2270 Fall 2016
## Back to the nature: Turning Cities into Natural Scenes

![N|Solid](https://vgc.poly.edu/~yychan/images/cg-final.jpg)

### Description
Buildings in Manhattan are widespread in the city with different heights. Imagine if we twists such information as control points for curves, there will be many natural curves that often appears in the nature like mountains and sprays.  Therefore, with this information I  use the buildings to generate natural scenes that convey the synergy of human creations and the nature.

### Techniques

1. Generating Surfaces with Height Map: Due to the technique challenges of converting geoprgahical information from geojson to opengl coordinates, I used an inforgraphics from http://visual.ly/manhattan-building-heights and add some Gaussian blur to the map to create a smooth terrain. Since blender generates a better terrain from the height map I also import blender's output as a triangular mesh.

2. Multitexture Mapping: Modern OpenGL implementations support the capability to apply two or more textures to geometry simultaneously by allowing you to bind separate texture objects to each of some number of available texture units.
