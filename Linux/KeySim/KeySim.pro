TEMPLATE = app
CONFIG += console c++17
QT = core
CONFIG(debug, debug|release) {
  message( "debug" )
#QMAKE_CXXFLAGS_DEBUG+="-fsanitize=address"
#QMAKE_LFLAGS_DEBUG+=" -fsanitize=address"
}



SOURCES += \
        main.cpp

LIBS += -lX11 -lXi -lXtst
