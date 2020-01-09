/***************************************************** vim:set ts=4 sw=4 sts=4:
  Phonon player plugin for KTTS.
  ------------------------------
  Copyright:
  (C) 2006 by Gary Cramblitt <garycramblitt@comcast.net>
  -------------------
  Original author: Gary Cramblitt <garycramblitt@comcast.net>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 ******************************************************************************/

// PhononPlayer includes.
#include "phononplayer.h"

// Qt includes.

// KDE includes.
#include <Phonon/AudioOutput>
#include <Phonon/MediaObject>
#include <kurl.h>

// KTTS includes.

PhononPlayer::PhononPlayer(QObject* parent, const QStringList& args) :
    Player(parent, "phononplayer", args)
{
    m_audioPlayer = new Phonon::MediaObject(this);
    m_audioOutput = new Phonon::AudioOutput(Phonon::AccessibilityCategory, this);
    Phonon::createPath(m_audioPlayer, m_audioOutput);
}

PhononPlayer::~PhononPlayer()
{
}

/*virtual*/ void PhononPlayer::startPlay(const QString& file)
{
    m_audioPlayer->setCurrentSource(file);
    m_audioPlayer->play();
}

/*virtual*/ void PhononPlayer::pause()
{
    m_audioPlayer->pause(); 
}

/*virtual*/ void PhononPlayer::stop() 
{
    m_audioPlayer->stop();
}

/*virtual*/ void PhononPlayer::setVolume(float volume)
{
    m_audioOutput->setVolume(volume);
}

/*virtual*/ float PhononPlayer::volume() const
{
    return m_audioOutput->volume();
}

/*virtual*/ bool PhononPlayer::playing() const
{
    return (m_audioPlayer->state() == Phonon::PlayingState || m_audioPlayer->state() == Phonon::BufferingState);
}

/*virtual*/ bool PhononPlayer::paused() const
{
    return m_audioPlayer->state() == Phonon::PausedState;
}

/*virtual*/ int PhononPlayer::totalTime() const
{
    return m_audioPlayer->totalTime();
}

/*virtual*/ int PhononPlayer::currentTime() const
{
    return m_audioPlayer->currentTime();
}

/*virtual*/ int PhononPlayer::position() const
{
    int totTime = totalTime();
    if (totTime > 0)
        return int(currentTime()/totTime);
    else
        return 0;
}

/*virtual*/ void PhononPlayer::seek(int seekTime)
{
    m_audioPlayer->seek(seekTime);
}

/*virtual*/ void PhononPlayer::seekPosition(int position)
{
    int totTime = totalTime();
    if (totTime > 0) {
        int pos = (int)(((long)position * totTime) / 100);
        seek(pos);
    }
}

#include "phononplayer.moc"

