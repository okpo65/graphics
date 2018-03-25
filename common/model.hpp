#ifndef MODEL_HPP
#define MODEL_HPP

#include <GL/glew.h>
#include <vector>
#include <glm/glm.hpp>
#include <glfw3.h>

using namespace std;
using namespace glm;

enum DRAW_TYPE {
	ARRAY,
	INDEX
};


class Model {
	vector<vec3> m_positions;
	vector<unsigned int> m_indices;
	vector<vec3> m_normals;
	vector<vec3> m_colors;

	mat4* m_projection;
	mat4* m_eye_rbt;
	mat4* m_model_rbt;

	mat4 m_model_matrix;
	mat4 m_parent_model_matrix;

	DRAW_TYPE m_draw_type;

	GLuint m_vertex_array_id;
	GLuint m_position_buffer_id;
	GLuint m_normal_buffer_id;
	GLuint m_color_buffer_id;
	GLuint m_index_buffer_id;
	
	GLint m_light_loc;
	GLint m_model_id;
public:
	GLuint m_glsl_program_id;
	vec3 gOrientation1;
	double lastTime = glfwGetTime();
	double lastFrameTime = lastTime;
	mat4 g_world_rbt = mat4(1.0f);
	Model();
	// Add vertex position in (x,y,z) coordinate in model space
	void AddPosition(float a_px, float a_py, float a_pz);
	// Add vertex position in (x,y,z) glm vector in model space
	void AddPosition(vec3 a_position);
	// Add vertex normal in (x,y,z) coordinate in model space
	void AddNormal(float a_nx, float a_ny, float a_nz);
	// Add vertex normal in (x,y,z) glm vector in model space
	void AddNormal(vec3 a_normal);
	// Add vertex color in (r,g,b) components
	void AddColor(float a_r, float a_g, float a_b);
	// Add vertex color in (r,g,b) glm vector
	void AddColor(vec3 a_color);
	// Add index for triangle
	void AddIndex(unsigned int a_index);
	// Set pointer of projection matrix for this model
	void SetProjection(mat4* p_projection);
	// Set pointer of camera rigid-body matrix for this model
	void SetEyeRbt(mat4* p_eye_rbt);
	// Set pointer of model matrix for this model
	void SetModelRbt(mat4* p_model_rbt);
	// Set directional light vector for model
	void SetDirectionalLight(vec3 a_dir_light);
	// Set a shader program for this model and transfer data which stored in m_vertices, m_normals, m_colors, (m_indices are optional)
	void InitializeGLSL(DRAW_TYPE a_draw_type, const char * a_vertex_shader_path, const char * a_fragment_shader_path);
	// Draw function
	void SetModelId(unsigned int p_model_id);
	void Draw(float model_speed_keyboard);

	mat4 GetModelMatrix();
	void SetParentModelMatrix(mat4 model_matrix);
	// Clean up all resources
	void CleanUp(void);
};


class Sphere
{
	mat4* s_projection;
	mat4* s_eye_rbt;
	mat4* s_model_rbt;
	GLint s_light_loc;

	mat4 s_model_matrix;
	mat4 s_parent_model_matrix;

	GLint s_model_id;
	const static int na=36;        // vertex grid size
	const static int nb=18;
	const static int na3=na*3;     // line in grid size
	const static int nn=nb*na3;    // whole grid size
	GLfloat sphere_pos[nn]; // vertex
	GLfloat sphere_nor[nn]; // normal
	GLfloat sphere_col[nn];   // color
	GLuint  sphere_ix [na*(nb-1)*6];    // indices
	GLuint sphere_vbo[4]={-1,-1,-1,-1};
	GLuint sphere_vao[4]={-1,-1,-1,-1};


public:

	Sphere();
	~Sphere();


	GLuint m_sphereProgramID;
	vec3 gOrientation1;
	double lastTime = glfwGetTime();
	double lastFrameTime = lastTime;
	void init(GLuint vertexPositionID);
	void cleanup();
	void Draw(float model_speed_keyboard,float model_transfer_keyboard[2], float model_transfer_direction);
	void SetProjection(mat4* p_projection);
	// Set pointer of camera rigid-body matrix for this model
	void SetEyeRbt(mat4* p_eye_rbt);
	// Set pointer of model matrix for this model
	void SetModelRbt(mat4* p_model_rbt);
	void SetDirectionalLight(vec3 a_dir_light);
	void SetModelId(unsigned int model_id);
	mat4 GetModelMatrix();
	void SetParentModelMatrix(mat4 model_matrix);
private:
	int lats, longs;
	bool isInited;
	GLuint m_vao, m_vboVertex, m_vboIndex,m_vboColor;
	int numsToDraw;
};

#endif
