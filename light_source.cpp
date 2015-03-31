/***********************************************************
     Starter code for Assignment 3

     This code was originally written by Jack Wang for
		    CSC418, SPRING 2005

		implements light_source.h

***********************************************************/

#include <cmath>
#include "light_source.h"


void PointLight::shade( Ray3D& ray ) {
// TODO: implement this function to fill in values for ray.col
// using phong shading. Make sure your vectors are normalized, and
// clamp colour values to 1.0.
//
// It is assumed at this point that the intersection information in ray
// is available. So be sure that traverseScene() is called on the ray
// before this function.

	Vector3D N = ray.intersection.normal;
	Vector3D V = -1* ray.dir;
	Vector3D Di = _pos-ray.intersection.point;
	Vector3D De = 2.*(Di.dot(N)*N)-Di;
	
	N.normalize();
	V.normalize();
	Di.normalize();
	De.normalize();
	
	//ambient illunination
	Colour Ia = (*ray.intersection.mat).ambient*_col_ambient;
	//diffuse illumination
	Colour Id = (*ray.intersection.mat).diffuse*(std::max(0.0,N.dot(Di))*_col_diffuse);
	//specular illumination
	Colour Is = (*ray.intersection.mat).specular*(std::max(0.0,pow(V.dot(De),(*ray.intersection.mat).specular_exp))*_col_specular);
	
	//Phong shading
	//ray.col = Ia + Id + Is;
	//Only diffuse and ambient
	//ray.col = Ia + Id;
	//Scene signature
	ray.col = (*ray.intersection.mat).diffuse;
	ray.col.clamp();
}

