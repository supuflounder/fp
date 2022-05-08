/*****************************************************************************
*           Change Log
*  Date     | Change
*-----------+-----------------------------------------------------------------
* 13-Dec-09 | REQ #005: Created
*****************************************************************************/
// NoNotifyHexEdit.cpp : implementation file
//

#include "stdafx.h"
#include "NoNotifyHexEdit.h"

// CNoNotifyHexEdit

IMPLEMENT_DYNAMIC(CNoNotifyHexEdit, CNoNotifyEdit)
CNoNotifyHexEdit::CNoNotifyHexEdit()
{
}

CNoNotifyHexEdit::~CNoNotifyHexEdit()
{
}


/****************************************************************************
*                                 Message Map
****************************************************************************/

BEGIN_MESSAGE_MAP(CNoNotifyHexEdit, CNoNotifyEdit)                  // REQ #005
    ON_WM_CHAR()                                                    // REQ #005
END_MESSAGE_MAP()                                                   // REQ #005



// CNoNotifyHexEdit message handlers

/****************************************************************************
*                           CNoNotifyHexEdit::OnChar
* Inputs:
*       UINT nChar:
*       UINT nRepCnt:
*       UINT nFlags:
* Result: void
*       
* Effect: 
*       Rejects characters that are not hex
****************************************************************************/

void CNoNotifyHexEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)// REQ #005
    {                                                               // REQ #005
     switch(nChar)                                                  // REQ #005
        { /* char */                                                // REQ #005
         case _T('A'):                                              // REQ #005
         case _T('a'):                                              // REQ #005
         case _T('B'):                                              // REQ #005
         case _T('b'):                                              // REQ #005
         case _T('C'):                                              // REQ #005
         case _T('c'):                                              // REQ #005
         case _T('D'):                                              // REQ #005
         case _T('d'):                                              // REQ #005
         case _T('E'):                                              // REQ #005
         case _T('e'):                                              // REQ #005
         case _T('F'):                                              // REQ #005
         case _T('f'):                                              // REQ #005
         case _T('0'):                                              // REQ #005
         case _T('1'):                                              // REQ #005
         case _T('2'):                                              // REQ #005
         case _T('3'):                                              // REQ #005
         case _T('4'):                                              // REQ #005
         case _T('5'):                                              // REQ #005
         case _T('6'):                                              // REQ #005
         case _T('7'):                                              // REQ #005
         case _T('8'):                                              // REQ #005
         case _T('9'):                                              // REQ #005
         case _T('\b'):                                             // REQ #005
            break;                                                  // REQ #005
         default:                                                   // REQ #005
            MessageBeep(0);                                         // REQ #005
            return;                                                 // REQ #005
        } /* char */                                                // REQ #005
                                                                    // REQ #005
     CNoNotifyEdit::OnChar(nChar, nRepCnt, nFlags);                 // REQ #005
    }                                                               // REQ #005
