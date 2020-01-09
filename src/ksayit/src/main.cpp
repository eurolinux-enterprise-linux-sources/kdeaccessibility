/***************************************************************************
                          main.cpp  -  description
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

#include <stdlib.h>

// QT includes
#include <QtCore/QByteArray>

// KDE includes
#include <kcmdlineargs.h>
#include <kaboutdata.h>
#include <kglobal.h>
#include <klocale.h>
#include <KApplication>
// App specific includes
#include "version.h"
#include "ksayit.h"

static const char *description =
	I18N_NOOP("KSayIt - A Text To Speech frontend for KDE");
// INSERT A DESCRIPTION FOR YOUR APPLICATION HERE


int main(int argc, char *argv[])
{

  KAboutData aboutData( "ksayit", 0, ki18n("KSayIt"),
    KSAYIT_VERSION, ki18n(description), KAboutData::License_GPL,
    ki18n("(c) 1996 - 2005, Robert Vogl"), KLocalizedString(), 0, "voglrobe@web.de");
  aboutData.addAuthor(ki18n("Robert Vogl"),KLocalizedString(), "voglrobe@web.de");
  KCmdLineArgs::init( argc, argv, &aboutData );

  KApplication app;

    // setup MCOP
    // TODO: aRts no longer supported.  Either eliminate audio
    // effects or port to Phonon.
    // KArtsDispatcher dispatcher;

    // setup DCOP
    // TODO: port to DBUS.
    // DCOPCString appID = app.dcopClient()->registerAs( app.name(), false );

    KSayItApp *ksayit = new KSayItApp(0, 0, 0 /*appID*/);
    // ksayit->hide();
    ksayit->show();

    return app.exec();
}
