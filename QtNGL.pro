# This specifies the exe name
TARGET=QtNGL
# where to put the .o files
OBJECTS_DIR=obj
# core Qt Libs to use add more here if needed.
QT+=gui opengl core

cache()

# where to put moc auto generated files
MOC_DIR=moc
# on a mac we don't create a .app bundle file ( for ease of multiplatform use)
CONFIG-=app_bundle
# Auto include all .cpp files in the project src directory (can specifiy individually if required)
SOURCES+= $$PWD/src/NGLScene.cpp \
          $$PWD/src/NGLSceneMouseControls.cpp \
					$$PWD/src/MainWindow.cpp \
					$$PWD/src/main.cpp
# same for the .h files
HEADERS+= $$PWD/include/NGLScene.h \
          $$PWD/include/MainWindow.h \
          $$PWD/include/WindowParams.h


FORMS+= $$PWD/ui/MainWindow.ui
# and add the include dir into the search path for Qt and make
INCLUDEPATH +=./include
# where our exe is going to live (root of project)
DESTDIR=./
# add the glsl shader files
OTHER_FILES+= README.md \
              shaders/*.glsl
# were are going to default to a console app
CONFIG += console
NGLPATH=$$(NGLDIR)
isEmpty(NGLPATH){ # note brace must be here
        message("including $HOME/NGL")
	include($(HOME)/NGL/UseNGL.pri)
         win32:include($(HOMEDRIVE)\$(HOMEPATH)\NGL\UseNGL.pri)

}
else{ # note brace must be here
	message("Using custom NGL location")
	include($(NGLDIR)/UseNGL.pri)
}


