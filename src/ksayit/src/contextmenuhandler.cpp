//
// C++ Implementation: %{MODULE}
//
// Description:
//
//
// Author: %{AUTHOR} <%{EMAIL}>, (C) %{YEAR}
//
// Copyright: See COPYING file that comes with this distribution
//
//

// Qt includes
#include <QtCore/QFile>

// KDE includes
#include <kdebug.h>
#include <klocale.h>
#include <kglobal.h>
#include <kstandarddirs.h>

// App specific includes
#include "Types.h"
#include "contextmenuhandler.h"

/**
 * XML-ActionHandler
 */
ContextActionHandler::ContextActionHandler(ContextMenuHandler *menuhandler)
 : QXmlDefaultHandler(), m_menuhandler(menuhandler)
{
    m_subName.clear();
    m_actionName.clear();
    m_qty.clear();
    m_popup = menuhandler->m_popupmenu;
    m_hit = false;
    m_searchID.clear();
}

ContextActionHandler::~ContextActionHandler()
{
}


void ContextActionHandler::setSearchID( const QString xmlID )
{
    m_searchID = xmlID;
}


bool ContextActionHandler::startElement( const QString &,
            const QString &,
            const QString &qName,
            const QXmlAttributes &atts )
{
    if ( qName == "Item" ){
        if ( atts.value("id") == m_searchID ){
            if ( atts.value("editable") == "yes"){
                  m_menuhandler->setItemEditable( true );
            }
            m_hit = true;
        } else {
            m_hit = false;
        }
    }
    if ( !(m_hit && m_menuhandler->m_popupmenu) )
        return true;

    if ( qName == "Action" ){
        m_actionName = "";
        m_qty = atts.value("qty");
    } else if ( qName == "Submenu" ){
        if ( !atts.value("name").isEmpty() ){
            m_subName = atts.value("name");
            m_popup = m_menuhandler->SubMenuFactory(m_popup);
        }
    }
    return true;
}


bool ContextActionHandler::endElement( const QString &,
            const QString &,
            const QString &qName )
{
    if ( !(m_hit && m_menuhandler->m_popupmenu) )
        return true;
#warning "kde4 port";
#if 0
    if ( qName == "Action" ){
        KAction* newAction = m_menuhandler->ActionFactory(m_actionName, m_qty);
        if ( newAction )
            newAction->plug( m_popup );
    } else if ( qName == "Submenu" ){
        if ( m_subName == "New" ){
            m_menuhandler->m_popupmenu->insertItem( i18n("New"), m_popup);
        }
        // only one sublevel spported -> back to toplevel menu
        m_popup = m_menuhandler->m_popupmenu;
    }
#endif
    return true;
}


bool ContextActionHandler::characters( const QString &ch )
{
    m_actionName += ch;
    return true;
}



/**
 * MenuHandler
 */
ContextMenuHandler::ContextMenuHandler(QObject *parent, const char *name)
 : QObject(parent, name), m_DocTreeView(parent)
{
    m_popupmenu = NULL;
    m_item = NULL;
    m_ac = NULL;
    initActions();
    m_XmlFilePath = KGlobal::dirs()->findResource("data", "ksayit/ContextMenus.xml");
}


ContextMenuHandler::~ContextMenuHandler()
{
    delete m_ac;
    delete renameItem;
    delete deleteItem;
    delete newBookInfo;
    delete newChapter;
    delete newKeywordSet;
    delete newKeyword;
    delete newAbstract;
    delete newAuthorGroup;
    delete newAuthor;
    delete newDate;
    delete newReleaseInfo;
    delete newTitle;
    delete newParagraph;
    delete newSection_1;
    delete newSection_2;
    delete newSection_3;
    delete newSection_4;
    delete newSection_5;

    if (m_popupmenu)
       delete m_popupmenu;
}


void ContextMenuHandler::initActions()
{
#warning "port kde4";
#if 0
    KActionCollection *m_ac = new KActionCollection(this);

    // User defined actions
    renameItem = new KAction (i18n("Rename..."), m_ac, "renameItem");
    connect(renameItem, SIGNAL(triggered(bool)),
        m_DocTreeView, SLOT(slotRenameItem()));

    deleteItem = new KAction (i18n("Delete..."), m_ac, "deleteItem");
    connect(deleteItem, SIGNAL(triggered(bool)),
        m_DocTreeView, SLOT(slotDeleteItem()));

    newBookInfo = new KAction ( i18n("Overview"), m_ac, "newBookInfo");
    connect(newBookInfo, SIGNAL(triggered(bool)),
        m_DocTreeView, SLOT(slotNewBookInfo()));

    newChapter = new KAction ( i18n("Chapter"), m_ac, "newChapter");
    connect(newChapter, SIGNAL(triggered(bool)),
        m_DocTreeView, SLOT(slotNewChapter()));

    newKeywordSet = new KAction ( i18n("Keywords"), m_ac, "newKeywordSet");
    connect(newKeywordSet, SIGNAL(triggered(bool)),
        m_DocTreeView, SLOT(slotNewKeywordSet()));

    newKeyword = new KAction ( i18n("Keyword"), m_ac, "newKeyword");
    connect(newKeyword, SIGNAL(triggered(bool)),
        m_DocTreeView, SLOT(slotNewKeyword()));

    newAbstract = new KAction ( i18n("Abstract"), m_ac, "newAbstract");
    connect(newAbstract, SIGNAL(triggered(bool)),
        m_DocTreeView, SLOT(slotNewAbstract()));

    newAuthorGroup = new KAction ( i18n("Authors"), m_ac, "newAuthorGroup");
    connect(newAuthorGroup, SIGNAL(triggered(bool)),
        m_DocTreeView, SLOT(slotNewAuthorGroup()));

    newAuthor = new KAction ( i18n("Author"), m_ac, "newAuthor");
    connect(newAuthor, SIGNAL(triggered(bool)),
        m_DocTreeView, SLOT(slotNewAuthor()));

    newDate = new KAction ( i18n("Date"), m_ac, "newDate");
    connect(newDate, SIGNAL(triggered(bool)),
        m_DocTreeView, SLOT(slotNewDate()));

    newReleaseInfo = new KAction ( i18n("Release Info"), m_ac, "newReleaseInfo");
    connect(newReleaseInfo, SIGNAL(triggered(bool)),
        m_DocTreeView, SLOT(slotNewReleaseInfo()));

    newTitle = new KAction ( i18n("Title"), m_ac, "newTitle");
    connect(newTitle, SIGNAL(triggered(bool)),
        m_DocTreeView, SLOT(slotNewTitle()));

    newParagraph = new KAction ( i18n("Paragraph"), m_ac, "newParagraph");
    connect(newParagraph, SIGNAL(triggered(bool)),
        m_DocTreeView, SLOT(slotNewParagraph()));

    newSection_1 = new KAction ( i18n("Section Level 1"), m_ac, "newSection_1");
    connect(newSection_1, SIGNAL(triggered(bool)),
        m_DocTreeView, SLOT(slotNewSection_1()));

    newSection_2 = new KAction ( i18n("Section Level 2"), m_ac, "newSection_2");
    connect(newSection_2, SIGNAL(triggered(bool)),
        m_DocTreeView, SLOT(slotNewSection_2()));

    newSection_3 = new KAction ( i18n("Section Level 3"), m_ac, "newSection_3");
    connect(newSection_3, SIGNAL(triggered(bool)),
        m_DocTreeView, SLOT(slotNewSection_3()));

    newSection_4 = new KAction ( i18n("Section Level 4"), m_ac, "newSection_4");
    connect(newSection_4, SIGNAL(triggered(bool)),
        m_DocTreeView, SLOT(slotNewSection_4()));

    newSection_5 = new KAction ( i18n("Section Level 5"), m_ac, "newSection_5");
    connect(newSection_5, SIGNAL(triggered(bool)),
        m_DocTreeView, SLOT(slotNewSection_5()));
#endif
}


KMenu* ContextMenuHandler::getPopupMenu(ListViewInterface *item)
{
    if ( !item )
        return NULL;

    m_item = item;

    QString xmlID = item->getXmlContextName();
    if ( xmlID.isNull() )
        return NULL;

    // delete old popup menu and create new one
    if ( m_popupmenu ){
        delete m_popupmenu;
        m_popupmenu = NULL;
    }
    m_popupmenu = new KMenu(0);

    bool res;
    res = parseXmlFile(xmlID);

    if ( res ){
        return m_popupmenu;
    } else {
        delete m_popupmenu;
        m_popupmenu = NULL;
        return NULL;
    }
}


void ContextMenuHandler::registerPopupMenu(ListViewInterface *item)
{
    if ( !item )
        return;

    m_item = item;

    QString xmlID = item->getXmlContextName();
    if ( xmlID.isNull() )
        return;

    // delete old popup menu
    if ( m_popupmenu ){
        delete m_popupmenu;
        m_popupmenu = NULL;
    }

    // if m_popupmenu == NULL, the parser should not create a popupmenu
    parseXmlFile(xmlID);
}


bool ContextMenuHandler::parseXmlFile(const QString &xmlID)
{
    kDebug(100200) << "ContextMenuHandler::parseXmlFile()";
    kDebug(100200) << "XML-File: " << m_XmlFilePath;

    QFile xmlFile( m_XmlFilePath );
    QXmlSimpleReader parser;
    ContextActionHandler handler(this);
    handler.setSearchID(xmlID);
    parser.setContentHandler( &handler );
    return parser.parse( &xmlFile );
}


KAction* ContextMenuHandler::ActionFactory( const QString &actionName, const QString &qty )
{
    bool enabled;
    if ( qty.toLower() == "n" ){
        // unlimited don't check return enabled action
        enabled = true;
    } else {
        // check if qty is within allowed limits
        ListViewInterface *i = static_cast<ListViewInterface*>(m_item->firstChild());
        int count = 0;
        QString name = QString();
        while (i){
            name = i->getXmlContextName();
            if ( name.toLower() == actionName.toLower() ){
                count++;
            }
            i = static_cast<ListViewInterface*>(i->nextSibling());
        }
        enabled = count < qty.toInt() ? true : false;
    }

    if ( actionName == "Rename" ){
        renameItem->setEnabled( enabled );
        return renameItem;
    } else if ( actionName == "Delete" ){
        deleteItem->setEnabled( enabled );
        return deleteItem;
    } else if ( actionName == "BookInfo" ){
        newBookInfo->setEnabled( enabled );
        return newBookInfo;
    } else if ( actionName == "Chapter" ){
        newChapter->setEnabled( enabled );
        return newChapter;
    } else if ( actionName == "KeywordSet" ){
        newKeywordSet->setEnabled( enabled );
        return newKeywordSet;
    } else if ( actionName == "Keyword" ){
        newKeyword->setEnabled( enabled );
        return newKeyword;
    } else if ( actionName == "Abstract" ){
        newAbstract->setEnabled( enabled );
        return newAbstract;
    } else if ( actionName == "AuthorGroup" ){
        newAuthorGroup->setEnabled( enabled );
        return newAuthorGroup;
    } else if ( actionName == "Author" ){
        newAuthor->setEnabled( enabled );
        return newAuthor;
    } else if ( actionName == "Date" ){
        newDate->setEnabled( enabled );
        return newDate;
    } else if ( actionName == "ReleaseInfo" ){
        newReleaseInfo->setEnabled( enabled );
        return newReleaseInfo;
    } else if ( actionName == "Title" ){
        newTitle->setEnabled( enabled );
        return newTitle;
    } else if ( actionName == "Paragraph" ){
        newParagraph->setEnabled( enabled );
        return newParagraph;
    } else if ( actionName == "Section_1" ){
        newSection_1->setEnabled( enabled );
        return newSection_1;
    } else if ( actionName == "Section_2" ){
        newSection_2->setEnabled( enabled );
        return newSection_2;
    } else if ( actionName == "Section_3" ){
        newSection_3->setEnabled( enabled );
        return newSection_3;
    } else if ( actionName == "Section_4" ){
        newSection_4->setEnabled( enabled );
        return newSection_4;
    } else if ( actionName == "Section_5" ){
        newSection_5->setEnabled( enabled );
        return newSection_5;
    }
    kDebug(100200) << "ERROR!!! ContextMenuHandler::ActionFactory()";
    return NULL;
}


KMenu* ContextMenuHandler::SubMenuFactory(KMenu *parent)
{
    return new KMenu(parent);
}


void ContextMenuHandler::setItemEditable( bool editable )
{
    if ( m_item )
        m_item->setEditable( editable );
}



#include "contextmenuhandler.moc"
