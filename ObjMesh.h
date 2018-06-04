// ObjMesh.h: interface for the ObjMesh class.
//
//////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////
////////// 20134007 김민식                 ////////////
////////// 기말 프로젝트 - obj 파일 로더  ////////////
////////// ObjMesh.h                        ////////////
///////////////////////////////////////////////////////

#if !defined(AFX_OBJMESH_H__796231AB_E521_4A69_8FDA_D1E69EE353C0__INCLUDED_)
#define AFX_OBJMESH_H__796231AB_E521_4A69_8FDA_D1E69EE353C0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class ObjMesh  
{
public:
	ObjMesh();
	virtual ~ObjMesh();
	void Render(int line, int faces, int texture); 
	void SetColor(float r, float g, float b);

	char* model_name; // obj 파일 이름

	float* m_vertices;		//정점 배열 
	float* m_texCoords;		//텍스쳐 배열
	float* m_normals;
	
	int* m_Faces;			//면(삼각형을 이루는 정점 3개)
	int* m_TextureFace;		//면(삼각형을 이루는 텍스쳐 3개)
	
	float m_Color[3];

	int m_numVertices;
	int m_numFaces;
	int m_numNormals;
	int m_numTexCoords;
};

#endif // !defined(AFX_OBJMESH_H__796231AB_E521_4A69_8FDA_D1E69EE353C0__INCLUDED_)
