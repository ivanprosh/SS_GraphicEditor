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
    $$PWD/global.hpp \
    $$PWD/itemtypes.hpp \
    $$PWD/smileyitem.hpp \
    $$PWD/swatch.hpp \
    $$PWD/textitem.hpp \
    $$PWD/textitemdialog.hpp \
    $$PWD/mainwindow.hpp \
    $$PWD/boxitem.hpp

SOURCES += \
    $$PWD/mainwindow.cpp \
    $$PWD/aqp/alt_key.cpp \
    $$PWD/aqp/aqp.cpp \
    $$PWD/aqp/kuhn_munkres.cpp \
    $$PWD/aqp/main.cpp \
    $$PWD/global.cpp \
    $$PWD/main.cpp \
    $$PWD/smileyitem.cpp \
    $$PWD/swatch.cpp \
    $$PWD/textitem.cpp \
    $$PWD/textitemdialog.cpp \
    $$PWD/boxitem.cpp
