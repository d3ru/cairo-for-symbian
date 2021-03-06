/* Pango
 * pango-layout.h: High-level layout driver
 *
 * Copyright (C) 2000 Red Hat Software
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

#ifndef __PANGO_LAYOUT_H__
#define __PANGO_LAYOUT_H__

#include <pango/pango-compiler-private.h>
#include <pango/pango-attributes.h>
#include <pango/pango-context.h>
#include <pango/pango-glyph-item.h>
#include <pango/pango-tabs.h>

G_BEGIN_DECLS

typedef struct _PangoLayout      PangoLayout;
typedef struct _PangoLayoutClass PangoLayoutClass;
typedef struct _PangoLayoutLine  PangoLayoutLine;

typedef PangoGlyphItem PangoLayoutRun;

typedef enum {
  PANGO_ALIGN_LEFT,
  PANGO_ALIGN_CENTER,
  PANGO_ALIGN_RIGHT
} PangoAlignment;

typedef enum {
  PANGO_WRAP_WORD,
  PANGO_WRAP_CHAR,
  PANGO_WRAP_WORD_CHAR
} PangoWrapMode;

/**
 * PangoEllipsizeMode
 * @PANGO_ELLIPSIZE_NONE: No ellipsization
 * @PANGO_ELLIPSIZE_START: Omit characters at the start of the text
 * @PANGO_ELLIPSIZE_MIDDLE: Omit characters in the middle of the text
 * @PANGO_ELLIPSIZE_END: Omit characters at the end of the text
 *
 * The #PangoEllipsizeMode type describes what sort of (if any)
 * ellipsization should be applied to a line of text. In
 * the ellipsization process characters are removed from the
 * text in order to make it fit to a given width and replaced
 * with an ellipsis.
 */
typedef enum {
  PANGO_ELLIPSIZE_NONE,
  PANGO_ELLIPSIZE_START,
  PANGO_ELLIPSIZE_MIDDLE,
  PANGO_ELLIPSIZE_END
} PangoEllipsizeMode;

struct _PangoLayoutLine
{
  PangoLayout *layout;
  gint         start_index;     /* start of line as byte index into layout->text */
  gint         length;		/* length of line in bytes */
  GSList      *runs;
  guint        is_paragraph_start : 1;  /* TRUE if this is the first line of the paragraph */
  guint        resolved_dir : 3;  /* Resolved PangoDirection of line */
};

#define PANGO_TYPE_LAYOUT              (pango_layout_get_type ())
#define PANGO_LAYOUT(object)           (G_TYPE_CHECK_INSTANCE_CAST ((object), PANGO_TYPE_LAYOUT, PangoLayout))
#define PANGO_LAYOUT_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), PANGO_TYPE_LAYOUT, PangoLayoutClass))
#define PANGO_IS_LAYOUT(object)        (G_TYPE_CHECK_INSTANCE_TYPE ((object), PANGO_TYPE_LAYOUT))
#define PANGO_IS_LAYOUT_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), PANGO_TYPE_LAYOUT))
#define PANGO_LAYOUT_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), PANGO_TYPE_LAYOUT, PangoLayoutClass))

/* The PangoLayout and PangoLayoutClass structs are private; if you
 * need to create a subclass of these, file a bug.
 */

PangoApi GType        pango_layout_get_type       (void) G_GNUC_CONST;
PangoApi PangoLayout *pango_layout_new            (PangoContext   *context);
PangoApi PangoLayout *pango_layout_copy           (PangoLayout    *src);

PangoApi PangoContext  *pango_layout_get_context    (PangoLayout    *layout);

PangoApi void           pango_layout_set_attributes (PangoLayout    *layout,
					    PangoAttrList  *attrs);
PangoApi PangoAttrList *pango_layout_get_attributes (PangoLayout    *layout);

PangoApi void           pango_layout_set_text       (PangoLayout    *layout,
					    const char     *text,
					    int             length);
PangoApi const char    *pango_layout_get_text       (PangoLayout    *layout);

PangoApi void           pango_layout_set_markup     (PangoLayout    *layout,
					    const char     *markup,
					    int             length);

PangoApi void           pango_layout_set_markup_with_accel (PangoLayout    *layout,
						   const char     *markup,
						   int             length,
						   gunichar        accel_marker,
						   gunichar       *accel_char);

PangoApi void           pango_layout_set_font_description (PangoLayout                *layout,
						  const PangoFontDescription *desc);

PangoApi G_CONST_RETURN PangoFontDescription *pango_layout_get_font_description (PangoLayout *layout);

PangoApi void           pango_layout_set_width            (PangoLayout                *layout,
						  int                         width);
PangoApi int            pango_layout_get_width            (PangoLayout                *layout);
PangoApi void           pango_layout_set_height           (PangoLayout                *layout,
						  int                         height);
PangoApi int            pango_layout_get_height           (PangoLayout                *layout);
PangoApi void           pango_layout_set_wrap             (PangoLayout                *layout,
						  PangoWrapMode               wrap);
PangoApi PangoWrapMode  pango_layout_get_wrap             (PangoLayout                *layout);
PangoApi gboolean       pango_layout_is_wrapped           (PangoLayout                *layout);
PangoApi void           pango_layout_set_indent           (PangoLayout                *layout,
						  int                         indent);
PangoApi int            pango_layout_get_indent           (PangoLayout                *layout);
PangoApi void           pango_layout_set_spacing          (PangoLayout                *layout,
						  int                         spacing);
PangoApi int            pango_layout_get_spacing          (PangoLayout                *layout);
PangoApi void           pango_layout_set_justify          (PangoLayout                *layout,
						  gboolean                    justify);
PangoApi gboolean       pango_layout_get_justify          (PangoLayout                *layout);
PangoApi void           pango_layout_set_auto_dir         (PangoLayout                *layout,
						  gboolean                    auto_dir);
PangoApi gboolean       pango_layout_get_auto_dir         (PangoLayout                *layout);
PangoApi void           pango_layout_set_alignment        (PangoLayout                *layout,
						  PangoAlignment              alignment);
PangoApi PangoAlignment pango_layout_get_alignment        (PangoLayout                *layout);

PangoApi void           pango_layout_set_tabs             (PangoLayout                *layout,
						  PangoTabArray              *tabs);

PangoApi PangoTabArray* pango_layout_get_tabs             (PangoLayout                *layout);

PangoApi void           pango_layout_set_single_paragraph_mode (PangoLayout                *layout,
						       gboolean                    setting);
PangoApi gboolean       pango_layout_get_single_paragraph_mode (PangoLayout                *layout);

PangoApi void               pango_layout_set_ellipsize (PangoLayout        *layout,
					       PangoEllipsizeMode  ellipsize);
PangoApi PangoEllipsizeMode pango_layout_get_ellipsize (PangoLayout        *layout);
PangoApi gboolean           pango_layout_is_ellipsized (PangoLayout        *layout);

PangoApi int      pango_layout_get_unknown_glyphs_count (PangoLayout    *layout);

PangoApi void     pango_layout_context_changed (PangoLayout    *layout);

PangoApi void     pango_layout_get_log_attrs (PangoLayout    *layout,
				     PangoLogAttr  **attrs,
				     gint           *n_attrs);

PangoApi void     pango_layout_index_to_pos         (PangoLayout    *layout,
					    int             index_,
					    PangoRectangle *pos);
PangoApi void     pango_layout_index_to_line_x      (PangoLayout    *layout,
					    int             index_,
					    gboolean        trailing,
					    int            *line,
					    int            *x_pos);
PangoApi void     pango_layout_get_cursor_pos       (PangoLayout    *layout,
					    int             index_,
					    PangoRectangle *strong_pos,
					    PangoRectangle *weak_pos);
PangoApi void     pango_layout_move_cursor_visually (PangoLayout    *layout,
					    gboolean        strong,
					    int             old_index,
					    int             old_trailing,
					    int             direction,
					    int            *new_index,
					    int            *new_trailing);
PangoApi gboolean pango_layout_xy_to_index          (PangoLayout    *layout,
					    int             x,
					    int             y,
					    int            *index_,
					    int            *trailing);
PangoApi void     pango_layout_get_extents          (PangoLayout    *layout,
					    PangoRectangle *ink_rect,
					    PangoRectangle *logical_rect);
PangoApi void     pango_layout_get_pixel_extents    (PangoLayout    *layout,
					    PangoRectangle *ink_rect,
					    PangoRectangle *logical_rect);
PangoApi void     pango_layout_get_size             (PangoLayout    *layout,
					    int            *width,
					    int            *height);
PangoApi void     pango_layout_get_pixel_size       (PangoLayout    *layout,
					    int            *width,
					    int            *height);
PangoApi int      pango_layout_get_baseline         (PangoLayout    *layout);

PangoApi int              pango_layout_get_line_count       (PangoLayout    *layout);
PangoApi PangoLayoutLine *pango_layout_get_line             (PangoLayout    *layout,
						    int             line);
PangoApi PangoLayoutLine *pango_layout_get_line_readonly    (PangoLayout    *layout,
						    int             line);
PangoApi GSList *         pango_layout_get_lines            (PangoLayout    *layout);
PangoApi GSList *         pango_layout_get_lines_readonly   (PangoLayout    *layout);


#define PANGO_TYPE_LAYOUT_LINE (pango_layout_line_get_type ())

PangoApi GType    pango_layout_line_get_type     (void) G_GNUC_CONST;

PangoApi PangoLayoutLine *pango_layout_line_ref   (PangoLayoutLine *line);
PangoApi void             pango_layout_line_unref (PangoLayoutLine *line);

PangoApi gboolean pango_layout_line_x_to_index   (PangoLayoutLine  *line,
					 int               x_pos,
					 int              *index_,
					 int              *trailing);
PangoApi void     pango_layout_line_index_to_x   (PangoLayoutLine  *line,
					 int               index_,
					 gboolean          trailing,
					 int              *x_pos);
PangoApi void     pango_layout_line_get_x_ranges (PangoLayoutLine  *line,
					 int               start_index,
					 int               end_index,
					 int             **ranges,
					 int              *n_ranges);
PangoApi void     pango_layout_line_get_extents  (PangoLayoutLine  *line,
					 PangoRectangle   *ink_rect,
					 PangoRectangle   *logical_rect);
PangoApi void     pango_layout_line_get_pixel_extents (PangoLayoutLine *layout_line,
					      PangoRectangle  *ink_rect,
					      PangoRectangle  *logical_rect);

typedef struct _PangoLayoutIter PangoLayoutIter;

#define PANGO_TYPE_LAYOUT_ITER         (pango_layout_iter_get_type ())

PangoApi GType            pango_layout_iter_get_type (void) G_GNUC_CONST;

PangoApi PangoLayoutIter *pango_layout_get_iter  (PangoLayout     *layout);
PangoApi PangoLayoutIter *pango_layout_iter_copy (PangoLayoutIter *iter);
PangoApi void             pango_layout_iter_free (PangoLayoutIter *iter);

PangoApi int              pango_layout_iter_get_index  (PangoLayoutIter *iter);
PangoApi PangoLayoutRun  *pango_layout_iter_get_run    (PangoLayoutIter *iter);
PangoApi PangoLayoutRun  *pango_layout_iter_get_run_readonly   (PangoLayoutIter *iter);
PangoApi PangoLayoutLine *pango_layout_iter_get_line   (PangoLayoutIter *iter);
PangoApi PangoLayoutLine *pango_layout_iter_get_line_readonly  (PangoLayoutIter *iter);
PangoApi gboolean         pango_layout_iter_at_last_line (PangoLayoutIter *iter);
PangoApi PangoLayout     *pango_layout_iter_get_layout (PangoLayoutIter *iter);

PangoApi gboolean pango_layout_iter_next_char    (PangoLayoutIter *iter);
PangoApi gboolean pango_layout_iter_next_cluster (PangoLayoutIter *iter);
PangoApi gboolean pango_layout_iter_next_run     (PangoLayoutIter *iter);
PangoApi gboolean pango_layout_iter_next_line    (PangoLayoutIter *iter);

PangoApi void pango_layout_iter_get_char_extents    (PangoLayoutIter *iter,
					    PangoRectangle  *logical_rect);
PangoApi void pango_layout_iter_get_cluster_extents (PangoLayoutIter *iter,
					    PangoRectangle  *ink_rect,
					    PangoRectangle  *logical_rect);
PangoApi void pango_layout_iter_get_run_extents     (PangoLayoutIter *iter,
					    PangoRectangle  *ink_rect,
					    PangoRectangle  *logical_rect);
PangoApi void pango_layout_iter_get_line_extents    (PangoLayoutIter *iter,
					    PangoRectangle  *ink_rect,
					    PangoRectangle  *logical_rect);
/* All the yranges meet, unlike the logical_rect's (i.e. the yranges
 * assign between-line spacing to the nearest line)
 */
PangoApi void pango_layout_iter_get_line_yrange     (PangoLayoutIter *iter,
					    int             *y0_,
					    int             *y1_);
PangoApi void pango_layout_iter_get_layout_extents  (PangoLayoutIter *iter,
					    PangoRectangle  *ink_rect,
					    PangoRectangle  *logical_rect);
PangoApi int  pango_layout_iter_get_baseline        (PangoLayoutIter *iter);

G_END_DECLS

#endif /* __PANGO_LAYOUT_H__ */

