// ObjMesh.cpp: implementation of the ObjMesh class.
//
//////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////
////////// 20134007 김민식                 ////////////
////////// 기말 프로젝트 - obj 파일 로더  ////////////
////////// ObjMesh.cpp                          ////////////
///////////////////////////////////////////////////////

#include "ObjMesh.h"
#include <STDIO.H>
#include <stdlib.h>
#include <gl/glut.h>
#include <math.h>

#include "ImageLoader.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ObjMesh::ObjMesh()
{
	model_name = NULL;

	m_vertices = NULL;
	m_texCoords = NULL;
	m_normals = NULL;
	
	m_Faces = NULL;
	m_TextureFace = NULL;

	m_numTexCoords = m_numVertices = m_numFaces = m_numNormals = 0;

	m_Color[0] = 0.5;
	m_Color[1] = 0.9;
	m_Color[2] = 0.7;
}

ObjMesh::~ObjMesh()
{
// 	if(m_vertices != NULL)
// 		free(m_vertices);
// 	if(m_normals != NULL)
// 		free(m_normals);
// 	if(m_Faces != NULL)
// 		free(m_Faces);
// 	if(m_texCoords != NULL)
// 		free(m_texCoords);
// 	if(m_TextureFace != NULL)
// 		free(m_TextureFace);
// 	if(m_TextureID != -1)
// 		glDeleteTextures(1, &m_TextureID);
}

void ObjMesh::Render(int line, int faces, int texture)
{
	if(m_numTexCoords != 0) // 텍스쳐 정보가 있을 경우에만, 맵핑할 것
	{
		//////// abcd.obj를 -> abcd.bmp로 변환  ////////////
		//////// 확장자 obj를 bmp로 바꾸는 작업 //////////////////////
		int length = strlen(model_name);
		char* texture_name = NULL;
		texture_name = (char *)malloc(sizeof(char) * length);
	
		for(int i = 0; i < length - 4; i++) texture_name[i] = model_name[i];
		texture_name[length - 4] = '.';
		texture_name[length - 3] = 'b';
		texture_name[length - 2] = 'm';
		texture_name[length - 1] = 'p';
		texture_name[length] = '\0';
		////////////////////////////////////////////////////////////////

		if(texture == 1 && m_numTexCoords != 0)
		{
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, LoadGLTextures(texture_name));
		}
		else
		{
			glDisable(GL_TEXTURE_2D);
		}
	}

	glPushMatrix();	
	
	if(faces == 1) // 면을 그리지 않을 경우, 생략한다.
	{
			//면 그리기
		glColor3fv(m_Color);
		float j = 0;
		for (int i = 0; i < m_numFaces * 3; i+=3)	
		{ 	
			float ax, ay, az, bx, by, bz; // 노말 벡터

			float f1[2], f2[2], f3[3]; // 텍스쳐

			glBegin(GL_TRIANGLES);
 			float p1[3] = {m_vertices[m_Faces[i+0] * 3 + 0], m_vertices[m_Faces[i+0] * 3 + 1], m_vertices[m_Faces[i+0] * 3 + 2]};
			float p2[3] = {m_vertices[m_Faces[i+1] * 3 + 0], m_vertices[m_Faces[i+1] * 3 + 1], m_vertices[m_Faces[i+1] * 3 + 2]};
			float p3[3] = {m_vertices[m_Faces[i+2] * 3 + 0], m_vertices[m_Faces[i+2] * 3 +1 ], m_vertices[m_Faces[i+2] * 3 + 2]};
			

			if(m_numTexCoords > 0) // 텍스쳐 정보가 있는 모델링float f1[2]
			{
				f1[0] = m_texCoords[m_TextureFace[i+0] * 2 + 0];
				f1[1] = m_texCoords[m_TextureFace[i+0] * 2 + 1];
				f2[0] = m_texCoords[m_TextureFace[i+1] * 2 + 0];
				f2[1] = m_texCoords[m_TextureFace[i+1] * 2 + 1];
				f3[0] = m_texCoords[m_TextureFace[i+2] * 2 + 0];
				f3[1] = m_texCoords[m_TextureFace[i+2] * 2 + 1];
			}

			ax = p1[0] - p2[0];
			ay = p1[1] - p2[1];
			az = p1[2] - p2[2];

			bx = p3[0] - p2[0];
			by = p3[1] - p2[1];
			bz = p3[2] - p2[2];

			GLfloat nx = ay*bz - az*by; // 노말벡터 x
			GLfloat ny = az*bx - ax*bz; // 노말벡터 y
			GLfloat nz = ax*by - ay*bx; // 노말벡터 z

			float leng = sqrt(nx*nx + ny*ny + nz*nz);
			if(leng > 0.0001)
			{
				nx /= leng;
				ny /= leng;
				nz /= leng;
			}

			glNormal3f(-nx, -ny, -nz); // 노말 벡터 적용

			if(m_numTexCoords > 0) // 텍스쳐 정보가 있는 면
			{ 			
				glTexCoord2fv(f1);
				glVertex3fv(p1);
				glTexCoord2fv(f2);
				glVertex3fv(p2);
				glTexCoord2fv(f3);
				glVertex3fv(p3);
			}
			else // 텍스쳐가 없는 면
			{
				glVertex3fv(p1);
				glVertex3fv(p2);
				glVertex3fv(p3);
			}
			
		
			glEnd();
		}	
	}

	if(line == 1) // 외곽선을 그리지 않을 경우, 생략한다.
	{
			//외곽선 그리기
		glScalef(1.0001, 1.0001, 1.0001);
		glColor3d(0, 0, 0);
		for (int i = 0; i < m_numFaces * 3; i+=3)	
		{	
			glBegin(GL_LINES);
			glVertex3f(m_vertices[m_Faces[i+0] * 3 + 0], m_vertices[m_Faces[i+0] * 3 +1], m_vertices[m_Faces[i+0] * 3 + 2]);
			glVertex3f(m_vertices[m_Faces[i+1] * 3 + 0], m_vertices[m_Faces[i+1] * 3 +1], m_vertices[m_Faces[i+1] * 3 + 2]);
		
			glVertex3f(m_vertices[m_Faces[i+1] * 3 + 0], m_vertices[m_Faces[i+1] * 3 +1], m_vertices[m_Faces[i+1] * 3 + 2]);
			glVertex3f(m_vertices[m_Faces[i+2] * 3 + 0], m_vertices[m_Faces[i+2] * 3 +1], m_vertices[m_Faces[i+2] * 3 + 2]);
		
			glVertex3f(m_vertices[m_Faces[i+2] * 3 + 0], m_vertices[m_Faces[i+2] * 3 +1], m_vertices[m_Faces[i+2] * 3 + 2]);
			glVertex3f(m_vertices[m_Faces[i+0] * 3 + 0], m_vertices[m_Faces[i+0] * 3 +1], m_vertices[m_Faces[i+0] * 3 + 2]);
			glEnd();
		}
	}

	glPopMatrix();
}

void ObjMesh::SetColor(float r, float g, float b)
{
	m_Color[0] = r;
	m_Color[1] = g;
	m_Color[2] = b;
}

