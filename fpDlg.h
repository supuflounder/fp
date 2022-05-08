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
*****************************************************************************/
// fpDlg.h : header file
//

#pragma once


#include "ArrowButton.h"
#include "NoNotifyHexEdit.h"
#include "afxwin.h"

// CfpDlg dialog
class CfpDlg : public CDialog
{
// Construction
public:
        CfpDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
        enum { IDD = IDD_FP_DIALOG };

        protected:
        virtual void DoDataExchange(CDataExchange* pDX);        // DDX/DDV support


// Implementation
protected:
        HICON m_hIcon;
        void BitsToFP();
        void FPToBits();
        void updateControls();
        void FSRtoControls(WORD status);
        void ShowStatus();
        WORD GetStatus();
        CFont FixedFont;                                            // REQ #007
        
        typedef union _dint {
           double d;
           ULONGLONG n;
           _dint() { n = 0; }
        } dint;

        dint number; // the number shown in the bits control
        dint number1; // first operand
        dint number2; // second operand
        dint number3; // result
        dint NaNPlus; // generic +#NaN
        dint NaNMinus; // generic -#Nan
        dint InfPlus; // +#INF
        dint InfMinus; // -#INF
        dint IndMinus; // -#IND

        BOOL initialized;
        
        void ShowNumber(dint & num, CEdit & fmt, CNoNotifyEdit & target);

        // Generated message map functions
        virtual BOOL OnInitDialog();
        afx_msg HCURSOR OnQueryDragIcon();
        afx_msg void OnBitChanged(UINT id);
        afx_msg void OnBnClicked1To2();
        afx_msg void OnBnClicked1To3();
        afx_msg void OnBnClicked2To1();
        afx_msg void OnBnClicked2To3();
        afx_msg void OnBnClicked3To1();
        afx_msg void OnBnClicked3To2();
        afx_msg void OnBnClickedAdd();
        afx_msg void OnBnClickedClear87();                          // REQ #006
        afx_msg void OnBnClickedDivide();
        afx_msg void OnBnClickedF2xm1();                            // REQ #007
        afx_msg void OnBnClickedFabs();                             // REQ #007
        afx_msg void OnBnClickedFchs();                             // REQ #007
        afx_msg void OnBnClickedFcom();                             // REQ #007
        afx_msg void OnBnClickedFld1();                             // REQ #007
        afx_msg void OnBnClickedFldl2e();                           // REQ #007
        afx_msg void OnBnClickedFldl2t();                           // REQ #007
        afx_msg void OnBnClickedFldlg2();                           // REQ #007
        afx_msg void OnBnClickedFldln2();                           // REQ #007
        afx_msg void OnBnClickedFldpi();                            // REQ #007
        afx_msg void OnBnClickedFldz();                             // REQ #007
        afx_msg void OnBnClickedFpatan();                           // REQ #007
        afx_msg void OnBnClickedFprem();                            // REQ #007
        afx_msg void OnBnClickedFprem1();                           // REQ #007
        afx_msg void OnBnClickedFptan();                            // REQ #007
        afx_msg void OnBnClickedFrndint();                          // REQ #007
        afx_msg void OnBnClickedFrom1();
        afx_msg void OnBnClickedFrom2();
        afx_msg void OnBnClickedFrom3();
        afx_msg void OnBnClickedFscale();                           // REQ #007
        afx_msg void OnBnClickedFsin();                             // REQ #007
        afx_msg void OnBnClickedFsincos();                          // REQ #007
        afx_msg void OnBnClickedFsqrt();                            // REQ #007
        afx_msg void OnBnClickedFxam();                             // REQ #007
        afx_msg void OnBnClickedFxtract();                          // REQ #007
        afx_msg void OnBnClickedFyl2x();                            // REQ #007
        afx_msg void OnBnClickedFyl2xp1();                          // REQ #007
        afx_msg void OnBnClickedInd();                              // REQ #003
        afx_msg void OnBnClickedInfMinus();
        afx_msg void OnBnClickedInfPlus();
        afx_msg void OnBnClickedMultiply();
        afx_msg void OnBnClickedNaNMinus();
        afx_msg void OnBnClickedNaNPlus();
        afx_msg void OnBnClickedNumberTo1();
        afx_msg void OnBnClickedNumberTo2();
        afx_msg void OnBnClickedNumberTo3();
        afx_msg void OnBnClickedSetFormat1();
        afx_msg void OnBnClickedSetFormat2();
        afx_msg void OnBnClickedSetFormat3();
        afx_msg void OnBnClickedSetFormatNumberN();
        afx_msg void OnBnClickedShowNybbles();                      // REQ #004
        afx_msg void OnBnClickedShowSpaces();
        afx_msg void OnBnClickedSubtract();
        afx_msg void OnChangeBinaryExponent();
        afx_msg void OnChangeExcess1023Exponent();                  // REQ #001
        afx_msg void OnChangeHexMantissa();
        afx_msg void OnChangeNumber();
        afx_msg void OnChangeNumber1();
        afx_msg void OnChangeNumber2();
        afx_msg void OnChangeNumber3();
        afx_msg void OnClearAllExponent();
        afx_msg void OnClearAllMantissa();
        afx_msg void OnClose();
        afx_msg void OnEnChangeHexExponent();                       // REQ #002
        afx_msg void OnPaint();
        afx_msg void OnSetAllExponent();
        afx_msg void OnSetAllMantissa();
        afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
        afx_msg void OnUpdateRequest();
        afx_msg void OnZeroExponent();
        virtual void OnOK();
        virtual void OnCancel();

        DECLARE_MESSAGE_MAP()

        CArrowButton c_SetFormatN;
        CArrowButton c_SetFormat1;
        CArrowButton c_SetFormat2;
        CArrowButton c_SetFormat3;
        CArrowButton c_NumberTo1;
        CArrowButton c_NumberTo2;
        CArrowButton c_NumberTo3;
        CArrowButton c_3ToNumber;
        CArrowButton c_2ToNumber;
        CArrowButton c_1ToNumber;
        CArrowButton c_1To2;
        CArrowButton c_1To3;
        CArrowButton c_2To1;
        CArrowButton c_2To3;
        CArrowButton c_3To1;
        CArrowButton c_3To2;
        CButton c_ShowSpaces;
        CNoNotifyHexEdit c_HexMantissa;                             // REQ #005
        CNoNotifyEdit c_Number;
        CNoNotifyEdit c_Number1;
        CNoNotifyEdit c_Number2;
        CNoNotifyEdit c_Number3;
        CEdit c_FormatN;
        CEdit c_Format1;
        CEdit c_Format2;
        CEdit c_Format3;
        CNoNotifyEdit c_BinaryExponent;
        CNoNotifyEdit c_Excess1023Exponent;                         // REQ #001
        CNoNotifyHexEdit c_HexExponent;                             // REQ #005
        CButton c_FormatMinus;
        CButton c_FormatPlus;
        CButton c_FormatZero;
        CButton c_FormatHash;
        CButton c_FormatBlank;
        CButton c_WidthNone;
        CButton c_UseWidth;
        CEdit c_Width;
        CSpinButtonCtrl c_SpinWidth;
        CButton c_PrecisionNone;
        CButton c_UsePrecision;
        CEdit c_Precision;
        CSpinButtonCtrl c_SpinPrecision;
        CButton c_Type_e;
        CButton c_Type_E;
        CButton c_Type_f;
        CButton c_Type_g;
        CButton c_Type_G;
        CStatic c_Dot;
        CEdit c_Format;
        CSpinButtonCtrl c_SpinBinaryExponent;
        CSpinButtonCtrl c_SpinExcess1023Exponent;                   // REQ #001
        CStatic c_Nybble01;                                         // REQ #004
        CStatic c_Nybble12;                                         // REQ #004
        CStatic c_Nybble23;                                         // REQ #004
        CStatic c_Nybble34;                                         // REQ #004
        CStatic c_Nybble45;                                         // REQ #004
        CStatic c_Nybble56;                                         // REQ #004
        CStatic c_Nybble67;                                         // REQ #004
        CStatic c_Nybble78;                                         // REQ #004
        CStatic c_Nybble89;                                         // REQ #004
        CStatic c_Nybble910;                                        // REQ #004
        CStatic c_Nybble1011;                                       // REQ #004
        CStatic c_Nybble1112;                                       // REQ #004
        CStatic c_Nybble1314;                                       // REQ #004
        CStatic c_Nybble1415;                                       // REQ #004
        CButton c_ShowNybbles;                                      // REQ #004
        CStatic c_NybbleFSR01;                                      // REQ #006
        CStatic c_NybbleFSR12;                                      // REQ #006
        CStatic c_NybbleFSR23;                                      // REQ #006
        CEdit c_Top;                                                // REQ #007
        CEdit c_Code;                                               // REQ #007
        CEdit c_StatusComment;                                      // REQ #007
public:
    afx_msg void OnBnClickedFcos();
};
