project(ksayit)
add_definitions (-DQT3_SUPPORT -DQT3_SUPPORT_WARNINGS)

add_subdirectory( KTTSD_Lib ) 
add_subdirectory( src ) 

message(STATUS "${CMAKE_CURRENT_SOURCE_DIR}: skipped subdir $(freeverbplugin_DIR)")



########### install files ###############





#original Makefile.am contents follow:

#if include_ksayit_audio_plugins
#  freeverbplugin_DIR = Freeverb_plugin
#endif
#
#SUBDIRS = KTTSD_Lib src $(freeverbplugin_DIR)
