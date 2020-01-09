//
// C++ Implementation: effectstack
//
// Description: 
//
//
// Author: Robert Vogl <voglrobe@lapislazuli>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//

// Qt includes

// KDE includes
#include <kdebug.h>

// App specific includes
#include "effectstack.h"
#include "fxpluginhandler.h"

EffectStack::EffectStack(FXPluginHandler *pluginhandler, KConfig *config)
    : 
     dispatcher(), server(),
     m_pluginhandler(pluginhandler),
     m_config(config)
{
}


EffectStack::~EffectStack()
{
}

bool EffectStack::loadEffects()
{
    kDebug(100200) << "EffectStack::loadEffects()";
    
    QStringList c_active;
    QStringList::Iterator fx_it;
  
    // create filterstack
    FX_Stack = server.server().outstack();
    
    m_config->setGroup("Effect Stack Configuration");
    c_active = m_config->readEntry("Activated",QStringList());
    for(fx_it=c_active.begin(); fx_it!=c_active.end(); ++fx_it){
        
        m_pluginhandler->activateEffect(*fx_it, &server, &FX_Stack);
        
    }
    return true;
}


bool EffectStack::unloadEffects()
{
    kDebug(100200) << "EffectStack::unloadEffects()";
    // remove all effects    
    m_pluginhandler->deactivateEffects(&FX_Stack);
    
    return true;
}

