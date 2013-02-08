#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include <iostream>

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>

#include <lua.hpp>

#include "color.h"
#include "vector2d.h"

class Application;

class RenderSystem
{
public:
  RenderSystem(Application *application);
  virtual ~RenderSystem();
  
  bool Initialize();
  void UnInitialize();
  
  void SetWindowTitle(const char* windowTitle);
  
  int GetDisplayWidth() const;
  int GetDisplayHeight() const;
  
  bool IsInViewport(const Vector2d& position) const;
  
  void DrawFilledRectangle(int x1, int y1, int x2, int y2, const Color& color);
  void DrawRectangle(int x1, int y1, int x2, int y2, const Color& color, float thickness);

  void DrawFilledRectangle(const Vector2d& a, const Vector2d& b, const Color& color);
  void DrawRectangle(const Vector2d& a, const Vector2d& b, const Color& color, float thickness);

  void DrawFilledCircle(int cx, int cy, int r, const Color& color);
  void DrawCircle(int cx, int cy, int r, const Color& color, float thickness);  
  
  void DrawFilledCircle(const Vector2d& position, int r, const Color& color);
  void DrawCircle(const Vector2d& position, int r, const Color& color, float thickness);  

  void DrawArc(const Vector2d& position, int r, float start, float delta, const Color& color, float thickness);
  
  void DrawLine(int x1, int y1, int x2, int y2, const Color& color, float thickness);
  void DrawLine(const Vector2d& a, const Vector2d& b, const Color& color, float thickness);
  
  void DrawText(int x, int y, const Color& color, const std::string& message);
  void DrawText(const Vector2d& position, const Color& color, const std::string& message);
  
  void DrawTextCentre(int x, int y, const Color& color, const std::string& message);
  void DrawTextCentre(const Vector2d& position, const Color& color, const std::string& message);
  
  ALLEGRO_DISPLAY* GetDisplay() const;
  
  static void RegisterWithLua(lua_State* L);
  
private:
  Application* _application;
  ALLEGRO_DISPLAY* _display;
  ALLEGRO_FONT* _font;
  int _displayWidth;
  int _displayHeight;
  
  ALLEGRO_COLOR ColorToAllegroColor(const Color& color);
  
};

#endif // RENDERSYSTEM_H
