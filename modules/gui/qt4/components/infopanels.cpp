/*****************************************************************************
 * infopanels.cpp : Panels for the information dialogs
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

#include "qt4.hpp"
#include "components/infopanels.hpp"

#include <QTreeWidget>
#include <QListView>
#include <QPushButton>
#include <QHeaderView>
#include <QList>
#include <QGridLayout>
#include <QLineEdit>
#include <QLabel>
#include <QSpinBox>
#include <QTabWidget>

/************************************************************************
 * Single panels
 ************************************************************************/

/* First Panel - Meta Info */

MetaPanel::MetaPanel( QWidget *parent, intf_thread_t *_p_intf ) :
                                    QWidget( parent ), p_intf( _p_intf )
{
    int line = 0;
    QGridLayout *l = new QGridLayout( this );
    l->setColumnStretch( 2, 5 );
    l->setColumnStretch( 5, 3 );

#define ADD_META( string, widget ) {                             \
    l->addWidget( new QLabel( qtr( string ) + " :" ), line, 0 ); \
    widget = new QLineEdit;                                      \
    l->addWidget( widget, line, 1, 1, 7 );                       \
    line++;            }

    ADD_META( VLC_META_TITLE, title_text ); /* OK */
    ADD_META( VLC_META_ARTIST, artist_text ); /* OK */
    ADD_META( VLC_META_GENRE, genre_text ); /* FIXME List id3genres.h is not
                                               includable yet ? */

    /* Album Name */
    l->addWidget( new QLabel( qfu( VLC_META_COLLECTION ) + " :" ), line, 0 );
    collection_text = new QLineEdit;
    l->addWidget( collection_text, line, 1, 1, 5 );
    l->addWidget( new QLabel( qfu( VLC_META_DATE ) + " :" ), line, 6 );
    /* Date (Should be in years) */
    date_text = new QSpinBox; setSpinBounds( date_text );
    l->addWidget( date_text, line, 7 );
    line++;

    /* Number and Rating */
    l->addWidget( new QLabel( qfu( _("Track number/Position" ) ) + " :" ),
                  line, 0 );
    seqnum_text = new QSpinBox; setSpinBounds( seqnum_text );
    l->addWidget( seqnum_text, line, 1, 1, 3 );

    l->addWidget( new QLabel( qfu( VLC_META_RATING ) + " :" ), line, 4 );
    rating_text = new QSpinBox; setSpinBounds( rating_text) ;
    l->addWidget( rating_text, line, 5, 1, 3 );
    line++;

    ADD_META( VLC_META_NOW_PLAYING, nowplaying_text );

    /* Language and settings */
    l->addWidget( new QLabel( qfu( VLC_META_LANGUAGE ) + " :" ), line, 0 );
    language_text = new QLineEdit;
    l->addWidget( language_text, line, 1, 1, 3 );
    l->addWidget( new QLabel( qfu( VLC_META_SETTING ) + " :" ), line, 4 );
    setting_text = new QLineEdit;
    l->addWidget( setting_text, line, 5, 1, 3 );
    line++;

    ADD_META( VLC_META_COPYRIGHT, copyright_text );
    ADD_META( VLC_META_PUBLISHER, publisher_text );

    ADD_META( VLC_META_ENCODED_BY, publisher_text );
    ADD_META( VLC_META_DESCRIPTION, description_text ); // Comment Two lines?

    /*  ADD_META( TRACKID)  DO NOT SHOW it */

    // ADD_META( _( "URI" ), uri_text ); // FIXME URI outside
    //    ADD_META( VLC_META_URL, setting_text );
    /* ART_URL */
}

MetaPanel::~MetaPanel()
{
}

void MetaPanel::update( input_item_t *p_item )
{
    char *psz_meta;
#define UPDATE_META( meta, widget ) {               \
    psz_meta = p_item->p_meta->psz_##meta;          \
    if( !EMPTY_STR( psz_meta ) )                    \
        widget->setText( qfu( psz_meta ) );         \
    else                                            \
        widget->setText( "" );          }

#define UPDATE_META_INT( meta, widget ) {           \
    psz_meta = p_item->p_meta->psz_##meta;          \
    if( !EMPTY_STR( psz_meta ) )                    \
        widget->setValue( atoi( psz_meta ) ); }/*FIXME Atoi ?*/

    /* Name / Title */
    psz_meta = p_item->p_meta->psz_title;
    if( !EMPTY_STR( psz_meta ) )
        title_text->setText( qfu( psz_meta ) );
    else if( !EMPTY_STR( p_item->psz_name ) )
        title_text->setText( qfu( p_item->psz_name ) );
    else title_text->setText( "" );

  /*  if( !EMPTY_STR( p_item->psz_uri ) )
        uri_text->setText( qfu( p_item->psz_uri ) );*/
///    else uri_text->setText( "" );

    UPDATE_META( artist, artist_text );
    UPDATE_META( genre, genre_text );
    UPDATE_META( copyright, copyright_text );
    UPDATE_META( album, collection_text );
    UPDATE_META( description, description_text );
    UPDATE_META( language, language_text );
    UPDATE_META( nowplaying, nowplaying_text );
    UPDATE_META( publisher, publisher_text );
    UPDATE_META( setting, setting_text );

    UPDATE_META_INT( date, date_text );
    UPDATE_META_INT( tracknum, seqnum_text );
    UPDATE_META_INT( rating, rating_text );

#undef UPDATE_META
}

void MetaPanel::clear(){}

ExtraMetaPanel::ExtraMetaPanel( QWidget *parent, intf_thread_t *_p_intf ) :
                                        QWidget( parent ), p_intf( _p_intf )
{
     QGridLayout *layout = new QGridLayout(this);
     QLabel *topLabel = new QLabel( qtr( "Extra metadata and other information"
                 " are shown in this list.\n" ) );
     topLabel->setWordWrap( true );
     layout->addWidget( topLabel, 0, 0 );

     extraMetaTree = new QTreeWidget( this );
     extraMetaTree->setAlternatingRowColors( true );
     extraMetaTree->setColumnCount( 2 );

     extraMetaTree->header()->hide();
/*     QStringList *treeHeaders;
     treeHeaders << qtr( "Test1" ) << qtr( "Test2" ); */

     layout->addWidget( extraMetaTree, 1, 0 );
}

void ExtraMetaPanel::update( input_item_t *p_item )
{
    vlc_meta_t *p_meta = p_item->p_meta;
    QStringList tempItem;

    QList<QTreeWidgetItem *> items;
    for (int i = 0; i < p_meta->i_extra; i++ )
    {
        tempItem.append( qfu( p_meta->ppsz_extra_name[i] ) + " : ");
        tempItem.append( qfu( p_meta->ppsz_extra_value[i] ) );
        items.append( new QTreeWidgetItem ( extraMetaTree, tempItem ) );
    }
    extraMetaTree->addTopLevelItems( items );
}

void ExtraMetaPanel::clear(){}

/* Second Panel - Stats */

InputStatsPanel::InputStatsPanel( QWidget *parent, intf_thread_t *_p_intf ) :
                                  QWidget( parent ), p_intf( _p_intf )
{
     QGridLayout *layout = new QGridLayout(this);
     StatsTree = new QTreeWidget(this);
     QList<QTreeWidgetItem *> items;
     QLabel *topLabel = new QLabel( qtr( "Various statistics about the current"
                 " media or stream.\n Played and streamed info are shown." ) );
     topLabel->setWordWrap( true );
     layout->addWidget( topLabel, 0, 0 );

     StatsTree->setColumnCount( 3 );
     StatsTree->header()->hide();

#define CREATE_TREE_ITEM( itemName, itemText, itemValue, unit ) {              \
    itemName =                                                                 \
      new QTreeWidgetItem((QStringList () << itemText << itemValue << unit )); \
    itemName->setTextAlignment( 1 , Qt::AlignRight ) ; }

#define CREATE_CATEGORY( catName, itemText ) {                           \
    CREATE_TREE_ITEM( catName, itemText , "", "" );                      \
    catName->setExpanded( true );                                        \
    StatsTree->addTopLevelItem( catName );    }

#define CREATE_AND_ADD_TO_CAT( itemName, itemText, itemValue, catName, unit ){ \
    CREATE_TREE_ITEM( itemName, itemText, itemValue, unit );                   \
    catName->addChild( itemName ); }

    /* Create the main categories */
    CREATE_CATEGORY( input, qtr("Input") );
    CREATE_CATEGORY( video, qtr("Video") );
    CREATE_CATEGORY( streaming, qtr("Streaming") );
    CREATE_CATEGORY( audio, qtr("Audio") );

    CREATE_AND_ADD_TO_CAT( read_media_stat, qtr("Read at media"),
                           "0", input , "kB" );
    CREATE_AND_ADD_TO_CAT( input_bitrate_stat, qtr("Input bitrate"),
                           "0", input, "kb/s" );
    CREATE_AND_ADD_TO_CAT( demuxed_stat, qtr("Demuxed"), "0", input, "kB") ;
    CREATE_AND_ADD_TO_CAT( stream_bitrate_stat, qtr("Stream bitrate"),
                           "0", input, "kb/s" );

    CREATE_AND_ADD_TO_CAT( vdecoded_stat, qtr("Decoded blocks"),
                           "0", video, "" );
    CREATE_AND_ADD_TO_CAT( vdisplayed_stat, qtr("Displayed frames"),
                           "0", video, "" );
    CREATE_AND_ADD_TO_CAT( vlost_frames_stat, qtr("Lost frames"),
                           "0", video, "" );

    CREATE_AND_ADD_TO_CAT( send_stat, qtr("Sent packets"), "0", streaming, "" );
    CREATE_AND_ADD_TO_CAT( send_bytes_stat, qtr("Sent bytes"),
                           "0", streaming, "kB" );
    CREATE_AND_ADD_TO_CAT( send_bitrate_stat, qtr("Sent bitrates"),
                           "0", streaming, "kb/s" );

    CREATE_AND_ADD_TO_CAT( adecoded_stat, qtr("Decoded blocks"),
                           "0", audio, "" );
    CREATE_AND_ADD_TO_CAT( aplayed_stat, qtr("Played buffers"),
                           "0", audio, "" );
    CREATE_AND_ADD_TO_CAT( alost_stat, qtr("Lost buffers"), "0", audio, "" );

    input->setExpanded( true );
    video->setExpanded( true );
    streaming->setExpanded( true );
    audio->setExpanded( true );

    StatsTree->resizeColumnToContents( 0 );
    StatsTree->setColumnWidth( 1 , 100 );

    layout->addWidget(StatsTree, 1, 0 );
}

InputStatsPanel::~InputStatsPanel()
{
}

void InputStatsPanel::update( input_item_t *p_item )
{
    vlc_mutex_lock( &p_item->p_stats->lock );

#define UPDATE( widget, format, calc... ) \
    { QString str; widget->setText( 1 , str.sprintf( format, ## calc ) );  }

    UPDATE( read_media_stat, "%8.0f",
            (float)(p_item->p_stats->i_read_bytes)/1000);
    UPDATE( input_bitrate_stat, "%6.0f",
                    (float)(p_item->p_stats->f_input_bitrate * 8000 ));
    UPDATE( demuxed_stat, "%8.0f",
                    (float)(p_item->p_stats->i_demux_read_bytes)/1000 );
    UPDATE( stream_bitrate_stat, "%6.0f",
                    (float)(p_item->p_stats->f_demux_bitrate * 8000 ));

    /* Video */
    UPDATE( vdecoded_stat, "%5i", p_item->p_stats->i_decoded_video );
    UPDATE( vdisplayed_stat, "%5i", p_item->p_stats->i_displayed_pictures );
    UPDATE( vlost_frames_stat, "%5i", p_item->p_stats->i_lost_pictures );

    /* Sout */
    UPDATE( send_stat, "%5i", p_item->p_stats->i_sent_packets );
    UPDATE( send_bytes_stat, "%8.0f",
            (float)(p_item->p_stats->i_sent_bytes)/1000 );
    UPDATE( send_bitrate_stat, "%6.0f",
            (float)(p_item->p_stats->f_send_bitrate*8)*1000 );

    /* Audio*/
    UPDATE( adecoded_stat, "%5i", p_item->p_stats->i_decoded_audio );
    UPDATE( aplayed_stat, "%5i", p_item->p_stats->i_played_abuffers );
    UPDATE( alost_stat, "%5i", p_item->p_stats->i_lost_abuffers );

    vlc_mutex_unlock(& p_item->p_stats->lock );
}

void InputStatsPanel::clear()
{
}

/* Third panel - Stream info */

InfoPanel::InfoPanel( QWidget *parent, intf_thread_t *_p_intf ) :
                                      QWidget( parent ), p_intf( _p_intf )
{
     QGridLayout *layout = new QGridLayout(this);
     InfoTree = new QTreeWidget(this);
     QList<QTreeWidgetItem *> items;

     QLabel *topLabel = new QLabel( qtr( "Information about what your media or"
              " stream is made of.\n Muxer, Audio and Video Codecs, Subtitles "
              "are shown." ) );
     topLabel->setWordWrap( true );
     layout->addWidget( topLabel, 0, 0 );

     InfoTree->setColumnCount( 1 );
     InfoTree->header()->hide();
     layout->addWidget(InfoTree, 1, 0 );
}

InfoPanel::~InfoPanel()
{
}

void InfoPanel::update( input_item_t *p_item)
{
    InfoTree->clear();
    QTreeWidgetItem *current_item = NULL;
    QTreeWidgetItem *child_item = NULL;

    for( int i = 0; i< p_item->i_categories ; i++)
    {
        current_item = new QTreeWidgetItem();
        current_item->setText( 0, qfu(p_item->pp_categories[i]->psz_name) );
        InfoTree->addTopLevelItem( current_item );

        for( int j = 0 ; j < p_item->pp_categories[i]->i_infos ; j++ )
        {
            child_item = new QTreeWidgetItem ();
            child_item->setText( 0,
                    qfu(p_item->pp_categories[i]->pp_infos[j]->psz_name)
                    + ": "
                    + qfu(p_item->pp_categories[i]->pp_infos[j]->psz_value));

            current_item->addChild(child_item);
        }
         InfoTree->setItemExpanded( current_item, true);
    }
}

void InfoPanel::clear()
{
    InfoTree->clear();
}
