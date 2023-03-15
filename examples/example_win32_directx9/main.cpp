#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"

#include <d3d9.h>
#pragma comment(lib,"d3d9.lib")
#include <d3dx9.h>
#pragma comment(lib,"d3dx9.lib")

#include <tchar.h>

#include "background_pic.h"
#include "blur.hpp"

#include "segue_font.h"
#include "ico_font.h"

float color_edit4[4] = { 1.00f, 1.00f, 1.00f, 1.000f };

float accent_color[4] = { 0.745f, 0.151f, 0.151f, 1.000f };

static int select_count = 0;

bool active = false;

float size_child = 0;

bool menu = false;
ImFont* ico = nullptr;
ImFont* ico_combo = nullptr;
ImFont* ico_button = nullptr;
ImFont* ico_grande = nullptr;
ImFont* segu = nullptr;
ImFont* default_segu = nullptr;
ImFont* bold_segu = nullptr;
LPDIRECT3D9              g_pD3D = NULL;
LPDIRECT3DDEVICE9        g_pd3dDevice = NULL;
D3DPRESENT_PARAMETERS    g_d3dpp = {};

bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void ResetDevice();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
IDirect3DTexture9* scene = nullptr;

int tab_count = 0, tabs = 0;

int main(int, char**)
{

    WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, L"ImGui Example", NULL };
    ::RegisterClassExW(&wc);
    HWND hwnd = ::CreateWindowW(wc.lpszClassName, L"Dear ImGui DirectX9 Example", WS_POPUP, 0, 0, 1920, 1060, NULL, NULL, wc.hInstance, NULL);

    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGuiContext& g = *GImGui;
    io.Fonts->AddFontFromMemoryTTF(&seguoe, sizeof seguoe, 22, NULL, io.Fonts->GetGlyphRangesCyrillic());

    default_segu = io.Fonts->AddFontFromMemoryTTF(&seguoe, sizeof seguoe, 22, NULL, io.Fonts->GetGlyphRangesCyrillic());

    segu = io.Fonts->AddFontFromMemoryTTF(&seguoe, sizeof seguoe, 40, NULL, io.Fonts->GetGlyphRangesCyrillic());

    bold_segu = io.Fonts->AddFontFromMemoryTTF(&bold_segue, sizeof bold_segue, 40, NULL, io.Fonts->GetGlyphRangesCyrillic());

    ico = io.Fonts->AddFontFromMemoryTTF(&icon, sizeof icon, 24, NULL, io.Fonts->GetGlyphRangesCyrillic());

    ico_combo = io.Fonts->AddFontFromMemoryTTF(&icon, sizeof icon, 19, NULL, io.Fonts->GetGlyphRangesCyrillic());

    ico_button = io.Fonts->AddFontFromMemoryTTF(&icon, sizeof icon, 25, NULL, io.Fonts->GetGlyphRangesCyrillic());

    ico_grande = io.Fonts->AddFontFromMemoryTTF(&icon, sizeof icon, 40, NULL, io.Fonts->GetGlyphRangesCyrillic());

    ImGui::StyleColorsDark();

    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX9_Init(g_pd3dDevice);


    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    bool done = false;
    while (!done)
    {

        MSG msg;
        while (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done)
            break;

        DWORD window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoBackground;

        ImGui_ImplDX9_NewFrame();
        ImGui_ImplWin32_NewFrame();

        ImGui::SetNextWindowPos(ImVec2(200, 200));

        ImGui::NewFrame();
        {

            ImGui::SetNextWindowSize(ImVec2(905, 624));

            ImGui::Begin("Hola!", &menu, window_flags);
            {
                ImVec2 P1, P2;
                ImDrawList* pDrawList;
                const auto& p = ImGui::GetWindowPos();
                const auto& pWindowDrawList = ImGui::GetWindowDrawList();
                const auto& pBackgroundDrawList = ImGui::GetBackgroundDrawList();
                const auto& pForegroundDrawList = ImGui::GetForegroundDrawList();

                DrawBackgroundBlur(pWindowDrawList, g_pd3dDevice);


                if (scene == nullptr) D3DXCreateTextureFromFileInMemoryEx(g_pd3dDevice, &background, sizeof(background), 1920, 1080, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &scene);

                ImGui::GetBackgroundDrawList()->AddImage(scene, ImVec2(0, 0), ImVec2(1920, 1080), ImVec2(0, 0), ImVec2(1, 1), ImColor(color_edit4[0], color_edit4[1], color_edit4[2], color_edit4[3]));

                pBackgroundDrawList->AddRectFilled(ImVec2(0.000f + p.x, 0.000f + p.y), ImVec2(905 + p.x, 624 + p.y), ImColor(9, 9, 9, 180), 10); // Background

                pWindowDrawList->AddRectFilled(ImVec2(189.000f + p.x, 75.000f + p.y), ImVec2(903 + p.x, 76 + p.y), ImColor(25, 25, 25, 180), 10); // bar line

                ImGui::SetCursorPos(ImVec2(800, 21));

                if (ImGui::OptButton("L", ImVec2(30, 30), false));

                ImGui::SameLine(840);

                if(ImGui::OptButton("B", ImVec2(30, 30), true));

                pWindowDrawList->AddRectFilled(ImVec2(0.000f + p.x, 0.000f + p.y), ImVec2(190 + p.x, 624 + p.y), ImGui::GetColorU32(ImGuiCol_ChildBg), 10, ImDrawFlags_RoundCornersLeft); // bar line

                    const int vtx_idx_1 = pWindowDrawList->VtxBuffer.Size;

                    pWindowDrawList->AddText(ico_grande, 40.f, ImVec2(20.000f + p.x, 20.000f + p.y), ImColor(0.60f, 0.60f, 0.60f, 0.70f), "U");

                    pWindowDrawList->AddText(bold_segu, 40.f, ImVec2(70.000f + p.x, 15.000f + p.y), ImColor(0.60f, 0.60f, 0.60f, 0.70f), "Evicted");

                    pWindowDrawList->AddRectFilled(ImVec2(70.000f + p.x, 51.000f + p.y), ImVec2(163 + p.x, 52 + p.y), ImColor(0.60f, 0.60f, 0.60f, 0.70f), 10); // bar line

                    const int vtx_idx_2 = pWindowDrawList->VtxBuffer.Size;

                    ImGui::ShadeVertsLinearColorGradientKeepAlpha(pWindowDrawList, vtx_idx_1, vtx_idx_2, ImVec2(0 + p.x, 0 + p.y), ImVec2(200 + p.x, 20 + p.y), ImColor(0.25f, 0.25f, 0.25f, 0.50f), ImColor(0.60f, 0.60f, 0.60f, 1.00f));

                    ImGui::SetCursorPosY(80);

                    if (ImGui::TabButton("P", "LegitBot", ImVec2(190, 40)) && tab_count != 0) {
                        tab_count = 0;
                        active = true;
                    }
                    if (ImGui::TabButton("N", "RageBot", ImVec2(190, 40)) && tab_count != 1) {
                        tab_count = 1;
                        active = true;
                    }

                    if (ImGui::TabButton("Q", "AntiAim", ImVec2(190, 40)) && tab_count != 2) {
                        tab_count = 2;
                        active = true;
                    }

                    if (ImGui::TabButton("I", "Visuals", ImVec2(190, 40)) && tab_count != 3) {
                        tab_count = 3;
                        active = true;
                    }

                    if (ImGui::TabButton("O", "Misc", ImVec2(190, 40)) && tab_count != 4) {
                        tab_count = 4;
                        active = true;
                    }

                    if (ImGui::TabButton("R", "PlayerList", ImVec2(190, 40)) && tab_count != 5) {
                        tab_count = 5;
                        active = true;
                    }

                    if (ImGui::TabButton("T", "Skins", ImVec2(190, 40)) && tab_count != 6) {
                        tab_count = 6;
                        active = true;
                    }

                    if (ImGui::TabButton("J", "Lua", ImVec2(190, 40)) && tab_count != 7) {
                        tab_count = 7;
                        active = true;
                    }

                    if (ImGui::TabButton("S", "Config", ImVec2(190, 40)) && tab_count != 8) {
                        tab_count = 8;
                        active = true;
                    }

                if (active) {
                    if (size_child <= 10) size_child += 1 / ImGui::GetIO().Framerate * 60.f;
                    else { active = false; tabs = tab_count; };
                }
                else {
                    if (size_child >= 0) size_child -= 1 / ImGui::GetIO().Framerate * 60.f;
                }

                pWindowDrawList->AddCircleFilled(ImVec2(57.000f + p.x, 570.000f + p.y), 25.000f, ImColor(10, 9, 10, 255), 30);

                pWindowDrawList->AddCircle(ImVec2(57.000f + p.x, 570.000f + p.y), 27.000f, ImColor(20, 19, 20, 255), 30, 4.000f);

                pWindowDrawList->AddText(segu, 40.f, ImVec2(51.000f + p.x, 548.000f + p.y), ImColor(0.60f, 0.60f, 0.60f, 0.50f), "?");



                const int vtx_idx_3 = pWindowDrawList->VtxBuffer.Size;

                pWindowDrawList->AddText(segu, 22.f, ImVec2(97.000f + p.x, 547.000f + p.y), ImColor(0.40f, 0.40f, 0.40f, 0.50f), "Benjy\nLifetime");

                const int vtx_idx_4 = pWindowDrawList->VtxBuffer.Size;

                ImGui::ShadeVertsLinearColorGradientKeepAlpha(pWindowDrawList, vtx_idx_3, vtx_idx_4, ImVec2(97.000f + p.x, 547.000f + p.y), ImVec2(200.000f + p.x, 567.000f + p.y), ImColor(0.35f, 0.35f, 0.35f, 0.50f), ImColor(0.90f, 0.90f, 0.90f, 1.00f));



                ImGui::SetCursorPos(ImVec2(203, 88 - size_child));

                switch (tabs) {

                case 0:

                    ImGui::BeginChild("Editor", ImVec2(339, 253), true); {

                        static bool test_0 = false;
                        ImGui::Checkbox("Checkbox_0", &test_0);

                        static bool test_1 = false;
                        ImGui::Checkbox("Checkbox_1", &test_1);

                        static int slider_test_0 = 50;
                        ImGui::SliderInt("Slider Intager", &slider_test_0, 1, 400);

                        static float slider_test_1 = 0;
                        ImGui::SliderFloat("Slider Float", &slider_test_1, 0.00f, 5.00f, "%.2f");

                    }ImGui::EndChild();

                    ImGui::SetCursorPos(ImVec2(555, 88 - size_child));

                    ImGui::BeginChild("Background", ImVec2(339, 210), true); {

                        ImGui::ColorEdit4("Background Color", (float*)&color_edit4, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);

                        ImGui::ColorEdit4("Widget Color", (float*)&accent_color, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);

                    }ImGui::EndChild();

                    ImGui::SetCursorPos(ImVec2(203, 353 - size_child));

                    ImGui::BeginChild("Aimbot", ImVec2(339, 258), true); {

                        static int selectedItem = 0;
                        static const char* items[]{ "Always", "Toggle" };
                        ImGui::Combo("Aimbot Mode", &selectedItem, items, IM_ARRAYSIZE(items), 5);

                        static char buf[64] = { "" };
                        ImGui::InputText("InputText", buf, 64);

                    }ImGui::EndChild();


                    ImGui::SetCursorPos(ImVec2(555, 313 - size_child));

                    ImGui::BeginChild("Keybinds", ImVec2(339, 298), true); {

                        static int k, m = 1;
                        ImGui::Keybind("Aimbot Keybind", &k, &m);

                    }ImGui::EndChild();

                    break;

                case 8:

                    ImGui::BeginChild("Configuration", ImVec2(339, 253), true); {


                    }ImGui::EndChild();

                    break;

                }
            }
            ImGui::End();
        }
        ImGui::EndFrame();
        g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
        g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
        g_pd3dDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
        D3DCOLOR clear_col_dx = D3DCOLOR_RGBA((int)(clear_color.x*clear_color.w*255.0f), (int)(clear_color.y*clear_color.w*255.0f), (int)(clear_color.z*clear_color.w*255.0f), (int)(clear_color.w*255.0f));
        g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_col_dx, 1.0f, 0);
        if (g_pd3dDevice->BeginScene() >= 0)
        {
            ImGui::Render();
            ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
            g_pd3dDevice->EndScene();
        }
        HRESULT result = g_pd3dDevice->Present(NULL, NULL, NULL, NULL);

        if (result == D3DERR_DEVICELOST && g_pd3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
            ResetDevice();
    }

    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);

    return 0;
}


bool CreateDeviceD3D(HWND hWnd)
{
    if ((g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
        return false;

    ZeroMemory(&g_d3dpp, sizeof(g_d3dpp));
    g_d3dpp.Windowed = TRUE;
    g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    g_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
    g_d3dpp.EnableAutoDepthStencil = TRUE;
    g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

    if (g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_d3dpp, &g_pd3dDevice) < 0)
        return false;

    return true;
}

void CleanupDeviceD3D()
{
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
    if (g_pD3D) { g_pD3D->Release(); g_pD3D = NULL; }
}

void ResetDevice()
{
    ImGui_ImplDX9_InvalidateDeviceObjects();
    HRESULT hr = g_pd3dDevice->Reset(&g_d3dpp);
    if (hr == D3DERR_INVALIDCALL)
        IM_ASSERT(0);
    ImGui_ImplDX9_CreateDeviceObjects();
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
        {
            g_d3dpp.BackBufferWidth = LOWORD(lParam);
            g_d3dpp.BackBufferHeight = HIWORD(lParam);
            ResetDevice();
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU)
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProc(hWnd, msg, wParam, lParam);
}
