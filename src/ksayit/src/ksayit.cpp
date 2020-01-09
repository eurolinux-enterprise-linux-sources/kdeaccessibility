/***************************************************************************
                          ksayit.cpp  -  description
                             -------------------
    begin                : Son Aug 10 13:26:57 EDT 2003
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
#include <stdlib.h> // getenv, system

// include files for QT
#include <QtCore/QDir>
#include <QtGui/QApplication>
#include <QtGui/QDockWidget>
#include <QtCore/QFileInfo>
#include <QtCore/QFile>
#include <QtCore/QByteArray>
#include <QtGui/QCloseEvent>

// include files for KDE
#include <klocale.h>
#include <kdebug.h>
#include <kiconloader.h>
#include <kmessagebox.h>
#include <kfiledialog.h>
#include <kmenubar.h>
#include <kmenu.h>
#include <kstatusbar.h>
#include <kconfig.h>
#include <kstandardaction.h>
#include <kedittoolbar.h>
#include <kshortcutsdialog.h>
#include <kurl.h>
#include <kstandarddirs.h>
#include <kglobal.h>
#include <KActionCollection>

// application specific includes
#include "ksayit.h"
#include <kconfiggroup.h>
#include "ksayitviewimpl.h"
#include "voicesetupdlg.h"
#include "ksayitsystemtray.h"
// #include "effectstack.h"
// #include "fxpluginhandler.h"
#include "kttsdlib.h"
#include "doctreeviewimpl.h"
#include "ksayitbookmarkhandler.h"

// #define KSAYITUI "/home/voglrobe/Projekte/ksayit/ksayit/ksayitui.rc"
#define KSAYITUI "ksayitui.rc"
#define ID_STATUS_MSG 1

KSayItApp::KSayItApp(QWidget* parent, Qt::WFlags f,
    const QByteArray &objID)
  : KXmlGuiWindow(parent, f)
{
    config    = NULL;
    view      = NULL;
    tray      = NULL;
    // es        = NULL;
    treeview  = NULL;
    bkManager = NULL;
    bkHandler = NULL;
    bkMenu    = NULL;
    m_currentBookmarkFile.clear();

    config=KGlobal::config();
    m_kttslib = new KTTSDLib(this, "KTTSD-Library", kapp);

    initView();
    initStatusBar();
    initActions();
    initBookmarkManager( i18n("Untitled") );

    createGUI(KSAYITUI);
    setAutoSaveSettings("MainWindow");

    readOptions();

    // connect TTS plugin handler to this object
    connect(m_kttslib, SIGNAL(signalFinished()),
            this, SLOT(slotTTSFinished()) );

    // only used if library thinks it has to call the preferences dialog.
    // e.g. when it detects a bad configuration.
    connect(m_kttslib, SIGNAL(signalCallPreferences()),
            this, SLOT(slotPreferences()) );

    // init Clipboard
    cb = QApplication::clipboard();
    connect(cb, SIGNAL(dataChanged()), this, SLOT(slotClipboardChanged()) );

    // create SystemTray object
    tray = new KSayItSystemTray(this, "system_tray");
    connect(tray, SIGNAL(signalCallPreferences()), this, SLOT(slotPreferences()) );
    connect(tray, SIGNAL(signalSayActivated()), this, SLOT(slotSayText()) );
    connect(tray, SIGNAL(signalSayClipboard()), this, SLOT(slotSayClipboard()) );
    connect(tray, SIGNAL(signalShutUpActivated()), this, SLOT(slotStopActivated()) );
    connect(tray, SIGNAL(signalPauseActivated()), this, SLOT(slotPauseActivated()) );
    connect(tray, SIGNAL(signalNextActivated()), this, SLOT(slotNextSentenceActivated()) );
    connect(tray, SIGNAL(signalPrevActivated()), this, SLOT(slotPrevSentenceActivated()) );
    tray->show();
    tray->setEnabled(true);

    // create the FX plugin handler
    // pluginhandler = new FXPluginHandler(this, "fxpluginhandler", config);
    // m_fxpluginhandler->searchPlugins();

    // create object for effectstack
    // es = new EffectStack(m_fxpluginhandler, config);

    // emit signalConfigChanged();
    m_kttslib->reloadConfiguration();
    // m_fxpluginhandler ->readConfiguration();

    // init docview
    // treeview->clear();
    // treeview->createEmptyDocument();
    slotClear();
}

KSayItApp::~KSayItApp()
{
  // delete kapp->mainWidget();
  delete m_kttslib;
  delete tray;
  // delete m_fxpluginhandler;
  // delete es;
  delete view;
  if (bkHandler)
      delete bkHandler;
  if (bkMenu)
      delete bkMenu;
}

void KSayItApp::initActions()
{
  // Standard-Actions
  open   = KStandardAction::open(this, SLOT(slotFileOpen()), actionCollection());
  save   = KStandardAction::save(this, SLOT(slotFileSave()), actionCollection());
  saveAs = KStandardAction::saveAs(this, SLOT(slotFileSaveAs()), actionCollection());
  KStandardAction::quit(this, SLOT(slotFileQuit()), actionCollection());
  KStandardAction::close(this, SLOT(slotCloseMainWindow()), actionCollection());
  cut   = KStandardAction::cut(view, SLOT(slotCut()), actionCollection());
  copy  = KStandardAction::copy(view, SLOT(slotCopy()), actionCollection());
  paste = KStandardAction::paste(view, SLOT(slotPaste()), actionCollection());
  preferences = KStandardAction::preferences(this, SLOT(slotPreferences()), actionCollection());
  KStandardAction::keyBindings(this, SLOT(slotEditKeys()), actionCollection());
  KStandardAction::configureToolbars(this, SLOT(slotConfigureToolbar()), actionCollection());
#ifdef __GNUC__
#warning "KStandardAction::showToolbar was deprecated"
#endif
#if 0
  KStandardAction::showToolbar("mainToolBar", actionCollection());
#endif
  statusBarAction = KStandardAction::showStatusbar(this, SLOT(slotToggleStatusBar()), actionCollection());

  // User defined actions
  say = new KAction (i18n("Say"),
              Qt::Key_F9,
              this, SLOT (slotSayText()),
              actionCollection(),
              "say_it");

  pause = new KAction (i18n("Pause"),
              Qt::Key_Pause,
              this, SLOT (slotPauseActivated()),
              actionCollection(),
              "pause");

  shutup = new KAction (i18n("Shut Up"),
              Qt::Key_F10,
              this, SLOT (slotStopActivated()),
              actionCollection(),
              "shut_up");

  next_sentence = new KAction (i18n("Next Sentence"),
              Qt::Key_PageDown,
              this, SLOT (slotNextSentenceActivated()),
              actionCollection(),
              "next_sentence");


  prev_sentence = new KAction (i18n("Previous Sentence"),
              Qt::Key_PageUp,
              this, SLOT(slotPrevSentenceActivated()),
              actionCollection(),
              "prev_sentence");

  clear = new KAction (i18n("Clear"),
              Qt::Key_F12,
              this, SLOT(slotClear()),
              actionCollection(),
              "clear");

  edit = new KToggleAction( i18n("Edit Text"),
             0,
             this, SLOT(slotEditToggled()),
             actionCollection(),
             "edittext");

  bookmarkmenu = new KActionMenu(
             KIcon("bookmarks"), // icon
             i18n("Bookmarks"),
             actionCollection(),
             "bookmarks"); // name
             bookmarkmenu->setDelayed(false);

  // default disable/enable
  save         ->setEnabled(false);
  saveAs       ->setEnabled(false);
  copy         ->setEnabled(false);
  cut          ->setEnabled(false);
  paste        ->setEnabled(false);
  say          ->setEnabled(false);
  pause        ->setEnabled(false);
  shutup       ->setEnabled(false);
  next_sentence->setEnabled(false);
  prev_sentence->setEnabled(false);
  preferences  ->setEnabled(true);
  edit         ->setChecked(false);
  bookmarkmenu ->setEnabled(false);
  m_ap_saying = false;
  m_ap_paused = false;
  m_textview_empty = true;
}


void KSayItApp::initBookmarkManager(const QString &filename)
{
    QString bkFile = getBookmarkDir( filename );
    if ( bkFile.isNull() )
        return;

    m_currentBookmarkFile = bkFile;
    bkManager = KBookmarkManager::managerForFile( bkFile, "ksayit" );

    // create BookmarkHandler object
    if ( bkHandler )
        delete bkHandler;
    bkHandler = new KSayItBookmarkHandler(bkManager, this);

    // create Bookmarkmenu
    KMenu *bkPopupMenu = bookmarkmenu->popupMenu();
    if ( bkMenu )
        delete bkMenu;
    bkMenu = new KBookmarkMenu(bkManager, bkHandler, bkPopupMenu, 0, true );
}


void KSayItApp::slotNotifyBookmarkHandler(const QString &ID, const QString &title)
{
    if ( bkHandler )
        bkHandler->notifyBookmarkHandler(ID, title);
}


void KSayItApp::slotDeleteBookmark(const QString &url, const QString &title)
{
    kDebug(100200) << "KSayItApp::slotDeleteBookmark(" << url << ")";
    if ( bkHandler )
        bkHandler->deleteBookmark( url, title );
}


QString KSayItApp::getBookmarkDir(const QString &filename)
{
    QString bkRelPath = "ksayit/ksayit_bookmarks/" + filename + ".bookmarks";
    QString bkFile;
    bkFile = KStandardDirs::locateLocal("data", bkRelPath);
    return bkFile;
}


void KSayItApp::slotSetBookmarkFilename(const QString &newname)
{
    kDebug(100200) << "KSayItApp::slotSetBookmarkFilename(" << newname << ")";

    initBookmarkManager(newname);
}


void KSayItApp::slotChangeBookmarkFilename(const QString &newname)
{
    kDebug(100200) << "KSayItApp::slotChangeBookmarkFilename(" << newname << ")";

    if ( m_currentBookmarkFile.isNull() )
        return;

    QString newbkFile = getBookmarkDir(newname);
    if ( newbkFile.isNull() )
        return;

    // copy old bookmarkfile to new file
    if ( m_currentBookmarkFile != newbkFile ){
        if ( QFile::exists(m_currentBookmarkFile) ){
            QString command = QString("cp %1 %2").arg(m_currentBookmarkFile).arg(newbkFile);
            system( command.ascii() );
        }
        // install new BookmarkHandler based on the new file
        initBookmarkManager(newname);
    }
}


QString KSayItApp::setItemByBookmark( const QString &ID, const QString &title )
{
    QString result;
    result = treeview->selectItemByID( ID, title );
    return result;
}


void KSayItApp::initStatusBar()
{
  // STATUSBAR
  statusBar()->insertItem(i18n("Ready."), ID_STATUS_MSG, 1);
  statusBar()->setItemAlignment(ID_STATUS_MSG, Qt::AlignLeft);
}


void KSayItApp::initView()
{
  // create the main widget
  view = new KSayItViewImpl(this);
  view->setMinimumSize(view->sizeHint());
  setCentralWidget(view);
  // connections
  connect( view, SIGNAL(signalEnableCopyCut(bool)),
    this, SLOT(slotEnableCopyCut(bool)));
  connect( view, SIGNAL(signalShowStatus(const QString &)),
    this, SLOT(slotStatusMsg(const QString &)));
  connect( view, SIGNAL(signalSetCaption(const QString &)),
    this, SLOT(slotSetCaption(const QString &)));
  connect( view, SIGNAL(signalTextChanged(bool)),
    this, SLOT(slotTextChanged(bool)) );

  // DockWindow
  QDockWidget *docview = new QDockWidget(this);
  treeview = new DocTreeViewImpl(docview, "treeview");
  treeview->enableContextMenus( true );
  // treeview->clear();
  docview->setWidget(treeview);
  docview->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea);
  connect( treeview, SIGNAL(signalContentChanged(const QString&)),
    this, SLOT(slotTreeViewChanged(const QString&)) );
  connect( treeview, SIGNAL(signalSetText(const QString&)),
    this, SLOT(slotSetText(const QString&)) );
  connect( treeview, SIGNAL(signalAllNodesProcessed()),
    this, SLOT(slotSayNode()) );
  connect( treeview, SIGNAL(signalEnableTextedit(bool)),
    this, SLOT(slotEnableTextedit(bool)) );
  connect( treeview, SIGNAL(signalNotifyBookmarkManager(const QString&, const QString&)),
    this, SLOT(slotNotifyBookmarkHandler(const QString&, const QString&)) );
  connect( treeview, SIGNAL(signalSetBookmarkFilename(const QString&)),
    this, SLOT(slotSetBookmarkFilename(const QString&)) );
  connect( treeview, SIGNAL(signalChangeBookmarkFilename(const QString&)),
    this, SLOT(slotChangeBookmarkFilename(const QString&)) );
  connect( treeview, SIGNAL(signalDeleteBookmark(const QString&, const QString&)),
    this, SLOT(slotDeleteBookmark(const QString&, const QString&)) );
}


void KSayItApp::readOptions()
{
    KConfigGroup grp = config->group( "MainWindow" );
  applyMainWindowSettings( grp );
  statusBarAction->setChecked(!statusBar()->isHidden());
}

void KSayItApp::closeEvent(QCloseEvent *ce)
{
  ce->ignore();
  if ( !this->isVisible() ){
    slotFileQuit();
  } else {
    this->hide();
  }
}


/////////////////////////////////////////////////////////////////////
// DCOP FUNCTIONS
/////////////////////////////////////////////////////////////////////

/*
ASYNC KSayItApp::dcopSayText(QString msg)
{
  slotSetText( msg );
  sayActivated();
}

ASYNC KSayItApp::dcopStop()
{
  slotStopActivated();
}


ASYNC KSayItApp::dcopSayXmlFile(KUrl url)
{
    slotStatusMsg(i18n("Say XML file..."));
    m_enableChangeNotifications = false;

    try{
        if ( !url.isEmpty() ){
            resetView();
            treeview->openFile( url );
            setActions(ACTIONS::PLAY | ACTIONS::FFWD);
            save  ->setEnabled(false);
            saveAs->setEnabled(true);
            edit->setChecked(false);
            treeview->setEditMode( false );
        }
    }
    catch( QString err ){
        KMessageBox::error(this, err);
        save  ->setEnabled(false);
        saveAs->setEnabled(true);
    }

    m_enableChangeNotifications = true;

    slotSayText();
}
*/

/////////////////////////////////////////////////////////////////////
// SLOT IMPLEMENTATION
/////////////////////////////////////////////////////////////////////

void KSayItApp::slotEditToggled()
{
    if ( edit->isChecked() ){
        treeview->setEditMode( true );
        slotStatusMsg(i18n("Edit Mode ON."));
    } else {
        treeview->setEditMode( false );
        slotStatusMsg(i18n("Edit Mode OFF."));
    }
}


void KSayItApp::slotSaveOptions()
{
    KConfigGroup grp = config->group( "MainWindow" );
  saveMainWindowSettings( grp );
  config->sync();
}


void KSayItApp::slotConfigureToolbar()
{
    KConfigGroup grp = config->group( "MainWindow" );
  saveMainWindowSettings( grp );
#if 0
  KEditToolBar dlg(actionCollection(), KSAYITUI);
  connect(&dlg,SIGNAL(newToolbarConfig()),this,SLOT(slotNewToolbarConfig()));
  if (dlg.exec())
  {
     createGUI(KSAYITUI);
  }
#endif
}


void KSayItApp::slotNewToolbarConfig()
{
    KConfigGroup grp = config->group( "MainWindow" );
  applyMainWindowSettings( grp );
}


void KSayItApp::slotFileOpen()
{
    slotStatusMsg(i18n("Open file..."));
    m_enableChangeNotifications = false;

    // open filerequester
    QString fn;
    QString usershome( getenv("HOME") );
    KUrl url = KFileDialog::getOpenUrl(usershome, QString(), this, i18n("File to Speak") );
//    try{
        if ( !url.isEmpty() ){
            resetView();
            treeview->openFile( url );
            setActions(ACTIONS::PLAY | ACTIONS::FFWD);
            save  ->setEnabled(false);
            saveAs->setEnabled(true);
            edit->setChecked(false);
            treeview->setEditMode( false );
            bookmarkmenu->setEnabled(true);
        }
//    }
//    catch( QString err ){
//        KMessageBox::error(this, err);
//        save  ->setEnabled(false);
//        saveAs->setEnabled(true);
//        bookmarkmenu->setEnabled(false);
//    }

  m_enableChangeNotifications = true;
}


void KSayItApp::slotFileSave()
{
   slotStatusMsg(i18n("Save file..."));
//   try {
         treeview->saveFile();
         save  ->setEnabled(false);
         saveAs->setEnabled(true);
//   }
//   catch ( QString err )
//   {
//      KMessageBox::error(this, err);
//      save  ->setEnabled(false);
//      saveAs->setEnabled(true);
//   }
}


void KSayItApp::slotFileSaveAs()
{
  slotStatusMsg(i18n("Save file as..."));
//   try {
         treeview->saveFileAs();
         save  ->setEnabled(true);
         saveAs->setEnabled(true);
         bookmarkmenu->setEnabled(true);
//   }
//   catch ( QString err )
//   {
//      KMessageBox::error(this, err);
//      save  ->setEnabled(true);
//      saveAs->setEnabled(true);
//      bookmarkmenu->setEnabled(false);
//   }
}


void KSayItApp::slotEnableCopyCut(bool enable)
{
  copy->setEnabled(enable);
  cut ->setEnabled(enable);
}


void KSayItApp::slotEditKeys()
{
  KShortcutsDialog::configure( actionCollection() ); //, xmlFile());
}


void KSayItApp::slotFileQuit()
{
  slotStatusMsg(i18n("Exiting..."));
  int really = KMessageBox::questionYesNo( this, i18n("Do you really want to quit?"), QString(), KStandardGuiItem::quit(), KStandardGuiItem::cancel() );
  if (really == KMessageBox::Yes){
     slotSaveOptions();
     kapp->quit();
  } else {
     slotStatusMsg(i18n("Ready."));
  }
}


void KSayItApp::slotCloseMainWindow()
{
  this->hide();
}


void KSayItApp::slotToggleStatusBar()
{
  slotStatusMsg(i18n("Toggle the statusbar..."));
  //turn Statusbar on or off
  if(statusBar()->isVisible())
    statusBar()->hide();
  else
    statusBar()->show();

  slotStatusMsg(i18n("Ready."));
}


void KSayItApp::slotStatusMsg(const QString &text)
{
#warning "kde4 port";
#if 0
  // change status message permanently
  statusBar()->clear();
  statusBar()->changeItem(text, ID_STATUS_MSG);
#endif
}


void KSayItApp::slotSetCaption(const QString &caption)
{
  this->setCaption(caption);
}


void KSayItApp::slotPreferences()
{
  // create Preferences object and open it
  VoiceSetupDlg *prefs = new VoiceSetupDlg(this, "preferences", i18n("Setup"), true,
                      config,
                      /*m_fxpluginhandler,*/
                      m_kttslib);

  if (prefs->exec() == QDialog::Accepted){
    delete prefs;
    m_kttslib->reloadConfiguration();
    // m_fxpluginhandler ->readConfiguration();
  } else {
    delete prefs;
  }
}


void KSayItApp::slotClipboardChanged()
{
  if ( cb->text().isEmpty() ){
    tray->changeState( StateCLIPEMPTY::Instance() );
    paste->setEnabled(false);
    return;
  }

  if ( cb->data(QClipboard::Clipboard)->provides("text/plain") ){
      tray->changeState( StateWAIT::Instance() );
      paste->setEnabled(true);
  } else {
      tray->changeState( StateCLIPEMPTY::Instance() );
      paste->setEnabled(false);
  }
}


void KSayItApp::slotTextChanged(bool empty)
{
    m_textview_empty = empty;

    // do nothing if change notifications from the Textedit are
    // currently not welcome.
    if ( !m_enableChangeNotifications )
        return;

    // do nothing if say is in progress
    if ( m_ap_saying )
        return;

    kDebug(100200) << "KSayItApp::slotTextChanged( " << empty << " )";

    // Update actions
    if ( empty ){
        setActions(0);
    } else {
        setActions(ACTIONS::PLAY);
    }
    // save  ->setEnabled(!empty);
    saveAs->setEnabled(!empty);

    // update TreeView-Item
    treeview->setNodeContent( view->getText() );

}


void KSayItApp::slotClear()
{
    m_enableChangeNotifications = false;
    // TextEdit clear
    view->textClear();

    // TreeView clear
    treeview->clear();
    treeview->createEmptyDocument();
    treeview->setEditMode( true );
    edit->setChecked(true);
    slotStatusMsg(i18n("Edit Mode ON."));

    // set actions
    save  ->setEnabled(false);
    saveAs->setEnabled(false);
    setActions(0);
    bookmarkmenu->setEnabled(false);
    m_enableChangeNotifications = true;
}


void KSayItApp::slotSayClipboard()
{
    // get Text from clipboard
    slotStatusMsg(i18n("speaking Clipboard..."));
    setActions(ACTIONS::PAUSE | ACTIONS::STOP | ACTIONS::FFWD | ACTIONS::FREV);
    tray->changeState( StateSAY::Instance() );

    clip = cb->text();
    // sayActivated( clip.trimmed() );
    // set text and say it
    slotSetText( clip.trimmed() );
    sayActivated();
}


void KSayItApp::slotSayText()
{
    // say Text from text-window
    setActions(ACTIONS::PAUSE | ACTIONS::STOP | ACTIONS::FFWD | ACTIONS::FREV);
    tray->changeState( StateSAY::Instance() );

    slotStatusMsg(i18n("synthesizing..."));

    if ( m_ap_paused ){
        m_ap_paused = false;
        m_kttslib->resume();
    } else {
        // es->loadEffects();
        treeview->sayActiveNodeAndChilds();
    }
}


void KSayItApp::slotSayNode()
{
    sayActivated();
}


void KSayItApp::slotSetText(const QString &text)
{
    // also invoked by the treeview for the each node
    if ( text.isNull() )
        return;

    // put text on the stack of KTTS
    m_kttslib->setText( text );
}

void KSayItApp::sayActivated()
{
    kDebug(100200) << "KSayItApp::sayActivated()";

    preferences->setEnabled(false);
    m_ap_saying = true;

//    try {
        m_kttslib->sayText();
//    }
//    catch (QString err){
//        kDebug(100200) << "Exception caught in KSayItApp::sayActivated()";
//        KMessageBox::sorry(0, err, i18n("Unable to speak text"));
//        slotFinished();
//    }
}


void KSayItApp::slotTTSFinished()
{
    kDebug(100200) << "KSayItApp::slotTTSFinished()";
    // es->unloadEffects();
    slotFinished();
}


void KSayItApp::slotFinished()
{
    kDebug(100200) << "KSayItApp::slotFinished()";

    // disable/enable actions
    m_ap_paused = false;

    if ( m_textview_empty ){
        setActions(0);
    } else {
        setActions(ACTIONS::PLAY);
    }
    tray->changeState( StateWAIT::Instance() );
    preferences->setEnabled(true);
    m_ap_saying = false;
    slotStatusMsg(i18n("Ready."));
}


void KSayItApp::slotStopActivated()
{
    m_kttslib->stop();
    // slotFinished();
}


void KSayItApp::slotPauseActivated()
{
    kDebug(100200) << "KSayItApp::slotPauseActivated()";

    m_kttslib->pause();
    m_ap_paused = true;
    setActions(ACTIONS::PLAY | ACTIONS::STOP);
    tray->changeState( StateSAY::Instance() );
    slotStatusMsg(i18n("Paused..."));
}


void KSayItApp::slotNextSentenceActivated()
{
    kDebug(100200) << "KSayItApp::slotNextSentenceActivated()";
    m_kttslib->ffwd();
}


void KSayItApp::slotPrevSentenceActivated()
{
    kDebug(100200) << "KSayItApp::slotPrevSentenceActivated()";
    m_kttslib->frev();
}


void KSayItApp::setActions(int actions)
{
    // ACTIONS::PLAY | ACTIONS::STOP  | ACTIONS::PAUSE | ACTIONS::FFWD | ACTIONS::FREV;

    // Get the mask of supported actions from the plugin.
    int mask = m_kttslib->getActions();
    kDebug(100200) << QString("KSayItApp:PSA: %1").arg(mask, 0, 2);

    // disable actions not supported by the plugin
    int ma = actions & mask;

    if (ma & ACTIONS::PLAY){
        say->setEnabled(true);
    } else {
        say->setEnabled(false);
    }
    if (ma & ACTIONS::STOP){
        shutup->setEnabled(true);
    } else {
        shutup->setEnabled(false);
    }
    if (ma & ACTIONS::PAUSE){
        pause->setEnabled(true);
    } else {
        pause->setEnabled(false);
    }
    if (ma & ACTIONS::FFWD){
        next_sentence->setEnabled(true);
    } else {
        next_sentence->setEnabled(false);
    }
    if (ma & ACTIONS::FREV){
        prev_sentence->setEnabled(true);
    } else {
        prev_sentence->setEnabled(false);
    }

    tray->setActions( say->isEnabled(), pause->isEnabled(), shutup->isEnabled(),
        next_sentence->isEnabled(), prev_sentence->isEnabled() );
}


void KSayItApp::slotTreeViewChanged(const QString &str)
{
    kDebug(100200) << "KSayItApp::slotTreeViewChanged()";

    if ( !m_ap_saying )
        setActions(ACTIONS::PLAY);

    m_enableChangeNotifications = false;
    view->textClear();
    view->setText(str);
    m_enableChangeNotifications = true;
}


void KSayItApp::slotEnableTextedit(bool enable)
{
    kDebug(100200) << "KSayItApp::slotEnableTextedit( " << enable << " )";

    view->enableTextedit( enable );
}


void KSayItApp::resetView()
{
    m_enableChangeNotifications = false;
    // TextEdit clear
    view->textClear();

    // TreeView clear
    treeview->clear();

    // set actions
    save  ->setEnabled(false);
    saveAs->setEnabled(false);
    setActions(0);
    m_enableChangeNotifications = true;
}


void KSayItApp::slotAddBookmark()
{
    kDebug(100200) << "KSayItApp::slotAddBookmark()";


}


void KSayItApp::slotEditBookmarks()
{
    kDebug(100200) << "KSayItApp::slotEditBookmark()";


}



#include "ksayit.moc"
