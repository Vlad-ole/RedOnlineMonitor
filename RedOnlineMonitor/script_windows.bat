REM script for Windows
REM
REM rootcint creates dictionaries
rootcint -f mainDic.cpp -c  mymainframe.h fithist.h vinogradovpdf.h linkdef.h
REM create cmd window with special enviroment for cl.exe (it is Visual Studio compiler)
REM compiler keys for cl.exe. Remember, that such keys should be the same as you used to build root cern libs
REM You can see all the flags in $(ROOTSYS)/include/compiledata.h
call "C:\Program Files (x86)\Microsoft Visual Studio 11.0\VC\vcvarsall.bat" x86 & cl ^
-nologo -Z7 -MD -GR -EHsc ^
-I"C:\root_v5.34.36\include" -FIw32pragma.h main.cpp mymainframe.cpp mymainframe_readoutloop.cpp ^
mymainframe_signal_slot.cpp mymainframe_analysis.cpp fithist.cpp vinogradovpdf.cpp mainDic.cpp   ^
/link -debug -LIBPATH:C:\root_v5.34.36\lib complexDict.lib dequeDict.lib freetype.lib ^
libAfterImage.lib libASImage.lib libASImageGui.lib libCint.lib libCintex.lib libCore.lib ^
libEG.lib libEGPythia8.lib libEve.lib libFitPanel.lib libFoam.lib libFTGL.lib libFumili.lib ^
libGdml.lib libGed.lib libGenetic.lib libGenVector.lib libGeom.lib libGeomBuilder.lib libGeomPainter.lib ^
libGLEW.lib libGpad.lib libGraf.lib libGraf3d.lib libGui.lib libGuiBld.lib libGuiHtml.lib libGviz3d.lib ^
libHist.lib libHistPainter.lib libHtml.lib libMathCore.lib libMathMore.lib libMatrix.lib libMinuit.lib ^
libMinuit2.lib libMLP.lib libNet.lib libPhysics.lib libPostscript.lib libProof.lib libProofDraw.lib ^
libProofPlayer.lib libPyROOT.lib libQuadp.lib libRecorder.lib libReflex.lib libReflexDict.lib libRGL.lib ^
libRHTTP.lib libRint.lib libRIO.lib libRODBC.lib libRooFit.lib libRooFitCore.lib libRooStats.lib libRootAuth.lib ^
libSessionViewer.lib libSmatrix.lib libSpectrum.lib libSpectrumPainter.lib libSPlot.lib libSQLIO.lib libTable.lib ^
libThread.lib libTMVA.lib libTree.lib libTreePlayer.lib libTreeViewer.lib libUnuran.lib libVMC.lib libWin32gdk.lib ^
libXMLIO.lib listDict.lib map2Dict.lib mapDict.lib mathtext.lib multimap2Dict.lib multimapDict.lib multisetDict.lib ^
setDict.lib vectorDict.lib /out:main.exe
pause
