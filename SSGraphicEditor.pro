TEMPLATE = app

# ���������� ������������ ����� ����������
INCLUDEPATH += lib/QtPropertyBrowser
DEPENDPATH += lib/QtPropertyBrowser

CONFIG(debug, debug|release) {
    # ���������� debug-������ ��������� ��� ������ ��������
    #win32: LIBS += -Ld:/WORK/_QT/SS/SSGraphicEditor/QtPropertyBrowser -lqtpropertybrowserd1
    win32: LIBS += -L$${PWD}/lib/QtPropertyBrowser -lqtpropertybrowserd1
    #unix: LIBS += -Llib -L. -lMyLibraryd -Wl,-rpath,lib -Wl,-rpath,.
} else {
    # ���������� release-������ ��������� ��� ������ ��������
    win32: LIBS += -L$${PWD}/lib/QtPropertyBrowser -lqtpropertybrowser1
    #unix: LIBS += -Llib -L. -lMyLibrary -Wl,-rpath,lib -Wl,-rpath,.
}

QT += widgets printsupport svg

# All sources
include($${PWD}/src/deployment.pri)

RESOURCES += \
    images.qrc
