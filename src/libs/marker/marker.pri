# ADD TO EACH PATH $$PWD VARIABLE!!!!!!
# This need for corect working file translations.pro

HEADERS += \
    $$PWD/stable.h \

SOURCES += \

*msvc*:SOURCES += $$PWD/stable.cpp
