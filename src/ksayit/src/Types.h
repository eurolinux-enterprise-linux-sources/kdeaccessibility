/***************************************************************************
                          Types.h  -  description
                             -------------------
    begin                : Fr Nov 21 2003
    copyright            : (C) 2003 by voglrobe
    email                : voglrobe@saphir
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef TYPES_H
#define TYPES_H

#include <vector>

// QT includes
#include <QtXml/qdom.h>
#include <QtCore/QMap>

// KDE includes
#include <k3listview.h>

typedef QList<int> ParameterList_type;
typedef QMap<QString, ParameterList_type> EffectConfig_type;
typedef std::vector<QDomNode> NodeList_type;

namespace KSayItGlobal {
    static const int item_initial_id = 100;
}

/**
This abstract class is the interface to access the items of the TreeView.
@author Robert Vogl
*/
class ListViewInterface : public K3ListViewItem
{
public:
    ListViewInterface(ListViewInterface *parent=0, QString label=QString());
    ListViewInterface(ListViewInterface *parent=0, ListViewInterface *after=0, QString label=QString());
    ListViewInterface(K3ListView *lv=0, QString label=QString());

    // ~ListViewItemInterface();
    
    /**
     * \returns The header of the item in RTF format. Supposed to be shown to
     * the user if KSayIt is not in edit mode.
     */
    virtual QString getRTFHeader() const = 0;

    /**
     * \returns The body text of the item in RTF format. Supposed to be shown to
     * the user if KSayIt is not in edit mode.
     */
    virtual QString getRTFData() const = 0;

    /**
     * \returns The header of the item that is deliverd to the TTS system.
     */
    virtual QString getHeader() const = 0;

    /**
     * \returns The body text of the item that is delivered to the TTS system.
     */
    virtual QString getData() const = 0;

    /**
     * \returns The raw data of the item including all DocBook tags. Supposed to
     * be shown to the user if KSayIt is in edit mode.
     */
    virtual QString getRawData() const = 0;

    /**
     * Stores the given string to the item and updates the corresponding DOM node.
     * \param data The new data stored to the DOM node. Unless otherwise noted
     * embedded XML tags are not allowed.
     */
    virtual void setData(const QString &data, QDomDocument &doc) = 0;
    
    /**
     * Called on the delete action of the item.
     */
    virtual void deleteAction() = 0;
    
    /**
     * \returns The identifier of the item as defined by
     * the \p id attribute in \p ContextMenus.xml.
     * Returns \p QString() if no identifier is needed.
     */
    virtual QString getXmlContextName() const = 0;
    
    /**
     * Stores alist of nodes that are associated to this TreeView
     * element. The order of nodes within the list is significant.
     * \param list List of \p QDomNode objects.
     */
    virtual void setNodes( const NodeList_type &list ) = 0;
    
    /**
     * \param list A reference to a list that returns the nodes
     * associated to this TreeView element.
     */
    virtual void getNodes( NodeList_type &list ) const = 0;
        
    virtual void setEditable(bool editable) = 0;
    
    virtual bool isEditable() const = 0;
};


#endif
