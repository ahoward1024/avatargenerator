#ifndef _CUSTOM_COLORS_H
#define _CUSTOM_COLORS_H

#include "imgui/imgui.h"

ImVec4 IMCOLOR_BLANK     = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);

ImVec4 IMCOLOR_RED        = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
ImVec4 IMCOLOR_GREEN      = ImVec4(0.00f, 1.00f, 0.00f, 1.00f);
ImVec4 IMCOLOR_BLUE       = ImVec4(0.00f, 0.00f, 1.00f, 1.00f);
ImVec4 IMCOLOR_CYAN       = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
ImVec4 IMCOLOR_YELLOW     = ImVec4(1.00f, 1.00f, 0.00f, 1.00f);
ImVec4 IMCOLOR_MAGENTA    = ImVec4(1.00f, 0.00f, 1.00f, 1.00f);
ImVec4 IMCOLOR_LIGHT_GREY = ImVec4(0.75f, 0.75f, 0.75f, 1.00f);
ImVec4 IMCOLOR_GREY       = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
ImVec4 IMCOLOR_DARK_GREY  = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
ImVec4 IMCOLOR_BLACK      = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
ImVec4 IMCOLOR_WHITE      = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);

ImVec4 IMCOLOR_TEXT      = IMCOLOR_BLACK;
ImVec4 IMCOLOR_BG        = ImVec4(0.09f, 0.11f, 0.13f, 1.00f);
ImVec4 IMCOLOR_CHILDBG   = ImVec4(0.45f, 0.50f, 0.54f, 1.00f);
ImVec4 IMCOLOR_CHILDBG2  = ImVec4(0.62f, 0.65f, 0.67f, 1.00f);
ImVec4 IMCOLOR_FRAMEBG   = ImVec4(0.88f, 0.88f, 0.88f, 1.00f);
ImVec4 IMCOLOR_BORDER    = ImVec4(0.21f, 0.26f, 0.36f, 1.00f);
ImVec4 IMCOLOR_BUTTON    = ImVec4(0.52f, 0.57f, 0.62f, 1.00f);
ImVec4 IMCOLOR_BUTTON2   = ImVec4(0.94f, 0.73f, 0.23f, 1.00f);
ImVec4 IMCOLOR_HEADER    = ImVec4(0.81f, 0.83f, 0.85f, 1.00f);
ImVec4 IMCOLOR_SCROLLBAR = ImVec4(0.23f, 0.27f, 0.32f, 1.00f);
ImVec4 IMCOLOR_SLIDER_OO = ImVec4(0.85f, 0.38f, 0.08f, 1.00f);
ImVec4 IMCOLOR_SLIDER_B  = ImVec4(0.11f, 0.55f, 0.89f, 1.00f);
ImVec4 IMCOLOR_SLIDER_O  = ImVec4(0.94f, 0.73f, 0.23f, 1.00f);
ImVec4 IMCOLOR_SLIDER_Y  = ImVec4(0.85f, 0.88f, 0.05f, 1.00f);
ImVec4 IMCOLOR_PLOTLINES = ImVec4(1.00f, 0.29f, 0.01f, 1.00f);
ImVec4 IMCOLOR_DARKENING = ImVec4(0.00f, 0.00f, 0.00f, 0.60f);

void ImGui_SetTheme(ImGuiStyle& style)
{
  style.WindowRounding                        = 0.0f;
  style.FrameRounding                         = 0.0f;
  style.ChildRounding                         = 6.0f;
  style.Colors[ImGuiCol_Text]                 = IMCOLOR_TEXT;
  style.Colors[ImGuiCol_Button]               = IMCOLOR_BUTTON;
  style.Colors[ImGuiCol_ButtonHovered]        = IMCOLOR_YELLOW;
  style.Colors[ImGuiCol_ButtonActive]         = IMCOLOR_YELLOW;
  style.Colors[ImGuiCol_CloseButton]          = IMCOLOR_BUTTON;
  style.Colors[ImGuiCol_CloseButtonHovered]   = IMCOLOR_YELLOW;
  style.Colors[ImGuiCol_CloseButtonActive]    = IMCOLOR_BLANK;
  style.Colors[ImGuiCol_Header]               = IMCOLOR_HEADER;
  style.Colors[ImGuiCol_HeaderHovered]        = IMCOLOR_HEADER;
  style.Colors[ImGuiCol_HeaderActive]         = IMCOLOR_HEADER;
  style.Colors[ImGuiCol_ChildWindowBg]        = IMCOLOR_CHILDBG;
  style.Colors[ImGuiCol_FrameBg]              = IMCOLOR_FRAMEBG;
  style.Colors[ImGuiCol_FrameBgHovered]       = IMCOLOR_RED;
  style.Colors[ImGuiCol_FrameBgActive]        = IMCOLOR_YELLOW;
  style.Colors[ImGuiCol_FrameBg]              = IMCOLOR_FRAMEBG;
  style.Colors[ImGuiCol_TitleBgActive]        = IMCOLOR_HEADER;
  style.Colors[ImGuiCol_Border]               = IMCOLOR_BORDER;
  style.Colors[ImGuiCol_Column]               = IMCOLOR_BORDER;
  style.Colors[ImGuiCol_ScrollbarBg]          = IMCOLOR_CHILDBG;
  style.Colors[ImGuiCol_ScrollbarGrab]        = IMCOLOR_SCROLLBAR;
  style.Colors[ImGuiCol_SliderGrab]           = IMCOLOR_BUTTON;
  style.Colors[ImGuiCol_SliderGrabActive]     = IMCOLOR_YELLOW;
  style.Colors[ImGuiCol_PlotLines]            = IMCOLOR_PLOTLINES;
  style.Colors[ImGuiCol_ModalWindowDarkening] = IMCOLOR_DARKENING;
}

#endif