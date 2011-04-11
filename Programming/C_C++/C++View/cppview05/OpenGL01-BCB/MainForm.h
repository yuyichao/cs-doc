//---------------------------------------------------------------------------

#ifndef MainFormH
#define MainFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall FormResize(TObject *Sender);
  void __fastcall FormKeyPress(TObject *Sender, char &Key);
private:	// User declarations
    HDC m_hDC;
    HGLRC m_hRC;
    int m_iPixelFormat;

    bool __fastcall InitGL();
    bool __fastcall SetWindowStats(int iWidth_, int iHeight_, int iColor);
public:		// User declarations
  __fastcall TForm1(TComponent* Owner);
    void __fastcall IdleLoop(TObject*, bool&);
    void __fastcall RenderGLScene();
    void __fastcall SetPixelFormatDescriptor();
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
