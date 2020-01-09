//
// C++ Interface: kttsdplugin
//
// Description: 
//
//
// Author: Robert Vogl <voglrobe@lapislazuli>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef KTTSDLIB_H
#define KTTSDLIB_H

// stl
#include <queue>
using namespace std;

// QT includes
#include <QtCore/QObject>
#include <QtGui/QWidget>
#include <Qt3Support/Q3Frame>

// KDE includes
#include <kdemacros.h>
#include "kdeexportfix.h"
#include <KApplication>
// App specific includes
#include "ui_KTTSDlibSetup.h"

namespace TTS {
    enum {AUDIOFILE=1};
}

namespace ACTIONS {
    enum {PLAY=1, STOP=2, PAUSE=4, FFWD=8, FREV=16};
}


// forward declarations
class kttsdlibtalker2;
class K3Process;

/**
@author Robert Vogl
*/
class KDE_EXPORT KTTSDLib : public QObject
{
Q_OBJECT   
signals:
    /** Emitted when the plugin has been finished speeking.
     */
    void signalFinished();
    
public slots:
    /** Called from the talker on textFinished().
     * \param job The number of the finished job.
     */
    void slotTextFinished(const uint job);
    
    /** Called from the talker on textStopped().
     * \param job The number of the stopped job.
     */
    void slotTextStopped(const uint job);
    
    /** Called from the talker on textStarted().
     * \param job The number of the stopped job.
     */
    void slotTextStarted(const uint job);
    
public:
    KTTSDLib(QObject *parent=0, const char *name=0, KApplication *Appl=0);

    ~KTTSDLib();

    /** Reimplementations from the base class.
     */
    QString getName() const;
    
    QString getDescription() const;
    
    int getActions();
    
    const QWidget* getGUI(QFrame *frame);
       
    void reloadConfiguration();
    
    bool saveWasClicked() const;
    
    void setText(const QString &text);
    
    void sayText();
    
    int getStatus() const;
    
    void stop();
    
    void pause();
    
    void resume();
    
    void ffwd();
    
    void frev();

private slots:
    void removeAllJobsFromList();  
    void slotLaunchControlcenter();
    void slotKCMProcessExited(K3Process *p);
    
private:
    KApplication *m_Appl;
    kttsdlibtalker2 *m_talker;
    uint m_curJobNum;
    typedef queue<uint> jobListType;
    jobListType jobList;
    Ui::KTTSDlibSetup *m_gui;
};

#endif
