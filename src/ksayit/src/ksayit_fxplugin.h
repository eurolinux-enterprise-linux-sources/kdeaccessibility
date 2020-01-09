//
// C++ Interface: fxplugin
//
// Description: 
//
//
// Author: Robert Vogl <voglrobe@lapislazuli>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef FXPLUGIN_H
#define FXPLUGIN_H

// QT includes
#include <QtCore/QObject>

// KDE includes
#include <arts/kartsserver.h>
#include <arts/artsflow.h>

using namespace Arts;

// App specific includes

/** FXPlugin is an abstract class to create effect plugins for KSayIt.
 *  If you would like to implement a plugin, simply make a class
 *  derived from FXPlugin, include 'fxplugin.h' and reimplement all the
 *  pure virtual functions provided herein.
 *  In addition you must implement two class factories.\n
 *  \p createPlugin(KApplication *Appl) returns a pointer to an instance of your plugin.
 *  *Appl is a pointer to the main application as delivered by \p KApplication::kApplication().
 *  It can be used for any reason i.e. to install a Qt translator.\n
 *  \p destroyPlugin() takes this pointer to destroy an instance of your plugin.\n
 *  Example:
 \code
    extern "C"
    {
        FXPlugin* createPlugin(KApplication *Appl)
        {
            return new MyNewPlugin(Appl);
        }
        
        void destroyPlugin(FXPlugin* p)
        {
            delete p;
        }
    };
 \endcode
 \author Robert Vogl
 */
class FXPlugin
{
public:
    FXPlugin(){};
  	virtual ~FXPlugin(){}; 
    /** Returns the name of the plugin. This name is the unique identifier
     *  for the plugin. A expressive name is recommended because this name
     *  may be shown to the user, i.e. in a configuration dialog.
     *  The PluginHandler internally references to each effect plugin by this name.\n
     *  Has to be reimplemented by the plugin implementation.
     */
    virtual QString getName_KS() const = 0;
    
    /** Returns the description of the plugin.\n
     *  Has to be reimplemented by the plugin implementation.
     */
    virtual QString getDescription_KS() const = 0;
    
    /** Shows the GUI to configure the plugin. The configuration can to be
     *  stored in the global configuration file of KSayIt.\n
     *  Has to be reimplemented by the plugin implementation.
     */
    virtual bool showGUI_KS() = 0;
    
    /** Activates the effect i.e. puts it on the soundservers stack.\n
     *  Returns the ID of the activated effect.
     *  Has to be reimplemented by the plugin implementation.
    \param server A pointer to the aRts soundserver instance.
    \param fx_stack A pointer to the effect stack of the soundserver.
     */ 
    virtual long activate_KS(KArtsServer *server,
                             StereoEffectStack *fx_stack) const = 0;

    /** Deactivates the effect i.e. removes it from the effect stack.\n
     *  Has to be reimplemented by the plugin implementation.
    \param fx_stack A pointer to the effect stack of the soundserver.
    \param EffectID The ID of the effect as returned by \p activate_KS().
     */
    virtual bool deactivate_KS(StereoEffectStack *fx_stack,
                               long EffectID) const = 0;

};


// The types of the class factories
typedef FXPlugin* (*create_fxpi)(KApplication*);
typedef void (*destroy_fxpi)(FXPlugin*);


#endif


