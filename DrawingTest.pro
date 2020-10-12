QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    calculator.cpp \
    dnfanalyticalconstructor.cpp \
    dnfbruteforceconstructor.cpp \
    dnfenhancedbruteforceconstructor.cpp \
    graph.cpp \
    grapheventprobabilityprovider.cpp \
    intpair.cpp \
    linearlogicequation.cpp \
    logicequation.cpp \
    logicequationnode.cpp \
    main.cpp \
    mainwindow.cpp \
    recursivesearcher.cpp \
    renderarea.cpp \
    setutils.cpp

HEADERS += \
    calculator.h \
    dnfanalyticalconstructor.h \
    dnfbruteforceconstructor.h \
    dnfenhancedbruteforceconstructor.h \
    eventprobabilityprovider.h \
    graph.h \
    grapheventprobabilityprovider.h \
    graphnode.h \
    intpair.h \
    linearlogicequation.h \
    logicequation.h \
    logicequationnode.h \
    logicoperation.h \
    mainwindow.h \
    netparams.h \
    path.h \
    probabilityprovider.h \
    recursivesearcher.h \
    renderarea.h \
    searcherbase.h \
    setutils.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
