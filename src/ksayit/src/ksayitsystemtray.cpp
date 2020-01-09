/***************************************************************************
                          ksayitsystemtray.cpp  -  description
                             -------------------
    begin                : Die Sep 2 2003
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

#include <iostream>

// QT includes
#include <QtCore/QTranslator>
#include <QtGui/QMouseEvent>

// KDE includes
#include <kglobal.h>
#include <klocale.h>
#include <kstandardaction.h>
#include <KComponentData>

// App specific includes
#include "ksayitsystemtray.h"

KSayItSystemTray::KSayItSystemTray(QWidget *parent, const char *name)
 : KSystemTrayIcon(parent)
{
  setObjectName(name);
  initActions();
  changeState( StateCLIPEMPTY::Instance() );
}

KSayItSystemTray::~KSayItSystemTray()
{
  delete StateWAIT::Instance();
  delete StateSAY::Instance();
  delete StateCLIPEMPTY::Instance();
}

void KSayItSystemTray::initActions()
{
#if 0
  // Context-menu
  menu = this->contextMenu();
  help = new KHelpMenu(this, KGlobal::mainComponent().aboutData(), false, actionCollection());
  // Standard actions
  settings = KStandardAction::preferences(this, SLOT(slotPreferences()), actionCollection());
  help_about = KStandardAction::aboutApp(help, SLOT(aboutApplication()), actionCollection());
  help_kde = KStandardAction::aboutKDE(help, SLOT(aboutKDE()), actionCollection());

  say = menu->addAction( i18n("Say"), this, SLOT(slotSayActivated() ) );
  say->setIcon( KIcon( "media-playback-start" ) );

  shutup = menu->addAction( i18n("Shut Up"),this, SLOT( slotStopActivated() ) );
  shutup->setIcon( KIcon( "media-playback-stop" ) );

  pause = menu->addAction(i18n("Pause"), this, SLOT(slotPauseActivated()) );
  pause->setIcon( KIcon( "media-playback-pause" ) );

  next_sentence = menu->addAction(i18n("Next Sentence"), this, SLOT(slotNextSentenceActivated()) );
  next_sentence->setIcon( KIcon( "media-skip-forward" ) );

  prev_sentence = menu->addAction( i18n("Previous Sentence"), this, SLOT(slotPrevSentenceActivated()) );
  prev_sentence->setIcon( KIcon( "media-skip-backward" ) );

  // Actions -> Context-menu
  settings->plug(menu);
  menu->addSeparator();
  say->plug(menu);
  shutup->plug(menu);
  pause->plug(menu);
  next_sentence->plug(menu);
  prev_sentence->plug(menu);
  menu->addSeparator();
  help_about->plug(menu);
  help_kde->plug(menu);

  // default enables/disables
  say          ->setEnabled(false);
  shutup       ->setEnabled(false);
  pause        ->setEnabled(false);
  next_sentence->setEnabled(false);
  prev_sentence->setEnabled(false);
#endif
}


void KSayItSystemTray::slotPreferences()
{
  // call Preferences
  emit signalCallPreferences();
}

void KSayItSystemTray::mousePressEvent(QMouseEvent *me)
{
  _state->mousePressEvent(this, me);
}

void KSayItSystemTray::mouseReleaseEvent(QMouseEvent *me)
{
  _state->mouseReleaseEvent(this, me);
}

void KSayItSystemTray::slotSayActivated()
{
  // start to say content of clipboard
  emit signalSayActivated();
}

void KSayItSystemTray::slotStopActivated()
{
  // stop saying
  emit signalShutUpActivated();
}

void KSayItSystemTray::slotPauseActivated()
{
  // pause saying
  emit signalPauseActivated();
}

void KSayItSystemTray::slotNextSentenceActivated()
{
  // next sentence
  emit signalNextActivated();
}

void KSayItSystemTray::slotPrevSentenceActivated()
{
  // previous sentence
  emit signalPrevActivated();
}


void KSayItSystemTray::changeState(State *state)
{
  _state = state;
  _state->setContext(this);
}

void KSayItSystemTray::setActions(bool sayEnabled, bool pauseEnabled, bool shutupEnabled,
      bool nextEnabled, bool prevEnabled)
{
  say          ->setEnabled(sayEnabled);
  pause        ->setEnabled(pauseEnabled);
  shutup       ->setEnabled(shutupEnabled);
  next_sentence->setEnabled(nextEnabled);
  prev_sentence->setEnabled(prevEnabled);
}


void KSayItSystemTray::normalMousePressEvent(QMouseEvent *e)
{
#if 0
  KSystemTrayIcon::mousePressEvent(e);
#endif
}

void KSayItSystemTray::normalMouseReleaseEvent(QMouseEvent *e)
{
#if 0
  KSystemTrayIcon::mouseReleaseEvent(e);
#endif
}

void KSayItSystemTray::sayClipboard()
{
    emit signalSayClipboard();
}




////////////////////////////////////////////
State::State(){
}
State::~State(){
}

void State::mousePressEvent(KSayItSystemTray *caller, QMouseEvent *e)
{
  // reimplemented by subclasses
}

void State::mouseReleaseEvent(KSayItSystemTray *caller, QMouseEvent *e)
{
  // reimplemented by subclasses
}

void State::setContext(KSayItSystemTray *caller)
{
  // reimplemented by subclasses
}

void State::changeState(KSayItSystemTray *caller, State *state)
{
  caller->changeState(state);
}

void State::say(KSayItSystemTray *caller)
{
  caller->sayClipboard();
}

void State::mousePressEventCall(KSayItSystemTray *caller, QMouseEvent *e)
{
  caller->normalMousePressEvent(e);
}

void State::mouseReleaseEventCall(KSayItSystemTray *caller, QMouseEvent *e)
{
  caller->normalMouseReleaseEvent(e);
}


////////////////////////////////////////////
StateWAIT::StateWAIT(){
  m_traypixmap = KIconLoader::global()->loadIcon("ksayit", KIconLoader::Toolbar);
}
StateWAIT::~StateWAIT(){
}
StateWAIT* StateWAIT::_instance = 0;

StateWAIT* StateWAIT::Instance()
{
  if (_instance == 0){
    _instance = new StateWAIT();
  }
  return _instance;
}

void StateWAIT::setContext(KSayItSystemTray *caller)
{
#if 0
  caller->setPixmap( m_traypixmap );
#endif
}

void StateWAIT::mousePressEvent(KSayItSystemTray *caller, QMouseEvent *e)
{
#if 0
  if (e->button()==Qt::LeftButton){ // left Mouse-button pressed
    QWidget::mousePressEvent(e); // do nothing (see mouseReleaseEvent)
  } else {
    mousePressEventCall(caller, e); // normal mouse-handling
  }
#endif
}

void StateWAIT::mouseReleaseEvent(KSayItSystemTray *caller, QMouseEvent *e)
{
#if 0
  if (e->button()==Qt::LeftButton){ // left Mouse-button released
    say(caller);
  } else {
    mouseReleaseEventCall(caller, e); // normal mouse-handling
  }
#endif
}



////////////////////////////////////////////
StateSAY::StateSAY(){
  m_traypixmap = KIconLoader::global()->loadIcon("ksayit_talking", KIconLoader::Toolbar);
}
StateSAY::~StateSAY(){
}
StateSAY* StateSAY::_instance = 0;

StateSAY* StateSAY::Instance()
{
  if (_instance == 0){
    _instance = new StateSAY();
  }
  return _instance;
}

void StateSAY::setContext(KSayItSystemTray *caller)
{
#if 0
  caller->setPixmap( m_traypixmap );
#endif
}

void StateSAY::mousePressEvent(KSayItSystemTray *caller, QMouseEvent *e)
{
#if 0
  if (e->button()==Qt::LeftButton){ // left Mouse-button pressed
    QWidget::mousePressEvent(e); // do nothing (see mouseReleaseEvent)
  } else {
    mousePressEventCall(caller, e); // normal mouse-handling
  }
#endif
}

void StateSAY::mouseReleaseEvent(KSayItSystemTray *caller, QMouseEvent *e)
{
#if 0
  if (e->button()==Qt::LeftButton){ // left Mouse-button released
    QWidget::mouseReleaseEvent(e); // do nothing (see mouseReleaseEvent)
  } else {
    mouseReleaseEventCall(caller, e); // normal mouse-handling
  }
  #endif
}




////////////////////////////////////////////
StateCLIPEMPTY::StateCLIPEMPTY(){
  m_traypixmap = KIconLoader::global()->loadIcon("ksayit_clipempty", KIconLoader::Toolbar);
}
StateCLIPEMPTY::~StateCLIPEMPTY(){
}
StateCLIPEMPTY* StateCLIPEMPTY::_instance = 0;

StateCLIPEMPTY* StateCLIPEMPTY::Instance()
{
  if (_instance == 0){
    _instance = new StateCLIPEMPTY();
  }
  return _instance;
}

void StateCLIPEMPTY::setContext(KSayItSystemTray *caller)
{
    #if 0
  caller->setPixmap( m_traypixmap );
#endif
}

void StateCLIPEMPTY::mousePressEvent(KSayItSystemTray *caller, QMouseEvent *e)
{
#if 0
  if (e->button()==Qt::LeftButton){ // left Mouse-button pressed
    QWidget::mousePressEvent(e); // do nothing (see mouseReleaseEvent)
  } else {
    mousePressEventCall(caller, e); // normal mouse-handling
  }
#endif
}

void StateCLIPEMPTY::mouseReleaseEvent(KSayItSystemTray *caller, QMouseEvent *e)
{
#if 0
  if (e->button()==Qt::LeftButton){ // left Mouse-button released
    QWidget::mouseReleaseEvent(e); // do nothing (see mouseReleaseEvent)
  } else {
    mouseReleaseEventCall(caller, e); // normal mouse-handling
  }
#endif
}


#include "ksayitsystemtray.moc"
