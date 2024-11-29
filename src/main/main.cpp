#include <sokol_app.h>
#include <entt/entt.hpp>

void init()
{

}

void frame() {}
void cleanup() {}

sapp_desc sokol_main(int, char **)
{
  sapp_desc desc = {};
  desc.init_cb = init;
  desc.frame_cb = frame;
  desc.cleanup_cb = cleanup;
  desc.window_title = "Test";
  desc.high_dpi = true;
  desc.win32_console_utf8 = true;
  desc.win32_console_create = true;
  desc.win32_console_attach = false;
  desc.icon.sokol_default = true;
  return desc;
}


