import os
import platform

OS_NAME=platform.system()
TK_ROOT = os.path.normpath(os.getcwd())
TK_SRC = os.path.join(TK_ROOT, 'src')
TK_3RD_ROOT = os.path.join(TK_ROOT, '3rd')
TK_TOOLS_ROOT = os.path.join(TK_ROOT, 'tools')
GTEST_ROOT = os.path.join(TK_ROOT, '3rd/gtest/googletest')
BIN_DIR=os.path.join(TK_ROOT, 'bin')
LIB_DIR=os.path.join(TK_ROOT, 'lib')

if OS_NAME == 'Windows':
  LCD='NANOVG'
else:
  LCD='SDL'
  LCD='NANOVG'
  VGCANVAS='AGG'
  VGCANVAS='PICASSO'

COMMON_CCFLAGS=' -DTK_ROOT=\\\"'+TK_ROOT+'\\\" -DHAS_STD_MALLOC -DSDL2'
COMMON_CCFLAGS=COMMON_CCFLAGS+' -DLUA_COMPAT_MODULE -DSTBTT_STATIC -DSTB_IMAGE_STATIC -DWITH_STB_IMAGE -DWITH_STB_FONT -DWITH_DYNAMIC_TR'

if LCD == 'NANOVG':
  VGCANVAS='NANOVG'
  COMMON_CCFLAGS = COMMON_CCFLAGS + ' -DWITH_NANOVG -DWITH_GL3'
else:
  if VGCANVAS == 'AGG':
    COMMON_CCFLAGS = COMMON_CCFLAGS + ' -DWITH_AGG'
  elif VGCANVAS == 'PICASSO':
    COMMON_CCFLAGS = COMMON_CCFLAGS + ' -DWITH_PICASSO -DIMAGE_RGBA'


os.environ['LCD'] = LCD
os.environ['VGCANVAS'] =VGCANVAS 
os.environ['BIN_DIR'] = BIN_DIR;
os.environ['LIB_DIR'] = LIB_DIR;
os.environ['TK_ROOT'] = TK_ROOT;
os.environ['GTEST_ROOT'] = GTEST_ROOT;

OS_LIBPATH=[]
OS_CPPPATH=[]
OS_FLAGS='-g -Wall'
OS_SUBSYSTEM_CONSOLE=''
OS_SUBSYSTEM_WINDOWS=''
OS_LINKFLAGS=''
OS_LIBS=['SDL2', 'glad', 'stdc++', 'pthread', 'm']

if OS_NAME == 'Darwin':
  OS_LINKFLAGS='-framework OpenGL'
  COMMON_CCFLAGS = COMMON_CCFLAGS + ' -DLUA_USE_POSIX -D__APPLE__ -DWITH_FS_RES'
  if VGCANVAS == 'PICASSO':
    OS_LIBS = ['freetype'] + OS_LIBS
    COMMON_CCFLAGS = COMMON_CCFLAGS + ' -DENABLE_FREE_TYPE2=1 -DFONT_FILE_NAME=\\\"'+TK_ROOT+'/demos/res/raw/fonts/default_ttf.ttf\\\"'

elif OS_NAME == 'Linux':
  OS_LIBS = ['GL'] + OS_LIBS + ['dl']
  COMMON_CCFLAGS = COMMON_CCFLAGS + ' -DLUA_USE_POSIX -DLINUX'
  if VGCANVAS == 'PICASSO':
    OS_LIBS = ['freetype'] + OS_LIBS
    COMMON_CCFLAGS = COMMON_CCFLAGS + ' -DENABLE_FREE_TYPE2=1 -DFONT_FILE_NAME=\\\"'+TK_ROOT+'/demos/res/raw/fonts/default_ttf.ttf\\\"'

elif OS_NAME == 'Windows':
  OS_LIBS=['SDL2', 'glad']
  OS_FLAGS='-DWIN32 -D_WIN32 -DWINDOWS /EHsc -D_CONSOLE  /DEBUG -DUNICODE -D_UNICODE /Od /ZI'
  OS_LINKFLAGS='/MACHINE:X64 /DEBUG'
  OS_LIBPATH=[TK_3RD_ROOT+'/SDL2-2.0.7/lib/x64']
  OS_CPPPATH=[TK_3RD_ROOT+'/SDL2-2.0.7/']
  OS_SUBSYSTEM_CONSOLE='/SUBSYSTEM:CONSOLE  '
  OS_SUBSYSTEM_WINDOWS='/SUBSYSTEM:WINDOWS  '
  
LIBS=['awtk', 'picasso', 'agg', 'nanovg'] + OS_LIBS

CCFLAGS=OS_FLAGS + COMMON_CCFLAGS 
CPPPATH=[TK_ROOT, 
  TK_SRC, 
  TK_3RD_ROOT, 
  os.path.join(TK_3RD_ROOT, 'nanovg/src'), 
  os.path.join(TK_3RD_ROOT, 'agg-2.5/include'), 
  os.path.join(TK_3RD_ROOT, 'picasso/src'), 
  os.path.join(TK_3RD_ROOT, 'picasso/build'), 
  os.path.join(TK_3RD_ROOT, 'picasso/include'), 
  os.path.join(TK_3RD_ROOT, 'picasso/src/gfx'), 
  os.path.join(TK_3RD_ROOT, 'picasso/src/include'), 
  os.path.join(TK_3RD_ROOT, 'freetype2/include'), 
  TK_TOOLS_ROOT] + OS_CPPPATH

DefaultEnvironment(CCFLAGS = CCFLAGS, 
  CPPPATH = CPPPATH,
  LIBS=LIBS,
  LINKFLAGS=OS_LINKFLAGS,
  OS_SUBSYSTEM_CONSOLE=OS_SUBSYSTEM_CONSOLE,
  OS_SUBSYSTEM_WINDOWS=OS_SUBSYSTEM_WINDOWS,
  LIBPATH=[os.path.join(TK_ROOT, 'lib')] + OS_LIBPATH)

SConscript([
  '3rd/agg-2.5/SConscript',
  '3rd/nanovg/SConscript',
  '3rd/picasso/SConscript',
  '3rd/glad/SConscript',
  'src/SConscript',
  'tools/common/SConscript', 
  'tools/theme_gen/SConscript', 
  'tools/font_gen/SConscript', 
  'tools/image_gen/SConscript', 
  'tools/res_gen/SConscript', 
  'tools/str_gen/SConscript', 
  'tools/ui_gen/xml_to_ui/SConscript',
  'demos/SConscript', 
  'tests/SConscript',
  '3rd/lua/SConscript',
  'lua/SConscript'
  ])
