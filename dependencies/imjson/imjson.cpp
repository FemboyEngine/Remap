//===-- imjson/imjson.cpp - imjson library implementation ------*- C++ -*-===//
//
// Part of the imjson project
//
//===----------------------------------------------------------------------===//
//
// This file contains the implementation of the imjson library, which allows
// theme definition for imgui through JSON files.
//
//===----------------------------------------------------------------------===//

#include "imjson.h"

#include <fstream>
#include <iostream>

namespace imjson {

	// helper functions to set imgui style colors & variables from a JSON object
	
	void set_style_colors(const json& j) {
		IMJSON_TRY
			for (auto it = j.begin(); it != j.end(); ++it) {
				const std::string& name = it.key();
				const json& color = it.value();
				if (color.is_array() && color.size() == 4) {
					ImVec4 col(color[0], color[1], color[2], color[3]);
					auto color_it = color_map.find(name);
					if (color_it != color_map.end()) {
						ImGui::PushStyleColor(color_it->second, col);
					}
				}
			}
		IMJSON_CATCH("Error setting style colors")
	}

	void set_style_vars(const json& j) {
		IMJSON_TRY
			for (auto it = j.begin(); it != j.end(); ++it) {
				const std::string& name = it.key();
				const json& value = it.value();
				auto style_it = style_map.find(name);
				if (style_it != style_map.end()) {
					ImGui::PushStyleVar(style_it->second, value);
				}
			}
		IMJSON_CATCH("Error setting style variables")
	}

	// helper function to load fonts
	void load_fonts(const json& j) {
		IMJSON_TRY
			if (j.contains("Default") && j["Default"].is_object()) {
				const json& default_font = j["Default"];
				if (default_font.contains("Path") && default_font["Path"].is_string() &&
					default_font.contains("SizePixels") && default_font["SizePixels"].is_number()) {
					std::string path = default_font["Path"];
					float size_pixels = default_font["SizePixels"];
					ImGuiIO& io = ImGui::GetIO();
					io.Fonts->AddFontFromFileTTF(path.c_str(), size_pixels);
				}
			}
		IMJSON_CATCH("Error loading fonts")
	}

	void load_theme_from_file(const std::string& filename) {
		IMJSON_TRY
			std::ifstream file(filename);
		if (file.is_open()) {
			json j;
			file >> j;
			load_theme_from_json(j);
		}
		IMJSON_CATCH("Error loading theme from file")
	}

	void load_theme_from_string(const std::string& json_str) {
		IMJSON_TRY
			json j = json::parse(json_str);
		load_theme_from_json(j);
		IMJSON_CATCH("Error loading theme from string")
	}

	void load_theme_from_json(const json& j) {
		IMJSON_TRY
			for (auto it = j.begin(); it != j.end(); ++it) {
				const std::string& name = it.key();
				const json& value = it.value();
				if (name == "Colors" && value.is_object()) {
					set_style_colors(value);
				}
				else if (name == "Fonts" && value.is_object()) {
					load_fonts(value);
				}
				else if (name == "Rounding" && value.is_object()) {
					set_style_vars(value);
				}
				else if (style_map.find(name) != style_map.end() && value.is_array()) {
					ImGui::PushStyleVar(style_map.at(name), ImVec2(value[0], value[1]));
				}
			}
		IMJSON_CATCH("Error loading theme from JSON")
	}
}