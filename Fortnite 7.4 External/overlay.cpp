#include "Overlay.h"
#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"


ID3D11ShaderResourceView* vtLogoImg = nullptr;
ID3D11ShaderResourceView* quishaImg = nullptr;
ID3D11ShaderResourceView* arIconImg = nullptr;
ID3D11ShaderResourceView* sgIconImg = nullptr;
ID3D11ShaderResourceView* smgIconImg = nullptr;
ID3D11ShaderResourceView* sniperIconImg = nullptr;
ID3D11ShaderResourceView* pistolIconImg = nullptr;
ID3D11ShaderResourceView* itemESPIconImg = nullptr;
ID3D11ShaderResourceView* playerESPIconImg = nullptr;






static ID3D11Device* g_pd3dDevice = nullptr;
static ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
static IDXGISwapChain* g_pSwapChain = nullptr;
static UINT                     g_ResizeWidth = 0, g_ResizeHeight = 0;
static ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;



bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

int TPS;

void loadImages() {
   



    //Quisha
    {
        int image_width = 0;
        int image_height = 0;
        unsigned char* image_data = stbi_load_from_memory(dataQuishaPNG, sizeof(dataQuishaPNG), &image_width, &image_height, NULL, 4);

        // Create texture
        D3D11_TEXTURE2D_DESC desc;
        ZeroMemory(&desc, sizeof(desc));
        desc.Width = image_width;
        desc.Height = image_height;
        desc.MipLevels = 1;
        desc.ArraySize = 1;
        desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        desc.SampleDesc.Count = 1;
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        desc.CPUAccessFlags = 0;

        ID3D11Texture2D* pTexture = NULL;
        D3D11_SUBRESOURCE_DATA subResource;
        subResource.pSysMem = image_data;
        subResource.SysMemPitch = desc.Width * 4;
        subResource.SysMemSlicePitch = 0;
        g_pd3dDevice->CreateTexture2D(&desc, &subResource, &pTexture);

        // Create texture view
        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
        ZeroMemory(&srvDesc, sizeof(srvDesc));
        srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Texture2D.MipLevels = desc.MipLevels;
        srvDesc.Texture2D.MostDetailedMip = 0;
        g_pd3dDevice->CreateShaderResourceView(pTexture, &srvDesc, &quishaImg);
        pTexture->Release();

        stbi_image_free(image_data);
    }

    //AR Icon
    {
        int image_width = 0;
        int image_height = 0;
        unsigned char* image_data = stbi_load_from_memory(ARIconData, sizeof(ARIconData), &image_width, &image_height, NULL, 4);

        // Create texture
        D3D11_TEXTURE2D_DESC desc;
        ZeroMemory(&desc, sizeof(desc));
        desc.Width = image_width;
        desc.Height = image_height;
        desc.MipLevels = 1;
        desc.ArraySize = 1;
        desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        desc.SampleDesc.Count = 1;
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        desc.CPUAccessFlags = 0;

        ID3D11Texture2D* pTexture = NULL;
        D3D11_SUBRESOURCE_DATA subResource;
        subResource.pSysMem = image_data;
        subResource.SysMemPitch = desc.Width * 4;
        subResource.SysMemSlicePitch = 0;
        g_pd3dDevice->CreateTexture2D(&desc, &subResource, &pTexture);

        // Create texture view
        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
        ZeroMemory(&srvDesc, sizeof(srvDesc));
        srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Texture2D.MipLevels = desc.MipLevels;
        srvDesc.Texture2D.MostDetailedMip = 0;
        g_pd3dDevice->CreateShaderResourceView(pTexture, &srvDesc, &arIconImg);
        pTexture->Release();


        stbi_image_free(image_data);
    }

    //Shotgun Icon
    {
        int image_width = 0;
        int image_height = 0;
        unsigned char* image_data = stbi_load_from_memory(SGIconData, sizeof(SGIconData), &image_width, &image_height, NULL, 4);

        // Create texture
        D3D11_TEXTURE2D_DESC desc;
        ZeroMemory(&desc, sizeof(desc));
        desc.Width = image_width;
        desc.Height = image_height;
        desc.MipLevels = 1;
        desc.ArraySize = 1;
        desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        desc.SampleDesc.Count = 1;
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        desc.CPUAccessFlags = 0;

        ID3D11Texture2D* pTexture = NULL;
        D3D11_SUBRESOURCE_DATA subResource;
        subResource.pSysMem = image_data;
        subResource.SysMemPitch = desc.Width * 4;
        subResource.SysMemSlicePitch = 0;
        g_pd3dDevice->CreateTexture2D(&desc, &subResource, &pTexture);

        // Create texture view
        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
        ZeroMemory(&srvDesc, sizeof(srvDesc));
        srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Texture2D.MipLevels = desc.MipLevels;
        srvDesc.Texture2D.MostDetailedMip = 0;
        g_pd3dDevice->CreateShaderResourceView(pTexture, &srvDesc, &sgIconImg);
        pTexture->Release();


        stbi_image_free(image_data);
    }

    //SMG Icon
    {
        int image_width = 0;
        int image_height = 0;
        unsigned char* image_data = stbi_load_from_memory(SMGIconData, sizeof(SMGIconData), &image_width, &image_height, NULL, 4);

        // Create texture
        D3D11_TEXTURE2D_DESC desc;
        ZeroMemory(&desc, sizeof(desc));
        desc.Width = image_width;
        desc.Height = image_height;
        desc.MipLevels = 1;
        desc.ArraySize = 1;
        desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        desc.SampleDesc.Count = 1;
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        desc.CPUAccessFlags = 0;

        ID3D11Texture2D* pTexture = NULL;
        D3D11_SUBRESOURCE_DATA subResource;
        subResource.pSysMem = image_data;
        subResource.SysMemPitch = desc.Width * 4;
        subResource.SysMemSlicePitch = 0;
        g_pd3dDevice->CreateTexture2D(&desc, &subResource, &pTexture);

        // Create texture view
        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
        ZeroMemory(&srvDesc, sizeof(srvDesc));
        srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Texture2D.MipLevels = desc.MipLevels;
        srvDesc.Texture2D.MostDetailedMip = 0;
        g_pd3dDevice->CreateShaderResourceView(pTexture, &srvDesc, &smgIconImg);
        pTexture->Release();


        stbi_image_free(image_data);
    }

    //Sniper Icon
    {
        int image_width = 0;
        int image_height = 0;
        unsigned char* image_data = stbi_load_from_memory(SniperIconData, sizeof(SniperIconData), &image_width, &image_height, NULL, 4);

        // Create texture
        D3D11_TEXTURE2D_DESC desc;
        ZeroMemory(&desc, sizeof(desc));
        desc.Width = image_width;
        desc.Height = image_height;
        desc.MipLevels = 1;
        desc.ArraySize = 1;
        desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        desc.SampleDesc.Count = 1;
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        desc.CPUAccessFlags = 0;

        ID3D11Texture2D* pTexture = NULL;
        D3D11_SUBRESOURCE_DATA subResource;
        subResource.pSysMem = image_data;
        subResource.SysMemPitch = desc.Width * 4;
        subResource.SysMemSlicePitch = 0;
        g_pd3dDevice->CreateTexture2D(&desc, &subResource, &pTexture);

        // Create texture view
        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
        ZeroMemory(&srvDesc, sizeof(srvDesc));
        srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Texture2D.MipLevels = desc.MipLevels;
        srvDesc.Texture2D.MostDetailedMip = 0;
        g_pd3dDevice->CreateShaderResourceView(pTexture, &srvDesc, &sniperIconImg);
        pTexture->Release();


        stbi_image_free(image_data);
    }

    //Pistol Icon
    {
        int image_width = 0;
        int image_height = 0;
        unsigned char* image_data = stbi_load_from_memory(PistolIconData, sizeof(PistolIconData), &image_width, &image_height, NULL, 4);

        // Create texture
        D3D11_TEXTURE2D_DESC desc;
        ZeroMemory(&desc, sizeof(desc));
        desc.Width = image_width;
        desc.Height = image_height;
        desc.MipLevels = 1;
        desc.ArraySize = 1;
        desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        desc.SampleDesc.Count = 1;
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        desc.CPUAccessFlags = 0;

        ID3D11Texture2D* pTexture = NULL;
        D3D11_SUBRESOURCE_DATA subResource;
        subResource.pSysMem = image_data;
        subResource.SysMemPitch = desc.Width * 4;
        subResource.SysMemSlicePitch = 0;
        g_pd3dDevice->CreateTexture2D(&desc, &subResource, &pTexture);

        // Create texture view
        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
        ZeroMemory(&srvDesc, sizeof(srvDesc));
        srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Texture2D.MipLevels = desc.MipLevels;
        srvDesc.Texture2D.MostDetailedMip = 0;
        g_pd3dDevice->CreateShaderResourceView(pTexture, &srvDesc, &pistolIconImg);
        pTexture->Release();


        stbi_image_free(image_data);
    }

    //Item ESP Icon
    {
        int image_width = 0;
        int image_height = 0;
        unsigned char* image_data = stbi_load_from_memory(itemESPIcon, sizeof(itemESPIcon), &image_width, &image_height, NULL, 4);

        // Create texture
        D3D11_TEXTURE2D_DESC desc;
        ZeroMemory(&desc, sizeof(desc));
        desc.Width = image_width;
        desc.Height = image_height;
        desc.MipLevels = 1;
        desc.ArraySize = 1;
        desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        desc.SampleDesc.Count = 1;
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        desc.CPUAccessFlags = 0;

        ID3D11Texture2D* pTexture = NULL;
        D3D11_SUBRESOURCE_DATA subResource;
        subResource.pSysMem = image_data;
        subResource.SysMemPitch = desc.Width * 4;
        subResource.SysMemSlicePitch = 0;
        g_pd3dDevice->CreateTexture2D(&desc, &subResource, &pTexture);

        // Create texture view
        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
        ZeroMemory(&srvDesc, sizeof(srvDesc));
        srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Texture2D.MipLevels = desc.MipLevels;
        srvDesc.Texture2D.MostDetailedMip = 0;
        g_pd3dDevice->CreateShaderResourceView(pTexture, &srvDesc, &itemESPIconImg);
        pTexture->Release();


        stbi_image_free(image_data);
    }

    //PlayerESP Icon
    {
        int image_width = 0;
        int image_height = 0;
        unsigned char* image_data = stbi_load_from_memory(playerESPIconData, sizeof(playerESPIconData), &image_width, &image_height, NULL, 4);

        // Create texture
        D3D11_TEXTURE2D_DESC desc;
        ZeroMemory(&desc, sizeof(desc));
        desc.Width = image_width;
        desc.Height = image_height;
        desc.MipLevels = 1;
        desc.ArraySize = 1;
        desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        desc.SampleDesc.Count = 1;
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        desc.CPUAccessFlags = 0;

        ID3D11Texture2D* pTexture = NULL;
        D3D11_SUBRESOURCE_DATA subResource;
        subResource.pSysMem = image_data;
        subResource.SysMemPitch = desc.Width * 4;
        subResource.SysMemSlicePitch = 0;
        g_pd3dDevice->CreateTexture2D(&desc, &subResource, &pTexture);

        // Create texture view
        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
        ZeroMemory(&srvDesc, sizeof(srvDesc));
        srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Texture2D.MipLevels = desc.MipLevels;
        srvDesc.Texture2D.MostDetailedMip = 0;
        g_pd3dDevice->CreateShaderResourceView(pTexture, &srvDesc, &playerESPIconImg);
        pTexture->Release();


        stbi_image_free(image_data);
    }

    g_pd3dDeviceContext->Flush();

}


void overlay::ApplyTheme() {

    ImGuiIO& io = ImGui::GetIO();


    ImGuiStyle* style = &ImGui::GetStyle();

    /*style->Colors[ImGuiCol_TitleBg] = ImColor(20, 20, 20);
    style->Colors[ImGuiCol_TitleBgActive] = ImColor(20, 20, 20);
    style->Colors[ImGuiCol_WindowBg] = ImColor(20, 20, 20);
    style->Colors[ImGuiCol_ChildBg] = ImColor(20, 20, 20);
    style->Colors[ImGuiCol_Text] = ImColor(255, 255, 255); */
    style->Colors[ImGuiCol_Button] = ImColor(255, 19, 250, 255);
    style->Colors[ImGuiCol_ButtonHovered] = ImColor(195, 9, 105);
    style->Colors[ImGuiCol_FrameBg] = ImColor(40, 40, 40);
    style->Colors[ImGuiCol_ButtonActive] = ImColor(94, 0, 92, 255);
    style->Colors[ImGuiCol_SliderGrab] = ImColor(255, 19, 250, 255);
    style->Colors[ImGuiCol_SliderGrabActive] = ImColor(94, 0, 92);
    style->Colors[ImGuiCol_CheckMark] = ImColor(255, 19, 250);
    style->Colors[ImGuiCol_FrameBgHovered] = ImColor(94, 0, 92, 255);
    style->Colors[ImGuiCol_FrameBgActive] = ImColor(195, 9, 105);
    style->Colors[ImGuiCol_Header] = ImColor(255, 19, 250, 255);
    style->Colors[ImGuiCol_HeaderHovered] = ImColor(94, 0, 92, 255);
    style->Colors[ImGuiCol_HeaderActive] = ImColor(195, 9, 105);
    style->Colors[ImGuiCol_ResizeGrip] = ImColor(0, 0, 0, 0);
    style->Colors[ImGuiCol_ResizeGripHovered] = ImColor(94, 0, 92);
    style->Colors[ImGuiCol_ResizeGripActive] = ImColor(195, 9, 105);

    style->ChildRounding = 4.f;
    style->WindowRounding = 4.f;
    style->TabRounding = 4.f;
    style->GrabRounding = 4.f;
    style->PopupRounding = 4.f;
    style->FrameRounding = 4.f;
    //->FramePadding = { 10.f,10.f };
    style->FrameBorderSize = 0.f;
};


void overlay::Render()
{

    WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"luhspotify", nullptr };
    ::RegisterClassExW(&wc);
    HWND hwnd = ::CreateWindowExW(WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_NOACTIVATE, wc.lpszClassName, L"spotifyfr", WS_POPUP, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), nullptr, nullptr, wc.hInstance, nullptr);

    SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 255, LWA_ALPHA);
    MARGINS margin = { -1 };
    DwmExtendFrameIntoClientArea(hwnd, &margin);

    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return;
    }

    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImFontConfig config;
    config.FontDataOwnedByAtlas = false;

    io.FontDefault = io.Fonts->AddFontFromMemoryTTF(ruda, std::size(ruda), 14.f, &config);
    io.IniFilename = nullptr;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    overlay::ApplyTheme();
    
   
    loadImages();

    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);


    ImVec4 clear_color = ImVec4(0.f, 0.f, 0.f, 0.f);

    bool done = false;
    while (!done)
    {

        if (!util::isActive()) {
            done = true;
        } 
        if (GetAsyncKeyState(VK_INSERT) & 1) overlay::Enabled = !overlay::Enabled;

        MSG msg;
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done)
            break;

        if (g_ResizeWidth != 0 && g_ResizeHeight != 0)
        {
            CleanupRenderTarget();
            g_pSwapChain->ResizeBuffers(0, g_ResizeWidth, g_ResizeHeight, DXGI_FORMAT_UNKNOWN, 0);
            g_ResizeWidth = g_ResizeHeight = 0;
            CreateRenderTarget();
        }



        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

       // overlay::menuDrawing();

        if (overlay::Enabled)
        {
            overlay::menuDrawing();

            SetWindowLong(hwnd, GWL_EXSTYLE, WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TOOLWINDOW);
        }
        else {
            SetWindowLong(hwnd, GWL_EXSTYLE, WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED | WS_EX_TOOLWINDOW);
        }
        mainLoop();
        TPS = util::measureTicksPerSecond();





        ImGui::Render();
        const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr);
        g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        g_pSwapChain->Present(syncInterval,0);
    

       
    }

    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
}

bool CreateDeviceD3D(HWND hWnd)
{
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res == DXGI_ERROR_UNSUPPORTED)
        res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res != S_OK)
        return false;

    CreateRenderTarget();
    return true;
}

void CleanupDeviceD3D()
{
    CleanupRenderTarget();
    if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = nullptr; }
    if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = nullptr; }
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
}

void CreateRenderTarget()
{
    ID3D11Texture2D* pBackBuffer;
    g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView);
    pBackBuffer->Release();
}

void CleanupRenderTarget()
{
    if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = nullptr; }
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (wParam == SIZE_MINIMIZED)
            return 0;
        g_ResizeWidth = (UINT)LOWORD(lParam);
        g_ResizeHeight = (UINT)HIWORD(lParam);
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU)
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}

