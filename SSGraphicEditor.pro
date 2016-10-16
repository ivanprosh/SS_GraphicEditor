TEMPLATE = app

# Подключаем заголовочные файлы библиотеки
INCLUDEPATH += lib/QtPropertyBrowser
DEPENDPATH += lib/QtPropertyBrowser

CONFIG(debug, debug|release) {
    # Подключаем debug-версии библиотек для разных платформ
    #win32: LIBS += -Ld:/WORK/_QT/SS/SSGraphicEditor/QtPropertyBrowser -lqtpropertybrowserd1
    win32: LIBS += -L$${PWD}/lib/QtPropertyBrowser -lqtpropertybrowserd1
    #unix: LIBS += -Llib -L. -lMyLibraryd -Wl,-rpath,lib -Wl,-rpath,.
} else {
    # Подключаем release-версии библиотек для разных платформ
    win32: LIBS += -L$${PWD}/lib/QtPropertyBrowser -lqtpropertybrowser1
    #unix: LIBS += -Llib -L. -lMyLibrary -Wl,-rpath,lib -Wl,-rpath,.
}

QT += widgets printsupport svg

# All sources
include($${PWD}/src/deployment.pri)

RESOURCES += \
    images.qrc
