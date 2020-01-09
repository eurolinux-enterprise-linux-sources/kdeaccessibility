//
// C++ Interface: kttsdlibtalker2
//
// Description: 
//
//
// Author: Robert Vogl <voglrobe@lapislazuli>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef KTTSDLIBTALKER2_H
#define KTTSDLIBTALKER2_H

// Qt includes
#include <QtCore/QObject>
#include <QtCore/QByteArray>
#include <QtCore/QObject>
#include <QtDBus/QtDBus>

// KDE includes
#include "kspeechinterface.h"
#include <KApplication>
/**
@author Robert Vogl
*/
class kttsdlibtalker2 : public QObject
{
Q_OBJECT

signals:
    void signalTextStarted(const uint);
    void signalTextFinished(const uint);
    void signalTextStopped(const uint);

public:
    kttsdlibtalker2(QObject *parent = 0, const char *name = 0);

    ~kttsdlibtalker2();
    
    /** Intializes the DCOP interface.
     */
    void KTTSD_init(KApplication *Appl);
    
    /** See \p KSpeech Class Reference. Returns the job number.
     */
    uint KTTSD_setText(const QString &text, QString lang);
    
    /** See \p KSpeech Class Reference.
     */
    void KTTSD_startText(uint jobNum);

    /** See \p KSpeech Class Reference.
     */
    void KTTSD_stopText(uint jobNum);
    
    /** See \p KSpeech Class Reference.
     */
    void KTTSD_pauseText(uint jobNum);
    
    /** See \p KSpeech Class Reference.
     */
    void KTTSD_resumeText(uint jobNum);
    
    /** See \p KSpeech Class Reference.
     */
    uint KTTSD_moveRelTextSentence(const int n, const uint jobNum = 0);
    
    /** See \p KSpeech Class Reference.
     */    
    int KTTSD_getTextJobState(uint jobNum);
    
    /** See \p KSpeech Class Reference.
     */    
    void KTTSD_removeText(uint jobNum);
    
    /** See \p KSpeech Class Reference.
     */    
    int KTTSD_getCurrentTextJob();


protected Q_SLOTS:
    ////////////////////////////////////////////
    // Slots for DCOP signals
    ////////////////////////////////////////////    
    void textStarted(const QString &appID, const uint jobNum);
    void textFinished(const QString &appID, const uint jobNum);
    void textStopped(const QString &appID, const uint jobNum);
    
private:
    QString m_appId;
    uint m_jobNum;
    org::kde::KSpeech* m_kspeech;
};

#endif
