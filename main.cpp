///////////////////////////////////////////////////////
////////// 20134007 김민식                 ////////////
////////// 기말 프로젝트 - obj 파일 로더  ////////////
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

void model_menu(int value); // 메뉴 관련 함수
void mouse_click(int button, int state, int x, int y);
// 클릭한 뷰포트의 카메라 줌을 조절할 수 있도록 구현

// 카메라 관련 함수
void arrow_key(int key, int x, int y); // 카메라 이동
void rotate_camera(); // 카메라 좌우 회전
void camera_zoom_up();
void camera_zoom_down();

// 광원 관련 함수
void moving_light(); // 광원을 이동하는 함수
void light(); // 빛
void change_light_color(); // 광원의 색을 변경
void light_reset(); // 광원 데이터 초기화

///////////////////// 스위치 ////////////////////////////////
int line_switch = 0; 	// 외곽선 on(1) , off(0)
int face_switch = 1;  // 면 on(1) , off(0)
int light_effect_1_switch = 0; // 광원 회전 효과
int light_effect_2_switch = 0; // 광원 색 변화
int grid_switch = 1; // Grid() 실행 여부 : on(1), off(0)
int texture_switch = 1; // 텍스쳐 on(1), off(0)
int viewport = 0; // 0 : 자유, 1 : 위, 2 : 앞, 3 : 옆
////////////////////////////////////////////////////////////

void Grid();
ObjMesh objMesh;

//////////////////////////////////// 카메라 /////////////////////////////////////////////
float camPos[3] = {0, 10, 45}; // 카메라 위치 // 디폴트 : 0, 10, 45
float cntPos[3] = {0, 10, 0}; // 카메라의 시선 // 디폴트 : 0, 10, 0
GLfloat move_x = 0.0, move_y = 0.0, move_z = 0.0; // 카메라 이동 -> glTranslatef();
float theta = 0.0;  float theta_y = 0.0;// 카메라 회전 각도 (삼각함수 곡선에 쓰임)
int spin = 0.0; // 각도
float zoom = 45; float zoom_top = 45; float zoom_side = 45; float zoom_front = 45;// 카메라의 줌
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////// 빛 ////////////////////////////////////////
GLfloat mat_spec[] = {1.0, 1.0, 1.0, 1.0}; // 머테리얼
GLfloat mat_shine[] = { 100.0 };

GLfloat light_position[] = {30.0, 30.0, 30.0, 1.0}; // 위치
GLfloat light_ambient[] = {1.0, 1.0, 1.0, 1.0}; // GL_AMBIENT
GLfloat light_diffuse[] = {1.0, 1.0, 1.0, 1.0}; // GL_SPECULAR
GLfloat light_spec[] = {1.0, 1.0, 1.0, 1.0}; // GL_SPECULAR
//////////////////////////////////////////////////////////////////////////


///////////////////////// 광원 효과 //////////////////////////////
int moving_mode = 0; // 광원의 효과를 설정하는 스위치
// 0 : 효과 없음(초기 상태로 돌아가 정지)
// 1 : 광원이 회전을 한다.
// 2 : 광원의 색이 변한다.
float direction_r = 0.01; // 색이 변하는 속도 및 방향
float direction_g = 0.008; // 색이 변하는 속도 및 방향
float direction_b = 0.005; // 색이 변하는 속도 및 방향
//////////////////////////////////////////////////////////////////

void Init()
{
	glDepthFunc(GL_LESS); // Depth Buffer Setup
    
    glClearColor (0.2, 0.2, 0.2, 0.0); // 배경 색 : 회색
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
    glutCreateWindow("20134007 obj loader : 카메라 줌 = free_view");

	CMeshFacotry::LoadObjModel("model/raspberry.obj", &objMesh);

	GLint SubMenu = glutCreateMenu(model_menu); // 샘플 모델링 메뉴
	// 예제로 받은 모델링 파일
	glutAddMenuEntry("tea", 1);
	glutAddMenuEntry("box", 2);
	glutAddMenuEntry("shave", 3);
	glutAddMenuEntry("fox", 4);
	glutAddMenuEntry("treasure_chest", 15);

	GLint line_mode = glutCreateMenu(model_menu); // 렌더링 모드 메뉴
	// 선만 렌더링하는 것도 가능하도록 수정
	glutAddMenuEntry("외곽선", 5);
	glutAddMenuEntry("면", 6);
	glutAddMenuEntry("그리드", 7);
	glutAddMenuEntry("텍스쳐", 8);

	GLint light_mode = glutCreateMenu(model_menu); // 광원 효과 메뉴
	// 키보드로 작동했던 광원 효과를 메뉴로 키거나 끌수 있도록 수정
	// 회전 효과와 색 변환 효과를 동시에 줄 수 있다.
	// 색 효과를 실행 할 땐, 가능한 텍스쳐를 끄고 실행해야 차이가 눈에 띈다.
	glutAddMenuEntry("광원 회전", 9);
	glutAddMenuEntry("광원 색 변경", 10);
	glutAddMenuEntry("초기화", 11);
	
	GLint modeling = glutCreateMenu(model_menu);
	glutAddMenuEntry("raspberry", 12);
	glutAddMenuEntry("sample(텍스쳐)", 13);
	glutAddMenuEntry("torus(테스트용)", 14);

	glutCreateMenu(model_menu);
	glutAddSubMenu("자작 모델", modeling); // 직접만든 모델링
	glutAddSubMenu("샘플 모델", SubMenu);
	glutAddSubMenu("렌더링 모드", line_mode);
	glutAddSubMenu("광원 효과", light_mode);
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
// 뷰포트를 클릭하면 클릭한 뷰포트의 카메라 줌을 조절할 수 있다.
{
	if(button == GLUT_LEFT_BUTTON, state == GLUT_DOWN)
	{
		if(x > 0 && x < 400 && y > 0 && y < 400)
		{
			glutSetWindowTitle("20134007 obj loader : 카메라 줌 = top");
			viewport = 1;
		}
		if(x > 0 && x < 400 && y > 400 && y < 800)
		{
			glutSetWindowTitle("20134007 obj loader : 카메라 줌 = front");
			viewport = 2;
		}
		if(x > 400 && x < 800 && y > 0 && y < 400)
		{
			glutSetWindowTitle("20134007 obj loader : 카메라 줌 = side");
			viewport = 3;
		}
		if(x > 400 && x < 800 && y > 400 && y < 800)
		{
			glutSetWindowTitle("20134007 obj loader : 카메라 줌 = free_view");
			viewport = 0;
		}
	}
}

void model_menu(int value) // 메뉴
{
	switch(value)
	{
		case 1: // 샘플 모델링 : tea
			CMeshFacotry::LoadObjModel("model/tea.obj", &objMesh);
			texture_switch = 0;
			glutPostRedisplay();
			break;

		case 2: // 샘플 모델링 : box
			CMeshFacotry::LoadObjModel("model/box.obj", &objMesh);
			texture_switch = 1; // -> 텍스쳐가 있는 모델링은 자동으로 텍스쳐 스위키가 켜진다.
			glutPostRedisplay();
			break;

		case 3: // 샘플 모델링 shave
			CMeshFacotry::LoadObjModel("model/shave.obj", &objMesh);
			texture_switch = 1;
			glutPostRedisplay();
			break;

		case 4: // 샘플 모델링 fox
			CMeshFacotry::LoadObjModel("model/fox.obj", &objMesh);
			texture_switch = 0;
			glutPostRedisplay();
			break;

		case 15: // 샘플 모델링 treasure_chest
			CMeshFacotry::LoadObjModel("model/treasure_chest.obj", &objMesh);
			texture_switch = 1;
			glutPostRedisplay();
			break;

		case 5: // 외곽선 
			if(line_switch == 1) line_switch = 0;
			else line_switch = 1;

			glutPostRedisplay();
			break;

		case 6: // 면
			if(face_switch == 1) face_switch = 0;
			else face_switch = 1;

			glutPostRedisplay();
			break;

		case 7: // 그리드
			if(grid_switch == 1) grid_switch = 0;
			else grid_switch = 1;

			glutPostRedisplay();
			break;

		case 8: // 텍스쳐
			if(texture_switch == 1) texture_switch = 0;
			else if(objMesh.m_numTexCoords != 0) texture_switch = 1;
			// 텍스쳐가 없는 모델링은 클릭해도 무반응

			glutPostRedisplay();
			break;

		case 9: // 팝업 메뉴 : "회전 On/Off"
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

		case 10: // 팝업 메뉴 : "색 변환 On/Off"
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

		case 11: // 팝업 메뉴 : "중지"
				light_effect_1_switch = 0;
				light_effect_2_switch = 0;
				light_reset();
				glutIdleFunc(NULL);
				glutPostRedisplay();
				break;

		case 12: // 자작 모델링 : 라즈베리
			CMeshFacotry::LoadObjModel("model/raspberry.obj", &objMesh);
			texture_switch = 1;
			glutPostRedisplay();
			break;

		case 13: // 자작 모델링 : 샘플 모델(텍스쳐가 적용됨)
			CMeshFacotry::LoadObjModel("model/sample(texture).obj", &objMesh);
			texture_switch = 1;
			glutPostRedisplay();
			break;

		case 14: // 자작 모델링 : 토러스(테스트용 임시 모델링)
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
	glRotatef(spin, 0.0, 0.0, 1.0); // 빛의 회전 이동 (moving_light()가 작동중 일 때만)
	light();
	glPopMatrix();

	////////////////////// 전면 뷰포트 //////////////////////////
	glPushMatrix();
	glViewport(0, 0, 400, 400);
	gluLookAt(0.0, 10.0, zoom_front, 0.0, 10.0, 0.0, 0.0, 1.0, 0.0);
	Grid();
	objMesh.Render(line_switch, face_switch, texture_switch);
	glPopMatrix();
	////////////////////////////////////////////////////////////////

	////////////////////// 윗면 뷰포트 //////////////////////////
	glPushMatrix();
	glViewport(0, 400, 400, 400);
	gluLookAt(0.0, zoom_top, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1.0);
	Grid();
	objMesh.Render(line_switch, face_switch, texture_switch);
	glPopMatrix();
	////////////////////////////////////////////////////////////////

	////////////////////// 카메라 뷰포트 //////////////////////////
	glPushMatrix();
	glViewport(400, 0, 400, 400);

	glTranslatef(move_x, move_y, move_z);
    gluLookAt(camPos[0], camPos[1], camPos[2], 0.0, 10.0, 0.0, 0.0, 1.0, 0.0);
	// 기본 좌표 : gluLookAt(0.0, 10.0, 45.0, 0.0, 10.0, 0.0, 0.0, 1.0, 0.0);
	if(grid_switch == 1) Grid();
	objMesh.Render(line_switch, face_switch, texture_switch);
	glPopMatrix();
	////////////////////////////////////////////////////////////////

	////////////////////// 측면 뷰포트 //////////////////////////
	glPushMatrix();
	glViewport(400, 400, 400, 400);
	gluLookAt(zoom_side, 5.0, 0.0, 0.0, 5.0, 0.0, 0.0, 1.0, 0.0);
	Grid();
	objMesh.Render(line_switch, face_switch, texture_switch);
	glPopMatrix();
	////////////////////////////////////////////////////////////////

    glutSwapBuffers(); //swap the buffers
}

void camera_zoom_up() // 카메라 줌 설정
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

void camera_zoom_down() // 카메라 줌 축소
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

void rotate_camera() // 카메라 회전
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

void moving_light() // 광원 효과
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
	if(key == 'w') // 카메라 상승 이동
	{
		move_y += 1;
		glutPostRedisplay();
	}
	if(key == 'a') // 카메라 좌측 이동
	{
		move_x -= 1;
		glutPostRedisplay();
	}
	if(key == 's') // 카메라 하강 이동
	{
		move_y -= 1;
		glutPostRedisplay();
	}
	if(key == 'd') // 카메라 우측 이동
	{
		move_x += 1;
		glutPostRedisplay();
	}

	if(key == 'q') // 축소
	{
		camera_zoom_down();
	}

	if(key == 'e') // 확대
	{
		camera_zoom_up();
	}

}

void arrow_key(int key, int x, int y)
{
	switch (key)
	{
		case 100: // 카메라 좌측 회전
			theta += 0.1;
			rotate_camera();
			glutPostRedisplay();
			break;
		case 101:
			if(theta_y < 0.9) // 축소
			{
				theta_y += 0.1;
				rotate_camera();
				glutPostRedisplay();
			}
			break;
		case 102: // 카메라 우측 회전
			theta -= 0.1;
			rotate_camera();
			glutPostRedisplay();
			break;
		case 103:
			if(theta_y > -0.5) // 확대
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
