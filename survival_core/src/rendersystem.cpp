#include "rendersystem.h"

#include <luabind/luabind.hpp>

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include "application.h"
#include "common.h"

RenderSystem::RenderSystem(Application* application) :
  _application(application),
  _display(nullptr),
  _font(nullptr),
  _displayWidth(-1),
  _displayHeight(-1)
{
}

RenderSystem::~RenderSystem()
{
  UnInitialize();
}

bool RenderSystem::Initialize()
{
  al_init_image_addon();  
  al_init_primitives_addon(); 
  al_init_font_addon();
  al_init_ttf_addon();
  
  ALLEGRO_DISPLAY_MODE display_mode;
  if (!al_get_display_mode(al_get_num_display_modes() - 1, &display_mode)) 
  {
    std::cout << "failed to get display mode!" << std::endl;
    return false;
  }
  
  int display_flags = ALLEGRO_WINDOWED;
  //int display_flags = ALLEGRO_FULLSCREEN;

  al_set_new_display_flags(display_flags); 
  
  _display = al_create_display(1280, 800); 
  if(!_display) 
  {
    std::cout << "failed to create display!" << std::endl;
    return false;
  }
  
  _displayWidth = al_get_display_width(_display);
  _displayHeight = al_get_display_height(_display);
 
  _font = al_load_ttf_font("fonts/font.ttf", 16, 0);
  if (!_font)
  {
    std::cout << "failed to load 'fonts/font.ttf'" << std::endl;
    return false;
  }
  
  return true;
}

void RenderSystem::UnInitialize()
{
  if (_font)
  {
    al_destroy_font(_font);
    _font = nullptr;
  }
  
  if (_display) 
  {
    al_destroy_display(_display);
    _display = nullptr;
  }
}

void RenderSystem::SetWindowTitle(const char* windowTitle)
{
  al_set_window_title(_display, windowTitle);
}

int RenderSystem::GetDisplayWidth() const
{
  return _displayWidth;
}

int RenderSystem::GetDisplayHeight() const
{
  return _displayHeight;
}

bool RenderSystem::IsInViewport(const Vector2d& position) const
{
  return position.x >= 0 && position.x <= GetDisplayWidth() &&
    position.y >= 0 && position.y <= GetDisplayHeight();
}

void RenderSystem::DrawFilledRectangle(int x1, int y1, int x2, int y2, const Color& color)
{
  al_draw_filled_rectangle(x1, y1, x2, y2, ColorToAllegroColor(color));
}

void RenderSystem::DrawRectangle(int x1, int y1, int x2, int y2, const Color& color, float thickness)
{
  al_draw_rectangle(x1, y1, x2, y2, ColorToAllegroColor(color), thickness);
}

void RenderSystem::DrawFilledRectangle(const Vector2d& a, const Vector2d& b, const Color& color)
{
  DrawFilledRectangle(a.x, a.y, b.x, b.y, color);
}

void RenderSystem::DrawRectangle(const Vector2d& a, const Vector2d& b, const Color& color, float thickness)
{
  DrawRectangle(a.x, a.y, b.x, b.y, color, thickness);
}

void RenderSystem::DrawFilledCircle(int cx, int cy, int r, const Color& color)
{
  al_draw_filled_circle(cx, cy, r, ColorToAllegroColor(color));
}

void RenderSystem::DrawCircle(int cx, int cy, int r, const Color& color, float thickness)
{  
  al_draw_circle(cx, cy, r, ColorToAllegroColor(color), thickness);
}

void RenderSystem::DrawFilledCircle(const Vector2d& position, int r, const Color& color)
{
  DrawFilledCircle(position.x, position.y, r, color);
}

void RenderSystem::DrawCircle(const Vector2d& position, int r, const Color& color, float thickness)
{
  DrawCircle(position.x, position.y, r, color, thickness);
}

void RenderSystem::DrawArc(const Vector2d& position, int r, float start, float delta, const Color& color, float thickness)
{
  al_draw_arc(position.x, position.y, r, DegreesToRadians(start), DegreesToRadians(delta), ColorToAllegroColor(color), thickness);
}

void RenderSystem::DrawLine(int x1, int y1, int x2, int y2, const Color& color, float thickness)
{
  al_draw_line(x1, y1, x2, y2, ColorToAllegroColor(color), thickness);
}

void RenderSystem::DrawLine(const Vector2d& a, const Vector2d& b, const Color& color, float thickness)
{
  DrawLine(a.x, a.y, b.x, b.y, color, thickness);
}

void RenderSystem::DrawText(int x, int y, const Color& color, const std::string& message)
{
  al_draw_text(_font, ColorToAllegroColor(color), x, y, ALLEGRO_ALIGN_LEFT, message.c_str());
}

void RenderSystem::DrawText(const Vector2d& position, const Color& color, const std::string& message)
{
  DrawText(position.x, position.y, color, message);
}

void RenderSystem::DrawTextCentre(int x, int y, const Color& color, const std::string& message)
{
  al_draw_text(_font, ColorToAllegroColor(color), x, y, ALLEGRO_ALIGN_CENTRE, message.c_str());
}

void RenderSystem::DrawTextCentre(const Vector2d& position, const Color& color, const std::string& message)
{
  DrawTextCentre(position.x, position.y, color, message);
}

ALLEGRO_DISPLAY* RenderSystem::GetDisplay() const
{
  return _display;
}

ALLEGRO_COLOR RenderSystem::ColorToAllegroColor(const Color& color)
{
  return al_map_rgba(color.r, color.g, color.b, (255.0 * color.a));
} 

void RenderSystem::RegisterWithLua(lua_State* L)
{
  luabind::module(L, "survival")
  [
    luabind::class_<RenderSystem>("RenderSystem")
      .def("draw_circle", (void(RenderSystem::*)(int, int, int, const Color&, float)) &RenderSystem::DrawCircle)
      .def("draw_filled_circle", (void(RenderSystem::*)(int, int, int, const Color&)) &RenderSystem::DrawFilledCircle)
      
      .def("draw_circle", (void(RenderSystem::*)(const Vector2d&, int, const Color&, float)) &RenderSystem::DrawCircle)
      .def("draw_filled_circle", (void(RenderSystem::*)(const Vector2d&, int, const Color&)) &RenderSystem::DrawFilledCircle)
      
      .def("draw_arc", &RenderSystem::DrawArc)
      
      .def("draw_rectangle", (void(RenderSystem::*)(int, int, int, int, const Color&, float)) &RenderSystem::DrawRectangle)
      .def("draw_filled_rectangle", (void(RenderSystem::*)(int, int, int, int, const Color&)) &RenderSystem::DrawFilledRectangle)
      
      .def("draw_rectangle", (void(RenderSystem::*)(const Vector2d&, const Vector2d&, const Color&, float)) &RenderSystem::DrawRectangle)
      .def("draw_filled_rectangle", (void(RenderSystem::*)(const Vector2d&, const Vector2d&, const Color&)) &RenderSystem::DrawFilledRectangle)
   
      .def("draw_line", (void(RenderSystem::*)(int, int, int, int, const Color&, float)) &RenderSystem::DrawLine)
      .def("draw_line", (void(RenderSystem::*)(const Vector2d&, const Vector2d&, const Color&, float)) &RenderSystem::DrawLine)
      
      .def("draw_text", (void(RenderSystem::*)(int, int, const Color&, const std::string&)) &RenderSystem::DrawText)
      .def("draw_text", (void(RenderSystem::*)(const Vector2d&, const Color&, const std::string&)) &RenderSystem::DrawText)
    
      .def("draw_text_centre", (void(RenderSystem::*)(int, int, const Color&, const std::string&)) &RenderSystem::DrawTextCentre)
      .def("draw_text_centre", (void(RenderSystem::*)(const Vector2d&, const Color&, const std::string&)) &RenderSystem::DrawTextCentre)
    
      .def("is_in_viewport", &RenderSystem::IsInViewport)
      
      .property("display_width", &RenderSystem::GetDisplayWidth)
      .property("display_height", &RenderSystem::GetDisplayHeight)
  ];
}