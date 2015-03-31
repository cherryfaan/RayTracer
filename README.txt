File Descriptions:

raytracer.{cpp, h} 
The main body of the raytracer, including the scene graph.  Simple 
addition and traversal code to the graph are provided to you.  

util.{cpp, h}
Includes definitions and implementations for points, vectors, matrices, 
and some structures for intersection and shading.  

light_source.{cpp, h}
Defines the basic light class.  You could define different types of 
lights, which shades the ray differently.  Point lights are sufficient 
for most scenes.  

scene_object.{cpp, h}
Defines object primitives in the scene (spheres, cylinders, etc...).  
Implements the intersect function which finds the intersection point 
between the ray and the primitive. 

bmp_io.{cpp, h}
I/O functions for .bmp files.  You shouldn't have to modify them.   
