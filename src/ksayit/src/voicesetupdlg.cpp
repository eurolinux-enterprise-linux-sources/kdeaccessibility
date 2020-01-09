/***************************************************************************
                          voicesetupdlg.cpp  -  description
                             -------------------
    begin                : Son Nov 2 2003
    copyright            : (C) 2003 by Robert Vogl
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

// #include <iostream>
// using namespace std;
 
// QT includes
#include <QtGui/QPixmap>
#include <QtGui/QComboBox>
#include <QtGui/QColor>
#include <QtGui/QLayout>
#include <QtGui/QWidget>
#include <QtGui/QVBoxLayout>

// KDE includes
#include <kdebug.h>
#include <kglobal.h>
#include <klocale.h>
#include <kiconloader.h>
#include <kmessagebox.h>
#include <kicon.h>

// App specific includes
#include "voicesetupdlg.h"
// #include "fxsetupimpl.h"
// #include "fxpluginhandler.h"
#include "kttsdlib.h"

VoiceSetupDlg::VoiceSetupDlg(QWidget *parent, const char *name, const QString &caption,
        bool modal,
        KConfig *config,
        /*FXPluginHandler *fxpluginhandler,*/
        KTTSDLib *ttslib)
 : KPageDialog(parent),
   m_config(config),
   /*m_fxpluginhandler(fxpluginhandler),*/
   m_kttslib(ttslib)
{
    setFaceType(KPageDialog::List);
    setButtons(KDialog::Ok|KDialog::Cancel);
    setDefaultButton(KDialog::Ok);
    setCaption(caption);
    setModal(modal);
    setObjectName(name);

    // setIconListAllVisible(true);
    connect (this, SIGNAL(aboutToShowPage(QWidget*)), this, SLOT(slotPageChanged(QWidget*)));
    connect (this, SIGNAL(okClicked()),SLOT(slotOk()));
    connect (this, SIGNAL(cancelClicked()),SLOT(slotCancel()));
    initVoicePage();
    // initFXPage();

}


VoiceSetupDlg::~VoiceSetupDlg()
{    
}


void VoiceSetupDlg::initVoicePage()
{
    QFrame *voiceFrame = new QFrame();
    KPageWidgetItem *voicePage = new KPageWidgetItem(voiceFrame, i18n("Voice Settings"));
    voicePage->setIcon(KIcon("ksayit"));
    const QWidget *voiceDialog = m_kttslib->getGUI( voiceFrame );
    addPage(voicePage);
}


/*
void VoiceSetupDlg::initFXPage()
{
    QPixmap icon = KIconLoader::global()->loadIcon("ksysguard", K3Icon::Toolbar, K3Icon::SizeMedium);
    m_fxPage = addPage( i18n( "Audio FX" ), i18n("Effect Stack"), icon );
    QVBoxLayout *fxLayout = new QVBoxLayout( m_fxPage, 0, spacingHint() );
    
    FX_SetupImpl *fxDialog = new FX_SetupImpl(m_fxPage, "fxsetup", m_config, m_fxpluginhandler);
    fxLayout->addWidget(fxDialog);
    connect( this, SIGNAL(signalOKWasClicked()), fxDialog, SLOT(slotSaveWasClicked()) );
    connect( this, SIGNAL(signalRemoveAllFX()), fxDialog, SLOT(slotRemoveAll()) );
    connect( this, SIGNAL(signalReloadFX()), fxDialog, SLOT(slotReload()) );
    
    // Disable/enable FX-Setup depending on TTS-libs capability
    int status = m_kttslib->getStatus() & TTS::AUDIOFILE;
    if ( status ){
        m_fxPage->setEnabled(true);
    } else {
        m_fxPage->setEnabled(false);
        emit signalRemoveAllFX();
    }
}
*/

void VoiceSetupDlg::slotPageChanged(QWidget *page)
{
    kDebug(100200) << "+++ entering VoiceSetupDlg::slotPageChanged: " << page;
    
    if ( page != m_fxPage )
        return;
    
    int status = m_kttslib->getStatus() & TTS::AUDIOFILE;
    if ( status ){
        m_fxPage->setEnabled(true);
    } else {
        QString q = i18n("The active TTS system does not make use of aRts effects.");
        KMessageBox::information( this, q, i18n("Plugin Configuration"), "KSayIt_Audiofile" );
        m_fxPage->setEnabled(false);
    }
}


void VoiceSetupDlg::slotOk()
{
    kDebug(100200) << "+++ entering VoiceSetupDlg::slotOK()";
    
    int status = m_kttslib->getStatus();
    if ( (status & TTS::AUDIOFILE) == 0 ){
        emit signalRemoveAllFX();
    }
    
    // Let all objects save their configuration
    emit signalOKWasClicked();

	// check if configuration was saved O.K.
    if ( m_kttslib->saveWasClicked() ){
		kDebug(100200) << "--- leaving VoiceSetupDlg::slotOk";
		QDialog::accept();
	}
}


void VoiceSetupDlg::slotCancel()
{
    kDebug(100200) << "VoiceSetupDlg::slotCancel()";
    QDialog::reject();
}

#include "voicesetupdlg.moc"
