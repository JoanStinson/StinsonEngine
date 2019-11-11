#include "Globals.h"
#include "../Libraries/ImGui/imgui.h"
#include <vector>
#include <string>

static ImGuiTextBuffer consoleBuf;
static bool logPause = false;

void Log(const char file[], int line, const char *format, ...) {
	static char tmp_string[4096];
	static char tmp_string2[4096];
	static va_list  ap;

	va_start(ap, format);
	vsprintf_s(tmp_string, 4096, format, ap);
	if (!logPause) consoleBuf.appendfv(format, ap);
	va_end(ap);
	sprintf_s(tmp_string2, 4096, "\n%s(%d) : %s", file, line, tmp_string);
	OutputDebugString((const wchar_t*)tmp_string2);
}

template <typename T>
bool ElementInVector(const std::vector<T> &vec, const T &element) {
	for (unsigned i = 0; i < vec.size(); ++i)
		if (vec[i] == element) return true;
	return false;
}

void DrawLogText(const std::vector<std::string> &strLines, int index) {
	if (strstr(strLines[index].c_str(), "<Severity:HIGH>")) {
		ImGui::TextColored(ImVec4(1.0F, 0.0F, 0.0F, 1.0F), "%s", strLines[index].c_str());
	}
	else if (strstr(strLines[index].c_str(), "<Severity:MEDIUM>")) {
		ImGui::TextColored(ImVec4(1.0F, 0.4F, 0.0F, 1.0F), "%s", strLines[index].c_str());
	}
	else if (strstr(strLines[index].c_str(), "<Severity:LOW>")) {
		ImGui::TextColored(ImVec4(1.0F, 1.0F, 0.0F, 1.0F), "%s", strLines[index].c_str());
	}
	else if (strstr(strLines[index].c_str(), "<Severity:NOTIFICATION>")) {
		ImGui::TextColored(ImVec4(0.5F, 0.9F, 0.5F, 1.0F), "%s", strLines[index].c_str());
	}
	else {
		ImGui::Text("%s", strLines[index].c_str());
	}
}

void DrawConsoleLog(bool *p_open) {
	static bool coloredText = false;
	static bool hasCleared = false;
	static bool collapse = false;
	static std::vector<std::string> strLines;

	if (ImGui::Begin("Console", p_open)) {
		if (ImGui::Button("Clear")) {
			consoleBuf.clear();
			strLines.clear();
			hasCleared = true;
		}
		ImGui::SameLine();
		if (ImGui::Button("Copy")) {
			ImGui::LogToClipboard();
			ImGui::LogText(consoleBuf.c_str());
			ImGui::LogFinish();
		}
		ImGui::SameLine();
		if (ImGui::Button("Collapse")) {
			collapse = !collapse;
		}
		ImGui::SameLine();
		if (ImGui::Button("Error Pause")) {
			logPause = !logPause;
		}
		ImGui::SameLine();
		static ImGuiTextFilter filter;
		filter.Draw();
		ImGui::Separator();

		static const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y;
		ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar);

		if (coloredText) {

			// Split buffer to lines
			//if (!ElementInVector(strLines, std::string(consoleBuf.c_str()))) {
				std::string strBuf = std::string(consoleBuf.c_str());
				consoleBuf.clear();
				std::string currLine;
				for (unsigned i = 0; i < strBuf.size(); ++i) {
					if (strBuf[i] != '\n')
						currLine += strBuf[i];
					else {
						strLines.push_back(currLine);
						currLine.clear();
					}
				}
				strBuf.clear();
			//}

			// Draw each line
			std::vector<std::string> previousLines;
			for (unsigned i = 0; i < strLines.size(); ++i) {
				if (filter.PassFilter(strLines[i].c_str())) {
					if (collapse) {
						if (!ElementInVector(previousLines, strLines[i]))
							DrawLogText(strLines, i);
					}
					else DrawLogText(strLines, i);
				}
				previousLines.push_back(strLines[i]);
			}
			if (!hasCleared) strLines.erase(strLines.begin() + 30, strLines.end());
			else strLines.clear();
		}
		else {
			if (!collapse) ImGui::TextUnformatted(consoleBuf.begin());
			else ImGui::Text(consoleBuf.begin());
		}

		if (!logPause) ImGui::SetScrollHere(1.0F);
		ImGui::EndChild();
	}
	ImGui::End();
}