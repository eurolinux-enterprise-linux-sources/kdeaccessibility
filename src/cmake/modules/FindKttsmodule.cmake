# Options for kttsd
# Copyright (c) 2006, Laurent Montel, <montel@kde.org>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.


# Note: since most of the synth modules run as K3Process, it is OK
# to build them, even if the corresponding executable is not available.
# If user later installs the executable, it will then just work.

OPTION(KDE4_KTTSD_COMMAND   "build KTTSD Command Plugin" ON)

OPTION(KDE4_KTTSD_EPOS   "build KTTSD Epos Plugin" ON)
if(KDE4_KTTSD_EPOS)
    FIND_PROGRAM(EPOS_EXECUTABLE NAMES epos say-epos)
    if(NOT EPOS_EXECUTABLE)
        MESSAGE(STATUS "The KTTSD Epos plugin will be built but the pos executable was not found.")
    endif(NOT EPOS_EXECUTABLE)
endif(KDE4_KTTSD_EPOS)

OPTION(KDE4_KTTSD_FESTIVAL   "build KTTSD Festival Interactive plugin" ON)
if(KDE4_KTTSD_FESTIVAL)
    FIND_PROGRAM(FESTIVAL_EXECUTABLE NAMES festival )
    if(NOT FESTIVAL_EXECUTABLE)
        MESSAGE(STATUS "The KTTSD Festival plugin will be built but the festival executable was not found.")
    endif(NOT FESTIVAL_EXECUTABLE)
endif(KDE4_KTTSD_FESTIVAL)

OPTION(KDE4_KTTSD_FLITE   "build KTTSD Festival Lite (flite)" ON)
if(KDE4_KTTSD_FLITE)
    FIND_PROGRAM(FLITE_EXECUTABLE NAMES flite )
    if(NOT FLITE_EXECUTABLE)
        MESSAGE(STATUS "The KTTSD Festival Lite (flite) plugin will be built but the flite executable was not found.")
    endif(NOT FLITE_EXECUTABLE)
endif(KDE4_KTTSD_FLITE)

OPTION(KDE4_KTTSD_FREETTS   "build KTTSD FreeTTS Plugin" ON)
if(KDE4_KTTSD_FREETTS)
    FIND_PROGRAM(FREETTS_EXECUTABLE NAMES freetts.jar freetts )
    if(NOT FREETTS_EXECUTABLE)
        MESSAGE(STATUS "The KTTSD FreeTTS plugin will be built but the freetts.jar file was not found.")
    endif(NOT FREETTS_EXECUTABLE)
endif(KDE4_KTTSD_FREETTS)

# Test if hadifix_txt2pho_bin and hadifix_mbrola_bin exists and /etc/txt2pho exists
OPTION(KDE4_KTTSD_HADIFIX   "build KTTSD Hadifix Plugin" ON)
if(KDE4_KTTSD_HADIFIX)
    FIND_PROGRAM(HADIFIX_EXECUTABLE NAMES txt2pho PATHS /usr/local/txt2pho)
    if(NOT HADIFIX_EXECUTABLE)
        MESSAGE(STATUS "The KTTSD Hadifix plugin will be built but the txt2pho executable was not found.")
    endif(NOT HADIFIX_EXECUTABLE)

    FIND_PROGRAM(HADIFIX_MBROLA_EXECUTABLE NAMES mbrola mbrola-linux-i386 PATHS /usr/local/mbrola /usr/mbrola)
    if(NOT HADIFIX_MBROLA_EXECUTABLE)
        MESSAGE(STATUS "The KTTSD Hadifix plugin will be built but the mbrola executable was not found.")
    endif(NOT HADIFIX_MBROLA_EXECUTABLE)
endif(KDE4_KTTSD_HADIFIX)
#TODO test files '/etc/txt2pho'

OPTION(KDE4_KTTSD_PHONON "Enable support for Phonon in KTTSD" ON)

OPTION(KDE4_KTTSD_ALSA "Enable support for ALSA in KTTSD" ON)
if(WITH_ALSA AND KDE4_KTTSD_ALSA)
    if(ALSA_FOUND)
        set(KDE4_KTTSD_ALSA ON)
    else(ALSA_FOUND)
        set(KDE4_KTTSD_ALSA OFF)
    endif(ALSA_FOUND)
endif(WITH_ALSA AND KDE4_KTTSD_ALSA)
if(NOT ALSA_FOUND)
    set(KDE4_KTTSD_ALSA OFF)
endif(NOT ALSA_FOUND)

