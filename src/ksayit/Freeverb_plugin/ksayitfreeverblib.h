//
// C++ Interface: ksayitfreeverblib
//
// Description: 
//
//
// Author: Robert Vogl <voglrobe@lapislazuli>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef KSAYITFREEVERBLIB_H
#define KSAYITFREEVERBLIB_H

// QT includes
#include <QtCore/QObject>

// KDE includes
#include <ksimpleconfig.h>

#include "ksayit_fxplugin.h"

class FreeverbPlugin : public QObject, public FXPlugin
{
Q_OBJECT
public: 
    // Constructor
    FreeverbPlugin(QObject *parent=0, const char* name=0, KApplication *Appl=0);
    
    // Destructor
    ~FreeverbPlugin();    
    
    /** returns the Name of the Plugin
    */
    QString getName_KS() const;
    
    /** returns a description of the plugin
    */
    QString getDescription_KS() const;
    
    /** shows the GUI to configure the plugin
    */
    bool showGUI_KS();
    
    /** activate the effect
    */
    long activate_KS(KArtsServer *server,
                             StereoEffectStack *fx_stack) const;
    
    /** deactivates the effect
    */
    bool deactivate_KS(StereoEffectStack *fx_stack,
                               long EffectID ) const;
                               

private:
    KApplication *m_Appl;
    KSimpleConfig *m_config;

};
    


#endif

