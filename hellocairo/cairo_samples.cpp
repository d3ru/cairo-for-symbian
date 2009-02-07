/*
 * Copyright © 2009 iwanj@users.sourceforge.net
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that copyright
 * notice and this permission notice appear in supporting documentation, and
 * that the name of the copyright holders not be used in advertising or
 * publicity pertaining to distribution of the software without specific,
 * written prior permission.  The copyright holders make no representations
 * about the suitability of this software for any purpose.  It is provided "as
 * is" without express or implied warranty.
 *
 * THE COPYRIGHT HOLDERS DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
 * EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
 * DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE
 * OF THIS SOFTWARE.
 */

#include <e32std.h>
#include <math.h>
#include "cairo_samples.h"
#include <cairo-ft.h>
#include <fontconfig.h>
#ifdef HAS_PANGO
#include <pangocairo.h>
#endif
#include <glib.h>

static const struct {
	double r,g,b;
} rgb[9] = {
	{0,0,0},
	{0,0,1},
	{0,1,0},
	{0,1,1},
	{1,0,0},
	{1,0,1},
	{1,1,0},
	{0.5,0.5,0.5},
	{0.5,0,0}
};

static void draw_text_fc(cairo_t* cr)
	{
    FcPattern *pattern = FcPatternCreate ();
    FcPatternAddString (pattern, FC_FAMILY, (const FcChar8 *)"Sans");
    FcPatternAddInteger (pattern, FC_WEIGHT, FC_WEIGHT_NORMAL);
    FcConfigSubstitute (NULL, pattern, FcMatchPattern);
    FcDefaultSubstitute (pattern);
    
    FcResult result;
    FcPattern *match = FcFontMatch (NULL, pattern, &result);
    if (match)
    	{
		cairo_font_face_t* face = cairo_ft_font_face_create_for_pattern (match);
		cairo_set_font_face (cr, face);
		cairo_set_font_size (cr, 40.0);
	
		/* draw rotated text */
		for (int i=0; i<10; ++i)
			{
			cairo_save(cr);
			cairo_rotate(cr, i *10.0 * M_PI /180.0);
			cairo_move_to (cr, 20.0, 20.0);
			cairo_set_source_rgba (cr, rgb[i].r, rgb[i].g, rgb[i].b, 0.75);
			cairo_show_text (cr, "Cairo Symbian OS");
			cairo_restore(cr);
			}
	
		cairo_font_face_destroy (face);
    	}
	
	FcPatternDestroy(match);
	FcPatternDestroy(pattern);
	}

#ifdef HAS_PANGO
static void draw_text_pango(cairo_t* cr)
	{
	PangoFontDescription* desc = pango_font_description_new();
	pango_font_description_set_family(desc, "Sans");
	pango_font_description_set_weight(desc, PANGO_WEIGHT_NORMAL);
	pango_font_description_set_absolute_size(desc, 40 * PANGO_SCALE);

	PangoLayout *layout = pango_cairo_create_layout(cr);
	pango_layout_set_font_description(layout, desc);
	pango_layout_set_text(layout, "Pango Symbian OS", -1);

	/* draw rotated text */
	for (int i=0; i<10; ++i)
		{
		cairo_save(cr);
		cairo_rotate(cr, i *10.0 * M_PI /180.0);
		cairo_move_to(cr, 20.0, 20.0);
		cairo_set_source_rgba (cr, rgb[i].r, rgb[i].g, rgb[i].b, 0.75);
		pango_cairo_update_layout(cr, layout);
		pango_cairo_show_layout_line(cr, pango_layout_get_line (layout, 0));		
		cairo_restore(cr);
		}
	
	g_object_unref(layout);
	pango_font_description_free(desc);
	}
#endif

/**
* The following drawing samples code were copied from Cairo code snippets found at  
* http://www.cairographics.org/samples/
*/

static void draw_arc(cairo_t* cr)
	{
	double xc = 128.0;
	double yc = 128.0;
	double radius = 100.0;
	double angle1 = 45.0  * (M_PI/180.0);  /* angles are specified */
	double angle2 = 180.0 * (M_PI/180.0);  /* in radians           */

	cairo_set_line_width (cr, 10.0);
	cairo_arc (cr, xc, yc, radius, angle1, angle2);
	cairo_stroke (cr);

	/* draw helping lines */
	cairo_set_source_rgba (cr, 1, 0.2, 0.2, 0.6);
	cairo_set_line_width (cr, 6.0);

	cairo_arc (cr, xc, yc, 10.0, 0, 2*M_PI);
	cairo_fill (cr);

	cairo_arc (cr, xc, yc, radius, angle1, angle1);
	cairo_line_to (cr, xc, yc);
	cairo_arc (cr, xc, yc, radius, angle2, angle2);
	cairo_line_to (cr, xc, yc);
	cairo_stroke (cr);
	}

static void draw_arc_negative(cairo_t* cr)
	{
	double xc = 128.0;
	double yc = 128.0;
	double radius = 100.0;
	double angle1 = 45.0  * (M_PI/180.0);  /* angles are specified */
	double angle2 = 180.0 * (M_PI/180.0);  /* in radians           */

	cairo_set_line_width (cr, 10.0);
	cairo_arc_negative (cr, xc, yc, radius, angle1, angle2);
	cairo_stroke (cr);

	/* draw helping lines */
	cairo_set_source_rgba (cr, 1, 0.2, 0.2, 0.6);
	cairo_set_line_width (cr, 6.0);

	cairo_arc (cr, xc, yc, 10.0, 0, 2*M_PI);
	cairo_fill (cr);

	cairo_arc (cr, xc, yc, radius, angle1, angle1);
	cairo_line_to (cr, xc, yc);
	cairo_arc (cr, xc, yc, radius, angle2, angle2);
	cairo_line_to (cr, xc, yc);
	cairo_stroke (cr);
	}

static void draw_clip(cairo_t* cr)
	{
	cairo_arc (cr, 128.0, 128.0, 76.8, 0, 2 * M_PI);
	cairo_clip (cr);

	cairo_new_path (cr);  /* current path is not
	                         consumed by cairo_clip() */
	cairo_rectangle (cr, 0, 0, 256, 256);
	cairo_fill (cr);
	cairo_set_source_rgb (cr, 0, 1, 0);
	cairo_move_to (cr, 0, 0);
	cairo_line_to (cr, 256, 256);
	cairo_move_to (cr, 256, 0);
	cairo_line_to (cr, 0, 256);
	cairo_set_line_width (cr, 10.0);
	cairo_stroke (cr);
	}

static void draw_clip_image(cairo_t* cr)
	{
	int              w, h;
	cairo_surface_t *image;

	cairo_arc (cr, 128.0, 128.0, 76.8, 0, 2*M_PI);
	cairo_clip (cr);
	cairo_new_path (cr); /* path not consumed by clip()*/

	image = cairo_image_surface_create_from_png ("c:\\data\\romedalen.png");
	w = cairo_image_surface_get_width (image);
	h = cairo_image_surface_get_height (image);

	cairo_scale (cr, 256.0/w, 256.0/h);

	cairo_set_source_surface (cr, image, 0, 0);
	cairo_paint (cr);

	cairo_surface_destroy (image);
	}

static void draw_curve_rectangle(cairo_t* cr)
	{
	/* a custom shape that could be wrapped in a function */
	double x0      = 25.6,   /* parameters like cairo_rectangle */
	       y0      = 25.6,
	       rect_width  = 204.8,
	       rect_height = 204.8,
	       radius = 102.4;   /* and an approximate curvature radius */

	double x1,y1;

	x1=x0+rect_width;
	y1=y0+rect_height;
	if (!rect_width || !rect_height)
	    return;
	if (rect_width/2<radius) {
	    if (rect_height/2<radius) {
	        cairo_move_to  (cr, x0, (y0 + y1)/2);
	        cairo_curve_to (cr, x0 ,y0, x0, y0, (x0 + x1)/2, y0);
	        cairo_curve_to (cr, x1, y0, x1, y0, x1, (y0 + y1)/2);
	        cairo_curve_to (cr, x1, y1, x1, y1, (x1 + x0)/2, y1);
	        cairo_curve_to (cr, x0, y1, x0, y1, x0, (y0 + y1)/2);
	    } else {
	        cairo_move_to  (cr, x0, y0 + radius);
	        cairo_curve_to (cr, x0 ,y0, x0, y0, (x0 + x1)/2, y0);
	        cairo_curve_to (cr, x1, y0, x1, y0, x1, y0 + radius);
	        cairo_line_to (cr, x1 , y1 - radius);
	        cairo_curve_to (cr, x1, y1, x1, y1, (x1 + x0)/2, y1);
	        cairo_curve_to (cr, x0, y1, x0, y1, x0, y1- radius);
	    }
	} else {
	    if (rect_height/2<radius) {
	        cairo_move_to  (cr, x0, (y0 + y1)/2);
	        cairo_curve_to (cr, x0 , y0, x0 , y0, x0 + radius, y0);
	        cairo_line_to (cr, x1 - radius, y0);
	        cairo_curve_to (cr, x1, y0, x1, y0, x1, (y0 + y1)/2);
	        cairo_curve_to (cr, x1, y1, x1, y1, x1 - radius, y1);
	        cairo_line_to (cr, x0 + radius, y1);
	        cairo_curve_to (cr, x0, y1, x0, y1, x0, (y0 + y1)/2);
	    } else {
	        cairo_move_to  (cr, x0, y0 + radius);
	        cairo_curve_to (cr, x0 , y0, x0 , y0, x0 + radius, y0);
	        cairo_line_to (cr, x1 - radius, y0);
	        cairo_curve_to (cr, x1, y0, x1, y0, x1, y0 + radius);
	        cairo_line_to (cr, x1 , y1 - radius);
	        cairo_curve_to (cr, x1, y1, x1, y1, x1 - radius, y1);
	        cairo_line_to (cr, x0 + radius, y1);
	        cairo_curve_to (cr, x0, y1, x0, y1, x0, y1- radius);
	    }
	}
	cairo_close_path (cr);

	cairo_set_source_rgb (cr, 0.5, 0.5, 1);
	cairo_fill_preserve (cr);
	cairo_set_source_rgba (cr, 0.5, 0, 0, 0.5);
	cairo_set_line_width (cr, 10.0);
	cairo_stroke (cr);	
	}

static void draw_curve_to(cairo_t* cr)
	{
	double x=25.6,  y=128.0;
	double x1=102.4, y1=230.4,
		   x2=153.6, y2=25.6,
		   x3=230.4, y3=128.0;
		   
	cairo_move_to (cr, x, y);
	cairo_curve_to (cr, x1, y1, x2, y2, x3, y3);
	cairo_set_line_width (cr, 10.0);
	cairo_stroke (cr);
	cairo_set_source_rgba (cr, 1, 0.2, 0.2, 0.6);
	cairo_set_line_width (cr, 6.0);
	cairo_move_to (cr,x,y);   cairo_line_to (cr,x1,y1);
	cairo_move_to (cr,x2,y2); cairo_line_to (cr,x3,y3);
	cairo_stroke (cr);
	}

static void draw_dash(cairo_t* cr)
	{
	double dashes[] = {50.0,  /* ink */
	                   10.0,  /* skip */
	                   10.0,  /* ink */
	                   10.0   /* skip*/
	                  };
	int    ndash  = sizeof (dashes)/sizeof(dashes[0]);
	double offset = -50.0;

	cairo_set_dash (cr, dashes, ndash, offset);
	cairo_set_line_width (cr, 10.0);

	cairo_move_to (cr, 128.0, 25.6);
	cairo_line_to (cr, 230.4, 230.4);
	cairo_rel_line_to (cr, -102.4, 0.0);
	cairo_curve_to (cr, 51.2, 230.4, 51.2, 128.0, 128.0, 128.0);

	cairo_stroke (cr);	
	}

static void draw_fill_and_stroke2(cairo_t* cr)
	{
	cairo_move_to (cr, 128.0, 25.6);
	cairo_line_to (cr, 230.4, 230.4);
	cairo_rel_line_to (cr, -102.4, 0.0);
	cairo_curve_to (cr, 51.2, 230.4, 51.2, 128.0, 128.0, 128.0);
	cairo_close_path (cr);

	cairo_move_to (cr, 64.0, 25.6);
	cairo_rel_line_to (cr, 51.2, 51.2);
	cairo_rel_line_to (cr, -51.2, 51.2);
	cairo_rel_line_to (cr, -51.2, -51.2);
	cairo_close_path (cr);

	cairo_set_line_width (cr, 10.0);
	cairo_set_source_rgb (cr, 0, 0, 1);
	cairo_fill_preserve (cr);
	cairo_set_source_rgb (cr, 0, 0, 0);
	cairo_stroke (cr);
	}

static void draw_fill_style(cairo_t* cr)
	{
	cairo_set_line_width (cr, 6);

	cairo_rectangle (cr, 12, 12, 232, 70);
	cairo_new_sub_path (cr); cairo_arc (cr, 64, 64, 40, 0, 2*M_PI);
	cairo_new_sub_path (cr); cairo_arc_negative (cr, 192, 64, 40, 0, -2*M_PI);

	cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);
	cairo_set_source_rgb (cr, 0, 0.7, 0); cairo_fill_preserve (cr);
	cairo_set_source_rgb (cr, 0, 0, 0); cairo_stroke (cr);

	cairo_translate (cr, 0, 128);
	cairo_rectangle (cr, 12, 12, 232, 70);
	cairo_new_sub_path (cr); cairo_arc (cr, 64, 64, 40, 0, 2*M_PI);
	cairo_new_sub_path (cr); cairo_arc_negative (cr, 192, 64, 40, 0, -2*M_PI);

	cairo_set_fill_rule (cr, CAIRO_FILL_RULE_WINDING);
	cairo_set_source_rgb (cr, 0, 0, 0.9); cairo_fill_preserve (cr);
	cairo_set_source_rgb (cr, 0, 0, 0); cairo_stroke (cr);
	}

static void draw_gradient(cairo_t* cr)
	{
	cairo_pattern_t *pat;

	pat = cairo_pattern_create_linear (0.0, 0.0,  0.0, 256.0);
	cairo_pattern_add_color_stop_rgba (pat, 1, 0, 0, 0, 1);
	cairo_pattern_add_color_stop_rgba (pat, 0, 1, 1, 1, 1);
	cairo_rectangle (cr, 0, 0, 256, 256);
	cairo_set_source (cr, pat);
	cairo_fill (cr);
	cairo_pattern_destroy (pat);

	pat = cairo_pattern_create_radial (115.2, 102.4, 25.6,
	                               102.4,  102.4, 128.0);
	cairo_pattern_add_color_stop_rgba (pat, 0, 1, 1, 1, 1);
	cairo_pattern_add_color_stop_rgba (pat, 1, 0, 0, 0, 1);
	cairo_set_source (cr, pat);
	cairo_arc (cr, 128.0, 128.0, 76.8, 0, 2 * M_PI);
	cairo_fill (cr);
	cairo_pattern_destroy (pat);
	
	}

static void draw_image(cairo_t* cr)
	{
	int              w, h;
	cairo_surface_t *image;

	image = cairo_image_surface_create_from_png ("c:\\data\\romedalen.png");
	w = cairo_image_surface_get_width (image);
	h = cairo_image_surface_get_height (image);

	cairo_translate (cr, 128.0, 128.0);
	cairo_rotate (cr, 45* M_PI/180);
	cairo_scale  (cr, 256.0/w, 256.0/h);
	cairo_translate (cr, -0.5*w, -0.5*h);

	cairo_set_source_surface (cr, image, 0, 0);
	cairo_paint (cr);
	cairo_surface_destroy (image);
	
	}

static void draw_image_pattern(cairo_t* cr)
	{
	int              w, h;
	cairo_surface_t *image;
	cairo_pattern_t *pattern;
	cairo_matrix_t   matrix;

	image = cairo_image_surface_create_from_png ("c:\\data\\romedalen.png");
	w = cairo_image_surface_get_width (image);
	h = cairo_image_surface_get_height (image);

	pattern = cairo_pattern_create_for_surface (image);
	cairo_pattern_set_extend (pattern, CAIRO_EXTEND_REPEAT);

	cairo_translate (cr, 128.0, 128.0);
	cairo_rotate (cr, M_PI / 4);
	cairo_scale (cr, 1 / sqrt (2), 1 / sqrt (2));
	cairo_translate (cr, -128.0, -128.0);

	cairo_matrix_init_scale (&matrix, w/256.0 * 5.0, h/256.0 * 5.0);
	cairo_pattern_set_matrix (pattern, &matrix);

	cairo_set_source (cr, pattern);

	cairo_rectangle (cr, 0, 0, 256.0, 256.0);
	cairo_fill (cr);

	cairo_pattern_destroy (pattern);
	cairo_surface_destroy (image);
	
	}

static void draw_multi_segment_caps(cairo_t* cr)
	{
	cairo_move_to (cr, 50.0, 75.0);
	cairo_line_to (cr, 200.0, 75.0);

	cairo_move_to (cr, 50.0, 125.0);
	cairo_line_to (cr, 200.0, 125.0);

	cairo_move_to (cr, 50.0, 175.0);
	cairo_line_to (cr, 200.0, 175.0);

	cairo_set_line_width (cr, 30.0);
	cairo_set_line_cap (cr, CAIRO_LINE_CAP_ROUND);
	cairo_stroke (cr);
	}

static void draw_set_line_cap(cairo_t* cr)
	{
	cairo_set_line_width (cr, 30.0);
	cairo_set_line_cap  (cr, CAIRO_LINE_CAP_BUTT); /* default */
	cairo_move_to (cr, 64.0, 50.0); cairo_line_to (cr, 64.0, 200.0);
	cairo_stroke (cr);
	cairo_set_line_cap  (cr, CAIRO_LINE_CAP_ROUND);
	cairo_move_to (cr, 128.0, 50.0); cairo_line_to (cr, 128.0, 200.0);
	cairo_stroke (cr);
	cairo_set_line_cap  (cr, CAIRO_LINE_CAP_SQUARE);
	cairo_move_to (cr, 192.0, 50.0); cairo_line_to (cr, 192.0, 200.0);
	cairo_stroke (cr);

	/* draw helping lines */
	cairo_set_source_rgb (cr, 1, 0.2, 0.2);
	cairo_set_line_width (cr, 2.56);
	cairo_move_to (cr, 64.0, 50.0); cairo_line_to (cr, 64.0, 200.0);
	cairo_move_to (cr, 128.0, 50.0);  cairo_line_to (cr, 128.0, 200.0);
	cairo_move_to (cr, 192.0, 50.0); cairo_line_to (cr, 192.0, 200.0);
	cairo_stroke (cr);
	}

static void draw_set_line_join(cairo_t* cr)
	{
	cairo_set_line_width (cr, 40.96);
	cairo_move_to (cr, 76.8, 84.48);
	cairo_rel_line_to (cr, 51.2, -51.2);
	cairo_rel_line_to (cr, 51.2, 51.2);
	cairo_set_line_join (cr, CAIRO_LINE_JOIN_MITER); /* default */
	cairo_stroke (cr);

	cairo_move_to (cr, 76.8, 161.28);
	cairo_rel_line_to (cr, 51.2, -51.2);
	cairo_rel_line_to (cr, 51.2, 51.2);
	cairo_set_line_join (cr, CAIRO_LINE_JOIN_BEVEL);
	cairo_stroke (cr);

	cairo_move_to (cr, 76.8, 238.08);
	cairo_rel_line_to (cr, 51.2, -51.2);
	cairo_rel_line_to (cr, 51.2, 51.2);
	cairo_set_line_join (cr, CAIRO_LINE_JOIN_ROUND);
	cairo_stroke (cr);
	}

static void draw_text(cairo_t* cr)
	{
	cairo_select_font_face (cr, "Sans", CAIRO_FONT_SLANT_NORMAL,
	                               CAIRO_FONT_WEIGHT_BOLD);
	cairo_set_font_size (cr, 90.0);

	cairo_move_to (cr, 10.0, 135.0);
	cairo_show_text (cr, "Hello");

	cairo_move_to (cr, 70.0, 165.0);
#ifdef HAS_CAIRO_TEXT_PATH_FIX
	cairo_text_path (cr, "void");
	cairo_set_source_rgb (cr, 0.5, 0.5, 1);
	cairo_fill_preserve (cr);
	cairo_set_source_rgb (cr, 0, 0, 0);
	cairo_set_line_width (cr, 2.56);
	cairo_stroke (cr);
#else
	cairo_set_source_rgb (cr, 0.5, 0.5, 1);	
	cairo_show_text (cr, "void");
#endif

	/* draw helping lines */
	cairo_set_source_rgba (cr, 1, 0.2, 0.2, 0.6);
	cairo_arc (cr, 10.0, 135.0, 5.12, 0, 2*M_PI);
	cairo_close_path (cr);
	cairo_arc (cr, 70.0, 165.0, 5.12, 0, 2*M_PI);
	cairo_fill (cr);
	}

static void draw_text_align_center(cairo_t* cr)
	{
	cairo_text_extents_t extents;

	const char *utf8 = "cairo";
	double x,y;

	cairo_select_font_face (cr, "Sans",
	    CAIRO_FONT_SLANT_NORMAL,
	    CAIRO_FONT_WEIGHT_NORMAL);

	cairo_set_font_size (cr, 52.0);
	cairo_text_extents (cr, utf8, &extents);
	x = 128.0-(extents.width/2 + extents.x_bearing);
	y = 128.0-(extents.height/2 + extents.y_bearing);

	cairo_move_to (cr, x, y);
	cairo_show_text (cr, utf8);

	/* draw helping lines */
	cairo_set_source_rgba (cr, 1, 0.2, 0.2, 0.6);
	cairo_set_line_width (cr, 6.0);
	cairo_arc (cr, x, y, 10.0, 0, 2*M_PI);
	cairo_fill (cr);
	cairo_move_to (cr, 128.0, 0);
	cairo_rel_line_to (cr, 0, 256);
	cairo_move_to (cr, 0, 128.0);
	cairo_rel_line_to (cr, 256, 0);
	cairo_stroke (cr);
	}
	
static void draw_text_extents(cairo_t* cr)
	{
	cairo_text_extents_t extents;

	const char *utf8 = "cairo";
	double x,y;

	cairo_select_font_face (cr, "Sans",
	    CAIRO_FONT_SLANT_NORMAL,
	    CAIRO_FONT_WEIGHT_NORMAL);

	cairo_set_font_size (cr, 100.0);
	cairo_text_extents (cr, utf8, &extents);

	x=25.0;
	y=150.0;

	cairo_move_to (cr, x,y);
	cairo_show_text (cr, utf8);

	/* draw helping lines */
	cairo_set_source_rgba (cr, 1, 0.2, 0.2, 0.6);
	cairo_set_line_width (cr, 6.0);
	cairo_arc (cr, x, y, 10.0, 0, 2*M_PI);
	cairo_fill (cr);
	cairo_move_to (cr, x,y);
	cairo_rel_line_to (cr, 0, -extents.height);
	cairo_rel_line_to (cr, extents.width, 0);
	cairo_rel_line_to (cr, extents.x_bearing, -extents.y_bearing);
	cairo_stroke (cr);
	}
	
/**********************************************************************************************
*
*/
struct func_rec
	{
	draw_function fn;	TBufC<32> nm;
	};

#define FUNC_REC(f)	{f, _L(#f)}

/**
 * Some test are excluded from GCCE target for now as they're known to crash when running on target device
 */
static const func_rec drawing_samples[] = 
	{
	FUNC_REC(draw_text_fc),
#ifdef HAS_PANGO	
	FUNC_REC(draw_text_pango),
#endif	
	FUNC_REC(draw_arc),
	FUNC_REC(draw_arc_negative),
	FUNC_REC(draw_clip),
	FUNC_REC(draw_clip_image),
	FUNC_REC(draw_curve_rectangle),
	FUNC_REC(draw_curve_to),
	FUNC_REC(draw_dash),
	FUNC_REC(draw_fill_and_stroke2),
	FUNC_REC(draw_fill_style),
	FUNC_REC(draw_gradient),
	FUNC_REC(draw_image),
	FUNC_REC(draw_image_pattern),
	FUNC_REC(draw_multi_segment_caps),
	FUNC_REC(draw_set_line_cap),
	FUNC_REC(draw_set_line_join),
	FUNC_REC(draw_text),
	FUNC_REC(draw_text_align_center),
	FUNC_REC(draw_text_extents)
	};

int number_of_cairo_samples()
	{
	return sizeof drawing_samples / sizeof drawing_samples[0];
	}

draw_function cairo_samples(int idx)
	{
	return drawing_samples[idx].fn;
	}

const TDesC& cairo_samples_name(int idx)
	{
	return drawing_samples[idx].nm;
	}
