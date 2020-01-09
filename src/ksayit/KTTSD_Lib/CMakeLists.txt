set(KTTSD_Lib_LIB_SRCS kttsdlib.cpp kttsdlibtalker2.cpp )

kde4_add_ui_files(KTTSD_Lib_LIB_SRCS KTTSDlibSetup.ui )

qt4_add_dbus_interfaces(KTTSD_Lib_LIB_SRCS ${KDE4_DBUS_INTERFACES_DIR}/org.kde.KSpeech.xml )

kde4_add_library(KTTSD_Lib SHARED ${KTTSD_Lib_LIB_SRCS})

target_link_libraries(KTTSD_Lib
    ${KDE4_KDECORE_LIBS} ${KDE4_KDE3SUPPORT_LIBS}
    ${KDE4_KDEUI_LIBS} )

set_target_properties(KTTSD_Lib PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION} )

install(TARGETS KTTSD_Lib  ${INSTALL_TARGETS_DEFAULT_ARGS} )

