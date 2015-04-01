#include <cmath>
#include <iostream>
#include "scene_object.h"
#include <math.h>

bool UnitSquare::intersect( Ray3D& ray, const Matrix4x4& worldToModel,
                const Matrix4x4& modelToWorld ) {
        // TODO: implement intersection code for UnitSquare, which is
        // defined on the xy-plane, with vertices (0.5, 0.5, 0), 
        // (-0.5, 0.5, 0), (-0.5, -0.5, 0), (0.5, -0.5, 0), and normal
        // (0, 0, 1).
        //
        // Your goal here is to fill ray.intersection with correct values
        // should an intersection occur.  This includes intersection.point, 
        // intersection.normal, intersection.none, intersection.t_value.   
        //
        // HINT: Remember to first transform the ray into object space  
        // to simplify the intersection test.
        
        //Transform ray to object space
        Ray3D rayObjectSpace = Ray3D(worldToModel * ray.origin, worldToModel * ray.dir);        
        //point = ray.origin + t_value * ray.origin
        //point for unitsquare is (0, 0, 0)
        double t_value = -rayObjectSpace.origin[2] / rayObjectSpace.dir[2];
        if(t_value <= 0) {
        	return false;
        }
        
        
        Point3D intersectionpoint = rayObjectSpace.origin + t_value * rayObjectSpace.dir;
        bool intersection = false;
        
        if(intersectionpoint[0] > 0.5 || intersectionpoint[0] < -0.5 || intersectionpoint[1] > 0.5 || intersectionpoint[1] < -0.5) {
        	return false;
        }
        
        if(ray.intersection.none || t_value < ray.intersection.t_value){
                intersection = true;
                ray.intersection.point = modelToWorld * intersectionpoint;
                ray.intersection.normal = worldToModel.transpose()*Vector3D(0, 0, 1);
                ray.intersection.normal.normalize();
                ray.intersection.none = false;
                ray.intersection.t_value = t_value;
                return true;
        }
        
        return false;
}




bool UnitSphere::intersect( Ray3D& ray, const Matrix4x4& worldToModel,
                const Matrix4x4& modelToWorld ) {
        // TODO: implement intersection code for UnitSphere, which is centred 
        // on the origin.  
        //
        // Your goal here is to fill ray.intersection with correct values
        // should an intersection occur.  This includes intersection.point, 
        // intersection.normal, intersection.none, intersection.t_value.   
        //
        // HINT: Remember to first transform the ray into object space  
        // to simplify the intersection test.
        
        bool intersection = false;
        Ray3D rayObjectSpace = Ray3D(worldToModel * ray.origin, worldToModel * ray.dir);
        //unitSphere is radius of 1 and center at (0, 0, 0)
        double cx = rayObjectSpace.origin[0];
        double cy = rayObjectSpace.origin[1];
        double cz = rayObjectSpace.origin[2];
        double dx = rayObjectSpace.dir[0];
        double dy = rayObjectSpace.dir[1];
        double dz = rayObjectSpace.dir[2];
        
        double a = dx * dx + dy * dy + dz * dz;
        double b = 2 * dx * cx + 2 * dy * cy + 2 * dz * cz;
        double c = cx * cx + cy * cy + cz * cz - 1;
        
        double t_value;
        double discriminant = b * b - 4 * a * c;
        if(discriminant < 0){
                return intersection;
        }
        else{
                double root1 = (-1*b + sqrt(discriminant)) / (2 * a);
                double root2 = (-1*b - sqrt(discriminant)) / (2 * a);
                
                if (root1 > 0 && root2 > 0){
                		if(root1 < root2) t_value = root1;
                    	else t_value = root2;
                        intersection = true;
                }else if (root2 > 0 && root1 <= 0) {
                        t_value = root2;
                        intersection = true;
                }else if (root1 > 0 && root2 <= 0){
                        t_value = root1;
                        intersection = true;
                }else {
                		return intersection;
                }
        }

        Point3D intersectionPoint = rayObjectSpace.origin + t_value * rayObjectSpace.dir;
        
        if(ray.intersection.none || t_value < ray.intersection.t_value){
                intersection = true;
                ray.intersection.point = modelToWorld * intersectionPoint;
                ray.intersection.normal = worldToModel.transpose()*Vector3D(intersectionPoint[0], intersectionPoint[1], intersectionPoint[2]);
                ray.intersection.normal.normalize();
                ray.intersection.none = false;
                ray.intersection.t_value = t_value;
                
        }       
        return intersection;
}


bool Cylinder::intersect( Ray3D& ray, const Matrix4x4& worldToModel,
                const Matrix4x4& modelToWorld){
        //UnitCylinder with unitCircle on top and bottom in xy-plane and z = 1 and z = -1 along z axis
        //quadratic function for unit cylinder is x^2 + y^2 = 1, centered at origin
        bool intersection = false;
        bool intersect_top = false;
        bool intersect_bot = false;

        Ray3D rayObjectSpace = Ray3D(worldToModel * ray.origin, worldToModel * ray.dir);

        double cx = rayObjectSpace.origin[0];
        double cy = rayObjectSpace.origin[1];
        double cz = rayObjectSpace.origin[2];
        double dx = rayObjectSpace.dir[0];
        double dy = rayObjectSpace.dir[1];
        double dz = rayObjectSpace.dir[2];
        double t_value;

        //ignore z first to find intersection on quadratic wall
        double a = dx * dx + dy * dy;
        double b = 2 * dx * cx + 2 * dy * cy;
        double c = cx * cx + cy * cy - 1;
        double discriminant = b * b - 4 * a * c;
        //find possible t_value on quadratic wall
        if(discriminant >= 0){
                double root1 = (-b + sqrt(discriminant)) / (2 * a);
                double root2 = (-b - sqrt(discriminant)) / (2 * a);
                if (root1 > 0 && root2 > 0 && root1 < root2){
                        double t_value1 = root1;
                        intersection = true;
                }if (root2 > 0 && root1 <= 0) {
                        double t_value2 = root2;
                        intersection = true;
                }if (root1 > 0 && root2 <= 0){
                        double t_value3 = root1;
                        intersection = true;

                }
        }

        //See if there is intersection on top and bottom of cylinder
        double t_valueTop = (1 - cz)/dz;
        double t_valueBot = (-1 - cz)/dz;
        Point3D intersectTopPoint = rayObjectSpace.origin + t_valueTop * rayObjectSpace.dir;
        Point3D intersectBotPoint = rayObjectSpace.origin + t_valueBot * rayObjectSpace.dir;

        //Find the smallest positive intersection inside the region from  multiple intersections
        //which is smallest t_value
        Point3D intersectionPoint1 = rayObjectSpace.origin + t_value1 * rayObjectSpace.dir;
        Point3D intersectionPoint2 = rayObjectSpace.origin + t_value2 * rayObjectSpace.dir;
        Point3D intersectionPoint3 = rayObjectSpace.origin + t_value3 * rayObjectSpace.dir;
        std::vector<Point3D> intersectPts;
        std::vector<Point3D> good_intersectPts;
        intersectPts.push_back(intersectTopPoint);
        intersectPts.push_back(intersectBotPoint);
        intersectPts.push_back(intersectionPoint1);
        intersectPts.push_back(intersectionPoint2);
        intersectPts.push_back(intersectionPoint3);
        for(unsigned int i = 0; i<intersectPts.size(); i++){
                if(intersectPts[i][2] >= -1 && intersectPts[i][2] <= 1 ){
                        good_intersectPts.push_back(intersectPts[i]);
                }
        }
        Point3D intersectionPoint = *std::min_element(good_intersectPts.begin(), good_intersectPts.end());
        t_value = intersectionPoint - rayObjectSpace.origin / rayObjectSpace.dir;
        if(ray.intersection.none || t_value < ray.intersection.t_value){
                intersection = true;
                ray.intersection.point = modelToWorld * intersectionPoint;
                ray.intersection.normal = worldToModel.transpose()*Vector3D(intersectionPoint[0], intersectionPoint[1], intersectionPoint[2]);
                ray.intersection.normal.normalize();
                ray.intersection.none = false;
                ray.intersection.t_value = t_value;
        }
        return intersection;
}
