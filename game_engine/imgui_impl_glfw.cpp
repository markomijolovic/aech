#include "imgui.h"
#include "imgui_impl_glfw.h"

#include "GLFW/glfw3.h"

enum GlfwClientApi
{
	GlfwClientApi_Unknown,
	GlfwClientApi_OpenGL,
	GlfwClientApi_Vulkan
};

static GLFWwindow*   g_Window{};
static GlfwClientApi g_ClientApi = GlfwClientApi_Unknown;
static double        g_Time{};
static bool          g_MouseJustPressed[5]{};
static GLFWcursor*   g_MouseCursors[ImGuiMouseCursor_COUNT]{};

static GLFWmousebuttonfun g_PrevUserCallbackMouseButton{};
static GLFWscrollfun      g_PrevUserCallbackScroll{};
static GLFWkeyfun         g_PrevUserCallbackKey{};
static GLFWcharfun        g_PrevUserCallbackChar{};

static const char* ImGui_ImplGlfw_GetClipboardText(void* user_data)
{
	return glfwGetClipboardString(static_cast<GLFWwindow*>(user_data));
}

static void ImGui_ImplGlfw_SetClipboardText(void* user_data, const char* text)
{
	glfwSetClipboardString(static_cast<GLFWwindow*>(user_data), text);
}

void ImGui_ImplGlfw_MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (g_PrevUserCallbackMouseButton != nullptr)
	{
		g_PrevUserCallbackMouseButton(window, button, action, mods);
	}

	if (action == GLFW_PRESS && button >= 0 && button < IM_ARRAYSIZE(g_MouseJustPressed))
	{
		g_MouseJustPressed[button] = true;
	}
}


void ImGui_ImplGlfw_ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (g_PrevUserCallbackScroll != nullptr)
	{
		g_PrevUserCallbackScroll(window, xoffset, yoffset);
	}

	auto& io = ImGui::GetIO();
	io.MouseWheelH += static_cast<float>(xoffset);
	io.MouseWheel += static_cast<float>(yoffset);
}

void ImGui_ImplGlfw_KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (g_PrevUserCallbackKey != nullptr)
		g_PrevUserCallbackKey(window, key, scancode, action, mods);

	ImGuiIO& io = ImGui::GetIO();
	if (action == GLFW_PRESS)
		io.KeysDown[key] = true;
	if (action == GLFW_RELEASE)
		io.KeysDown[key] = false;

	// Modifiers are not reliable across systems
	io.KeyCtrl  = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
	io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
	io.KeyAlt   = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
	io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];
}


void ImGui_ImplGlfw_CharCallback(GLFWwindow* window, unsigned int c)
{
	if (g_PrevUserCallbackChar != nullptr)
		g_PrevUserCallbackChar(window, c);

	ImGuiIO& io = ImGui::GetIO();
	io.AddInputCharacter(c);
}


static bool ImGui_ImplGlfw_Init(GLFWwindow* window, bool install_callbacks, GlfwClientApi client_api)
{
	g_Window = window;
	g_Time   = 0;

	auto& io = ImGui::GetIO();
	io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
	io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
	io.BackendPlatformName = "imgui_impl_glfw";

	io.KeyMap[ImGuiKey_Tab]         = GLFW_KEY_TAB;
	io.KeyMap[ImGuiKey_LeftArrow]   = GLFW_KEY_LEFT;
	io.KeyMap[ImGuiKey_RightArrow]  = GLFW_KEY_RIGHT;
	io.KeyMap[ImGuiKey_UpArrow]     = GLFW_KEY_UP;
	io.KeyMap[ImGuiKey_DownArrow]   = GLFW_KEY_DOWN;
	io.KeyMap[ImGuiKey_PageUp]      = GLFW_KEY_PAGE_UP;
	io.KeyMap[ImGuiKey_PageDown]    = GLFW_KEY_PAGE_DOWN;
	io.KeyMap[ImGuiKey_Home]        = GLFW_KEY_HOME;
	io.KeyMap[ImGuiKey_End]         = GLFW_KEY_END;
	io.KeyMap[ImGuiKey_Insert]      = GLFW_KEY_INSERT;
	io.KeyMap[ImGuiKey_Delete]      = GLFW_KEY_DELETE;
	io.KeyMap[ImGuiKey_Backspace]   = GLFW_KEY_BACKSPACE;
	io.KeyMap[ImGuiKey_Space]       = GLFW_KEY_SPACE;
	io.KeyMap[ImGuiKey_Enter]       = GLFW_KEY_ENTER;
	io.KeyMap[ImGuiKey_Escape]      = GLFW_KEY_ESCAPE;
	io.KeyMap[ImGuiKey_KeyPadEnter] = GLFW_KEY_KP_ENTER;
	io.KeyMap[ImGuiKey_A]           = GLFW_KEY_A;
	io.KeyMap[ImGuiKey_C]           = GLFW_KEY_C;
	io.KeyMap[ImGuiKey_V]           = GLFW_KEY_V;
	io.KeyMap[ImGuiKey_X]           = GLFW_KEY_X;
	io.KeyMap[ImGuiKey_Y]           = GLFW_KEY_Y;
	io.KeyMap[ImGuiKey_Z]           = GLFW_KEY_Z;

	io.SetClipboardTextFn = ImGui_ImplGlfw_SetClipboardText;
	io.GetClipboardTextFn = ImGui_ImplGlfw_GetClipboardText;
	io.ClipboardUserData  = g_Window;

	g_MouseCursors[ImGuiMouseCursor_Arrow]     = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
	g_MouseCursors[ImGuiMouseCursor_TextInput] = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
	g_MouseCursors[ImGuiMouseCursor_ResizeAll] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
	// FIXME: GLFW doesn't have this.
	g_MouseCursors[ImGuiMouseCursor_ResizeNS]   = glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR);
	g_MouseCursors[ImGuiMouseCursor_ResizeEW]   = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
	g_MouseCursors[ImGuiMouseCursor_ResizeNESW] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
	// FIXME: GLFW doesn't have this.
	g_MouseCursors[ImGuiMouseCursor_ResizeNWSE] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
	// FIXME: GLFW doesn't have this.
	g_MouseCursors[ImGuiMouseCursor_Hand] = glfwCreateStandardCursor(GLFW_HAND_CURSOR);

	if (install_callbacks)
	{
		g_PrevUserCallbackMouseButton = glfwSetMouseButtonCallback(window, ImGui_ImplGlfw_MouseButtonCallback);
		g_PrevUserCallbackScroll      = glfwSetScrollCallback(window, ImGui_ImplGlfw_ScrollCallback);
		g_PrevUserCallbackKey         = glfwSetKeyCallback(window, ImGui_ImplGlfw_KeyCallback);
		g_PrevUserCallbackChar        = glfwSetCharCallback(window, ImGui_ImplGlfw_CharCallback);
	}

	g_ClientApi = client_api;
	return true;
}

bool ImGui_ImplGlfw_InitForOpenGL(GLFWwindow* window, bool install_callbacks)
{
	return ImGui_ImplGlfw_Init(window, install_callbacks, GlfwClientApi_OpenGL);
}

void ImGui_ImplGlfw_Shutdown()
{
	for (ImGuiMouseCursor cursor_n = 0; cursor_n < ImGuiMouseCursor_COUNT; cursor_n++)
	{
		glfwDestroyCursor(g_MouseCursors[cursor_n]);
		g_MouseCursors[cursor_n] = nullptr;
	}

	g_ClientApi = GlfwClientApi_Unknown;
}

static void ImGui_ImplGlfw_UpdateMousePosAndButtons()
{
	// Update buttons
	auto& io = ImGui::GetIO();
	for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++)
	{
		// If a mouse press event came, always pass it as "mouse held this frame", so we don't miss click-release events that are shorter than 1 frame.
		io.MouseDown[i]       = g_MouseJustPressed[i] || glfwGetMouseButton(g_Window, i) != 0;
		g_MouseJustPressed[i] = false;
	}

	// Update mouse position
	const ImVec2 mouse_pos_backup = io.MousePos;
	io.MousePos                   = ImVec2(-FLT_MAX, -FLT_MAX);
	const bool focused            = glfwGetWindowAttrib(g_Window, GLFW_FOCUSED) != 0;
	if (focused)
	{
		if (io.WantSetMousePos)
		{
			glfwSetCursorPos(g_Window,
			                 static_cast<double>(mouse_pos_backup.x),
			                 static_cast<double>(mouse_pos_backup.y));
		}
		else
		{
			double mouse_x, mouse_y;
			glfwGetCursorPos(g_Window, &mouse_x, &mouse_y);
			io.MousePos = ImVec2(static_cast<float>(mouse_x), static_cast<float>(mouse_y));
		}
	}
}

static void ImGui_ImplGlfw_UpdateMouseCursor()
{
	auto& io = ImGui::GetIO();
	if ((io.ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange) || glfwGetInputMode(g_Window, GLFW_CURSOR) ==
	    GLFW_CURSOR_DISABLED)
		return;

	ImGuiMouseCursor imgui_cursor = ImGui::GetMouseCursor();
	if (imgui_cursor == ImGuiMouseCursor_None || io.MouseDrawCursor)
	{
		// Hide OS mouse cursor if imgui is drawing it or if it wants no cursor
		glfwSetInputMode(g_Window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	}
	else
	{
		// Show OS mouse cursor
		// FIXME-PLATFORM: Unfocused windows seems to fail changing the mouse cursor with GLFW 3.2, but 3.3 works here.
		glfwSetCursor(g_Window,
		              g_MouseCursors[imgui_cursor]
			              ? g_MouseCursors[imgui_cursor]
			              : g_MouseCursors[ImGuiMouseCursor_Arrow]);
		glfwSetInputMode(g_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}

void ImGui_ImplGlfw_NewFrame()
{
	auto& io = ImGui::GetIO();
	IM_ASSERT(io.Fonts->IsBuilt() &&
	          "Font atlas not built! It is generally built by the renderer back-end. Missing call to renderer _NewFrame() function? e.g. ImGui_ImplOpenGL3_NewFrame().");

	int w, h, display_w, display_h;

	glfwGetWindowSize(g_Window, &w, &h);
	glfwGetFramebufferSize(g_Window, &display_w, &display_h);

	io.DisplaySize = ImVec2(static_cast<float>(w), static_cast<float>(h));
	if (w > 0 && h > 0)
		io.DisplayFramebufferScale = ImVec2(static_cast<float>(display_w) / w, static_cast<float>(display_h) / h);

	double current_time = glfwGetTime();
	io.DeltaTime        = g_Time > 0.0 ? static_cast<float>(current_time - g_Time) : static_cast<float>(1.0f / 60.0f);
	g_Time              = current_time;

	ImGui_ImplGlfw_UpdateMousePosAndButtons();
	ImGui_ImplGlfw_UpdateMouseCursor();
}
