#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <common/model.hpp>

using namespace glm;

vec3 vertices[8] = {
	vec3(-0.5, -0.5, 0.5),
	vec3(-0.5, 0.5, 0.5),
	vec3(0.5, 0.5, 0.5),
	vec3(0.5, -0.5, 0.5),
	vec3(-0.5, -0.5, -0.5),
	vec3(-0.5, 0.5, -0.5),
	vec3(0.5, 0.5, -0.5),
	vec3(0.5, -0.5, -0.5)
};
vec3 vertices_triangle[] = {
//		vec3(0, 0, 0),
//		vec3(1, 0, 0),
//		vec3(0, 1, 0),
//
//		vec3(0, 0, 1),
//		vec3(1, 0, 1),
//		vec3(0, 1, 1)

		vec3(-0.5,0,0.5),
		vec3(0.5,0,0.5),
		vec3(0.5,0,-0.5),
		vec3(-0.5,0,-0.5),
		vec3(0,1,0)

};
vec3 vertices_line[] = {

};

// input: a_model (model's reference), a_a (positon of a), a_b (postion of b), a_c (position of c)
void ComputeNormal(Model &a_model, vec3 a_a, vec3 a_b, vec3 a_c)
{
	a_model.AddNormal(normalize(cross(a_b - a_a, a_c - a_a)));
	a_model.AddNormal(normalize(cross(a_b - a_a, a_c - a_a)));
	a_model.AddNormal(normalize(cross(a_b - a_a, a_c - a_a)));
}

// input: a_model (model's reference), a_a (positon of a), a_b (postion of b), a_c (position of c), a_d (position of d), a_color (color of quad)
void Quad(Model &a_model, int a_a, int a_b, int a_c, int a_d, vec3 a_color)
{
	/* a -- d
	 * |    |
	 * b -- c
	 */

	a_model.AddPosition(vertices[a_a]);
	a_model.AddPosition(vertices[a_b]);
	a_model.AddPosition(vertices[a_c]);
	a_model.AddPosition(vertices[a_a]);
	a_model.AddPosition(vertices[a_c]);
	a_model.AddPosition(vertices[a_d]);

//	ComputeNormal(a_model, vertices[a_b], vertices[a_a], vertices[a_c]);
	ComputeNormal(a_model, vertices[a_a], vertices[a_b], vertices[a_c]);
//	ComputeNormal(a_model, vertices[a_c], vertices[a_b], vertices[a_a]);
	ComputeNormal(a_model, vertices[a_a], vertices[a_c], vertices[a_d]);

	a_model.AddColor(a_color);
	a_model.AddColor(a_color);
	a_model.AddColor(a_color);
	a_model.AddColor(a_color);
	a_model.AddColor(a_color);
	a_model.AddColor(a_color);
}
void Quad_v2(Model &a_model, int a_a, int a_b, int a_c,int a_d, vec3 a_color, int type)
{
//	  b -- a
//	  |
//	  c
	//삼각형
	if(type == 0){
		a_model.AddPosition(vertices_triangle[a_a]);
		a_model.AddPosition(vertices_triangle[a_b]);
		a_model.AddPosition(vertices_triangle[a_c]);
		ComputeNormal(a_model, vertices_triangle[a_a], vertices_triangle[a_b], vertices_triangle[a_c]);
		a_model.AddColor(a_color);
		a_model.AddColor(a_color);
		a_model.AddColor(a_color);

	}else{
		a_model.AddPosition(vertices_triangle[a_a]);
		a_model.AddPosition(vertices_triangle[a_b]);
		a_model.AddPosition(vertices_triangle[a_c]);
		a_model.AddPosition(vertices_triangle[a_a]);
		a_model.AddPosition(vertices_triangle[a_c]);
		a_model.AddPosition(vertices_triangle[a_d]);

		ComputeNormal(a_model, vertices_triangle[a_a], vertices_triangle[a_b], vertices_triangle[a_c]);
		ComputeNormal(a_model, vertices_triangle[a_a], vertices_triangle[a_c], vertices_triangle[a_d]);

		a_model.AddColor(a_color);
		a_model.AddColor(a_color);
		a_model.AddColor(a_color);
		a_model.AddColor(a_color);
		a_model.AddColor(a_color);
		a_model.AddColor(a_color);
	}
}

// input: a_model (model's reference), a_color (color of cube)
void InitDataCube(Model &a_model, vec3 a_color)
{
   /*   5 -- 6
    *  /|   /|
    * 1(4)-2 7
	* |    |/
	* 0 -- 3
	*/

	Quad(a_model, 1, 0, 3, 2, a_color);
	Quad(a_model, 2, 3, 7, 6, a_color);
	Quad(a_model, 3, 0, 4, 7, a_color);
	Quad(a_model, 6, 5, 1, 2, a_color);
	Quad(a_model, 4, 5, 6, 7, a_color);
	Quad(a_model, 5, 4, 0, 1, a_color);
}

// input: a_model (model's reference), a_colors (color of each face of cube)
void InitDataRubic(Model& a_model, vec3* a_colors)
{
	/*  5 -- 6
    *  /|   /|
    * 1(4)-2 7
	* |    |/
	* 0 -- 3
	*/

	Quad(a_model, 1, 0, 3, 2, a_colors[0]);
	Quad(a_model, 2, 3, 7, 6, a_colors[4]);
	Quad(a_model, 3, 0, 4, 7, a_colors[5]);
	Quad(a_model, 6, 5, 1, 2, a_colors[3]);
	Quad(a_model, 4, 5, 6, 7, a_colors[1]);
	Quad(a_model, 5, 4, 0, 1, a_colors[2]);
}

// input: a_model (model's reference), a_color (color of sphere)
void InitDataSphere(Model &a_model, vec3 a_color)
{
	// TODO: Fill the vertex position, normal, color for your sphere object
	// Reference: Four ways to create a mesh for a sphere
	// https://medium.com/game-dev-daily/four-ways-to-create-a-mesh-for-a-sphere-d7956b825db4
}
void InitDataTriangle(Model &a_model, vec3 a_color)
{

//	//triangle
//	Quad_v2(a_model,0,2,1,0,a_color,0);
//	Quad_v2(a_model,3,4,5,0,a_color,0);
//
//	//side rectangle
//	Quad_v2(a_model,3,4,1,0,a_color,1);
//	Quad_v2(a_model,2,0,3,5,a_color,1);
//	Quad_v2(a_model,4,1,2,5,a_color,1);

	Quad_v2(a_model,4,0,1,0,a_color,0);
	Quad_v2(a_model,4,1,2,0,a_color,0);
	Quad_v2(a_model,4,2,3,0,a_color,0);
	Quad_v2(a_model,4,3,0,0,a_color,0);

	Quad_v2(a_model,3,0,1,2,a_color,1);

}
#endif
