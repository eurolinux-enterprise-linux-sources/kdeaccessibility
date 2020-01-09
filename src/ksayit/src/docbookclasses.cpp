//
// C++ Implementation: docbookclasses
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
#include <QtGui/QWidget>
#include <QtCore/QTextStream>
#include <QtGui/QPixmap>
#include <QtCore/QRegExp>

// tests
#include <QtCore/QFile>

//KDE includes
#include <kdebug.h>
#include <kglobal.h>
#include <klocale.h>
#include <kiconloader.h>
#include <kmessagebox.h>

// App specific includes
#include "parasaxparser.h"
#include "docbookclasses.h"

//////////////////////////////////////
// Interface
//////////////////////////////////////
ListViewInterface::ListViewInterface(ListViewInterface *parent, QString label)
    : K3ListViewItem( parent, label )
{
}

ListViewInterface::ListViewInterface(ListViewInterface *parent, ListViewInterface *after, QString label) : K3ListViewItem( parent, after, label )
{
}

ListViewInterface::ListViewInterface(K3ListView *lv, QString label)
    : K3ListViewItem( lv, label )
{
}


//////////////////////////////////////
// Classes for Roberts elements
//////////////////////////////////////
/**
 * RobDocument
 */
RobDocument::RobDocument(K3ListView *lv, QString label)
    : ListViewInterface( lv, label )
{
    QPixmap pixmap = KIconLoader::global()->loadIcon("help-contents", KIconLoader::Small);
    this->setPixmap(0, pixmap);
    m_iseditable = false;   
}

RobDocument::~RobDocument()
{
}

QString RobDocument::getRTFHeader() const
{
    QString header = "<H1><center>" + this->text(0) + "</center></H1>";
    return header;
}

QString RobDocument::getHeader() const
{
    QString header = this->text(0) + '\n';
    return header;
}



//////////////////////////////////////
// Classes for Docbook elements
//////////////////////////////////////
/**
 * Overview
 */
Overview::Overview(ListViewInterface *parent, ListViewInterface *after, QString label)
    : ListViewInterface(parent, after, label)
{
    QPixmap pixmap = KIconLoader::global()->loadIcon("document-new", KIconLoader::Small);
    this->setPixmap(0, pixmap);
    this->setText(3, "0"); // upmost element
    m_iseditable = false;   
}

Overview::~Overview()
{
}

QString Overview::getRTFHeader() const
{
    QString header = "<H1>" + this->text(0) + "</H1>";
    return header;
}

QString Overview::getHeader() const
{
    QString header = this->text(0) + '\n';
    return header;
}


/**
 * Date
 */
Date::Date(ListViewInterface *parent, ListViewInterface *after, QString label)
    : ListViewInterface(parent, after, label)
{
    QPixmap pixmap = KIconLoader::global()->loadIcon("document-new", KIconLoader::Small);
    this->setPixmap(0, pixmap);
    this->setText(3, "00000001");
    m_node = QDomNode();
    m_iseditable = false;   
}

Date::~Date()
{
}
    
void Date::setNodes( const NodeList_type &list )
{
    if ( !list.empty() )
        m_node = list.front();
}

void Date::getNodes( NodeList_type &list ) const
{
    list.push_back( m_node );
}

QString Date::getRTFHeader() const
{
    QString header = "<B>" + this->text(0) + ": </B>";
    return header;
}

QString Date::getHeader() const
{
    QString header = this->text(0) + ": ";
    return header;
}

QString Date::getRTFData() const
{
    if ( !m_node.isNull() ){
        QString date = m_node.nodeValue() + "<br>"; 
        return date;
    } else
        return QString();
}

QString Date::getData() const
{
    if ( !m_node.isNull() ){
        return m_node.nodeValue();
    } else
        return QString();
}
    
QString Date::getRawData() const
{
    QString ret;
    
    // get content of element
    QTextStream in(&ret, QIODevice::WriteOnly);
    in.setEncoding(QTextStream::UnicodeUTF8);
    in << m_node;
    
    return ret;
}

void Date::setData(const QString &data, QDomDocument &)
{
    m_node.setNodeValue(data);
    this->setText(1, data); // update ListView
}


/**
 * ReleaseInfo
 */
ReleaseInfo::ReleaseInfo(ListViewInterface *parent, ListViewInterface *after, QString label)
    : ListViewInterface(parent, after, label)
{
    QPixmap pixmap = KIconLoader::global()->loadIcon("document-new", KIconLoader::Small);
    this->setPixmap(0, pixmap);
    this->setText(3, "00000002");
    m_node = QDomNode();
    m_iseditable = false;   
}

ReleaseInfo::~ReleaseInfo()
{
}
    
void ReleaseInfo::setNodes( const NodeList_type &list )
{
    if ( !list.empty() )
        m_node = list.front();
}

void ReleaseInfo::getNodes( NodeList_type &list ) const
{
    list.push_back( m_node );
}

QString ReleaseInfo::getRTFHeader() const
{
    QString header = "<B>" + this->text(0) + ": </B>";
    return header;
}

QString ReleaseInfo::getHeader() const
{
    QString header = this->text(0) + ": ";
    return header;
}

QString ReleaseInfo::getRTFData() const
{
    if ( !m_node.isNull() ){
        QString releaseinfo = m_node.nodeValue() + "<br>"; 
        return releaseinfo;
    } else
        return QString();
}

QString ReleaseInfo::getData() const
{
    if ( !m_node.isNull() ){
        return m_node.nodeValue();
    } else
        return QString();
}
    
QString ReleaseInfo::getRawData() const
{
    QString ret;
    
    // get content of element
    QTextStream in(&ret, QIODevice::WriteOnly);
    in.setEncoding(QTextStream::UnicodeUTF8);
    in << m_node;
    
    return ret;
}

void ReleaseInfo::setData(const QString &data, QDomDocument &)
{
    m_node.setNodeValue(data);
    this->setText(1, data); // update ListView
}


/**
 * Authorgroup
 */
AuthorGroup::AuthorGroup(ListViewInterface *parent, ListViewInterface *after, QString label)
    : ListViewInterface(parent, after, label)
{
    QPixmap pixmap = KIconLoader::global()->loadIcon("kdmconfig", KIconLoader::Small);
    this->setPixmap(0, pixmap);
    this->setText(3, "00000003");
    m_iseditable = false;   
}

AuthorGroup::~AuthorGroup()
{
}
    
QString AuthorGroup::getRTFHeader() const
{
    QString header = "<H2>" + this->text(0) + "</H2>";
    return header;
}

QString AuthorGroup::getHeader() const
{
    QString header = this->text(0) + '\n';
    return header;
}


/**
 * Author
 */
Author::Author(ListViewInterface *parent, ListViewInterface *after, QString label)
    : ListViewInterface(parent, after, label)
{
    QPixmap pixmap = KIconLoader::global()->loadIcon("personal", KIconLoader::Small);
    this->setPixmap(0, pixmap);
    m_node1 = QDomNode();
    m_node2 = QDomNode();    
    m_iseditable = false;   
}

Author::~Author()
{
}
    
void Author::setNodes( const NodeList_type &list )
{
    NodeList_type l_list = list;
    NodeList_type::iterator it = l_list.begin();
    m_node1 = *it;
    m_node2 = *(++it);
}

void Author::getNodes( NodeList_type &list ) const
{
    list.push_back( m_node1 );
    list.push_back( m_node2 );
}


QString Author::getRTFHeader() const
{
    QString header = "<B>" + this->text(0) + ": </B>";
    return header;
}

QString Author::getHeader() const
{
    QString author = this->text(0) + ": ";
    return author;
}

QString Author::getRTFData() const
{
    QString ret = QString();
    if ( !m_node1.isNull() ){
        ret = m_node1.nodeValue() + ' ';
    }
    
    if ( !m_node2.isNull() ){
        ret += m_node2.nodeValue() + "<br>";
    }
    return ret;
}

QString Author::getData() const
{
    QString ret = QString();
    if ( !m_node1.isNull() ){
        ret = m_node1.nodeValue() + ' ';
    }
    
    if ( !m_node2.isNull() ){
        ret += m_node2.nodeValue();
    }
    return ret;
}
    
QString Author::getRawData() const
{
    QString ret = QString();
    if ( !m_node1.isNull() ){
        ret = m_node1.nodeValue() + ' ';
    }
    
    if ( !m_node2.isNull() ){
        ret += m_node2.nodeValue();
    }
    return ret;
}

void Author::setData(const QString &data, QDomDocument &)
{
    // canonify string
    QString m_data = data;
    m_data.replace( QRegExp("\n"), "" ); // remove Newlines
    m_data.replace( QRegExp(" {2,}"), " " ); // remove multiple spaces
    m_data.replace( QRegExp("[\t|\r]{1,}"), ""); // remove Tabs
    // split string "firstname surname"
    QString firstname = m_data.section(' ', 0, 0);
    QString surname = m_data.section(' ', 1, 1);
    // update node elements
    m_node1.setNodeValue(firstname);
    m_node2.setNodeValue(surname);
    // update Listview
    this->setText(1, firstname + ' ' + surname);
}


/**
 * KeywordSet
 */
KeywordSet::KeywordSet(ListViewInterface *parent, ListViewInterface *after, QString label)
    : ListViewInterface(parent, after, label)
{
    QPixmap pixmap = KIconLoader::global()->loadIcon("txt", KIconLoader::Small);
    this->setPixmap(0, pixmap);
    this->setText(3, "00000004");
    m_iseditable = false;   
}

KeywordSet::~KeywordSet()
{
}
    
QString KeywordSet::getRTFHeader() const
{
    QString header = "<H2>" + this->text(0) + "</H2>";
    return header;
}

QString KeywordSet::getHeader() const
{
    QString header = this->text(0) + '\n';
    return header;
}


/**
 * Keyword
 */
Keyword::Keyword(ListViewInterface *parent, ListViewInterface *after, QString label)
    : ListViewInterface(parent, after, label)
{
    m_node = QDomNode();
    m_iseditable = false;   
}

Keyword::~Keyword()
{
}
    
void Keyword::setNodes( const NodeList_type &list )
{
    if ( !list.empty() )
        m_node = list.front();
}

void Keyword::getNodes( NodeList_type &list ) const
{
    list.push_back( m_node );
}    

QString Keyword::getRTFData() const
{
    if ( !m_node.isNull() ){
        QString keyword = m_node.nodeValue() + "<br>"; 
        return keyword;
    } else
        return QString();
}

QString Keyword::getData() const
{
    if ( !m_node.isNull() ){
        return m_node.nodeValue();
    } else
        return QString();
}

QString Keyword::getRawData() const
{
    QString ret;
    
    // get content of element
    QTextStream in(&ret, QIODevice::WriteOnly);
    in.setEncoding(QTextStream::UnicodeUTF8);
    in << m_node;
    
    return ret;
}

void Keyword::setData(const QString &data, QDomDocument &)
{
    m_node.setNodeValue(data);
    this->setText(0, data); // update ListView
}


/**
 * Abstract
 */
Abstract::Abstract(ListViewInterface *parent, ListViewInterface *after, QString label)
    : ListViewInterface(parent, after, label)
{
    QPixmap pixmap = KIconLoader::global()->loadIcon("document-new", KIconLoader::Small);
    this->setPixmap(0, pixmap);
    this->setText(3, "00000005");
    m_iseditable = false;   
}

Abstract::~Abstract()
{
}

QString Abstract::getRTFHeader() const
{
    QString header = "<H2>" + this->text(0) + "</H2>";
    return header;
}

QString Abstract::getHeader() const
{
    QString header = this->text(0) + '\n';
    return header;
}


/**
 * Para
 */
Para::Para(ListViewInterface *parent, ListViewInterface *after, QString label)
    : ListViewInterface(parent, after, label)
{
    QPixmap pixmap = KIconLoader::global()->loadIcon("leftjust", KIconLoader::Small);
    this->setPixmap(0, pixmap);
    m_node = QDomNode();
    m_iseditable = false;
}

Para::~Para()
{
}

void Para::setNodes( const NodeList_type &list )
{
    if ( !list.empty() )
        m_node = list.front();
}

void Para::getNodes( NodeList_type &list ) const
{
    list.push_back( m_node );
}

QString Para::getRTFHeader() const
{
    return QString();
}

QString Para::getHeader() const
{
    QString header = this->text(0) + '\n';
    return header;
}

QString Para::getRTFData() const
{
    // get content of element
    QString paragraph;
    QTextStream in(&paragraph, QIODevice::WriteOnly);
    in.setEncoding(QTextStream::UnicodeUTF8);
    in << m_node;
    
    // delegate job to SAX-Parser
    QString data;
    QString ret;
    
    ParaSaxParser *p = ParaSaxParser::Instance();
    p->setData( paragraph );
    p->getRTFText( data );
    ret = "<p>" + data + "</p>";
    
    return ret;
}

QString Para::getData() const
{
    // get content of element
    QString paragraph;
    QTextStream in(&paragraph, QIODevice::WriteOnly);
    in.setEncoding(QTextStream::UnicodeUTF8);
    in << m_node;
    
    // delegate job to SAX-Parser
    QString data;
    
    ParaSaxParser *p = ParaSaxParser::Instance();
    p->setData( paragraph );
    p->getText( data );
    
    return data;
}

QString Para::getRawData() const
{
    QString ret;
    
    // get content of element
    QTextStream in(&ret, QIODevice::WriteOnly);
    in.setEncoding(QTextStream::UnicodeUTF8);
    in << m_node;
    // remove opening and closing para-tags
    ret.replace( QRegExp("</?(para|Para|PARA)/?>"),"");
    ret.replace( QRegExp("^ "),"" );
    ret.replace( QRegExp("^\n"), "" );

    return ret;
}

void Para::setData(const QString &data, QDomDocument &)
{
    kDebug(100200) << "Para::setData()";

    // add tags
    QString l_data = "<para>\n" + data + "\n</para>";

    // create a temporary document
    QString err;
    QDomDocument tempDoc;
    if ( !tempDoc.setContent(l_data) ){
        l_data = "<para><![CDATA[\n" + data + "\n]]></para>";
        // try again
        if ( !tempDoc.setContent(l_data) ){
            err = i18n("Unable to make document persistent.");
            kDebug() << err;
            // throw err;
            return;
        }
        err = i18n("This paragraph contains mismatching tags. It will will be handled as CDATA.");
        KMessageBox::information(0, err, i18n("XML-Error"));
    }

    // get parent
    QDomNode parent = m_node.parentNode();
    if ( parent.isNull() ){
        kDebug(100200) << "Para::setData(): Structural DocBook format error.";
        return;
    }

    // replace Node
    parent.replaceChild(tempDoc, m_node);
    m_node = tempDoc;
}


/**
 * Chapter
 */
Chapter::Chapter(ListViewInterface *parent, ListViewInterface *after, QString label)
    : ListViewInterface(parent, after, label)
{
    QPixmap pixmap = KIconLoader::global()->loadIcon("leftjust", KIconLoader::Small);
    this->setPixmap(0, pixmap);
    m_iseditable = false;
}

Chapter::~Chapter()
{
}

QString Chapter::getRTFHeader() const
{
    QString header = "<H1>" + this->text(0) + "</H1>";
    return header;
}

QString Chapter::getHeader() const
{
    QString header = this->text(0) + '\n';
    return header;
}


/**
 * Sect1
 */
Sect1::Sect1(ListViewInterface *parent, ListViewInterface *after, QString label)
    : ListViewInterface(parent, after, label)
{
    QPixmap pixmap = KIconLoader::global()->loadIcon("leftjust", KIconLoader::Small);
    this->setPixmap(0, pixmap);
    m_iseditable = false;
}

Sect1::~Sect1()
{
}
    
QString Sect1::getRTFHeader() const
{
    QString header = "<H2>" + this->text(0) + "</H2>";
    return header;
}

QString Sect1::getHeader() const
{
    QString header = this->text(0) + '\n';
    return header;
}


/**
 * Sect2
 */
Sect2::Sect2(ListViewInterface *parent, ListViewInterface *after, QString label)
    : ListViewInterface(parent, after, label)
{
    QPixmap pixmap = KIconLoader::global()->loadIcon("leftjust", KIconLoader::Small);
    this->setPixmap(0, pixmap);
    m_iseditable = false;
}

Sect2::~Sect2()
{
}

QString Sect2::getRTFHeader() const
{
    QString header = "<H3>" + this->text(0) + "</H3>";
    return header;
}

QString Sect2::getHeader() const
{
    QString header = this->text(0) + '\n';
    return header;
}


/**
 * Sect3
 */
Sect3::Sect3(ListViewInterface *parent, ListViewInterface *after, QString label)
    : ListViewInterface(parent, after, label)
{
    QPixmap pixmap = KIconLoader::global()->loadIcon("leftjust", KIconLoader::Small);
    this->setPixmap(0, pixmap);
    m_iseditable = false;
}

Sect3::~Sect3()
{
}

QString Sect3::getRTFHeader() const
{
    QString header = "<B>" + this->text(0) + "</B><br>";
    return header;
}

QString Sect3::getHeader() const
{
    QString header = this->text(0) + '\n';
    return header;
}


/**
 * Sect4
 */
Sect4::Sect4(ListViewInterface *parent, ListViewInterface *after, QString label)
    : ListViewInterface(parent, after, label)
{
    QPixmap pixmap = KIconLoader::global()->loadIcon("leftjust", KIconLoader::Small);
    this->setPixmap(0, pixmap);
    m_iseditable = false;
}

Sect4::~Sect4()
{
}
    
QString Sect4::getRTFHeader() const
{
    QString header = "<B>" + this->text(0) + "</B><br>";
    return header;
}

QString Sect4::getHeader() const
{
    QString header = this->text(0) + '\n';
    return header;
}


/**
 * Sect5
 */
Sect5::Sect5(ListViewInterface *parent, ListViewInterface *after, QString label)
    : ListViewInterface(parent, after, label)
{
    QPixmap pixmap = KIconLoader::global()->loadIcon("leftjust", KIconLoader::Small);
    this->setPixmap(0, pixmap);
    m_iseditable = false;
}

Sect5::~Sect5()
{
}

QString Sect5::getRTFHeader() const
{
    QString header = "<B>" + this->text(0) + "</B><br>";
    return header;
}

QString Sect5::getHeader() const
{
    QString header = this->text(0) + '\n';
    return header;
}

