#ifndef __MOONLIGHT_WINDOW_H__
#define __MOONLIGHT_WINDOW_H__
#include <MoonLight/Base/Topology.h>
#include <MoonLight/Base/Event.h>
#include <MoonLight/Base/Color.h>
#include <MoonLight/Base/Math.h>
#include <MoonLight/Base/Ptr.h>
#include <Windows.h>
#include <d3d11.h>
#include <string>
#include <queue>

namespace ml
{
	class VertexInputLayout;

	class Window
	{
	public:
		struct Config
		{
			Config()
			{
				Fullscreen = false;
				Sample.Count = 1;
				Sample.Quality = 0;
			}

			bool Fullscreen;
			DXGI_SAMPLE_DESC Sample;
		};

		enum class Style
		{
			None,
			Resizable,
			Default
		};

		Window();

		// create the window with given width and height, title and style
		void Create(DirectX::XMINT2 size, std::string name, Style style, Config info = Config());

		// if shw == false windows will hide the cursor
		void ShowCursor(bool shw);

		// set/get window size
		void Resize(const DirectX::XMINT2& size);
		DirectX::XMINT2 GetSize();
		float GetAspectRatio();

		// set/get window position
		void SetPosition(const DirectX::XMINT2& pos);
		DirectX::XMINT2 GetPosition();

		// handler for window messages
		bool GetEvent(ml::Event& e);
		void mProcessEvent(UINT msg, WPARAM wParam, LPARAM lParam);

		// is the window currently opened?
		inline bool IsOpen() { return mOpen; }

		// returns true if this window has focus
		inline bool HasFocus() { return GetForegroundWindow() == mWnd; }

		// class name
		static LPCWSTR Name;

	// D3D stuff
	public:
		// free memory
		void Destroy();

		// set/get vsync state
		inline void SetVSync(bool vs) { mVSync = vs; }
		inline bool GetVSync() { return mVSync; }

		// set/get clear color
		inline ml::Color GetClearColor() { return ml::Color(255*mClearColor[0], 255*mClearColor[1], 255*mClearColor[2], 255*mClearColor[3]); }
		void SetClearColor(ml::Color color);

		// clear back buffer
		void Clear();

		// clear depth/stencil texture
		void ClearDepthStencil(float depth, ml::UInt8 stencil);
		void ClearStencil(ml::UInt8 value);

		// set primitive topology
		void SetTopology(ml::Topology topology);

		// set id3d11inputlayout
		void SetInputLayout(ml::VertexInputLayout& inputLayout);

		// draw onto the screen
		void Draw(ml::UInt32 vertCount, ml::UInt32 start = 0);

		// display back buffer
		void Render();

		// execute compute shader
		void Compute(UInt32 x, UInt32 y, UInt32 z);

		// reset some states
		void RemoveBlendState();
		void RemoveRasterizerState();
		void RemoveDepthStencilState();
		void RemoveGeometryShader();
		void RemoveComputeShader();
		void RemoveUnorderedAccess(UInt32 slot = 0);

		// win32 and D3D stuff - only access from "Base" directory please
		inline HWND GetWindowHandle() { return mWnd; }
		inline D3D_FEATURE_LEVEL GetFeatureLevel() { return mFeatureLevel; }
		inline ID3D11Device* GetDevice() { return mDevice.Get(); }
		inline ID3D11DeviceContext* GetDeviceContext() { return mContext.Get(); }
		inline IDXGISwapChain* GetSwapChain() { return mSwapChain.Get(); }


	private:
		// handler for window messages
		static LRESULT CALLBACK mHandleEvent(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam);
		
		// resize the buffers
		void mResize();

		ml::Ptr<ID3D11Device> mDevice;
		ml::Ptr<ID3D11DeviceContext> mContext;
		ml::Ptr<IDXGISwapChain> mSwapChain;
		ml::Ptr<ID3D11RenderTargetView> mRTView;
		ml::Ptr<ID3D11Texture2D> mDepthTexture;
		ml::Ptr<ID3D11DepthStencilView> mDepthView;
		D3D_FEATURE_LEVEL mFeatureLevel;

		HWND mWnd;						// WIN32 window handle
		bool mCursorVisible;			// protection for ShowCursor() internal counter
		std::queue<ml::Event> mEvents;	// list of events that arent proccessed yet
		DirectX::XMINT2 mSize;			// window size
		bool mResizing;					// are we currently resizing
		bool mOpen;						// is the window currently opened
		bool mVSync;					// is vsync turned on?
		float mClearColor[4];			// clear color
		Config mInitialConfig;			// config that was used on startup
	};
}

#endif //__MOONLIGHT_WINDOW_H__