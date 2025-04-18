#define SOKOL_IMPL
#ifdef _WIN32
#define SOKOL_D3D11
#else
#define SOKOL_GLCORE
#endif

#define SOKOL_DEBUG

#include <sokol_gfx.h>
#include <sokol_gp.h>
#include <sokol_app.h>
#include <sokol_glue.h>
