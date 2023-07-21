#include "pch.h"
#include "RenderingEngineProjMain.h"
#include "Common\DirectXHelper.h"

using namespace RenderingEngineProj;
using namespace Windows::Foundation;
using namespace Windows::System::Threading;
using namespace Concurrency;

// Loads and initializes application assets when the application is loaded.
RenderingEngineProjMain::RenderingEngineProjMain(const std::shared_ptr<DX::DeviceResources>& deviceResources) :
	m_deviceResources(deviceResources), m_pointerLocationX(0.0f)
{
	// Register to be notified if the Device is lost or recreated
	m_deviceResources->RegisterDeviceNotify(this);

	// TODO: Replace this with your app's content initialization.
	m_sceneRenderer = std::unique_ptr<Sample3DSceneRenderer>(new Sample3DSceneRenderer(m_deviceResources));

	m_fpsTextRenderer = std::unique_ptr<SampleFpsTextRenderer>(new SampleFpsTextRenderer(m_deviceResources));

	// TODO: Change the timer settings if you want something other than the default variable timestep mode.
	// e.g. for 60 FPS fixed timestep update logic, call:
	/*
	m_timer.SetFixedTimeStep(true);
	m_timer.SetTargetElapsedSeconds(1.0 / 60);
	*/
}



RenderingEngineProjMain::~RenderingEngineProjMain()
{
	// Deregister device notification
	m_deviceResources->RegisterDeviceNotify(nullptr);
}



// Updates application state when the window size changes (e.g. device orientation change)
void RenderingEngineProjMain::CreateWindowSizeDependentResources() 
{
	// TODO: Replace this with the size-dependent initialization of your app's content.
	m_sceneRenderer->CreateWindowSizeDependentResources();
}



void RenderingEngineProjMain::StartRenderLoop()
{
	// If the animation render loop is already running then do not start another thread.
	if (m_renderLoopWorker != nullptr && m_renderLoopWorker->Status == AsyncStatus::Started)
	{
		return;
	}

	// Create a task that will be run on a background thread.
	auto workItemHandler = ref new WorkItemHandler([this](IAsyncAction ^ action)
	{
		// Calculate the updated frame and render once per vertical blanking interval.
		while (action->Status == AsyncStatus::Started)
		{
			critical_section::scoped_lock lock(m_criticalSection);
			Update();
			if (Render())
			{
				m_deviceResources->Present();
			}
		}
	});

	// Run task on a dedicated high priority background thread.
	m_renderLoopWorker = ThreadPool::RunAsync(workItemHandler, WorkItemPriority::High, WorkItemOptions::TimeSliced);
}



void RenderingEngineProjMain::StopRenderLoop()
{
	m_renderLoopWorker->Cancel();
}



// Updates the application state once per frame.
void RenderingEngineProjMain::Update() 
{
	ProcessInput();

	// Update scene objects.
	m_timer.Tick([&]()
	{
		// TODO: Replace this with your app's content update functions.
		m_sceneRenderer->Update(m_timer);
		m_fpsTextRenderer->Update(m_timer);
	});
}



// Process all input from the user before updating game state
void RenderingEngineProjMain::ProcessInput()
{


	/*if (MouseRButtonPressed == true)
	{
		pointer_start_X = m_pointerLocationX;
		pointer_start_Y = m_pointerLocationY;
		m_sceneRenderer->TrackingUpdate(pointer_start_X, pointer_start_Y);
		// Reset
		pointer_start_X = 0;
		pointer_start_Y = 0;
		MouseRButtonPressed = false;
	}*/



	// TODO: Add per frame input handling here.
	m_sceneRenderer->TrackingUpdate(m_pointerLocationX, m_pointerLocationY);
	//
	if (Zoom_pressed == true)
	{
		m_sceneRenderer->Zoom(Zoom_value);
		Zoom_pressed = false;
	}
	//
	if (ImportMesh1_Button_Pressed == true)
	{
		const std::string filename = "part.3mf";
		m_sceneRenderer->Import3MFFile(filename);
		ImportMesh1_Button_Pressed = false;
	}
	if (ImportMesh2_Button_Pressed == true)
	{
		const std::string filename = "dragon.3mf";
		m_sceneRenderer->Import3MFFile(filename);
		ImportMesh2_Button_Pressed = false;
	}
	if (ImportMesh3_Button_Pressed == true)
	{
		const std::string filename = "sensor.3mf";
		m_sceneRenderer->Import3MFFile(filename);
		ImportMesh3_Button_Pressed = false;
	}
	if (ImportMesh4_Button_Pressed == true)
	{
		const std::string filename = "implant.3mf";
		m_sceneRenderer->Import3MFFile(filename);
		ImportMesh4_Button_Pressed = false;
	}
	if (ImportMesh5_Button_Pressed == true)
	{
		const std::string filename = "skull.3mf";
		m_sceneRenderer->Import3MFFile(filename);
		ImportMesh5_Button_Pressed = false;
	}
	if (ImportMesh6_Button_Pressed == true)
	{
		const std::string filename = "panther.3mf";
		m_sceneRenderer->Import3MFFile(filename);
		ImportMesh6_Button_Pressed = false;
	}
	//
	if (ShiftX_Button_Pressed == true)
	{
		m_sceneRenderer->Shift(0.5f , 0.0f, 0.0f);
		ShiftX_Button_Pressed = false;
	}
	if (ShiftX1_Button_Pressed == true)
	{
		m_sceneRenderer->Shift(-0.5f, 0.0f, 0.0f);
		ShiftX1_Button_Pressed = false;
	}
	if (ShiftY_Button_Pressed == true)
	{
		m_sceneRenderer->Shift(0.0f, 0.5f, 0.0f);
		ShiftY_Button_Pressed = false;
	}
	if (ShiftY1_Button_Pressed == true)
	{
		m_sceneRenderer->Shift(0.0f, -0.5f, 0.0f);
		ShiftY1_Button_Pressed = false;
	}
	if (ShiftZ_Button_Pressed == true)
	{
		m_sceneRenderer->Shift(0.0f, 0.0f, 0.5f);
		ShiftZ_Button_Pressed = false;
	}
	if (ShiftZ1_Button_Pressed == true)
	{
		m_sceneRenderer->Shift(0.0f, 0.0f, -0.5f);
		ShiftZ1_Button_Pressed = false;
	}
	//
	if (Yaw_Button_Pressed == true)
	{
		m_sceneRenderer->Yaw( 0.05f );
		Yaw_Button_Pressed = false;
	}
	if (Pitch_Button_Pressed == true)
	{
		m_sceneRenderer->Pitch( 0.05f );
		Pitch_Button_Pressed = false;
	}
	if (Roll_Button_Pressed == true)
	{
		m_sceneRenderer->Roll( 0.05f );
		Roll_Button_Pressed = false;
	}
}



// Renders the current frame according to the current application state.
// Returns true if the frame was rendered and is ready to be displayed.
bool RenderingEngineProjMain::Render() 
{
	// Don't try to render anything before the first Update.
	if (m_timer.GetFrameCount() == 0)
	{
		return false;
	}

	auto context = m_deviceResources->GetD3DDeviceContext();

	// Reset the viewport to target the whole screen.
	auto viewport = m_deviceResources->GetScreenViewport();
	context->RSSetViewports(1, &viewport);

	// Reset render targets to the screen.
	ID3D11RenderTargetView *const targets[1] = { m_deviceResources->GetBackBufferRenderTargetView() };
	context->OMSetRenderTargets(1, targets, m_deviceResources->GetDepthStencilView());

	// Clear the back buffer and depth stencil view.
	context->ClearRenderTargetView(m_deviceResources->GetBackBufferRenderTargetView(), DirectX::Colors::CornflowerBlue);
	context->ClearDepthStencilView(m_deviceResources->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// Render the scene objects.
	// TODO: Replace this with your app's content rendering functions.
	m_sceneRenderer->Render();
	m_fpsTextRenderer->Render();

	return true;
}



// Notifies renderers that device resources need to be released.
void RenderingEngineProjMain::OnDeviceLost()
{
	m_sceneRenderer->ReleaseDeviceDependentResources();
	m_fpsTextRenderer->ReleaseDeviceDependentResources();
}



// Notifies renderers that device resources may now be recreated.
void RenderingEngineProjMain::OnDeviceRestored()
{
	m_sceneRenderer->CreateDeviceDependentResources();
	m_fpsTextRenderer->CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();
}


