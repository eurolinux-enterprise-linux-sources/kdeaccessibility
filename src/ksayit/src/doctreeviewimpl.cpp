//
// C++ Implementation: doctreeviewimpl
//
// Description: 
//
//
// Author: Robert Vogl <voglrobe@lapislazuli>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include <stdlib.h> // getenv

// Qt includes
#include <QtCore/QFile>
#include <QtGui/QWidget>
#include <QtCore/QTextStream>
#include <Qt3Support/Q3CString>
#include <QtCore/QRegExp>
#include <QtCore/QDateTime>
#include <QtGui/QColor>
#include <Qt3Support/Q3ListView>
#include <QtGui/QContextMenuEvent>

//KDE includes
#include <kdebug.h>
#include <kglobal.h>
#include <klocale.h>
#include <kfiledialog.h>
#include <kmessagebox.h>
#include <kinputdialog.h>

// App specific includes
#include "doctreeviewimpl.h"
#include "contextmenuhandler.h"
#include "parasaxparser.h"
#include "docbookclasses.h"

//////////////////////////////////////
// TreeView Implementation
//////////////////////////////////////
DocTreeViewImpl::DocTreeViewImpl(QWidget* parent, const char* name, Qt::WFlags fl)
 : DocTreeView(parent, name, fl)
{
    m_rootItem = NULL;
    m_url = KUrl();
    // the hidden formated id to set a sort order
    listView->setSorting(3, true);
    listView->setColumnText(0, i18n("Chapter"));
    listView->setColumnText(1, i18n("Info"));
    listView->setColumnText(2, i18n("Page"));
    listView->setColumnText(3, "");
    listView->setResizeMode( K3ListView::NoColumn );
    listView->setColumnWidthMode(0, K3ListView::Maximum );
    listView->setColumnWidthMode(1, K3ListView::Maximum );
    listView->setColumnWidthMode(2, K3ListView::Maximum );
    listView->setColumnWidthMode(3, K3ListView::Manual );
    listView->setColumnWidth(3, 0);
    listView->setAlternateBackground( QColor(230, 230, 240) );
    listView->setSelectionModeExt( K3ListView::Single );
    
    m_idCounter = KSayItGlobal::item_initial_id;
    m_stopped = false;
    m_currentItem = m_rootItem;
    m_parasaxparser = ParaSaxParser::Instance();
    m_editMode = false;
    m_changedContent.clear();
    
    m_contextmenuhandler = NULL;
    m_contextmenu = NULL;
}


DocTreeViewImpl::~DocTreeViewImpl()
{
    if ( m_rootItem )
        delete m_rootItem;
    delete m_parasaxparser;
    if ( m_contextmenuhandler )
        delete m_contextmenuhandler;
}


void DocTreeViewImpl::enableContextMenus( bool enabled )
{
    if ( enabled ){
        m_contextmenuhandler = new ContextMenuHandler(this, "contextmenuhandler");
    } else {
        if ( m_contextmenuhandler )
            delete m_contextmenuhandler;
        m_contextmenuhandler = NULL;
    }    
}


void DocTreeViewImpl::clear()
{
    kDebug(100200) << "DocTreeViewImpl::clear()";
    m_url = KUrl();
    m_domTree.clear();   
    
    if ( m_rootItem )
        delete m_rootItem;
    
    // inform BookmarkHandler
    // 
    emit signalSetBookmarkFilename( QString() );
    
    listView->clear();
    m_rootItem = new RobDocument( listView, i18n("unnamed") );
    m_idCounter = KSayItGlobal::item_initial_id;
    m_stopped = false;
    m_currentItem = static_cast<ListViewInterface*>(m_rootItem);
}


void DocTreeViewImpl::createEmptyDocument()
{
    kDebug(100200) << "DocTreeViewImpl::createEmptyDocument()";
    // create empty document
    QByteArray data;
    QTextStream wrapped(data, QIODevice::ReadWrite);
    wrapped.setEncoding(QTextStream::UnicodeUTF8);

    wrapped << "<?xml version=\"1.0\" ?>" << endl;
    wrapped << "<!--" << endl;
    wrapped << "!DOCTYPE book PUBLIC \"-//KDE//DTD DocBook XML V4.2-Based Variant V1.1//EN\" \"dtd/kdex.dtd\"" << endl;
    wrapped << "-->" << endl;
    
    QString header(data);
    m_parasaxparser->setProcessingInstruction( header );
    
    wrapped << "<book>" << endl;
    wrapped << "<bookinfo>" << endl;
    wrapped << "<title><![CDATA[" << i18n("Empty Document") << "]]></title>" << endl;  
    wrapped << "<authorgroup>" << endl;
    wrapped << "<author>" << endl;
    wrapped << "<firstname>KSayIt</firstname>" << endl;
    wrapped << "<surname>Wrapper</surname>" << endl;
    wrapped << "</author>" << endl;
    wrapped << "</authorgroup>" << endl;
    wrapped << "<copyright>" << endl;
    wrapped << "<year>2005</year>" << endl;
    wrapped << "<holder>Robert Vogl</holder>" << endl;        
    wrapped << "</copyright>" << endl;
    wrapped << "</bookinfo>" << endl;       
    wrapped << "<chapter>" << endl;
    wrapped << "<title><![CDATA[" << i18n("Untitled") << "]]></title>" << endl;
    wrapped << "<para>" << endl;
    wrapped << "";
    wrapped << "</para>" << endl;
    wrapped << "</chapter>" << endl;
    wrapped << "</book>" << endl;
    
    if ( !m_domTree.setContent( data ) ){
        kDebug(100200) << "Kein gueltiges Dokument!!";
    };
    // inform BookmarkHandler
    emit signalSetBookmarkFilename( i18n("Untitled") );

    QDomElement root = m_domTree.documentElement();
    QDomNode node;  
    if( root.tagName().toLower() == "book" ){ // DocBook
        parseBook(root , m_rootItem); // we parse only ONE Book  
    }
    
    // Initial view
    m_currentItem = static_cast<ListViewInterface*>( listView->lastItem() );
    listView->setSelected( m_currentItem, true );
    listView->ensureItemVisible( m_currentItem );
    slotItemClicked( m_currentItem );
}


void DocTreeViewImpl::openFile(const KUrl &url)
{
    kDebug(100200) << "DocTreeViewImpl::openFile(" << url.path() << ")";

    m_url = url;
    // open file
    QString err;
    QFile file( url.path() );
    if( !file.open(QIODevice::ReadOnly) ){
        err = i18n("Unable to open File.");
        // throw(err);
        kDebug() << err;
    } else {
        // check document
        if( m_domTree.setContent(&file) ){
            // extract Header
            file.reset();
            QString header = QString();
            QString line;
            int offset;
            line = file.readLine();
            while( !file.atEnd() && (offset = line.find("<book", 0, false)) < 0 ){
                header += line;    
                line = file.readLine();  
            }
            file.close();
            header += line.left( offset );
            kDebug(100200) << "### Header: "<< header;       
            m_parasaxparser->setProcessingInstruction( header );
            // inform BookmarkHandler about the opened file
            emit signalSetBookmarkFilename( url.fileName() );
        } else {
            // File is not a valid XML-File. Wrap it
            file.reset();
            QByteArray data;
            QTextStream wrapped(data, QIODevice::ReadWrite);
            wrapped.setEncoding(QTextStream::UnicodeUTF8);

            wrapped << "<?xml version=\"1.0\" ?>" << endl;
            wrapped << "<!--" << endl;
            wrapped << "!DOCTYPE book PUBLIC \"-//KDE//DTD DocBook XML V4.2-Based Variant V1.1//EN\" \"dtd/kdex.dtd\"" << endl;
            wrapped << "-->" << endl;

            QString header(data);
            kDebug(100200) << "Header (Wrapper): "<< header;
            m_parasaxparser->setProcessingInstruction( header );

            wrapped << "<book>" << endl;
            wrapped << "<bookinfo>" << endl;
            wrapped << "<title><![CDATA[" << url.path() << "]]></title>" << endl;
            wrapped << "<authorgroup>" << endl;
            wrapped << "<author>" << endl;
            wrapped << "<firstname>KSayIt</firstname>" << endl;
            wrapped << "<surname>Wrapper</surname>" << endl;
            wrapped << "</author>" << endl;
            wrapped << "</authorgroup>" << endl;
            wrapped << "<copyright>" << endl;
            wrapped << "<year>2005</year>" << endl;
            wrapped << "<holder>Robert Vogl</holder>" << endl;        
            wrapped << "</copyright>" << endl;
            wrapped << "</bookinfo>" << endl;       
            wrapped << "<chapter>" << endl;
            wrapped << "<title><![CDATA[" << i18n("Plain File") << "]]></title>" << endl;
            wrapped << "<para><![CDATA[";
            wrapped << QString( file.readAll() );
            wrapped << "]]></para>" << endl;
            wrapped << "</chapter>" << endl;
            wrapped << "</book>" << endl;
            file.close();

            // Try again
            if (!m_domTree.setContent(data)){
                err = i18n("Failed wrapping the file into XML.");
                kDebug() << err;
                // throw(err);
            }
            emit signalSetBookmarkFilename( i18n("Plain File") );
        }

        // check if correct document type
        QDomElement root = m_domTree.documentElement();
        if( root.tagName().toLower() == "book" ){ // DocBook
            parseBook(root , m_rootItem); // we parse only ONE Book  
            // Initial view
            listView->setSelected( m_rootItem, true );
            slotItemClicked( m_rootItem );
        } else {
            err = i18n("The file is of type %1, 'book' expected.", root.tagName() );
            // throw(err);
            kDebug() << err;
        }
    }
}


void DocTreeViewImpl::saveFile()
{
    kDebug(100200) << "DocTreeViewImpl::saveFile()";
    
    makeCurrentNodePersistent();
    
    QString err;

    if ( m_url.isEmpty() ){ // file has no name, ask user
        QString usershome( getenv("HOME") );
        m_url = KFileDialog::getSaveUrl(usershome, "*.docbook", this, i18n("Save File"));
    }
    if ( m_url.isEmpty() ) // dialog cancelled
        return;
    if ( !m_url.isValid() ){
        err = i18n("The given URL is invalid. Try 'File save as...' instead.");
        kDebug() << err;
        // throw( err );
    } else {
        if ( m_url.isLocalFile() ){
            QFile file( m_url.path() );
            if ( !file.open(QIODevice::WriteOnly) ){
                err = i18n("Unable open file to write.");
                kDebug() << err;
                // throw( err );
            } else {
                QTextStream out(&file);
                out.setEncoding(QTextStream::UnicodeUTF8);
                m_domTree.save( out, 0 );
                m_rootItem->setText(0, m_url.path() );
                file.close();
                // inform BookmarkHandler
                emit signalChangeBookmarkFilename( m_url.fileName() );
                slotItemClicked( m_currentItem );
            }
        } else {
            err = i18n("Save operation currently works on local files only.");
            kDebug() << err;
            // throw( err );
        }
    }
}


void DocTreeViewImpl::saveFileAs()
{
    kDebug(100200) << "DocTreeViewImpl::saveFileAs()";
    QString err;

    makeCurrentNodePersistent();
    
    m_url = KFileDialog::getSaveUrl(QString(), "*.docbook", this, i18n("Save File As"));
    if ( m_url.isEmpty() ) // dialog cancelled
        return;
    if ( !m_url.isValid() ){
        err = i18n("The given URL is invalid.");
        kDebug() << err;
        // throw( err );
    } else {
        if ( m_url.isLocalFile() ){
            QFile file( m_url.path() );
            if ( !file.open(QIODevice::WriteOnly) ){
                err = i18n("Unable open file to write.");
                kDebug() << err;
                // throw( err );
            } else {
                QTextStream out(&file);
                out.setEncoding(QTextStream::UnicodeUTF8);
                m_domTree.save( out, 0 );
                m_rootItem->setText(0, m_url.path() );
                file.close();
                // inform BookmarkHandler
                emit signalChangeBookmarkFilename( m_url.fileName() );
                slotItemClicked( m_currentItem );
            }
        } else {
            err = i18n("Save operation currently works on local files only.");
            kDebug() << err;
            // throw( err );
        }
    }
}


void DocTreeViewImpl::setEditMode(bool mode)
{
    m_editMode = mode;   
    if ( (m_currentItem->isEditable()) && mode ){
        emit signalEnableTextedit( true );
        slotItemClicked( m_currentItem );
        return;
    }
    
    makeCurrentNodePersistent();
    slotItemClicked( m_currentItem );
 
    emit signalEnableTextedit( false );
}


QString DocTreeViewImpl::selectItemByID(const QString &ID, const QString title)
{
    kDebug(100200) << "DocTreeViewImpl::selectItemByID(" << ID << ")";

    if ( ID.isNull() )
        return QString();
        
    QString err = QString();
    
    /**
     * Search on structure (URL)
     * e.g. 1=RobDokument|1=BookInfo|1=KeywordSet|2=Keyword
     */
    QStringList partList;
    partList = ID.split( "|");
    QStringList::Iterator it = partList.begin();
    it++; // skip first element (always RobDocument)
    int childNum = 0;
    QString childType = QString();
    ListViewInterface *item = m_rootItem;
    bool hit = false;
    
    for (; it!=partList.end(); ++it){
        kDebug(100200) << "Part: " << (*it);
        if ( !item )
            break; // expected item does not exist
        item = static_cast<ListViewInterface*>( item->firstChild() );
        if ( item ){
            childNum  = ((*it).section('=', 0, 0)).toInt();
            childType =  (*it).section('=', 1, 1);
            for (int i=1; i<childNum; i++){
                item = static_cast<ListViewInterface*>( item->nextSibling() );
                if ( !item )
                    break; // expected item does not exist
            }
            if ( !item )
                break; // expected item does not exist
            QString type = item->getXmlContextName();
            if ( childType != item->getXmlContextName() )
                break; // structure has been changed
        }
    }
    
    if ( item ){
        QString itemTitle = getItemTitle( item );
        if ( title.left(itemTitle.length()) == itemTitle )
            // Title made unique by KDE by appending "(2)", "(3)"...
            hit = true;
    }
    
    if ( !hit ){
        /**
        * Not found, search on title
        */
        // walk trough the entire tree and try to find an item
        // with the given title.
        item = NULL;
        Q3ListViewItemIterator sit( m_rootItem );
        QString itemTitle = QString();
        while ( sit.current() ) {
            item = static_cast<ListViewInterface*>(sit.current());
            if ( !item )
                break; // expected item does not exist
            itemTitle = getItemTitle( item );
            if ( title.left(itemTitle.length()) == itemTitle ){
                hit = true;
                err = i18n("Maybe the structure of the document has been changed. Please check if this bookmark is still valid.");
                break;
            }
            ++sit;
        }
    }
    
    // still not found => Failure
    if ( !hit ){
        err = i18n("Bookmark not found. Maybe the file content or the bookmark URL has been changed.");
        return err;
    }
    
    // show
    if ( item->isEditable() )
        makeCurrentNodePersistent();

    listView->setSelected( item, true );
    listView->ensureItemVisible(item );
    slotItemClicked( item );
    return err;
}


void DocTreeViewImpl::slotItemClicked(Q3ListViewItem *item)
{
    kDebug(100200) << "DocTreeViewImpl::slotItemClicked()";
    
    if ( item != m_currentItem ){
        makeCurrentNodePersistent();
    }
    
    if ( item ){
        m_currentItem = static_cast<ListViewInterface*>(item);
        QString str=QString();
        QTextStream msg(&str, QIODevice::ReadWrite);
        msg.setEncoding(QTextStream::UnicodeUTF8);

        // check if item has a child and check state
        // of Edit Mode.
        bool editable = (m_currentItem->isEditable()) && m_editMode;
        
        emit signalEnableTextedit( editable );
        recursiveTextCollector( m_currentItem, msg, !editable );
        emit signalContentChanged( str );
        
        // Create Bookmark-ID depending on the location of the item
        // within the tree
        QString idstring = QString();
        QTextStream id(&idstring, QIODevice::ReadWrite);
        id.setEncoding(QTextStream::UnicodeUTF8);
        id << "1=RobDocument";
        recursiveBuildItemIdentifier( m_currentItem, id );
        
        // inform the Bookmark Manager about the new item.
        QString title = getItemTitle( m_currentItem );
        emit signalNotifyBookmarkManager(idstring, title);
    }
}


void DocTreeViewImpl::recursiveBuildItemIdentifier(ListViewInterface* item, QTextStream &idstring)
{
    kDebug(100200) << "DocTreeViewImpl::recursiveBuildItemIdentifier()";
    
    if ( !item )
        return;
    
    // 1. Do we have a parent (=parentItem)?
    // If no -> return, if yes -> recursive call
    ListViewInterface *parentItem = NULL;
    parentItem = static_cast<ListViewInterface*>( item->parent() );
    if ( parentItem ){
        recursiveBuildItemIdentifier( parentItem, idstring );
    } else {
        // break recursion
        return;
    }
    
    // 2. Which child of the parentItem are we? Append no. to idstring.
    int childno = 0;
    QString itemID = item->text(3);
    ListViewInterface *i = static_cast<ListViewInterface*>( parentItem->firstChild() );
    while( i ){
        childno++;
        if ( i->text(3) == itemID )
            break;
        i = static_cast<ListViewInterface*>( i->nextSibling() );
    }
    idstring << "|" << childno;
    
    // 3. Who are we? Append ID to idstring.
    QString itemType = item->getXmlContextName();
    idstring << "=" << itemType; 
}


QString DocTreeViewImpl::getItemTitle( ListViewInterface *item )
{
    if ( !item )
        return QString();
        
    QString col0 = item->text(0);
    QString title= QString();
    if ( col0 == i18n("Paragraph") ){
        title = (item->getData()).left(32);
        // canonify string
        title.replace( QRegExp("^( |\t|\n)+"), "");
        title.replace( QRegExp("( |\t|\n)$+"), "");            
    } else {
        title = col0.left(32);
    }
    return title;
}
    
  
void DocTreeViewImpl::recursiveTextCollector(ListViewInterface* item, QTextStream &msg, bool header)
{
    kDebug(100200) << "DocTreeViewImpl::recursiveTextCollector()";

    QString text;
    if ( header ){
        // if header==true get Headers
        msg << item->getRTFHeader();
        text = item->getRTFData();
    } else {
        // get raw data (Edit Mode)
        text = item->getRawData();    
    }
    if ( !text.isNull() )
        msg << text;
  
    // check if current item has a child
    ListViewInterface *i = static_cast<ListViewInterface*>(item->firstChild());

    while( i ){
        recursiveTextCollector( i, msg, header );
        i = static_cast<ListViewInterface*>(i->nextSibling());
    }
}


void DocTreeViewImpl::sayActiveNodeAndChilds()
{
    kDebug(100200) << "DocTreeViewImpl::sayActiveNodeAndChilds()";

    m_stopped = false;
    if ( !(m_currentItem->firstChild()) ){
        // current item has no childs. It may be changed.
        makeCurrentNodePersistent();
    }
    
    recursiveSayNodes( m_currentItem );
    emit signalAllNodesProcessed();
}


void DocTreeViewImpl::recursiveSayNodes(ListViewInterface* item)
{
    kDebug(100200) << "DocTreeViewImpl::recursiveSayNodes()";

    QString str;
    QTextStream msg(&str, QIODevice::ReadWrite);
    msg.setEncoding(QTextStream::UnicodeUTF8);

    // write Text of item (heading)
    msg << item->getHeader();

    // get data of current item
    msg << item->getData() << "\n" << endl;

    // request say task
    if ( !m_stopped ){
        emit signalSetText( str );
    } else {
        return;
    }

    // check if current item has a child
    ListViewInterface *i = static_cast<ListViewInterface*>(item->firstChild());
    while( i ){
        recursiveSayNodes( i );
        i = static_cast<ListViewInterface*>(i->nextSibling());
    }
}


void DocTreeViewImpl::stop()
{
    kDebug(100200) << "DocTreeViewImpl::stop()";
    m_stopped = true;
}


void DocTreeViewImpl::setNodeContent(QString &text)
{
    kDebug(100200) << "DocTreeViewImpl::setNodeContent()";
    m_changedContent = text;
}


void DocTreeViewImpl::makeCurrentNodePersistent()
{
    kDebug(100200) << "DocTreeViewImpl::slotMakeCurrentNodePersistent()";
    if ( m_changedContent.isNull() )
        return; // no changes were happen

//  TODO: Exceptions not supported.        
//    try{
        m_currentItem->setData(m_changedContent, m_domTree);
        m_changedContent.clear();
//    }
//    catch(QString err){
//        KMessageBox::error( this, err, i18n("XML-Error") );
//        m_changedContent.clear();        
//    }
}    


void DocTreeViewImpl::contextMenuEvent(QContextMenuEvent *e)
{
    kDebug(100200) << "DocTreeViewImpl::contextMenuEvent()";

    if ( !m_currentItem )
        return;

    m_contextmenu = m_contextmenuhandler->getPopupMenu( m_currentItem );
    if ( !m_contextmenu )
        return;

    m_contextmenu->exec( e->globalPos() );
}


/******************************************
 * Context-Menu functions
 ******************************************/

void DocTreeViewImpl::slotRenameItem()
{
    kDebug(100200) << "DocTreeViewImpl::slotRenameItem()";
    
    if (!m_currentItem)
        return;
        
    // get node of current TreeView item
    QDomNode thisNode = QDomNode();   
    NodeList_type list;
    list.clear();
    m_currentItem->getNodes( list );
    thisNode = list.front();    
               
    if ( thisNode.isNull() )
        return;
    
    bool responseOK;
    QString newname;
    newname = KInputDialog::getText(
        i18n("Rename Item"),
        i18n("Please enter the new name of the item:"),
        m_currentItem->text(0),
        &responseOK,
        this/*,
        "queryNewItemName"  */  
    );
    if ( !responseOK )
        return;
             
    // new Title element
    QDomElement newTitle = m_domTree.createElement("Title");
    QDomText textOfTitle = m_domTree.createTextNode(newname);
    newTitle.appendChild( textOfTitle );
    
    // find Title-element, a child of thisNode
    QDomNode oldTitle = thisNode.firstChild();
    while( !oldTitle.isNull() ){
        if( oldTitle.isElement() && oldTitle.nodeName().toLower()=="title" ){
            // replace node
            thisNode.replaceChild(newTitle, oldTitle);
            break;
        }
        oldTitle = oldTitle.nextSibling();
    }
    
    // update TreeView
    m_currentItem->setText(0, newname);
}


void DocTreeViewImpl::slotDeleteItem()
{
    kDebug(100200) << "DocTreeViewImpl::slotDeleteItem()";
    
    ListViewInterface *itemToDelete = m_currentItem;
    ListViewInterface *parentItem = NULL;
    parentItem = static_cast<ListViewInterface*>(itemToDelete->parent());
    if ( !parentItem )
        return; // delete only items with parent
    
    // remove DOM nodes
    QDomNode parentNode;
    NodeList_type nodes;
    NodeList_type::iterator it;
    nodes.clear();
    itemToDelete->getNodes( nodes );
    for ( it=nodes.begin(); it!=nodes.end(); ++it ){
        parentNode = (*it).parentNode();
        if ( !parentNode.isNull() ){
            kDebug(100200) << "remove node...";
            parentNode.removeChild( *it );
        }
    }
    
    // try to delete bookmark
    QString url=QString();
    QTextStream id(&url, QIODevice::ReadWrite);
    id.setEncoding(QTextStream::UnicodeUTF8);
    id << "ksayit://1=RobDocument";
    recursiveBuildItemIdentifier( itemToDelete, id );
    QString title = getItemTitle( itemToDelete );
    emit signalDeleteBookmark( url, title );
    
    // remove TreeView item
    delete itemToDelete;
    
    // reindex entire remaining tree
    ListViewInterface *item = NULL;
    m_idCounter = KSayItGlobal::item_initial_id;
    Q3ListViewItemIterator itr( m_rootItem );
    while ( itr.current() ) {
        item = static_cast<ListViewInterface*>(itr.current());
        item->setText(3, QString("%1").arg(++m_idCounter).rightJustified(8,'0') );    
        ++itr;
    }
    
    // show
    m_rootItem->sort();
    listView->setSelected( parentItem, true );
    listView->ensureItemVisible( parentItem );
    slotItemClicked( parentItem );
}


void DocTreeViewImpl::slotNewBookInfo()
{    
    if ( m_currentItem->getXmlContextName() == "RobDocument" ){
        int newIndex = newIndexFirstChild();
        Overview *overview = new Overview( m_currentItem, NULL, i18n("Overview") );
        overview->setText(3, QString("%1").arg(newIndex).rightJustified(8,'0') );
        
        NodeList_type list;
        list.clear();
        m_currentItem->getNodes( list );
        QDomElement parentElement = (list.front()).toElement();
        QDomNode newNode;        
        if ( parentElement.isElement() ){      
            QDomElement newChild = m_domTree.createElement("BookInfo");
            newNode = parentElement.appendChild( newChild );    
        }
    
        if ( !newNode.isNull() ){
            list.clear();
            list.push_back( newNode );
            overview->setNodes( list );
            m_contextmenuhandler->registerPopupMenu( overview );
        }
    }
    m_rootItem->sort();
}

void DocTreeViewImpl::slotNewChapter()
{
    if ( m_currentItem->getXmlContextName() == "RobDocument" ){
        int newIndex = newIndexLastChild();
        Chapter *chapter = new Chapter( m_currentItem, NULL, i18n("Chapter") );
        chapter->setText(3, QString("%1").arg(newIndex).rightJustified(8,'0') );
        chapter->setText(0, i18n("New Chapter Title") );
        
        // New Dom Element
        NodeList_type list;
        list.clear();
        m_currentItem->getNodes( list );
        QDomElement parentElement = (list.front()).toElement();
        QDomNode newNode;        
        if ( parentElement.isElement() ){      
            QDomElement newChild = m_domTree.createElement("Chapter");
            QDomElement newTitle = m_domTree.createElement("Title");
            QDomText textOfTitle = m_domTree.createTextNode( i18n("New Chapter Title") );
            
            newTitle.appendChild( textOfTitle );
            newChild.appendChild( newTitle );
            newNode = parentElement.appendChild( newChild );    
        }
    
        if ( !newNode.isNull() ){
            list.clear();
            list.push_back( newNode );
            chapter->setNodes( list );
            m_contextmenuhandler->registerPopupMenu( chapter );
        }
    }
    m_rootItem->sort();
}

void DocTreeViewImpl::slotNewKeywordSet()
{
    if ( m_currentItem->getXmlContextName() == "BookInfo" ){
        // New TreeView item
        int newIndex = newIndexFirstChild();
        KeywordSet *keywordset = new KeywordSet(m_currentItem, NULL, i18n("Keywords"));
        keywordset->setText(3, QString("%1").arg(newIndex).rightJustified(8,'0') );

        // New Dom Element
        NodeList_type list;
        list.clear();
        m_currentItem->getNodes( list );
        QDomElement parentElement = (list.front()).toElement();
        QDomNode newNode;
        if ( parentElement.isElement() ){
            QDomElement newChild = m_domTree.createElement("KeywordSet");
            newNode = parentElement.appendChild( newChild );
        }

        if ( !newNode.isNull() ){
            list.clear();
            list.push_back( newNode );
            keywordset->setNodes( list );
            m_contextmenuhandler->registerPopupMenu( keywordset );
        }
    }
    m_rootItem->sort();
}

void DocTreeViewImpl::slotNewKeyword()
{
    if ( m_currentItem->getXmlContextName() == "KeywordSet" ){
        // New TreeView item
        int newIndex = newIndexLastChild();
        Keyword *keyword = new Keyword(m_currentItem, NULL, i18n("Keyword"));
        keyword->setText(3, QString("%1").arg(newIndex).rightJustified(8,'0') );
        keyword->setText(0, i18n("New Keyword") );

        // New Dom Element
        NodeList_type list;
        list.clear();
        m_currentItem->getNodes( list );
        QDomElement parentElement = (list.front()).toElement();
        QDomNode newNode;
        QDomText newText;
        if ( parentElement.isElement() ){
            QDomElement newChild = m_domTree.createElement("Keyword");
            newText = m_domTree.createTextNode( i18n("New Keyword") );
            newChild.appendChild( newText );
            newNode = parentElement.appendChild( newChild );
        }

        if ( !newText.isNull() ){
            list.clear();
            list.push_back( newText );
            keyword->setNodes( list );
            m_contextmenuhandler->registerPopupMenu( keyword );
        }
    }
    m_rootItem->sort();
}

void DocTreeViewImpl::slotNewAbstract()
{
    if ( m_currentItem->getXmlContextName() == "BookInfo" ){
        // New TreeView item
        int newIndex = newIndexLastChild();
        Abstract *abstract = new Abstract(m_currentItem, NULL, i18n("Abstract"));
        abstract->setText(3, QString("%1").arg(newIndex).rightJustified(8,'0') );

        // New Dom Element
        NodeList_type list;
        list.clear();
        m_currentItem->getNodes( list );
        QDomElement parentElement = (list.front()).toElement();
        QDomNode newNode;
        if ( parentElement.isElement() ){
            QDomElement newAbstract = m_domTree.createElement("Abstract");
            // QDomElement newPara  = m_domTree.createElement("Para");
            // newAbstract.appendChild( newPara );
            newNode = parentElement.appendChild( newAbstract );
        }

        if ( !newNode.isNull() ){
            list.clear();
            list.push_back( newNode );
            abstract->setNodes( list );
            m_contextmenuhandler->registerPopupMenu( abstract );
        }
        // add a new Para-Element
        ListViewInterface *backup = m_currentItem;
        m_currentItem = abstract;
        slotNewParagraph();
        m_currentItem = backup;
    }
    m_rootItem->sort();
}

void DocTreeViewImpl::slotNewAuthorGroup()
{
    if ( m_currentItem->getXmlContextName() == "BookInfo" ){
        // New TreeView item
        int newIndex = newIndexFirstChild();
        AuthorGroup *authorgroup = new AuthorGroup(m_currentItem, NULL, i18n("Author(s)"));
        authorgroup->setText(3, QString("%1").arg(newIndex).rightJustified(8,'0') );

        // New Dom Element
        NodeList_type list;
        list.clear();
        m_currentItem->getNodes( list );
        QDomElement parentElement = (list.front()).toElement();
        QDomNode newNode;
        if ( parentElement.isElement() ){
            QDomElement newAuthorGroup = m_domTree.createElement("AuthorGroup");
            newNode = parentElement.appendChild( newAuthorGroup );
        }

        if ( !newNode.isNull() ){
            list.clear();
            list.push_back( newNode );
            authorgroup->setNodes( list );
            m_contextmenuhandler->registerPopupMenu( authorgroup );
        }
        // add a new Author-Element
        ListViewInterface *backup = m_currentItem;
        m_currentItem = authorgroup;
        slotNewAuthor();
        m_currentItem = backup;
    }
    m_rootItem->sort();
}

void DocTreeViewImpl::slotNewAuthor()
{
    if ( m_currentItem->getXmlContextName() == "AuthorGroup" ){
        // New TreeView item
        int newIndex = newIndexLastChild();
        Author *author = new Author(m_currentItem);
        author->setText(3, QString("%1").arg(newIndex).rightJustified(8,'0') );
        author->setText(0, i18n("Author") );
        author->setText(1, i18n("Firstname Surname") );

        // New Dom Element
        NodeList_type list;
        list.clear();
        m_currentItem->getNodes( list );
        QDomElement parentElement = (list.front()).toElement();
        QDomText firstname = QDomText();
        QDomText surname = QDomText();
        
        if ( parentElement.isElement() ){
            QDomElement newAuthor = m_domTree.createElement("Author");
            QDomElement newFirstname = m_domTree.createElement("Firstname");
            firstname = m_domTree.createTextNode( i18n("Firstname") );
            newFirstname.appendChild( firstname );
             
            QDomElement newSurname = m_domTree.createElement("Surname");
            surname = m_domTree.createTextNode( i18n("Surname") );
            newSurname.appendChild( surname );
            
            newAuthor.appendChild( newFirstname );
            newAuthor.appendChild( newSurname );           
            
            parentElement.appendChild( newAuthor );
        }

        if ( ! (firstname.isNull() || surname.isNull()) ){
            list.clear();
            list.push_back( firstname );
            list.push_back( surname );
            author->setNodes( list );
            m_contextmenuhandler->registerPopupMenu( author );
        }
    }
    m_rootItem->sort();
}

void DocTreeViewImpl::slotNewDate()
{
    if ( m_currentItem->getXmlContextName() == "BookInfo" ){
        // New TreeView item
        int newIndex = newIndexLastChild();
        Date *date = new Date(m_currentItem, NULL, i18n("Date"));
        date->setText(3, QString("%1").arg(newIndex).rightJustified(8,'0') );
        // get current date
        QString today;
        today = KGlobal::locale()->formatDate(QDate::currentDate(Qt::LocalTime), KLocale::ShortDate);
        date->setText( 1, today);

        // New Dom Element
        NodeList_type list;
        list.clear();
        m_currentItem->getNodes( list );
        QDomElement parentElement = (list.front()).toElement();
        QDomText dateToday;
        if ( parentElement.isElement() ){
            QDomElement newDate = m_domTree.createElement("Date");
            dateToday  = m_domTree.createTextNode( today );
            newDate.appendChild( dateToday );
            parentElement.appendChild( newDate );
        }

        if ( !dateToday.isNull() ){
            list.clear();
            list.push_back( dateToday );
            date->setNodes( list );
            m_contextmenuhandler->registerPopupMenu( date );
        }
    }
    m_rootItem->sort();
}

void DocTreeViewImpl::slotNewReleaseInfo()
{
    if ( m_currentItem->getXmlContextName() == "BookInfo" ){
        // New TreeView item
        int newIndex = newIndexLastChild();
        ReleaseInfo *relinfo = new ReleaseInfo(m_currentItem, NULL, i18n("Release"));
        relinfo->setText(3, QString("%1").arg(newIndex).rightJustified(8,'0') );
        relinfo->setText( 1, "0.0.0" );

        // New Dom Element
        NodeList_type list;
        list.clear();
        m_currentItem->getNodes( list );
        QDomElement parentElement = (list.front()).toElement();
        QDomText releaseText;
        if ( parentElement.isElement() ){
            QDomElement newRelease = m_domTree.createElement("ReleaseInfo");
            releaseText = m_domTree.createTextNode("0.0.0");
            newRelease.appendChild( releaseText );
            parentElement.appendChild( newRelease );
        }

        if ( !releaseText.isNull() ){
            list.clear();
            list.push_back( releaseText );
            relinfo->setNodes( list );
            m_contextmenuhandler->registerPopupMenu( relinfo );
        }
    }
    m_rootItem->sort();
}

void DocTreeViewImpl::slotNewTitle()
{

}

void DocTreeViewImpl::slotNewParagraph()
{
    // Create Null element
    QDomElement parentElement = QDomElement();

    NodeList_type list;
    list.clear();
    m_currentItem->getNodes( list );
    parentElement = (list.front()).toElement();

    if ( parentElement.isNull() )
        return;

    // New TreeView item
    int newIndex = newIndexLastChild();
    Para *para = new Para(m_currentItem, NULL, i18n("Paragraph"));
    para->setText(3, QString("%1").arg(newIndex).rightJustified(8,'0') );

    // New Dom Element
    QDomNode newNode;
    if ( parentElement.isElement() ){
        QDomElement newChild = m_domTree.createElement("Para");
        newNode = parentElement.appendChild( newChild );
    }

    if ( !newNode.isNull() ){
        list.clear();
        list.push_back( newNode );
        para->setNodes( list );
        m_contextmenuhandler->registerPopupMenu( para );
    }
    m_rootItem->sort();
}

void DocTreeViewImpl::slotNewSection_1()
{
    if ( m_currentItem->getXmlContextName() == "Chapter" ){
        int newIndex = newIndexLastChild();
        Sect1 *sect1 = new Sect1(m_currentItem, NULL, i18n("Section Level 1"));
        sect1->setText(3, QString("%1").arg(newIndex).rightJustified(8,'0') );
        
        // New Dom Element
        NodeList_type list;
        list.clear();
        m_currentItem->getNodes( list );
        QDomElement parentElement = (list.front()).toElement();
        QDomNode newNode;        
        if ( parentElement.isElement() ){      
            QDomElement newSect = m_domTree.createElement("Sect1");
            QDomElement newTitle = m_domTree.createElement("Title");
            QDomText textOfTitle = m_domTree.createTextNode( i18n("New Section Title") );
            
            newTitle.appendChild( textOfTitle );
            newSect.appendChild( newTitle );
            newNode = parentElement.appendChild( newSect );    
        }
    
        if ( !newNode.isNull() ){
            list.clear();
            list.push_back( newNode );
            sect1->setNodes( list );
            m_contextmenuhandler->registerPopupMenu( sect1 );
        }
    }
    m_rootItem->sort();
}

void DocTreeViewImpl::slotNewSection_2()
{
    if ( m_currentItem->getXmlContextName() == "Sect1" ){
        int newIndex = newIndexLastChild();
        Sect2 *sect2 = new Sect2(m_currentItem, NULL, i18n("Section Level 2"));
        sect2->setText(3, QString("%1").arg(newIndex).rightJustified(8,'0') );
        
        // New Dom Element
        NodeList_type list;
        list.clear();
        m_currentItem->getNodes( list );
        QDomElement parentElement = (list.front()).toElement();
        QDomNode newNode;        
        if ( parentElement.isElement() ){      
            QDomElement newSect = m_domTree.createElement("Sect2");
            QDomElement newTitle = m_domTree.createElement("Title");
            QDomText textOfTitle = m_domTree.createTextNode( i18n("New Section Title") );
            
            newTitle.appendChild( textOfTitle );
            newSect.appendChild( newTitle );
            newNode = parentElement.appendChild( newSect );    
        }
    
        if ( !newNode.isNull() ){
            list.clear();
            list.push_back( newNode );
            sect2->setNodes( list );
            m_contextmenuhandler->registerPopupMenu( sect2 );
        }
    }
    m_rootItem->sort();
}

void DocTreeViewImpl::slotNewSection_3()
{
    if ( m_currentItem->getXmlContextName() == "Sect2" ){
        int newIndex = newIndexLastChild();
        Sect3 *sect3 = new Sect3(m_currentItem, NULL, i18n("Section Level 3"));
        sect3->setText(3, QString("%1").arg(newIndex).rightJustified(8,'0') );
        
        // New Dom Element
        NodeList_type list;
        list.clear();
        m_currentItem->getNodes( list );
        QDomElement parentElement = (list.front()).toElement();
        QDomNode newNode;        
        if ( parentElement.isElement() ){      
            QDomElement newSect = m_domTree.createElement("Sect3");
            QDomElement newTitle = m_domTree.createElement("Title");
            QDomText textOfTitle = m_domTree.createTextNode( i18n("New Section Title") );
            
            newTitle.appendChild( textOfTitle );
            newSect.appendChild( newTitle );
            newNode = parentElement.appendChild( newSect );    
        }
    
        if ( !newNode.isNull() ){
            list.clear();
            list.push_back( newNode );
            sect3->setNodes( list );
            m_contextmenuhandler->registerPopupMenu( sect3 );
        }
    }
    m_rootItem->sort();
}

void DocTreeViewImpl::slotNewSection_4()
{
    if ( m_currentItem->getXmlContextName() == "Sect3" ){
        int newIndex = newIndexLastChild();
        Sect4 *sect4 = new Sect4(m_currentItem, NULL, i18n("Section Level 4"));
        sect4->setText(3, QString("%1").arg(newIndex).rightJustified(8,'0') );
        
        // New Dom Element
        NodeList_type list;
        list.clear();
        m_currentItem->getNodes( list );
        QDomElement parentElement = (list.front()).toElement();
        QDomNode newNode;        
        if ( parentElement.isElement() ){      
            QDomElement newSect = m_domTree.createElement("Sect4");
            QDomElement newTitle = m_domTree.createElement("Title");
            QDomText textOfTitle = m_domTree.createTextNode( i18n("New Section Title") );
            
            newTitle.appendChild( textOfTitle );
            newSect.appendChild( newTitle );
            newNode = parentElement.appendChild( newSect );    
        }
    
        if ( !newNode.isNull() ){
            list.clear();
            list.push_back( newNode );
            sect4->setNodes( list );
            m_contextmenuhandler->registerPopupMenu( sect4 );
        }
    }
    m_rootItem->sort();
}

void DocTreeViewImpl::slotNewSection_5()
{
    if ( m_currentItem->getXmlContextName() == "Sect4" ){
        int newIndex = newIndexLastChild();
        Sect5 *sect5 = new Sect5(m_currentItem, NULL, i18n("Section Level 5"));
        sect5->setText(3, QString("%1").arg(newIndex).rightJustified(8,'0') );
        
        // New Dom Element
        NodeList_type list;
        list.clear();
        m_currentItem->getNodes( list );
        QDomElement parentElement = (list.front()).toElement();
        QDomNode newNode;        
        if ( parentElement.isElement() ){      
            QDomElement newSect = m_domTree.createElement("Sect5");
            QDomElement newTitle = m_domTree.createElement("Title");
            QDomText textOfTitle = m_domTree.createTextNode( i18n("New Section Title") );
            
            newTitle.appendChild( textOfTitle );
            newSect.appendChild( newTitle );
            newNode = parentElement.appendChild( newSect );    
        }
    
        if ( !newNode.isNull() ){
            list.clear();
            list.push_back( newNode );
            sect5->setNodes( list );
            m_contextmenuhandler->registerPopupMenu( sect5 );
        }
    }
    m_rootItem->sort();
}
  
int DocTreeViewImpl::newIndexFirstChild()
{
    kDebug(100200) << "DocTreeViewImpl::newIndexFirstChild()";
    
    int currentIndex = (m_currentItem->text(3)).toInt();
    
    // walk trough the entire tree and increment all
    // indices greater than currentIndex
    ListViewInterface *item = NULL;
    int itemIndex;
    Q3ListViewItemIterator it( m_rootItem );
    while ( it.current() ) {
        item = static_cast<ListViewInterface*>(it.current());
        itemIndex = (item->text(3)).toInt();
        if ( itemIndex > currentIndex ){
            item->setText(3, QString("%1").arg(itemIndex+1).rightJustified(8,'0') );;    
        }
        ++it;
    }
       
    m_idCounter++;
    currentIndex += 1;
    return currentIndex;    
}    


int DocTreeViewImpl::newIndexLastChild()
{
    kDebug(100200) << "DocTreeViewImpl::newIndexLastChild()";
       
    // find highest index of the subtree with
    // m_currentItem as root
    int lastIndex = 0;
    findHighestIndex( m_currentItem, lastIndex );
    kDebug(100200) << "Letzter Index: " << lastIndex;
    
    // walk trough the entire tree and increment all
    // indices greater than lastIndex
    int itemIndex;
    ListViewInterface *item;
    Q3ListViewItemIterator it( m_rootItem );
    while ( it.current() ) {
        item = static_cast<ListViewInterface*>(it.current());
        itemIndex = (item->text(3)).toInt();
        if ( itemIndex > lastIndex ){
            item->setText(3, QString("%1").arg(itemIndex+1).rightJustified(8,'0') );    
        }
        ++it;
    }

    m_idCounter++;
    lastIndex += 1;
    return lastIndex;    
}    


void DocTreeViewImpl::findHighestIndex(ListViewInterface* item, int &index)
{
    int currentIndex = (item->text(3)).toInt();
    index = currentIndex > index ? currentIndex : index; 
    
    ListViewInterface *i = static_cast<ListViewInterface*>(item->firstChild());
    while( i ){
        findHighestIndex(i, index);
        i = static_cast<ListViewInterface*>(i->nextSibling());
    }
}    


   
/******************************************
 * XML-Parser functions
 ******************************************/
//
// DocBook parser
//
void DocTreeViewImpl::parseBook(const QDomElement &element, ListViewInterface *item)
{
    kDebug(100200) << "+++ entering parseBook()";
    item->setText(3, QString("%1").arg(++m_idCounter).rightJustified(8,'0') );
    // register node
    NodeList_type nodelist;
    nodelist.clear();
    nodelist.push_back(element);
    item->setNodes( nodelist );
    // register Popup menu
    m_contextmenuhandler->registerPopupMenu( item );
          
    // TODO: Attributes Id, Lang

    // Elements (parse BookInfo before Chapter)
    QDomNode node = element.firstChild();
    while( !node.isNull() ){
        if( node.isElement() && node.nodeName().toLower()=="bookinfo" ){
            parseBookInfo( node.toElement(), item );
            break;
        }
        node = node.nextSibling();
    }
    node = element.firstChild();
    while( !node.isNull() ){    
        if ( node.isElement() && node.nodeName().toLower()=="chapter" ){
            parseChapter( node.toElement(), item );
        }
        node = node.nextSibling();
    }
    kDebug(100200) << "--- leaving parseBook()";
}


void DocTreeViewImpl::parseBookInfo(const QDomElement &element, ListViewInterface *item)
{
    kDebug(100200) << "+++ entering parseBookInfo()";

    Overview *overview = new Overview(item, NULL, i18n("Overview"));
    overview->setText(3, QString("%1").arg(++m_idCounter).rightJustified(8,'0') );
    // register node
    NodeList_type list;
    list.clear();
    list.push_back( element );
    overview->setNodes(list);
    // register Popup menu
    m_contextmenuhandler->registerPopupMenu( overview );
    
    QDomText data;
    QDomNode node = element.firstChild();
    // fixed order
    while( !node.isNull() ){
        if( node.isElement() && node.nodeName().toLower()=="title" ){
            data = node.firstChild().toText();
            if( !data.isNull() ){
                item->setText( 0, data.nodeValue() ); // assign title to RootItem
                break;
            }
        }
        node = node.nextSibling();
    }
    node = element.firstChild();
    while( !node.isNull() ){
        if ( node.isElement() && node.nodeName().toLower()=="authorgroup" ){
            parseAuthorGroup( node.toElement(), overview );
        }
        node = node.nextSibling();
    }
    node = element.firstChild();
    while( !node.isNull() ){
        if ( node.isElement() && node.nodeName().toLower()=="date" ){
            data = node.firstChild().toText();
            if( !data.isNull() ){
                Date *date = new Date(overview, NULL, i18n("Date"));
                date->setText(3, QString("%1").arg(++m_idCounter).rightJustified(8,'0') );
                date->setText( 1, data.nodeValue() );
                // register node
                list.clear();
                list.push_back( data );
                date->setNodes(list);
                // register Popup menu
                m_contextmenuhandler->registerPopupMenu( date );
            }
        }
        node = node.nextSibling();
    }
    node = element.firstChild();
    while( !node.isNull() ){
        if ( node.isElement() && node.nodeName().toLower()=="releaseinfo" ){
            data = node.firstChild().toText();
            if( !data.isNull() ){
                ReleaseInfo *relinfo = new ReleaseInfo(overview, NULL, i18n("Release"));
                relinfo->setText(3, QString("%1").arg(++m_idCounter).rightJustified(8,'0') );
                relinfo->setText( 1, data.nodeValue() );
                // register node
                list.clear();
                list.push_back( data );
                relinfo->setNodes( list );
                // register Popup menu
                m_contextmenuhandler->registerPopupMenu( relinfo );
            }
        }
        node = node.nextSibling();
    }    
    node = element.firstChild();
    while( !node.isNull() ){
         if ( node.isElement() && node.nodeName().toLower()=="keywordset" ){
            parseKeywordSet( node.toElement(), overview );
        }
        node = node.nextSibling();
    }        
    node = element.firstChild();
    while( !node.isNull() ){        
        if ( node.isElement() && node.nodeName().toLower()=="abstract" ){
            parseAbstract( node.toElement(), overview );
        }
        node = node.nextSibling();
    }
    kDebug(100200) << "--- leaving parseBookInfo()";
}


void DocTreeViewImpl::parseAuthorGroup(const QDomElement &element, ListViewInterface *item)
{
    kDebug(100200) << "+++ entering parseAuthorGroup()";

    // item = overview
    AuthorGroup *authorgroup = new AuthorGroup(item, NULL, i18n("Author(s)"));
    authorgroup->setText(3, QString("%1").arg(++m_idCounter).rightJustified(8,'0') );
    // register node
    NodeList_type list;
    list.clear();
    list.push_back( element );
    authorgroup->setNodes( list );
    // register Popup menu
    m_contextmenuhandler->registerPopupMenu( authorgroup );

    QDomNode node = element.firstChild();
    while( !node.isNull() ){
        if( node.isElement() && node.nodeName().toLower()=="author" ){
            parseAuthor( node.toElement(), authorgroup );
        }
        node = node.nextSibling();
    }
    kDebug(100200) << "--- leaving parseAuthorGroup()";
}


void DocTreeViewImpl::parseAuthor(const QDomElement &element, ListViewInterface *item)
{
    kDebug(100200) << "+++ entering parseAuthor()";

    // item = authorgroup
    QDomText data;
    QDomText firstname = QDomText();
    QDomText surname = QDomText();
    QString s_firstname = QString();
    QString s_surname = QString();
    
    Author *author = new Author(item);
    author->setText(3, QString("%1").arg(++m_idCounter).rightJustified(8,'0') );

    QDomNode node = element.firstChild();
    while( !node.isNull() ){
        if( node.isElement() && node.nodeName().toLower()=="firstname" ){
            data = node.firstChild().toText();
            if( !data.isNull() ){
                s_firstname = data.nodeValue();
                firstname = data;
            }
        } else if ( node.isElement() && node.nodeName().toLower()=="surname" ){
            data = node.firstChild().toText();
            if( !data.isNull() ){
                s_surname = data.nodeValue();
                surname = data;
            }
        }
        node = node.nextSibling();
    }
    // register nodes
    NodeList_type list;
    list.clear();
    list.push_back( firstname );
    list.push_back( surname );
    author->setNodes( list );
    // register Popup menu
    m_contextmenuhandler->registerPopupMenu( author );
    
    author->setText(0, i18n("Author") );
    author->setText(1, s_firstname + ' ' + s_surname);
    
    kDebug(100200) << "--- leaving parseAuthor()";
}


void DocTreeViewImpl::parseKeywordSet(const QDomElement &element, ListViewInterface *item)
{
    kDebug(100200) << "+++ entering parseKeywordSet()";

    // item = overview
    KeywordSet *keywordset = new KeywordSet(item, NULL, i18n("Keywords"));
    keywordset->setText(3, QString("%1").arg(++m_idCounter).rightJustified(8,'0') );
    // register node
    NodeList_type list;
    list.clear();
    list.push_back( element );
    keywordset->setNodes( list );
    // register Popup menu
    m_contextmenuhandler->registerPopupMenu( keywordset );
    
    QDomText data;
    QDomNode node = element.firstChild();
    while( !node.isNull() ){
        if( node.isElement() && node.nodeName().toLower()=="keyword" ){
            data = node.firstChild().toText();
            if( !data.isNull() ){
                Keyword *keyword = new Keyword(keywordset);
                keyword->setText(3, QString("%1").arg(++m_idCounter).rightJustified(8,'0') );
                keyword->setText(0, data.nodeValue() );
                // register node
                list.clear();
                list.push_back( data );
                keyword->setNodes( list );
                // register Popup menu
                m_contextmenuhandler->registerPopupMenu( keyword );
            }
        }
        node = node.nextSibling();
    }
    kDebug(100200) << "--- leaving parseKeywordSet()";
}


void DocTreeViewImpl::parseAbstract(const QDomElement &element, ListViewInterface *item)
{
    kDebug(100200) << "+++ entering parseAbstract()";

    // item = overview
    QDomText data;
    Abstract *abstract = new Abstract(item, NULL, i18n("Abstract"));
    abstract->setText(3, QString("%1").arg(++m_idCounter).rightJustified(8,'0') );
    // register node
    NodeList_type list;
    list.clear();
    list.push_back( element );
    abstract->setNodes( list );
    // register Popup menu
    m_contextmenuhandler->registerPopupMenu( abstract );

    QDomNode node = element.firstChild();
    while( !node.isNull() ){
        if( node.isElement() && node.nodeName().toLower()=="para" ){
            parsePara( node.toElement(), abstract );
        }
        node = node.nextSibling();
    }
    kDebug(100200) << "--- leaving parseAbstract()";
}


void DocTreeViewImpl::parseChapter(const QDomElement &element, ListViewInterface *item)
{
    kDebug(100200) << "+++ entering parseChapter()";
    
    QDomText data;
    Chapter *chapter = new Chapter(item, NULL, i18n("Chapter"));
    chapter->setText(3, QString("%1").arg(++m_idCounter).rightJustified(8,'0') );
    // register node
    NodeList_type list;
    list.clear();
    list.push_back( element );
    chapter->setNodes( list );
    // register Popup menu
    m_contextmenuhandler->registerPopupMenu( chapter );
    
    QDomNode node = element.firstChild();
    while( !node.isNull() ){
        if( node.isElement() && node.nodeName().toLower()=="title" ){
            data = node.firstChild().toText();
            if( !data.isNull() ){
                chapter->setText( 0, data.nodeValue() );
            }
        } else if ( node.isElement() && node.nodeName().toLower()=="para" ){
            parsePara( node.toElement(), chapter );
        } else if ( node.isElement() && node.nodeName().toLower()=="sect1" ){
            parseSect1( node.toElement(), chapter );
        }
        node = node.nextSibling();
    }
    kDebug(100200) << "--- leaving parseChapter()";
}


void DocTreeViewImpl::parseSect1(const QDomElement &element, ListViewInterface *item)
{
    kDebug(100200) << "+++ entering parseSect1()";
    
    QDomText data;
    Sect1 *sect1 = new Sect1(item, NULL, i18n("Section Level 1"));
    sect1->setText(3, QString("%1").arg(++m_idCounter).rightJustified(8,'0') );
    // register node
    NodeList_type list;
    list.clear();
    list.push_back( element );
    sect1->setNodes( list );
    // register Popup menu
    m_contextmenuhandler->registerPopupMenu( sect1 );

    QDomNode node = element.firstChild();
    while( !node.isNull() ){
        if( node.isElement() && node.nodeName().toLower()=="title" ){
            data = node.firstChild().toText();
            if( !data.isNull() ){
                sect1->setText( 0, data.nodeValue() );
            }
        } else if ( node.isElement() && node.nodeName().toLower()=="para" ){
            parsePara( node.toElement(), sect1 );
        } else if ( node.isElement() && node.nodeName().toLower()=="sect2" ){
            parseSect2( node.toElement(), sect1 );
        }
        node = node.nextSibling();
    }
    kDebug(100200) << "--- leaving parseSect1()";
}


void DocTreeViewImpl::parseSect2(const QDomElement &element, ListViewInterface *item)
{
    kDebug(100200) << "+++ entering parseSect2()";
    
    QDomText data;
    Sect2 *sect2 = new Sect2(item, NULL, i18n("Section Level 2"));
    sect2->setText(3, QString("%1").arg(++m_idCounter).rightJustified(8,'0') );
    // register node
    NodeList_type list;
    list.clear();
    list.push_back( element );
    sect2->setNodes( list );
    // register Popup menu
    m_contextmenuhandler->registerPopupMenu( sect2 );

    QDomNode node = element.firstChild();
    while( !node.isNull() ){
        if( node.isElement() && node.nodeName().toLower()=="title" ){
            data = node.firstChild().toText();
            if( !data.isNull() ){
                sect2->setText( 0, data.nodeValue() );
            }
        } else if ( node.isElement() && node.nodeName().toLower()=="para" ){
            parsePara( node.toElement(), sect2 );
        } else if ( node.isElement() && node.nodeName().toLower()=="sect3" ){
            parseSect3( node.toElement(), sect2 );
        }
        node = node.nextSibling();
    }
    kDebug(100200) << "--- leaving parseSect2()";
}


void DocTreeViewImpl::parseSect3(const QDomElement &element, ListViewInterface *item)
{
    kDebug(100200) << "+++ entering parseSect3()";
    
    QDomText data;
    Sect3 *sect3 = new Sect3(item, NULL, i18n("Section Level 3"));
    sect3->setText(3, QString("%1").arg(++m_idCounter).rightJustified(8,'0') );
    // register node
    NodeList_type list;
    list.clear();
    list.push_back( element );
    sect3->setNodes( list );
    // register Popup menu
    m_contextmenuhandler->registerPopupMenu( sect3 );

    QDomNode node = element.firstChild();
    while( !node.isNull() ){
        if( node.isElement() && node.nodeName().toLower()=="title" ){
            data = node.firstChild().toText();
            if( !data.isNull() ){
                sect3->setText( 0, data.nodeValue() );
            }
        } else if ( node.isElement() && node.nodeName().toLower()=="para" ){
            parsePara( node.toElement(), sect3 );
        } else if ( node.isElement() && node.nodeName().toLower()=="sect4" ){
            parseSect4( node.toElement(), sect3 );
        }
        node = node.nextSibling();
    }
    kDebug(100200) << "--- leaving parseSect3()";
}


void DocTreeViewImpl::parseSect4(const QDomElement &element, ListViewInterface *item)
{
    kDebug(100200) << "+++ entering parseSect4()";
    
    QDomText data;
    Sect4 *sect4 = new Sect4(item, NULL, i18n("Section Level 4"));
    sect4->setText(3, QString("%1").arg(++m_idCounter).rightJustified(8,'0') );
    // register node
    NodeList_type list;
    list.clear();
    list.push_back( element );
    sect4->setNodes( list );
    // register Popup menu
    m_contextmenuhandler->registerPopupMenu( sect4 );

    QDomNode node = element.firstChild();
    while( !node.isNull() ){
        if( node.isElement() && node.nodeName().toLower()=="title" ){
            data = node.firstChild().toText();
            if( !data.isNull() ){
                sect4->setText( 0, data.nodeValue() );
            }
        } else if ( node.isElement() && node.nodeName().toLower()=="para" ){
            parsePara( node.toElement(), sect4 );
        } else if ( node.isElement() && node.nodeName().toLower()=="sect5" ){
            parseSect5( node.toElement(), sect4 );
        }
        node = node.nextSibling();
    }
    kDebug(100200) << "--- leaving parseSect4()";
}


void DocTreeViewImpl::parseSect5(const QDomElement &element, ListViewInterface *item)
{
    kDebug(100200) << "+++ entering parseSect5()";
    
    QDomText data;
    Sect5 *sect5 = new Sect5(item, NULL, i18n("Section Level 4"));
    sect5->setText(3, QString("%1").arg(++m_idCounter).rightJustified(8,'0') );
    // register node
    NodeList_type list;
    list.clear();
    list.push_back( element );
    sect5->setNodes( list );
    // register Popup menu
    m_contextmenuhandler->registerPopupMenu( sect5 );

    QDomNode node = element.firstChild();
    while( !node.isNull() ){
        if( node.isElement() && node.nodeName().toLower()=="title" ){
            data = node.firstChild().toText();
            if( !data.isNull() ){
                sect5->setText( 0, data.nodeValue() );
            }
        } else if ( node.isElement() && node.nodeName().toLower()=="para" ){
            parsePara( node.toElement(), sect5 );
        }
        node = node.nextSibling();
    }
    kDebug(100200) << "--- leaving parseSect5()";
}


void DocTreeViewImpl::parsePara(const QDomElement &element, ListViewInterface *item)
{
    kDebug(100200) << "+++ entering parsePara()";

    Para *para = new Para(item, NULL, i18n("Paragraph"));
    para->setText(3, QString("%1").arg(++m_idCounter).rightJustified(8,'0') );
    // register node
    NodeList_type list;
    list.clear();
    list.push_back( element );
    para->setNodes( list );
    // register Popup menu
    m_contextmenuhandler->registerPopupMenu( para );
    
    kDebug(100200) << "--- leaving parsePara()";
}


