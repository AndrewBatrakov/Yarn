#-------------------------------------------------
#
# Project created by QtCreator 2015-03-14T09:51:41
#
#-------------------------------------------------

QT       += gui\
            sql\
            network\
            widgets\

TARGET = Yarn
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \

HEADERS  += mainwindow.h \


include(Services.pri)
include(SQLTable.pri)

TRANSLATIONS =  Yarn_ru.ts

RESOURCES += \
    resource.qrc

RC_FILE = Yarn.rc


FORMS +=

DISTFILES += \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

