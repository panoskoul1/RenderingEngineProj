#pragma once

#include "Common\StepTimer.h"
#include "Common\DeviceResources.h"
#include "Content\Sample3DSceneRenderer.h"
#include "Content\SampleFpsTextRenderer.h"

// Renders Direct2D and 3D content on the screen.
namespace RenderingEngineProj
{
	class RenderingEngineProjMain : public DX::IDeviceNotify
	{
	public:
		RenderingEngineProjMain(const std::shared_ptr<DX::DeviceResources>& deviceResources);
		~RenderingEngineProjMain();
		void CreateWindowSizeDependentResources();
		void StartRenderLoop();
		void StopRenderLoop();
		Concurrency::critical_section& GetCriticalSection() { return m_criticalSection; }


		//Mouse pointer events
		void StartTracking(float x, float y) { MouseRButtonPressed = true; m_sceneRenderer->StartTracking(x, y); }
		void TrackingUpdate(float positionX, float positionY) { m_pointerLocationX = positionX; m_pointerLocationY = positionY;}
		void StopTracking(float x, float y) { m_sceneRenderer->StopTracking(x, y); }
		bool IsTracking() { return m_sceneRenderer->IsTracking(); }
		void ImportMesh1_Pressed() { ImportMesh1_Button_Pressed = true; }
		void ImportMesh2_Pressed() { ImportMesh2_Button_Pressed = true; }
		void ImportMesh3_Pressed() { ImportMesh3_Button_Pressed = true; }
		void ShiftX_Pressed() { ShiftX_Button_Pressed = true; }
		void ShiftX1_Pressed() { ShiftX1_Button_Pressed = true; }
		void ShiftY_Pressed() { ShiftY_Button_Pressed = true; }
		void ShiftY1_Pressed() { ShiftY1_Button_Pressed = true; }
		void ShiftZ_Pressed() { ShiftZ_Button_Pressed = true; }
		void ShiftZ1_Pressed() { ShiftZ1_Button_Pressed = true; }
		void Yaw_Pressed() { Yaw_Button_Pressed = true; }
		void Pitch_Pressed() { Pitch_Button_Pressed = true; }
		void Roll_Pressed() { Roll_Button_Pressed = true; }
		
		bool MouseRButtonPressed = false;
		void Set_Zoom(int dx) { Zoom_value = -dx; Zoom_pressed = true; };

		
		float pointer_start_X;
		float pointer_start_Y;

		// IDeviceNotify
		virtual void OnDeviceLost();
		virtual void OnDeviceRestored();

	private:
		void ProcessInput();
		void Update();
		bool Render();

		// Cached pointer to device resources.
		std::shared_ptr<DX::DeviceResources> m_deviceResources;

		// TODO: Replace with your own content renderers.
		std::unique_ptr<Sample3DSceneRenderer> m_sceneRenderer;
		std::unique_ptr<SampleFpsTextRenderer> m_fpsTextRenderer;

		Windows::Foundation::IAsyncAction^ m_renderLoopWorker;
		Concurrency::critical_section m_criticalSection;

		// Rendering loop timer.
		DX::StepTimer m_timer;

		// Track current input pointer position.
		float m_pointerLocationX;
		float m_pointerLocationY;

		//Zoom constant
		int Zoom_value = 0;
		bool Zoom_pressed = false;

		//Buttons
		bool ImportMesh1_Button_Pressed = false;
		bool ImportMesh2_Button_Pressed = false;
		bool ImportMesh3_Button_Pressed = false;
		bool ShiftX_Button_Pressed = false;
		bool ShiftX1_Button_Pressed = false;
		bool ShiftY_Button_Pressed = false;
		bool ShiftY1_Button_Pressed = false;
		bool ShiftZ_Button_Pressed = false;
		bool ShiftZ1_Button_Pressed = false;
		bool Yaw_Button_Pressed = false;
		bool Pitch_Button_Pressed = false;
		bool Roll_Button_Pressed = false;


	};
}