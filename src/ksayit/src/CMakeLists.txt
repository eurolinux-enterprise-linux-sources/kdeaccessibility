
include_directories( 
    ${CMAKE_SOURCE_DIR}/ksayit/KTTSD_Lib
    ${CMAKE_BINARY_DIR}/ksayit/KTTSD_Lib  )


########### next target ###############

set(ksayit_SRCS 
   doctreeviewimpl.cpp 
   ksayitsystemtray.cpp 
   ksayitviewimpl.cpp 
   main.cpp 
   voicesetupdlg.cpp 
   ksayit.cpp 
   parasaxparser.cpp 
   saxhandler.cpp 
   ksayitbookmarkhandler.cpp 
   contextmenuhandler.cpp 
   docbookclasses.cpp )

#   effectstack.cpp 
#   fxpluginhandler.cpp 
#   fxsetupimpl.cpp 


kde4_add_ui3_files(ksayit_SRCS DocTreeView.ui KSayItView.ui )

# fx_setup.ui

# kde4_add_dcop_skels(ksayit_SRCS ksayit.h )

kde4_add_executable(ksayit ${ksayit_SRCS})

target_link_libraries(ksayit 
    ${KDE4_KDECORE_LIBS}
    ${KDE4_KDEUI_LIBS}
    ${KDE4_KIO_LIBS}
    KTTSD_Lib )
#    kio )
#    artskde
#    soundserver_idl
#    artsflow )

install(TARGETS ksayit  ${INSTALL_TARGETS_DEFAULT_ARGS} )


########### install files ###############

install( FILES ksayit.desktop  DESTINATION  ${XDG_APPS_INSTALL_DIR} )
install( FILES ksayitui.rc ContextMenus.xml  DESTINATION  ${DATA_INSTALL_DIR}/ksayit )
install( FILES ksayit_fxplugin.h DESTINATION ${INCLUDE_INSTALL_DIR})

kde4_install_icons( ${ICON_INSTALL_DIR}   )

