/*****************************************************************************
*           Change Log
*  Date     | Change
*-----------+-----------------------------------------------------------------
* 13-Dec-09 | REQ #005: Created
*****************************************************************************/
#pragma once                                                        // REQ #005
                                                                    // REQ #005
#include "NoNotifyEdit.h"                                           // REQ #005
// CNoNotifyHexEdit                                                 // REQ #005
                                                                    // REQ #005
class CNoNotifyHexEdit : public CNoNotifyEdit                       // REQ #005
{                                                                   // REQ #005
        DECLARE_DYNAMIC(CNoNotifyHexEdit)                           // REQ #005
                                                                    // REQ #005
public:                                                             // REQ #005
        CNoNotifyHexEdit();                                         // REQ #005
        virtual ~CNoNotifyHexEdit();                                // REQ #005
                                                                    // REQ #005
protected:                                                          // REQ #005
        DECLARE_MESSAGE_MAP()                                       // REQ #005
protected:                                                          // REQ #005
        afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags); // REQ #005
};                                                                  // REQ #005


