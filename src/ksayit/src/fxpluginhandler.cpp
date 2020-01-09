//
// C++ Implementation: fxpluginhandler
//
// Description: 
//
//
// Author: Robert Vogl <voglrobe@lapislazuli>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//

// QT includes
#include <QtCore/QDir>

// KDE includes
#include <kdebug.h>
#include <kstandarddirs.h>
#include <kglobal.h>
#include <kmessagebox.h>

// App specific includes
#include "fxpluginhandler.h"
#include "ksayit_fxplugin.h"
#include "ksayit_ttsplugin.h"

FXPluginHandler::FXPluginHandler(QObject *parent, const char *name, KConfig *config)
 : QObject(parent, name), m_config(config)
{
}


FXPluginHandler::~FXPluginHandler()
{
    QMap<QString, fx_struct>::Iterator mit;
     
    for (mit=pmap.begin(); mit!=pmap.end(); ++mit){
        if ( (*mit).p != 0 ){
            (*mit).pdestroy( (*mit).p );
            (*mit).handle->unload();
        }
        delete (*mit).handle;
    }
}


void FXPluginHandler::searchPlugins()
{  
    kDebug(100200) << "FXPluginHandler::searchPlugins()";
    
    QDir pdir;
    QStringList plist, tmplist;
    QStringList::Iterator fit, it;
    QString ppath;
    
    plist.clear();
    pmap.clear();
    
    // get .so-files of all KSayIt plugin directories
    QStringList global_dirs = KGlobal::dirs()->resourceDirs("data");
    for (fit=global_dirs.begin(); fit!=global_dirs.end(); ++fit){ // for each dir
        ppath = *fit + "ksayit/fxplugins";
        // kDebug(100200) << "Plugin-Directory: " << ppath;
        pdir.setPath( ppath );
        tmplist = pdir.entryList("*.so", QDir::Files); // list of all .so-files of dir
        for (it=tmplist.begin(); it!=tmplist.end(); ++it){ // for each .so-file of dir
            if( !plist.contains(*it) ){ // .so-file already in list?
                if ( initPluginMap( ppath + '/' + *it ) ){
                    plist.append(*it); // append if plugin scuccessfully opened.
                }
            }
        }
        tmplist.clear();
    }
}


bool FXPluginHandler::initPluginMap(const QString &libpath)
{
    // kDebug(100200) << "FXPluginHandler::initPluginMap() " << libpath;
    
    fx_struct plugin;
    
    plugin.handle = new QLibrary( libpath );
    if ( !plugin.handle->load() ){
        kDebug(100200) << "OOPS: Cannot load library!";
        return false;
    }
    
    create_fxpi  createPlugin  = (create_fxpi)  plugin.handle->resolve("createPlugin");
    destroy_fxpi destroyPlugin = (destroy_fxpi) plugin.handle->resolve("destroyPlugin");
    if ( createPlugin && destroyPlugin ){ // it is a KSayIt plugin
        FXPlugin *p = createPlugin( KApplication::kApplication() );
        plugin.name        = p->getName_KS();
        plugin.description = p->getDescription_KS();
        plugin.EffectID    = 0;
        kDebug(100200) << "FX-Plugin " << plugin.name << " Handle: " << plugin.handle << " registered.";
        destroyPlugin(p);
        plugin.p = NULL;
        // add to list of available plugins
        pmap[plugin.name] = plugin;
        if ( !plugin.handle->unload() ){
            kDebug(100200) << "Unload plugin failed.";
        }
    } else { // not a KSayIt plugin
        kDebug(100200) << "Malformed plugin found!";
        plugin.handle->unload(); // no KSayIt plugin: unload & destroy
        delete plugin.handle;
        return false;
    }
    return true;
}


void FXPluginHandler::readConfiguration()
{
    kDebug(100200) << "+++ entering FXPluginHandler::readConfiguration()";
    
    QMap<QString, fx_struct>::Iterator mit;
    fx_struct plugin;
    create_fxpi  createPlugin;
    destroy_fxpi destroyPlugin;
    
    // unload all plugins and destroy the effect objects
    for (mit=activePlugins.begin(); mit!=activePlugins.end(); ++mit){ // for all active plugins
        plugin = *mit;
        if ((plugin.p != NULL) && (plugin.EffectID == 0)){ // check if really exits and not in use
            plugin.pdestroy(plugin.p); // delete plugin object
            plugin.p = NULL; // mark as deleted
            plugin.handle->unload(); // unload SO
            plugin.handle = NULL;
            plugin.pcreate = NULL; // no longer valid
            plugin.pdestroy = NULL; // no longer valid
            plugin.name.clear();
            plugin.description.clear();
            plugin.EffectID = 0;
        }
    }
    
    // load active plugins and create effect objects
    activePlugins.clear();
    m_config->setGroup("Effect Stack Configuration");
    QStringList conf_active = m_config->readEntry("Activated",QStringList());
    QStringList::Iterator it;
    for (it=conf_active.begin(); it!=conf_active.end(); ++it){ // for all names in config
        mit = pmap.find(*it);
        if( mit!=pmap.end() ){
            // plugin found in list of registered plugins 
            plugin = *mit;
            // resolve factory functions
            createPlugin  = (create_fxpi)  plugin.handle->resolve("createPlugin");
            destroyPlugin = (destroy_fxpi) plugin.handle->resolve("destroyPlugin");
            if ( createPlugin && destroyPlugin ){
                plugin.pcreate  = createPlugin;
                plugin.pdestroy = destroyPlugin;
                plugin.p = createPlugin( KApplication::kApplication() );
                // append to list of active plugins
                activePlugins[plugin.name] = plugin;
                kDebug(100200) << "FXPluginHandler: FX-Plugin " << plugin.name << " active.";
            }
        } else {
            kDebug(100200) << "Fatal error. Conflict with configuration file.";
        }
    }
    kDebug(100200) << "--- leaving FXPluginHandler::readConfiguration()";
}



void FXPluginHandler::showEffectGUI(const QString &pname)
{
    kDebug(100200) << "FXPluginHandler::showEffectGUI(): " << pname;
    
    QMap<QString, fx_struct>::Iterator mit;
    
    // find plugin with name==pname in active list and show its GUI
    mit = activePlugins.find(pname);
    if ( mit != activePlugins.end() ){
        if ( (*mit).p!=NULL ){
            // named and active plugin found
            (*mit).p->showGUI_KS();
        }
        return;
    }
    
    // Not found in active list. Check if an effect with the given name is installed    
    mit = pmap.find(pname);
    if ( mit == pmap.end() ){
        kDebug(100200) << "Sorry. Effect with name " << pname << " is not installed.";
        return;
    }
    
    // plugin exists, but effect is not active yet
    fx_struct plugin;
    create_fxpi  createPlugin;
    destroy_fxpi destroyPlugin;
    
    kDebug(100200) << "Effect not active yet. Loading...";
    plugin = *mit;
    // resolve factory functions
    createPlugin  = (create_fxpi)  plugin.handle->resolve("createPlugin");
    destroyPlugin = (destroy_fxpi) plugin.handle->resolve("destroyPlugin");
    if ( createPlugin && destroyPlugin ){
        plugin.pcreate  = createPlugin;
        plugin.pdestroy = destroyPlugin;
        plugin.p = createPlugin( KApplication::kApplication() );
        // append to list of active plugins
        activePlugins[plugin.name] = plugin;
        kDebug(100200) << "FX-Plugin " << plugin.name << " now active.";
        plugin.p->showGUI_KS();
    }
}


void FXPluginHandler::getPlugins(QStringList &pluginlist)
{
    // kDebug(100200) << "FXPluginHandler::getPlugins";
    
    pluginlist.clear();
    
    QMap<QString, fx_struct>::Iterator mit;
    for (mit=pmap.begin(); mit!=pmap.end(); ++mit){
        pluginlist.append( (*mit).name );    
    }
} 


void FXPluginHandler::activateEffect(const QString &pname,
                                   KArtsServer *server,
                                   StereoEffectStack *fx_stack)
{
    kDebug(100200) << "FXPluginHandler::activateEffect: " << pname;
    
    QMap<QString, fx_struct>::Iterator mit;
    
    // find plugin with name==pname
    mit = activePlugins.find(pname);
    if ( mit==activePlugins.end() || (*mit).p==NULL){
        kDebug(100200) << "Effect " << pname << " not found or inactive.";
        return;        
    }
    (*mit).EffectID = (*mit).p->activate_KS(server, fx_stack);
}


void FXPluginHandler::deactivateEffects(StereoEffectStack *fx_stack)
{
    kDebug(100200) << "FXPluginHandler::deactivateEffects.";
    
    QMap<QString, fx_struct>::Iterator mit;  
    for (mit=activePlugins.begin(); mit!=activePlugins.end(); ++mit){
        if ( ((*mit).EffectID!=0) && ((*mit).p!=NULL) ) {
        // effect active and on stack
            (*mit).p->deactivate_KS(fx_stack, (*mit).EffectID);
            kDebug(100200) << (*mit).name << " with Effect ID " << (*mit).EffectID << " removed.";
            (*mit).EffectID = 0;
        }
    }
}





#include "fxpluginhandler.moc"
