//===-- imjson/imjson.h - imjson library header file ------------*- C++ -*-===//
//
// Part of the imjson project
//
//===----------------------------------------------------------------------===//
//
// This file contains the declarations for the imjson library, which allows
// theme definition for imgui through JSON files.
//
//===----------------------------------------------------------------------===//

#pragma once

#if __has_include(<imgui/imgui.h>)
#include <imgui/imgui.h>
#elif __has_include("imgui/imgui.h")
#include "imgui/imgui.h"
#else
#error "imgui/imgui.h not found, specify include path for imgui"
#endif

#include <nlohmann/json.hpp>

// error handling macros
#define IMJSON_ASSERT(cond, msg) do { if (!(cond)) { std::cerr << "imjson error: " << msg << std::endl; } } while (0)
#define IMJSON_TRY try {
#define IMJSON_CATCH(msg) } catch (...) { IMJSON_ASSERT(false, msg); }

namespace imjson {
	using json = nlohmann::json;

	static const std::map<std::string, ImGuiStyleVar> style_map = {
		{"Alpha", ImGuiStyleVar_::ImGuiStyleVar_Alpha},
		{"DisabledAlpha", ImGuiStyleVar_::ImGuiStyleVar_DisabledAlpha},
		{"WindowPadding", ImGuiStyleVar_::ImGuiStyleVar_WindowPadding},
		{"WindowRounding", ImGuiStyleVar_::ImGuiStyleVar_WindowRounding},
		{"WindowBorderSize", ImGuiStyleVar_::ImGuiStyleVar_WindowBorderSize},
		{"WindowMinSize", ImGuiStyleVar_::ImGuiStyleVar_WindowMinSize},
		{"WindowTitleAlign", ImGuiStyleVar_::ImGuiStyleVar_WindowTitleAlign},
		{"ChildRounding", ImGuiStyleVar_::ImGuiStyleVar_ChildRounding},
		{"ChildBorderSize", ImGuiStyleVar_::ImGuiStyleVar_ChildBorderSize},
		{"PopupRounding", ImGuiStyleVar_::ImGuiStyleVar_PopupRounding},
		{"PopupBorderSize", ImGuiStyleVar_::ImGuiStyleVar_PopupBorderSize},
		{"FramePadding", ImGuiStyleVar_::ImGuiStyleVar_FramePadding},
		{"FrameRounding", ImGuiStyleVar_::ImGuiStyleVar_FrameRounding},
		{"FrameBorderSize", ImGuiStyleVar_::ImGuiStyleVar_FrameBorderSize},
		{"ItemSpacing", ImGuiStyleVar_::ImGuiStyleVar_ItemSpacing},
		{"ItemInnerSpacing", ImGuiStyleVar_::ImGuiStyleVar_ItemInnerSpacing},
		{"IndentSpacing", ImGuiStyleVar_::ImGuiStyleVar_IndentSpacing},
		{"ScrollbarSize", ImGuiStyleVar_::ImGuiStyleVar_ScrollbarSize},
		{"ScrollbarRounding", ImGuiStyleVar_::ImGuiStyleVar_ScrollbarRounding},
		{"GrabMinSize", ImGuiStyleVar_::ImGuiStyleVar_GrabMinSize},
		{"GrabRounding", ImGuiStyleVar_::ImGuiStyleVar_GrabRounding},
		{"TabRounding", ImGuiStyleVar_::ImGuiStyleVar_TabRounding},
		{"ButtonTextAlign", ImGuiStyleVar_::ImGuiStyleVar_ButtonTextAlign},
		{"SelectableTextAlign", ImGuiStyleVar_::ImGuiStyleVar_SelectableTextAlign},
	};

	static const std::map<std::string, ImGuiCol> color_map = {
		{"Text", ImGuiCol_::ImGuiCol_Text},
		{"TextDisabled", ImGuiCol_::ImGuiCol_TextDisabled},
		{"WindowBg", ImGuiCol_::ImGuiCol_WindowBg},
		{"ChildBg", ImGuiCol_::ImGuiCol_ChildBg},
		{"PopupBg", ImGuiCol_::ImGuiCol_PopupBg},
		{"Border", ImGuiCol_::ImGuiCol_Border},
		{"BorderShadow", ImGuiCol_::ImGuiCol_BorderShadow},
		{"FrameBg", ImGuiCol_::ImGuiCol_FrameBg},
		{"FrameBgHovered", ImGuiCol_::ImGuiCol_FrameBgHovered},
		{"FrameBgActive", ImGuiCol_::ImGuiCol_FrameBgActive},
		{"TitleBg", ImGuiCol_::ImGuiCol_TitleBg},
		{"TitleBgActive", ImGuiCol_::ImGuiCol_TitleBgActive},
		{"TitleBgCollapsed", ImGuiCol_::ImGuiCol_TitleBgCollapsed},
		{"MenuBarBg", ImGuiCol_::ImGuiCol_MenuBarBg},
		{"ScrollbarBg", ImGuiCol_::ImGuiCol_ScrollbarBg},
		{"ScrollbarGrab", ImGuiCol_::ImGuiCol_ScrollbarGrab},
		{"ScrollbarGrabHovered", ImGuiCol_::ImGuiCol_ScrollbarGrabHovered},
		{"ScrollbarGrabActive", ImGuiCol_::ImGuiCol_ScrollbarGrabActive},
		{"CheckMark", ImGuiCol_::ImGuiCol_CheckMark},
		{"SliderGrab", ImGuiCol_::ImGuiCol_SliderGrab},
		{"SliderGrabActive", ImGuiCol_::ImGuiCol_SliderGrabActive},
		{"Button", ImGuiCol_::ImGuiCol_Button},
		{"ButtonHovered", ImGuiCol_::ImGuiCol_ButtonHovered},
		{"ButtonActive", ImGuiCol_::ImGuiCol_ButtonActive},
		{"Header", ImGuiCol_::ImGuiCol_Header},
		{"HeaderHovered", ImGuiCol_::ImGuiCol_HeaderHovered},
		{"HeaderActive", ImGuiCol_::ImGuiCol_HeaderActive},
		{"Separator", ImGuiCol_::ImGuiCol_Separator},
		{"SeparatorHovered", ImGuiCol_::ImGuiCol_SeparatorHovered},
		{"SeparatorActive", ImGuiCol_::ImGuiCol_SeparatorActive},
		{"ResizeGrip", ImGuiCol_::ImGuiCol_ResizeGrip},
		{"ResizeGripHovered", ImGuiCol_::ImGuiCol_ResizeGripHovered},
		{"ResizeGripActive", ImGuiCol_::ImGuiCol_ResizeGripActive},
		{"Tab", ImGuiCol_::ImGuiCol_Tab},
		{"TabHovered", ImGuiCol_::ImGuiCol_TabHovered},
		{"TabActive", ImGuiCol_::ImGuiCol_TabActive},
		{"TabUnfocused", ImGuiCol_::ImGuiCol_TabUnfocused},
		{"TabUnfocusedActive", ImGuiCol_::ImGuiCol_TabUnfocusedActive},
		{"PlotLines", ImGuiCol_::ImGuiCol_PlotLines},
		{"PlotLinesHovered", ImGuiCol_::ImGuiCol_PlotLinesHovered},
		{"PlotHistogram", ImGuiCol_::ImGuiCol_PlotHistogram},
		{"PlotHistogramHovered", ImGuiCol_::ImGuiCol_PlotHistogramHovered},
		{"TextSelectedBg", ImGuiCol_::ImGuiCol_TextSelectedBg},
		{"DragDropTarget", ImGuiCol_::ImGuiCol_DragDropTarget},
		{"NavHighlight", ImGuiCol_::ImGuiCol_NavHighlight},
		{"NavWindowingHighlight", ImGuiCol_::ImGuiCol_NavWindowingHighlight},
		{"NavWindowingDimBg", ImGuiCol_::ImGuiCol_NavWindowingDimBg},
		{"ModalWindowDimBg", ImGuiCol_::ImGuiCol_ModalWindowDimBg},
	};

	/**
	 * @brief Load theme from a file.
	 *
	 * @param filename The name of the file containing the theme information in JSON format.
	 */
	void load_theme_from_file(const std::string& filename);

	/**
	 * @brief Load theme from a string.
	 *
	 * @param json_str The string containing the theme information in JSON format.
	 */
	void load_theme_from_string(const std::string& json_str);

	/**
	 * @brief Load theme from a JSON object.
	 *
	 * @param j The JSON object containing the theme information.
	 */
	void load_theme_from_json(const json& j);

} // namespace imjson