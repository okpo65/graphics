#include <iostream>
#include <vector>

#include "model.hpp"
#include "shader.hpp"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace std;
using namespace glm;



Model::Model()
{
	// Initialize model information
	m_positions = vector<vec3>();
	m_indices = vector<unsigned int>();
	m_normals = vector<vec3>();
	m_colors = vector<vec3>();
	m_model_matrix = glm::mat4();
	m_parent_model_matrix = glm::mat4();
}
Sphere::Sphere()
{
	isInited = false;
	m_vao = 0;
	m_vboVertex = 0;
	m_vboIndex = 0;

	lats = 40;
	longs = 40;
}

void Model::AddPosition(float a_px, float a_py, float a_pz)
{
	m_positions.push_back(vec3(a_px, a_py, a_pz));
}

void Model::AddPosition(vec3 a_position)
{
	m_positions.push_back(a_position);
}

void Model::AddNormal(float a_nx, float a_ny, float a_nz)
{
	m_normals.push_back(vec3(a_nx, a_ny, a_nz));
}

void Model::AddNormal(vec3 a_normal)
{
	m_normals.push_back(a_normal);
}

void Model::AddColor(float a_r, float a_g, float a_b)
{
	m_colors.push_back(vec3(a_r, a_g, a_b));
}

void Model::AddColor(vec3 a_color)
{
	m_colors.push_back(a_color);
}

void Model::AddIndex(unsigned int a_index)
{
	m_indices.push_back(a_index);
}

void Model::SetProjection(mat4* p_projection)
{
	m_projection = p_projection;
}

void Model::SetEyeRbt(mat4* p_eye_rbt)
{
	m_eye_rbt = p_eye_rbt;
}

void Model::SetModelRbt(mat4* p_model_rbt)
{
	m_model_rbt = p_model_rbt;
}

void Model::SetDirectionalLight(vec3 a_dir_light)
{
	m_light_loc = glGetUniformLocation(m_glsl_program_id, "uLight");
	glUniform3f(m_light_loc, a_dir_light.x, a_dir_light.y, a_dir_light.z);
}
void Model::SetModelId(unsigned int p_model_id)
{
	m_model_id = p_model_id;
}

mat4 Model::GetModelMatrix()
{
	return m_model_matrix;
}
void Model::SetParentModelMatrix(mat4 model_matrix)
{
	m_parent_model_matrix = model_matrix;
}
void Model::InitializeGLSL(DRAW_TYPE a_draw_type, const char * a_vertex_shader_path, const char * a_fragment_shader_path)
{
	m_glsl_program_id = LoadShaders(a_vertex_shader_path, a_fragment_shader_path);
	m_draw_type = a_draw_type;

	glGenVertexArrays(1, &m_vertex_array_id);
	glBindVertexArray(m_vertex_array_id);

	glGenBuffers(1, &m_position_buffer_id);
	glBindBuffer(GL_ARRAY_BUFFER, m_position_buffer_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3)*m_positions.size(), &m_positions[0], GL_STATIC_DRAW);

	if (m_draw_type == DRAW_TYPE::INDEX)
	{
		glGenBuffers(1, &m_index_buffer_id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_buffer_id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*m_indices.size(), &m_indices[0], GL_STATIC_DRAW);
	}

	glGenBuffers(1, &m_normal_buffer_id);
	glBindBuffer(GL_ARRAY_BUFFER, m_normal_buffer_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3)*m_normals.size(), &m_normals[0], GL_STATIC_DRAW);

	glGenBuffers(1, &m_color_buffer_id);
	glBindBuffer(GL_ARRAY_BUFFER, m_color_buffer_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3)*m_colors.size(), &m_colors[0], GL_STATIC_DRAW);
}

void Model::Draw(float model_speed_keyboard)
{
	glUseProgram(m_glsl_program_id);
	GLint projection_id = glGetUniformLocation(m_glsl_program_id, "Projection");
	GLint eye_id = glGetUniformLocation(m_glsl_program_id, "Eye");
	GLint model_id = glGetUniformLocation(m_glsl_program_id, "ModelTransform");

	glUniformMatrix4fv(projection_id, 1, GL_FALSE, &(*(m_projection))[0][0]);
	glUniformMatrix4fv(eye_id, 1, GL_FALSE, &(*(m_eye_rbt))[0][0]);


	double currentTime = glfwGetTime();
	float deltaTime = (float)(currentTime - lastFrameTime);

	lastFrameTime = currentTime;


	gOrientation1.y += 3.14159f/2.0f * (deltaTime * model_speed_keyboard);
	// Build the model matrix
	glm::mat4 RotationMatrix = eulerAngleYXZ(gOrientation1.y, gOrientation1.x, gOrientation1.z);
	glm::mat4 TransformMatrix = m_parent_model_matrix;
	glm::mat4 ModelMatrix = *m_model_rbt;
	glm::mat4 ScaleMatrix = glm::scale(0.7f, 0.7f ,0.7f);
	switch (m_model_id){
		case 0:

			break;
		case 1:
			ModelMatrix =  TransformMatrix * RotationMatrix * translate(glm::mat4(), vec3(-3.0f, -2.0f, -0.15f)) * ScaleMatrix;
			m_model_matrix = TransformMatrix * translate(RotationMatrix, vec3(-3.0f, -2.0f, -0.15f));
			break;
		case 2:
			ModelMatrix =  TransformMatrix * RotationMatrix * translate(glm::mat4(), vec3(3.0f, -2.0f, 0.2f)) * ScaleMatrix;
			m_model_matrix = TransformMatrix * translate(RotationMatrix, vec3(3.0f, -2.0f, 0.2f));
			break;
		case 3:
			break;
		case 4:
			ModelMatrix =  TransformMatrix * RotationMatrix * translate(glm::mat4(), vec3(1.5f, -2.0f, 0.0f)) * ScaleMatrix;
			m_model_matrix = TransformMatrix * translate(RotationMatrix, vec3(1.5f, -2.0f, 0.0f));
			break;
		case 5:
			ModelMatrix =  TransformMatrix * RotationMatrix * translate(glm::mat4(), vec3(-1.5f, -2.0f, 0.0f)) * ScaleMatrix;
			m_model_matrix = TransformMatrix * translate(RotationMatrix, vec3(-1.5f, -2.0f, 0.0f));
			break;
		case 6:
			ModelMatrix =  TransformMatrix * RotationMatrix * translate(glm::mat4(), vec3(1.5f, -2.0f, 0.0f)) * ScaleMatrix;
			m_model_matrix = TransformMatrix * translate(RotationMatrix, vec3(1.5f, -2.0f, 0.0f));
			break;

		default:
            ScaleMatrix = glm::scale(0.05f, 0.8f ,0.05f);

            if (m_model_id <= 9){
                ModelMatrix = TransformMatrix * RotationMatrix  * translate(glm::mat4(), vec3(0.0f, -0.5f, 0.0f)) * ScaleMatrix;
                m_model_matrix = TransformMatrix;
            }else if (m_model_id <= 15){
                ModelMatrix = TransformMatrix * RotationMatrix  * translate(glm::mat4(), vec3(0.0f, +0.5f, 0.0f)) * ScaleMatrix;
                m_model_matrix = TransformMatrix;
            }else{
                if(m_model_id == 16) {
                    ScaleMatrix = glm::scale(6.3f, 0.2f ,0.2f);
                }else{
                    ScaleMatrix = glm::scale(3.3f, 0.2f ,0.2f);
                }
                ModelMatrix = TransformMatrix * RotationMatrix * translate(glm::mat4(), vec3(0.0f, -1.0f, 0.0f)) * ScaleMatrix;
            }
			break;
	}
	glUniformMatrix4fv(model_id, 1, GL_FALSE, &(ModelMatrix)[0][0]);

	glBindVertexArray(m_vertex_array_id);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, m_position_buffer_id);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), ((GLvoid*)(0)));

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, m_normal_buffer_id);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), ((GLvoid*)(0)));

	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, m_color_buffer_id);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), ((GLvoid*)(0)));

	if (m_draw_type == DRAW_TYPE::ARRAY)
	{
		glDrawArrays(GL_TRIANGLES, 0, (GLsizei) m_positions.size());
	}
	else {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_buffer_id);
		glDrawElements(GL_TRIANGLES, (GLsizei) m_indices.size(), GL_UNSIGNED_INT, ((GLvoid *)0));
	}
}

void Model::CleanUp()
{
	// Clean up data structures
	m_positions.clear();
	m_positions.shrink_to_fit();

	m_indices.clear();
	m_indices.shrink_to_fit();

	m_normals.clear();
	m_normals.shrink_to_fit();

	m_colors.clear();
	m_colors.shrink_to_fit();

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	// Cleanup VBO and shader
	glDeleteBuffers(1, &m_vertex_array_id);
	glDeleteBuffers(1, &m_normal_buffer_id);
	glDeleteBuffers(1, &m_color_buffer_id);
	if (m_draw_type == DRAW_TYPE::INDEX) glDeleteBuffers(1, &m_index_buffer_id);
	glDeleteProgram(m_glsl_program_id);
	glDeleteVertexArrays(1, &m_vertex_array_id);
}


Sphere::~Sphere()
{

}

void Sphere::SetProjection(mat4* p_projection)
{
	s_projection = p_projection;
}

void Sphere::SetEyeRbt(mat4* p_eye_rbt)
{
	s_eye_rbt = p_eye_rbt;
}

void Sphere::SetModelRbt(mat4* p_model_rbt)
{
	s_model_rbt = p_model_rbt;
}

void Sphere::SetDirectionalLight(vec3 a_dir_light)
{
	s_light_loc = glGetUniformLocation(m_sphereProgramID, "uLight");
	glUniform3f(s_light_loc, a_dir_light.x, a_dir_light.y, a_dir_light.z);
}
void Sphere::SetModelId(unsigned int model_id)
{
    s_model_id = model_id;
}

mat4 Sphere::GetModelMatrix()
{
	return s_model_matrix;
}
void Sphere::SetParentModelMatrix(mat4 model_matrix)
{
	s_parent_model_matrix = model_matrix;
}
void Sphere::init(GLuint vertexPositionID)
{

    GLfloat x,y,z,a,b,da,db,r=4;
    int ia,ib,ix,iy;
    da=2.0*M_PI/GLfloat(na);
    db=    M_PI/GLfloat(nb-1);
    // [Generate sphere point data]
    // spherical angles a,b covering whole sphere surface
    for (ix=0,b=-0.5*M_PI,ib=0;ib<nb;ib++,b+=db)
        for (a=0.0,ia=0;ia<na;ia++,a+=da,ix+=3)
        {
            // unit sphere
            x=cos(b)*cos(a);
            y=cos(b)*sin(a);
            z=sin(b);
            sphere_pos[ix+0]=x*r;
            sphere_pos[ix+1]=y*r;
            sphere_pos[ix+2]=z*r;
            sphere_nor[ix+0]=x;
            sphere_nor[ix+1]=y;
            sphere_nor[ix+2]=z;
			if (s_model_id == 0){
				sphere_col[ix+0]=0.5f;
				sphere_col[ix+1]=0.5f;
				sphere_col[ix+2]=1.0f;
			}else{
				sphere_col[ix+0]=0.5f;
				sphere_col[ix+1]=0.5f;
				sphere_col[ix+2]=0.0f;
			}


        }
    // [Generate GL_TRIANGLE indices]
    for (ix=0,iy=0,ib=1;ib<nb;ib++)
    {
        for (ia=1;ia<na;ia++,iy++)
        {
            // first half of QUAD
            sphere_ix[ix]=iy;      ix++;
            sphere_ix[ix]=iy+1;    ix++;
            sphere_ix[ix]=iy+na;   ix++;
            // second half of QUAD
            sphere_ix[ix]=iy+na;   ix++;
            sphere_ix[ix]=iy+1;    ix++;
            sphere_ix[ix]=iy+na+1; ix++;
        }
        // first half of QUAD
        sphere_ix[ix]=iy;       ix++;
        sphere_ix[ix]=iy+1-na;  ix++;
        sphere_ix[ix]=iy+na;    ix++;
        // second half of QUAD
        sphere_ix[ix]=iy+na;    ix++;
        sphere_ix[ix]=iy-na+1;  ix++;
        sphere_ix[ix]=iy+1;     ix++;
        iy++;
    }
    // [VAO/VBO stuff]
    GLuint i;
	m_sphereProgramID = LoadShaders("VertexShader.glsl", "FragmentShader.glsl");

    glGenVertexArrays(4,sphere_vao);
    glGenBuffers(4,sphere_vbo);
    glBindVertexArray(sphere_vao[0]);
    i=0; // vertex
    glBindBuffer(GL_ARRAY_BUFFER,sphere_vbo[i]);
    glBufferData(GL_ARRAY_BUFFER,sizeof(sphere_pos),sphere_pos,GL_STATIC_DRAW);
    glEnableVertexAttribArray(i);
    glVertexAttribPointer(i,3,GL_FLOAT,GL_FALSE,0,0);
    i=3; // indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,sphere_vbo[i]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(sphere_ix),sphere_ix,GL_STATIC_DRAW);
    glEnableVertexAttribArray(i);
    glVertexAttribPointer(i,4,GL_UNSIGNED_INT,GL_FALSE,0,0);
    i=1; // normal
    glBindBuffer(GL_ARRAY_BUFFER,sphere_vbo[i]);
    glBufferData(GL_ARRAY_BUFFER,sizeof(sphere_nor),sphere_nor,GL_STATIC_DRAW);
    glEnableVertexAttribArray(i);
    glVertexAttribPointer(i,3,GL_FLOAT,GL_FALSE,0,0);

    i=2; // color
    glBindBuffer(GL_ARRAY_BUFFER,sphere_vbo[i]);
    glBufferData(GL_ARRAY_BUFFER,sizeof(sphere_col),sphere_col,GL_STATIC_DRAW);
    glEnableVertexAttribArray(i);
    glVertexAttribPointer(i,3,GL_FLOAT,GL_FALSE,0,0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);


    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);
}

void Sphere::cleanup()
{
    glDeleteVertexArrays(4,sphere_vao);
    glDeleteBuffers(4,sphere_vbo);
}


void Sphere::Draw(float model_speed_keyboard, float model_transfer_keyboard[2], float model_transfer_direction)
{
	glUseProgram(m_sphereProgramID);
	GLint projection_id = glGetUniformLocation(m_sphereProgramID, "Projection");
	GLint eye_id = glGetUniformLocation(m_sphereProgramID, "Eye");
	GLint model_id = glGetUniformLocation(m_sphereProgramID, "ModelTransform");

	glUniformMatrix4fv(projection_id, 1, GL_FALSE, &(*(s_projection))[0][0]);
	glUniformMatrix4fv(eye_id, 1, GL_FALSE, &(*(s_eye_rbt))[0][0]);

	double currentTime = glfwGetTime();
	float deltaTime = (float)(currentTime - lastFrameTime);

	lastFrameTime = currentTime;

    gOrientation1.y += 3.14159f/2.0f * (deltaTime * model_speed_keyboard) ;


    // Build the model matrix

    glm::mat4 RotationMatrix = eulerAngleYXZ(gOrientation1.y, gOrientation1.x, gOrientation1.z);
	glm::mat4 TransformMatrix = s_parent_model_matrix;
	glm::mat4 ModelMatrix = *s_model_rbt;
	glm::mat4 scalingMatrix;
	switch (s_model_id)
	{
		case 0:
			scalingMatrix = glm::scale(0.1f, 0.1f ,0.1f);
			TransformMatrix = glm::translate(glm::mat4(), glm::vec3(model_transfer_keyboard[1], 8.0f + model_transfer_keyboard[0], 0.0f));
			ModelMatrix =  TransformMatrix * RotationMatrix * scalingMatrix;
            s_model_matrix = translate(glm::mat4(), glm::vec3(model_transfer_keyboard[1], 8.0f + model_transfer_keyboard[0], 0.0f));
			break;
		case 3:
			scalingMatrix = glm::scale(0.1f, 0.1f ,0.1f);
			ModelMatrix =  TransformMatrix * RotationMatrix * translate(glm::mat4(), vec3(-1.5f, -2.0f, 0.0f)) * scalingMatrix;
            s_model_matrix = TransformMatrix * translate(RotationMatrix, vec3(-1.5f,-2.0f,0.0f));

			break;
		default:
			break;
	}
	glUniformMatrix4fv(model_id, 1, GL_FALSE, &(ModelMatrix)[0][0]);

	glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
    glMatrixMode(GL_TEXTURE);
//    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_TEXTURE_2D);

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glBindVertexArray(sphere_vao[0]);
    glDrawArrays(GL_POINTS,0,sizeof(sphere_pos)/sizeof(GLfloat));                   // POINTS ... no indices for debug
    glDrawElements(GL_TRIANGLES,sizeof(sphere_ix)/sizeof(GLuint),GL_UNSIGNED_INT,0);    // indices (choose just one line not both !!!)
    glBindVertexArray(0);
}


