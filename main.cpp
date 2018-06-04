///////////////////////////////////////////////////////
////////// 20134007 ��ν�                 ////////////
////////// �⸻ ������Ʈ - obj ���� �δ�  ////////////
////////// main.cpp                          ////////////
///////////////////////////////////////////////////////
#include <iostream>
#include <cmath>
#include <GL/glut.h>

#include "MeshFactory.h"
#include "ObjMesh.h"

using namespace std;

void Display();
void Keyboard(unsigned char key, int x, int y);

void model_menu(int value); // �޴� ���� �Լ�
void mouse_click(int button, int state, int x, int y);
// Ŭ���� ����Ʈ�� ī�޶� ���� ������ �� �ֵ��� ����

// ī�޶� ���� �Լ�
void arrow_key(int key, int x, int y); // ī�޶� �̵�
void rotate_camera(); // ī�޶� �¿� ȸ��
void camera_zoom_up();
void camera_zoom_down();

// ���� ���� �Լ�
void moving_light(); // ������ �̵��ϴ� �Լ�
void light(); // ��
void change_light_color(); // ������ ���� ����
void light_reset(); // ���� ������ �ʱ�ȭ

///////////////////// ����ġ ////////////////////////////////
int line_switch = 0; 	// �ܰ��� on(1) , off(0)
int face_switch = 1;  // �� on(1) , off(0)
int light_effect_1_switch = 0; // ���� ȸ�� ȿ��
int light_effect_2_switch = 0; // ���� �� ��ȭ
int grid_switch = 1; // Grid() ���� ���� : on(1), off(0)
int texture_switch = 1; // �ؽ��� on(1), off(0)
int viewport = 0; // 0 : ����, 1 : ��, 2 : ��, 3 : ��
////////////////////////////////////////////////////////////

void Grid();
ObjMesh objMesh;

//////////////////////////////////// ī�޶� /////////////////////////////////////////////
float camPos[3] = {0, 10, 45}; // ī�޶� ��ġ // ����Ʈ : 0, 10, 45
float cntPos[3] = {0, 10, 0}; // ī�޶��� �ü� // ����Ʈ : 0, 10, 0
GLfloat move_x = 0.0, move_y = 0.0, move_z = 0.0; // ī�޶� �̵� -> glTranslatef();
float theta = 0.0;  float theta_y = 0.0;// ī�޶� ȸ�� ���� (�ﰢ�Լ� ��� ����)
int spin = 0.0; // ����
float zoom = 45; float zoom_top = 45; float zoom_side = 45; float zoom_front = 45;// ī�޶��� ��
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////// �� ////////////////////////////////////////
GLfloat mat_spec[] = {1.0, 1.0, 1.0, 1.0}; // ���׸���
GLfloat mat_shine[] = { 100.0 };

GLfloat light_position[] = {30.0, 30.0, 30.0, 1.0}; // ��ġ
GLfloat light_ambient[] = {1.0, 1.0, 1.0, 1.0}; // GL_AMBIENT
GLfloat light_diffuse[] = {1.0, 1.0, 1.0, 1.0}; // GL_SPECULAR
GLfloat light_spec[] = {1.0, 1.0, 1.0, 1.0}; // GL_SPECULAR
//////////////////////////////////////////////////////////////////////////


///////////////////////// ���� ȿ�� //////////////////////////////
int moving_mode = 0; // ������ ȿ���� �����ϴ� ����ġ
// 0 : ȿ�� ����(�ʱ� ���·� ���ư� ����)
// 1 : ������ ȸ���� �Ѵ�.
// 2 : ������ ���� ���Ѵ�.
float direction_r = 0.01; // ���� ���ϴ� �ӵ� �� ����
float direction_g = 0.008; // ���� ���ϴ� �ӵ� �� ����
float direction_b = 0.005; // ���� ���ϴ� �ӵ� �� ����
//////////////////////////////////////////////////////////////////

void Init()
{
	glDepthFunc(GL_LESS); // Depth Buffer Setup
    
    glClearColor (0.2, 0.2, 0.2, 0.0); // ��� �� : ȸ��
	glEnable(GL_SMOOTH);

	light();

	glEnable(GL_DEPTH_TEST); // Enables Depth Testing
}

void Reshape (int w, int h) 
{
    glViewport (0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode (GL_PROJECTION); 

	glLoadIdentity();
    gluPerspective (60, (GLfloat)w / (GLfloat)h, 0.1 , 100000.0);
	
    glMatrixMode (GL_MODELVIEW); 
}

int main (int argc, char **argv) 
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 800);
    glutCreateWindow("20134007 obj loader : ī�޶� �� = free_view");

	CMeshFacotry::LoadObjModel("model/raspberry.obj", &objMesh);

	GLint SubMenu = glutCreateMenu(model_menu); // ���� �𵨸� �޴�
	// ������ ���� �𵨸� ����
	glutAddMenuEntry("tea", 1);
	glutAddMenuEntry("box", 2);
	glutAddMenuEntry("shave", 3);
	glutAddMenuEntry("fox", 4);
	glutAddMenuEntry("treasure_chest", 15);

	GLint line_mode = glutCreateMenu(model_menu); // ������ ��� �޴�
	// ���� �������ϴ� �͵� �����ϵ��� ����
	glutAddMenuEntry("�ܰ���", 5);
	glutAddMenuEntry("��", 6);
	glutAddMenuEntry("�׸���", 7);
	glutAddMenuEntry("�ؽ���", 8);

	GLint light_mode = glutCreateMenu(model_menu); // ���� ȿ�� �޴�
	// Ű����� �۵��ߴ� ���� ȿ���� �޴��� Ű�ų� ���� �ֵ��� ����
	// ȸ�� ȿ���� �� ��ȯ ȿ���� ���ÿ� �� �� �ִ�.
	// �� ȿ���� ���� �� ��, ������ �ؽ��ĸ� ���� �����ؾ� ���̰� ���� ���.
	glutAddMenuEntry("���� ȸ��", 9);
	glutAddMenuEntry("���� �� ����", 10);
	glutAddMenuEntry("�ʱ�ȭ", 11);
	
	GLint modeling = glutCreateMenu(model_menu);
	glutAddMenuEntry("raspberry", 12);
	glutAddMenuEntry("sample(�ؽ���)", 13);
	glutAddMenuEntry("torus(�׽�Ʈ��)", 14);

	glutCreateMenu(model_menu);
	glutAddSubMenu("���� ��", modeling); // �������� �𵨸�
	glutAddSubMenu("���� ��", SubMenu);
	glutAddSubMenu("������ ���", line_mode);
	glutAddSubMenu("���� ȿ��", light_mode);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	Init();
	
    glutDisplayFunc(Display);
    glutReshapeFunc(Reshape);
    glutKeyboardFunc(Keyboard);
	glutSpecialFunc(arrow_key);
	glutIdleFunc(moving_light);
	glutMouseFunc(mouse_click);

    glutMainLoop();

    return 0;
}

void light()
{
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_spec);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shine);

	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_spec);

	glEnable(GL_LIGHTING);

	glPushMatrix();
	glEnable(GL_LIGHT0);
	glPopMatrix();
}

void mouse_click(int button, int state, int x, int y)
// ����Ʈ�� Ŭ���ϸ� Ŭ���� ����Ʈ�� ī�޶� ���� ������ �� �ִ�.
{
	if(button == GLUT_LEFT_BUTTON, state == GLUT_DOWN)
	{
		if(x > 0 && x < 400 && y > 0 && y < 400)
		{
			glutSetWindowTitle("20134007 obj loader : ī�޶� �� = top");
			viewport = 1;
		}
		if(x > 0 && x < 400 && y > 400 && y < 800)
		{
			glutSetWindowTitle("20134007 obj loader : ī�޶� �� = front");
			viewport = 2;
		}
		if(x > 400 && x < 800 && y > 0 && y < 400)
		{
			glutSetWindowTitle("20134007 obj loader : ī�޶� �� = side");
			viewport = 3;
		}
		if(x > 400 && x < 800 && y > 400 && y < 800)
		{
			glutSetWindowTitle("20134007 obj loader : ī�޶� �� = free_view");
			viewport = 0;
		}
	}
}

void model_menu(int value) // �޴�
{
	switch(value)
	{
		case 1: // ���� �𵨸� : tea
			CMeshFacotry::LoadObjModel("model/tea.obj", &objMesh);
			texture_switch = 0;
			glutPostRedisplay();
			break;

		case 2: // ���� �𵨸� : box
			CMeshFacotry::LoadObjModel("model/box.obj", &objMesh);
			texture_switch = 1; // -> �ؽ��İ� �ִ� �𵨸��� �ڵ����� �ؽ��� ����Ű�� ������.
			glutPostRedisplay();
			break;

		case 3: // ���� �𵨸� shave
			CMeshFacotry::LoadObjModel("model/shave.obj", &objMesh);
			texture_switch = 1;
			glutPostRedisplay();
			break;

		case 4: // ���� �𵨸� fox
			CMeshFacotry::LoadObjModel("model/fox.obj", &objMesh);
			texture_switch = 0;
			glutPostRedisplay();
			break;

		case 15: // ���� �𵨸� treasure_chest
			CMeshFacotry::LoadObjModel("model/treasure_chest.obj", &objMesh);
			texture_switch = 1;
			glutPostRedisplay();
			break;

		case 5: // �ܰ��� 
			if(line_switch == 1) line_switch = 0;
			else line_switch = 1;

			glutPostRedisplay();
			break;

		case 6: // ��
			if(face_switch == 1) face_switch = 0;
			else face_switch = 1;

			glutPostRedisplay();
			break;

		case 7: // �׸���
			if(grid_switch == 1) grid_switch = 0;
			else grid_switch = 1;

			glutPostRedisplay();
			break;

		case 8: // �ؽ���
			if(texture_switch == 1) texture_switch = 0;
			else if(objMesh.m_numTexCoords != 0) texture_switch = 1;
			// �ؽ��İ� ���� �𵨸��� Ŭ���ص� ������

			glutPostRedisplay();
			break;

		case 9: // �˾� �޴� : "ȸ�� On/Off"
			if(light_effect_1_switch == 1)
			{
				light_effect_1_switch = 0;
			}
			else
			{
				light_position[0] = 30.0;
				light_position[1] = 10;
				light_position[2] = 0.0;

				light_effect_1_switch = 1;
				glutIdleFunc(moving_light);
			}
			break;

		case 10: // �˾� �޴� : "�� ��ȯ On/Off"
			if(light_effect_2_switch == 1)
			{
				light_effect_2_switch = 0;
			}
			else
			{
				light_ambient[0] = 0.1;
				light_ambient[1] = 0.1;
				light_ambient[2] = 0.1;

				light_effect_2_switch = 1;
				glutIdleFunc(moving_light);
			}
			break;

		case 11: // �˾� �޴� : "����"
				light_effect_1_switch = 0;
				light_effect_2_switch = 0;
				light_reset();
				glutIdleFunc(NULL);
				glutPostRedisplay();
				break;

		case 12: // ���� �𵨸� : �����
			CMeshFacotry::LoadObjModel("model/raspberry.obj", &objMesh);
			texture_switch = 1;
			glutPostRedisplay();
			break;

		case 13: // ���� �𵨸� : ���� ��(�ؽ��İ� �����)
			CMeshFacotry::LoadObjModel("model/sample(texture).obj", &objMesh);
			texture_switch = 1;
			glutPostRedisplay();
			break;

		case 14: // ���� �𵨸� : �䷯��(�׽�Ʈ�� �ӽ� �𵨸�)
			CMeshFacotry::LoadObjModel("model/torus.obj", &objMesh);
			texture_switch = 0;
			glutPostRedisplay();
			break;
	}
}

void Display (void) 
{
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear the color buffer and the depth buffer

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glPushMatrix();
	glRotatef(spin, 0.0, 0.0, 1.0); // ���� ȸ�� �̵� (moving_light()�� �۵��� �� ����)
	light();
	glPopMatrix();

	////////////////////// ���� ����Ʈ //////////////////////////
	glPushMatrix();
	glViewport(0, 0, 400, 400);
	gluLookAt(0.0, 10.0, zoom_front, 0.0, 10.0, 0.0, 0.0, 1.0, 0.0);
	Grid();
	objMesh.Render(line_switch, face_switch, texture_switch);
	glPopMatrix();
	////////////////////////////////////////////////////////////////

	////////////////////// ���� ����Ʈ //////////////////////////
	glPushMatrix();
	glViewport(0, 400, 400, 400);
	gluLookAt(0.0, zoom_top, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1.0);
	Grid();
	objMesh.Render(line_switch, face_switch, texture_switch);
	glPopMatrix();
	////////////////////////////////////////////////////////////////

	////////////////////// ī�޶� ����Ʈ //////////////////////////
	glPushMatrix();
	glViewport(400, 0, 400, 400);

	glTranslatef(move_x, move_y, move_z);
    gluLookAt(camPos[0], camPos[1], camPos[2], 0.0, 10.0, 0.0, 0.0, 1.0, 0.0);
	// �⺻ ��ǥ : gluLookAt(0.0, 10.0, 45.0, 0.0, 10.0, 0.0, 0.0, 1.0, 0.0);
	if(grid_switch == 1) Grid();
	objMesh.Render(line_switch, face_switch, texture_switch);
	glPopMatrix();
	////////////////////////////////////////////////////////////////

	////////////////////// ���� ����Ʈ //////////////////////////
	glPushMatrix();
	glViewport(400, 400, 400, 400);
	gluLookAt(zoom_side, 5.0, 0.0, 0.0, 5.0, 0.0, 0.0, 1.0, 0.0);
	Grid();
	objMesh.Render(line_switch, face_switch, texture_switch);
	glPopMatrix();
	////////////////////////////////////////////////////////////////

    glutSwapBuffers(); //swap the buffers
}

void camera_zoom_up() // ī�޶� �� ����
{
	switch(viewport)
	{
		case 0:
			if(zoom < 100.0) zoom += 1;
			else zoom = 99.0;
			rotate_camera();
			glutPostRedisplay();
			break;

		case 1:
			if(zoom_top < 100.0) zoom_top += 1;
			else zoom_top = 99.0;			
			glutPostRedisplay();
			break;

		case 2:
			if(zoom_front < 100.0) zoom_front += 1;
			else zoom_front = 99.0;	
			glutPostRedisplay();
			break;

		case 3:
			if(zoom_side < 100.0) zoom_side += 1;
			else zoom_side = 99.0;	
			glutPostRedisplay();
			break;
	}
}

void camera_zoom_down() // ī�޶� �� ���
{
	switch(viewport)
	{
		case 0:
			if(zoom > 5.0) zoom -= 1;
			else zoom = 5.0;		
			rotate_camera();
			glutPostRedisplay();
			break;

		case 1:		
			if(zoom_top > 5.0) zoom_top -= 1;
			else zoom_top = 5.0;
			glutPostRedisplay();
			break;

		case 2:
			if(zoom_front > 5.0) zoom_front -= 1;
			else zoom_front = 5.0;
			glutPostRedisplay();
			break;

		case 3:
			if(zoom_side > 5.0) zoom_side -= 1;
			else zoom_side = 5.0;
			glutPostRedisplay();
			break;
	}

}

void rotate_camera() // ī�޶� ȸ��
{
	camPos[0] = zoom * sin(theta) * cos(theta_y);
	camPos[1] = zoom * sin(theta_y) + 10;
	camPos[2] = zoom * cos(theta) * cos(theta_y);
} 

void light_reset()
{
	light_position[0] = 30.0;
	light_position[1] = 30.0;
	light_position[2] = 30.0;

	light_ambient[0] = 0.5;
	light_ambient[1] = 0.5;
	light_ambient[2] = 0.5;
}

void moving_light() // ���� ȿ��
{
	if(light_effect_1_switch == 1)
	{
		spin = (spin + 3) % 360;
	}
	if(light_effect_2_switch == 1)
	{
		light_ambient[0] += direction_r;
		light_ambient[1] += direction_g;
		light_ambient[2] += direction_b;

		if(light_ambient[0] >= 1.5 || light_ambient[0] < 0.0) direction_r *= -1;
		if(light_ambient[1] >= 1.5 || light_ambient[1] < 0.0) direction_g *= -1;
		if(light_ambient[2] >= 1.5 || light_ambient[2] < 0.0) direction_b *= -1;
	}

	glutPostRedisplay();
}

void Keyboard(unsigned char key, int x, int y)
{
    if(key == 27) {
        exit(0);
    }
	if(key == 'w') // ī�޶� ��� �̵�
	{
		move_y += 1;
		glutPostRedisplay();
	}
	if(key == 'a') // ī�޶� ���� �̵�
	{
		move_x -= 1;
		glutPostRedisplay();
	}
	if(key == 's') // ī�޶� �ϰ� �̵�
	{
		move_y -= 1;
		glutPostRedisplay();
	}
	if(key == 'd') // ī�޶� ���� �̵�
	{
		move_x += 1;
		glutPostRedisplay();
	}

	if(key == 'q') // ���
	{
		camera_zoom_down();
	}

	if(key == 'e') // Ȯ��
	{
		camera_zoom_up();
	}

}

void arrow_key(int key, int x, int y)
{
	switch (key)
	{
		case 100: // ī�޶� ���� ȸ��
			theta += 0.1;
			rotate_camera();
			glutPostRedisplay();
			break;
		case 101:
			if(theta_y < 0.9) // ���
			{
				theta_y += 0.1;
				rotate_camera();
				glutPostRedisplay();
			}
			break;
		case 102: // ī�޶� ���� ȸ��
			theta -= 0.1;
			rotate_camera();
			glutPostRedisplay();
			break;
		case 103:
			if(theta_y > -0.5) // Ȯ��
			{
				theta_y -= 0.1;
				rotate_camera();
				glutPostRedisplay();
			}	
			break;
	}
}

void Grid()
{
    glPushMatrix();
    glColor3f(1,1,1);
    for(int i=-50; i < 50; i++) 
	{
        glBegin(GL_LINES);
		glNormal3f(0,1,0);
        glVertex3f(i, 0, -50);
        glVertex3f(i, 0, 50);
        glEnd();
    }
	
    for(int i=-50; i < 50; i++) 
	{
        glBegin(GL_LINES);
		glNormal3f(0,1,0);
        glVertex3f(-50, 0, i);
        glVertex3f(50, 0, i);
        glEnd();
    }
	
    glPopMatrix();
}
