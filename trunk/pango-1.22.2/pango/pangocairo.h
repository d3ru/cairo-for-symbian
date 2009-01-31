/* Pango
 * pangocairo.h:
 *
 * Copyright (C) 1999, 2004 Red Hat, Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifndef __PANGOCAIRO_H__
#define __PANGOCAIRO_H__

#include <pango/pango-compiler-private.h>
#include <pango/pango.h>
#include <cairo.h>

G_BEGIN_DECLS

/**
 * PangoCairoFont:
 *
 * #PangoCairoFont is an interface exported by fonts for
 * use with Cairo. The actual type of the font will depend
 * on the particular font technology Cairo was compiled to use.
 *
 * Since: 1.18
 **/
typedef struct _PangoCairoFont      PangoCairoFont;
#define PANGO_TYPE_CAIRO_FONT       (pango_cairo_font_get_type ())
#define PANGO_CAIRO_FONT(object)    (G_TYPE_CHECK_INSTANCE_CAST ((object), PANGO_TYPE_CAIRO_FONT, PangoCairoFont))
#define PANGO_IS_CAIRO_FONT(object) (G_TYPE_CHECK_INSTANCE_TYPE ((object), PANGO_TYPE_CAIRO_FONT))

/**
 * PangoCairoFontMap:
 *
 * #PangoCairoFontMap is an interface exported by font maps for
 * use with Cairo. The actual type of the font map will depend
 * on the particular font technology Cairo was compiled to use.
 *
 * Since: 1.10
 **/
typedef struct _PangoCairoFontMap        PangoCairoFontMap;
#define PANGO_TYPE_CAIRO_FONT_MAP       (pango_cairo_font_map_get_type ())
#define PANGO_CAIRO_FONT_MAP(object)    (G_TYPE_CHECK_INSTANCE_CAST ((object), PANGO_TYPE_CAIRO_FONT_MAP, PangoCairoFontMap))
#define PANGO_IS_CAIRO_FONT_MAP(object) (G_TYPE_CHECK_INSTANCE_TYPE ((object), PANGO_TYPE_CAIRO_FONT_MAP))

typedef void (* PangoCairoShapeRendererFunc) (cairo_t        *cr,
					      PangoAttrShape *attr,
					      gboolean        do_path,
					      gpointer        data);

/*
 * PangoCairoFontMap
 */
PangoApi GType         pango_cairo_font_map_get_type          (void) G_GNUC_CONST;

PangoApi PangoFontMap *pango_cairo_font_map_new               (void);
PangoApi PangoFontMap *pango_cairo_font_map_new_for_font_type (cairo_font_type_t fonttype);
PangoApi PangoFontMap *pango_cairo_font_map_get_default       (void);
PangoApi void          pango_cairo_font_map_set_default       (PangoCairoFontMap *fontmap);
PangoApi cairo_font_type_t pango_cairo_font_map_get_font_type (PangoCairoFontMap *fontmap);

PangoApi void          pango_cairo_font_map_set_resolution (PangoCairoFontMap *fontmap,
						   double             dpi);
PangoApi double        pango_cairo_font_map_get_resolution (PangoCairoFontMap *fontmap);
#ifndef PANGO_DISABLE_DEPRECATED
PangoApi PangoContext *pango_cairo_font_map_create_context (PangoCairoFontMap *fontmap);
#endif

/*
 * PangoCairoFont
 */
PangoApi GType         pango_cairo_font_get_type               (void) G_GNUC_CONST;

PangoApi cairo_scaled_font_t *pango_cairo_font_get_scaled_font (PangoCairoFont *font);

/* Update a Pango context for the current state of a cairo context
 */
PangoApi void         pango_cairo_update_context (cairo_t      *cr,
					 PangoContext *context);

PangoApi void                        pango_cairo_context_set_font_options (PangoContext               *context,
								  const cairo_font_options_t *options);
PangoApi const cairo_font_options_t *pango_cairo_context_get_font_options (PangoContext               *context);

PangoApi void               pango_cairo_context_set_resolution     (PangoContext       *context,
							   double              dpi);
PangoApi double             pango_cairo_context_get_resolution     (PangoContext       *context);

PangoApi void                        pango_cairo_context_set_shape_renderer (PangoContext                *context,
								    PangoCairoShapeRendererFunc  func,
								    gpointer                     data,
								    GDestroyNotify               dnotify);
PangoApi PangoCairoShapeRendererFunc pango_cairo_context_get_shape_renderer (PangoContext                *context,
								    gpointer                    *data);

/* Convenience
 */
PangoApi PangoContext *pango_cairo_create_context (cairo_t   *cr);
PangoApi PangoLayout *pango_cairo_create_layout (cairo_t     *cr);
PangoApi void         pango_cairo_update_layout (cairo_t     *cr,
					PangoLayout *layout);

/*
 * Rendering
 */
PangoApi void pango_cairo_show_glyph_string (cairo_t          *cr,
				    PangoFont        *font,
				    PangoGlyphString *glyphs);
PangoApi void pango_cairo_show_glyph_item   (cairo_t          *cr,
				    const char       *text,
				    PangoGlyphItem   *glyph_item);
PangoApi void pango_cairo_show_layout_line  (cairo_t          *cr,
				    PangoLayoutLine  *line);
PangoApi void pango_cairo_show_layout       (cairo_t          *cr,
				    PangoLayout      *layout);

PangoApi void pango_cairo_show_error_underline (cairo_t       *cr,
				       double         x,
				       double         y,
				       double         width,
				       double         height);

/*
 * Rendering to a path
 */
PangoApi void pango_cairo_glyph_string_path (cairo_t          *cr,
				    PangoFont        *font,
				    PangoGlyphString *glyphs);
PangoApi void pango_cairo_layout_line_path  (cairo_t          *cr,
				    PangoLayoutLine  *line);
PangoApi void pango_cairo_layout_path       (cairo_t          *cr,
				    PangoLayout      *layout);

PangoApi void pango_cairo_error_underline_path (cairo_t       *cr,
				       double         x,
				       double         y,
				       double         width,
				       double         height);

G_END_DECLS

#endif /* __PANGOCAIRO_H__ */
