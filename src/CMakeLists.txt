project(kdeaccessibility)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules )

# search packages used by KDE
find_package(KDE4 REQUIRED)
include (KDE4Defaults)
include (MacroLibrary)

if(Q_WS_X11)
  if(NOT X11_XTest_FOUND)
    macro_log_feature(X11_XTest_FOUND "libXtst" "X11 Testing Resource extension library" "http://xorg.freedesktop.org" FALSE "" "needed to build kmousetool.")
  endif(NOT X11_XTest_FOUND)
endif(Q_WS_X11)

macro_optional_find_package(Alsa)
macro_log_feature(ALSA_FOUND "ALSA" "The Advanced Linux Sound Architecture (ALSA) library" "http://www.alsa-project.org" FALSE "0.9" "Provides the ALSA audio plugin for KTTSD.")
# KttsModule must be after the ALSA check
find_package(Kttsmodule)

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include_directories (${KDE4_INCLUDES})

macro_optional_add_subdirectory(ColorSchemes)
macro_optional_add_subdirectory(IconThemes)

if(Q_WS_X11)
    macro_optional_add_subdirectory(kttsd)
    
    macro_optional_add_subdirectory(kmag)
    if(X11_XTest_FOUND)
        macro_optional_add_subdirectory(kmousetool)
    endif(X11_XTest_FOUND)
endif(Q_WS_X11)

macro_optional_add_subdirectory(kmouth)

message(STATUS "Skipping ksayit. It does not currently compile.")
# add_subdirectory(ksayit)

macro_optional_add_subdirectory(doc)
macro_display_feature_log()
