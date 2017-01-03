#CONFIG += console debug
TEMPLATE = app
#rus/eng
TRANSLATIONS = $${PWD}/_ru.ts

# ���������� ������������ ����� ����������
INCLUDEPATH += lib/QtPropertyBrowser
DEPENDPATH += lib/QtPropertyBrowser

CONFIG(debug, debug|release) {
    # ���������� debug-������ ��������� ��� ������ ��������
    win32: LIBS += -L$${PWD}/lib/QtPropertyBrowser -lqtpropertybrowserd1
    #LIBS += -L$$QTPROPERTYBROWSER_LIBDIR -l$$QTPROPERTYBROWSER_LIBNAME
} else {
    # ���������� release-������ ��������� ��� ������ ��������
    win32: LIBS += -L$${PWD}/lib/QtPropertyBrowser -lqtpropertybrowser1
}

QT += widgets printsupport svg

# All sources
include($${PWD}/src/deployment.pri)

DEFINES += ANIMATE_ALIGNMENT ALTERNATIVE_RESIZING
DEFINES += QT_QTPROPERTYBROWSER_IMPORT

RESOURCES += \
    images.qrc \
    src/aqp/aqp.qrc \
    translations/translations.qrc

DISTFILES +=

