/* Manually edited */

#include "module-defs.h"

void _pango_basic_ft2_script_engine_list (PangoEngineInfo **engines, gint *n_engines);
void _pango_basic_ft2_script_engine_init (GTypeModule *module);
void _pango_basic_ft2_script_engine_exit (void);
PangoEngine *_pango_basic_ft2_script_engine_create (const char *id);

#ifdef __GCCE__
const 
#endif
PangoIncludedModule _pango_included_fc_modules[] = {
 { 
   _pango_basic_ft2_script_engine_list,  
   _pango_basic_ft2_script_engine_init, 
   _pango_basic_ft2_script_engine_exit,  
   _pango_basic_ft2_script_engine_create 
 },
 { NULL, NULL, NULL, NULL },
};
