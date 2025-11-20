QT += core gui widgets sql network

CONFIG += c++17

TARGET = FlightSystem
TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    flightsearchwidget.cpp \
    flightbookingwidget.cpp \
    usermanagementwidget.cpp \
    flightdetailswidget.cpp \
    apimanager.cpp \
    databasehelper.cpp \
    customwidgets.cpp

HEADERS += \
    mainwindow.h \
    flightsearchwidget.h \
    flightbookingwidget.h \
    usermanagementwidget.h \
    flightdetailswidget.h \
    apimanager.h \
    databasehelper.h \
    customwidgets.h

FORMS += \
    mainwindow.ui \
    flightsearchwidget.ui \
    flightbookingwidget.ui \
    usermanagementwidget.ui \
    flightdetailswidget.ui

RESOURCES += \
    resources.qrc

# Windows specific
win32 {
    RC_ICONS = icons/flight_icon.ico
}

# Linux specific
unix:!macx {
    target.path = /usr/local/bin
    INSTALLS += target
}