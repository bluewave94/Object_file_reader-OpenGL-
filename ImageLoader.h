///////////////////////////////////////////////////////
////////// 20134007 김민식                 ////////////
////////// 기말 프로젝트 - obj 파일 로더  ////////////
////////// ImageLoader.h                    ////////////
///////////////////////////////////////////////////////
#pragma once
#include <gl/GLAUX.H>
#pragma comment(lib, "glaux.lib ")
#include <stdio.h>
#include <stdlib.h>

AUX_RGBImageRec* LoadBMP(char* Filename)				
{
	FILE *File=NULL;									

	if (!Filename)										
	{
		return NULL;									
	}
	
	File=fopen(Filename,"r");								

	if (File)											
	{
		fclose(File);									
		return auxDIBImageLoad(Filename);				
	}
	
	return NULL;										
}

GLuint LoadGLTextures(char* textureFileName)			
{
	GLuint texID;
	
	AUX_RGBImageRec *TextureImage[1];					
	
	memset(TextureImage,0,sizeof(void *)*1);           	
	
	if (TextureImage[0]=LoadBMP(textureFileName))
	{
		glGenTextures(1, &texID);					// Create The Texture
		
		glBindTexture(GL_TEXTURE_2D, texID);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	}
	
	if (TextureImage[0])									// If Texture Exists
	{
		if (TextureImage[0]->data)							// If Texture Image Exists
		{
			free(TextureImage[0]->data);					// Free The Texture Image Memory
		}
		
		free(TextureImage[0]);								// Free The Image Structure
	}
	
	return texID;											// Return The Status
}