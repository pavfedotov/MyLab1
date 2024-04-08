
//------------------------------------------------------- 
//      GL_2D.cpp
//       =========
//  ��������� ������� �����  � OpenGL
//--------------------------------------------------------
#define STRICT

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <gl\gl.h>
#include <gl\\glu.h>
#include <vector>
#include <string>
// #include <gl\\glaux.h>
#pragma comment (linker, "/defaultlib:opengl32.lib")
#pragma comment (linker, "/defaultlib:glu32.lib")


#define MIN(a,b) ((a)>=(b) ? (b) : (a))
#define MAX(a,b) ((a)>=(b) ? (a) : (b))
#define MsgBox(str) MessageBox(hwndMain, str, aczAppName, 0L)
# define PI           3.14159265358979323846  /* pi */

char aczAppName[512] = "GlBaseSamp";
LPCSTR pczMainWndClass = "WC__FrameWnd";

HINSTANCE hiApp;
HWND hwndMain;
HDC hdcMain;
HGLRC hGlRc;
// ������� ������ � ������� �����������
RECT rcViewport;

//  ������ ������� ������ � ��������� (���������� ���)
# define DIM_SCENE 2.0 

struct Vect {
	double x,y;
};

void InitOpenGL();
void UninitOpenGL();
void InitViewport(const int cx, const int cy);
void Draw(); 
Vect route(Vect,double,double x, double y);
void DrawModel(int  ,int);
LRESULT WINAPI MainWndProc(HWND, UINT, WPARAM, LPARAM);
void MainOnPaint(HWND);
int RegisterWndClasses(void);


const double R = 1.8;
const double W = 0.18;



// ������������� ��������� ��������������� OpenGL
void InitOpenGL()
{
	hdcMain = GetDC(hwndMain);
	int iPixelFormat;
	// ������������� ��������� ������ ��������
	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),  // ������ ���������
		1,                              // ����� ������ OpenGL
		PFD_DRAW_TO_WINDOW |          // ��������� ������ � ����
		PFD_SUPPORT_OPENGL |          // ��������� OpenGL
		PFD_DOUBLEBUFFER,		// ������� �����������
		PFD_TYPE_RGBA,          // ����� � ������ RGBA
		24,                     // 24 ������� �� ����
		0, 0, 0, 0, 0, 0,       // ���� ����� ������������
		0,                      // �� ������������ �����-��������
		0,                      // �������� ������ ������������
		0,                      // ����� ������������ �� ������������
		0, 0, 0, 0,             // ���� ������������ ������������
		32,                     // 32-��������� ����� �������
		0,                      // ����� ��������� �� ������������
		0,                      // ��������������� ����� �� ������������
		PFD_MAIN_PLANE,         // �������� ����
		0,                      // ��������������
		0, 0, 0                 // ����� ���� ������������
	};
	// ������ pix-�������
	iPixelFormat = ChoosePixelFormat(hdcMain, &pfd);
	if (!iPixelFormat)
		MsgBox("�������� ������ ��������");
	// ��������� pix-�������
	if (!SetPixelFormat(hdcMain, iPixelFormat, &pfd))
		MsgBox("������ �������� �� ����������");
	// �������� OpenGL-���������
	hGlRc = wglCreateContext(hdcMain);
	if (!hGlRc)
		MsgBox("�������� OpenGL �� ������");
	// ��������� �������� OpenGL-���������
	if (!wglMakeCurrent(hdcMain, hGlRc))
		MsgBox("�������� OpenGL �� ����������");
}  // func InitOpenGL

 // ������������ ��������� ���������������
void UninitOpenGL()
{
	// �������� ������� �������� ��������������� OpenGL
	wglMakeCurrent(NULL, NULL);
	// ��������� ��������� ��������������� OpenGL
	if (hGlRc)
		wglDeleteContext(hGlRc);
	// ������������ ��������� ������� �������
	ReleaseDC(hwndMain, hdcMain);
}  //func UninitOpenGL

 // ������������� ������� ������
void InitViewport(const int cx,
	const int cy)
{
	// ������ ������� ������ OpenGL �� ����������� ������� ����
	rcViewport.left = 10;
	rcViewport.top = 10;
	int viewSize = MIN(cx, cy) - 20;
	rcViewport.right = rcViewport.left + viewSize;
	rcViewport.bottom = rcViewport.top + viewSize;
	glMatrixMode(GL_PROJECTION);
	glViewport(rcViewport.left,
		cy - rcViewport.bottom,
		viewSize, viewSize);
	glLoadIdentity();
	glOrtho(-DIM_SCENE, DIM_SCENE, -DIM_SCENE, DIM_SCENE, -DIM_SCENE, DIM_SCENE);
	glMatrixMode(GL_MODELVIEW);
}  //func InitViewport

 // ����� ������
void DrawModel(int numVertices,int num)
{
	// ���������� ������ ���� ����
	glClearColor(1, 1, 1, 1.0);

	// ���������� �����.
	double k_gab = 0.95;
	double razmer_figuri = 1.5; // 

	// �������� ����� �����
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3d(1, 1, 1);
	glBegin(GL_LINE_LOOP);
	glVertex2d(-DIM_SCENE * k_gab, -DIM_SCENE * k_gab);
	glVertex2d(DIM_SCENE * k_gab, -DIM_SCENE * k_gab);
	glVertex2d(DIM_SCENE * k_gab, DIM_SCENE * k_gab);
	glVertex2d(-DIM_SCENE * k_gab, DIM_SCENE * k_gab);
	glVertex2d(-DIM_SCENE * k_gab, -DIM_SCENE * k_gab);
	glEnd();




	// ������ ��������� ��������� �������
	double angle;
	double x;
	double y;
	
	double incrR = R / num, incrW = W / num;
	//�������
	double r = R;
	double  w = W;
	for (int i = 0; i < num; i++) {
		

		glBegin(GL_QUADS);
		for (int i = 0; i < numVertices; ++i) {

			angle = (i * 2 * PI / (numVertices));

			x = cos(angle) *r;
			y = sin(angle) * r;


			glColor3d(1, 1, 0);
			Vect
				vertex1 = route({ x + w, y + w }, angle, x, y),
				vertex2 = route({ x - w, y + w }, angle, x, y),
				vertex3 = route({ x - w, y - w }, angle, x, y),
				vertex4 = route({ x + w, y - w }, angle, x, y);

			//auto str = std::to_string(vertex2.x);
			//MessageBox(hwndMain, str.c_str(), "", S_OK);

			glVertex2d(vertex1.x, vertex1.y);
			glVertex2d(vertex2.x, vertex2.y);
			glVertex2d(vertex3.x, vertex3.y);
			glVertex2d(vertex4.x, vertex4.y);

			
		}
		glEnd();
		r -= incrR;
		w -= incrW;
	}
	//������� ����� 
	r = R;  w = W;
	for (int i = 0; i < num; i++) {
		
		glBegin(GL_LINES);
		for (int i = 0; i < numVertices; ++i) {

			angle = (i * 2 * PI / (numVertices));

			x = cos(angle) * r;
			y = sin(angle) * r;


			glColor3d(0, 0, 0);
			Vect
				vertex1 = route({ x + w, y + w }, angle, x, y),
				vertex2 = route({ x - w, y + w }, angle, x, y),
				vertex3 = route({ x - w, y - w }, angle, x, y),
				vertex4 = route({ x + w, y - w }, angle, x, y);

			//auto str = std::to_string(vertex2.x);
			//MessageBox(hwndMain, str.c_str(), "", S_OK);

			glVertex2d(vertex4.x, vertex4.y);
			glVertex2d(vertex3.x, vertex3.y);

			glVertex2d(vertex2.x, vertex2.y);
			glVertex2d(vertex1.x, vertex1.y);
			


		}
		glEnd();
		r -= incrR;
		w -= incrW;
	}
	//����� �������
	
	r = R;  w = W;
	for (int i = 0; i < num; i++) {
		
		glBegin(GL_LINE_LOOP);
		for (int i = 0; i < numVertices; ++i) {

			angle = (i * 2 * PI / (numVertices));

			x = cos(angle) * r;
			y = sin(angle) * r;


			glColor3d(0, 0, 0);
			Vect
				vertex1 = route({ x + w, y + w }, angle, x, y),
				vertex2 = route({ x - w, y + w }, angle, x, y),
				vertex3 = route({ x - w, y - w }, angle, x, y),
				vertex4 = route({ x + w, y - w }, angle, x, y);

			//auto str = std::to_string(vertex2.x);
			//MessageBox(hwndMain, str.c_str(), "", S_OK);
			glVertex2d(vertex4.x, vertex4.y);
			glVertex2d(vertex1.x, vertex1.y);
			


		}
		glEnd();
		r -= incrR;
		w -= incrW;
	}
	//����� ��������� 
	
	r = R;  w = W;
	for (int i = 0; i < num; i++) {
		
		glBegin(GL_LINE_LOOP);
		for (int i = 0; i < numVertices; ++i) {

			angle = (i * 2 * PI / (numVertices));

			x = cos(angle) * r;
			y = sin(angle) * r;


			glColor3d(0, 0, 0);
			Vect
				vertex1 = route({ x + w, y + w }, angle, x, y),
				vertex2 = route({ x - w, y + w }, angle, x, y),
				vertex3 = route({ x - w, y - w }, angle, x, y),
				vertex4 = route({ x + w, y - w }, angle, x, y);

			//auto str = std::to_string(vertex2.x);
			//MessageBox(hwndMain, str.c_str(), "", S_OK);
			glVertex2d(vertex3.x, vertex3.y);
			glVertex2d(vertex2.x, vertex2.y);
		}
		glEnd();
		r -= incrR;
		w -= incrW;
	}
	//�����
	r = R;  w = W;
	glPointSize(5.0f);
	for (int i = 0; i < num; i++) {
		
		glBegin(GL_POINTS);
		for (int i = 0; i < numVertices; ++i) {

			angle = (i * 2 * PI / (numVertices));

			x = cos(angle) * r;
			y = sin(angle) * r;


			glColor3d(1, 0, 0);
			
			Vect
				vertex1 = route({ x + w, y + w }, angle, x, y),
				vertex2 = route({ x - w, y + w }, angle, x, y),
				vertex3 = route({ x - w, y - w }, angle, x, y),
				vertex4 = route({ x + w, y - w }, angle, x, y);

			//auto str = std::to_string(vertex2.x);
			//MessageBox(hwndMain, str.c_str(), "", S_OK);

			glVertex2d(vertex1.x, vertex1.y);
			glVertex2d(vertex2.x, vertex2.y);
			glVertex2d(vertex3.x, vertex3.y);
			glVertex2d(vertex4.x, vertex4.y);


		}
		glEnd();
		r -= incrR;
		w -= incrW;
	}
	

	glFinish();

	SwapBuffers(hdcMain);
} 

// ������� ��� ��������� n ��������������� � �������� radius � ������� � ����� (x, y)
void DrawRectangles(double x, double y, double radius, int n)
{
	// �������� ��������� �����
	glBegin(GL_LINE_LOOP);

	// ��������� ��������� �����
	glEnd();
}
// ��������� �������� ����
LRESULT WINAPI MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {
	case WM_SIZE:
		InitViewport(LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_PAINT:
		MainOnPaint(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0L;
}  // func MainWndProc

 // ���������� WM_PAINT
void MainOnPaint(HWND hwnd)
{
	PAINTSTRUCT ps;
	BeginPaint(hwnd, &ps);

	DrawModel(8,3);
	// ������ ��������������


	EndPaint(hwnd, &ps);
}  //func MainOnPaint

 // ����������� ������� ����
int RegisterWndClasses()
{
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(wc));
	wc.hInstance = hiApp;
	wc.style = CS_VREDRAW | CS_HREDRAW | CS_OWNDC;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpfnWndProc = (WNDPROC)MainWndProc;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	// wc.hbrBackground = CreateSolidBrush(GetSysColor(COLOR_3DFACE));
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	//wc.lpszMenuName  = MAKEINTRESOURCE(IDR_MENU);
	wc.lpszClassName = pczMainWndClass;
	if (!RegisterClassEx(&wc))
		return 0;
	return 1;
}  // func RegisterWndClasses

 // ������������� ����������
void InitializeApp()
{
	// ���������������� ����� � ������� ������� ����
	RegisterWndClasses();
	hwndMain = CreateWindow(pczMainWndClass, NULL,
		WS_CAPTION | WS_VISIBLE | WS_SYSMENU | WS_THICKFRAME |
		WS_OVERLAPPED | WS_MAXIMIZEBOX | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
		0, 0, 800, 600, NULL,
		NULL, hiApp, NULL);
	if (!hwndMain)
		return;
	// ���������������� OpenGL
	InitOpenGL();
	// ������� ����� � ���� ��� ������� ������� ������
	SelectObject(hdcMain, GetStockObject(NULL_BRUSH));
	SelectObject(hdcMain, GetStockObject(WHITE_PEN));
	//
	RECT r;
	GetClientRect(hwndMain, &r);
	InitViewport(r.right, r.bottom);
}  //func InitializeApp

 // ������������ �������� ����������
void UninitializeApp()
{
	UnregisterClass(pczMainWndClass, hiApp);
}  //func UninitializeApp

 // ����� ����� ����������
int APIENTRY WinMain(HINSTANCE hi, HINSTANCE hi_void, LPSTR pc_cl, int wsm)
{
	hiApp = hi;
	InitializeApp();
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
		DispatchMessage(&msg);
	UninitializeApp();
	return 0;
}  //func WinMain


Vect route(Vect mass,double ang,double x,double y){
	//��������� � ���� ������ ��� ��� �� �� ���
	Vect retur;

	double x1_relative = mass.x - x;
	double 	y1_relative = mass.y - y;
	
	double x2_relative = (x1_relative * cos(ang)) - (y1_relative * sin(ang));
	double y2_relative = (x1_relative * sin(ang)) + (y1_relative * cos(ang));

	retur.x = x2_relative + x;
	retur.y = y2_relative + y;

	 return retur;
}