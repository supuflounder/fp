#if !defined(AFX_NONOTIFYEDIT_H__5263A9FD_68A3_11D7_A087_00095B1A7BE1__INCLUDED_)
#define AFX_NONOTIFYEDIT_H__5263A9FD_68A3_11D7_A087_00095B1A7BE1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NoNotifyEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNoNotifyEdit window

class CNoNotifyEdit : public CEdit
{
    DECLARE_DYNCREATE(CNoNotifyEdit)
// Construction
public:
        CNoNotifyEdit();

// Attributes
public:
    BOOL SetNotify(BOOL newmode) { BOOL old = notify; notify = newmode; return old; }
    BOOL GetNotify() { return notify; }
// Operations
public:
    void SetWindowTextNoNotify(LPCTSTR s);
    void SetWindowTextNoNotify(int n);
    void SetWindowText(LPCTSTR s);
    int GetWindowInt();
// Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CNoNotifyEdit)
        //}}AFX_VIRTUAL

// Implementation
public:
        virtual ~CNoNotifyEdit();

        // Generated message map functions
protected:
        BOOL notify;
        //{{AFX_MSG(CNoNotifyEdit)
        afx_msg BOOL OnChange();
        //}}AFX_MSG

        DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NONOTIFYEDIT_H__5263A9FD_68A3_11D7_A087_00095B1A7BE1__INCLUDED_)
