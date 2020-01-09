# Directories, include, lib,
!if defined(DEBUG_BUILD)
LIB_TYPE = Debug
EXECUTABLE = Graph_libd
BUILDDIR = lib\Debug
!else
LIB_TYPE = Release
EXECUTABLE = Graph_lib
BUILDDIR = lib\Release
!endif

TDT4102_DIR = "%ProgramFiles%/tdt4102"
FLTK_LIB_DIR = $(TDT4102_DIR)/FLTK-1.4/lib/$(LIB_TYPE)
# Somehow clang-cl is unaware of $env.INCLUDE
# Probably wrong SDK and MSVC version if this fails.
UCRT = "%ProgramFiles(x86)%\Windows Kits\10\Include\10.0.18362.0\ucrt"
INC = "%ProgramFiles(x86)%\Microsoft Visual Studio\2019\BuildTools\VC\Tools\MSVC\14.24.28314\include"
SHARED = "%ProgramFiles(x86)%\Windows Kits\10\include\10.0.18362.0\shared"
UM = "%ProgramFiles(x86)%\Windows Kits\10\include\10.0.18362.0\um"
WINRT = "%ProgramFiles(x86)%\Windows Kits\10\include\10.0.18362.0\winrt"
CPPWINRT = "%ProgramFiles(x86)%\Windows Kits\10\include\10.0.18362.0\cppwinrt"


FLTK_INCLUDE = $(TDT4102_DIR)/FLTK-1.4
INCLUDE = include

GRAPH_LIB_LIBRARIES = $(GRAPH_LIB_DIR)/lib/$(LIB_TYPE)/*.lib
FLTK_LIBRARIES = $(FLTK_LIB_DIR)/*.lib
WIN_LIBRARIES = wsock32.lib comctl32.lib kernel32.lib user32.lib gdi32.lib \
				winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib \
				oleaut32.lib uuid.lib odbc32.lib odbccp32.lib

# Compiler and linker config
CXX = clang-cl
CXXFLAGS = /std:c++17 -TP -m64 -fdiagnostics-absolute-paths /W4 /GR \
			/DEBUG /INCREMENTAL /nologo /EHsc \
			-I$(INCLUDE) \
			-I$(FLTK_INCLUDE) \
			/imsvc$(UCRT) \
			/imsvc$(INC) \
			/imsvc$(SHARED) \
			/imsvc$(UM) \
			/imsvc$(WINRT) \
			/imsvc$(CPPWINRT)
DEBUG = /Od /Ob0 -DWIN32 /MDd /Zi /RTC1
RELEASE = /O2 /Ob2 -DNDEBUG -DWIN32_LEAN_AND_MEAN /MD
MACROS = -D_WINDOWS -DNOGDI -D_CRT_SECURE_NO_WARNINGS
DEBUGLD = /DEBUG /INCREMENTAL /Zi
RELEASELD = /INCREMENTAL:NO
LDFLAGS = -m64 $(FLTK_LIBRARIES) $(WIN_LIBRARIES)
LINK = $(CXX)

!if defined(DEBUG_BUILD)
CXXFLAGS = $(CXXFLAGS) $(DEBUG)
LDFLAGS = $(LDFLAGS) $(DEBUGLD)
!else
CXXFLAGS = $(CXXFLAGS) $(RELEASE)
LDFLAGS = $(LDFLAGS) $(RELEASELD)
!endif

SOURCES = *.cpp
OBJECTS = $(BUILDDIR)/$(SOURCES:.cpp=.obj)

# Inference rules, etc.
.SUFFIXES: .obj .cpp

{src}.cpp{$(BUILDDIR)}.obj:
	if not exist "$(BUILDDIR)" mkdir $(BUILDDIR)
	$(CXX) $(CXXFLAGS) $(MACROS) /c $< /Fo$(@D)/

$(BUILDDIR)/$(EXECUTABLE).lib: $(OBJECTS)
	llvm-lib /out:$@ $(OBJECTS)
	@echo ...
	@echo Successfully built ./$@
	-del $(BUILDDIR)\*.obj

$(EXECUTABLE).lib: $(BUILDDIR)/$(EXECUTABLE).lib

clean:
	@echo Cleaning the output files for $(BUILDDIR)
	-rmdir /s /q $(BUILDDIR)

# Helper targets to ease build of debug and release versions
rebuild: debug release

debug:
	$(MAKE) "DEBUG_BUILD=1" clean
	$(MAKE) "DEBUG_BUILD=1" Graph_libd.lib

release:
	$(MAKE) clean
	$(MAKE) Graph_lib.lib
