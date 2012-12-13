#if !defined(AFX_BAISCOBJ_H__6F90C6A0_F5E4_4482_BA6B_136D5C922B31__INCLUDED_)
#define AFX_BAISCOBJ_H__6F90C6A0_F5E4_4482_BA6B_136D5C922B31__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


extern HWND hWnd;

class baiscobj  
{
public:
	baiscobj();
	virtual ~baiscobj();
public:

	float		g_terrain	[MAP_W*MAP_W][3];
	GLuint		g_index		[MAP_W*MAP_W* 2];
	float		g_texcoord	[MAP_W*MAP_W][2];
	void		InitTerrain(float h);	
	void		DrawSand();	
	float		GetHeight(float x, float z); 

	float	    g_eye [3]; 
	float	    g_look[3];
	float	    rad_xz;	
	float	    g_Angle;
	float	    g_elev;	
	BOOL		DisplayScene();	

	UINT		g_cactus[16];
	BITMAPINFOHEADER  g_bit;
	unsigned char    *g_imageData; 
	void		CreateSkyBox(int a,int wi,int he,int le);
	void		texture(UINT textur);
	void		light0();    

	void		picter(float x,float y,float z); 
	bool		LoadT8(char *filename, GLuint &texture);
	void		LoadT16(char *filename, GLuint &texture);
	unsigned char* LoadBit(char *filename, BITMAPINFOHEADER *bitmap);

};

#endif // !defined(AFX_BAISCOBJ_H__6F90C6A0_F5E4_4482_BA6B_136D5C922B31__INCLUDED_)
