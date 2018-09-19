#ifndef DRAWING_H
#define DRAWING_H

void ImGuiRender()
{
  glViewport(0, 0, (int)ImGui::GetIO().DisplaySize.x, (int)ImGui::GetIO().DisplaySize.y);
  glClearColor(IMCOLOR_BG.x, IMCOLOR_BG.y, IMCOLOR_BG.z, 1.00f);
  glClear(GL_COLOR_BUFFER_BIT);
  ImGui::Render();
}

// Thank you: https://stackoverflow.com/questions/3229391/sdl-sdl-ttf-transparent-blended-text#12773144
// Changes a surface's alpha value, by altering per-pixel alpha if necessary.
void SetSurfaceAlpha (SDL_Surface *surface, Uint8 alpha)
{
    SDL_PixelFormat* fmt = surface->format;

    // If surface has no alpha channel, just set the surface alpha.
    if( fmt->Amask == 0 ) {
        SDL_SetSurfaceAlphaMod(surface, alpha);
    }
    // Else change the alpha of each pixel.
    else {
        unsigned bpp = fmt->BytesPerPixel;
        // Scaling factor to clamp alpha to [0, alpha].
        float scale = alpha / 255.0f;

        SDL_LockSurface(surface);

        for (int y = 0; y < surface->h; ++y) 
        for (int x = 0; x < surface->w; ++x) {
            // Get a pointer to the current pixel.
            Uint32* pixel_ptr = (Uint32 *)( 
                    (Uint8 *)surface->pixels
                    + y * surface->pitch
                    + x * bpp
                    );

            // Get the old pixel components.
            Uint8 r, g, b, a;
            SDL_GetRGBA( *pixel_ptr, fmt, &r, &g, &b, &a );

            // Set the pixel with the new alpha.
            *pixel_ptr = SDL_MapRGBA( fmt, r, g, b, scale * a );
        }   

        SDL_UnlockSurface(surface);
    }       
}   

void flipVert(GLubyte *pixels, int width, int height)
{
  int stride = width * 4;
  GLubyte *swapline = (GLubyte *)malloc(stride);
  for(int row = 0; row < height / 2; row++) {
    memcpy(swapline, pixels + row * stride, stride);
    memcpy(pixels + row * stride, pixels + (height - row - 1) * stride, stride);
    memcpy(pixels + (height - row -1) * stride, swapline, stride);
  }
}

void Debug_SurfaceBoundingBox(SDL_Surface *surface, uint32_t color, bool midpoints = false)
{
  shlineColor(surface, 0, surface->w, 0, color); // TOP
  shlineColor(surface, 0, surface->w - 1, surface->h - 1, color); // BOT
  svlineColor(surface, 0, 0, surface->h - 1, color); // LEFT
  svlineColor(surface, surface->w - 1, 0, surface->h - 1, color); // RIGHT
  if(midpoints)
  {
    shlineColor(surface, 0, surface->w, surface->h / 2, color); // MID H
    svlineColor(surface, surface->w / 2, 0, surface->h, color); // MID V
  }
}

void Debug_TextMetricsVisual(Text *text, uint32_t color)
{
  int w = text->surface->w;
  GlyphMetric gm = GetGlyphMetric(text->font, 'a');
  int bot = text->fm.ascent;
  int top = text->fm.ascent - gm.maxy; // FIXME
  int ftop = text->fm.ascent - text->fm.maxy;
  int fbot = text->fm.ascent - text->fm.miny;
  #if 1
  //shlineColor(surface, 0, w, bot, color); // BOT DESCENT
  //shlineColor(surface, 0, w, top, color); // TOP ASCENT
  //shlineColor(surface, 0, w, ftop, color); // FULL TOP
  //shlineColor(surface, 0, w, fbot, color); // FULL BOT
  //shlineColor(surface, 0, w, ftop - font->fm.descent, 0x0000FFFF);
  //shlineColor(surface, 0, w, font->fm.midpoint, 0x00FF00FF);
  //shlineColor(surface, 0, w, font->fm.full_midpoint, 0x00FF00FF);
  //svlineColor(surface, surface->w / 2, 0, surface->h, 0x00FF00FF);
  #endif
}

void Debug_ImGuiBoundingBox(ImDrawList *draw_list, ImVec2 mid, ImVec2 dim)
{
  draw_list->AddLine(ImVec2(mid.x, mid.y + (dim.y / 2)), ImVec2(mid.x, mid.y - (dim.y / 2)), ImColor(1.0f, 0.0f, 0.0f, 0.5f));
  draw_list->AddLine(ImVec2(mid.x - (dim.x / 2), mid.y), ImVec2(mid.x + (dim.x / 2), mid.y), ImColor(1.0f, 0.0f, 0.0f, 0.5f));

  int xbuf = 20;
  int ybuf = 60;
  
  draw_list->AddLine(ImVec2(mid.x - (dim.x / 2) + xbuf, mid.y - (dim.y / 2)), ImVec2(mid.x - (dim.x / 2) + xbuf, mid.y + (dim.y / 2)), ImColor(0.0f, 1.0f, 1.0f, 0.5f));
  draw_list->AddLine(ImVec2(mid.x + (dim.x / 2) - xbuf, mid.y - (dim.y / 2)), ImVec2(mid.x + (dim.x / 2) - xbuf, mid.y + (dim.y / 2)), ImColor(0.0f, 1.0f, 1.0f, 0.5f));

  draw_list->AddLine(ImVec2(mid.x - (dim.x / 2), mid.y + ybuf), ImVec2(mid.x + (dim.x / 2), mid.y + ybuf), ImColor(0.0f, 1.0f, 1.0f, 0.5f));
  draw_list->AddLine(ImVec2(mid.x - (dim.x / 2), mid.y - ybuf), ImVec2(mid.x + (dim.x / 2), mid.y - ybuf), ImColor(0.0f, 1.0f, 1.0f, 0.5f));
}

uint32_t _getpixel(SDL_Surface *surface, int x, int y)
{
  int bpp = surface->format->BytesPerPixel;
  uint8_t *p = (uint8_t *)surface->pixels + y * surface->pitch + x * bpp;
  switch(bpp)
  {
    case 1: return *p; break;
    case 2: return *(uint16_t *)p; break;
    case 3:
      if(SDL_BYTEORDER == SDL_BIG_ENDIAN) return p[0] << 16 | p[1] << 8 | p[2];
      else return p[0] | p[1] << 8 | p[2] << 16;
      break;
    case 4: return *(uint32_t *)p; break;
    default: return 0;
  }
}

void getpixel(SDL_Surface *surface, int x, int y, uint8_t *r, uint8_t *g, uint8_t *b, uint8_t *a)
{
  if((x < 0 || x > surface->w - 1) || (y < 0 || y > surface->h - 1)) return;
  SDL_GetRGBA(_getpixel(surface, x, y), surface->format, r, g, b, a);
}

uint32_t getpixel(SDL_Surface *surface, int x, int y)
{
  if((x < 0) || (x > surface->w - 1) || (y < 0) || (y > surface->h - 1)) return 0xDEADBEEF;
  uint8_t r, g, b, a;
  return _getpixel(surface, x, y);
}

void _setpixel(SDL_Surface *surface, int x, int y, uint32_t pixel)
{
  int bpp = surface->format->BytesPerPixel;
  uint8_t *p = (uint8_t *)surface->pixels + y * surface->pitch + x * bpp;
  switch(bpp)
  {
    case 1: *p = pixel; break;
    case 2: *(uint16_t *)p = pixel; break;
    case 3:
      if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
      {
        p[0] = (pixel >> 16) & 0xff;
        p[1] = (pixel >> 8) & 0xff;
        p[2] = pixel & 0xff;
      }
      else 
      {
        p[0] = pixel & 0xff;
        p[1] = (pixel >> 8) & 0xff;
        p[2] = (pixel >> 16) & 0xff;
      }
      break;
    case 4: *(uint32_t *)p = pixel; break;
  }
}

void setpixel(SDL_Surface *surface, int x, int y, uint32_t color)
{
  _setpixel(surface, x, y, color);
}

void setpixel(SDL_Surface *surface, int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
  _setpixel(surface, x, y, SDL_MapRGBA(surface->format, r, g, b, a));
}

#endif
