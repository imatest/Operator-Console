Operator Console July 8, 2013

==========
WHAT'S NEW
==========
This version supports live capture from a DirectShow camera, using the ESCAPI dll.

The contents of the Log Window gets saved to a text file (set by LOG_FILENAME)

Calls the blemish and SFRPlus are done using data in RGB format


=====================
KNOWN PROBLEMS/ISSUES
=====================
The image display in the dialog automatically resizes at runtime based on the width and height of the image, and
in some cases the display doesn't get positioned nicely due to some problem with MFC.  This is under investigation.

The ESCAPI (Extremely Simple Capture API) dll is a free DLL for doing simple DirectShow captures.  It does 
not come with source code, and has no error handling.

If the camera is disconnected during live capture, the application will lock up and must be ended via the Task
Manager (Ctrl-Alt-Delete).  This is due to the lack of error handling in the ESCAPI dll.

When testing with the Logitech C110 webcam, both sfrplus and blemish return errors.  To get all
of the error messages from the Imatest libraries, run hte program via load.exe (see below).


===================
RUNNING THE PROGRAM
===================
These files must be in the same directory as OperatorConsole.exe:
escapi.dll
filechck.dll
imatest_library.dll
load.exe (only needed if you want to get console output from the Imatest libraries, see RDIRECT_STDIO below)
md5.exe

To redirect the console output of the Imatest DLLs to the Operator Console's Log Window,  the application must
be run by launching load.exe.  This sets up the redirection and launches OperatorConsole.exe.


====================
BUILDING THE PROJECT
====================
This is a Visual Studio 2012 project.  Depending on where Imatest IT is installed on your computer,
you may have to modify some project propeties (In the PROJECT menu, select Properties):

C++
   General
      Additional Include directories

Linker
   General
      Additional Library directories
   Input
      Additional dependencies

The program currently uses several hard coded values.  These are all defined in OperatorConsole.h.
To run the program on your computer, open OperatorConsole.h and change these values:

IMAGE_NAME
RAW_BLEMISH_IMAGE
RAW_SFRPLUS_IMAGE
RAW_WIDTH_BLEMISH
RAW_HEIGHT_BLEMISH
RAW_WIDTH_SFRPLUS
RAW_HEIGHT_SFRPLUS

INI_FILENAME
PROGRAMPATH
EXTENSION
FILE_ROOT
SERIAL_NUMBER
PART_NUMBER

CAMERA_WIDTH
CAMERA_HEIGHT


The following definitions are in OperatorConsole.cpp

REDIRECT_STDIO enables the display of console messages (stdout, stderr, cout, cerr) in the Log Window.  
To get console messages from the Imaetest libs, launch the program by double-clicking on load.exe, which 
will enable the library console output to be redirected to the log window (load.exe will launch 
OperatorConsole.exe; both exe files must be in the same directory).

USE_RGB_DATA tells the application that the image data is in RGB format rather than RAW format

LOG_FILENAME this is the name of the file to which the contents of the log window will be saved when the program exits
