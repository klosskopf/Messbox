SOURCES += \
    control.cpp \
    decoder.cpp \
    gpio.cpp \
    graphersteller.cpp \
    karte.cpp \
    karte_gui.cpp \
    main.cpp \
    mainwindow.cpp \
    paket.cpp \
    parameter.cpp \
    parameterauswahl.cpp \
    post.cpp \
    rechenblock.cpp \
    rechenbloecke.cpp \
    rechenfeld.cpp

HEADERS += \
    daten.h \
    decoder.h \
    gpio.h \
    graphersteller.h \
    karte.h \
    karte_gui.h \
    mainwindow.h \
    paket.h \
    parameter.h \
    parameterauswahl.h \
    post.h \
    rechenblock.h \
    rechenbloecke.h \
    rechenfeld.h \
    control.h

QT += widgets
QT += charts

DISTFILES +=

RESOURCES +=

LIBS += -lwiringPi
