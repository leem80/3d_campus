#include "StdAfx.h"
#include "baiscobj.h"
#include "bitmap.h"


extern GLfloat	r; 
float  gao=1.8f;
extern CString	test;
//////////////////////////////////////////////////////////////////////
baiscobj::baiscobj()
{	g_eye[0]= MAP;//
g_eye[2]=-MAP;//
g_Angle=0;//
g_elev=-0;//

////////////////////////////////////////////////////////////////////////
g_imageData = LoadBit("../images/Terrain1.bmp",&g_bit);	//调等高地形图
LoadT8("../images/sand0.bmp",	 g_cactus[0]);	//地面帖图
LoadT8("../images/2RBack.bmp",g_cactus[2]);	//天空贴图后
LoadT8("../images/2Front.bmp",g_cactus[3]);	//天空贴图前
LoadT8("../images/2Top.bmp",	 g_cactus[4]);	//天空贴图顶
LoadT8("../images/2Left.bmp", g_cactus[5]);	//天空贴图左
LoadT8("../images/2Right.bmp",g_cactus[6]);	//天空贴图右

InitTerrain(5);//初始化地面

glEnable(GL_TEXTURE_2D);
}
baiscobj::~baiscobj()
{	for(int i=0;i<16;i++) glDeleteTextures(1, &g_cactus[i]);
glDisableClientState(GL_VERTEX_ARRAY);
glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}
void baiscobj::light0()
{	GLfloat light_position[] = {1.0,5.0,1.0,1.0};
glLightfv(GL_LIGHT0, GL_POSITION, light_position);
glEnable(GL_LIGHTING);
glEnable(GL_LIGHT0);
glEnable(GL_DEPTH_TEST);
glEnable(GL_COLOR_MATERIAL);
}
BOOL baiscobj::DisplayScene()
{ float speed=0.5f;	

float x=g_eye[0],y=g_eye[2],z=g_eye[2];
if (KEY_DOWN(VK_SHIFT))  speed   =speed*2;
if (KEY_DOWN(VK_LEFT))   g_Angle-=speed*2;
if (KEY_DOWN(VK_RIGHT))  g_Angle+=speed*2;
rad_xz = float (3.13149* g_Angle/180.0f);
if (KEY_DOWN(33))		   g_elev +=speed;
if (KEY_DOWN(34))		   g_elev -=speed;
if (g_elev<-360)		   g_elev  =-360;
if (g_elev> 360)		   g_elev  = 360;
if (KEY_DOWN(VK_UP))	
{ g_eye[2]+=(float)sin(rad_xz)*speed;	
g_eye[0]+=(float)cos(rad_xz)*speed;	
}
if (KEY_DOWN(VK_DOWN))
{ g_eye[2]-=(float)sin(rad_xz)*speed;	
g_eye[0]-=(float)cos(rad_xz)*speed;	
}
if(g_eye[0]<  MAP_SCALE)			g_eye[0]=  MAP_SCALE;
if(g_eye[0]> (MAP_W-2)*MAP_SCALE)	g_eye[0]= (MAP_W-2)*MAP_SCALE;
if(g_eye[2]<-(MAP_W-2)*MAP_SCALE)	g_eye[2]=-(MAP_W-2)*MAP_SCALE;
if(g_eye[2]> -MAP_SCALE)			g_eye[2]= -MAP_SCALE;
g_eye[1] =GetHeight((float)g_eye[0],(float)g_eye[2])+gao;
g_look[0] = (float)(g_eye[0] +100*cos(rad_xz));
g_look[2] = (float)(g_eye[2] +100*sin(rad_xz));
g_look[1] = g_eye[1] +g_elev;	
gluLookAt(g_eye[0],g_eye[1],g_eye[2],
	g_look[0],g_look[1],g_look[2],
	0.0,1.0,0.0	
	);
int r0=abs((int)g_Angle);

return TRUE;
}
//==========================================================================
void baiscobj::InitTerrain(float h)
{ int index = 0;
int Vertex;
for (int z = 0; z < MAP_W; z++)
	for (int x = 0; x < MAP_W; x++)
	{ Vertex = z * MAP_W + x;
g_terrain [Vertex][0] = float(x)*MAP_SCALE;
g_terrain [Vertex][1] = (float)(g_imageData[(z*MAP_W+x)*3]/3);
g_terrain [Vertex][2] = -float(z)*MAP_SCALE;
g_texcoord[Vertex][0] = (float) x;
g_texcoord[Vertex][1] = (float) z;	
g_index [index++] = Vertex;
g_index [index++] = Vertex+ MAP_W;
}
glEnableClientState(GL_VERTEX_ARRAY);	
glVertexPointer    (3,GL_FLOAT,0,g_terrain);
glEnableClientState(GL_TEXTURE_COORD_ARRAY);
glTexCoordPointer  (2,GL_FLOAT,0,g_texcoord);
}
void baiscobj::DrawSand()
{ glBindTexture(GL_TEXTURE_2D, g_cactus[0]);
glTexEnvf    (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
for (int z = 0; z < MAP_W-1; z++)
	glDrawElements(GL_TRIANGLE_STRIP,MAP_W*2,GL_UNSIGNED_INT,&g_index[z*MAP_W*2]);
}
float baiscobj::GetHeight(float x, float z)
{ 	float CameraX = x/MAP_SCALE;
float CameraZ =-z/MAP_SCALE;  
int Col0 = int(CameraX);  
int Row0 = int(CameraZ);
int Col1 = Col0 + 1;
int Row1 = Row0 + 1; 
if (Col1 > MAP_W)	Col1 = 0;
if (Row1 > MAP_W)	Row1 = 0;
float h00=g_terrain[Col0 + Row0*MAP_W][1];
float h01=g_terrain[Col1 + Row0*MAP_W][1];
float h11=g_terrain[Col1 + Row1*MAP_W][1];
float h10=g_terrain[Col0 + Row1*MAP_W][1];
float tx =CameraX - int(CameraX);
float ty =CameraZ - int(CameraZ);
float txty = tx * ty;
return h00*(1.0f-ty-tx+txty) 
	+ h01*(tx-txty)
	+ h11*txty
	+ h10*(ty-txty); 
}
void baiscobj::CreateSkyBox(int a,int wi,int he,int le)
{	float width =MAP*wi;
float height=MAP*he;
float length=MAP*le;
float x = MAP  -width /2;
float y = MAP/a-height/2;
float z = -MAP -length/2;
///////////////////////////////////////////////////////////////////////////////
texture(g_cactus[2]);
glBegin(GL_QUADS);
glTexCoord2f(1.0f,0.0f); glVertex3f(x+width,y,		 z);
glTexCoord2f(1.0f,1.0f); glVertex3f(x+width,y+height,z); 
glTexCoord2f(0.0f,1.0f); glVertex3f(x,		y+height,z);
glTexCoord2f(0.0f,0.0f); glVertex3f(x,		y,		 z);
glEnd();
texture(g_cactus[3]);
glBegin(GL_QUADS);
glTexCoord2f(1.0f,0.0f); glVertex3f(x,		y,		 z+length);
glTexCoord2f(1.0f,1.0f); glVertex3f(x,		y+height,z+length);
glTexCoord2f(0.0f,1.0f); glVertex3f(x+width,y+height,z+length); 
glTexCoord2f(0.0f,0.0f); glVertex3f(x+width,y,		 z+length);
glEnd();

texture(g_cactus[4]);
glBegin(GL_QUADS);	
glTexCoord2f(0.0f,1.0f); glVertex3f(x+width,y+height,z);
glTexCoord2f(0.0f,0.0f); glVertex3f(x+width,y+height,z+length); 
glTexCoord2f(1.0f,0.0f); glVertex3f(x,		y+height,z+length);
glTexCoord2f(1.0f,1.0f); glVertex3f(x,		y+height,z);
glEnd();
texture(g_cactus[5]);
glBegin(GL_QUADS);
glTexCoord2f(1.0f,1.0f); glVertex3f(x,		y+height,z);	
glTexCoord2f(0.0f,1.0f); glVertex3f(x,		y+height,z+length); 
glTexCoord2f(0.0f,0.0f); glVertex3f(x,		y,		 z+length);
glTexCoord2f(1.0f,0.0f); glVertex3f(x,		y,		 z);		
glEnd();
texture(g_cactus[6]);
glBegin(GL_QUADS);
glTexCoord2f(0.0f,0.0f); glVertex3f(x+width,y,		 z);
glTexCoord2f(1.0f,0.0f); glVertex3f(x+width,y,		 z+length);
glTexCoord2f(1.0f,1.0f); glVertex3f(x+width,y+height,z+length); 
glTexCoord2f(0.0f,1.0f); glVertex3f(x+width,y+height,z);
glEnd();
}
void baiscobj::texture(UINT textur)
{	glBindTexture  (GL_TEXTURE_2D, textur);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
}
//==========================================================================
void baiscobj::picter(float x,float y,float z)
{y=GetHeight(x,z);
glDisable(GL_TEXTURE_2D);
glPushAttrib(GL_CURRENT_BIT);
glPushMatrix();
glTranslatef(x,y+0.5f,z);
glColor3f(0.0f,1.0f,0.2f);
auxSolidCube(1);
glTranslatef(0.0f,0.8f,0.0f);
glColor3f(0.0f,0.0f,1.0f);
auxSolidBox(.2f,1.3f,.2f);
glPopMatrix();
glPushMatrix();
glTranslatef(x,y+2.5f,z);
glRotatef(r-90,0.0,1.0,0.0);
//=======================================
glColor3f(1.0f,1.0f,1.0f);
glRotatef(45, 1.0, 0.0, 0.0);
auxWireCone(1.5,0.6f);	
//=======================================
glRotatef(180, 1.0, 0.0, 0.0);
glTranslatef(0.0f,0.0f,-0.7f);
auxWireCone(0.2f,2.0f);	
glColor3f(FRAND,0,0);
glTranslatef(0.0f,0.0f,2.0f);
auxSolidSphere(0.1f);
glPopMatrix();

glEnable(GL_TEXTURE_2D);
glPopAttrib();
r+=1.0f;if(r>360) r=0;
glEnable(GL_TEXTURE_2D);
}
bool baiscobj::LoadT8(char *filename, GLuint &texture)
{	AUX_RGBImageRec *pImage = NULL;
pImage = auxDIBImageLoad(filename);
if(pImage == NULL)		return false;
glGenTextures(1, &texture);	
glBindTexture    (GL_TEXTURE_2D,texture);
gluBuild2DMipmaps(GL_TEXTURE_2D,4, pImage->sizeX, 
	pImage->sizeY,GL_RGB, GL_UNSIGNED_BYTE,pImage->data);
free(pImage->data);
free(pImage);	
return true;
}
void baiscobj::LoadT16(char *filename, GLuint &texture)
{ glGenTextures(1, &texture);  
glBindTexture(GL_TEXTURE_2D, texture);
BITMAPINFOHEADER bitHeader;
unsigned char *buffer;  
buffer=LoadBitmapFileWithAlpha(filename,&bitHeader);
gluBuild2DMipmaps	( GL_TEXTURE_2D,  
	4,    
	bitHeader.biWidth, 
	bitHeader.biHeight,
	GL_RGBA, 
	GL_UNSIGNED_BYTE,
	buffer  
	); 
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR); 
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
free(buffer);  
}
unsigned char * baiscobj::LoadBit(char *filename, BITMAPINFOHEADER *bitmap)
{ FILE *filePtr;  
BITMAPFILEHEADER  Header; 
unsigned char    *Image; 
unsigned int      imageIdx = 0; 
unsigned char     tempRGB;  
filePtr = fopen(filename, "rb");
if (filePtr == NULL)    return NULL;
fread(&Header, sizeof(BITMAPFILEHEADER), 1, filePtr);
if (Header.bfType != BITMAP_ID)
{ fclose(filePtr);
return NULL;
}
fread(bitmap, sizeof(BITMAPINFOHEADER), 1, filePtr);
fseek(filePtr, Header.bfOffBits, SEEK_SET);
Image = (unsigned char*)malloc(bitmap->biSizeImage);
if (!Image)
{ free(Image);
fclose(filePtr);
return NULL;
}
fread(Image, 1, bitmap->biSizeImage, filePtr);
if (Image == NULL)
{ fclose(filePtr);
return NULL;
}
for (imageIdx = 0; imageIdx < bitmap->biSizeImage; imageIdx+=3)
{ tempRGB = Image[imageIdx];
Image[imageIdx] = Image[imageIdx + 2];
Image[imageIdx + 2] = tempRGB;
}
fclose(filePtr);
return Image;
}
/////////////////////////////////////////////////////////////////////////////

