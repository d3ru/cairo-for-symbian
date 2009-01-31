#ifndef PANGO_COMPILER_PRIVATE_H
#define PANGO_COMPILER_PRIVATE_H

#ifdef __SYMBIAN32__
#include <e32def.h>

# ifdef PANGO_BUILD_LIBRARY
#   define PangoApi EXPORT_C
# else
#   define PangoApi IMPORT_C
# endif

#endif

#endif