unix:!android {
    isEmpty(target.path) {
        qnx {
            target.path = /tmp/$${TARGET}/bin
        } else {
            target.path = /opt/$${TARGET}/bin
        }
        export(target.path)
    }
    INSTALLS += target
}

android {
    message( "My android build..." )
    deployment.path = /assets
    deployment.files += database/RussianCook.sqlite \

    images.path = /assets/images
    images.files = images/*
    content.path =  /assets/content
    content.files = content/*
    INSTALLS += images
    INSTALLS += content


} else {
    message( "My Win build in...")
    message($$OUT_PWD )
    deployment.path = $$OUT_PWD/
    #deployment.files += database/RussianCook.sqlite \
}

INSTALLS += deployment

export(INSTALLS)

HEADERS += \
    $$PWD/aqp/alt_key.hpp \
    $$PWD/aqp/aqp.hpp \
    $$PWD/aqp/kuhn_munkres.hpp \
    $$PWD/itemtypes.hpp \
    $$PWD/smileyitem.hpp \
    $$PWD/swatch.hpp \
    $$PWD/textitem.hpp \
    $$PWD/textitemdialog.hpp \
    $$PWD/mainwindow.hpp \
    $$PWD/boxitem.hpp \
    $$PWD/graphicsview.hpp \
    $$PWD/textedit.hpp \
    $$PWD/global.hpp \
    $$PWD/propmanager.h \
    $$PWD/SSobjects/ssindicator.h \
    $$PWD/SSobjects/ssitemdialog.h \
    $$PWD/SSobjects/standardtablemodel.hpp \
    $$PWD/SSobjects/proxymodel.hpp \
    $$PWD/SSobjects/templateimage.h \
    $$PWD/SSobjects/imagemodeldelegate.hpp \
    $$PWD/SSobjects/fileedit.h \
    $$PWD/SSobjects/templmodelinfo.h \
    $$PWD/SSobjects/checkboxdelegate.h \
    $$PWD/SSobjects/uniqueproxymodel.hpp \
    $$PWD/commands.h \
    $$PWD/graphicscene.h

SOURCES += \
    $$PWD/main.cpp \
    $$PWD/aqp/alt_key.cpp \
    $$PWD/aqp/aqp.cpp \
    $$PWD/aqp/kuhn_munkres.cpp \
    $$PWD/swatch.cpp \
    $$PWD/textedit.cpp \
    $$PWD/textitem.cpp \
    $$PWD/textitemdialog.cpp \
    $$PWD/boxitem.cpp \
    $$PWD/mainwindow.cpp \
    $$PWD/smileyitem.cpp \
    $$PWD/global.cpp \
    $$PWD/propmanager.cpp \
    $$PWD/SSobjects/ssindicator.cpp \
    $$PWD/SSobjects/ssitemdialog.cpp \
    $$PWD/SSobjects/standardtablemodel.cpp \
    $$PWD/SSobjects/proxymodel.cpp \
    $$PWD/SSobjects/templateimage.cpp \
    $$PWD/SSobjects/imagemodeldelegate.cpp \
    $$PWD/SSobjects/fileedit.cpp \
    $$PWD/SSobjects/checkboxdelegate.cpp \
    $$PWD/SSobjects/uniqueproxymodel.cpp \
    $$PWD/commands.cpp \
    $$PWD/graphicscene.cpp

