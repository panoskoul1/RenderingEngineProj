#pragma once

#include "..\Common\DeviceResources.h"
#include "ShaderStructures.h"
#include "..\Common\StepTimer.h"
#include <DirectXMath.h>

namespace RenderingEngineProj
{
	// This sample renderer instantiates a basic rendering pipeline.
	class Sample3DSceneRenderer
	{
	public:
		Sample3DSceneRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources);
		void CreateDeviceDependentResources();
		void CreateWindowSizeDependentResources();
		void ReleaseDeviceDependentResources();
		void Update(DX::StepTimer const& timer);
		void Render();
		void StartTracking(float x, float y);
		void TrackingUpdate(float positionX, float positionY);
		void StopTracking(float x, float y);
		bool IsTracking() { return m_tracking; }


		// Camera operations
		void RotateCam(float deltaX, float deltaY);
		void Zoom(int dx);
		void current_cam_position(float x, float y, float z) { cam_pos1[0] = x; cam_pos1[1] = y; cam_pos1[2] = z; };


		// Eye is at (0,0.7,1.5), looking at point (0,-0.1,0) with the up-vector along the y-axis.
		DirectX::XMVECTOR eye = { 50.0f, 50.0f , 50.0f };
		DirectX::XMVECTOR at;
		DirectX::XMVECTOR up;
		std::vector<float> cam_pos1 = { 50.0f, 50.0f , 50.0f };

		// Mesh object operations
		void Shift(float x, float y, float z);
		void Yaw(float radians);
		void Pitch(float radians);
		void Roll(float radians);




		// Global Variables.
		float pointer_start_X;
		float pointer_start_Y;
		float m_yaw = 0;
		float m_pitch = 0;
		float m_roll = 0;
		VertexPositionColor Vertices[6000000];
		VertexPositionColor Vertices_table[8];
		int nVertexCount;
		unsigned short Indices[40000000];
		unsigned short Indices_table[36];
		float pos_x = 0;
		float pos_y = 0;
		float pos_z = 0;

		int nTriangleCount;



		void Import3MFFile(const std::string& fileName);
		void DragObjectToOrigin(int numVertices, int numIndices);
		void CalculateNormals(int vertexCount, int indexCount);
		void CalculateNormals1(int vertexCount, int indexCount);
		void Sample3DSceneRenderer::ApplyTransformation(DirectX::XMMATRIX transformation);


		DirectX::XMVECTOR Sample3DSceneRenderer::GetCenterOfMass(int numVertices);

		void Sample3DSceneRenderer::CreateMesh();
		void Sample3DSceneRenderer::CreatePlane();



	private:
		// Cached pointer to device resources.
		std::shared_ptr<DX::DeviceResources> m_deviceResources;

		// Direct3D resources for main object geometry.
		Microsoft::WRL::ComPtr<ID3D11InputLayout>	m_inputLayout;
		Microsoft::WRL::ComPtr<ID3D11Buffer>		m_vertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer>		m_indexBuffer;
		Microsoft::WRL::ComPtr<ID3D11VertexShader>	m_vertexShader;
		Microsoft::WRL::ComPtr<ID3D11PixelShader>	m_pixelShader;
		Microsoft::WRL::ComPtr<ID3D11Buffer>		m_constantBuffer;


		// Direct3D resources for Table geometry.
		Microsoft::WRL::ComPtr<ID3D11InputLayout>	m_inputLayout1;
		Microsoft::WRL::ComPtr<ID3D11Buffer>		m_vertexBuffer1;
		Microsoft::WRL::ComPtr<ID3D11Buffer>		m_indexBuffer1;
		Microsoft::WRL::ComPtr<ID3D11VertexShader>	m_vertexShader1;
		Microsoft::WRL::ComPtr<ID3D11PixelShader>	m_pixelShader1;
		Microsoft::WRL::ComPtr<ID3D11Buffer>		m_constantBuffer1;


		// System resources for cube geometry.
		ModelViewProjectionConstantBuffer	m_constantBufferData;
		uint32	m_indexCount;
		ModelViewProjectionConstantBuffer	m_constantBufferData1;
		uint32	m_indexCount1;

		// Variables used with the rendering loop.
		bool	m_loadingComplete;
		float	m_degreesPerSecond;
		bool	m_tracking;
		int		gbl_cnt = 0;
	};
}

