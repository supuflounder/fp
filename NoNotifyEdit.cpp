// NoNotifyEdit.cpp : implementation file
//

#include "stdafx.h"
#include "NoNotifyEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CNoNotifyEdit, CEdit)
/////////////////////////////////////////////////////////////////////////////
// CNoNotifyEdit

CNoNotifyEdit::CNoNotifyEdit()
{
 notify = TRUE;
}

CNoNotifyEdit::~CNoNotifyEdit()
{
}


BEGIN_MESSAGE_MAP(CNoNotifyEdit, CEdit)
        //{{AFX_MSG_MAP(CNoNotifyEdit)
        ON_CONTROL_REFLECT_EX(EN_CHANGE, OnChange)
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNoNotifyEdit message handlers

BOOL CNoNotifyEdit::OnChange() 
{
 return !notify;
}

void CNoNotifyEdit::SetWindowTextNoNotify(LPCTSTR s)
    { 
     CString old;
     CEdit::GetWindowText(old);
     if(old == s)
        return; // do nothing, already set
     BOOL previous = notify;
     notify = FALSE;
     CEdit::SetWindowText(s);
     notify = previous;
    }

/****************************************************************************
*                    CNoNotifyEdit::SetWindowTextNoNotify
* Inputs:
*       int n: Value to set
* Result: void
*       
* Effect: 
*       Convets int to %d number and sets it
****************************************************************************/

void CNoNotifyEdit::SetWindowTextNoNotify(int n)
    {
     CString s;
     s.Format(_T("%d"), n);
     SetWindowTextNoNotify(s);
    } // CNoNotifyEdit::SetWindowTextNoNotify

/****************************************************************************
*                        CNoNotifyEdit::SetWindowText
* Inputs:
*       LPCTSTR s: String to set
* Result: void
*       
* Effect: 
*       Sets the window text, unless the contents are already the same
****************************************************************************/

void CNoNotifyEdit::SetWindowText(LPCTSTR s)
    {
     CString old;
     CEdit::GetWindowText(old);
     if(old == s)
        return;
     CEdit::SetWindowText(s);
    } // CNoNotifyEdit::SetWindowText

/****************************************************************************
*                         CNoNotifyEdit::GetWindowInt
* Result: int
*       
* Effect: 
*       Numeric value of edit control
****************************************************************************/

int CNoNotifyEdit::GetWindowInt()
    {
     CString s;
     GetWindowText(s);
     return _ttoi(s);
    } // CNoNotifyEdit::GetWindowInt
