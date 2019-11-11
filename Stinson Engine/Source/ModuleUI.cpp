#include "ModuleUI.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleCamera.h"
#include "ModuleModelLoader.h"
#include "ModuleProgram.h"
#include <SDL.h>
#include <il.h>
#include <ilu.h>
#include <ilut.h>
#include <glew.h>
#include "../Libraries/ImGui/imgui_impl_sdl.h"
#include "../Libraries/ImGui/imgui_impl_opengl3.h"

#define RED ImVec4(1.0F, 0.0F, 0.0F, 1.0F)
#define GREEN ImVec4(0.0F, 1.0F, 0.0F, 1.0F)
#define BLUE ImVec4(0.0F, 0.0F, 1.0F, 1.0F)
#define PINK ImVec4(1.0F, 0.0F, 1.0F, 1.0F)
#define YELLOW ImVec4(1.0F, 1.0F, 0.0F, 1.0F)
#define LIGHT_BLUE ImVec4(0.0F, 1.0F, 1.0F, 1.0F)

bool ModuleUI::Init() {
	LOG("Init Module UI\n");
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	ImGui::StyleColorsDark();
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer->GetContext());
	ImGui_ImplOpenGL3_Init("#version 430 core");
	return true;
}

bool ModuleUI::Start() {
	if (SDL_Has3DNow()) caps += "3DNow, ";
	if (SDL_HasAVX()) caps += "AVX, ";
	if (SDL_HasAVX2()) caps += "AVX2, ";
	if (SDL_HasMMX()) caps += "MMX, ";
	if (SDL_HasRDTSC()) caps += "RDTSC, ";
	if (SDL_HasSSE()) caps += "SSE, ";
	if (SDL_HasSSE2()) caps += "SSE2, ";
	if (SDL_HasSSE3()) caps += "SSE3, ";
	if (SDL_HasSSE41()) caps += "SSE41, ";
	if (SDL_HasSSE42()) caps += "SSE42";
	return true;
}

UpdateStatus ModuleUI::PreUpdate() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();
	return UpdateStatus::CONTINUE;
}

UpdateStatus ModuleUI::Update() {
	currentTime = SDL_GetTicks();

	UpdateStatus ret = DrawMainBar();

	if (showConfigWindow)
		DrawConfigWindow(&showConfigWindow);

	if (showConsoleLogWindow)
		DrawConsoleLog(&showConsoleLogWindow);

	if (showDemoWindow)
		ImGui::ShowTestWindow(&showDemoWindow);

	if (showAboutWindow)
		DrawAboutWindow(&showAboutWindow);

	return ret;
}

UpdateStatus ModuleUI::PostUpdate() {
	ImGui::Render();
	glViewport(0, 0, (GLsizei)io.DisplaySize.x, (GLsizei)io.DisplaySize.y);
	//ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	//glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
	//glClear(GL_COLOR_BUFFER_BIT);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	SDL_GL_SwapWindow(App->window->window);
	return UpdateStatus::CONTINUE;
}

bool ModuleUI::CleanUp() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	return true;
}

void ModuleUI::ProcessEvent(const SDL_Event *event) const {
	ImGui_ImplSDL2_ProcessEvent(event);
}

UpdateStatus ModuleUI::DrawMainBar() {
	ImGui::BeginMainMenuBar();

	if (ImGui::BeginMenu("File")) {
		//ImGui::MenuItem("New Scene", "Ctrl+N");
		//ImGui::MenuItem("Open Scene", "Ctrl+O");
		//ImGui::Separator();
		if (ImGui::MenuItem("Quit", "Alt+F4"))
			return UpdateStatus::STOP;
		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Window")) {
		ImGui::MenuItem("Configuration", "Ctrl+Alt+C", &showConfigWindow);
		ImGui::MenuItem("Console", "Ctrl+Shift+C", &showConsoleLogWindow);

		if (ImGui::BeginMenu("Texture")) {

			if (ImGui::BeginMenu("Shape")) {
				if (ImGui::Checkbox("Triangle", &showTriangle)) {
					App->renderer->drawTriangle = !App->renderer->drawTriangle;
				}
				if (ImGui::Checkbox("Square", &showSquare)) {
					App->renderer->drawSquare = !App->renderer->drawSquare;
				}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Image")) {
				if (ImGui::Checkbox("Sonic", &showSanicTexture)) {
					App->renderer->texture = App->textures->Load("../Resources/Assets/sonic.png", App->renderer->imageInfo);
				}
				if (ImGui::Checkbox("Butterflies", &showButterfliesTexture)) {
					App->renderer->texture = App->textures->Load("../Resources/Assets/butterflies.jpg", App->renderer->imageInfo);
				}
				if (ImGui::Checkbox("Link", &showLinkTexture)) {
					App->renderer->texture = App->textures->Load("../Resources/Assets/link.dds", App->renderer->imageInfo);
				}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Wrapping Mode")) {
				if (ImGui::Checkbox("Repeat", &showRepeat)) {
					if (showRepeat)
						App->renderer->texture = App->textures->Load("../Resources/Assets/link.dds", App->renderer->imageInfo, 1, 0, true);
				}
				ImGui::SameLine();
				HelpMarker("Repeats the texture image");
				if (ImGui::Checkbox("Mirrored Repeat", &showMirroredRepeat)) {
					if (showMirroredRepeat)
						App->renderer->texture = App->textures->Load("../Resources/Assets/link.dds", App->renderer->imageInfo, 2, 0, true);
				}
				ImGui::SameLine();
				HelpMarker("Same as repeat, but mirrors the image with each repeat");
				if (ImGui::Checkbox("Clamp to Edge", &showClampToEdge)) {
					if (showClampToEdge)
						App->renderer->texture = App->textures->Load("../Resources/Assets/link.dds", App->renderer->imageInfo, 3, 0, true);
				}
				ImGui::SameLine();
				HelpMarker("Clamps the coordinates between 0 and 1. The result is that higher coordinates become clamped to the edge, resulting in a stretched edge pattern");
				if (ImGui::Checkbox("Clamp to Border", &showClampToBorder)) {
					if (showClampToBorder)
						App->renderer->texture = App->textures->Load("../Resources/Assets/link.dds", App->renderer->imageInfo, 4, 0, true);
				}
				ImGui::SameLine();
				HelpMarker("Coordinates outside the range are now given a user-specified border color");
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Filtering Mode")) {
				if (ImGui::Checkbox("Nearest", &showNearest)) {
					if (showNearest)
						App->renderer->texture = App->textures->Load("../Resources/Assets/link.dds", App->renderer->imageInfo, 4, 1, false);
				}
				ImGui::SameLine();
				HelpMarker("OpenGL selects the pixel which center is closest to the texture coordinate");
				if (ImGui::Checkbox("Linear", &showLinear)) {
					if (showLinear)
						App->renderer->texture = App->textures->Load("../Resources/Assets/link.dds", App->renderer->imageInfo, 4, 0, false);
				}
				ImGui::SameLine();
				HelpMarker("Takes an interpolated value from the texture coordinate's neighboring texels, approximating a color between the texels");
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Mipmaps", &showMipMaps)) {
				if (ImGui::Checkbox("Yes", &showMipMaps)) {
					if (showMipMaps)
						App->renderer->texture = App->textures->Load("../Resources/Assets/link.dds", App->renderer->imageInfo, 4, 0, true);
				}
				ImGui::SameLine();
				HelpMarker("After a certain distance threshold from the viewer, OpenGL will use a different mipmap texture that best suits the distance to the object. Because the object is far away, the smaller resolution will not be noticeable to the user. Also, mipmaps have the added bonus feature that they're good for performance as well.");
				ImGui::EndMenu();
			}

			ImGui::Separator();
			ImGui::Text("Width:");
			ImGui::SameLine();
			ImGui::TextColored(LIGHT_BLUE, " %dpx (%db)", App->renderer->imageInfo->Width, App->renderer->imageInfo->Width * App->renderer->imageInfo->Bpp);
			ImGui::Text("Height:");
			ImGui::SameLine();
			ImGui::TextColored(LIGHT_BLUE, "%dpx (%db)", App->renderer->imageInfo->Height, App->renderer->imageInfo->Height * App->renderer->imageInfo->Bpp);
			ImGui::Text("Pixel Depth:");
			ImGui::SameLine();
			ImGui::TextColored(LIGHT_BLUE, "%d", App->renderer->imageInfo->Depth);
			ImGui::Text("Format:");
			ImGui::SameLine();
			ILenum format = App->renderer->imageInfo->Type;
			if (format == (int)IL_PNG)		ImGui::TextColored(LIGHT_BLUE, "PNG");
			else if (format == (int)IL_JPG) ImGui::TextColored(LIGHT_BLUE, "JPG");
			else if (format == (int)IL_DDS) ImGui::TextColored(LIGHT_BLUE, "DDS");
			else ImGui::TextColored(RED, "INVALID");

			ImGui::EndMenu();
		}

		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Help")) {
		ImGui::MenuItem("Gui Demo", nullptr, &showDemoWindow);
		if (ImGui::MenuItem("Documentation"))
			RequestBrowser("https://github.com/JoanStinson/Cplusplus_for_Video_Games/wiki");
		if (ImGui::MenuItem("Download latest"))
			RequestBrowser("https://github.com/JoanStinson/Cplusplus_for_Video_Games/releases");
		if (ImGui::MenuItem("Report a bug"))
			RequestBrowser("https://github.com/JoanStinson/Cplusplus_for_Video_Games/issues");
		ImGui::MenuItem("About", nullptr, &showAboutWindow);
		ImGui::EndMenu();
	}

	ImGui::EndMainMenuBar();
	return UpdateStatus::CONTINUE;
}

void ModuleUI::DrawConfigWindow(bool *p_open) {
	ImGui::Begin("Configuration", p_open);
	if (showConfigWindow) {
		ImGui::Text("Welcome to %s!", &engineName[0]);
		ImGui::Spacing();

		if (ImGui::CollapsingHeader("Camera")) {
			static float fov = App->camera->frustum.verticalFov;
			static float aspectRatio = App->camera->aspectRatio;
			static ImVec4 color = ImVec4(114.0F / 255.0F, 144.0F / 255.0F, 154.0F / 255.0F, 200.0F / 255.0F);

			if (ImGui::DragFloat3("Front", (float*)&App->camera->frustum.front)) {
				App->camera->CalculateMatrixes();
			}
			if (ImGui::DragFloat3("Up", (float*)&App->camera->frustum.up)) {
				App->camera->CalculateMatrixes();
			}
			if (ImGui::DragFloat3("Position", (float*)&App->camera->frustum.pos)) {
				App->camera->CalculateMatrixes();
			}
			ImGui::DragFloat("Mov Speed", &App->camera->movSpeed);
			ImGui::DragFloat("Rot Speed", &App->camera->rotSpeed);
			ImGui::DragFloat("Zoom Speed", &App->camera->zoomSpeed);
			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::Spacing();
			if (ImGui::DragFloat("Near Plane", &App->camera->frustum.nearPlaneDistance)) {
				App->camera->CalculateMatrixes();
			}
			if (ImGui::DragFloat("Far Plane", &App->camera->frustum.farPlaneDistance)) {
				App->camera->CalculateMatrixes();
			}
			if (ImGui::DragFloat("Field of View", &fov)) {
				App->camera->SetFOV(fov);
			}
			if (ImGui::DragFloat("Aspect Ratio", &aspectRatio)) {
				App->camera->SetAspectRatio(aspectRatio);
			}
			ImGui::Text("Background Color:");
			ImGui::SameLine();
			ImGui::ColorEdit4("Background Color##3", (float*)&color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
			ImGui::SameLine();
			HelpMarker("Click to open color picker and change bg color");
			if (ImGui::Button("Reset Camera")) {
				App->camera->ResetCamera();
				fov = App->camera->frustum.verticalFov;
				aspectRatio = App->camera->aspectRatio;
				color = ImVec4(114.0F / 255.0F, 144.0F / 255.0F, 154.0F / 255.0F, 200.0F / 255.0F);
			}
		}

		if (ImGui::CollapsingHeader("Application")) {
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0F / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			DrawLogFrames(fpsLog, ImGui::GetIO().Framerate, fpsTitle, sizeof(fpsTitle) / sizeof(fpsTitle[0]), "Framerate %.1f", "##framerate", 100.0F, fpsTime);
			DrawLogFrames(msLog, 1000.0F / ImGui::GetIO().Framerate, msTitle, sizeof(msTitle) / sizeof(msTitle[0]), "Milliseconds %0.1f", "##milliseconds", 40.0F, msTime);
		}

		if (ImGui::CollapsingHeader("Window")) {
			if (ImGui::SliderFloat("Brightness", &brightness, 0.0F, 1.0F))
				App->window->SetWindowBrightness(brightness);
			if (ImGui::SliderInt("Width", &width, 640, 1920))
				App->window->SetWindowSize(width, height);
			if (ImGui::SliderInt("Height", &height, 480, 1080))
				App->window->SetWindowSize(width, height);
			ImGui::Text("Refresh rate:");
			ImGui::SameLine();
			SDL_DisplayMode current;
			SDL_GetCurrentDisplayMode(0, &current);
			ImGui::TextColored(ImVec4(1.0F, 1.0F, 0.0F, 1.0F), "%d", current.refresh_rate);
			if (ImGui::Checkbox("Fullscreen", &fullscreen))
				App->window->SetFullscreen(fullscreen);
			ImGui::SameLine();
			if (ImGui::Checkbox("Resizable", &resizable))
				App->window->SetResizable(resizable);
			if (ImGui::Checkbox("Borderless", &borderless))
				App->window->SetBorderless(borderless);
			ImGui::SameLine();
			if (ImGui::Checkbox("Full Desktop", &fullDesktop))
				App->window->SetFullDesktop(fullDesktop);
		}

		if (ImGui::CollapsingHeader("Hardware")) {
			ImGui::Text("CPUs:");
			ImGui::SameLine();
			ImGui::TextColored(YELLOW, "%d", SDL_GetCPUCount());
			ImGui::SameLine();
			ImGui::TextColored(YELLOW, "(Cache: %dkb)", SDL_GetCPUCacheLineSize());
			ImGui::Text("System RAM:");
			ImGui::SameLine();
			ImGui::TextColored(YELLOW, "%.1fGb", (float)SDL_GetSystemRAM() / 1000);
			ImGui::Text("Caps:");
			ImGui::SameLine();
			ImGui::TextColored(YELLOW, "%s", &caps[0]);
			ImGui::Separator();
			ImGui::Text("GPU:");
			ImGui::SameLine();
			ImGui::TextColored(YELLOW, "%s", glGetString(GL_RENDERER));
			ImGui::Text("Brand:");
			ImGui::SameLine();
			ImGui::TextColored(YELLOW, "%s", glGetString(GL_VENDOR));
			static GLint totalMemoryKb = 0;
			static GLint currentMemoryKb = 0;
			glGetIntegerv(GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX, &totalMemoryKb);
			glGetIntegerv(GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &currentMemoryKb);
			ImGui::Text("VRAM Budget:");
			ImGui::SameLine();
			ImGui::TextColored(YELLOW, "%.1f Mb", (float)totalMemoryKb / 1000);
			ImGui::Text("VRAM Usage:");
			ImGui::SameLine();
			ImGui::TextColored(YELLOW, "%.1f Mb", ((float)totalMemoryKb / 1000) - ((float)currentMemoryKb / 1000));
			ImGui::Text("VRAM Available:");
			ImGui::SameLine();
			ImGui::TextColored(YELLOW, "%.1f Mb", (float)currentMemoryKb / 1000);
			ImGui::Text("VRAM Reserved:");
			ImGui::SameLine();
			ImGui::TextColored(YELLOW, "%.1f Mb", 0.0F);
		}

		if (ImGui::Button("Change Model")) {
			App->model->ChangeMesh(0, "../Resources/Assets/samus.obj", App->textures->Load("../Resources/Assets/samus.png"), *App->programs->textureProgram);
		}
	}
	ImGui::End();
}

void ModuleUI::DrawAboutWindow(bool *p_open) {
	if (ImGui::Begin("About", p_open)) {
		ImGui::Text("%s 1.0 WIP", &engineName[0]);

		ImGui::Separator();
		if (ImGui::InputTextWithHint("Engine Name", "Enter engine name", &engineName[0], sizeof(engineName) / sizeof(engineName[0])))
			App->window->SetWindowTitle(engineName);
		ImGui::SameLine();
		HelpMarker("Enter name to change the engine's name");
		ImGui::Text("Description:");
		ImGui::SameLine();
		ImGui::TextColored(LIGHT_BLUE, "%s", &description[0]);
		ImGui::Text("Author:");
		ImGui::SameLine();
		ImGui::TextColored(PINK, "%s", &author[0]);
		ImGui::Separator();
		if (ImGui::TreeNode("Libraries")) {
			static SDL_version compiled;
			SDL_VERSION(&compiled);
			ImGui::BulletText("SDL Version:");
			ImGui::SameLine();
			ImGui::TextColored(YELLOW, "%d.%d.%d", compiled.major, compiled.minor, compiled.patch);
			ImGui::BulletText("Glew Version:");
			ImGui::SameLine();
			ImGui::TextColored(YELLOW, "%s", glewGetString(GLEW_VERSION));
			ImGui::BulletText("DevIL Version:");
			ImGui::SameLine();
			ImGui::TextColored(YELLOW, "%d.%d.%d", (ilGetInteger(IL_VERSION_NUM) % 1000) / 100, (ilGetInteger(IL_VERSION_NUM) % 100) / 10, ilGetInteger(IL_VERSION_NUM) % 10);
			ImGui::BulletText("OpenGL Version:");
			ImGui::SameLine();
			ImGui::TextColored(YELLOW, "%s", glGetString(GL_VERSION));
			ImGui::TreePop();
		}
		ImGui::Separator();
		ImGui::Text("%s is licensed under the MIT License.", &engineName[0]);
		if (ImGui::SmallButton("GitHub repository"))
			RequestBrowser("https://github.com/JoanStinson/Cplusplus_for_Video_Games");
		ImGui::SameLine();
		if (ImGui::SmallButton("MIT License"))
			RequestBrowser("https://github.com/JoanStinson/Cplusplus_for_Video_Games/blob/master/LICENSE");
		ImGui::SameLine();
		HelpMarker("See LICENSE for more information");
	}
	ImGui::End();
}

void ModuleUI::DrawLogFrames(std::vector<float> &frames, float value, char *titleBuf, int titleSize, const char *title, const char *histogramTitle, float maxHeight, unsigned int &time) {
	if (frames.size() < 1)
		frames.push_back(value);
	else if (currentTime > time + 1000) {
		if (frames.size() > 100) {
			for (size_t i = 1; i < frames.size(); ++i)
				frames[i - 1] = frames[i];
			frames[frames.size() - 1] = value;
		}
		else frames.push_back(value);
		time = currentTime;
	}
	sprintf_s(titleBuf, titleSize, title, frames[frames.size() - 1]);
	ImGui::PlotHistogram(histogramTitle, &frames[0], frames.size(), 0, titleBuf, 0.0F, maxHeight, ImVec2(310, 100));
}

void ModuleUI::HelpMarker(const char *desc, bool showText) const {
	if (showText) ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered()) {
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0F);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

void ModuleUI::RequestBrowser(const char *link) const {
	ShellExecuteA(nullptr, "open", link, nullptr, nullptr, SW_SHOWNORMAL);
}
