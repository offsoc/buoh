/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 *  Authors : Carlos Garc�a Campos <carlosgc@gnome.org>
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <glib.h>
#include <gtk/gtk.h>

#include "buoh-view-message.h"

struct _BuohViewMessagePrivate {
	GtkWidget *title;
	GtkWidget *text;
	GtkWidget *icon;
};

#define BUOH_VIEW_MESSAGE_GET_PRIVATE(object) \
        (G_TYPE_INSTANCE_GET_PRIVATE ((object), BUOH_TYPE_VIEW_MESSAGE, BuohViewMessagePrivate))

static GtkViewportClass *parent_class = NULL;

static void buoh_view_message_init          (BuohViewMessage *m_view);
static void buoh_view_message_class_init    (BuohViewMessageClass *klass);

GType
buoh_view_message_get_type ()
{
	static GType type = 0;

	if (!type) {
		static const GTypeInfo info = {
			sizeof (BuohViewMessageClass),
			(GBaseInitFunc) NULL,
			(GBaseFinalizeFunc) NULL,
			(GClassInitFunc) buoh_view_message_class_init,
			NULL,
			NULL,
			sizeof (BuohViewMessage),
			0,
			(GInstanceInitFunc) buoh_view_message_init
		};

		type = g_type_register_static (GTK_TYPE_VIEWPORT, "BuohViewMessage",
					       &info, 0);
	}

	return type;
}

static void
buoh_view_message_init (BuohViewMessage *m_view)
{
	GtkWidget *align;
	GtkWidget *hbox;
	GtkWidget *vbox;
	
	m_view->priv = BUOH_VIEW_MESSAGE_GET_PRIVATE (m_view);

	vbox = gtk_vbox_new (FALSE, 24);

	m_view->priv->title = gtk_label_new (NULL);
	gtk_label_set_line_wrap (GTK_LABEL (m_view->priv->title), TRUE);
	gtk_label_set_selectable (GTK_LABEL (m_view->priv->title), TRUE);
	gtk_misc_set_alignment (GTK_MISC  (m_view->priv->title), 0.5, 0.0);
	gtk_box_pack_start (GTK_BOX (vbox), m_view->priv->title,
			    FALSE, FALSE, 0);
	gtk_widget_show (m_view->priv->title);
	
	m_view->priv->text = gtk_label_new (NULL);
	gtk_label_set_line_wrap (GTK_LABEL (m_view->priv->text), TRUE);
	gtk_label_set_selectable (GTK_LABEL (m_view->priv->text), TRUE);
	gtk_misc_set_alignment (GTK_MISC  (m_view->priv->text), 0.5, 0.5);
	gtk_box_pack_start (GTK_BOX (vbox), m_view->priv->text,
			    TRUE, TRUE, 0);
	gtk_widget_show (m_view->priv->text);

	hbox = gtk_hbox_new (FALSE, 24);
	gtk_container_set_border_width (GTK_CONTAINER (hbox), 24);
	
	m_view->priv->icon = gtk_image_new_from_stock (NULL, GTK_ICON_SIZE_DIALOG);
	gtk_misc_set_alignment (GTK_MISC (m_view->priv->icon), 0.5, 0.0);
	gtk_box_pack_start (GTK_BOX (hbox), m_view->priv->icon,
			    TRUE, FALSE, 0);
	gtk_widget_show (m_view->priv->icon);

	gtk_box_pack_start (GTK_BOX (hbox), vbox,
			    TRUE, TRUE, 0);
	gtk_widget_show (vbox);

	align = gtk_alignment_new (0.5, 0.5, 0.0, 0.0);
	gtk_container_add (GTK_CONTAINER (align), hbox);
	gtk_widget_show (hbox);

	gtk_container_add (GTK_CONTAINER (m_view), align);
	gtk_widget_show (align);

	gtk_widget_show (GTK_WIDGET (m_view));
}

static void
buoh_view_message_class_init (BuohViewMessageClass *klass)
{
	parent_class = g_type_class_peek_parent (klass);

	g_type_class_add_private (klass, sizeof (BuohViewMessagePrivate));
}

GtkWidget *
buoh_view_message_new (void)
{
	GtkWidget *m_view;

	m_view = GTK_WIDGET (g_object_new (BUOH_TYPE_VIEW_MESSAGE,
					   "border-width", 24,
					   "shadow-type", GTK_SHADOW_IN,
					   NULL));
	return m_view;
}

void
buoh_view_message_set_title (BuohViewMessage *m_view, const gchar *title)
{
	gint size;
	PangoFontDescription *font_desc;
	
	g_return_if_fail (title != NULL);

	gtk_label_set_text (GTK_LABEL (m_view->priv->title), title);
	
	/* unset the font settings */
	gtk_widget_modify_font (m_view->priv->title, NULL);

	size = pango_font_description_get_size (m_view->priv->title->style->font_desc);
	
	font_desc = pango_font_description_new ();

	pango_font_description_set_weight (font_desc, PANGO_WEIGHT_BOLD);
	pango_font_description_set_size (font_desc, size * PANGO_SCALE_X_LARGE);

	gtk_widget_modify_font (m_view->priv->title, font_desc);

	pango_font_description_free (font_desc);

	gtk_widget_show (m_view->priv->title);
}

void
buoh_view_message_set_text (BuohViewMessage *m_view, const gchar *text)
{
	g_return_if_fail (text != NULL);

	gtk_label_set_markup (GTK_LABEL (m_view->priv->text), text);

	gtk_widget_show (m_view->priv->text);
}

void
buoh_view_message_set_icon (BuohViewMessage *m_view, const gchar *icon)
{
	gchar       *icon_path;
	GdkPixbuf   *pixbuf = NULL;
	GtkStockItem item;
	
	g_return_if_fail (icon != NULL);

	if (gtk_stock_lookup (icon, &item)) {
		gtk_image_set_from_stock (GTK_IMAGE (m_view->priv->icon), icon,
					  GTK_ICON_SIZE_DIALOG);
	} else {
		icon_path = g_build_filename (PIXMAPS_DIR, icon, NULL);
		pixbuf = gdk_pixbuf_new_from_file_at_size (icon_path, 48, 48, NULL);
		g_free (icon_path);

		if (pixbuf) {
			gtk_image_set_from_pixbuf (GTK_IMAGE (m_view->priv->icon),
						   pixbuf);
			g_object_unref (pixbuf);
		} else {
			gtk_image_set_from_stock (GTK_IMAGE (m_view->priv->icon),
						  NULL, GTK_ICON_SIZE_DIALOG);
		}
	}
}