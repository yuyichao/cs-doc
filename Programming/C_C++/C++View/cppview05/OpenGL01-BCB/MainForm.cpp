//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <gl/gl.h> 
#include <gl/glu.h> 
#include <float.h>

#include "MainForm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
    : TForm(Owner)
{
    Application->OnIdle = IdleLoop;
    _control87(MCW_EM, MCW_EM);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::IdleLoop(TObject*, bool& done)
{
     done = false;
     wglMakeCurrent(m_hDC, m_hRC);
     RenderGLScene();
     SwapBuffers(m_hDC);
     wglMakeCurrent(m_hDC, NULL);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormCreate(TObject *Sender)
{
    SetWindowStats(640, 480, 16);

    m_hDC = GetDC(Handle);
    SetPixelFormatDescriptor();
    m_hRC = wglCreateContext(m_hDC);
    wglMakeCurrent(m_hDC, m_hRC);

    // Necessary to set up before InitGL
    FormResize(NULL);

    InitGL();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormDestroy(TObject *Sender)
{
    if(BorderStyle == bsNone)
  		ChangeDisplaySettings(NULL,0);

    ReleaseDC(Handle, m_hDC);
    wglMakeCurrent(m_hDC, NULL);
    wglDeleteContext(m_hRC);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::SetPixelFormatDescriptor()
{
    PIXELFORMATDESCRIPTOR pfd = {
        sizeof(PIXELFORMATDESCRIPTOR),
        1,
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
        PFD_TYPE_RGBA,
        24,           // Color buffer
        0,0,0,0,0,0,
        0,0,
        0,0,0,0,0,
        32,           // Depth buffer
        0,            // Stencil buffer
        0,
        PFD_MAIN_PLANE,
        0,
        0,0,0
        };
    m_iPixelFormat = ChoosePixelFormat(m_hDC, &pfd);
    SetPixelFormat(m_hDC, m_iPixelFormat, &pfd);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormResize(TObject *Sender)
{
   wglMakeCurrent(m_hDC, m_hRC);

   glViewport(0, 0, ClientWidth, ClientHeight);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();

	 // Calculate The Aspect Ratio Of The Window
	 gluPerspective(45.0, static_cast<double>(ClientWidth) /
                        static_cast<double>(ClientHeight),
                  0.1,100.0);

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   wglMakeCurrent(m_hDC, NULL);
}
//---------------------------------------------------------------------------
bool __fastcall TForm1::SetWindowStats(int iWidth_, int iHeight_, int iColor_)
{
    ClientWidth = iWidth_;
    ClientHeight = iHeight_;

    // Check for full screen
    if(Application->MessageBox("Would You Like To Run In Fullscreen Mode?",
                               "Start FullScreen?",
                               MB_YESNO|MB_ICONQUESTION)==IDYES)
    {
      // Switch screen resolution to the same as the form
      DEVMODE dmScreenSettings;								// Device Mode
		  memset(&dmScreenSettings,0,sizeof(dmScreenSettings));	// Makes Sure Memory's Cleared
		  dmScreenSettings.dmSize=sizeof(dmScreenSettings);		// Size Of The Devmode Structure
		  dmScreenSettings.dmPelsWidth	= iWidth_;			    // Selected Screen Width
		  dmScreenSettings.dmPelsHeight	= iHeight_;				  // Selected Screen Height
		  dmScreenSettings.dmBitsPerPel	= iColor_;  	   	  // Selected Bits Per Pixel
		  dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

  		// Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
	  	if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
		  {
        return false;
		  }
      else
      {
        BorderStyle = bsNone;
        Left = 0;
        Top = 0;
      }
    }

    return true;
}
//---------------------------------------------------------------------------

bool __fastcall TForm1::InitGL()
{
  wglMakeCurrent(m_hDC, m_hRC);
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);	// Black Background
  wglMakeCurrent(m_hDC, NULL);
	return TRUE;										      // Initialization Went OK
}
//---------------------------------------------------------------------------
void __fastcall TForm1::RenderGLScene()
{
	glClear(GL_COLOR_BUFFER_BIT);	   // Clear Screen Buffer
	glLoadIdentity();								 // Reset The Current Modelview Matrix
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormKeyPress(TObject *Sender, char &Key)
{
    if(Key == VK_ESCAPE)
        Close();
}
//---------------------------------------------------------------------------

