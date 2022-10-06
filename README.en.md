# PominGL

#### Description
Computer Graphics and GIS Visualization, NNU, 2021 Fall
Group Member：Zhang Jingchi, Peng Qi and Yao Zezhong

#### Software Architecture
A system aped OpenGL and realized its major functions including raster images, clipping, vector file importing, transformation matrices, surface shading, texture mapping, ray tracing.

My job is to complete the functions of drawing and coloring lines, polylines, circle, triangle, rectangle and polygons; clipping lines, polylines, rectangles and polygons; finishing MVP(model, view, projection) transformation matrices, Z-BUFFER blanking algorithm, shading model such as LAMBERT model, PHONG model, BLINN-PHONG model. Software utilized: C/C++.

The system includes servals modulars, namely Geometric Modeling, Geometric Transformation, Color Mode Setting, Illumination Model Setting, Texture Mapping and Vector-Raster file management.

Besides, user can also use mouse and keyboard to rotate, move, scale the graphics; change the texture, color, view and light of the 3D cube.

The system' s  **menu bar** :
| Name              | Function                                                                                                                |
|-------------------|-------------------------------------------------------------------------------------------------------------------------|
| 文件(Files)         | Open and rasterize the vector map file and basic   functions such as open, close, exit                           |
| 2D图形(2D graphics) | basic 2D geometry drawing                                                                                               |
| 三维(3D graphics)   | Implement perspective projection , hidden surface, texture mapping and illumination settings for 3D cubes               |
| 设置(Setting)       | Set the size of the background pixels, implement grid mode, pixel mode and geometric drawing mode                       |
| 工具(Tools)         | Set the color fill function, the clear canvas function and the function to reset to the drawing on the original canvas. |
| 画笔颜色(Brush color) | Red, green and blue colors can be selected to achieve the drawing function                                              |
### System Demo
 **User Interface：** 

![UI](UI.png)

 **2D geometry drawing** 
- line

![line](line.png)
- polyline:

![polyline](polyline.png)
- circle

![circle](circle.png)
- ellipse

![ellipse](ellipse.png)
- polygon

![polygon](polygon.png)
- triangle

![triangle](triangle.png)
- filled polygon:

![filled polygon](filledpolygon.png)
- filled triangle

![filled triangle](filledtriangle.png)

 **2D operation** 
- filling

![polygon filling](polygonfilling.png)

- clipping

Cohen Sutherland, MidPoint Algorithm
![clipping line](clippingline.png)


Sutherland Hodgeman
![Sutherland Hodgeman](clippingpolygon1.png)

Weiler Atherton
![Weiler Atherton](clippingpolygon2.png)

 **drawing mode** 
- pixel mode

![pixel mode](pixelmode.png)
- grid mode

![grid mode](gridmode.png)

 **vector-raster file** 

![vector-raster file](vector2raster.png)

 **3D rendering** 

- 3D Cube model

![3Dcube](3Dcube.png)

- hidden surface

![hidden surface](zbuffer.png)

- color

![colorcube](colorcube.png)

- illumination model

Lambert Lambert Model
![shading](shading.png)

Gouraud Model
![Gouraud Model](Gouraud.png)

- Texture Mapping

![texture1](texture1.png)
![texture2](texture2.png)




