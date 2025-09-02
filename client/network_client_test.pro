QT += core network
QT -= gui

CONFIG += c++17 console
CONFIG -= app_bundle

TARGET = network_client_test
TEMPLATE = app

SOURCES += \
    test_network_client.cpp \
    networkmanager_client.cpp

HEADERS += \
    networkmanager_client.h

# 输出目录
DESTDIR = ../build-test/

# 临时文件目录
OBJECTS_DIR = ../build-test/obj/
MOC_DIR = ../build-test/moc/

# 编译选项
QMAKE_CXXFLAGS += -Wall -Wextra

# 调试信息
CONFIG(debug, debug|release) {
    QMAKE_CXXFLAGS += -g
    TARGET = $$join(TARGET,,,_debug)
}
