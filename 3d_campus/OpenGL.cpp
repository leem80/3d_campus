#include "StdAfx.h"
#include "OpenGL.h"

GLfloat	r;
CString	test;
//////////////////////////////////////////////////////////////////////
OpenGL::OpenGL()
{	
}
OpenGL::~OpenGL()
{	CleanUp();
}
BOOL OpenGL::SetupPixelFormat(HDC hDC0)//��ⰲװOpenGL
{	
int nPixelFormat;					  // ���ص��ʽ
hDC=hDC0;
PIXELFORMATDESCRIPTOR pfd = { 
	sizeof(PIXELFORMATDESCRIPTOR),    // pfd�ṹ�Ĵ�С 
	1,                                // �汾�� 
	PFD_DRAW_TO_WINDOW |              // ֧���ڴ����л�ͼ 
	PFD_SUPPORT_OPENGL |              // ֧�� OpenGL 
	PFD_DOUBLEBUFFER,                 // ˫����ģʽ 
	PFD_TYPE_RGBA,                    // RGBA ��ɫģʽ 
	16,                               // 24 λ��ɫ��� 
	0, 0, 0, 0, 0, 0,                 // ������ɫλ 
	0,                                // û�з�͸���Ȼ��� 
	0,                                // ������λλ 
	0,                                // ���ۼӻ��� 
	0, 0, 0, 0,                       // �����ۼ�λ 
	16,                               // 32 λ��Ȼ���     
	0,                                // ��ģ�建�� 
	0,                                // �޸������� 
	PFD_MAIN_PLANE,                   // ���� 
	0,                                // ���� 
	0, 0, 0                           // ���Բ�,�ɼ��Ժ������ģ 
}; 
if (!(nPixelFormat = ChoosePixelFormat(hDC, &pfd)))
{ MessageBox(NULL,"û�ҵ����ʵ���ʾģʽ","Error",MB_OK|MB_ICONEXCLAMATION);
return FALSE;
}
SetPixelFormat(hDC,nPixelFormat,&pfd);//���õ�ǰ�豸�����ص��ʽ
hRC = wglCreateContext(hDC);          //��ȡ��Ⱦ�������
wglMakeCurrent(hDC, hRC);             //������Ⱦ�������
m_baiscobj=new baiscobj();
m_baiscobj->light0();
return TRUE;
}
void OpenGL::init(int Width, int Height)
{	glViewport(0,0,Width,Height);			// ����OpenGL�ӿڴ�С��	
glMatrixMode(GL_PROJECTION);			// ���õ�ǰ����ΪͶӰ����
glLoadIdentity();						// ���õ�ǰָ���ľ���Ϊ��λ����
gluPerspective							// ����͸��ͼ
	( 54.0f,							// ͸�ӽ�����Ϊ 45 ��
	(GLfloat)Width/(GLfloat)Height,	// ���ڵĿ���߱�
	0.1f,								// ��Ұ͸�����:����1.0f
	3000.0f							// ��Ұ͸�����:ʼ��0.1fԶ��1000.0f
	);
// �������������ƣ���һ���������þ�ͷ��Ƕȣ��ڶ��������ǳ���ȣ�������Զ�����С�
glMatrixMode(GL_MODELVIEW);				// ���õ�ǰ����Ϊģ����ͼ����
glLoadIdentity();						// ���õ�ǰָ���ľ���Ϊ��λ����
//	
//====================================================
}
void OpenGL::Render()//OpenGLͼ�δ���
{	glClearColor(0.0f, 0.0f, 0.3f, 1.0f);			 // ����ˢ�±���ɫ
glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);// ˢ�±���
glLoadIdentity();								 // ���õ�ǰ��ģ�͹۲����
//////////////////////////////////////////////////////////////////////////
m_baiscobj->DisplayScene();
m_baiscobj->CreateSkyBox(3,6,3,6);
m_baiscobj->DrawSand();
srand(100);
for(int i=0;i<300;i++)
{float x= RAND_COORD((MAP_W-1)*MAP_SCALE);
float z= RAND_COORD((MAP_W-1)*MAP_SCALE);
float size=4.0f+rand()%4;	
float h=-size/10;	
int   cactus=rand()%4+11;

}
m_baiscobj->picter(MAP+10,0,-MAP);
//////////////////////////////////////////////////////////////////////////
SwapBuffers(hDC);								 // �л�������
}
void OpenGL::CleanUp()
{	 wglMakeCurrent(hDC, NULL);                       //���OpenGL
wglDeleteContext(hRC);                           //���OpenGL
}
