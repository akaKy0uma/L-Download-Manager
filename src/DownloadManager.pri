CONFIG(debug, debug|release) {
    APP_OUTPUT_DIR = $$clean_path($$PWD/build/debug)
} else {
    APP_OUTPUT_DIR = $$clean_path($$PWD/build/release)
}

APP_PLUGIN_OUTPUT_DIR = $$APP_OUTPUT_DIR/plugins
APP_LIBRARY_OUTPUT_DIR = $$APP_OUTPUT_DIR

LIBS += -L$$APP_OUTPUT_DIR -L$$APP_PLUGIN_OUTPUT_DIR

INCLUDEPATH += $$PWD/public

win32:DEFINES += _CRT_SECURE_NO_WARNINGS
