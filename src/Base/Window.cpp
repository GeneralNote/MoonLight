#include <MoonLight/Base/Window.h>
#include <MoonLight/Base/VertexInputLayout.h>
#include <assert.h>
#include <array>

namespace ml
{
	LPCWSTR Window::Name = L"mlWindow";

	Window::Window() :
		mWnd(NULL),
		mCursorVisible(true),
		mSize(0, 0),
		mResizing(false),
		mOpen(false),
		mVSync(false)
	{
	}

	void Window::Create(DirectX::XMINT2 size, std::string name, ml::Window::Style style, Config info)
	{
		assert(size.x > 0);
		assert(size.y > 0);

		mInitialConfig = info;

		SetProcessDPIAware();

		// convert std::string to std::wstring
		std::wstring title(name.begin(), name.end());

		// register window class
		WNDCLASS wndClass = { 0 };
		wndClass.lpfnWndProc = &Window::mHandleEvent;
		wndClass.hInstance = NULL;
		wndClass.lpszClassName = Window::Name;
		wndClass.hCursor = LoadCursorW(NULL, IDC_ARROW);
		RegisterClassW(&wndClass);

		// generate window style
		DWORD winStyle = WS_VISIBLE;
		if (style == ml::Window::Style::None) winStyle |= WS_POPUP;
		else if (style == ml::Window::Style::Default) winStyle |= WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;
		else if (style == ml::Window::Style::Resizable) winStyle |= WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU | WS_THICKFRAME | WS_MAXIMIZEBOX;

		// position the window in the center
		int wndX = (GetSystemMetrics(SM_CXSCREEN) - size.x) / 2;
		int wndY = (GetSystemMetrics(SM_CYSCREEN) - size.y) / 2;

		// create the window
		mWnd = CreateWindowExW(0L, Window::Name, title.c_str(), winStyle,
							   wndX, wndY, size.x, size.y,
							   NULL, NULL, NULL, this);

		// we "opened" the window
		mOpen = true;
		mSize = size;


		// D3D11 flags
		UINT createFlags = 0;
	#ifdef _DEBUG
		createFlags |= D3D11_CREATE_DEVICE_DEBUG;
	#endif

		// D3D feature levels
		D3D_FEATURE_LEVEL featureLevels[] = {
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0
		};

		// swap chain description
		DXGI_SWAP_CHAIN_DESC swapDesc = { 0 };
		swapDesc.BufferCount = 1;
		swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapDesc.BufferDesc.Height = size.y;
		swapDesc.BufferDesc.RefreshRate.Numerator = 60;
		swapDesc.BufferDesc.RefreshRate.Denominator = 1;
		swapDesc.BufferDesc.Width = size.x;
		swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapDesc.OutputWindow = mWnd;
		swapDesc.SampleDesc = info.Sample;
		swapDesc.Windowed = !info.Fullscreen;

		// set up D3D11
		HRESULT hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, NULL, createFlags,
			featureLevels, _ARRAYSIZE(featureLevels), D3D11_SDK_VERSION,
			&swapDesc, mSwapChain.GetAddressOf(), mDevice.GetAddressOf(),
			&mFeatureLevel, mContext.GetAddressOf());

		if (FAILED(hr)) {
			// TODO: implement ml::Logger class
			printf("Failed to create D3D device.\n");
		} else {
			// TODO: mLog->Add("[D3D] Succeded creating D3D device.\n");
		}


		// create render target
		ml::Ptr<ID3D11Texture2D> rtTexture;
		
		mSwapChain->GetBuffer(0, IID_ID3D11Texture2D, (LPVOID*)rtTexture.GetAddressOf());
		mDevice->CreateRenderTargetView(rtTexture.Get(), nullptr, mRTView.GetAddressOf());


		// create depth/stencil resource and view
		D3D11_TEXTURE2D_DESC depthTexDesc = { 0 };
		depthTexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthTexDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthTexDesc.Height = size.y;
		depthTexDesc.ArraySize = 1;
		depthTexDesc.MipLevels = 1;
		depthTexDesc.Usage = D3D11_USAGE_DEFAULT;
		depthTexDesc.Width = size.x;
		depthTexDesc.SampleDesc = info.Sample;

		mDevice->CreateTexture2D(&depthTexDesc, nullptr, mDepthTexture.GetAddressOf());
		mDevice->CreateDepthStencilView(mDepthTexture.Get(), nullptr, mDepthView.GetAddressOf());


		// bind render target
		mContext->OMSetRenderTargets(1, mRTView.GetAddressOf(), mDepthView.Get());


		// set viewport
		D3D11_VIEWPORT view;
		view.TopLeftX = view.TopLeftY = 0;
		view.Width = size.x;
		view.Height = size.y;
		view.MaxDepth = 1.0f;
		view.MinDepth = 0.0f;

		mContext->RSSetViewports(1, &view);
	}

	void Window::ShowCursor(bool shw)
	{
		if (shw != mCursorVisible) {
			mCursorVisible = shw;
			ShowCursor(shw);
		}
	}

	void Window::Resize(const DirectX::XMINT2& size)
	{
		assert(size.x > 0);
		assert(size.y > 0);

		RECT rect = { 0, 0, size.x, size.y };
		AdjustWindowRect(&rect, GetWindowLong(mWnd, GWL_STYLE), false);

		SetWindowPos(mWnd, NULL, 0, 0,
					 rect.right - rect.left, rect.bottom - rect.top,
					 SWP_NOMOVE | SWP_NOZORDER);

		mResize();
	}

	DirectX::XMINT2 Window::GetSize()
	{
		RECT bounds;
		GetClientRect(mWnd, &bounds);

		return DirectX::XMINT2(bounds.right - bounds.left, bounds.bottom - bounds.top);
	}

	float Window::GetAspectRatio()
	{
		DirectX::XMINT2 size = this->GetSize();
		return (float)size.x/size.y;
	}

	void Window::SetPosition(const DirectX::XMINT2& pos)
	{
		SetWindowPos(mWnd, NULL, pos.x, pos.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	}

	DirectX::XMINT2 Window::GetPosition()
	{
		RECT bounds;
		GetWindowRect(mWnd, &bounds);

		return DirectX::XMINT2(bounds.left, bounds.top);
	}

	bool Window::GetEvent(ml::Event& e)
	{
		// check if any event happend
		MSG msg;
		while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}

		if (!mEvents.empty()) {
			e = mEvents.front();
			mEvents.pop();
			return true;
		}

		return false;
	}

	void Window::mProcessEvent(UINT msg, WPARAM wParam, LPARAM lParam)
	{
		// Don't process any message until window is created
		if (mWnd == NULL)
			return;

		switch (msg) {
			// window close event
			case WM_CLOSE:
			{
				mOpen = false;
				ml::Event event;
				event.Type = ml::EventType::WindowClose;
				mEvents.push(event);

				break;
			}

			// start resizing
			case WM_ENTERSIZEMOVE:
			{
				mResizing = true;
				break;
			}

			// resize event
			case WM_SIZE:
			{
				if (wParam != SIZE_MINIMIZED && !mResizing && mSize != GetSize()) {
					mSize = GetSize();

					ml::Event event;
					event.Type = ml::EventType::WindowResize;
					event.WindowSize = mSize;
					mEvents.push(event);

					mResize();
				}

				break;
			}

			// stop resizing
			case WM_EXITSIZEMOVE:
			{
				mResizing = false;

				// only send this event if the size changed
				if (mSize != GetSize()) {
					// update the last window size
					mSize = GetSize();

					// push a resize event
					Event event;
					event.Type = EventType::WindowResize;
					event.WindowSize = mSize;
					mEvents.push(event);

					mResize();
				}

				break;
			}

			// gained focus event
			case WM_SETFOCUS:
			{
				Event event;
				event.Type = EventType::WindowGainedFocus;
				mEvents.push(event);

				break;
			}

			// lost focus event
			case WM_KILLFOCUS:
			{
				Event event;
				event.Type = EventType::WindowLostFocus;
				mEvents.push(event);

				break;
			}

			// text event
			case WM_CHAR:
			{
				Event event;
				event.Type = EventType::TextEnter;
				event.TextCode = wParam;
				mEvents.push(event);

				break;
			}

			// key press event
			case WM_KEYDOWN:
			case WM_SYSKEYDOWN:
			{
				Event event;
				event.Type = EventType::KeyPress;
				event.Keyboard.Control = HIWORD(GetKeyState(VK_CONTROL));
				event.Keyboard.Shift = HIWORD(GetKeyState(VK_SHIFT));
				event.Keyboard.Alt = HIWORD(GetKeyState(VK_MENU));
				event.Keyboard.VK = wParam;
				mEvents.push(event);

				break;
			}

			// key release event
			case WM_KEYUP:
			case WM_SYSKEYUP:
			{
				Event event;
				event.Type = EventType::KeyRelease;
				event.Keyboard.Control = HIWORD(GetKeyState(VK_CONTROL));
				event.Keyboard.Shift = HIWORD(GetKeyState(VK_SHIFT));
				event.Keyboard.Alt = HIWORD(GetKeyState(VK_MENU));
				event.Keyboard.VK = wParam;
				mEvents.push(event);

				break;
			}

			// mouse move event
			case WM_MOUSEMOVE:
			{
				// capture the mouse outside only if a button was pressed
				if ((wParam & (MK_LBUTTON | MK_MBUTTON | MK_RBUTTON | MK_XBUTTON1 | MK_XBUTTON2)) == 0 && GetCapture() == mWnd)
					ReleaseCapture();
				else if (GetCapture() != mWnd)
					SetCapture(mWnd);

				Event event;
				event.Type = EventType::MouseMove;
				event.MouseMove = DirectX::XMINT2(LOWORD(lParam), HIWORD(lParam));
				mEvents.push(event);

				break;
			}

			// mouse wheel event
			case WM_MOUSEWHEEL:
			{
				POINT mpos;
				mpos.x = LOWORD(lParam);
				mpos.y = HIWORD(lParam);
				ScreenToClient(mWnd, &mpos);

				ml::SInt16 delta = HIWORD(wParam);

				Event event;
				event.Type = EventType::Scroll;
				event.MouseWheel.Delta = delta / (float)WHEEL_DELTA;
				event.MouseWheel.Position = DirectX::XMINT2(mpos.x, mpos.y);
				mEvents.push(event);

				break;
			}

			// Mouse left button down event
			case WM_LBUTTONDOWN:
			case WM_RBUTTONDOWN:
			case WM_MBUTTONDOWN:
			case WM_XBUTTONDOWN:
			{
				Event event;
				event.Type = EventType::MouseButtonPress;

				if (msg == WM_LBUTTONDOWN) event.MouseButton.VK = VK_LBUTTON;
				else if (msg == WM_RBUTTONDOWN) event.MouseButton.VK = VK_RBUTTON;
				else if (msg == WM_MBUTTONDOWN) event.MouseButton.VK = VK_MBUTTON;
				else if (msg == WM_XBUTTONDOWN) event.MouseButton.VK = (wParam & MK_XBUTTON1) ? VK_XBUTTON1 : VK_XBUTTON2;

				event.MouseButton.Position = DirectX::XMINT2(LOWORD(lParam), HIWORD(lParam));
				mEvents.push(event);

				break;
			}

			// Mouse left button up event
			case WM_LBUTTONUP:
			case WM_RBUTTONUP:
			case WM_MBUTTONUP:
			case WM_XBUTTONUP:
			{
				Event event;
				event.Type = EventType::MouseButtonRelease;

				if (msg == WM_LBUTTONDOWN) event.MouseButton.VK = VK_LBUTTON;
				else if (msg == WM_RBUTTONDOWN) event.MouseButton.VK = VK_RBUTTON;
				else if (msg == WM_MBUTTONDOWN) event.MouseButton.VK = VK_MBUTTON;
				else if (msg == WM_XBUTTONDOWN) event.MouseButton.VK = (wParam & MK_XBUTTON1) ? VK_XBUTTON1 : VK_XBUTTON2;

				event.MouseButton.Position = DirectX::XMINT2(LOWORD(lParam), HIWORD(lParam));
				mEvents.push(event);

				break;
			}
		}
	}

	void Window::Destroy()
	{
		// release DirectX objects
		mDevice.Reset();
		mContext.Reset();
		mSwapChain.Reset();
		mRTView.Reset();
		mDepthTexture.Reset();
		mDepthView.Reset();

		// destroy the window
		if (mWnd)
			DestroyWindow(mWnd);

		// unregister the class name
		UnregisterClassW(Window::Name, NULL);
	}

	void Window::SetClearColor(ml::Color color)
	{
		mClearColor[0] = color.R * 1.0f/255;
		mClearColor[1] = color.G * 1.0f / 255;
		mClearColor[2] = color.B * 1.0f / 255;
		mClearColor[3] = color.A * 1.0f / 255;
	}

	void Window::Clear()
	{
		mContext->ClearRenderTargetView(mRTView.Get(), mClearColor);
	}

	void Window::ClearDepthStencil(float depth, ml::UInt8 stencil)
	{
		mContext->ClearDepthStencilView(mDepthView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, depth, stencil);
	}

	void Window::ClearStencil(ml::UInt8 value)
	{
		mContext->ClearDepthStencilView(mDepthView.Get(), D3D11_CLEAR_STENCIL, 1.0f, value);
	}

	void Window::SetTopology(ml::Topology topology)
	{
		mContext->IASetPrimitiveTopology((D3D11_PRIMITIVE_TOPOLOGY)topology);
	}

	void Window::SetInputLayout(ml::VertexInputLayout & inputLayout)
	{
		mContext->IASetInputLayout(inputLayout.GetInputLayout());
	}

	void Window::Draw(ml::UInt32 vertCount, ml::UInt32 start)
	{
		mContext->Draw(vertCount, start);
	}

	void Window::Render()
	{
		mSwapChain->Present(mVSync, 0);
	}

	void Window::Compute(UInt32 x, UInt32 y, UInt32 z)
	{
		mContext->Dispatch(x, y, z);
	}

	void Window::RemoveRasterizerState()
	{
		mContext->RSSetState(nullptr);
	}

	void Window::RemoveDepthStencilState()
	{
		mContext->OMSetDepthStencilState(nullptr, 0);
	}

	void Window::RemoveGeometryShader()
	{
		mContext->GSSetShader(nullptr, nullptr, 0);
	}

	void Window::RemoveComputeShader()
	{
		mContext->CSSetShader(nullptr, nullptr, 0);
	}

	void Window::RemoveUnorderedAccess(UInt32 slot)
	{
		ID3D11UnorderedAccessView* UAV_NULL = NULL;
		mContext->CSSetUnorderedAccessViews(slot, 1, &UAV_NULL, 0);
	}

	void Window::RemoveBlendState()
	{
		static const float factor[4] = { 0, 0, 0, 0 };
		mContext->OMSetBlendState(nullptr, factor, 0xFFFFFFFF);
	}

	LRESULT CALLBACK Window::mHandleEvent(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		// set the window as user data
		if (msg == WM_CREATE) {
			LONG_PTR window = (LONG_PTR)((CREATESTRUCT*)lParam)->lpCreateParams;
			SetWindowLongPtrW(hwnd, GWLP_USERDATA, window);
		}

		// get the window
		Window* wnd = hwnd ? (Window*)(GetWindowLongPtr(hwnd, GWLP_USERDATA)) : NULL;

		// Forward the event to the appropriate function
		if (wnd)
			wnd->mProcessEvent(msg, wParam, lParam); // TODO: build for x64

		if (msg == WM_SYSKEYDOWN || msg == WM_SYSKEYUP)
			return true;

		return DefWindowProcW(hwnd, msg, wParam, lParam);
	}

	void Window::mResize()
	{
		mRTView.Reset();
		mDepthView.Reset();
		mDepthTexture.Reset();

		// resize swap chain and create rt view
		mSwapChain->ResizeBuffers(1, mSize.x, mSize.y, DXGI_FORMAT_R8G8B8A8_UNORM, 0);

		ml::Ptr<ID3D11Texture2D> backBuffer;
		mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)backBuffer.GetAddressOf());
		mDevice->CreateRenderTargetView(backBuffer.Get(), 0, mRTView.GetAddressOf());

		// create depth stencil texture and view
		D3D11_TEXTURE2D_DESC depthTexDesc = { 0 };
		depthTexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthTexDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthTexDesc.Height = mSize.y;
		depthTexDesc.ArraySize = 1;
		depthTexDesc.SampleDesc = mInitialConfig.Sample;
		depthTexDesc.MipLevels = 1;
		depthTexDesc.Usage = D3D11_USAGE_DEFAULT;
		depthTexDesc.Width = mSize.x;

		mDevice->CreateTexture2D(&depthTexDesc, 0, mDepthTexture.GetAddressOf());
		mDevice->CreateDepthStencilView(mDepthTexture.Get(), 0, mDepthView.GetAddressOf());

		// bind
		mContext->OMSetRenderTargets(1, mRTView.GetAddressOf(), mDepthView.Get());

		// set viewport
		D3D11_VIEWPORT view;
		view.Height = mSize.y;
		view.Width = mSize.x;
		view.TopLeftX = view.TopLeftY = 0;
		view.MinDepth = 0.0f;
		view.MaxDepth = 1.0f;
		mContext->RSSetViewports(1, &view);
	}
}
