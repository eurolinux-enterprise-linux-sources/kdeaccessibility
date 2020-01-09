//
// C++ Interface: docbookclasses.h
//
// Description: 
//
//
// Author: Robert Vogl <voglrobe@lapislazuli>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef DOCBOOKCLASSES_H
#define DOCBOOKCLASSES_H

// Qt includes
#include <QtXml/qdom.h>

// KDE includes
#include <k3listview.h>
 
// App specific includes
#include "Types.h"

// forward declarations

//
// Classes for Roberts elements
//
/**
 Represents the root item of the document, therefore it expects
 a \p QListView object as parent.\n
 Structural item only (no references to a DOM node).
 @author Robert Vogl
 */
class RobDocument : public ListViewInterface
{
public:
    /**
     * Constructor
     * \param lv The ListView parent.
     * \param label The label is shown in column 0 of the TreeView and is also
     * used as bookmark title.
     */
    RobDocument(K3ListView *lv=0, QString label=QString());
    ~RobDocument();

    void setNodes( const NodeList_type &list ){m_list = list;};
    void getNodes( NodeList_type &list ) const {list = m_list;};    
    QString getRTFHeader() const;
    QString getHeader() const ;
    QString getRTFData() const {return QString();};
    QString getData() const {return QString();};
    QString getRawData() const {return QString();};
    void setData(const QString &, QDomDocument &) { /** structural element only */ };
    void setEditable(bool editable) {m_iseditable = editable;};    
    bool isEditable() const {return m_iseditable;};
    
    void deleteAction() {};  
    QString getXmlContextName() const {return "RobDocument";};
    
private:
    NodeList_type m_list; 
    bool m_iseditable;
};


//
// Classes for Docbook elements
//
/**
 Represents the DocBook \p Overview element of the document.\n
 Structural item only (no references to a DOM node).
 @author Robert Vogl
 */
class Overview : public ListViewInterface
{
public:
    Overview(ListViewInterface *parent=0, ListViewInterface *after=0, QString label=QString());
    ~Overview();

    void setNodes( const NodeList_type &list ){m_list = list;};
    void getNodes( NodeList_type &list ) const {list = m_list;};    
    QString getRTFHeader() const;
    QString getHeader() const;
    QString getRTFData() const {return QString();};
    QString getData() const {return QString();};
    QString getRawData() const {return QString();};
    void setData(const QString &, QDomDocument &) { /** structural element only */ };
    void setEditable(bool editable) {m_iseditable = editable;};    
    bool isEditable() const {return m_iseditable;};
    
    void deleteAction() {};
    QString getXmlContextName() const {return "BookInfo";};

private:
    NodeList_type m_list;
    bool m_iseditable;
};


/**
 Represents the DocBook \p Date element of the document.\n
 @author Robert Vogl
 */
class Date : public ListViewInterface
{
public:
    Date(ListViewInterface *parent=0, ListViewInterface *after=0, QString label=QString());
    ~Date();

    void setNodes( const NodeList_type &list );
    void getNodes( NodeList_type &list ) const;    
    QString getRTFHeader() const;
    QString getHeader() const;
    QString getRTFData() const;
    QString getData() const;
    QString getRawData() const;
    void setData(const QString &data, QDomDocument &);
    void setEditable(bool editable) {m_iseditable = editable;};    
    bool isEditable() const {return m_iseditable;};
    
    void deleteAction() {};  
    QString getXmlContextName() const {return "Date";};  

private:
    // Pointer to the associated Data
    QDomNode m_node;
    bool m_iseditable;
};


/**
 Represents the DocBook \p ReleasInfo element of the document.\n
 @author Robert Vogl
 */
class ReleaseInfo : public ListViewInterface
{
public:
    ReleaseInfo(ListViewInterface *parent=0, ListViewInterface *after=0, QString label=QString());
    ~ReleaseInfo();

    void setNodes( const NodeList_type &list );
    void getNodes( NodeList_type &list ) const;    
    QString getRTFHeader() const;
    QString getHeader() const;
    QString getRTFData() const;
    QString getData() const;
    QString getRawData() const;
    void setData(const QString &data, QDomDocument &);
    void setEditable(bool editable) {m_iseditable = editable;};    
    bool isEditable() const {return m_iseditable;};
    
    void deleteAction() {};  
    QString getXmlContextName() const {return "ReleaseInfo";};  

private:
    // Pointer to the associated Data
    QDomNode m_node;
    bool m_iseditable;
};


/**
 Represents the DocBook \p AuthorGroup element of the document.\n
 Structural item only (no references to a DOM node).
 @author Robert Vogl
 */
class AuthorGroup : public ListViewInterface
{
public:
    AuthorGroup(ListViewInterface *parent=0, ListViewInterface *after=0, QString label=QString());
    ~AuthorGroup();
    
    void setNodes( const NodeList_type &list ){m_list = list;};
    void getNodes( NodeList_type &list ) const {list = m_list;};    
    QString getRTFHeader() const;     
    QString getHeader() const;    
    QString getRTFData() const {return QString();};
    QString getData() const {return QString();};
    QString getRawData() const {return QString();};
    void setData(const QString &, QDomDocument &) { /** structural element only */ };
    void setEditable(bool editable) {m_iseditable = editable;};    
    bool isEditable() const {return m_iseditable;};
    
    void deleteAction() {};  
    QString getXmlContextName() const {return "AuthorGroup";};  

private:
    NodeList_type m_list;
    bool m_iseditable;
};


/**
 Represents the DocBook \p Author element and its Subelements
 \p Firstname and \p Surname of the document.\n
 @author Robert Vogl
 */
class Author : public ListViewInterface
{
public:
    Author(ListViewInterface *parent=0, ListViewInterface *after=0, QString label=QString());
    ~Author();

    /**
     * \param node1 = Firstname
     * \param node2 = Surname
     */
    void setNodes( const NodeList_type &list );
    void getNodes( NodeList_type &list ) const;    
    QString getRTFHeader() const;
    QString getHeader() const;
    QString getRTFData() const;
    
    void deleteAction() {};  
    QString getXmlContextName() const {return "Author";};  

    /**
     * \returns Firstname + Space + Surname
     */
    QString getRawData() const;

    /**
     * \returns Firstname + Space + Surname
     */
    QString getData() const;

    /**
     * Expects firstname and surname sperated by one or more space characters
     * and updates the corresponding DOM elements.
     * \param A string in the form Firstname + Space + Surname
     */
    void setData(const QString &data, QDomDocument &);
    void setEditable(bool editable) {m_iseditable = editable;};    
    bool isEditable() const {return m_iseditable;};

private:
    // Pointer to the associated Data
    QDomNode m_node1;
    QDomNode m_node2;
    bool m_iseditable;
};


/**
 Represents the DocBook \p KeywordSet element of the document.\n
 Structural item only (no references to a DOM node).
 @author Robert Vogl
 */
class KeywordSet : public ListViewInterface
{
public:
    KeywordSet(ListViewInterface *parent=0, ListViewInterface *after=0, QString label=QString());
    ~KeywordSet();

    void setNodes( const NodeList_type &list ){m_list = list;};
    void getNodes( NodeList_type &list ) const {list = m_list;};    
    QString getRTFHeader() const;
    QString getHeader() const;
    QString getRTFData() const {return QString();};
    QString getData() const {return QString();};
    QString getRawData() const {return QString();};
    void setData(const QString &, QDomDocument &) { /** structural element only */ };
    void setEditable(bool editable) {m_iseditable = editable;};    
    bool isEditable() const {return m_iseditable;};
    
    void deleteAction() {};  
    QString getXmlContextName() const {return "KeywordSet";};  

private:
    NodeList_type m_list;
    bool m_iseditable;
};


/**
 Represents the DocBook \p Keyword element of the document.\n
 @author Robert Vogl
 */
class Keyword : public ListViewInterface
{
public:
    Keyword(ListViewInterface *parent=0, ListViewInterface *after=0, QString label=QString());
    ~Keyword();

    void setNodes( const NodeList_type &list );
    void getNodes( NodeList_type &list ) const;    
    QString getRTFHeader() const {return QString();};
    QString getHeader() const {return QString();};    
    QString getRTFData() const;
    QString getData() const;
    QString getRawData() const;
    void setData(const QString &data, QDomDocument &);
    void setEditable(bool editable) {m_iseditable = editable;};    
    bool isEditable() const {return m_iseditable;};
    
    void deleteAction() {};  
    QString getXmlContextName() const {return "Keyword";};  

private:    
    // Pointer to the associated Data
    QDomNode m_node;
    bool m_iseditable;
};


/**
 Represents the DocBook \p Abstract element of the document.\n
 Structural item only (no references to a DOM node).
 @author Robert Vogl
 */
class Abstract : public ListViewInterface
{
public:
    Abstract(ListViewInterface *parent=0, ListViewInterface *after=0, QString label=QString());
    ~Abstract();
    
    void setNodes( const NodeList_type &list ){m_list = list;};
    void getNodes( NodeList_type &list ) const {list = m_list;};    
    QString getRTFHeader() const;     
    QString getHeader() const;    
    QString getRTFData() const {return QString();};
    QString getData() const {return QString();};
    QString getRawData() const {return QString();};
    void setData(const QString &, QDomDocument &) { /** structural element only */ };
    void setEditable(bool editable) {m_iseditable = editable;};    
    bool isEditable() const {return m_iseditable;};
    
    void deleteAction() {};  
    QString getXmlContextName() const {return "Abstract";};  

private:
    NodeList_type m_list;
    bool m_iseditable;
};


/**
 Represents a DocBook Paragraph item of the document.\n
 @author Robert Vogl
 */
class Para : public ListViewInterface
{
public:
    Para(ListViewInterface *parent=0, ListViewInterface *after=0, QString label=QString());
    ~Para();
    
    void setNodes( const NodeList_type &list );
    void getNodes( NodeList_type &list ) const;    
    QString getRTFHeader() const;     
    QString getHeader() const;    
    QString getRTFData() const;
    QString getData() const;
    QString getRawData() const;
    
    void deleteAction() { /** TODO */ };  
    QString getXmlContextName() const {return "Para";};  

    /**
     * Embedded XML elements are allowed.
     * Checks if any embedded tags are consistent. If not, the data
     * will be stored as \p CDATA.
     * \param data The new content of a DocBook paragraph without
     * opening and closing \p \<para\> ...\p \</para\> tags.\n
     */
    void setData(const QString &data, QDomDocument &);
    void setEditable(bool editable) {m_iseditable = editable;};    
    bool isEditable() const {return m_iseditable;};

private:    
    // Pointer to the associated Data
    QDomNode m_node;
    bool m_iseditable;
};


/**
 Represents the DocBook \p Chapter element of the document.\n
 Structural item only (no references to a DOM node).
 @author Robert Vogl
 */
class Chapter : public ListViewInterface
{
public:
    Chapter(ListViewInterface *parent=0, ListViewInterface *after=0, QString label=QString());
    ~Chapter();
    
    void setNodes( const NodeList_type &list ){m_list = list;};
    void getNodes( NodeList_type &list ) const {list = m_list;};    
    QString getRTFHeader() const;     
    QString getHeader() const;    
    QString getRTFData() const {return QString();};
    QString getData() const {return QString();};
    QString getRawData() const {return QString();};
    void setData(const QString &, QDomDocument &) { /** structural element only */ };
    void setEditable(bool editable) {m_iseditable = editable;};    
    bool isEditable() const {return m_iseditable;};
    
    void deleteAction() { /** TODO */ };
    QString getXmlContextName() const {return "Chapter";};  

private:    
    // Pointer to the associated Data
    NodeList_type m_list;    
    bool m_iseditable;
};


/**
 Represents the DocBook \p Sec1 element of the document.\n
 Structural item only (no references to a DOM node).
 @author Robert Vogl
 */
class Sect1 : public ListViewInterface
{
public:
    Sect1(ListViewInterface *parent=0, ListViewInterface *after=0, QString label=QString());
    ~Sect1();

    void setNodes( const NodeList_type &list ){m_list = list;};
    void getNodes( NodeList_type &list ) const {list = m_list;};    
    QString getRTFHeader() const;
    QString getHeader() const;
    QString getRTFData() const {return QString();};
    QString getData() const {return QString();};
    QString getRawData() const {return QString();};
    void setData(const QString &, QDomDocument &) { /** structural element only */ };
    void setEditable(bool editable) {m_iseditable = editable;};    
    bool isEditable() const {return m_iseditable;};
    
    void deleteAction() { /** TODO */ };  
    QString getXmlContextName() const {return "Sect1";};  

private:    
    // Pointer to the associated Data
    NodeList_type m_list;
    bool m_iseditable;
};


/**
 Represents the DocBook \p Sec2 element of the document.\n
 Structural item only (no references to a DOM node).
 @author Robert Vogl
 */
class Sect2 : public ListViewInterface
{
public:
    Sect2(ListViewInterface *parent=0, ListViewInterface *after=0, QString label=QString());
    ~Sect2();

    void setNodes( const NodeList_type &list ){m_list = list;};
    void getNodes( NodeList_type &list ) const {list = m_list;};    
    QString getRTFHeader() const;
    QString getHeader() const;
    QString getRTFData() const {return QString();};
    QString getData() const {return QString();};
    QString getRawData() const {return QString();};
    void setData(const QString &, QDomDocument &) { /** structural element only */ };
    void setEditable(bool editable) {m_iseditable = editable;};    
    bool isEditable() const {return m_iseditable;};
    
    void deleteAction() { /** TODO */ };  
    QString getXmlContextName() const {return "Sect2";};  

private:    
    // Pointer to the associated Data
    NodeList_type m_list;
    bool m_iseditable;
};


/**
 Represents the DocBook \p Sec3 element of the document.\n
 Structural item only (no references to a DOM node).
 @author Robert Vogl
 */
class Sect3 : public ListViewInterface
{
public:
    Sect3(ListViewInterface *parent=0, ListViewInterface *after=0, QString label=QString());
    ~Sect3();
    
    void setNodes( const NodeList_type &list ){m_list = list;};
    void getNodes( NodeList_type &list ) const {list = m_list;};    
    QString getRTFHeader() const;     
    QString getHeader() const;    
    QString getRTFData() const {return QString();};
    QString getData() const {return QString();};
    QString getRawData() const {return QString();};
    void setData(const QString &, QDomDocument &) { /** structural element only */ };
    void setEditable(bool editable) {m_iseditable = editable;};    
    bool isEditable() const {return m_iseditable;};
    
    void deleteAction() { /** TODO */ };  
    QString getXmlContextName() const {return "Sect3";};  

private:    
    // Pointer to the associated Data
    NodeList_type m_list;
    bool m_iseditable;
};


/**
 Represents the DocBook \p Sec4 element of the document.\n
 Structural item only (no references to a DOM node).
 @author Robert Vogl
 */
class Sect4 : public ListViewInterface
{
public:
    Sect4(ListViewInterface *parent=0, ListViewInterface *after=0, QString label=QString());
    ~Sect4();
    
    void setNodes( const NodeList_type &list ){m_list = list;};
    void getNodes( NodeList_type &list ) const {list = m_list;};    
    QString getRTFHeader() const;     
    QString getHeader() const;    
    QString getRTFData() const {return QString();};
    QString getData() const {return QString();};
    QString getRawData() const {return QString();};
    void setData(const QString &, QDomDocument &) { /** structural element only */ };
    void setEditable(bool editable) {m_iseditable = editable;};    
    bool isEditable() const {return m_iseditable;};
    
    void deleteAction() { /** TODO */ };  
    QString getXmlContextName() const {return "Sect4";};  

private:    
    // Pointer to the associated Data
    NodeList_type m_list;
    bool m_iseditable;
};


/**
 Represents the DocBook \p Sec5 element of the document.\n
 Structural item only (no references to a DOM node).
 @author Robert Vogl
 */
class Sect5 : public ListViewInterface
{
public:
    Sect5(ListViewInterface *parent=0, ListViewInterface *after=0, QString label=QString());
    ~Sect5();
    
    void setNodes( const NodeList_type &list ){m_list = list;};
    void getNodes( NodeList_type &list ) const {list = m_list;};    
    QString getRTFHeader() const;     
    QString getHeader() const;    
    QString getRTFData() const {return QString();};
    QString getData() const {return QString();};
    QString getRawData() const {return QString();};
    void setData(const QString &, QDomDocument &) { /** structural element only */ };
    void setEditable(bool editable) {m_iseditable = editable;};    
    bool isEditable() const {return m_iseditable;};
    
    void deleteAction() { /** TODO */ };  
    QString getXmlContextName() const {return "Sect5";};  

private:    
    // Pointer to the associated Data
    NodeList_type m_list;
    bool m_iseditable;
};

#endif
