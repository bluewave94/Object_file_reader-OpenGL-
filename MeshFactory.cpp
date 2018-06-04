///////////////////////////////////////////////////////
////////// 20134007 김민식                 ////////////
////////// 기말 프로젝트 - obj 파일 로더  ////////////
////////// MeshFactory.cpp                ////////////
///////////////////////////////////////////////////////
#pragma once
#include "MeshFactory.h"
#include <windows.h>
#include "ObjMesh.h"

void CMeshFacotry::LoadObjModel(char* fileDir, ObjMesh* mesh)
{
	mesh->model_name = fileDir;
	printf("%s\n", mesh->model_name);

	FILE* fp = fopen(fileDir, "r");
	if(fp == NULL){
		printf("%s file can not open", fileDir);
		exit(1);
	}
	int numVertex = 0;
	int numFaces = 0;
	int numNormals = 0;
	int numTexcoords = 0;
	
	float* vertices = NULL;
	float* normals = NULL;
	int* faces = NULL;
	int* textureFace = NULL;
	float* texCoords = NULL;

	char line[256];
	
	while(!feof(fp)){
		fgets(line, 256, fp);
		if(line[0] == 'v'){
			if(line[1] == 't'){			//texture
				numTexcoords++;
			}
			else if(line[1] == 'n'){	//normal
				numNormals++;
			}
			else if(line[1] == ' ')		//vertex
				numVertex++;
		}
		else if(line[0] == 'f'){		//face
			numFaces++;			
		}
	}
	rewind(fp);
	
	printf("number of vertices : %d\n", numVertex);
	printf("number of indices : %d\n", numFaces);
	printf("number of textures : %d\n", numTexcoords);
	
	vertices = (float*)malloc(sizeof(float) * 3 * numVertex);		//정점 
	texCoords = (float*)malloc(sizeof(float) * 2 * numTexcoords);	//텍스쳐 
	faces = (int*)malloc(sizeof(int) * 3 * numFaces);				//면 (정점)
	textureFace = (int*)malloc(sizeof(int) * 3 * numFaces);			//면 (텍스쳐)
	normals = (float*)malloc(sizeof(float) * 3 * numNormals); // 노말

	int j =0, k = 0, l = 0, m = 0;
	int IdxFace = 0;
	int IdxTexCoord = 0;

	while(!feof(fp)){
		fgets(line, 256, fp);
		if(line[0] == 'v'){				//정점만 읽기// vt는 텍스쳐 정보 
			if(line[1] == 't'){			//texture
				fseek(fp, -(strlen(line)+1), SEEK_CUR);		//한줄 앞으로
				
				float x, y, z;
				fscanf(fp, "%s %f %f %f", line, &x, &y, &z);
				texCoords[l++] = x;	texCoords[l++] = y;
				//printf("%f %f\n", texCoords[l-2], texCoords[l-1]);
			}
			else if(line[1] == 'n'){	//normal

			} 
			else{						//vertex
				fseek(fp, -(strlen(line)+1), SEEK_CUR);		//한줄 앞으로
				
				float x, y, z;
				fscanf(fp, "%s %f %f %f", line, &x, &y, &z);
				vertices[j++] = x;	vertices[j++] = y;	vertices[j++] = z;
				//printf("%f %f %f\n", vertices[j-3], vertices[j-2], vertices[j-1]);
			}			
		}
		else if(line[0] == 'f'){		//면정보 읽기
			if(numTexcoords > 0){		//Obj 텍스쳐가 있으면 "/" 구분자를 구별 해야 함
				for (int i = 0; i < strlen(line); ++i){
					if(line[i] == '/')	line[i] = ' ';
				}
				
				char* token;
				token = strtok(line, " ");	//token이 line문자열을 공백 기준으로 분리한 결과를 담음 "f 1212 1212 2323 2323 4343 4343"
				
				int p1 = atoi(strtok(NULL, " ")), t1 = atoi(strtok(NULL, " "));
				int p2 = atoi(strtok(NULL, " ")), t2 = atoi(strtok(NULL, " "));
				int p3 = atoi(strtok(NULL, " ")), t3 = atoi(strtok(NULL, " "));	
				
				//면의 정점 순서 정보
				faces[IdxFace++] = p1-1;			faces[IdxFace++] = p2-1;			faces[IdxFace++] = p3-1;
				//면의 텍스쳐 순서 정보
				textureFace[IdxTexCoord++] = t1-1;	textureFace[IdxTexCoord++] = t2-1;	textureFace[IdxTexCoord++] = t3-1;

				//printf("%d %d %d %d %d %d\n", p1, t1, p2, t2, p3, t3);
			}
			else{						//텍스쳐가 없는 면 정보
				int f1, f2, f3;	
				fseek(fp, -(strlen(line)+1), SEEK_CUR);		//한줄 앞으로
				fscanf(fp, "%s %d %d %d", line, &f1, &f2, &f3);		//Obj 텍스쳐가 없으면 3개의 면정보만 존재 
				faces[k++] = f1-1;	faces[k++] = f2-1;	faces[k++] = f3-1;
				//printf("%d %d %d\n", faces[j-3], faces[j-2], faces[j-1]);
			}
		}
	}
	
	//정점
	mesh->m_numVertices = numVertex;
	mesh->m_vertices = vertices;
	
	//텍스쳐 
	mesh->m_numTexCoords = numTexcoords;
	mesh->m_texCoords = texCoords;

	//면정보, 정점, 텍스쳐 순서
	mesh->m_numFaces = numFaces;
	mesh->m_TextureFace = textureFace;
	mesh->m_Faces = faces;

	//노말
	mesh->m_numNormals = numNormals;
	mesh->m_normals = normals;

}
