//#include <Windows.h>
//#include "d3d9.h"
//#include "d3dx9.h"
//
//#pragma comment (lib, "d3d9.lib")
//#pragma comment (lib, "d3dx9.lib")
//#pragma comment (lib, "winmm.lib")
//
//LPCTSTR WndClassName = L"DirectXFenster";
//HWND hwnd = NULL; // Fenster Sachbearbeiter
//
//const int Width = 800;
//const int Height = 450;
//
//// DIrectX Stuff
//IDirect3D9* d3d9 = NULL;
//IDirect3DDevice9* d3dev = NULL;
//
//IDirect3DVertexBuffer9* VertexBuffer = 0;
//IDirect3DIndexBuffer9* IndexBuffer = 0;
//
//IDirect3DTexture9* tex = 0;
//
//D3DLIGHT9 light;
//D3DMATERIAL9 material_white;
//
//struct Vertex
//{
//    float x, y, z;
//    float nx, ny, nz;
//    float u, v;
//
//};
//
//const DWORD VertexFVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;
//
//// Forward declarations
//bool InitializeWindow(HINSTANCE hInstance, int ShowWnd, int width, int height, bool windowed, D3DDEVTYPE devType, IDirect3DDevice9** d3dDevice);
//int messageloop();
//LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
//bool SetupScene();
//void CleanUp();
//bool RenderScene(float deltaTime);
//
//
//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPSTR lpCmdLine, int nShowCmd)
//{
//    // -----------------------------WINDOW INIT-------------------------
//    if (!InitializeWindow(hInstance, nShowCmd, Width, Height,
//        true, D3DDEVTYPE_HAL, &d3dev))
//    {
//        MessageBox(0, L"Window init error", L"Error", MB_OK);
//        return 1;
//    }
//
//    if (!SetupScene())
//    {
//        MessageBox(0, L"Scene Setup Failed", L"Error", MB_OK);
//        return 1;
//    }
//
//    messageloop();
//
//    CleanUp();
//
//    return 0;
//}
//
//bool InitializeWindow(HINSTANCE hInstance, int ShowWnd, int width, int height, bool windowed, D3DDEVTYPE devType, IDirect3DDevice9** d3dDevice)
//{
//    WNDCLASSEX wc;
//
//    wc.cbClsExtra = NULL;
//    wc.cbSize = sizeof(WNDCLASSEX);
//    wc.cbWndExtra = NULL;
//    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
//    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
//    wc.hIcon = LoadIcon(NULL, IDI_EXCLAMATION);
//    wc.hIconSm = LoadIcon(NULL, IDI_EXCLAMATION);
//    wc.hInstance = hInstance;
//    wc.lpfnWndProc = WndProc;
//    wc.lpszClassName = WndClassName;
//    wc.lpszMenuName = NULL;
//    wc.style = CS_HREDRAW | CS_VREDRAW;
//
//    if (!RegisterClassEx(&wc))
//    {
//        MessageBox(0, L"Register Class error", L"Error", MB_OK | MB_ICONERROR);
//        return false;
//    }
//
//    HWND hwnd = CreateWindowEx(
//        NULL, WndClassName, L"DirectSaft",
//        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height,
//        NULL, NULL, hInstance, NULL);
//
//    if (!hwnd)
//    {
//        MessageBox(0, L"Create Window error", L"Error", MB_OK | MB_ICONERROR);
//        return false;
//    }
//
//    ShowWindow(hwnd, ShowWnd);
//    UpdateWindow(hwnd);
//
//    // ------------- INIT DirectX
//
//    d3d9 = Direct3DCreate9(D3D_SDK_VERSION);
//    if (!d3d9)
//    {
//        MessageBox(0, L"D3D not initialized", L"Error", MB_OK | MB_ICONERROR);
//        return false;
//    }
//
//    D3DCAPS9 caps;   //Device capabilities
//
//    d3d9->GetDeviceCaps(D3DADAPTER_DEFAULT, devType, &caps);
//    int vertexProc = NULL;
//
//    if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
//        vertexProc = D3DCREATE_HARDWARE_VERTEXPROCESSING;
//    else
//        vertexProc = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
//
//    D3DPRESENT_PARAMETERS d3dpp;
//
//    d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
//    d3dpp.EnableAutoDepthStencil = true;
//    d3dpp.BackBufferCount = 1;
//    d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
//    d3dpp.BackBufferHeight = height;
//    d3dpp.BackBufferWidth = width;
//    d3dpp.Flags = NULL;
//    d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
//    d3dpp.hDeviceWindow = hwnd;
//    d3dpp.MultiSampleQuality = NULL;
//    d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
//    d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
//    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
//    d3dpp.Windowed = windowed;
//
//    d3d9->CreateDevice(D3DADAPTER_DEFAULT, devType, hwnd, vertexProc, &d3dpp, d3dDevice);
//
//    d3d9->Release();
//
//    return true;
//}
//
//int messageloop()
//{
//    MSG msg;
//    ZeroMemory(&msg, sizeof(msg));
//
//    static float lastTime = (float)timeGetTime();
//
//    while (true)
//    {
//        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
//        {
//            if (msg.message == WM_QUIT) break;
//
//            TranslateMessage(&msg);
//            DispatchMessage(&msg);
//        }
//
//        else
//        {
//            float currTime = (float)timeGetTime();
//            float deltaTime = (currTime - lastTime) * 0.001f;
//
//            //UPDATE 
//            RenderScene(deltaTime);
//
//            lastTime = currTime;
//            // HIER PASSIERT DER KRASSE DIRECTX KRAM !
//        }
//    }
//    if (d3d9) {
//        d3d9->Release(); d3d9 = NULL;
//    }
//
//    return 0;
//}
//
//LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
//{
//    switch (msg)
//    {
//    case WM_DESTROY:
//        PostQuitMessage(0);
//        return 0;
//    case WM_KEYDOWN:
//        if (wParam == VK_ESCAPE)
//        {
//            MessageBox(0, L"Wirklich?", L"ENDE", MB_YESNO | MB_ICONQUESTION);
//            DestroyWindow(hwnd);
//        }
//        return 0;
//    }
//
//
//    return DefWindowProc(hwnd, msg, wParam, lParam);
//}
//
//bool SetupScene()
//{
//    d3dev->CreateVertexBuffer(
//        sizeof(Vertex) * 5, D3DUSAGE_WRITEONLY, VertexFVF, D3DPOOL_MANAGED,
//        &VertexBuffer, 0);
//
//    d3dev->CreateIndexBuffer(
//        sizeof(WORD) * 9, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED,
//        &IndexBuffer, 0);
//
//
//    //------------------------------- VERTEX BUFFER
//    Vertex* vertices;
//
//    VertexBuffer->Lock(0, 0, (void**)&vertices, 0);
//    // LEFT DOWN
//    vertices[0].x = -0.5f;
//    vertices[0].y = -0.5f;
//    vertices[0].z = 0.0f;
//    vertices[0].nx = 0;
//    vertices[0].ny = 0;
//    vertices[0].nz = -1;
//    vertices[0].u = 0.20f;
//    vertices[0].v = 1;
//    // Top LEFT
//    vertices[1].x = -0.5f;
//    vertices[1].y = 0.5f;
//    vertices[1].z = 0.0f;
//    vertices[1].nx = 0;
//    vertices[1].ny = 0;
//    vertices[1].nz = -1;
//    vertices[1].u = 0.20f;
//    vertices[1].v = 0.25f;
//    // RIGHT UP
//    vertices[2].x = 0.5f;
//    vertices[2].y = 0.5f;
//    vertices[2].z = 0.0f;
//    vertices[2].nx = 0;
//    vertices[2].ny = 0;
//    vertices[2].nz = -1;
//    vertices[2].u = 0.75f;
//    vertices[2].v = 0.25f;
//    // RIGHT DOWN
//    vertices[3].x = +0.5f;
//    vertices[3].y = -0.5f;
//    vertices[3].z = 0.0f;
//    vertices[3].nx = 0;
//    vertices[3].ny = 0;
//    vertices[3].nz = -1;
//    vertices[3].u = 0.75f;
//    vertices[3].v = 1;
//    //DACH
//    vertices[4].x = 0.0f;
//    vertices[4].y = 0.8f;
//    vertices[4].z = 0.0f;
//    vertices[4].nx = 0;
//    vertices[4].ny = 0;
//    vertices[4].nz = -1;
//    vertices[4].u = 0.5f;
//    vertices[4].v = 0;
//
//    VertexBuffer->Unlock();
//
//    // ------------------ INDEX BUFFER
//
//    WORD* indices = 0;
//
//    IndexBuffer->Lock(0, 0, (void**)&indices, 0);
//
//    indices[0] = 0; indices[1] = 1; indices[2] = 2;
//    indices[3] = 0; indices[4] = 2; indices[5] = 3;
//    indices[6] = 1; indices[7] = 4; indices[8] = 2;
//
//    IndexBuffer->Unlock();
//
//    // ------------- TEXTURE
//
//    D3DXCreateTextureFromFile(d3dev, L"texture.png", &tex);
//
//    d3dev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
//    d3dev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
//    d3dev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
//
//    // ------------- LIGHT
//
//    ZeroMemory(&light, sizeof(light));
//    light.Type = D3DLIGHT_DIRECTIONAL;
//    light.Diffuse = D3DXCOLOR(1, 1, 1, 1);
//    light.Ambient = D3DXCOLOR(1, 1, 1, 1) * 0.2f;
//    light.Specular = D3DXCOLOR(1, 1, 1, 1) * 0.1f;
//    light.Direction = D3DXVECTOR3(1, -0.2f, 0.7f);
//
//    // ------------ MATERIAL
//
//    ZeroMemory(&material_white, sizeof(material_white));
//    material_white.Ambient = D3DXCOLOR(1, 1, 1, 1);
//    material_white.Diffuse = D3DXCOLOR(1, 1, 1, 1);
//    material_white.Emissive = D3DXCOLOR(0, 0, 0, 1);
//    material_white.Power = 4;
//    material_white.Specular = D3DXCOLOR(1, 1, 1, 1);
//    // ------------- CAMERA
//
//    D3DXVECTOR3 eye(0.0f, 0.0f, -1.5f);
//    D3DXVECTOR3 at(0, 0, 0);
//    D3DXVECTOR3 up(0, 1, 0);
//
//    D3DXMATRIX View;
//    D3DXMatrixLookAtLH(&View, &eye, &at, &up);
//
//    d3dev->SetTransform(D3DTS_VIEW, &View);
//
//    // ----- PROJECTION
//
//    D3DXMATRIX Projection;
//    D3DXMatrixPerspectiveFovLH(
//        &Projection,
//        D3DX_PI * 0.5f,                 // FOV in rad
//        (float)Width / (float)Height,   // Aspect Ratio
//        1,                              // NearPlane
//        1000);                          // Far Plane
//
//    d3dev->SetTransform(D3DTS_PROJECTION, &Projection);
//
//    // --------- RENDER STATE
//    d3dev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
//    d3dev->SetRenderState(D3DRS_LIGHTING, true);
//    d3dev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
//
//    d3dev->SetLight(0, &light);
//    d3dev->LightEnable(0, true);
//
//    return true;
//}
//
//void CleanUp()
//{
//    VertexBuffer->Release();
//    IndexBuffer->Release();
//    tex->Release();
//}
//
//bool RenderScene(float deltaTime)
//{
//    if (d3dev)
//    {
//        D3DXMATRIX world;
//        D3DXMATRIX translate;
//        D3DXMATRIX rotZ;
//        D3DXMATRIX rotY;
//        D3DXMATRIX scale;
//
//        static float X = 0;
//        static float R = 0;
//
//        if (GetAsyncKeyState(VK_LEFT) & 0x8000f) X -= 2 * deltaTime;
//        if (GetAsyncKeyState(VK_RIGHT) & 0x8000f) X += 2 * deltaTime;
//
//        R += deltaTime; if (R > 6.282f) R = 0;
//
//        D3DXMatrixTranslation(&translate, X, 0, 0);
//        D3DXMatrixRotationZ(&rotZ, R);
//        D3DXMatrixRotationY(&rotY, R);
//        D3DXMatrixScaling(&scale, R, R, R);
//
//        world = rotY * translate;
//
//        d3dev->SetTransform(D3DTS_WORLD, &world);
//
//        d3dev->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00FF0044, 1.0f, 0);
//
//        d3dev->BeginScene();
//        d3dev->SetTexture(0, tex);
//        d3dev->SetMaterial(&material_white);
//        d3dev->SetStreamSource(0, VertexBuffer, 0, sizeof(Vertex));
//        d3dev->SetIndices(IndexBuffer);
//        d3dev->SetFVF(VertexFVF);
//        d3dev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 5, 0, 3);
//        d3dev->EndScene();
//
//        d3dev->Present(0, 0, 0, 0);
//    }
//    return true;
//}
//
////       \:.             . :/
////        \``._________.'' /
////         \              /
////  .--.--, / .':.   .':. \            MOIN
///// __:  /  | '::' . '::' |
/////     /   |`   .._.   .'|
/////    /    |.'         '.|
/////___-_- , |.\  \   /  /.|
//////        |''\.;   ;,/ '|
////`= =      | : =     = : |
////           `.         .'
////            :-._____.-:
////            `''     `''