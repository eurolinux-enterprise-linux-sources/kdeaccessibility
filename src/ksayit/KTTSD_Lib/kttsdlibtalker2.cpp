//
// C++ Implementation: kttsdlibtalker2
//
// Description: 
//
//
// Author: Robert Vogl <voglrobe@lapislazuli>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include <unistd.h> //sleep

// Qt includes
#include <QtDBus/QtDBus>

// KDE includes
#include <kdebug.h>

// App specific includes
#include "kttsdlibtalker2.h"
#include <ktoolinvocation.h>

kttsdlibtalker2::kttsdlibtalker2(QObject *parent, const char *name)
    : QObject(parent)
{
    setObjectName(name);
}


kttsdlibtalker2::~kttsdlibtalker2()
{
}


void kttsdlibtalker2::KTTSD_init(KApplication *Appl)
{
    Q_UNUSED(Appl);
    kDebug(100200) << "kttsdlibtalker2::KTTSD_init()";
#if 0 
    bool kttsdRunning = (QDBusConnection::sessionBus().interface()->isServiceRegistered("org.kde.kttsd"));
    if (!kttsdRunning)
    {
        // kDebug() << "KCMKttsMgr::slotEnableKttsd_toggled:: Starting KTTSD";
        QString error;
        if (KToolInvocation::startServiceByDesktopName("kttsd", QStringList(), &error))
            // throw(error);
            kDebug() << "Starting KTTSD failed with message " << error;
    }
    m_kspeech = (org::kde::KSpeech*)(QDBusConnection::sessionBus().findInterface<org::kde::KSpeech>("org.kde.kttsd", "/org/kde/KSpeech"));
    m_kspeech->setParent(this);
    // Connect KTTSD DBUS signals to our slots.
    connect(m_kspeech, SIGNAL(textStarted(const QString&, uint)),
        this, SLOT(textStarted(const QString&, uint)));
    connect(m_kspeech, SIGNAL(textFinished(const QString&, uint)),
        this, SLOT(textFinished(const QString&, uint)));
    connect(m_kspeech, SIGNAL(textStopped(const QString&, uint)),
        this, SLOT(textStopped(const QString&, uint)));
    m_appId = QDBus::sessionBus().baseService();
#endif
    kDebug(100200) << "    successfully registered. appId: " << m_appId; 
}


uint kttsdlibtalker2::KTTSD_setText(const QString &text, QString lang)
{
#if 0	
    kDebug(100200) << "kttsdlibtalker2::KTTSD_setText()";
    uint jobNum = m_kspeech->setText(text, lang);
    return jobNum;
#endif
}


void kttsdlibtalker2::KTTSD_startText(uint jobNum)
{
#if 0	
    kDebug(100200) << "kttsdlibtalker2::KTTSD_startText()";
    m_kspeech->startText(jobNum);
#endif
}


void kttsdlibtalker2::KTTSD_stopText(uint jobNum)
{
#if 0	
    kDebug(100200) << "kttsdlibtalker2::KTTSD_stopText()";
    m_kspeech->stopText(jobNum);
#endif
}


void kttsdlibtalker2::KTTSD_pauseText(uint jobNum)
{
#if 0	
    kDebug(100200) << "kttsdlibtalker2::KTTSD_pauseText()";
    m_kspeech->pauseText(jobNum);
#endif
}


void kttsdlibtalker2::KTTSD_resumeText(uint jobNum)
{
#if 0	
    kDebug(100200) << "kttsdlibtalker2::KTTSD_resumeText()";
    m_kspeech->resumeText(jobNum);
#endif
}


uint kttsdlibtalker2::KTTSD_moveRelTextSentence(const int n, const uint jobNum)
{
#if 0	
    kDebug(100200) << "kttsdlibtalker2::KTTSD_moveRelTextSentence()";
    uint seqNum = m_kspeech->moveRelTextSentence(n, jobNum);
    return seqNum;
#endif
}


void kttsdlibtalker2::KTTSD_removeText(uint jobNum)
{
#if 0	
    kDebug(100200) << "kttsdlibtalker2::KTTSD_removeText()";
    m_kspeech->removeText(jobNum);
#endif
}


int kttsdlibtalker2::KTTSD_getTextJobState(uint jobNum)
{
    Q_UNUSED(jobNum);
    return 0;
}


int kttsdlibtalker2::KTTSD_getCurrentTextJob()
{

    return 0;
}



////////////////////////////////////////////
// DCOP signals
////////////////////////////////////////////    

void kttsdlibtalker2::textStarted(const QString &appID,
        const uint jobNum)
{
    kDebug(100200) << "*** kttsdlibtalker2::textStarted()";
    if ( appID != m_appId )
        return;
    emit signalTextStarted(jobNum);
}


void kttsdlibtalker2::textFinished(const QString &appID,
        const uint jobNum)
{
    kDebug(100200) << "*** kttsdlibtalker2::textFinished()";
    if ( appID != m_appId )
        return;
    emit signalTextFinished(jobNum);
}


void kttsdlibtalker2::textStopped(const QString &appID,
        const uint jobNum)
{
    kDebug(100200) << "*** kttsdlibtalker2::textStopped()";
    if ( appID != m_appId )
        return;
    emit signalTextStopped(jobNum);
}

#include "kttsdlibtalker2.moc"
