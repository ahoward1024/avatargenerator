/*
 *	Contact Avatar Generation
 *  Author: Alex Howard
 *  SDL Version: 2.0.4  
 *  Last updated: 5-14-2016
 *  
 */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <limits.h>

#include "imgui/imgui.h"
#include "imgui_impl_sdl_gl3.h"
#include "gl3w/GL/gl3w.h"
#include "imgui_colors.h"

#include "SDL2/SDL.h"
#include "SDL2/SDL_main.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL2_gfxPrimitives.h"
#include "SDL2/SDL2_sgfxPrimitives.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_colors.h"

#include "SDL_FontCache.h"

#include "savepng.c"

#define  XTRACE_SUPPLEMENT
#include "xtrace.h"

#define CONTACT_SIZE 16
#define FONTLIST_COUNT 513

static bool     G_running  = true;
static int      G_WindowW  = 1920;
static int      G_WindowH  = 1080;

static const char *ASCII = "!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
static const char *G_GL_ALL = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789`~!@#$%^&*()-_=+[]{}\\|;:'\",.<>/?";
static const char *G_GL_LUN = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
static const char *G_GL_LU = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
static const char *G_GL_U = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
static const char *G_GL_US = "ABCDEFGHIJKLMNOPQRSTUVWXYZ`~!@#$%^&*()-_=+[]{}\\|;:'\",.<>/?";
static const char *G_GL_UN = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
static const char *G_GL_L = "abcdefghijklmnopqrstuvwxyz";
static const char *G_GL_LS = "abcdefghijklmnopqrstuvwxyz`~!@#$%^&*()-_=+[]{}\\|;:'\",.<>/?";
static const char *G_GL_LN = "abcdefghijklmnopqrstuvwxyz0123456789";
static const char *G_GL_N = "0123456789";
static const char *G_GL_S = "`~!@#$%^&*()-_=+[]{}\\|;:'\",.<>/?";

static const uint32_t G_COLOR_0  = 0xFF1519C5;
static const uint32_t G_COLOR_1  = 0xFF1842CA;
static const uint32_t G_COLOR_2  = 0xFF206DEC;
static const uint32_t G_COLOR_3  = 0xFF2781FE;
static const uint32_t G_COLOR_4  = 0xFF28CA9A;
static const uint32_t G_COLOR_5  = 0xFF54B6C8;
static const uint32_t G_COLOR_6  = 0xFF389BC7;
static const uint32_t G_COLOR_7  = 0xFF2076CA;
static const uint32_t G_COLOR_8  = 0xFF42B086;
static const uint32_t G_COLOR_9  = 0xFF27A553;
static const uint32_t G_COLOR_10 = 0xFF1B9867;
static const uint32_t G_COLOR_11 = 0xFF4B8613;
static const uint32_t G_COLOR_12 = 0xFF66A921;
static const uint32_t G_COLOR_13 = 0xFF93B366;
static const uint32_t G_COLOR_14 = 0xFFA69718;
static const uint32_t G_COLOR_15 = 0xFFB9B370;
static const uint32_t G_COLOR_16 = 0xFFE3B73C;
static const uint32_t G_COLOR_17 = 0xFFCB9A1A;
static const uint32_t G_COLOR_18 = 0xFFE89319;
static const uint32_t G_COLOR_19 = 0xFFCF8917;
static const uint32_t G_COLOR_20 = 0xFFCA6682;
static const uint32_t G_COLOR_21 = 0xFFB24C75;
static const uint32_t G_COLOR_22 = 0xFFC96865;
static const uint32_t G_COLOR_23 = 0xFFB7571B;
static const uint32_t G_COLOR_24 = 0xFFCA69A9;
static const uint32_t G_COLOR_25 = 0xFFCA3B98;
static const uint32_t G_COLOR_26 = 0xFFB6459D;
static const uint32_t G_COLOR_27 = 0xFF4B46FD;
static const uint32_t G_COLOR_28 = 0xFF9794A3;
static const uint32_t G_COLOR_29 = 0xFF817CA3;
static const uint32_t G_COLOR_30 = 0xFF7E5EC3;
static const uint32_t G_COLOR_31 = 0xFF6425E6;
static const uint32_t G_COLOR_32 = 0xFF424242;
static const uint32_t G_COLOR_33 = 0xFF333333;

static struct gcolors
{
  uint32_t colors[34];
  const int size = 34;
} Google_Colors;

struct GlyphMetric
{
  int minx;
  int miny;
  int maxx;
  int maxy;
  int advance;
};

struct FontMetric
{
  int height;
  int ascent;
  int descent;
  int lineskip;
  int midpoint;
  int full_midpoint;
  int minx = INT_MAX;
  int maxx = INT_MIN;
  int miny = INT_MAX;
  int maxy = INT_MIN;
  int thinnest = INT_MAX;
  int widest = INT_MIN;
  char widest_c;
  char thinnest_c;
  GlyphMetric *gml;
  const char *glyphs;
  int num_glyphs;
};

struct Text
{
  TTF_Font *font;
  FontMetric fm;
  SDL_Surface *surface;
  SDL_Rect rect;
};

void InitGoogleColors()
{
  Google_Colors.colors[0]  = G_COLOR_0;
  Google_Colors.colors[1]  = G_COLOR_1;
  Google_Colors.colors[2]  = G_COLOR_2;
  Google_Colors.colors[3]  = G_COLOR_3;
  Google_Colors.colors[4]  = G_COLOR_4;
  Google_Colors.colors[5]  = G_COLOR_5;
  Google_Colors.colors[6]  = G_COLOR_6;
  Google_Colors.colors[7]  = G_COLOR_7;
  Google_Colors.colors[8]  = G_COLOR_8;
  Google_Colors.colors[9]  = G_COLOR_9;
  Google_Colors.colors[10] = G_COLOR_10;
  Google_Colors.colors[11] = G_COLOR_11;
  Google_Colors.colors[12] = G_COLOR_12;
  Google_Colors.colors[13] = G_COLOR_13;
  Google_Colors.colors[14] = G_COLOR_14;
  Google_Colors.colors[15] = G_COLOR_15;
  Google_Colors.colors[16] = G_COLOR_16;
  Google_Colors.colors[17] = G_COLOR_17;
  Google_Colors.colors[18] = G_COLOR_18;
  Google_Colors.colors[19] = G_COLOR_19;
  Google_Colors.colors[20] = G_COLOR_20;
  Google_Colors.colors[21] = G_COLOR_21;
  Google_Colors.colors[22] = G_COLOR_22;
  Google_Colors.colors[23] = G_COLOR_23;
  Google_Colors.colors[24] = G_COLOR_24;
  Google_Colors.colors[25] = G_COLOR_25;
  Google_Colors.colors[26] = G_COLOR_26;
  Google_Colors.colors[27] = G_COLOR_27;
  Google_Colors.colors[28] = G_COLOR_28;
  Google_Colors.colors[29] = G_COLOR_29;
  Google_Colors.colors[30] = G_COLOR_30;
  Google_Colors.colors[31] = G_COLOR_31;
  Google_Colors.colors[32] = G_COLOR_32;
  Google_Colors.colors[33] = G_COLOR_33;

}

GlyphMetric GetGlyphMetric(TTF_Font *font, char c)
{
  GlyphMetric gm;
  TTF_GlyphMetrics(font, c, &gm.minx, &gm.maxx, &gm.miny, &gm.maxy, &gm.advance);
  return gm;
}

void UpdateGlyphMetric(GlyphMetric *gm, TTF_Font *font, char c)
{
  TTF_GlyphMetrics(font, c, &gm->minx, &gm->maxx, &gm->miny, &gm->maxy, &gm->advance);
}

FontMetric BuildFontMetric(TTF_Font *font, const char *glyphs)
{
  FontMetric fm;
  fm.height = TTF_FontHeight(font);
  fm.ascent = TTF_FontAscent(font);
  fm.descent = TTF_FontDescent(font);
  fm.lineskip = TTF_FontLineSkip(font);
  fm.glyphs = glyphs;
  fm.num_glyphs = strlen(fm.glyphs);
  fm.gml = (GlyphMetric *)calloc(fm.num_glyphs, sizeof(GlyphMetric));
  if(!fm.gml) return fm;
  for(int i = 0; i < fm.num_glyphs; ++i)
  {
    fm.gml[i] = GetGlyphMetric(font, fm.glyphs[i]);
    if(fm.gml[i].minx < fm.minx) fm.minx = fm.gml[i].minx;
    if(fm.gml[i].maxx > fm.maxx) fm.maxx = fm.gml[i].maxx;
    if(fm.gml[i].miny < fm.miny) fm.miny = fm.gml[i].miny;
    if(fm.gml[i].maxy > fm.maxy) fm.maxy = fm.gml[i].maxy;
    if(fm.gml[i].advance > fm.widest) { fm.widest = fm.gml[i].advance; fm.widest_c = i; }
    if(fm.gml[i].advance < fm.thinnest) { fm.thinnest = fm.gml[i].advance; fm.thinnest_c = i; }
  }
  GlyphMetric gm = GetGlyphMetric(font, 'a');
  int bot = fm.ascent;
  int top = fm.ascent - gm.maxy;
  fm.midpoint = (bot + top) / 2;
  int ftop = fm.ascent - fm.maxy;
  int fbot = fm.ascent - fm.miny;
  fm.full_midpoint = (fbot + ftop) / 2;
  return fm;
}

void UpdateFontMetric(FontMetric *fm, TTF_Font *font, const char *glyphs)
{
  fm->height = TTF_FontHeight(font);
  fm->ascent = TTF_FontAscent(font);
  fm->descent = TTF_FontDescent(font);
  fm->lineskip = TTF_FontLineSkip(font);
  fm->glyphs = glyphs;
  fm->num_glyphs = strlen(fm->glyphs);
  for(int i = 0; i < fm->num_glyphs; ++i)
  {
    UpdateGlyphMetric(&fm->gml[i], font, fm->glyphs[i]);
    if(fm->gml[i].minx < fm->minx) fm->minx = fm->gml[i].minx;
    if(fm->gml[i].maxx > fm->maxx) fm->maxx = fm->gml[i].maxx;
    if(fm->gml[i].miny < fm->miny) fm->miny = fm->gml[i].miny;
    if(fm->gml[i].maxy > fm->maxy) fm->maxy = fm->gml[i].maxy;
    if(fm->gml[i].advance > fm->widest) { fm->widest = fm->gml[i].advance; fm->widest_c = i; }
    if(fm->gml[i].advance < fm->thinnest) { fm->thinnest = fm->gml[i].advance; fm->thinnest_c = i; }
  }
  GlyphMetric gm = GetGlyphMetric(font, 'a');
  int bot = fm->ascent;
  int top = fm->ascent - gm.maxy;
  fm->midpoint = (bot + top) / 2;
  int ftop = fm->ascent - fm->maxy;
  int fbot = fm->ascent - fm->miny;
  fm->full_midpoint = (fbot + ftop) / 2;
}

FontMetric BuildFontMetric(TTF_Font *font)
{
  FontMetric fm = BuildFontMetric(font, G_GL_L);
  return fm;
}

void PrintGlyphMetric(const char c, const GlyphMetric gm)
{
  printf("%c: minx: %3d\tmaxx: %3d\tminy: %3d\tmaxy: %3d\tadvance: %3d\n", c, 
         gm.minx, gm.maxx, gm.miny, gm.maxy, gm.advance);
}

void PrintFontMetric(const FontMetric *fm, const char *font_file)
{
  printf("Font Props: %s\n", font_file);
  printf("Height: %d\nAscent: %d\nDescent: %d\nLine Skip: %d\n", 
         fm->height, fm->ascent, fm->descent, fm->lineskip);
  printf("Max X: %d | Min X: %d\n", fm->maxx, fm->minx);
  printf("Max Y: %d | Min Y: %d\n", fm->maxy, fm->miny);
  printf("Widest: %d, %c\n", fm->widest, fm->widest_c + 33);
  printf("Thinnest: %d, %c\n", fm->thinnest, fm->thinnest_c + 33);
  printf("Glyphs:\n");
  for(int i = 0; i < fm->num_glyphs; ++i)
  {
    PrintGlyphMetric(fm->glyphs[i], fm->gml[i]);
  }
  printf("\n");
}

void UpdateTestText(Text *text, TTF_Font *font, SDL_Color color, const char *glyphs)
{
  int x, y, w, h;
  TTF_SizeUTF8(font, glyphs, &w, &h);
  text->rect = {0, 0, w, h};
  SDL_FreeSurface(text->surface);
  text->surface = TTF_RenderUTF8_Blended(font, glyphs, color);
}

Text CreateText(TTF_Font *font, SDL_Color color, const char *glyphs)
{
  Text text = {};
  text.font = font;
  text.fm = BuildFontMetric(font, glyphs);
  int x, y, w, h;
  TTF_SizeUTF8(font, glyphs, &w, &h);
  text.rect = {0, 0, w, h};
  text.surface = TTF_RenderUTF8_Blended(font, glyphs, color);
  if(!text.surface)
  {
    printf("Could not create text\n");
  }
  return text;
}

void UpdateText(Text *text, TTF_Font *font, SDL_Color color, const char *glyphs)
{
  text->font = font;
  UpdateFontMetric(&text->fm, font, glyphs);
  UpdateTestText(text, font, color, glyphs);
}

TTF_Font **CreateFontList(const char *fontfile)
{
  TTF_Font **fontlist = (TTF_Font **)malloc(FONTLIST_COUNT * sizeof(TTF_Font *));
  if(!fontlist) return NULL;
  for(int i = 0; i < FONTLIST_COUNT; ++i)
    fontlist[i] = TTF_OpenFont(fontfile, i);

  return fontlist;
}

void FreeFontList(TTF_Font **fontlist)
{
  for(int i = 0; i < FONTLIST_COUNT; ++i)
    TTF_CloseFont(fontlist[i]);
  free(fontlist);
}

void UseGoogleColors(char* gcolorbutton_name, const char* gc_str, const char *type, ImVec4* color)
{
  for(int i = 0; i < Google_Colors.size; ++i)
  {
    strcpy(gcolorbutton_name, "");
    char num[4];
    snprintf(num, 4, "%d", i);
    snprintf(gcolorbutton_name, 20, "%s%s%s", gc_str, num, type);
    ImGui::PushStyleColor(ImGuiCol_Button, Google_Colors.colors[i]);
    if(ImGui::Button(gcolorbutton_name)) *color = ImGui::ColorConvertU32ToFloat4(Google_Colors.colors[i]);
    if(i != Google_Colors.size - 1) ImGui::SameLine();
    ImGui::PopStyleColor(1);
  }
}

void HandleEvents(SDL_Window *window)
{
  SDL_GetWindowSize(window, &G_WindowW, &G_WindowH);
  SDL_Event event;
  while(SDL_PollEvent(&event))
  {
    ImGui_ImplSdlGL3_ProcessEvent(&event);
    switch(event.type)
    {
      case SDL_QUIT: G_running = false; break;
      case SDL_DROPFILE:
      {

      } break;
      case SDL_KEYDOWN:
      {
        switch(event.key.keysym.sym)
        {
          case SDLK_ESCAPE: G_running = false; break;
        }
      } break;
    }
  }
  ImGui_ImplSdlGL3_NewFrame(window);
}

#include "drawing.h"

int main(int argc, char **argv)
{
  srand(time(NULL));
	printf("Hello world!\n\n");

	if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		printf("Could not init SDL.\n%s\nExiting.\n", SDL_GetError());
    return -1;
	}
  if(TTF_Init() == -1)
  {
    printf("Could not init SDL_TTF.\n%s\nExiting.\n", SDL_GetError());
    return -1;
  }
  if(IMG_Init(IMG_INIT_PNG) == -1)
  {
    printf("Could not init SDL_Image\n%s\nExiting.\n", SDL_GetError());
    return -1;
  }

  InitGoogleColors();

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 4);
  SDL_DisplayMode current;
  SDL_GetCurrentDisplayMode(0, &current);
  SDL_Window *window = SDL_CreateWindow("Avatar Generation",
                                        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
                                        G_WindowW, G_WindowH,
                                        SDL_WINDOW_RESIZABLE|SDL_WINDOW_OPENGL);
  SDL_GLContext glcontext = SDL_GL_CreateContext(window);
  gl3wInit();
  ImGui_ImplSdlGL3_Init(window);

  glViewport(0, 0, (int)ImGui::GetIO().DisplaySize.x, (int)ImGui::GetIO().DisplaySize.y);
  glClearColor(0.0f, 0.0f, 0.0f, 1.00f);
  glClear(GL_COLOR_BUFFER_BIT);
  SDL_GL_SwapWindow(window);

  int dim = 512;

  TTF_Font **fontlist_roboto = CreateFontList("fonts/Roboto-Light.ttf");
  char *contact_name = (char *)calloc(CONTACT_SIZE, sizeof(char));
  strcpy(contact_name, "contact");
  Text text = CreateText(fontlist_roboto[256], SDLC_white, contact_name);
  GLuint contact_image = 0;
  glGenTextures(1, &contact_image);
  glBindTexture(GL_TEXTURE_2D, contact_image);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, text.surface->w, text.surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, text.surface->pixels);

  TTF_Font **fontlist_circlethings = CreateFontList("fonts/circlethings.ttf");
  SDL_Surface *circle = TTF_RenderUTF8_Blended(fontlist_circlethings[dim-3], "a", SDLC_red);
  SDL_SetSurfaceBlendMode(circle, SDL_BLENDMODE_BLEND);
  GLuint circle_image = 1;
  glGenTextures(1, &circle_image);
  glBindTexture(GL_TEXTURE_2D, circle_image);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, circle->w, circle->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, circle->pixels);

  const ImU32 original_circle_color = 0xFF2C3133;
  ImVec4 circle_color = ImGui::ColorConvertU32ToFloat4(original_circle_color);
  const ImU32 original_text_color = 0xFFFFFFFF;
  ImVec4 text_color = ImGui::ColorConvertU32ToFloat4(original_text_color);

  ImGui::StyleColorsDark(&ImGui::GetStyle());
  ImGuiStyle& style = ImGui::GetStyle();

  SDL_Surface *output_surface = NULL;
  GLuint output_image = 2;
  glGenTextures(1, &circle_image);
  glBindTexture(GL_TEXTURE_2D, circle_image);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, circle->w, circle->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, circle->pixels);
  
  bool saved = false;
  float save_timer = 0.0f;
  bool error = false;
  float error_timer = 0.0f;

  ImGuiIO io = ImGui::GetIO();
  const char *gc_str = "    ##gcolor";
  char *gcolorbutton_name = (char *)calloc(strlen(gc_str) + 4 + 1, sizeof(char));

  uint64_t start = SDL_GetTicks();
	do
	{
		HandleEvents(window);
    
    ImGui::SetNextWindowSize(ImVec2(G_WindowW,G_WindowH));
    ImGui::SetNextWindowPos(ImVec2(0,0));
    static bool p_open;
    ImGui::Begin("##Window", &p_open, ImVec2(0,0), ImGuiWindowFlags_NoTitleBar, ImGuiWindowFlags_NoResize);
    {
      ImDrawList *draw_list = ImGui::GetWindowDrawList();
      static int size = 145;

      ImGui::SliderInt("Font Size", &size, 16, FONTLIST_COUNT - 1);

      static bool use_contact_name = true;
      ImGui::Checkbox("Use contact name for filename?", &use_contact_name);
      static char filename_save[CONTACT_SIZE];
      if(!use_contact_name) {
        ImGui::InputText("File name", filename_save, CONTACT_SIZE);
      }

      ImGui::InputText("Contact Name", contact_name, CONTACT_SIZE);

      ImGui::Text("Circle Color: ");
      ImGui::SameLine();
      static bool usegcolors_circle = false;
      ImGui::Checkbox("Use Google Colors##Circle", &usegcolors_circle);
      ImGui::SameLine();
      if(usegcolors_circle) {
        UseGoogleColors(gcolorbutton_name, "   ##circle", gc_str, &circle_color);
      } else {
        ImGui::ColorEdit3("##circlecolor3", &circle_color.x); ImGui::SameLine();
        if(ImGui::Button("Reset##circle")) circle_color = ImGui::ColorConvertU32ToFloat4(original_circle_color);
      }

      ImGui::Text("Text Color:   ");
      ImGui::SameLine();
      static bool usegcolor_text;
      ImGui::Checkbox("Use Google Colors##Text", &usegcolor_text);
      ImGui::SameLine();
      if(usegcolor_text) {
        UseGoogleColors(gcolorbutton_name, "   ##text", gc_str, &text_color);
      } else {
        ImGui::ColorEdit3("##textcolor3", &text_color.x); ImGui::SameLine();
        if(ImGui::Button("Reset##text")) text_color = ImGui::ColorConvertU32ToFloat4(original_text_color);
      }

      static bool show_debug_lines = false;
      ImGui::Checkbox("Show lines", &show_debug_lines);
      if(ImGui::Button("Save"))
      {
        if(text.surface)
        {
          output_surface = SDL_CreateRGBSurface(0, dim, dim, 32, bmask, gmask, rmask, amask);
          SDL_FillRect(output_surface, NULL, 0x00000000);
          SDL_Rect rect = {(output_surface->w / 2) - (circle->w / 2), (output_surface->h / 2) - (circle->h / 2) + 4, circle->w, circle->h};
          SDL_BlitSurface(circle, NULL, output_surface, &rect);
          rect = {(output_surface->w / 2) - (text.surface->w / 2), (output_surface->h / 2) - (text.fm.midpoint), text.surface->w, text.surface->h};
          SDL_BlitSurface(text.surface, NULL, output_surface, &rect);
          int size = strlen(contact_name) + strlen(".png") + 4 + 1;
          char *output_filename = (char *)calloc(size, sizeof(char));
          output_filename[size - 1] = '\0';
          strcat(output_filename, "out/");
          if(use_contact_name) strcat(output_filename, contact_name);
          else strcat(output_filename, filename_save);
          strcat(output_filename, ".png");
          // NOTE: Win32, blech
          if(CreateDirectory("out", NULL) || ERROR_ALREADY_EXISTS == GetLastError())
          {
            SDL_SavePNG(output_surface, output_filename);
            free(output_filename);
            saved = true;
          }
          else
          {
            error = true;
          }
        }
        else
        {
          error = true;
        }
      }
      if(error)
      {
        if(!text.surface) {ImGui::SameLine(); ImGui::Text("Please enter a contact name first!");}
        else {ImGui::SameLine(); ImGui::Text("Error saving file!!!");}
        error_timer += io.DeltaTime;
        if(error_timer > 3.0f) 
        {
          error = false;
          error_timer = 0.0f;
        }
      }
      if(saved)
      {
        ImGui::SameLine(); ImGui::Text("Saved! (It Looks like this):");
        Debug_SurfaceBoundingBox(output_surface, 0x323232FF);
        glBindTexture(GL_TEXTURE_2D, output_image);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, output_surface->w, output_surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, output_surface->pixels);
        ImGui::Image((void *)(intptr_t)output_image, ImVec2(output_surface->w, output_surface->h));
        save_timer += io.DeltaTime;
        if(save_timer > 3.0f)
        {
          save_timer = 0.0f;
          saved = false;
          SDL_FreeSurface(output_surface);
        }
      }
      ImGui::TextColored(ImVec4(0xFF, 0x00, 0x00, 0xFF),"(Warning! Will overwrite any file with the same name!)");
      SDL_Color t_color = {(uint8_t)(text_color.z * 0xFF), (uint8_t)(text_color.y * 0xFF), (uint8_t)(text_color.x * 0xFF), (uint8_t)(text_color.w * 0xFF)};
      UpdateText(&text, fontlist_roboto[size], t_color, contact_name);
      draw_list->AddRectFilled(ImVec2((G_WindowW / 2) - (circle->w / 2) + 5, (G_WindowH / 2) - (circle->h / 2) - 5), ImVec2((G_WindowW / 2) + (circle->w / 2) - 5, (G_WindowH / 2) + (circle->h / 2) + 5), ImColor(ImVec4(1.0f, 1.0f, 1.0f, 0.03f)));
      if(circle)
      {
        SDL_Color c_color = {(uint8_t)(circle_color.z * 0xFF), (uint8_t)(circle_color.y * 0xFF), (uint8_t)(circle_color.x * 0xFF), (uint8_t)(circle_color.w * 0xFF)};
        SDL_FreeSurface(circle);
        circle = TTF_RenderText_Blended(fontlist_circlethings[dim-3], "a", c_color);
        glBindTexture(GL_TEXTURE_2D, circle_image);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, circle->w, circle->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, circle->pixels);
        ImGui::SetCursorScreenPos(ImVec2((G_WindowW / 2) - (circle->w / 2), (G_WindowH / 2) - (circle->h / 2) + 4));
        ImGui::Image((void *)(intptr_t)circle_image, ImVec2(circle->w, circle->h));
      }
      if(text.surface)
      {
        text.rect = {(G_WindowW / 2) - (text.surface->w / 2), (G_WindowH / 2) - (text.fm.midpoint), text.surface->w, text.surface->h};
        ImGui::SetCursorScreenPos(ImVec2(text.rect.x, text.rect.y));
        glBindTexture(GL_TEXTURE_2D, contact_image);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, text.surface->w, text.surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, text.surface->pixels);
        ImGui::Image((void *)(intptr_t)contact_image, ImVec2(text.surface->w, text.surface->h));
      }
      ImVec2 bb_a = ImVec2((G_WindowW / 2) - (circle->w / 2) + 5, (G_WindowH / 2) - (circle->h / 2) - 5);
      ImVec2 bb_b = ImVec2((G_WindowW / 2) + (circle->w / 2) - 5, (G_WindowH / 2) + (circle->h / 2) + 5);
      if(show_debug_lines) Debug_ImGuiBoundingBox(draw_list, ImVec2(G_WindowW / 2, G_WindowH / 2), ImVec2(dim, dim));
      draw_list->AddRect(bb_a, bb_b, ImColor(0.3f, 0.3f, 0.3f, 1.0f));
    }
    ImGui::End();

    ImGuiRender();
    SDL_GL_SwapWindow(window);
    //-----------------------------//
    uint64_t end = SDL_GetTicks();
    uint64_t mspf = end - start;
    char s[512];
    sprintf(s, "MSPF: %lld", mspf);
    SDL_SetWindowTitle(window, s);
    start = SDL_GetTicks();
    //-----------------------------//
	} while(G_running);

  ImGui_ImplSdlGL3_Shutdown();
  SDL_GL_DeleteContext(glcontext);
  FreeFontList(fontlist_roboto);
  TTF_Quit();
	SDL_Quit();

	printf("Goodbye!\n\n");
	
	return 0;
}
