/*****************************************************************************
*           Change Log
*  Date     | Change
*-----------+-----------------------------------------------------------------
* 17-Apr-07 | Created
* 13-Dec-09 | Created change log
* 13-Dec-09 | REQ #001: Fixed excess-1024 to be excess-1023
*           | REQ #002: Added hex exponent control
*           | REQ #003: Added ability to use #IND
*           | REQ #004: Added nybble marks
*           | REQ #005: Do not allow other than hex characters in hex controls
*           | REQ #006: Show x87 FPU status register
*****************************************************************************/
// fpDlg.cpp : implementation file
//

#include "stdafx.h"
#include "fp.h"
#include "fpDlg.h"
#include ".\fpdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifndef _tcscpy_s
#include <StrSafe.h>
#define _tcscpy_s(dest, len, src) StringCchCopy(dest, len, src)
#endif

#ifndef _countof
#define _countof(x) (sizeof(x) / sizeof((x)[0]))
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
        CAboutDlg();

// Dialog Data
        enum { IDD = IDD_ABOUTBOX };

        protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
        DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
        CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CfpDlg dialog

/****************************************************************************
*                               Masks for FP components
****************************************************************************/

const ULONGLONG SignMask     = 0x8000000000000000;
const ULONGLONG ExponentMask = 0x7FF0000000000000;
const ULONGLONG MantissaMask = 0x000FFFFFFFFFFFFF;
const ULONGLONG SignShift    = 63;
const ULONGLONG ExponentShift= 52;

/****************************************************************************
*                         Masks for Status Word Bits
****************************************************************************/

static const WORD TopShift = 11;                                         // REQ #007
static const WORD IEmask = 1;                                       // REQ #007
static const WORD DEmask = (1 << 1);                                // REQ #007
static const WORD ZEmask = (1 << 2);                                // REQ #007
static const WORD OEmask = (1 << 3);                                // REQ #007
static const WORD UEmask = (1 << 4);                                // REQ #007
static const WORD PEmask = (1 << 5);                                // REQ #007
static const WORD SFmask = (1 << 6);                                // REQ #007
static const WORD ESmask = (1 << 7);                                // REQ #007
static const WORD C0mask = (1 << 8);                                // REQ #007
static const WORD C1mask = (1 << 9);                                // REQ #007
static const WORD C2mask = (1 << 10);                               // REQ #007
static const WORD TOPmask = (7 << TopShift);                        // REQ #007
static const WORD C3mask = (1 << 14);                               // REQ #007
static const WORD Bmask = (1 << 15);                                // REQ #007


/****************************************************************************
*                               CfpDlg::CfpDlg
****************************************************************************/

CfpDlg::CfpDlg(CWnd* pParent /*=NULL*/)
        : CDialog(CfpDlg::IDD, pParent)
   {
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    initialized = FALSE;
   }


/****************************************************************************
*                            CfpDlg::DoDataExchange
* Inputs:
*       CDataExchange * pDX:
* Result: void
*       
* Effect: 
*       Binds controls to variables
****************************************************************************/

void CfpDlg::DoDataExchange(CDataExchange* pDX)
   {
   CDialog::DoDataExchange(pDX);

   DDX_Control(pDX, IDC_STATUS_COMMENT, c_StatusComment);           // REQ #007
   DDX_Control(pDX, IDC_CODE, c_Code);                              // REQ #007
   DDX_Control(pDX, IDC_NYBBLEFSR01, c_NybbleFSR01);                // REQ #006
   DDX_Control(pDX, IDC_NYBBLEFSR12, c_NybbleFSR12);                // REQ #006
   DDX_Control(pDX, IDC_NYBBLEFSR23, c_NybbleFSR23);                // REQ #006
   DDX_Control(pDX, IDC_TOP, c_Top);                                // REQ #006

   DDX_Control(pDX, IDC_NYBBLE01, c_Nybble01);                      // REQ #004
   DDX_Control(pDX, IDC_NYBBLE12, c_Nybble12);                      // REQ #004
   DDX_Control(pDX, IDC_NYBBLE23, c_Nybble23);                      // REQ #004
   DDX_Control(pDX, IDC_NYBBLE34, c_Nybble34);                      // REQ #004
   DDX_Control(pDX, IDC_NYBBLE45, c_Nybble45);                      // REQ #004
   DDX_Control(pDX, IDC_NYBBLE56, c_Nybble56);                      // REQ #004
   DDX_Control(pDX, IDC_NYBBLE67, c_Nybble67);                      // REQ #004
   DDX_Control(pDX, IDC_NYBBLE78, c_Nybble78);                      // REQ #004
   DDX_Control(pDX, IDC_NYBBLE89, c_Nybble89);                      // REQ #004
   DDX_Control(pDX, IDC_NYBBLE910, c_Nybble910);                    // REQ #004
   DDX_Control(pDX, IDC_NYBBLE1011, c_Nybble1011);                  // REQ #004
   DDX_Control(pDX, IDC_NYBBLE1112, c_Nybble1112);                  // REQ #004
   DDX_Control(pDX, IDC_NYBBLE1314, c_Nybble1314);                  // REQ #004
   DDX_Control(pDX, IDC_NYBBLE1415, c_Nybble1415);                  // REQ #004

   DDX_Control(pDX, IDC_HEX_EXPONENT, c_HexExponent);
   DDX_Control(pDX, IDC_HEX_MANTISSA, c_HexMantissa);
   DDX_Control(pDX, IDC_NUMBER, c_Number);
   DDX_Control(pDX, IDC_FORMAT_N, c_FormatN);
   DDX_Control(pDX, IDC_FORMAT_1, c_Format1);
   DDX_Control(pDX, IDC_FORMAT_2, c_Format2);
   DDX_Control(pDX, IDC_FORMAT_3, c_Format3);
   DDX_Control(pDX, IDC_BINARY_EXPONENT, c_BinaryExponent);
   DDX_Control(pDX, IDC_SPIN_BINARY_EXPONENT, c_SpinBinaryExponent);
   DDX_Control(pDX, IDC_EXCESS1023_EXPONENT, c_Excess1023Exponent);
   DDX_Control(pDX, IDC_SPIN_EXCESS_1023_EXPONENT, c_SpinExcess1023Exponent);
   DDX_Control(pDX, IDC_NUMBER1, c_Number1);
   DDX_Control(pDX, IDC_NUMBER2, c_Number2);
   DDX_Control(pDX, IDC_NUMBER3, c_Number3);

   DDX_Control(pDX, IDC_FORMAT, c_Format);

   DDX_Control(pDX, IDC_SET_FORMAT_N, c_SetFormatN);
   DDX_Control(pDX, IDC_SET_FORMAT_1, c_SetFormat1);
   DDX_Control(pDX, IDC_SET_FORMAT_2, c_SetFormat2);
   DDX_Control(pDX, IDC_SET_FORMAT_3, c_SetFormat3);

   // flags
   DDX_Control(pDX, IDC_FORMAT_MINUS, c_FormatMinus);
   DDX_Control(pDX, IDC_FORMAT_PLUS, c_FormatPlus);
   DDX_Control(pDX, IDC_FORMAT_ZERO, c_FormatZero);
   DDX_Control(pDX, IDC_FORMAT_HASH, c_FormatHash);
   DDX_Control(pDX, IDC_FORMAT_BLANK, c_FormatBlank);

   // width
   DDX_Control(pDX, IDC_WIDTH_NONE, c_WidthNone);
   DDX_Control(pDX, IDC_USE_WIDTH, c_UseWidth);
   DDX_Control(pDX, IDC_WIDTH, c_Width);
   DDX_Control(pDX, IDC_SPIN_WIDTH, c_SpinWidth);

   // precision
   DDX_Control(pDX, IDC_PRECISION_NONE, c_PrecisionNone);
   DDX_Control(pDX, IDC_USE_PRECISION, c_UsePrecision);
   DDX_Control(pDX, IDC_PRECISION, c_Precision);
   DDX_Control(pDX, IDC_SPIN_PRECISION, c_SpinPrecision);
   DDX_Control(pDX, IDC_DOT, c_Dot);

   // type
   DDX_Control(pDX, IDC_TYPE_LOWER_E, c_Type_e);
   DDX_Control(pDX, IDC_TYPE_UPPER_E, c_Type_E);
   DDX_Control(pDX, IDC_TYPE_F, c_Type_f);
   DDX_Control(pDX, IDC_TYPE_LOWER_G, c_Type_g);
   DDX_Control(pDX, IDC_TYPE_UPPER_G, c_Type_G);

   // bits <=> values
   DDX_Control(pDX, IDC_TO_1, c_NumberTo1);
   DDX_Control(pDX, IDC_TO_2, c_NumberTo2);
   DDX_Control(pDX, IDC_TO_3, c_NumberTo3);

   DDX_Control(pDX, IDC_FROM_1, c_1ToNumber);
   DDX_Control(pDX, IDC_FROM_2, c_2ToNumber);
   DDX_Control(pDX, IDC_FROM_3, c_3ToNumber);

   DDX_Control(pDX, IDC_SHOW_SPACES, c_ShowSpaces);

   DDX_Control(pDX, IDC_1_TO_2, c_1To2);
   DDX_Control(pDX, IDC_1_TO_3, c_1To3);
   DDX_Control(pDX, IDC_2_TO_1, c_2To1);
   DDX_Control(pDX, IDC_2_TO_3, c_2To3);
   DDX_Control(pDX, IDC_3_TO_1, c_3To1);
   DDX_Control(pDX, IDC_3_TO_2, c_3To2);

   DDX_Control(pDX, IDC_SHOW_NYBBLES, c_ShowNybbles);
   }

/****************************************************************************
*                                 Message Map
****************************************************************************/

BEGIN_MESSAGE_MAP(CfpDlg, CDialog)
        ON_WM_SYSCOMMAND()
        ON_CONTROL_RANGE(BN_CLICKED, IDC_00, IDC_63, OnBitChanged)
        ON_BN_CLICKED(IDC_SET_ALL_EXPONENT, OnSetAllExponent)
        ON_BN_CLICKED(IDC_CLEAR_ALL_EXPONENT, OnClearAllExponent)
        ON_BN_CLICKED(IDC_ZERO_EXPONENT, OnZeroExponent)
        ON_BN_CLICKED(IDC_SET_ALL_MANTISSA, OnSetAllMantissa)
        ON_BN_CLICKED(IDC_CLEAR_ALL_MANTISSA, OnClearAllMantissa)
        ON_BN_CLICKED(IDC_PLUS_INFINITY, OnBnClickedInfPlus)
        ON_BN_CLICKED(IDC_MINUS_INFINITY, OnBnClickedInfMinus)
        ON_BN_CLICKED(IDC_NAN_PLUS, OnBnClickedNaNPlus)
        ON_BN_CLICKED(IDC_NAN_MINUS, OnBnClickedNaNMinus)
        ON_BN_CLICKED(IDC_TYPE_UPPER_E, OnUpdateRequest)
        ON_BN_CLICKED(IDC_TYPE_LOWER_E, OnUpdateRequest)
        ON_BN_CLICKED(IDC_TYPE_UPPER_G, OnUpdateRequest)
        ON_BN_CLICKED(IDC_TYPE_LOWER_G, OnUpdateRequest)
        ON_BN_CLICKED(IDC_TYPE_F, OnUpdateRequest)
        ON_BN_CLICKED(IDC_PRECISION_NONE, OnUpdateRequest)
        ON_BN_CLICKED(IDC_USE_PRECISION, OnUpdateRequest)
        ON_BN_CLICKED(IDC_WIDTH_NONE, OnUpdateRequest)
        ON_BN_CLICKED(IDC_USE_WIDTH, OnUpdateRequest)
        ON_BN_CLICKED(IDC_FORMAT_MINUS, OnUpdateRequest)
        ON_BN_CLICKED(IDC_FORMAT_PLUS, OnUpdateRequest)
        ON_BN_CLICKED(IDC_FORMAT_ZERO, OnUpdateRequest)
        ON_BN_CLICKED(IDC_FORMAT_HASH, OnUpdateRequest)
        ON_EN_CHANGE(IDC_WIDTH, OnUpdateRequest)
        ON_EN_CHANGE(IDC_NUMBER, OnChangeNumber)
        ON_EN_CHANGE(IDC_PRECISION, OnUpdateRequest)
        ON_EN_CHANGE(IDC_BINARY_EXPONENT, OnChangeBinaryExponent)
        ON_EN_CHANGE(IDC_NUMBER1,OnChangeNumber1)
        ON_EN_CHANGE(IDC_NUMBER2,OnChangeNumber2)
        ON_EN_CHANGE(IDC_NUMBER3,OnChangeNumber3)
        ON_EN_CHANGE(IDC_EXCESS1023_EXPONENT, OnChangeExcess1023Exponent)
        ON_EN_CHANGE(IDC_HEX_MANTISSA, OnChangeHexMantissa)
        ON_BN_CLICKED(IDC_SET_FORMAT_N, OnBnClickedSetFormatNumberN)
        ON_BN_CLICKED(IDC_SET_FORMAT_1, OnBnClickedSetFormat1)
        ON_BN_CLICKED(IDC_SET_FORMAT_2, OnBnClickedSetFormat2)
        ON_BN_CLICKED(IDC_SET_FORMAT_3, OnBnClickedSetFormat3)
        ON_BN_CLICKED(IDC_SHOW_SPACES, OnBnClickedShowSpaces)
        ON_BN_CLICKED(IDC_1_TO_2, OnBnClicked1To2)
        ON_BN_CLICKED(IDC_1_TO_3, OnBnClicked1To3)
        ON_BN_CLICKED(IDC_2_TO_1, OnBnClicked2To1)
        ON_BN_CLICKED(IDC_2_TO_3, OnBnClicked2To3)
        ON_BN_CLICKED(IDC_3_TO_1, OnBnClicked3To1)
        ON_BN_CLICKED(IDC_3_TO_2, OnBnClicked3To2)
        ON_BN_CLICKED(IDC_TO_1, OnBnClickedNumberTo1)
        ON_BN_CLICKED(IDC_TO_2, OnBnClickedNumberTo2)
        ON_BN_CLICKED(IDC_TO_3, OnBnClickedNumberTo3)
        ON_BN_CLICKED(IDC_ADD, OnBnClickedAdd)
        ON_BN_CLICKED(IDC_SUBTRACT, OnBnClickedSubtract)
        ON_BN_CLICKED(IDC_MULTIPLY, OnBnClickedMultiply)
        ON_BN_CLICKED(IDC_DIVIDE, OnBnClickedDivide)
        ON_BN_CLICKED(IDC_FROM_1, OnBnClickedFrom1)
        ON_BN_CLICKED(IDC_FROM_2, OnBnClickedFrom2)
        ON_BN_CLICKED(IDC_FROM_3, OnBnClickedFrom3)
        ON_WM_CLOSE()
        ON_EN_CHANGE(IDC_HEX_EXPONENT, OnEnChangeHexExponent)       // REQ #002
        ON_BN_CLICKED(IDC_IND, OnBnClickedInd)
        ON_BN_CLICKED(IDC_SHOW_NYBBLES, OnBnClickedShowNybbles)     // REQ #004
        ON_BN_CLICKED(IDC_CLEAR87, OnBnClickedClear87)              // REQ #006
        ON_BN_CLICKED(IDC_F2XM1, OnBnClickedF2xm1)                  // REQ #007
        ON_BN_CLICKED(IDC_FCOM, OnBnClickedFcom)                    // REQ #007
        ON_BN_CLICKED(IDC_FCHS, OnBnClickedFchs)                    // REQ #007
        ON_BN_CLICKED(IDC_FABS, OnBnClickedFabs)
        ON_BN_CLICKED(IDC_FLD1, OnBnClickedFld1)
        ON_BN_CLICKED(IDC_FLDL2T, OnBnClickedFldl2t)
        ON_BN_CLICKED(IDC_FLDL2E, OnBnClickedFldl2e)
        ON_BN_CLICKED(IDC_FLDPI, OnBnClickedFldpi)
        ON_BN_CLICKED(IDC_FLDLG2, OnBnClickedFldlg2)
        ON_BN_CLICKED(IDC_FLDLN2, OnBnClickedFldln2)
        ON_BN_CLICKED(IDC_FLDZ, OnBnClickedFldz)
        ON_BN_CLICKED(IDC_FPATAN, OnBnClickedFpatan)
        ON_BN_CLICKED(IDC_FPREM, OnBnClickedFprem)
        ON_BN_CLICKED(IDC_FPREM1, OnBnClickedFprem1)
        ON_BN_CLICKED(IDC_FPTAN, OnBnClickedFptan)
        ON_BN_CLICKED(IDC_FRNDINT, OnBnClickedFrndint)
        ON_BN_CLICKED(IDC_FSCALE, OnBnClickedFscale)
        ON_BN_CLICKED(IDC_FSIN, OnBnClickedFsin)
        ON_BN_CLICKED(IDC_FSINCOS, OnBnClickedFsincos)
        ON_BN_CLICKED(IDC_FSQRT, OnBnClickedFsqrt)
        ON_BN_CLICKED(IDC_FXAM, OnBnClickedFxam)
        ON_BN_CLICKED(IDC_FXTRACT, OnBnClickedFxtract)
        ON_BN_CLICKED(IDC_FYL2X, OnBnClickedFyl2x)
        ON_BN_CLICKED(IDC_FYL2XP1, OnBnClickedFyl2xp1)
        ON_BN_CLICKED(IDC_FCOS, OnBnClickedFcos)
END_MESSAGE_MAP()

// CfpDlg message handlers

/****************************************************************************
*                             CfpDlg::OnInitDialog
* Result: BOOL
*       
* Effect: 
*       Initializes the dialog
****************************************************************************/

BOOL CfpDlg::OnInitDialog()
{
        CDialog::OnInitDialog();

        // Add "About..." menu item to system menu.

        // IDM_ABOUTBOX must be in the system command range.
        ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
        ASSERT(IDM_ABOUTBOX < 0xF000);

        CMenu* pSysMenu = GetSystemMenu(FALSE);
        if (pSysMenu != NULL)
        {
                CString strAboutMenu;
                strAboutMenu.LoadString(IDS_ABOUTBOX);
                if (!strAboutMenu.IsEmpty())
                {
                        pSysMenu->AppendMenu(MF_SEPARATOR);
                        pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
                }
        }

        // Set the icon for this dialog.  The framework does this automatically
        //  when the application's main window is not a dialog
        SetIcon(m_hIcon, TRUE);                 // Set big icon
        SetIcon(m_hIcon, FALSE);                // Set small icon

        // TODO: Add extra initialization here
        
        c_SpinPrecision.SetRange(1,24);
        c_SpinPrecision.SetPos(1);
        c_SpinWidth.SetRange(1,24);
        c_SpinWidth.SetPos(1);

        c_SpinBinaryExponent.SetRange(0, 2047);
        c_SpinBinaryExponent.SetPos(1023);

        c_SpinExcess1023Exponent.SetRange(-1024, 1023);
        c_SpinExcess1023Exponent.SetPos(0);

        CheckRadioButton(IDC_WIDTH_NONE, IDC_USE_WIDTH, IDC_WIDTH_NONE);
        CheckRadioButton(IDC_PRECISION_NONE, IDC_USE_PRECISION, IDC_PRECISION_NONE);
        CheckRadioButton(IDC_TYPE_LOWER_E, IDC_TYPE_UPPER_G, IDC_TYPE_F);

        c_FormatN.SetWindowText(_T("%f"));
        c_SetFormatN.SetDirection(DrawArrow::ARROW_N);

        c_Format1.SetWindowText(_T("%f"));
        c_SetFormat1.SetDirection(DrawArrow::ARROW_N);

        c_Format2.SetWindowText(_T("%f"));
        c_SetFormat2.SetDirection(DrawArrow::ARROW_N);

        c_Format3.SetWindowText(_T("%f"));
        c_SetFormat3.SetDirection(DrawArrow::ARROW_N);

        c_NumberTo1.SetDirection(DrawArrow::ARROW_S);
        c_NumberTo2.SetDirection(DrawArrow::ARROW_S);
        c_NumberTo3.SetDirection(DrawArrow::ARROW_S);
        c_1ToNumber.SetDirection(DrawArrow::ARROW_N);
        c_2ToNumber.SetDirection(DrawArrow::ARROW_N);
        c_3ToNumber.SetDirection(DrawArrow::ARROW_N);

        c_1To2.SetDirection(DrawArrow::ARROW_E);
        c_1To3.SetDirection(DrawArrow::ARROW_E);
        c_2To3.SetDirection(DrawArrow::ARROW_E);
        c_2To1.SetDirection(DrawArrow::ARROW_W);
        c_3To1.SetDirection(DrawArrow::ARROW_W);
        c_3To2.SetDirection(DrawArrow::ARROW_W);

        // initialize the special numbers
        NaNPlus.n  = 0x7FF0000000000001;                                    
        NaNMinus.n = 0xFFF0000000000001;
        InfPlus.n  = 0x7FF0000000000000;
        InfMinus.n = 0xFFF0000000000000;
        IndMinus.n = 0xFFF8000000000000;

        CFont * f = GetFont();                                      // REQ #007
        LOGFONT lf;                                                 // REQ #007
        f->GetLogFont(&lf);                                         // REQ #007
        _tcscpy_s(lf.lfFaceName, _countof(lf.lfFaceName), _T("Courier New"));// REQ #007
        FixedFont.CreateFontIndirect(&lf);                          // REQ #007
        c_Code.SetFont(&FixedFont);                                 // REQ #007

        initialized = TRUE;
        updateControls();

        return TRUE;  // return TRUE  unless you set the focus to a control
}

/****************************************************************************
*                             CfpDlg::OnSysCommand
* Inputs:
*       UINT nID:
*       LPARAM lParam:
* Result: void
*       
* Effect: 
*       Handles system commands, particularly IDM_ABOUTBOX
****************************************************************************/

void CfpDlg::OnSysCommand(UINT nID, LPARAM lParam)
   {
    if ((nID & 0xFFF0) == IDM_ABOUTBOX)
       {
        CAboutDlg dlgAbout;
        dlgAbout.DoModal();
       }
    else
       {
        CDialog::OnSysCommand(nID, lParam);
       }
   }

/****************************************************************************
*                                CfpDlg::OnOK
* Result: void
*       
* Effect: 
*       Does nothing; this prevents closing the dialog with <enter>
****************************************************************************/

void CfpDlg::OnOK()
    {
    }

/****************************************************************************
*                              CfpDlg::OnCancel
* Result: void
*       
* Effect: 
*       Does nothing; this prevents closing the app with <esc>
****************************************************************************/

void CfpDlg::OnCancel()
    {
    }

/****************************************************************************
*                               CfpDlg::OnClose
* Result: void
*       
* Effect: 
*       Closes the app
****************************************************************************/

void CfpDlg::OnClose()
    {
     CDialog::OnOK();
    }

/****************************************************************************
*                            CfpDlg::OnBitChanged
* Inputs:
*       UINT id: ID of bit that changed
* Result: void
*       
* Effect: 
*       Converts the binary value to a floating point word
****************************************************************************/

void CfpDlg::OnBitChanged(UINT id)
    {
     BitsToFP();
     updateControls();
    } // CfpDlg::OnBitChanged

/****************************************************************************
*                           CfpDlg::updateControls
* Result: void
*       
* Effect: 
*       Updates the state of the controls
****************************************************************************/

void CfpDlg::updateControls()
    {
     if(!initialized)
        return;
     
     CString format = _T("%");

     if(c_FormatMinus.GetCheck() == BST_CHECKED)
        format += _T("-");
     if(c_FormatPlus.GetCheck() == BST_CHECKED)
        format += _T("+");
     if(c_FormatZero.GetCheck() == BST_CHECKED)
        format += _T("0");
     if(c_FormatHash.GetCheck() == BST_CHECKED)
        format += _T("#");
     if(c_FormatBlank.GetCheck() == BST_CHECKED)
        format += _T(" ");
     
     if(c_UseWidth.GetCheck() == BST_CHECKED)
        { /* has width */
         int n = c_SpinWidth.GetPos32();
         CString s;
         s.Format(_T("%d"), n);
         format += s;
        } /* has width */

     if(c_UsePrecision.GetCheck() == BST_CHECKED)
        { /* has precision */
         c_Dot.ShowWindow(SW_SHOW);
         int n = c_SpinPrecision.GetPos32();
         CString s;
         s.Format(_T(".%d"), n);
         format += s;
        } /* has precision */
     else
        { /* no precision */
         c_Dot.ShowWindow(SW_HIDE);
        } /* no precision */

     if(c_Type_e.GetCheck() == BST_CHECKED)
        format += _T("e");
     if(c_Type_E.GetCheck() == BST_CHECKED)
              format += _T("E");
     if(c_Type_f.GetCheck() == BST_CHECKED)
        format += _T("f");
     if(c_Type_g.GetCheck() == BST_CHECKED)
        format += _T("g");
     if(c_Type_G.GetCheck() == BST_CHECKED)
        format += _T("G");

     c_Format.SetWindowText(format);
     
     c_Precision.EnableWindow(c_UsePrecision.GetCheck() == BST_CHECKED);
     c_SpinPrecision.EnableWindow(c_UsePrecision.GetCheck() == BST_CHECKED);

     c_Width.EnableWindow(c_UseWidth.GetCheck() == BST_CHECKED);
     c_SpinWidth.EnableWindow(c_UseWidth.GetCheck() == BST_CHECKED);
        

     // Show the various numbers

     if(GetFocus() != &c_Number)
        ShowNumber(number, c_FormatN, c_Number);

     if(GetFocus() != &c_HexMantissa)
        { /* show hex */
         CString s;
         s.Format(_T("%013I64X"), number.n & MantissaMask);
         c_HexMantissa.SetWindowText(s);
        } /* show hex */

     if(GetFocus() != &c_HexExponent)                               // REQ #002
        { /* show hex exponent */                                   // REQ #002
         CString s;                                                 // REQ #002
         s.Format(_T("%04X"), (number.n & ExponentMask) >> 52);     // REQ #002
         c_HexExponent.SetWindowText(s);                            // REQ #002
        } /* show hex exponent */                                   // REQ #002

     if(GetFocus() != &c_Number1)
        ShowNumber(number1, c_Format1, c_Number1);
     if(GetFocus() != &c_Number2)
        ShowNumber(number2, c_Format2, c_Number2);
     if(GetFocus() != &c_Number3)
        ShowNumber(number3, c_Format3, c_Number3);
     
     if(GetFocus() != &c_BinaryExponent &&
        GetFocus() != &c_SpinBinaryExponent)
        { /* change binary exponent */
         int ex = (int)((number.n & ExponentMask) >> 52);
         CString s;
         s.Format(_T("%d"), ex);
         c_BinaryExponent.SetWindowTextNoNotify(s);
        } /* change binary exponent */

     if(GetFocus() != &c_Excess1023Exponent &&                      // REQ #001
        GetFocus() != &c_SpinExcess1023Exponent)                    // REQ #001
        { /* change excess 1023 exponent */
         int ex = (int)(((number.n & ExponentMask) >> 52) - 1023);  // REQ #001
         CString s;
         s.Format(_T("%d"), ex);
         c_Excess1023Exponent.SetWindowTextNoNotify(s);             // REQ #001
        } /* change excess 1023 exponent */                         // REQ #001

     UINT show = c_ShowNybbles.GetCheck() == BST_CHECKED ? SW_SHOW : SW_HIDE;// REQ #004
     c_Nybble01.ShowWindow(show);                                   // REQ #004
     c_Nybble12.ShowWindow(show);                                   // REQ #004
     c_Nybble23.ShowWindow(show);                                   // REQ #004
     c_Nybble34.ShowWindow(show);                                   // REQ #004
     c_Nybble45.ShowWindow(show);                                   // REQ #004
     c_Nybble56.ShowWindow(show);                                   // REQ #004
     c_Nybble67.ShowWindow(show);                                   // REQ #004
     c_Nybble78.ShowWindow(show);                                   // REQ #004
     c_Nybble89.ShowWindow(show);                                   // REQ #004
     c_Nybble910.ShowWindow(show);                                  // REQ #004
     c_Nybble1011.ShowWindow(show);                                 // REQ #004
     c_Nybble1112.ShowWindow(show);                                 // REQ #004
     c_Nybble1314.ShowWindow(show);                                 // REQ #004
     c_Nybble1415.ShowWindow(show);                                 // REQ #004
     c_NybbleFSR01.ShowWindow(show);                                // REQ #006
     c_NybbleFSR12.ShowWindow(show);                                // REQ #006
     c_NybbleFSR23.ShowWindow(show);                                // REQ #006

     c_StatusComment.SetWindowText(_T(""));                         // REQ #007
    } // CfpDlg::updateControls

/****************************************************************************
*                             CfpDlg::ShowNumber
* Inputs:
*       dint & num:
*       CEdit & fmt: Desired format
*       CNoNotifyEdit & target
* Result: void
*       
* Effect: 
*       Formats num according to fmt and places it in edit
****************************************************************************/

void CfpDlg::ShowNumber(dint & num, CEdit & fmt, CNoNotifyEdit & target)
    {
     CString f;
     fmt.GetWindowText(f);
     ASSERT(!f.IsEmpty());

     CString s;
     s.Format(f, num.d);
     if(c_ShowSpaces.GetCheck() == BST_CHECKED)
        s.Replace(_T(" "), _T("·"));
     target.SetWindowTextNoNotify(s);
    } // CfpDlg::ShowNumber

/****************************************************************************
*                           CfpDlg::OnUpdateRequest
* Result: void
*       
* Effect: 
*       Handles controls that ony want to update
****************************************************************************/

void CfpDlg::OnUpdateRequest()
    {
     updateControls();
    } // CfpDlg::OnUpdateRequest

/****************************************************************************
*                              CfpDlg::BitsToFP
* Result: void
*       
* Effect: 
*       Takes the bits from the controls and puts them into the floating-point
*       dint 'number'
****************************************************************************/

void CfpDlg::BitsToFP()
    {
     ULONGLONG n = 0;
     for(int i = 0; i < 64; i++)
        { /* scan buttons */
         CButton * bit = (CButton*)GetDlgItem(IDC_00 + i);
         n |= (bit->GetCheck() == BST_CHECKED ? ((ULONGLONG)1) << i : 0);
        } /* scan buttons */
     number.n = n;
     ULONGLONG mantissa = n & MantissaMask;
    } // CfpDlg::BitsToFP

/****************************************************************************
*                     CfpDlg::OnBnClickedSetFormatNumberN
* Result: void
*       
* Effect: 
*       Sets the number format for the input number display
****************************************************************************/

void CfpDlg::OnBnClickedSetFormatNumberN()
    {
     CString s;
     c_Format.GetWindowText(s);
     c_FormatN.SetWindowText(s);
     updateControls();
    } // CfpDlg::OnBnClickedSetFormatNumberN

/****************************************************************************
*                        CfpDlg::OnBnClickedSetFormat1
* Result: void
*       
* Effect: 
*       Sets the format1 box
****************************************************************************/

void CfpDlg::OnBnClickedSetFormat1()
    {
     CString s;
     c_Format.GetWindowText(s);
     c_Format1.SetWindowText(s);
     updateControls();
    } // CfpDlg::OnBnClickedSetFormat1

/****************************************************************************
*                        CfpDlg::OnBnClickedSetFormat2
* Result: void
*       
* Effect: 
*       Sets the format1 box
****************************************************************************/

void CfpDlg::OnBnClickedSetFormat2()
    {
     CString s;
     c_Format.GetWindowText(s);
     c_Format2.SetWindowText(s);
     updateControls();
    } // CfpDlg::OnBnClickedSetFormat2

/****************************************************************************
*                        CfpDlg::OnBnClickedSetFormat3
* Result: void
*       
* Effect: 
*       Sets the format3 box
****************************************************************************/

void CfpDlg::OnBnClickedSetFormat3()
    {
     CString s;
     c_Format.GetWindowText(s);
     c_Format3.SetWindowText(s);
     updateControls();
    } // CfpDlg::OnBnClickedSetFormat3

/****************************************************************************
*                           CfpDlg::OnChangeNumber
* Result: void
*       
* Effect: 
*       The number has changed by user interaction; update the binary representation
****************************************************************************/

void CfpDlg::OnChangeNumber()
    {
     CString s;
     c_Number.GetWindowText(s);
     s.Replace(_T("·"), _T(" "));  // in case '¶' button is selected
     double d = _tstof(s);
     number.d = d;
     FPToBits();
     updateControls();
    } // CfpDlg::OnChangeNumber

/****************************************************************************
*                              CfpDlg::FPToBits
* Result: void
*       
* Effect: 
*       Converts the floating point 'number' to bits
****************************************************************************/

void CfpDlg::FPToBits()
    {
     for(int i = 0; i < 64; i++)
        { /* scan buttons */
         CButton * bit = (CButton*)GetDlgItem(IDC_00 + i);
         bit->SetCheck( number.n & ((ULONGLONG)1) << i ? BST_CHECKED : BST_UNCHECKED);
        } /* scan buttons */
    } // CfpDlg::FPToBits

/****************************************************************************
*                         CfpDlg::OnBnClickedNaNPlus
* Result: void
*       
* Effect: 
*       Sets +NaN
****************************************************************************/

void CfpDlg::OnBnClickedNaNPlus()
    {
     number = NaNPlus;
     FPToBits();
     updateControls();
    } // CfpDlg::OnBnClickedNaNPlus

/****************************************************************************
*                         CfpDlg::OnBnClickedNaNMinus
* Result: void
*       
* Effect: 
*       sets -NaN as the number
****************************************************************************/

void CfpDlg::OnBnClickedNaNMinus()
    {
     number = NaNMinus;
     FPToBits();
     updateControls();
    } // CfpDlg::OnBnClickedNaNMinus

/****************************************************************************
*                         CfpDlg::OnBnClickedInfMinus
* Result: void
*       
* Effect: 
*       Sets -Inf to number
****************************************************************************/

void CfpDlg::OnBnClickedInfMinus()
    {
     number = InfMinus;
     FPToBits();
     updateControls();
    } // CfpDlg::OnBnClickedInfMinus

/****************************************************************************
*                         CfpDlg::OnBnClickedInfPlus
* Result: void
*       
* Effect: 
*       Sets +Inf to number
****************************************************************************/

void CfpDlg::OnBnClickedInfPlus()
    {
     number = InfPlus;
     FPToBits();
     updateControls();
    } // CfpDlg::OnBnClickedInfPlus

/****************************************************************************
*                        CfpDlg::OnBnClickedShowSpaces
* Result: void
*       
* Effect: 
*       Toggles the show-space option
****************************************************************************/

void CfpDlg::OnBnClickedShowSpaces()
    {
     updateControls();
    } // CfpDlg::OnBnClickedShowSpaces

/****************************************************************************
*                           CfpDlg::OnBnClicked1To2
* Result: void
*       
* Effect: 
*       1 => 2
****************************************************************************/

void CfpDlg::OnBnClicked1To2()
    {
     number2 = number1;
     ShowStatus();
     updateControls();
    } // CfpDlg::OnBnClicked1To2

/****************************************************************************
*                           CfpDlg::OnBnClicked1To3
* Result: void
*       
* Effect: 
*       number3 = number1
****************************************************************************/

void CfpDlg::OnBnClicked1To3()
    {
     number3 = number1;
     ShowStatus();
     updateControls();
    } // CfpDlg::OnBnClicked1To3

/****************************************************************************
*                           CfpDlg::OnBnClicked2To1
* Result: void
*       
* Effect: 
*       number1 = number2
****************************************************************************/

void CfpDlg::OnBnClicked2To1()
    {
     number1 = number2;
     ShowStatus();
     updateControls();
    } // CfpDlg::OnBnClicked2To1

/****************************************************************************
*                          CfpDlg::OnBnClicked2To3
* Result: void
*       
* Effect: 
*       number3 = number2
****************************************************************************/

void CfpDlg::OnBnClicked2To3()
    {
     number3 = number2;
     ShowStatus();
     updateControls();
    } // CfpDlg::OnBnClicked2To3

/****************************************************************************
*                           CfpDlg::OnBnClicked3To1
* Result: void
*       
* Effect: 
*       number1 = number3
****************************************************************************/

void CfpDlg::OnBnClicked3To1()
    {
     number1 = number3;
     ShowStatus();
     updateControls();
    } // CfpDlg::OnBnClicked3To1

/****************************************************************************
*                           CfpDlg::OnBnClicked3To2
* Result: void
*       
* Effect: 
*       number2 = number3
****************************************************************************/

void CfpDlg::OnBnClicked3To2()
    {
     number2 = number3;
     ShowStatus();
     updateControls();
    } // CfpDlg::OnBnClicked3To2

/****************************************************************************
*                        CfpDlg::OnBnClickedNumberTo1
* Result: void
*       
* Effect: 
*       number1 = number
****************************************************************************/

void CfpDlg::OnBnClickedNumberTo1()
    {
     number1 = number;
     ShowStatus();
     updateControls();
    } // CfpDlg::OnBnClickedNumberTo1

/****************************************************************************
*                        CfpDlg::OnBnClickedNumberTo2
* Result: void
*       
* Effect: 
*       number2 = number
****************************************************************************/

void CfpDlg::OnBnClickedNumberTo2()
    {
     number2 = number;
     ShowStatus();
     updateControls();
    } // CfpDlg::OnBnClickedNumberTo2

/****************************************************************************
*                        CfpDlg::OnBnClickedNumberTo3
* Result: void
*       
* Effect: 
*       number3 = number
****************************************************************************/

void CfpDlg::OnBnClickedNumberTo3()
    {
     number3 = number;
     ShowStatus();
     updateControls();
    } // CfpDlg::OnBnClickedNumberTo3

/****************************************************************************
*                           CfpDlg::OnBnClickedAdd
* Result: void
*       
* Effect: 
*       number3 = number1 + number2
****************************************************************************/

void CfpDlg::OnBnClickedAdd()
    {
     number3.d = number1.d + number2.d;
     ShowStatus();
     updateControls();
    } // CfpDlg::OnBnClickedAdd

/****************************************************************************
*                         CfpDlg::OnBnClickedSubtract
* Result: void
*       
* Effect: 
*       number3 = number1 - number2
****************************************************************************/

void CfpDlg::OnBnClickedSubtract()
    {
     number3.d = number1.d - number2.d;
     ShowStatus();
     updateControls();
    } // CfpDlg::OnBnClickedSubtract

/****************************************************************************
*                         CfpDlg::OnBnClickedMultiply
* Result: void
*       
* Effect: 
*       number3 = number1 * number2
****************************************************************************/

void CfpDlg::OnBnClickedMultiply()
    {
     number3.d = number1.d * number2.d;
     ShowStatus();
     updateControls();
    } // CfpDlg::OnBnClickedMultiply

/****************************************************************************
*                          CfpDlg::OnBnClickedDivide
* Result: void
*       
* Effect: 
*       number3 = number1 / number2
****************************************************************************/

void CfpDlg::OnBnClickedDivide()
    {
     number3.d = number1.d / number2.d;
     ShowStatus();
     updateControls();
    } // CfpDlg::OnBnClickedDivide

/****************************************************************************
*                          CfpDlg::OnChangeHexMantissa
* Result: void
*       
* Effect: 
*       Handles a change in the hex mantissa value
****************************************************************************/

void CfpDlg::OnChangeHexMantissa()
    {
     CString s;
     c_HexMantissa.GetWindowText(s);
     ULONGLONG mantissa = _tcstoui64(s, NULL, 16);
     number.n = (number.n & (SignMask | ExponentMask)) | (mantissa & MantissaMask);
     FPToBits();
     updateControls();
    } // CfpDlg::OnChangeHexMantissa

/****************************************************************************
*                          CfpDlg::OnChangeBinaryExponent
* Result: void
*       
* Effect: 
*       Handles a change in the binary exponent
****************************************************************************/

void CfpDlg::OnChangeBinaryExponent()
    {
     if(!initialized)
         return;
     DWORD n = c_SpinBinaryExponent.GetPos32();
     if(n & ~0x7FF)
        return; // illegal exponent value
     ULONGLONG ex = (ULONGLONG)n << 52;
     number.n = (number.n & ~ExponentMask) | (ex & ExponentMask);
     FPToBits();
     updateControls();
    } // CfpDlg::OnChangeBinaryExponent

/****************************************************************************
*                     CfpDlg::OnChangeExcess1023Exponent            // REQ #001
* Result: void
*       
* Effect: 
*       ¶
****************************************************************************/

void CfpDlg::OnChangeExcess1023Exponent()                           // REQ #001
    {
     if(!initialized)
        return;
     DWORD n = c_SpinExcess1023Exponent.GetPos32();                 // REQ #001
     ULONGLONG ex = (ULONGLONG)(n + 1023) << 52;                    // REQ #001
     number.n = (number.n & ~ExponentMask) | (ex & ExponentMask);
     FPToBits();
     updateControls();
    } // CfpDlg::OnChangeExcess1023Exponent                         // REQ #001


/****************************************************************************
*                           CfpDlg::OnChangeNumber1
* Result: void
*       
* Effect: 
*       The user has typed a value in the number 1 edit control
****************************************************************************/

void CfpDlg::OnChangeNumber1()
    {
     CString s;
     c_Number1.GetWindowText(s);
     double d = _tstof(s);
     number1.d = d;
    } // CfpDlg::OnChangeNumber1

/****************************************************************************
*                           CfpDlg::OnChangeNumber2
* Result: void
*       
* Effect: 
*       The user has typed something into number 2
****************************************************************************/

void CfpDlg::OnChangeNumber2()
    {
     CString s;
     c_Number2.GetWindowText(s);
     double d = _tstof(s);
     number2.d = d;
    } // CfpDlg::OnChangeNumber2

/****************************************************************************
*                           CfpDlg::OnChangeNumber3
* Result: void
*       
* Effect: 
*       The user typed something into number 3
****************************************************************************/

void CfpDlg::OnChangeNumber3()
    {
     CString s;
     c_Number3.GetWindowText(s);
     double d = _tstof(s);
     number3.d = d;
    } // CfpDlg::OnChangeNumber3

/****************************************************************************
*                          CfpDlg::OnBnClickedFrom1
* Result: void
*       
* Effect: 
*       number = number1
****************************************************************************/

void CfpDlg::OnBnClickedFrom1()
    {
     number = number1;
     FPToBits();
     updateControls();
    } // CfpDlg::OnBnClickedFrom1

/****************************************************************************
*                          CfpDlg::OnBnClickedFrom2
* Result: void
*       
* Effect: 
*       number = number2
****************************************************************************/

void CfpDlg::OnBnClickedFrom2()
    {
     number = number2;
     FPToBits();
     updateControls();
    } // CfpDlg::OnBnClickedFrom2

/****************************************************************************
*                          CfpDlg::OnBnClickedFrom3
* Result: void
*       
* Effect: 
*       number = number3
****************************************************************************/

void CfpDlg::OnBnClickedFrom3()
    {
     number = number3;
     FPToBits();
     updateControls();
    } // CfpDlg::OnBnClickedFrom3

/****************************************************************************
*                          CfpDlg::OnSetAllMantissa
* Result: void
*       
* Effect: 
*       Sets all the bits of the mantissa
****************************************************************************/

void CfpDlg::OnSetAllMantissa()
    {
     number.n |= MantissaMask;
     FPToBits();
     updateControls();
    } // CfpDlg::OnSetAllMantissa

/****************************************************************************
*                         CfpDlg::OnClearAllMantissa
* Result: void
*       
* Effect: 
*       Clears all the bits of the mantissa
****************************************************************************/

void CfpDlg::OnClearAllMantissa()
    {
     number.n &= ~MantissaMask;
     FPToBits();
     updateControls();
    } // CfpDlg::OnClearAllMantissa

/****************************************************************************
*                          CfpDlg::OnSetAllExponent
* Result: void
*       
* Effect: 
*       Sets all exponent bits
****************************************************************************/

void CfpDlg::OnSetAllExponent()
    {
     number.n |= ExponentMask;
     FPToBits();
     updateControls();
    } // CfpDlg::OnSetAllExponent

/****************************************************************************
*                         CfpDlg::OnClearAllExponent
* Result: void
*       
* Effect: 
*       Clears all exponent bits
****************************************************************************/

void CfpDlg::OnClearAllExponent()
    {
     number.n &= ~ExponentMask;
     FPToBits();
     updateControls();
    } // CfpDlg::OnClearAllExponent

/****************************************************************************
*                           CfpDlg::OnZeroExponent
* Result: void
*       
* Effect: 
*       Sets the exponent to 2**0
****************************************************************************/

void CfpDlg::OnZeroExponent()
    {
     c_SpinExcess1023Exponent.SetPos(0);                            // REQ #001
     updateControls();
    } // CfpDlg::OnZeroExponent


/****************************************************************************
*                        CfpDlg::OnEnChangeHexExponent
* Result: void
*       
* Effect: 
*       Changes the exponent
****************************************************************************/

void CfpDlg::OnEnChangeHexExponent()                                // REQ #002
    {                                                               // REQ #002
     CString s;                                                     // REQ #002
     c_HexExponent.GetWindowText(s);                                // REQ #002
     ULONGLONG exponent = _tcstoul(s, NULL, 16);                    // REQ #002
     number.n = (number.n & (SignMask | MantissaMask)) | ((exponent << 52) & ExponentMask);// REQ #002
     FPToBits();                                                    // REQ #002
     updateControls();                                              // REQ #002
    }                                                               // REQ #002


/****************************************************************************
*                            CfpDlg::OnBnClickedInd
* Result: void
*       
* Effect: 
*       Sets the value to be -#IND
****************************************************************************/

void CfpDlg::OnBnClickedInd()                                       // REQ #003
    {                                                               // REQ #003
     number = IndMinus;                                             // REQ #003
     FPToBits();                                                    // REQ #003
     updateControls();                                              // REQ #003
    }                                                               // REQ #003


/****************************************************************************
*                        CfpDlg::OnBnClickedShowNybbles
* Result: void
*       
* Effect: 
*       Shows/hides nybble marks
****************************************************************************/

void CfpDlg::OnBnClickedShowNybbles()                               // REQ #004
    {                                                               // REQ #004
     updateControls();                                              // REQ #004
    }                                                               // REQ #004

/****************************************************************************
*                            CfpDlg::FSRtoControls
* Inputs:
*       WORD status: Status word
* Result: void
*       
* Effect: 
*       Transfers the status word to the display
****************************************************************************/

void CfpDlg::FSRtoControls(WORD status)                             // REQ #006
    {                                                               // REQ #006
     for(int i = 0; i < 16; i++)                                    // REQ #006
        { /* transfer */                                            // REQ #006
         CButton * btn = (CButton *)GetDlgItem(IDC_FSR_0 + i);      // REQ #006
         btn->SetCheck(status & (1 << i) ? BST_CHECKED : BST_UNCHECKED);// REQ #006
        } /* transfer */                                            // REQ #006
     WORD tos = ((status & TOPmask) >> TopShift);                   // REQ #006// REQ #007
     CString s;                                                     // REQ #006
     s.Format(_T("%d"), tos);                                       // REQ #006
     c_Top.SetWindowText(s);                                        // REQ #006
    } // CfpDlg::FSRtoControls                                      // REQ #006

/****************************************************************************
*                             CfpDlg::ShowStatus
* Result: void
*       
* Effect: 
*       Shows the FPU status word
* Notes:
*       The _status87() call returns a platform-independent representation
*       of the register (suitable for MIPS, PowerPC, Itanic, etc.)
*       Since this is an x86 application, we want the raw x87 FPU register
*       to display what it shows
*       For the x64, the inline assembler will have to be moved to
*       a separate .asm file because x64 C compiler does not support _asm
*       inline assembler
****************************************************************************/

void CfpDlg::ShowStatus()                                           // REQ #006
    {                                                               // REQ #006
     WORD status = GetStatus();                                     // REQ #006
     FSRtoControls(status);                                         // REQ #006
    } // CfpDlg::ShowStatus                                         // REQ #006

/****************************************************************************
*                              CfpDlg::GetStatus
* Result: WORD
*       x87 FPU status word
* Effect: 
*       ¶
****************************************************************************/

WORD CfpDlg::GetStatus()                                            // REQ #006
    {                                                               // REQ #006
     WORD result;                                                   // REQ #006
     __asm wait                                                     // REQ #006
     __asm fnstsw ax                                                // REQ #006
     __asm mov result, ax                                           // REQ #006
     return result;                                                 // REQ #006
    } // CfpDlg::GetStatus                                          // REQ #006

/****************************************************************************
*                          CfpDlg::OnBnClickedClear87
* Result: void
*       
* Effect: 
*       Clears the x87 FPU status
****************************************************************************/

void CfpDlg::OnBnClickedClear87()                                   // REQ #006
    {                                                               // REQ #006
     _clear87();                                                    // REQ #006
     ShowStatus();                                                  // REQ #006
    }                                                               // REQ #006


/****************************************************************************
*                           CfpDlg::OnBnClickedF2xm1
* Result: void
*       
* Effect: 
*       Computes (2**n1) - 1 and stores the result in n3
****************************************************************************/

void CfpDlg::OnBnClickedF2xm1()                                     // REQ #007
    {                                                               // REQ #007
     double d = number1.d;                                          // REQ #007
     _asm fld d                                                     // REQ #007
     _asm f2xm1                                                     // REQ #007
     _asm fstp d                                                    // REQ #007
     number3.d = d;                                                 // REQ #007
     ShowStatus();                                                  // REQ #007
     updateControls();                                              // REQ #007
     CString code;                                                  // REQ #007
     code = _T("fld n1\r\n")                                        // REQ #007
            _T("f2xm1\r\n")                                         // REQ #007
            _T("fstp n3\r\n")                                       // REQ #007
            ;                                                       // REQ #007
    }                                                               // REQ #007


/****************************************************************************
*                           CfpDlg::OnBnClickedFcom
* Result: void
*       
* Effect: 
*       FCOM n1, n2
****************************************************************************/

void CfpDlg::OnBnClickedFcom()                                      // REQ #007
    {                                                               // REQ #007
     double d1 = number1.d;                                         // REQ #007
     double d2 = number2.d;                                         // REQ #007
     _asm fld d1                                                    // REQ #007
     _asm fcomp d2                                                  // REQ #007
     ShowStatus();                                                  // REQ #007
     updateControls();                                              // REQ #007
     CString code = _T("      fld n1\r\n")                                // REQ #007
                    _T("      fcomp\r\n")                                 // REQ #007
                    _T("      fnstw ax\r\n")                              // REQ #007
                    _T(";****************\r\n")
                    _T("; <\r\n")
                    _T("      test ah, 041h\r\n")
                    _T("      jne GE\r\n")
                    _T("      ; less than\r\n")
                    _T("      jmp endLT\r\n")
                    _T("GE:   ; greater than or equal\r\n")
                    _T("endLT:; end of test\r\n")
                    _T(";****************\r\n")
                    _T("; <=\r\n")
                    _T("      test ah, 01h\r\n")
                    _T("      jne GT\r\n")
                    _T("      ; less than or equal\r\n")
                    _T("      jmp endLE\r\n")
                    _T("GT:   ; greater than\r\n")
                    _T("endLE:; end of test\r\n")
                    _T(";****************\r\n")
                    _T(" == \r\n")
                    _T("      test ah, 044h\r\n")
                    _T("      jp NE\r\n")
                    _T("      ; equal\r\n")
                    _T("      jmp endEQ\r\n")
                    _T("NE:   ; not equal\r\n")
                    _T("endEQ:;end of test\r\n")
                    _T(";****************\r\n")
                    _T("; != \r\n")
                    _T("      test ah, 044h\r\n")
                    _T("      jnp EQ\r\n")
                    _T("      ; not equal\r\n")
                    _T("      jmp endNE\r\n")
                    _T("EQ:   ; equal\r\n")
                    _T("endNE:; end of test\r\n")
                    _T(";****************\r\n")
                    _T("; > \r\n")
                    _T("      test ah, 05h\r\n")
                    _T("      jp LE\r\n")
                    _T("      ; greater than\r\n")
                    _T("      jmp endGT\r\n")
                    _T("LE:   ; less than or equal\r\n")
                    _T("endGT:;end of test\r\n")
                    _T(";****************\r\n")
                    _T("; >= \r\n")
                    _T("      test ah, 041h\r\n")
                    _T("      jp LT\r\n")
                    _T("      ; greater than or equal\r\n")
                    _T("      jmp endGE\r\n")
                    _T("LT:   ; less than\r\n")
                    _T("endGE:; end of test\r\n")
                    ;                                               // REQ #007
     c_Code.SetWindowText(code);                                    // REQ #007
    }                                                               // REQ #007


/****************************************************************************
*                           CfpDlg::OnBnClickedFchs
* Result: void
*       
* Effect: 
*       Changes the sign of n1 and stores the result in n3
****************************************************************************/

void CfpDlg::OnBnClickedFchs()                                      // REQ #007
    {                                                               // REQ #007
     double d1 = number1.d;                                         // REQ #007
     _asm fld d1                                                    // REQ #007
     _asm fchs                                                      // REQ #007
     _asm fstp d1                                                   // REQ #007
     number3.d = d1;                                                // REQ #007
     ShowStatus();                                                  // REQ #007
     updateControls();                                              // REQ #007
     CString code = _T("fld n1\r\n")                                // REQ #007
                    _T("fchs\r\n")                                  // REQ #007
                    _T("fstp n3");                                  // REQ #007
     c_Code.SetWindowText(code);                                    // REQ #007
    }                                                               // REQ #007

/****************************************************************************
*                           CfpDlg::OnBnClickedFabs
* Result: void
*       
* Effect: 
*       FABS
****************************************************************************/

void CfpDlg::OnBnClickedFabs()                                      // REQ #007
    {                                                               // REQ #007
     double d1 = number1.d;                                         // REQ #007
     _asm fld d1                                                    // REQ #007
     _asm fabs                                                      // REQ #007
     _asm fstp d1                                                   // REQ #007
     number3.d = d1;                                                // REQ #007
     ShowStatus();                                                  // REQ #007
     updateControls();                                              // REQ #007
     CString code = _T("fld n1\r\n")                                // REQ #007
                    _T("fabs\r\n")                                  // REQ #007
                    _T("fstp n3");                                  // REQ #007
     c_Code.SetWindowText(code);                                    // REQ #007
    }                                                               // REQ #007

/****************************************************************************
*                           CfpDlg::OnBnClickedFld1
* Result: void
*       
* Effect: 
*       FLD1
****************************************************************************/

void CfpDlg::OnBnClickedFld1()                                      // REQ #007
    {                                                               // REQ #007
     double d1;                                                     // REQ #007
     _asm fld1                                                      // REQ #007
     _asm fstp d1                                                   // REQ #007
     number3.d = d1;                                                // REQ #007
     ShowStatus();                                                  // REQ #007
     updateControls();                                              // REQ #007
     CString code = _T("fld1\r\n")                                  // REQ #007
                    _T("fstp n3");                                  // REQ #007
     c_Code.SetWindowText(code);                                    // REQ #007
    }                                                               // REQ #007

/****************************************************************************
*                          CfpDlg::OnBnClickedFldl2t
* Result: void
*       
* Effect: 
*       FLDL2T
****************************************************************************/

void CfpDlg::OnBnClickedFldl2t()                                    // REQ #007
    {                                                               // REQ #007
     double d1;                                                     // REQ #007
     _asm fldl2t                                                    // REQ #007
     _asm fstp d1                                                   // REQ #007
     number3.d = d1;                                                // REQ #007
     ShowStatus();                                                  // REQ #007
     updateControls();                                              // REQ #007
     CString code = _T("fldl2t\r\n")                                // REQ #007
                    _T("fstp n3");                                  // REQ #007
     c_Code.SetWindowText(code);                                    // REQ #007
    }                                                               // REQ #007

/****************************************************************************
*                          CfpDlg::OnBnClickedFldl2e
* Result: void
*       
* Effect: 
*       FLDL2E
****************************************************************************/

void CfpDlg::OnBnClickedFldl2e()                                    // REQ #007
    {                                                               // REQ #007
     double d1;                                                     // REQ #007
     _asm fldl2e                                                    // REQ #007
     _asm fstp d1                                                   // REQ #007
     number3.d = d1;                                                // REQ #007
     ShowStatus();                                                  // REQ #007
     updateControls();                                              // REQ #007
     CString code = _T("fldl2e\r\n")                                // REQ #007
                    _T("fstp n3");                                  // REQ #007
     c_Code.SetWindowText(code);                                    // REQ #007
    }                                                               // REQ #007

/****************************************************************************
*                           CfpDlg::OnBnClickedFldpi
* Result: void
*       
* Effect: 
*       FLDPI
****************************************************************************/

void CfpDlg::OnBnClickedFldpi()                                     // REQ #007
    {                                                               // REQ #007
     double d1;                                                     // REQ #007
     _asm fldpi                                                     // REQ #007
     _asm fstp d1                                                   // REQ #007
     number3.d = d1;                                                // REQ #007
     ShowStatus();                                                  // REQ #007
     updateControls();                                              // REQ #007
     CString code = _T("fldpi\r\n")                                 // REQ #007
                    _T("fstp n3");                                  // REQ #007
     c_Code.SetWindowText(code);                                    // REQ #007
    }                                                               // REQ #007

/****************************************************************************
*                          CfpDlg::OnBnClickedFldlg2
* Result: void
*       
* Effect: 
*       FLDLG2
****************************************************************************/

void CfpDlg::OnBnClickedFldlg2()                                    // REQ #007
    {                                                               // REQ #007
     double d1;                                                     // REQ #007
     _asm fldlg2                                                    // REQ #007
     _asm fstp d1                                                   // REQ #007
     number3.d = d1;                                                // REQ #007
     ShowStatus();                                                  // REQ #007
     updateControls();                                              // REQ #007
     CString code = _T("fldlg2\r\n")                                // REQ #007
                    _T("fstp n3");                                  // REQ #007
     c_Code.SetWindowText(code);                                    // REQ #007
    }                                                               // REQ #007

/****************************************************************************
*                          CfpDlg::OnBnClickedFldln2
* Result: void
*       
* Effect: 
*       FLDLN2
****************************************************************************/

void CfpDlg::OnBnClickedFldln2()                                    // REQ #007
    {                                                               // REQ #007
     double d1;                                                     // REQ #007
     _asm fldln2                                                    // REQ #007
     _asm fstp d1                                                   // REQ #007
     number3.d = d1;                                                // REQ #007
     ShowStatus();                                                  // REQ #007
     updateControls();                                              // REQ #007
     CString code = _T("fldln2\r\n")                                // REQ #007
                    _T("fstp n3");                                  // REQ #007
     c_Code.SetWindowText(code);                                    // REQ #007
    }                                                               // REQ #007

/****************************************************************************
*                           CfpDlg::OnBnClickedFldz
* Result: void
*       
* Effect: 
*       FLDZ
****************************************************************************/

void CfpDlg::OnBnClickedFldz()                                      // REQ #007
    {                                                               // REQ #007
     double d1;                                                     // REQ #007
     _asm fldz                                                      // REQ #007
     _asm fstp d1                                                   // REQ #007
     number3.d = d1;                                                // REQ #007
     ShowStatus();                                                  // REQ #007
     updateControls();                                              // REQ #007
     CString code = _T("fldz\r\n")                                  // REQ #007
                    _T("fstp n3");                                  // REQ #007
     c_Code.SetWindowText(code);                                    // REQ #007
    }                                                               // REQ #007

/****************************************************************************
*                           CfpDlg::OnBnClickedFpatan
* Result: void
*       
* Effect: 
*       PATAN n1/n2
****************************************************************************/

void CfpDlg::OnBnClickedFpatan()                                    // REQ #007
    {                                                               // REQ #007
     double d1 = number1.d;                                         // REQ #007
     double d2 = number2.d;                                         // REQ #007
     double d3;                                                     // REQ #007
                                                                    // REQ #007
     __asm fld d1  ;                                                // REQ #007
     __asm fld d2  ;                                                // REQ #007
     __asm fpatan  ;                                                // REQ #007
     __asm fstp d3 ;                                                // REQ #007
                                                                    // REQ #007
     number3.d = d3;                                                // REQ #007
     ShowStatus();                                                  // REQ #007
     updateControls();                                              // REQ #007
     CString code = _T("fld n1\r\n")                                // REQ #007
                    _T("fld n2\r\n")                                // REQ #007
                    _T("patan\r\n")                                 // REQ #007
                    _T("fstp n3\r\n")                               // REQ #007
                    ;                                               // REQ #007
     c_Code.SetWindowText(code);                                    // REQ #007
    }                                                               // REQ #007

/****************************************************************************
*                           CfpDlg::OnBnClickedFprem
* Result: void
*       
* Effect: 
*       FPREM n1 / n2
****************************************************************************/

void CfpDlg::OnBnClickedFprem()                                     // REQ #007
    {                                                               // REQ #007
     double d1 = number1.d;                                         // REQ #007
     double d2 = number2.d;                                         // REQ #007
     double d3;                                                     // REQ #007
                                                                    // REQ #007
     __asm fld d2                                                   // REQ #007
     __asm fld d1                                                   // REQ #007
     __asm fprem                                                    // REQ #007
     __asm fstp d3                                                  // REQ #007
     __asm fstp d2                                                  // REQ #007
     number3.d = d3;                                                // REQ #007
     ShowStatus();                                                  // REQ #007
     updateControls();                                              // REQ #007
     CString code = _T("fld d2\r\n")                                // REQ #007
                    _T("fld d1\r\n")                                // REQ #007
                    _T("fprem\r\n")                                 // REQ #007
                    _T("fstp d3\r\n")                               // REQ #007
                    _T("fstp d2\r\n")                               // REQ #007
                    ;                                               // REQ #007
     c_Code.SetWindowText(code);                                    // REQ #007
    }                                                               // REQ #007

/****************************************************************************
*                          CfpDlg::OnBnClickedFprem1
* Result: void
*       
* Effect: 
*       FPREM1
****************************************************************************/

void CfpDlg::OnBnClickedFprem1()                                    // REQ #007
    {                                                               // REQ #007
     double d1 = number1.d;                                         // REQ #007
     double d2 = number2.d;                                         // REQ #007
     double d3;                                                     // REQ #007
                                                                    // REQ #007
     __asm fld d2                                                   // REQ #007
           __asm fld d1                                             // REQ #007
           __asm fprem1                                             // REQ #007
           __asm fstp d3                                            // REQ #007
           __asm fstp d2                                            // REQ #007
           number3.d = d3;                                          // REQ #007
     ShowStatus();                                                  // REQ #007
     updateControls();                                              // REQ #007
     CString code = _T("fld d2\r\n")                                // REQ #007
                    _T("fld d1\r\n")                                // REQ #007
                    _T("fprem1\r\n")                                // REQ #007
                    _T("fstp d3\r\n")                               // REQ #007
                    _T("fstp d2\r\n")                               // REQ #007
                    ;                                               // REQ #007
     c_Code.SetWindowText(code);                                    // REQ #007
    }                                                               // REQ #007

/****************************************************************************
*                           CfpDlg::OnBnClickedFptan
* Result: void
*       
* Effect: 
*       FPTAN
****************************************************************************/

void CfpDlg::OnBnClickedFptan()                                     // REQ #007
    {                                                               // REQ #007
     double d1 = number1.d;                                         // REQ #007
     double d3;                                                     // REQ #007
                                                                    // REQ #007
     __asm fld d1                                                   // REQ #007
     __asm fptan                                                    // REQ #007
     __asm fstp d3                                                  // REQ #007
     __asm fstp d3                                                  // REQ #007
     number3.d = d3;                                                // REQ #007
     ShowStatus();                                                  // REQ #007
     updateControls();                                              // REQ #007
     CString code = _T("fld d1\r\n")                                // REQ #007
                    _T("fptan\r\n")                                 // REQ #007
                    _T("fstp d3 ; remove 1\r\n")                    // REQ #007
                    _T("fstp d3 ; store result\r\n")                // REQ #007
                    ;                                               // REQ #007
     c_Code.SetWindowText(code);                                    // REQ #007
    }                                                               // REQ #007

/****************************************************************************
*                          CfpDlg::OnBnClickedFrndint
* Result: void
*       
* Effect: 
*       FPRNDINT
****************************************************************************/

void CfpDlg::OnBnClickedFrndint()                                   // REQ #007
    {                                                               // REQ #007
     double d1 = number1.d;                                         // REQ #007
     double d3;                                                     // REQ #007
                                                                    // REQ #007
     __asm fld d1                                                   // REQ #007
     __asm frndint                                                  // REQ #007
     __asm fstp d3                                                  // REQ #007
                                                                    // REQ #007
     number3.d = d3;                                                // REQ #007
     ShowStatus();                                                  // REQ #007
     updateControls();                                              // REQ #007
     CString code = _T("fld d1\r\n")                                // REQ #007
                    _T("frndint\r\n")                               // REQ #007
                    _T("fstp d3\r\n")                               // REQ #007
                    ;                                               // REQ #007
     c_Code.SetWindowText(code);                                    // REQ #007
    }                                                               // REQ #007

/****************************************************************************
*                          CfpDlg::OnBnClickedFscale
* Result: void
*       
* Effect: 
*       FSCALE
****************************************************************************/

void CfpDlg::OnBnClickedFscale()                                    // REQ #007
    {                                                               // REQ #007
     double d1 = number1.d;                                         // REQ #007
     double d2 = number2.d;                                         // REQ #007
     double d3;                                                     // REQ #007
                                                                    // REQ #007
     __asm fld d2                                                   // REQ #007
     __asm fld d1                                                   // REQ #007
     __asm fscale                                                   // REQ #007
     __asm fstp d3                                                  // REQ #007
                                                                    // REQ #007
     number3.d = d3;                                                // REQ #007
     ShowStatus();                                                  // REQ #007
     updateControls();                                              // REQ #007
     CString code = _T("fld d2\r\n")                                // REQ #007
                    _T("fld d1\r\n")                                // REQ #007
                    _T("fscale\r\n")                                // REQ #007
                    _T("fstp d3\r\n")                               // REQ #007
                    ;                                               // REQ #007
     c_Code.SetWindowText(code);                                    // REQ #007
    }                                                               // REQ #007

/****************************************************************************
*                           CfpDlg::OnBnClickedFsin
* Result: void
*       
* Effect: 
*       FSIN
****************************************************************************/

void CfpDlg::OnBnClickedFsin()                                      // REQ #007
    {                                                               // REQ #007
     double d = number1.d;                                          // REQ #007
     double r;                                                      // REQ #007
     __asm fld d                                                    // REQ #007
     __asm fsin                                                     // REQ #007
     __asm fstp r                                                   // REQ #007
     number3.d = r;                                                 // REQ #007
     ShowStatus();                                                  // REQ #007
     updateControls();                                              // REQ #007
     CString code = _T("fld n1\r\n")                                // REQ #007
                    _T("fsin\r\n")                                  // REQ #007
                    _T("fstp n3\r\n")                               // REQ #007
                    ;                                               // REQ #007
     c_Code.SetWindowText(code);                                    // REQ #007
    }                                                               // REQ #007

/****************************************************************************
*                          CfpDlg::OnBnClickedFsincos
* Result: void
*       
* Effect: 
*       FSINCOS n1->n2,n3
****************************************************************************/

void CfpDlg::OnBnClickedFsincos()                                   // REQ #007
    {                                                               // REQ #007
     double d1 = number1.d;                                         // REQ #007
     double d2;                                                     // REQ #007
     double d3;                                                     // REQ #007
                                                                    // REQ #007
     __asm fld d1                                                   // REQ #007
     __asm fsincos                                                  // REQ #007
     __asm fstp d3                                                  // REQ #007
     __asm fstp d2                                                  // REQ #007
     number2.d = d2;                                                // REQ #007
     number3.d = d3;                                                // REQ #007
     ShowStatus();                                                  // REQ #007
     updateControls();                                              // REQ #007
     CString code = _T("fld n1\r\n")                                // REQ #007
                    _T("fsincos\r\n")                               // REQ #007
                    _T("fstp n3 ; cos\r\n")                         // REQ #007
                    _T("fstp n2 ; sin\r\n")                         // REQ #007
                    ;                                               // REQ #007
     c_Code.SetWindowText(code);                                    // REQ #007
    }                                                               // REQ #007

/****************************************************************************
*                           CfpDlg::OnBnClickedFsqrt
* Result: void
*       
* Effect: 
*       FSQRT
****************************************************************************/

void CfpDlg::OnBnClickedFsqrt()                                     // REQ #007
    {                                                               // REQ #007
     double d1 = number1.d;                                         // REQ #007
     __asm fld d1                                                   // REQ #007
     __asm fsqrt                                                    // REQ #007
     __asm fstp d1                                                  // REQ #007
     number3.d = d1;                                                // REQ #007
     ShowStatus();                                                  // REQ #007
     updateControls();                                              // REQ #007
     CString code = _T("fld n1\r\n")                                // REQ #007
                    _T("fsqrt\r\n")                                 // REQ #007
                    _T("fstp n3\r\n");                              // REQ #007
     c_Code.SetWindowText(code);                                    // REQ #007
    }                                                               // REQ #007


/****************************************************************************
*                           CfpDlg::OnBnClickedFxam
* Result: void
*       
* Effect: 
*       FXAM
****************************************************************************/

void CfpDlg::OnBnClickedFxam()                                      // REQ #007
    {                                                               // REQ #007
     double d = number1.d;                                          // REQ #007
     __asm fld d                                                    // REQ #007
     __asm fxam                                                     // REQ #007
     __asm fstp d                                                   // REQ #007
                                                                    // REQ #007
     WORD w = GetStatus();                                          // REQ #007
     ShowStatus();                                                  // REQ #007
                                                                    // REQ #007
     static const WORD Cmask = (C0mask | C2mask | C3mask);          // REQ #007
     CString s;                                                     // REQ #007
     switch(w & Cmask)                                              // REQ #007
        { /* cmask */                                               // REQ #007
         case 0: // unsupported                                     // REQ #007
            s.LoadString(IDS_UNSUPPORTED);                          // REQ #007
            break;                                                  // REQ #007
         case C0mask:                                               // REQ #007
            s.LoadString(IDS_NAN);                                  // REQ #007
            break;                                                  // REQ #007
         case C2mask:                                               // REQ #007
            s.LoadString(IDS_NORMAL_FINITE);                        // REQ #007
            break;                                                  // REQ #007
         case C2mask | C0mask:                                      // REQ #007
            s.LoadString(IDS_INFINITY);                             // REQ #007
            break;                                                  // REQ #007
         case C3mask:                                               // REQ #007
            s.LoadString(IDS_ZERO);                                 // REQ #007
            break;                                                  // REQ #007
         case C3mask | C0mask:                                      // REQ #007
            s.LoadString(IDS_EMPTY);                                // REQ #007
            break;                                                  // REQ #007
         case C3mask | C2mask:                                      // REQ #007
            s.LoadString(IDS_DENORMAL);                             // REQ #007
            break;                                                  // REQ #007
         default:                                                   // REQ #007
            s.LoadString(IDS_FP_UNKNOWN);                           // REQ #007
            break;                                                  // REQ #007
        } /* cmask */                                               // REQ #007
                                                                    // REQ #007
     updateControls();                                              // REQ #007
     c_StatusComment.SetWindowText(s);                              // REQ #007
     CString code = _T("        fld n1\r\n")                                                // REQ #007
                    _T("        fxam\r\n")                                                  // REQ #007
                    _T("        fstp n1\r\n")                                               // REQ #007
                    _T("        fnstw ax\r\n")                                              // REQ #007
                    _T("        sahf\r\n")                                                  // REQ #007
                    _T("        ; ZF PF CF\r\n")                                            // REQ #007
                    _T("        ; C3 C2 C0\r\n")                                            // REQ #007
                    _T("        jc      C3_?_C2_?_C0_1  ; ??1------------------------+\r\n")// REQ #007
                    _T("        jp      C3_?_C2_1_C1_0  ; ?10--------------------+   |\r\n")// REQ #007
                    _T("        jz      C3_1_C2_0_C0_0  ; 100----------------+   |   |\r\n")// REQ #007
                    _T("        ; >> 000: Unsupported format <<              |   |   |\r\n")// REQ #007
                    _T("        jmp end                 ;                    |   |   |\r\n")// REQ #007
                    _T("C3_1_C2_0_C0_0: ; <----------------------------------+   |   |\r\n")// REQ #007
                    _T("        ; >> 100: Zero <<                                |   |\r\n")// REQ #007
                    _T("        jmp end                 ;                        |   |\r\n")// REQ #007
                    _T("C3_?_C2_1_C1_0: ; ?10<-----------------------------------+   |\r\n")// REQ #007
                    _T("        jz C3_1_C2_1_C3_0 ; 110--------------------------+   |\r\n")// REQ #007
                    _T("        ; >> 010: Normal Finite <<                       |   |\r\n")// REQ #007
                    _T("        jmp end                 ;                        |   |\r\n")// REQ #007
                    _T("C3_1_C2_1_C3_0: ; 110<-----------------------------------+   |\r\n")// REQ #007
                    _T("        ; >> 110: Denormalized <<                            |\r\n")// REQ #007
                    _T("        jmp end                 ;                            |\r\n")// REQ #007
                    _T("C3_?_C2_?_C0_1: ; ??1<---------------------------------------+\r\n")// REQ #007
                    _T("        jp C3_?_C2_1_C0_1       ; ?11------------------------+\r\n")// REQ #007
                    _T("        jz C3_1_C2_0_C0_1       ; 101--------------------+   |\r\n")// REQ #007
                    _T("        ; >> 001: NaN <<        ;                        |   |\r\n")// REQ #007
                    _T("        jmp end                 ;                        |   |\r\n")// REQ #007
                    _T("C3_1_C2_0_C0_1: ; 101<-----------------------------------+   |\r\n")// REQ #007
                    _T("        ; >> 101: Empty <<      ;                            |\r\n")// REQ #007
                    _T("        jmp end                 ;                            |\r\n")// REQ #007
                    _T("C3_?_C2_1_C0_1: ; ?11<---------------------------------------+\r\n")// REQ #007
                    _T("        jz C3_1_C2_1_C0_1 ; 111------------------------------+\r\n")// REQ #007
                    _T("        ; >> 011: Infinity <<                                |\r\n")// REQ #007
                    _T("        jmp end           ;                                  |\r\n")// REQ #007
                    _T("C3_1_C2_1_C0_1: ; 111<---------------------------------------+\r\n")// REQ #007
                    _T("        ; >> 111: illegal combination\r\n")                         // REQ #007
                    _T("end:\r\n")                                                          // REQ #007
                    ;                                                                       // REQ #007
     c_Code.SetWindowText(code);                                                            // REQ #007
    }                                                                                       // REQ #007

/****************************************************************************
*                          CfpDlg::OnBnClickedFxtract
* Result: void
*       
* Effect: 
*       FXTRACT
****************************************************************************/

void CfpDlg::OnBnClickedFxtract()                                   // REQ #007
    {                                                               // REQ #007
     double d1 = number1.d;                                         // REQ #007
     double d2;                                                     // REQ #007
     double d3;                                                     // REQ #007
     __asm fld d1                                                   // REQ #007
     __asm fxtract                                                  // REQ #007
     __asm fstp d3 ; significand                                    // REQ #007
     __asm fstp d2 ; exponent                                       // REQ #007
     number2.d = d2;                                                // REQ #007
     number3.d = d3;                                                // REQ #007
     ShowStatus();                                                  // REQ #007
     updateControls();                                              // REQ #007
     CString code = _T("fld n1\r\n")                                // REQ #007
                    _T("fxtract\r\n")                               // REQ #007
                    _T("fstp n3 ; significand\r\n")                 // REQ #007
                    _T("fstp n2 ; exponent\r\n")                    // REQ #007
                    ;                                               // REQ #007
    }                                                               // REQ #007

/****************************************************************************
*                           CfpDlg::OnBnClickedFyl2x
* Result: void
*       
* Effect: 
*       FYL2X n1 * log2(n2) -> n3
****************************************************************************/

void CfpDlg::OnBnClickedFyl2x()                                     // REQ #007
    {                                                               // REQ #007
     double d1 = number1.d;                                         // REQ #007
     double d2 = number2.d;                                         // REQ #007
     double d3;                                                     // REQ #007
     __asm fld d1                                                   // REQ #007
     __asm fld d2                                                   // REQ #007
     __asm fyl2x                                                    // REQ #007
     __asm fstp d3                                                  // REQ #007
     number3.d = d3;                                                // REQ #007
     ShowStatus();                                                  // REQ #007
     updateControls();                                              // REQ #007
     CString code = _T("fld n1\r\n")                                // REQ #007
                    _T("fld n2\r\n")                                // REQ #007
                    _T("fyl2x ; ST(1) = ST(1) * log2(ST(0)); pop\r\n")// REQ #007
                    _T("fstp d3\r\n")                               // REQ #007
                    ;                                               // REQ #007
     c_Code.SetWindowText(code);                                    // REQ #007
    }                                                               // REQ #007

/****************************************************************************
*                          CfpDlg::OnBnClickedFyl2xp1
* Result: void
*       
* Effect: 
*       FYL2XP1
****************************************************************************/

void CfpDlg::OnBnClickedFyl2xp1()                                   // REQ #007
    {                                                               // REQ #007
     double d1 = number1.d;                                         // REQ #007
     double d2 = number2.d;                                         // REQ #007
     double d3;                                                     // REQ #007
     __asm fld d1                                                   // REQ #007
           __asm fld d2                                             // REQ #007
           __asm fyl2xp1                                            // REQ #007
           __asm fstp d3                                            // REQ #007
           number3.d = d3;                                          // REQ #007
     ShowStatus();                                                  // REQ #007
     updateControls();                                              // REQ #007
     CString code = _T("fld n1\r\n")                                // REQ #007
                    _T("fld n2\r\n")                                // REQ #007
                    _T("fyl2xp1 ; ST(1) = ST(1) * log2(ST(0) + 1); pop\r\n")// REQ #007
                    _T("fstp d3\r\n")                               // REQ #007
                    ;                                               // REQ #007
     c_Code.SetWindowText(code);                                    // REQ #007
    }                                                               // REQ #007

/****************************************************************************
*                           CfpDlg::OnBnClickedFcos
* Result: void
*       
* Effect: 
*       FCOS n1 -> n3
****************************************************************************/

void CfpDlg::OnBnClickedFcos()                                      // REQ #007
    {                                                               // REQ #007
     double d = number1.d;                                          // REQ #007
     double r;                                                      // REQ #007
     __asm fld d                                                    // REQ #007
     __asm fcos                                                     // REQ #007
     __asm fstp r                                                   // REQ #007
     number3.d = r;                                                 // REQ #007
     ShowStatus();                                                  // REQ #007
     updateControls();                                              // REQ #007
     CString code = _T("fld n1\r\n")                                // REQ #007
                    _T("fcos\r\n")                                  // REQ #007
                    _T("fstp n3\r\n")                               // REQ #007
                    ;                                               // REQ #007
     c_Code.SetWindowText(code);                                    // REQ #007
    }                                                               // REQ #007
