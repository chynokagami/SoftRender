#include"application.h"
#include<windowsx.h>

Application* Application::mInstance = nullptr;
Application* Application::getInstance() {
	if (mInstance == nullptr) {
		mInstance = new Application();
	}

	return mInstance;
}

Application::Application() {}
Application::~Application() {}

LRESULT CALLBACK Wndproc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	Application::getInstance()->handleMessage(hWnd, message, wParam, lParam);
	return(DefWindowProc(hWnd, message, wParam, lParam));
}

bool Application::initApplication(HINSTANCE hInstance, const uint32_t& width, const uint32_t& height) {
	mWidth = width;
	mHeight = height;

	registerWindowClass(hInstance);

	if (!createWindow(hInstance)) {
		return false;
	}

	mhDC = GetDC(mHwnd);
	mCanvasDC = CreateCompatibleDC(mhDC);

	BITMAPINFO  bmpInfo{};
	bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpInfo.bmiHeader.biWidth = mWidth;
	bmpInfo.bmiHeader.biHeight = mHeight;
	bmpInfo.bmiHeader.biPlanes = 1;
	bmpInfo.bmiHeader.biBitCount = 32;
	bmpInfo.bmiHeader.biCompression = BI_RGB; //ʵ���ϴ洢��ʽΪbgra


	mhBmp = CreateDIBSection(mCanvasDC, &bmpInfo, DIB_RGB_COLORS, (void**)&mCanvasBuffer, 0, 0);


	SelectObject(mCanvasDC, mhBmp);

	memset(mCanvasBuffer, 0, mWidth * mHeight * 4);

	return true;
}

ATOM Application::registerWindowClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wndClass;

	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = Wndproc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = hInstance;
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = mWindowClassName;
	wndClass.hIconSm = LoadIcon(NULL, IDI_WINLOGO);

	return RegisterClassExW(&wndClass);
}

BOOL Application::createWindow(HINSTANCE hInstance)
{
	mWindowInst = hInstance;



	auto dwExStyle = WS_EX_APPWINDOW;
	auto dwStyle = WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN; 


	RECT windowRect;
	windowRect.left = 0L;
	windowRect.top = 0L;
	windowRect.right = (long)mWidth;
	windowRect.bottom = (long)mHeight;
	AdjustWindowRectEx(&windowRect, dwStyle, FALSE, dwExStyle);

	mHwnd = CreateWindowW(
		mWindowClassName,
		(LPCWSTR)"GraphicLearning",
		dwStyle,
		0,
		0,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		nullptr,
		nullptr,
		hInstance,
		nullptr);


	if (!mHwnd)
	{
		return FALSE;
	}

	ShowWindow(mHwnd, true);
	UpdateWindow(mHwnd);

	return TRUE;
}

bool Application::peekMessage() {
	MSG msg;
	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return mAlive;
}

void Application::handleMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message)
	{
	case WM_KEYDOWN: {
		if (mCamera) {
			mCamera->onKeyDown(wParam);
		}
		break;
	}
	case WM_KEYUP: {
		if (mCamera) {
			mCamera->onKeyUp(wParam);
		}
		break;
	}
	case WM_RBUTTONDOWN: {
		if (mCamera) {
			mCamera->onRMouseDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		}
		break;
	}
	case WM_RBUTTONUP: {
		if (mCamera) {
			mCamera->onRMouseUp(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		}
		break;
	}

	case WM_MOUSEMOVE: {
		if (mCamera) {
			mCamera->onMouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		}
		break;
	}
	case WM_CLOSE: {
		DestroyWindow(hWnd);
		break;
	}
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY: {
		PostQuitMessage(0);
		mAlive = false;
		break;
	}
	}
}

void Application::show() {
	BitBlt(mhDC, 0, 0, mWidth, mHeight, mCanvasDC, 0, 0, SRCCOPY);
}

void Application::setCamera(Camera* camera) {
	mCamera = camera;
}
