/* GStreamer
 * Copyright (C) 2020 FIXME <fixme@example.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Suite 500,
 * Boston, MA 02110-1335, USA.
 */
/**
 * SECTION:element-gstadvaudiosrc
 *
 * The advaudiosrc element does FIXME stuff.
 *
 * <refsect2>
 * <title>Example launch line</title>
 * |[
 * gst-launch-1.0 -v fakesrc ! advaudiosrc ! FIXME ! fakesink
 * ]|
 * FIXME Describe what the pipeline does.
 * </refsect2>
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gst/gst.h>
#include <gst/audio/gstaudiosrc.h>
#include "gstadvaudiosrc.h"

GST_DEBUG_CATEGORY_STATIC (gst_adv_audio_src_debug_category);
#define GST_CAT_DEFAULT gst_adv_audio_src_debug_category

/* prototypes */


static void gst_adv_audio_src_set_property (GObject * object,
    guint property_id, const GValue * value, GParamSpec * pspec);
static void gst_adv_audio_src_get_property (GObject * object,
    guint property_id, GValue * value, GParamSpec * pspec);
static void gst_adv_audio_src_dispose (GObject * object);
static void gst_adv_audio_src_finalize (GObject * object);

static gboolean gst_adv_audio_src_open (GstAudioSrc * src);
static gboolean gst_adv_audio_src_prepare (GstAudioSrc * src,
    GstAudioRingBufferSpec * spec);
static gboolean gst_adv_audio_src_unprepare (GstAudioSrc * src);
static gboolean gst_adv_audio_src_close (GstAudioSrc * src);
static guint gst_adv_audio_src_read (GstAudioSrc * src, gpointer data,
    guint length, GstClockTime * timestamp);
static guint gst_adv_audio_src_delay (GstAudioSrc * src);
static void gst_adv_audio_src_reset (GstAudioSrc * src);

enum
{
  PROP_0
};

/* pad templates */

/* FIXME add/remove the formats that you want to support */
static GstStaticPadTemplate gst_adv_audio_src_src_template =
GST_STATIC_PAD_TEMPLATE ("src",
    GST_PAD_SRC,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS ("audio/x-raw,format=S16LE,rate=[1,max],"
        "channels=[1,max],layout=interleaved")
    );


/* class initialization */

G_DEFINE_TYPE_WITH_CODE (GstAdvAudioSrc, gst_adv_audio_src, GST_TYPE_AUDIO_SRC,
    GST_DEBUG_CATEGORY_INIT (gst_adv_audio_src_debug_category, "advaudiosrc", 0,
        "debug category for advaudiosrc element"));

static void
gst_adv_audio_src_class_init (GstAdvAudioSrcClass * klass)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
  GstAudioSrcClass *audio_src_class = GST_AUDIO_SRC_CLASS (klass);

  /* Setting up pads and setting metadata should be moved to
     base_class_init if you intend to subclass this class. */
  gst_element_class_add_static_pad_template (GST_ELEMENT_CLASS (klass),
      &gst_adv_audio_src_src_template);

  gst_element_class_set_static_metadata (GST_ELEMENT_CLASS (klass),
      "FIXME Long name", "Generic", "FIXME Description",
      "FIXME <fixme@example.com>");

  gobject_class->set_property = gst_adv_audio_src_set_property;
  gobject_class->get_property = gst_adv_audio_src_get_property;
  gobject_class->dispose = gst_adv_audio_src_dispose;
  gobject_class->finalize = gst_adv_audio_src_finalize;
  audio_src_class->open = GST_DEBUG_FUNCPTR (gst_adv_audio_src_open);
  audio_src_class->prepare = GST_DEBUG_FUNCPTR (gst_adv_audio_src_prepare);
  audio_src_class->unprepare = GST_DEBUG_FUNCPTR (gst_adv_audio_src_unprepare);
  audio_src_class->close = GST_DEBUG_FUNCPTR (gst_adv_audio_src_close);
  audio_src_class->read = GST_DEBUG_FUNCPTR (gst_adv_audio_src_read);
  audio_src_class->delay = GST_DEBUG_FUNCPTR (gst_adv_audio_src_delay);
  audio_src_class->reset = GST_DEBUG_FUNCPTR (gst_adv_audio_src_reset);

}

static void
gst_adv_audio_src_init (GstAdvAudioSrc * advaudiosrc)
{
}

void
gst_adv_audio_src_set_property (GObject * object, guint property_id,
    const GValue * value, GParamSpec * pspec)
{
  GstAdvAudioSrc *advaudiosrc = GST_ADV_AUDIO_SRC (object);

  GST_DEBUG_OBJECT (advaudiosrc, "set_property");

  switch (property_id) {
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
  }
}

void
gst_adv_audio_src_get_property (GObject * object, guint property_id,
    GValue * value, GParamSpec * pspec)
{
  GstAdvAudioSrc *advaudiosrc = GST_ADV_AUDIO_SRC (object);

  GST_DEBUG_OBJECT (advaudiosrc, "get_property");

  switch (property_id) {
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
  }
}

void
gst_adv_audio_src_dispose (GObject * object)
{
  GstAdvAudioSrc *advaudiosrc = GST_ADV_AUDIO_SRC (object);

  GST_DEBUG_OBJECT (advaudiosrc, "dispose");

  /* clean up as possible.  may be called multiple times */

  G_OBJECT_CLASS (gst_adv_audio_src_parent_class)->dispose (object);
}

void
gst_adv_audio_src_finalize (GObject * object)
{
  GstAdvAudioSrc *advaudiosrc = GST_ADV_AUDIO_SRC (object);

  GST_DEBUG_OBJECT (advaudiosrc, "finalize");

  /* clean up object here */

  G_OBJECT_CLASS (gst_adv_audio_src_parent_class)->finalize (object);
}

/* open the device with given specs */
static gboolean
gst_adv_audio_src_open (GstAudioSrc * src)
{
  GstAdvAudioSrc *advaudiosrc = GST_ADV_AUDIO_SRC (src);

  GST_DEBUG_OBJECT (advaudiosrc, "open");

  return TRUE;
}

/* prepare resources and state to operate with the given specs */
static gboolean
gst_adv_audio_src_prepare (GstAudioSrc * src, GstAudioRingBufferSpec * spec)
{
  GstAdvAudioSrc *advaudiosrc = GST_ADV_AUDIO_SRC (src);

  GST_DEBUG_OBJECT (advaudiosrc, "prepare");

  return TRUE;
}

/* undo anything that was done in prepare() */
static gboolean
gst_adv_audio_src_unprepare (GstAudioSrc * src)
{
  GstAdvAudioSrc *advaudiosrc = GST_ADV_AUDIO_SRC (src);

  GST_DEBUG_OBJECT (advaudiosrc, "unprepare");

  return TRUE;
}

/* close the device */
static gboolean
gst_adv_audio_src_close (GstAudioSrc * src)
{
  GstAdvAudioSrc *advaudiosrc = GST_ADV_AUDIO_SRC (src);

  GST_DEBUG_OBJECT (advaudiosrc, "close");

  return TRUE;
}

/* read samples from the device */
static guint
gst_adv_audio_src_read (GstAudioSrc * src, gpointer data, guint length,
    GstClockTime * timestamp)
{
  GstAdvAudioSrc *advaudiosrc = GST_ADV_AUDIO_SRC (src);

  GST_DEBUG_OBJECT (advaudiosrc, "read");

  return 0;
}

/* get number of samples queued in the device */
static guint
gst_adv_audio_src_delay (GstAudioSrc * src)
{
  GstAdvAudioSrc *advaudiosrc = GST_ADV_AUDIO_SRC (src);

  GST_DEBUG_OBJECT (advaudiosrc, "delay");

  return 0;
}

/* reset the audio device, unblock from a write */
static void
gst_adv_audio_src_reset (GstAudioSrc * src)
{
  GstAdvAudioSrc *advaudiosrc = GST_ADV_AUDIO_SRC (src);

  GST_DEBUG_OBJECT (advaudiosrc, "reset");

}
/*
static gboolean
plugin_init (GstPlugin * plugin)
{
*/
  /* FIXME Remember to set the rank if it's an element that is meant
     to be autoplugged by decodebin. */
 /*
  return gst_element_register (plugin, "advaudiosrc", GST_RANK_NONE,
      GST_TYPE_ADVAUDIOSRC);
}
*/

/* FIXME: these are normally defined by the GStreamer build system.
   If you are creating an element to be included in gst-plugins-*,
   remove these, as they're always defined.  Otherwise, edit as
   appropriate for your external plugin package. */
/*
#ifndef VERSION
#define VERSION "0.0.FIXME"
#endif
#ifndef PACKAGE
#define PACKAGE "FIXME_package"
#endif
#ifndef PACKAGE_NAME
#define PACKAGE_NAME "FIXME_package_name"
#endif
#ifndef GST_PACKAGE_ORIGIN
#define GST_PACKAGE_ORIGIN "http://FIXME.org/"
#endif

GST_PLUGIN_DEFINE (GST_VERSION_MAJOR,
    GST_VERSION_MINOR,
    advaudiosrc,
    "FIXME plugin description",
    plugin_init, VERSION, "LGPL", PACKAGE_NAME, GST_PACKAGE_ORIGIN)
*/