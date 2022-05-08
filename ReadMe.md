#fp: The Floating Point Explorer
![The FP main screen](./assets/images/fpscreen.png)
The screen is divided into several sections.  The upper part of the screen deals with the representation of a double.  The 64 check boxes represent the 64 bits of a double, shown in binary.  The high-order bit is the sign.  The next 11 bits are the exponent value, and the remaining 52 bits are the mantissa.

The `Set All` and `Clear All` buttons will set all the bits of the exponent or mantissa.  The `2^0` button sets the exponent to represent `2^0`. 

The Value edit control and associated spin control show and allow you to modify the exponent value, which is allowed to be in the range of 0..2047.  However, the more natural represention would be the excess-1024 representation.  Thus 0 given in the Excess-1024 control represents the absolute numeric value in the exponent of 1024.

The representation also allows you to see the hexadecimal value of the mantissa, and edit it by changing the hex value displayed.

There are four buttons to select specific representations, such as `+inf`, `-inf`, `+Nan` and `-NaN`.  For the NaN representations, an exponent of all 1s and a nonzero mantissa is a NaN, and I arbitrarily chose to set the low-order bit of the mantissa to represent these values.  You can experiment with other bits.

Together, all of these controls allow you to manipulate what is called the number. 

The large vertical arrow buttons pointing downward transfer a value from the number to one of the three "working registers" designated `n1`, `n2` and `n3`. The large upward pointing arrows transfer a value from one of the working registers to the number, so it can be examined in detail.

The horizontal arrow buttons transfer a value from the working register below them to one of the other working registers. 

The operation buttons perform the indicated operation `n3 = n1 op n2`. 

A typical sort of test might involve typing  value into `n1` (for example, 80), typing a value into `n2` (for example, 100) and performing an operation such as division, seeing the result in `n3`.  Then the large up-arrow above `n3` is clicked to transfer the result back to the number so its representation can be examined.

In terms of formatting, this also allows you to see the effects of the various formatting options.  The Format options allow selection of the flags, the width, the precision, and the type.  The formatting is shown to the left of the Format box.  The format can be associated with the display of `n1`, `n2`, `n3` or the number by clicking one of the small arrow buttons.
##Microsoft Foundation Class Output

================================================================================
    MICROSOFT FOUNDATION CLASS LIBRARY : fp Project Overview
================================================================================

The application wizard has created this `fp` application for 
you.  This application not only demonstrates the basics of using the Microsoft 
Foundation Classes but is also a starting point for writing your application.

This file contains a summary of what you will find in each of the files that
make up your fp application.

`fp.vcproj`
    This is the main project file for VC++ projects generated using an application wizard. 
    It contains information about the version of Visual C++ that generated the file, and 
    information about the platforms, configurations, and project features selected with the
    application wizard.

`fp.h`
    This is the main header file for the application.  It includes other
    project specific headers (including `Resource.h`) and declares the
    `CfpApp` application class.

`fp.cpp`
    This is the main application source file that contains the application
    class `CfpApp`.

`fp.rc`
    This is a listing of all of the Microsoft Windows resources that the
    program uses.  It includes the icons, bitmaps, and cursors that are stored
    in the `RES` subdirectory.  This file can be directly edited in Microsoft
    Visual C++. Your project resources are in 1033.

`res\fp.ico`
    This is an icon file, which is used as the application's icon.  This
    icon is included by the main resource file `fp.rc`.

`res\fp.rc2`
    This file contains resources that are not edited by Microsoft 
    Visual C++. You should place all resources not editable by
    the resource editor in this file.

/////////////////////////////////////////////////////////////////////////////

The application wizard creates one dialog class:
`fpDlg.h`, `fpDlg.cpp` - the dialog
    These files contain your `CfpDlg` class.  This class defines
    the behavior of your application's main dialog.  The dialog's template is
    in `fp.rc`, which can be edited in Microsoft Visual C++.
/////////////////////////////////////////////////////////////////////////////


`StdAfx.h`, `StdAfx.cpp`
    These files are used to build a precompiled header (`.PCH`) file
    named `fp.pch` and a precompiled types file named `StdAfx.obj`.

`Resource.h`
    This is the standard header file, which defines new resource IDs.
    Microsoft Visual C++ reads and updates this file.

`fp.manifest`
	Application manifest files are used by Windows XP to describe an applications 
	dependency on specific versions of Side-by-Side assemblies. The loader uses this 
	information to load the appropriate assembly from the assembly cache or private 
	from the application. The Application manifest  maybe included for redistribution 
	as an external .manifest file that is installed in the same folder as the application 
	executable or it may be included in the executable in the form of a resource. 
/////////////////////////////////////////////////////////////////////////////

###Other notes:

The application wizard uses "TODO:" to indicate parts of the source code you
should add to or customize.

If your application uses MFC in a shared DLL, and your application is in a 
language other than the operating system's current language, you will need 
to copy the corresponding localized resources `MFC70XXX.DLL` from the Microsoft
Visual C++ CD-ROM under the Win\System directory to your computer's system or 
system32 directory, and rename it to be `MFCLOC.DLL`.  ("`XXX`" stands for the 
language abbreviation.  For example, `MFC70DEU.DLL` contains resources 
translated to German.)  If you don't do this, some of the UI elements of 
your application will remain in the language of the operating system.

/////////////////////////////////////////////////////////////////////////////
