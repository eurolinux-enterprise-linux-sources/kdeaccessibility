//
// C++ Implementation: kttsdplugin
//
// Description: 
//
//
// Author: Robert Vogl <voglrobe@lapislazuli>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//
// #include <time.h> // nanosleep

// Qt includes
#include <QtGui/QFrame>

// KDE includes
#include <kglobal.h>
#include <klocale.h>
#include <kstandarddirs.h>
#include <k3process.h>
#include <kmessagebox.h>
#include <kdebug.h>

// App specific includes
#include "kttsdlib.h"
#include "kttsdlibtalker2.h"
// #include "kttsdlibsetupimpl.h"

KTTSDLib::KTTSDLib(QObject *parent, const char *name, KApplication *Appl)
 : QObject(parent), m_Appl(Appl), m_gui(NULL)
{
    setObjectName(name);
    KGlobal::locale()->insertCatalog("libKTTSD");
    m_talker = new kttsdlibtalker2(static_cast<QObject*>(this), "kttsdlibtalker");
    connect(m_talker, SIGNAL(signalTextFinished(const uint)),
        this, SLOT(slotTextFinished(const uint)) );
    connect(m_talker, SIGNAL(signalTextStopped(const uint)),
        this, SLOT(slotTextStopped(const uint)) );
    connect(m_talker, SIGNAL(signalTextStarted(const uint)),
        this, SLOT(slotTextStarted(const uint)) );
    
    // reset list of currently processed jobs
    while ( !jobList.empty() ){
        jobList.pop();
    }
    
    // initialize the talker
    m_talker->KTTSD_init(m_Appl);
    
    // some presets
    // m_config = new KSimpleConfig("ksayit_kttsdpluginrc");
    // m_usersetupimpl = NULL;
}


KTTSDLib::~KTTSDLib()
{
    // delete m_config;
}


QString KTTSDLib::getName() const
{
    return "KDE KTTSD";
}


QString KTTSDLib::getDescription() const
{
    QString str;
    str =  i18n("<qt><big><u>Description:</u></big><br>");
    str += i18n("This plugin uses the KDE TTS Daemon for speech output.");
        
    return str;
}


int KTTSDLib::getActions()
{
    return ACTIONS::PLAY | ACTIONS::STOP | ACTIONS::PAUSE | ACTIONS::FFWD | ACTIONS::FREV;
}


int KTTSDLib::getStatus() const
{
    return TTS::AUDIOFILE;
}
    

const QWidget* KTTSDLib::getGUI(QFrame *frame)
{
    kDebug(100200) << "KTTSDLib::getGUI()";
    QWidget *w = new QWidget(frame);
    m_gui = new Ui::KTTSDlibSetup();
    m_gui->setupUi(w);
    connect(m_gui->kcm_Button, SLOT(clicked()),
        this, SLOT(slotLaunchControlcenter()));
    return w;
}


void KTTSDLib::reloadConfiguration()
{
    kDebug(100200) << "KTTSDLib::reloadConfiguration()";
        // N.A.
}


bool KTTSDLib::saveWasClicked() const
{
    kDebug(100200) << "KTTSDLib::saveWasClicked()";
        // N.A.
    
    return true;
}

void KTTSDLib::slotLaunchControlcenter()
{
    kDebug(100200) << "KTTSDlibSetupImpl::slotLaunchControlCenter()";

    // check if controllcenter module for KTTSD exists
    FILE *fp;
    char cmdresult[20];

    // TODO: Don't launch kcmshell4 kcmkttsd if already active.
    // bool configActive = (QDBus::sessionBus().busService()->nameHasOwner("org.kde.kcmshell_kcmkttsd"));

    // if ( (fp = popen("kcmshell4 --list | grep kcmkttsmgr", "r")) != NULL){
    if ( (fp = popen("kcmshell4 --list | grep kcmkttsd", "r")) != NULL){
        fgets(cmdresult, 18, fp);
        pclose(fp);
    }
    if ( !QByteArray(cmdresult).contains("kcmkttsd") ){
            QString error = i18n("Control Center Module for KTTSD not found.");
            KMessageBox::sorry(m_Appl->activeWindow(), error, i18n("Problem"));
            return;
    }

    // invoke the Control Center Module
    K3Process *kcmproc = new K3Process();
    connect(kcmproc, SIGNAL(processExited(K3Process*)),
                    this, SLOT(slotKCMProcessExited(K3Process*)) );
    (*kcmproc) << "kcmshell4";
    (*kcmproc) << "kcmkttsd";
    (*kcmproc) << "--caption" << i18n("KDE Text-to-Speech");
    kcmproc->start(K3Process::NotifyOnExit);

    m_gui->kcm_Button->setEnabled(false);
}


void KTTSDLib::slotKCMProcessExited(K3Process *p)
{
    Q_UNUSED(p);
    kDebug(100200) << "slotKCMProcessExited()";
    m_gui->kcm_Button->setEnabled(true);
}


void KTTSDLib::setText(const QString &text)
{
    kDebug(100200) << "KTTSDLib::setText()";
    
    uint jobNum = m_talker->KTTSD_setText(text, "");
    jobList.push(jobNum);    
}
    


void KTTSDLib::sayText()
{
    kDebug(100200) << "KTTSDLib::sayText()";
     
    if( !jobList.empty() ){
        m_curJobNum = jobList.front();
        jobList.pop();
        kDebug(100200) << "   Starting job No.: " << m_curJobNum;
        m_talker->KTTSD_startText( m_curJobNum );
    }
}
    

void KTTSDLib::removeAllJobsFromList()
{
    kDebug(100200) << "KTTSDLib::removeAllJobsFromList()";
    
    m_talker->KTTSD_removeText(m_curJobNum);    
    
    while( !jobList.empty() ){
        uint job = jobList.front();
        jobList.pop();
        kDebug(100200) << "*** removing... " << job;
        m_talker->KTTSD_removeText( job );
    }
}

void KTTSDLib::stop()
{
    kDebug(100200) << "***** KTTSDLib::stop(" << m_curJobNum << ")";
    m_talker->KTTSD_stopText(m_curJobNum);
    removeAllJobsFromList();
    emit signalFinished();
}


void KTTSDLib::pause()
{
    kDebug(100200) << "void KTTSDLib::pause(" << m_curJobNum << ")";
    
    m_talker->KTTSD_pauseText( m_curJobNum );
}
    
    
void KTTSDLib::resume()
{
    kDebug(100200) << "void KTTSDLib::resume(" << m_curJobNum << ")";
    
    m_talker->KTTSD_resumeText( m_curJobNum );
}


void KTTSDLib::ffwd()
{
    kDebug(100200) << "void KTTSDLib::ffwd(" << m_curJobNum << ")";
    m_talker->KTTSD_moveRelTextSentence(1, m_curJobNum);
}


void KTTSDLib::frev()
{
    kDebug(100200) << "void KTTSDLib::frev(" << m_curJobNum << ")";
    m_talker->KTTSD_moveRelTextSentence(-1, m_curJobNum);
}


//////////////////////////////////////
// Signals from the talker
//////////////////////////////////////

void KTTSDLib::slotTextFinished(const uint job)
{
    kDebug(100200) << "---- KTTSDLib::slotTextFinished(" << job << ")";
    
    // check if List is empty. If yes, send signalFinished().
    if ( jobList.empty() ){
        kDebug(100200) << "    All jobs processed.";
        emit signalFinished();
    } else {
        sayText();
    }
}
    
void KTTSDLib::slotTextStopped(const uint job)
{
    kDebug(100200) << "---- KTTSDLib::slotTextStopped(" << job << ")";
    // removeAllJobsFromList();
    // emit signalFinished();
}


void KTTSDLib::slotTextStarted(const uint job)
{
    kDebug(100200) << "---- KTTSDLib::slotTextStarted(" << job << ")";
    // m_curJobNum = job;    
}

#include "kttsdlib.moc"
