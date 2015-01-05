rem ***************************************************************
rem  * Name:  	 MakeWin.bat
rem  * Purpose:
rem  * Author:    LETARTARE
rem  * Created:   2014-20-08
rem  * Copyright: LETARTARE
rem  * License:   GPL
rem  **************************************************************

rem  GCV-Tx

rem for release

qmake -config release -spec win32-g++ GCV.pro
mingw32-make -fmakefile.release

xcopy /Y  %PATH_MINGW32%bin\libstdc++-6.dll  bin\
xcopy /Y  %PATH_QT%bin\QGLViewer2.dll   bin\


rem for debug

qmake -config debug -spec win32-g++ GCV.pro
mingw32-make -fmakefile.debug

echo on 
xcopy /Y  %PATH_MINGW32%bin\libstdc++-6.dll  bin\debug
xcopy /Y  %PATH_QT%bin\QGLViewerd2.dll   bin\debug

cd ..
