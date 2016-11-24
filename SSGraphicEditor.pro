#CONFIG += console debug
TEMPLATE = app

# Подключаем заголовочные файлы библиотеки
INCLUDEPATH += lib/QtPropertyBrowser
DEPENDPATH += lib/QtPropertyBrowser

CONFIG(debug, debug|release) {
    # Подключаем debug-версии библиотек для разных платформ
    win32: LIBS += -L$${PWD}/lib/QtPropertyBrowser -lqtpropertybrowserd1
    #LIBS += -L$$QTPROPERTYBROWSER_LIBDIR -l$$QTPROPERTYBROWSER_LIBNAME
} else {
    # Подключаем release-версии библиотек для разных платформ
    win32: LIBS += -L$${PWD}/lib/QtPropertyBrowser -lqtpropertybrowser1
}

QT += widgets printsupport svg

# All sources
include($${PWD}/src/deployment.pri)

DEFINES += ANIMATE_ALIGNMENT
DEFINES += QT_QTPROPERTYBROWSER_IMPORT

RESOURCES += \
    images.qrc \
    src/aqp/aqp.qrc

