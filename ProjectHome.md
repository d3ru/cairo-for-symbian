# Overview #
[Cairo](http://www.cairographics.org) for Symbian OS is based on Cairo version 1.8.6 with new surface backend added to allow Cairo rendering to a Symbian OS window ([RWindow](http://developer.symbian.com/main/documentation/sdl/symbian94/sdk/doc_source/reference/reference-cpp/WSERV8.1/RWindowClass.html#_top%20title=)).

<img src='http://cairo-for-symbian.googlecode.com/files/cairo-rotated-text-a.png' align='right'>

<a href='http://www.freetype.org'>FreeType</a> font backend and <a href='http://www.fontconfig.org'>Fontconfig</a> support have also been integrated. <a href='http://www.pango.org'>Pango</a> with basic shaper engine (basic-fc module) has been ported too, so it is now possible to draw text using Pango instead of using Cairo "toy" text API.<br>
<br>
Libraries used in this project i.e. <a href='http://expat.sourceforge.net'>expat</a>, <a href='http://www.libpng.org/pub/png/libpng.html'>libpng</a>, <a href='http://www.cairographics.org/releases/'>pixman</a>, freetype, fontconfig and cairo are now compiled as DLLs for WINSCW, GCCE and ARMv5 build. Everything seems to work fine on both emulator and target device, but there are known isses (see <a href='#Issues.md'>this</a> for more details).<br>
<br>
Here are some more <a href='#Screenshots.md'>screenshots</a> of S60 Cairo demo application. Please read <a href='#Demo.md'>this</a> before installing pre-built hellocairo SIS installer.<br>
<br>
<h1>Code Examples</h1>
An example how Cairo can be used from a Symbian OS application would be as follow:<br>
<pre><code>// CMyAppView derives from CCoeControl<br>
//<br>
void CMyAppView::ConstrucL(const TRect&amp; aRect)<br>
    {<br>
    CreateWindowL();<br>
    SetRect(aRect);<br>
    ActivateL();<br>
<br>
    iSurface = cairo_symbian_surface_create(&amp;Window());<br>
    iContext = cairo_create(iSurface);<br>
    }<br>
</code></pre>

<pre><code>// implement CCoeControl::Draw method<br>
//<br>
void CMyAppView::Draw(const TRect&amp;) const<br>
    {<br>
    // start drawing using Cairo here<br>
    // please note that mixing Cairo and native rendering i.e. using CWindowGc API<br>
    // is not supported and will produce undefined result<br>
    ...<br>
    cairo_t* cr = Context(); // shortcut to iContext<br>
    cairo_set_source_rgb(cr, 1, 1, 1);<br>
    cairo_paint(cr);<br>
    ...<br>
    }<br>
</code></pre>

<pre><code>// cleanup<br>
//<br>
CMyAppView::~CMyAppView()<br>
    {<br>
    ...<br>
    cairo_destroy(iContext);<br>
    cairo_surface_destroy(iSurface);<br>
    }<br>
</code></pre>

It is also possible to use Cairo for rendering into a Symbian OS pixmap (<a href='http://developer.symbian.com/main/documentation/sdl/symbian94/sdk/doc_source/reference/reference-cpp/FBSERV/CFbsBitmapClass.html#_top%20title='>CFbsBitmap</a>), which allows mixing between native and Cairo rendering. To do that, the existing functionality of Cairo Image Surface can be used in the following way,<br>
<pre><code>    ...<br>
    CFbsBitmap b;<br>
    b.Create(size, displayMode);<br>
    b.LockHeap();<br>
<br>
    // cairo_format func is used to map TDisplayMode to cairo_format_t e.g.<br>
    // EColor16MU -&gt; CAIRO_FORMAT_RGB24<br>
    // EColor16MA -&gt; CAIRO_FORMAT_ARGB32<br>
    //<br>
    cairo_surface_t* surface = cairo_image_surface_create_for_data((unsigned char*)b.DataAddress(), <br>
			cairo_format(displayMode),<br>
			size.iWidth,<br>
			size.iHeight,<br>
			b.DataStride());<br>
    cairo_t* cr = cairo_create(surface);<br>
    draw(cr);<br>
    cairo_destroy(cr);<br>
    cairo_surface_destroy(surface);<br>
<br>
    b.UnlockHeap();<br>
    ...<br>
</code></pre>

Here's a complete code example that was used to produce rotated text shown above,<br>
<pre><code>    ...<br>
    /* create font using Fontconfig and Cairo FT backend */<br>
    FcInit();<br>
    FcPattern *pattern = FcPatternCreate ();<br>
    FcPatternAddString (pattern, FC_FAMILY, (const FcChar8 *)"Sans");<br>
    FcPatternAddInteger (pattern, FC_SLANT, FC_SLANT_ROMAN);<br>
    FcPatternAddInteger (pattern, FC_WEIGHT, FC_WEIGHT_NORMAL);<br>
<br>
    FcResult result;<br>
    FcPattern *match = FcFontMatch (NULL, pattern, &amp;result);<br>
    cairo_font_face_t* face = cairo_ft_font_face_create_for_pattern (match);<br>
    cairo_set_font_face (cr, face);<br>
    cairo_set_font_size (cr, 40.0);<br>
<br>
    /* draw rotated text */<br>
    for (int i=0; i&lt;10; ++i)<br>
        {<br>
	cairo_save(cr);<br>
	cairo_rotate(cr, i * 10.0 * M_PI /180.0);<br>
	cairo_move_to (cr, 20.0, 20.0);<br>
	cairo_set_source_rgba (cr, rgb[i].r, rgb[i].g, rgb[i].b, 0.75);<br>
	cairo_show_text (cr, "Cairo Symbian OS");<br>
	cairo_restore(cr);<br>
	}<br>
<br>
    cairo_font_face_destroy (face);<br>
	<br>
    FcPatternDestroy(match);<br>
    FcPatternDestroy(pattern);<br>
    FcFini();<br>
	<br>
    /* capture screen to a bitmap and then use Image Surface to write it to a PNG file */<br>
    CWsScreenDevice* screen = CCoeEnv::Static()-&gt;ScreenDevice();<br>
    const TSize sz = screen-&gt;SizeInPixels();<br>
    const TDisplayMode dm = EColor16MU;<br>
    CFbsBitmap b;<br>
    b.Create(sz, dm);<br>
    screen-&gt;CopyScreenToBitmap(&amp;b);<br>
	<br>
    b.LockHeap();<br>
    cairo_surface_t* s = cairo_image_surface_create_for_data((unsigned char*)b.DataAddress(),<br>
			CAIRO_FORMAT_RGB24,<br>
			sz.iWidth,<br>
			sz.iHeight,<br>
			b.DataStride());<br>
    cairo_surface_write_to_png(s, "c:\\data\\text.png");<br>
    cairo_surface_flush(s);<br>
    cairo_surface_destroy(s);<br>
	<br>
    b.UnlockHeap();<br>
    b.Reset();<br>
<br>
    ...<br>
</code></pre>

To achieve the same result using Pango, we can use the following code,<br>
<pre><code>    PangoFontDescription* desc = pango_font_description_new();<br>
    pango_font_description_set_family(desc, "Sans");<br>
    pango_font_description_set_weight(desc, PANGO_WEIGHT_NORMAL);<br>
    pango_font_description_set_absolute_size(desc, 40 * PANGO_SCALE);<br>
<br>
    PangoLayout *layout = pango_cairo_create_layout(cr);<br>
    pango_layout_set_font_description(layout, desc);<br>
    pango_layout_set_text(layout, "Cairo Symbian OS", -1);<br>
<br>
    for (int i=0; i&lt;10; ++i)<br>
	{<br>
	cairo_save(cr);<br>
	cairo_rotate(cr, i *10.0 * M_PI /180.0);<br>
	cairo_move_to(cr, 20.0, 20.0);<br>
	cairo_set_source_rgba (cr, rgb[i].r, rgb[i].g, rgb[i].b, 0.75);<br>
<br>
	pango_cairo_update_layout(cr, layout);<br>
	pango_cairo_show_layout_line(cr, pango_layout_get_line (layout, 0));<br>
<br>
	cairo_restore(cr);<br>
	}<br>
	<br>
    g_object_unref(layout);<br>
    pango_font_description_free(desc);<br>
</code></pre>

<h1>Build Instructions</h1>
The development is done using <a href='http://www.forum.nokia.com'>Nokia S60 3rd Edition SDK and Open C plugin</a>. The build system is currently done using Symbian OS specific way (bldmake, abld). The plan is to migrate them to use standard Makefile system in the future (either using MSYS or MozillaBuild)<br>
<br>
To build the project you need to install the following SDK and tools:<br>
<ol><li>S60 3rd Edition (I'm using Feature Pack 1)<br>
</li><li>Open C plugin for S60 3rd Edition (for libc, libpthread and libz)<br>
</li><li>Carbide.c++ (to get Nokia/MetroWerks compiler)<br>
</li><li>GCCE version 4.3.2 (to allow proper support for EPOCALLOWDLLDATA)</li></ol>

Pango and Cairo for Symbian OS has dependency on the following libraries, which are included in the .zip file available from Featured Downloads section:<br>
<ol><li><a href='http://www.zlib.org'>zlib</a> (using libz from Open C)<br>
</li><li>libpng version 1.2.3.4<br>
</li><li>pixman version 0.13.2<br>
</li><li>freetype version 2.3.7<br>
</li><li>fontconfig version 2.6.0<br>
</li><li>expat version 2.0.1<br>
</li><li>pango version 1.22.2</li></ol>

Steps to build and test the project on S60 emulator:<br>
<ol><li>Add <code>#include &lt;glib_global.h&gt;</code> to %EPOCROOT%\epoc32\include\stdapis\glib-2.0\glib.h<br>
</li><li>Extract the .zip file<br>
</li><li>Go to cairo-for-symbian\build directory<br>
</li><li>bldmake bldfiles && abld build winscw udeb<br>
</li><li>copy cairo-for-symbian\helloworld\data\romedalen.png to %EPOCROOT%\epoc32\winscw\c\data<br>
</li><li>copy fonts.conf and fonts.dtd from cairo-for-symbian\fontconfig-2.6.0\projects\symbian to %EPOCROOT%\epoc32\winscw\c\data\fontconfig<br>
</li><li>Launch emulator and run hellocairo application (located under Installed folder)</li></ol>

To build for GCCE target you need to replace GCCE that comes with SDK (version 3.4.3) with the latest version. Follow the instructions on this <a href='http://developer.symbian.com/wiki/display/KBSDN/Using+GCCE+4+with+Symbian+OS+SDKs'>wikipage</a> on how to do that. You may not have to follow all the instructions depending on your SDK environment. In my case, updating the perl scripts, d32locd.h and gcce.h is enough. The fix for missing integer division is included in this project. You may also need to modify e32def.h i.e. go to %EPOCROOT%\epoc32\include directory and modify e32def.h by replacing this line<br>
<pre><code>static const char* const KSuppressPlatSecDiagnostic = KSuppressPlatSecDiagnosticMagicValue;<br>
</code></pre>
with<br>
<pre><code>static const char* const KSuppressPlatSecDiagnostic = (const char*)1;<br>
</code></pre>

Once you've done that, build Cairo and create SIS installer for S60 phone with the following steps:<br>
<ol><li>Go to cairo-for-symbian\build directory<br>
</li><li>bldmake bldfiles && abld build gcce udeb<br>
</li><li>Go to cairo-for-symbian\hellocairo\sis directory<br>
</li><li>create_package.bat udeb</li></ol>

<h1>Demo</h1>
If your S60 phone is older than S60 3rd Edition Feature Pack 2, you will need to install Open C plugin into your phone first before you can run hellocairo application. Download and install Open C from <a href='http://www.forum.nokia.com/info/sw.nokia.com/id/91d89929-fb8c-4d66-bea0-227e42df9053/Open_C_SDK_Plug-In.html'>here</a>. The Open C SIS installer can be found under %S60_SDK_INSTALL_DIR%\nokia_plugin\openc\s60opencsis directory.<br>
<br>
<h1>Issues</h1>
Known issues:<br>
<ul><li><code>cairo_text_path</code> does not work and is replaced with <code>cairo_show_text</code> in demo application.<br>
</li><li>Pango does not yet compile on ARMv5 build. Apply Jani's RVCT patch to fix that.<br>
</li><li>GCCE release build somehow does not work on target device, the debug build does.<br>
</li><li>Exhaustive testing has not been done, so there may be some other undiscovered bugs.</li></ul>

<h1>Screenshots</h1>
Some screenshots of Cairo helloworld application running on S60 emulator (using the same code snippets found <a href='http://www.cairographics.org/samples'>here</a>):<br>
<hr />
<ul><li><b>arc</b>
<a href='http://cairo-for-symbian.googlecode.com/files/draw_arc.PNG'>http://cairo-for-symbian.googlecode.com/files/draw_arc.PNG</a>
<hr />
</li><li><b>arc negative</b>
<a href='http://cairo-for-symbian.googlecode.com/files/draw_arc_negative.PNG'>http://cairo-for-symbian.googlecode.com/files/draw_arc_negative.PNG</a>
<hr />
</li><li><b>clip</b>
<a href='http://cairo-for-symbian.googlecode.com/files/draw_clip.PNG'>http://cairo-for-symbian.googlecode.com/files/draw_clip.PNG</a>
<hr />
</li><li><b>clip image</b>
<a href='http://cairo-for-symbian.googlecode.com/files/draw_clip_image.PNG'>http://cairo-for-symbian.googlecode.com/files/draw_clip_image.PNG</a>
<hr />
</li><li><b>curve rectangle</b>
<a href='http://cairo-for-symbian.googlecode.com/files/draw_curve_rectangle.PNG'>http://cairo-for-symbian.googlecode.com/files/draw_curve_rectangle.PNG</a>
<hr />
</li><li><b>curve to</b>
<a href='http://cairo-for-symbian.googlecode.com/files/draw_curve_to.PNG'>http://cairo-for-symbian.googlecode.com/files/draw_curve_to.PNG</a>
<hr />
</li><li><b>dash</b>
<a href='http://cairo-for-symbian.googlecode.com/files/draw_dash.PNG'>http://cairo-for-symbian.googlecode.com/files/draw_dash.PNG</a>
<hr />
</li><li><b>fill and stroke</b>
<a href='http://cairo-for-symbian.googlecode.com/files/draw_fill_and_stroke2.PNG'>http://cairo-for-symbian.googlecode.com/files/draw_fill_and_stroke2.PNG</a>
<hr />
</li><li><b>fill style</b>
<a href='http://cairo-for-symbian.googlecode.com/files/draw_fill_style.PNG'>http://cairo-for-symbian.googlecode.com/files/draw_fill_style.PNG</a>
<hr />
</li><li><b>gradient</b>
<a href='http://cairo-for-symbian.googlecode.com/files/draw_gradient.PNG'>http://cairo-for-symbian.googlecode.com/files/draw_gradient.PNG</a>
<hr />
</li><li><b>image</b>
<a href='http://cairo-for-symbian.googlecode.com/files/draw_image.PNG'>http://cairo-for-symbian.googlecode.com/files/draw_image.PNG</a>
<hr />
</li><li><b>image pattern</b>
<a href='http://cairo-for-symbian.googlecode.com/files/draw_image_pattern.PNG'>http://cairo-for-symbian.googlecode.com/files/draw_image_pattern.PNG</a>
<hr />
</li><li><b>multi segment caps</b>
<a href='http://cairo-for-symbian.googlecode.com/files/draw_multi_segment_caps.PNG'>http://cairo-for-symbian.googlecode.com/files/draw_multi_segment_caps.PNG</a>
<hr />
</li><li><b>set line cap</b>
<a href='http://cairo-for-symbian.googlecode.com/files/draw_set_line_cap.PNG'>http://cairo-for-symbian.googlecode.com/files/draw_set_line_cap.PNG</a>
<hr />
</li><li><b>set line join</b>
<a href='http://cairo-for-symbian.googlecode.com/files/draw_set_line_join.PNG'>http://cairo-for-symbian.googlecode.com/files/draw_set_line_join.PNG</a>
<hr />
</li><li><b>text</b>
<a href='http://cairo-for-symbian.googlecode.com/files/draw_text.PNG'>http://cairo-for-symbian.googlecode.com/files/draw_text.PNG</a>
<hr />
</li><li><b>text align center</b>
<a href='http://cairo-for-symbian.googlecode.com/files/draw_text_align_center.PNG'>http://cairo-for-symbian.googlecode.com/files/draw_text_align_center.PNG</a>
<hr />
</li><li><b>text extents</b>
<a href='http://cairo-for-symbian.googlecode.com/files/draw_text_extents.PNG'>http://cairo-for-symbian.googlecode.com/files/draw_text_extents.PNG</a>
<hr /></li></ul>




