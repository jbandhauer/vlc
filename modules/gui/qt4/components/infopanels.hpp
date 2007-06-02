/*****************************************************************************
 * infopanels.hpp : Panels for the information dialogs
 ****************************************************************************
 * Copyright (C) 2006-2007 the VideoLAN team
 * $Id$
 *
 * Authors: Clément Stenac <zorglub@videolan.org>
 *          Jean-Baptiste Kempf <jb@videolan.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

#ifndef _INFOPANELS_H_
#define _INFOPANELS_H_

#include <vlc/vlc.h>
#include <vlc_meta.h>

#include <QWidget>

#ifdef HAVE_LIMITS_H
#   include <limits.h>
#endif

#define setSpinBounds( spinbox ) {               \
    spinbox->setRange( 0, INT_MAX );             \
    spinbox->setAccelerated( true ) ;            \
    spinbox->setAlignment( Qt::AlignRight );     \
    spinbox->setSpecialValueText(""); }

class QTreeWidget;
class QTreeWidgetItem;
class QTreeView;
class QSpinBox;
class QLineEdit;

class MetaPanel: public QWidget
{
    Q_OBJECT;
public:
    MetaPanel( QWidget *, intf_thread_t * );
    virtual ~MetaPanel();
private:
    intf_thread_t *p_intf;
    QLineEdit *uri_text;
    QLineEdit *title_text;
    QLineEdit *artist_text;
    QLineEdit *genre_text;
    QLineEdit *copyright_text;
    QLineEdit *collection_text;
    QSpinBox *seqnum_text;
    QLineEdit *description_text;
    QSpinBox *rating_text;
    QSpinBox *date_text;
    QLineEdit *setting_text;
    QLineEdit *language_text;
    QLineEdit *nowplaying_text;
    QLineEdit *publisher_text;

public slots:
    void update( input_item_t * );
    void clear();
};

class ExtraMetaPanel: public QWidget
{
    Q_OBJECT;
public:
    ExtraMetaPanel( QWidget *, intf_thread_t * );
    virtual  ~ExtraMetaPanel() {};
private:
    intf_thread_t *p_intf;
    QTreeWidget *extraMetaTree;
public slots:
    void update( input_item_t * );
    void clear();
};

class InputStatsPanel: public QWidget
{
    Q_OBJECT;
public:
    InputStatsPanel( QWidget *, intf_thread_t * );
    virtual ~InputStatsPanel();
private:
    intf_thread_t *p_intf;

    QTreeWidget *StatsTree;
    QTreeWidgetItem *input;
    QTreeWidgetItem *read_media_stat;
    QTreeWidgetItem *input_bitrate_stat;
    QTreeWidgetItem *demuxed_stat;
    QTreeWidgetItem *stream_bitrate_stat;

    QTreeWidgetItem *video;
    QTreeWidgetItem *vdecoded_stat;
    QTreeWidgetItem *vdisplayed_stat;
    QTreeWidgetItem *vlost_frames_stat;

    QTreeWidgetItem *streaming;
    QTreeWidgetItem *send_stat;
    QTreeWidgetItem *send_bytes_stat;
    QTreeWidgetItem *send_bitrate_stat;

    QTreeWidgetItem *audio;
    QTreeWidgetItem *adecoded_stat;
    QTreeWidgetItem *aplayed_stat;
    QTreeWidgetItem *alost_stat;

public slots:
    void update( input_item_t * );
    void clear();
};

class InfoPanel: public QWidget
{
    Q_OBJECT;
public:
    InfoPanel( QWidget *, intf_thread_t * );
    virtual ~InfoPanel();
private:
    intf_thread_t *p_intf;
    QTreeWidget *InfoTree;
public slots:
    void update( input_item_t * );
    void clear();
};
#endif
