#include "pch.h"
#include "Sample3DSceneRenderer.h"

#include "..\Common\DirectXHelper.h"
#include "lib3mf_implicit.hpp"
#include <iostream>



using namespace RenderingEngineProj;

using namespace DirectX;
using namespace Windows::Foundation;





// Loads vertex and pixel shaders from files and instantiates the cube geometry.
Sample3DSceneRenderer::Sample3DSceneRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources) :
	m_loadingComplete(false),
	m_degreesPerSecond(45),
	m_indexCount(0),
	m_indexCount1(0),
	m_tracking(false),
	m_deviceResources(deviceResources)
{
	CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();
}





// Initializes view parameters when the window size changes.
void Sample3DSceneRenderer::CreateWindowSizeDependentResources()
{
	Size outputSize = m_deviceResources->GetOutputSize();
	float aspectRatio = outputSize.Width / outputSize.Height;
	float fovAngleY = 70.0f * XM_PI / 180.0f;

	// This is a simple example of change that can be made when the app is in
	// portrait or snapped view.
	if (aspectRatio < 1.0f)
	{
		fovAngleY *= 2.0f;
	}

	// Note that the OrientationTransform3D matrix is post-multiplied here
	// in order to correctly orient the scene to match the display orientation.
	// This post-multiplication step is required for any draw calls that are
	// made to the swap chain render target. For draw calls to other targets,
	// this transform should not be applied.

	// This sample makes use of a right-handed coordinate system using row-major matrices.
	XMMATRIX perspectiveMatrix = XMMatrixPerspectiveFovRH(
		fovAngleY,
		aspectRatio,
		0.1f,
		100000.0f
		);

	XMFLOAT4X4 orientation = m_deviceResources->GetOrientationTransform3D();

	XMMATRIX orientationMatrix = XMLoadFloat4x4(&orientation);

	XMStoreFloat4x4(
		&m_constantBufferData.projection,
		XMMatrixTranspose(perspectiveMatrix * orientationMatrix)
		);

	XMStoreFloat4x4(
		&m_constantBufferData1.projection,
		XMMatrixTranspose(perspectiveMatrix * orientationMatrix)
	);

	// Eye is at (0,0.7,1.5), looking at point (0,-0.1,0) with the up-vector along the y-axis.
	//XMVECTORF32 eye = {values[0], values[1], values[2], 0.0f};
	eye = { cam_pos1[0], cam_pos1[1], cam_pos1[2], 0.0f };
	at = { 0.0f, 0.0f, 0.0f, 0.0f };
	up = { 0.0f, 1.0f, 0.0f, 0.0f };

	XMStoreFloat4x4(&m_constantBufferData.view, XMMatrixTranspose(XMMatrixLookAtRH(eye, at, up)));
	XMStoreFloat4x4(&m_constantBufferData1.view, XMMatrixTranspose(XMMatrixLookAtRH(eye, at, up)));

	//XMStoreFloat4x4(&m_constantBufferData2.view, XMMatrixTranspose(XMMatrixLookAtRH(eye, at, up)));
	XMStoreFloat4x4(&m_constantBufferData.model, XMMatrixTranspose(XMMatrixRotationY(0)));
	XMStoreFloat4x4(&m_constantBufferData1.model, XMMatrixTranspose(XMMatrixRotationY(0)));

	//XMStoreFloat4x4(&m_constantBufferData2.model, XMMatrixTranspose(XMMatrixRotationY(0)));
	//XMStoreFloat4(&m_constantBufferData.eye, eye);
	//XMStoreFloat4(&m_constantBufferData2.eye, eye);



}





// Called once per frame, rotates the cube and calculates the model and view matrices.
void Sample3DSceneRenderer::Update(DX::StepTimer const& timer)
{
	if (!m_tracking)
	{
		// Convert degrees to radians, then convert seconds to rotation angle
		//float radiansPerSecond = XMConvertToRadians(m_degreesPerSecond);
		//double totalRotation = timer.GetTotalSeconds() * radiansPerSecond;
		//float radians = static_cast<float>(fmod(totalRotation, XM_2PI));

		//Rotate(radians);
	}
}






void Sample3DSceneRenderer::RotateCam(float deltaX, float deltaY)
{
	// Map mouse movement to rotation angles
	float rotationSpeed1 = 0.007f;
	float rotationSpeed2 = 0.007f;
	float azimuthalAngle = -deltaX * rotationSpeed1;
	float inclinationAngle = -deltaY * rotationSpeed2;

	// Update eye position using the rotation angles
	XMMATRIX rotationMatrix = XMMatrixRotationAxis(up, azimuthalAngle) * XMMatrixRotationAxis(XMVector3Cross(up, eye - at), inclinationAngle);
	eye = XMVector3Transform(eye - at, rotationMatrix) + at;
	//
	//
	Size outputSize = m_deviceResources->GetOutputSize();
	float aspectRatio = outputSize.Width / outputSize.Height;
	float fovAngleY = 70.0f * XM_PI / 180.0f;
	if (aspectRatio < 1.0f)
	{
		fovAngleY *= 2.0f;
	}
	XMMATRIX perspectiveMatrix = XMMatrixPerspectiveFovRH(
		fovAngleY,
		aspectRatio,
		0.01f,
		100000.0f
	);
	XMFLOAT4X4 orientation = m_deviceResources->GetOrientationTransform3D();
	XMMATRIX orientationMatrix = XMLoadFloat4x4(&orientation);
	XMStoreFloat4x4(
		&m_constantBufferData.projection,
		XMMatrixTranspose(perspectiveMatrix * orientationMatrix)
	);
	XMStoreFloat4x4(
		&m_constantBufferData1.projection,
		XMMatrixTranspose(perspectiveMatrix * orientationMatrix)
	);
	//

	// Update world matrices for all components/ models.
	XMStoreFloat4x4(&m_constantBufferData.view, XMMatrixTranspose(XMMatrixLookAtRH(eye, at, up)));
	XMStoreFloat4x4(&m_constantBufferData1.view, XMMatrixTranspose(XMMatrixLookAtRH(eye, at, up)));

	//XMStoreFloat4x4(&m_constantBufferData2.view, XMMatrixTranspose(XMMatrixLookAtRH(eye, at, up)));
	//XMStoreFloat4x4(&m_constantBufferData.model, XMMatrixTranspose(XMMatrixRotationY(0)));
	//XMStoreFloat4x4(&m_constantBufferData1.model, XMMatrixTranspose(XMMatrixRotationY(0)));

	//XMStoreFloat4x4(&m_constantBufferData2.model, XMMatrixTranspose(XMMatrixRotationY(0)));
	//XMStoreFloat4(&m_constantBufferData.eye, eye);
	//XMStoreFloat4(&m_constantBufferData2.eye, eye);

	cam_pos1[0] = XMVectorGetX(eye);
	cam_pos1[1] = XMVectorGetY(eye);
	cam_pos1[2] = XMVectorGetZ(eye);
}


void Sample3DSceneRenderer::Zoom(int dx)
{
	// Update the camera's position based on the mouse scroll input
	float zoomSensitivity = 1.0f;
	//XMVECTOR zoomDirection = eye - at;
	XMVECTOR zoomDirection = XMVectorSubtract(eye, at);

	zoomDirection = XMVector3Normalize(zoomDirection) * zoomSensitivity * (dx / WHEEL_DELTA);
	//eye += zoomDirection;
	XMVECTOR result = XMVectorAdd(eye, zoomDirection);

	// Update the view matrix with the new camera position
	//UpdateViewMatrix();

	//std::vector<float> cam_pos= EyePosition(XMVectorGetX(result), XMVectorGetY(result), XMVectorGetZ(result));


	Sample3DSceneRenderer::current_cam_position(XMVectorGetX(result), XMVectorGetY(result), XMVectorGetZ(result));


	eye = { cam_pos1[0], cam_pos1[1], cam_pos1[2], 0.0f };
	at = { 0.0f, 0.0f, 0.0f, 0.0f };
	up = { 0.0f, 1.0f, 0.0f, 0.0f };

	XMStoreFloat4x4(&m_constantBufferData.view, XMMatrixTranspose(XMMatrixLookAtRH(eye, at, up)));
	XMStoreFloat4x4(&m_constantBufferData1.view, XMMatrixTranspose(XMMatrixLookAtRH(eye, at, up)));
}







// Rotate the 3D cube model a set amount of radians.
void Sample3DSceneRenderer::Yaw(float radians)
{
	// Increase yaw by radians.
	m_yaw += radians;

	XMVECTOR centerOfMass = GetCenterOfMass(nVertexCount);

	XMVECTOR translationToOriginVec = -centerOfMass; 
	XMMATRIX translationToOrigin = XMMatrixTranslationFromVector(translationToOriginVec); 

	ApplyTransformation(translationToOrigin);

	XMMATRIX rotationMatrixY = XMMatrixRotationY(m_yaw);  

	ApplyTransformation(rotationMatrixY);

	XMVECTOR translationBackVec = centerOfMass;
	XMMATRIX translationBack = XMMatrixTranslationFromVector(translationBackVec);  

	ApplyTransformation(translationBack);

}

void Sample3DSceneRenderer::Pitch(float radians)
{
	m_pitch += radians;

	XMVECTOR centerOfMass = GetCenterOfMass(nVertexCount);

	XMVECTOR translationToOriginVec = -centerOfMass;  
	XMMATRIX translationToOrigin = XMMatrixTranslationFromVector(translationToOriginVec);  

	ApplyTransformation(translationToOrigin);

	XMMATRIX rotationMatrixX = XMMatrixRotationX(m_pitch);  

	ApplyTransformation(rotationMatrixX);

	XMVECTOR translationBackVec = centerOfMass;
	XMMATRIX translationBack = XMMatrixTranslationFromVector(translationBackVec);  

	ApplyTransformation(translationBack);
}

void Sample3DSceneRenderer::Roll(float radians)  
{
	m_roll += radians;  

	XMVECTOR centerOfMass = GetCenterOfMass(nVertexCount);

	XMVECTOR translationToOriginVec = -centerOfMass;  
	XMMATRIX translationToOrigin = XMMatrixTranslationFromVector(translationToOriginVec);  

	ApplyTransformation(translationToOrigin);

	XMMATRIX rotationMatrixZ = XMMatrixRotationZ(m_roll);  

	ApplyTransformation(rotationMatrixZ);

	XMVECTOR translationBackVec = centerOfMass;
	XMMATRIX translationBack = XMMatrixTranslationFromVector(translationBackVec);  

	ApplyTransformation(translationBack);
}




XMVECTOR Sample3DSceneRenderer::GetCenterOfMass(int numVertices)
{
	Sample3DSceneRenderer::Vertices;

	// Initialize the center of mass
	XMVECTOR centerOfMass = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

	// Sum up the positions of all vertices
	for (int i = 0; i < numVertices; ++i) {
		centerOfMass = XMVectorAdd(centerOfMass, XMLoadFloat3(&Vertices[i].pos));
	}

	// Divide by the number of vertices to get the average position, i.e., the center of mass
	centerOfMass = XMVectorScale(centerOfMass, 1.0f / numVertices);

	return centerOfMass;
}





void Sample3DSceneRenderer::ApplyTransformation(DirectX::XMMATRIX transformation)
{
	// Load the current model matrix
	XMMATRIX modelMatrix = XMLoadFloat4x4(&m_constantBufferData.model);

	// Apply the transformation
	modelMatrix = modelMatrix * transformation;

	// Store the updated model matrix in the shader constant buffer
	XMStoreFloat4x4(&m_constantBufferData.model, modelMatrix);
}


// Function to translate the cube vertices
void Sample3DSceneRenderer::Shift(float x, float y, float z)
{
	pos_x = pos_x + x;
	pos_y = pos_y + y;
	pos_z = pos_z + z;

	// Create a translation matrix
	DirectX::XMMATRIX translationMatrix = DirectX::XMMatrixTranslation(pos_x, pos_z, pos_y);

	// Store the translation matrix in m_constantBufferData
	XMStoreFloat4x4(&m_constantBufferData.model, XMMatrixTranspose(translationMatrix));
}



void Sample3DSceneRenderer::StartTracking(float x, float y)
{
	m_tracking = true;
	//pointer_start_X = x;
	//pointer_start_Y = y;

}





// When tracking, the 3D cube can be rotated around its Y axis by tracking pointer position relative to the output screen width.
void Sample3DSceneRenderer::TrackingUpdate(float positionX, float positionY)
{

	float distX = (positionX - pointer_start_X);
	float distY = (positionY - pointer_start_Y);
	/*if (distX < 10 || distY < 10)
	{
		distX = 0;
		distY = 0;
	}*/


	if (m_tracking)
	{

		//float radiansx = (XM_2PI * 2.0f * distX) / m_deviceResources->GetOutputSize().Width;
		//float radiansy= (XM_2PI * 2.0f * distY) / m_deviceResources->GetOutputSize().Height;
		//float radiansz = 0;
		//
		RotateCam(distX, distY);
		//
		pointer_start_X = positionX;
		pointer_start_Y = positionY;

	}

	
}




void Sample3DSceneRenderer::StopTracking(float x, float y)
{
	m_tracking = false;

}





// Renders one frame using the vertex and pixel shaders.
void Sample3DSceneRenderer::Render()
{
	// Loading is asynchronous. Only draw geometry after it's loaded.
	if (!m_loadingComplete)
	{
		return;
	}

	auto context = m_deviceResources->GetD3DDeviceContext();

	// Each vertex is one instance of the VertexPositionColor struct.
	UINT stride = sizeof(VertexPositionColor);
	UINT offset = 0;

	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context->IASetInputLayout(m_inputLayout.Get());

	// Attach our vertex shader.
	context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
	// Attach our pixel shader.
	context->PSSetShader(m_pixelShader.Get(), nullptr, 0);
	//
	//
	//
	//
	//
	// Prepare the constant buffer to send it to the graphics device.
	context->UpdateSubresource1(
		m_constantBuffer.Get(),
		0,
		NULL,
		&m_constantBufferData,
		0,
		0,
		0
		);

	// Each vertex is one instance of the VertexPositionColor struct.
	context->IASetVertexBuffers(
		0,
		1,
		m_vertexBuffer.GetAddressOf(),
		&stride,
		&offset
		);

	context->IASetIndexBuffer(
		m_indexBuffer.Get(),
		DXGI_FORMAT_R16_UINT, // Each index is one 16-bit unsigned integer (short).
		0
		);

	// Send the constant buffer to the graphics device.
	context->VSSetConstantBuffers1(
		0,
		1,
		m_constantBuffer.GetAddressOf(),
		nullptr,
		nullptr
		);

	// Draw the objects.
	context->DrawIndexed(
		m_indexCount,
		0,
		0
		);
	//
	//
	//
	//
	//
	//context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	// Each vertex is one instance of the VertexPositionColor struct.
	//UINT stride1 = sizeof(VertexPositionColor);
	UINT offset1 = 0;
	// Now we prepare the second constant buffer for the second mesh.
	context->UpdateSubresource1(
		m_constantBuffer1.Get(),
		0,
		NULL,
		&m_constantBufferData1,
		0,
		0,
		0
	);

    // Render the second mesh (plane object)
    // Each vertex is one instance of the VertexPositionColor struct.
	context->IASetVertexBuffers(
		0,
		1,
		m_vertexBuffer1.GetAddressOf(),
		&stride,
		&offset1
	);

	context->IASetIndexBuffer(
		m_indexBuffer1.Get(),
		DXGI_FORMAT_R16_UINT, // Each index is one 16-bit unsigned integer (short).
		0
	);

	// Send the new constant buffer to the graphics device.
	context->VSSetConstantBuffers1(
		0,
		1,
		m_constantBuffer1.GetAddressOf(),
		nullptr,
		nullptr
	);

	// Draw the objects.
	context->DrawIndexed(
		m_indexCount1,
		0,
		0
	);
}









void Sample3DSceneRenderer::CreateDeviceDependentResources()
{
	// Load shaders asynchronously.
	auto loadVSTask = DX::ReadDataAsync(L"SampleVertexShader.cso");
	auto loadPSTask = DX::ReadDataAsync(L"SamplePixelShader.cso");

	// After the vertex shader file is loaded, create the shader and input layout.
	auto createVSTask = loadVSTask.then([this](const std::vector<byte>& fileData) {
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateVertexShader(
				&fileData[0],
				fileData.size(),
				nullptr,
				&m_vertexShader
			)
		);

		static const D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "EYE", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateInputLayout(
				vertexDesc,
				ARRAYSIZE(vertexDesc),
				&fileData[0],
				fileData.size(),
				&m_inputLayout
			)
		);
		});

	// After the pixel shader file is loaded, create the shader and constant buffer.
	auto createPSTask = loadPSTask.then([this](const std::vector<byte>& fileData) {
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreatePixelShader(
				&fileData[0],
				fileData.size(),
				nullptr,
				&m_pixelShader
			)
		);

		CD3D11_BUFFER_DESC constantBufferDesc(sizeof(ModelViewProjectionConstantBuffer), D3D11_BIND_CONSTANT_BUFFER);
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateBuffer(
				&constantBufferDesc,
				nullptr,
				&m_constantBuffer
			)
		);
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateBuffer(
				&constantBufferDesc,
				nullptr,
				&m_constantBuffer1
			)
		);
		});

	// Wait for all async tasks to complete and then create meshes.
	(createPSTask && createVSTask).then([this]() {
		CreateMesh();
		CreatePlane();
		m_loadingComplete = true;
		});
}






void Sample3DSceneRenderer::CreateMesh()
{
	// Your mesh creation code here...
				// Load mesh vertices.
	VertexPositionColor CubeVertices[] =
	{
		{XMFLOAT3(-0.5f, 0.0f, -0.5f), XMFLOAT3(0.5f, 0.5f, 1.0f)},
		{XMFLOAT3(-0.5f, 0.0f,  0.5f), XMFLOAT3(0.5f, 0.5f, 1.0f)},
		{XMFLOAT3(-0.5f,  0.5f, -0.5f), XMFLOAT3(0.5f, 0.5f, 1.0f)},
		{XMFLOAT3(-0.5f,  0.5f,  0.5f), XMFLOAT3(0.5f, 0.5f, 1.0f)},
		{XMFLOAT3(0.5f, 0.0f, -0.5f), XMFLOAT3(0.5f, 0.5f, 1.0)},
		{XMFLOAT3(0.5f, 0.0f,  0.5f), XMFLOAT3(0.5f, 0.5f, 1.0f)},
		{XMFLOAT3(0.5f,  0.5f, -0.5f), XMFLOAT3(0.5f, 0.5f, 1.0f)},
		{XMFLOAT3(0.5f,  0.5f,  0.5f), XMFLOAT3(0.5f, 0.5f, 1.0f)},
	};
	//
	// Load mesh indices.
	unsigned short CubeIndices[] =
	{
		0,2,1, // -x
		1,2,3,

		4,5,6, // +x
		5,7,6,

		0,1,5, // -y
		0,5,4,

		2,6,7, // +y
		2,7,3,

		0,4,6, // -z
		0,6,2,

		1,3,7, // +z
		1,7,5,
	};
	//
	// 
	DirectX::XMFLOAT3 eyeFloat3;
	for (int i = 0; i < 8; i++) {
		DirectX::XMStoreFloat3(&eyeFloat3, eye);
		Vertices[i].eye = eyeFloat3;
		//
		Vertices[i].pos = CubeVertices[i].pos;
		Vertices[i].color = CubeVertices[i].color;
	}
	for (int i = 0; i < 36; i++) {
		Indices[i] = CubeIndices[i];
	}
	// 
	nVertexCount =8;
	// 
	// Calculate noramls for the vertices
	CalculateNormals(8, 36);
	// Create buffers to hold cube geometry data.
	D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 };
	vertexBufferData.pSysMem = Vertices;
	vertexBufferData.SysMemPitch = 0;
	vertexBufferData.SysMemSlicePitch = 0;
	CD3D11_BUFFER_DESC vertexBufferDesc(sizeof(VertexPositionColor) * 8, D3D11_BIND_VERTEX_BUFFER);
	DX::ThrowIfFailed(
		m_deviceResources->GetD3DDevice()->CreateBuffer(
			&vertexBufferDesc,
			&vertexBufferData,
			&m_vertexBuffer
		)
	);
	//
	m_indexCount = ARRAYSIZE(Indices);
	D3D11_SUBRESOURCE_DATA indexBufferData = { 0 };
	indexBufferData.pSysMem = Indices;
	indexBufferData.SysMemPitch = 0;
	indexBufferData.SysMemSlicePitch = 0;
	CD3D11_BUFFER_DESC indexBufferDesc(sizeof(unsigned int) * 36, D3D11_BIND_INDEX_BUFFER);
	DX::ThrowIfFailed(
		m_deviceResources->GetD3DDevice()->CreateBuffer(
			&indexBufferDesc,
			&indexBufferData,
			&m_indexBuffer
		)
	);
}

void Sample3DSceneRenderer::CreatePlane()
{
	// Your plane creation code here...
			// Plane---Load mesh vertices.
	VertexPositionColor PlaneVertices[] =
	{
		{XMFLOAT3(-20.0f, -6.8f, -20.0f), XMFLOAT3(0.5f, 0.5f, 0.5f)},
		{XMFLOAT3(-20.0f, -6.8f, 20.0f), XMFLOAT3(0.5f, 0.5f, 0.5f)},
		{XMFLOAT3(20.0f, -6.8f, -20.0f), XMFLOAT3(0.5f, 0.5f, 0.5f)},
		{XMFLOAT3(20.0f, -6.8f, 20.0f), XMFLOAT3(0.5f, 0.5f, 0.5f)},
		{XMFLOAT3(-20.0f, -7.0f, -20.0f), XMFLOAT3(0.5f, 0.5f, 0.5f)},
		{XMFLOAT3(-20.0f, -7.0f, 20.0f), XMFLOAT3(0.5f, 0.5f, 0.5f)},
		{XMFLOAT3(20.0f, -7.0f, -20.0f), XMFLOAT3(0.5f, 0.5f, 0.5f)},
		{XMFLOAT3(20.0f, -7.0f, 20.0f), XMFLOAT3(1.0f, 1.0f, 1.0f)},
	};
	//
	// Load mesh indices.
	unsigned short PlaneIndices[] =
	{
		0,1,2, // -z
		1,3,2,

		4,6,5, // +z
		5,6,7,

		0,4,5, // -x
		0,5,1,

		2,3,7, // +x
		2,7,6,

		0,2,6, // -y
		0,6,4,

		1,5,7, // +y
		1,7,3,
	};

	// 
	DirectX::XMFLOAT3 eyeFloat3;
	for (int i = 0; i < 8; i++) {
		DirectX::XMStoreFloat3(&eyeFloat3, eye);
		Vertices_table[i].eye = eyeFloat3;
		//
		Vertices_table[i].pos = PlaneVertices[i].pos;
		Vertices_table[i].color = PlaneVertices[i].color;
	}
	for (int i = 0; i < 36; i++) {
		Indices_table[i] = PlaneIndices[i];
	}
	// 
	// Calculate noramls for the vertices
	CalculateNormals1(8, 36);
	//
	// Create buffers to hold cube geometry data.
	D3D11_SUBRESOURCE_DATA vertexBufferData1 = { 0 };
	vertexBufferData1.pSysMem = PlaneVertices;
	vertexBufferData1.SysMemPitch = 0;
	vertexBufferData1.SysMemSlicePitch = 0;
	CD3D11_BUFFER_DESC vertexBufferDesc(sizeof(VertexPositionColor) * 8, D3D11_BIND_VERTEX_BUFFER);
	DX::ThrowIfFailed(
		m_deviceResources->GetD3DDevice()->CreateBuffer(
			&vertexBufferDesc,
			&vertexBufferData1,
			&m_vertexBuffer1
		)
	);
	//
	m_indexCount1 = ARRAYSIZE(PlaneIndices);
	D3D11_SUBRESOURCE_DATA indexBufferData1 = { 0 };
	indexBufferData1.pSysMem = PlaneIndices;
	indexBufferData1.SysMemPitch = 0;
	indexBufferData1.SysMemSlicePitch = 0;
	CD3D11_BUFFER_DESC indexBufferDesc(sizeof(unsigned int) * 36, D3D11_BIND_INDEX_BUFFER);
	DX::ThrowIfFailed(
		m_deviceResources->GetD3DDevice()->CreateBuffer(
			&indexBufferDesc,
			&indexBufferData1,
			&m_indexBuffer1
		)
	);


}




void Sample3DSceneRenderer::ReleaseDeviceDependentResources()
{
	m_loadingComplete = false;
	m_vertexShader.Reset();
	m_inputLayout.Reset();
	m_pixelShader.Reset();
	m_constantBuffer.Reset();
	m_vertexBuffer.Reset();
	m_indexBuffer.Reset();

}








void Sample3DSceneRenderer::Import3MFFile(const std::string& fileName)
{
	using namespace Lib3MF;

	Sample3DSceneRenderer::Vertices;
	Sample3DSceneRenderer::Indices;

	// Create a wrapper instance
	PWrapper wrapper = CWrapper::loadLibrary();
	PModel model = wrapper->CreateModel();

	// Import Model from 3MF File
	PReader reader = model->QueryReader("3mf");

	// And deactivate the strict mode (default is "false", anyway. This just demonstrates where/how to use it).
	reader->SetStrictModeActive(false);
	reader->ReadFromFile(fileName);

	// Get the mesh object
	PObjectIterator objectIterator = model->GetObjects();
	int vertexCount = 0 ; // Counter for the number of vertices
	int indexCount = 0 ; // Counter for the number of indices
	

	// Iterate through all objects
	while (objectIterator->MoveNext()) {
		PObject object = objectIterator->GetCurrentObject();
		if (object->IsMeshObject()) {
			PMeshObject meshObject = model->GetMeshObjectByID(object->GetResourceID());
			nVertexCount = meshObject->GetVertexCount();
			nTriangleCount = meshObject->GetTriangleCount();

			// Get the vertex buffer
			std::vector<Lib3MF::sPosition> vertices(nVertexCount);
			meshObject->GetVertices(vertices);

			// Access vertices and indices for further processing
			for (const auto& vertex : vertices) {
				// Store the vertex coordinates in the Vertices array
				Vertices[vertexCount].pos.x = vertex.m_Coordinates[0];
				Vertices[vertexCount].pos.y = vertex.m_Coordinates[1];
				Vertices[vertexCount].pos.z = vertex.m_Coordinates[2];
				Vertices[vertexCount].color.x = 0.5f;
				Vertices[vertexCount].color.y = 0.5f;
				Vertices[vertexCount].color.z = 0.1f;

				DirectX::XMFLOAT3 eyeFloat3;
				DirectX::XMStoreFloat3(&eyeFloat3, eye);
				Vertices[vertexCount].eye = eyeFloat3;

				vertexCount++; // Increment the vertex count
			}

			// Get the triangle indices buffer
			std::vector<Lib3MF::sTriangle> triangles(nTriangleCount);
			meshObject->GetTriangleIndices(triangles);

			for (const auto& triangle : triangles) {
				// Store the triangle indices in the Indices array, accounting for the vertex offset
				Indices[indexCount] = static_cast<unsigned short>(triangle.m_Indices[0]);
				Indices[indexCount + 1] = static_cast<unsigned short>(triangle.m_Indices[2]); // Swap the second and third indices
				Indices[indexCount + 2] = static_cast<unsigned short>(triangle.m_Indices[1]); // Swap the second and third indices
				indexCount += 3; // Increment the index count
			}
		}
	}


	nVertexCount = vertexCount;

	// Drag the object to the origin
	this->DragObjectToOrigin(vertexCount, indexCount);
	// Calculate noramls for the vertices
	this->CalculateNormals(vertexCount, indexCount);

	// Create vertex buffer
	D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 };
	vertexBufferData.pSysMem = Vertices;
	vertexBufferData.SysMemPitch = 0;
	vertexBufferData.SysMemSlicePitch = 0;
	CD3D11_BUFFER_DESC vertexBufferDesc(sizeof(VertexPositionColor) * vertexCount, D3D11_BIND_VERTEX_BUFFER);
	DX::ThrowIfFailed(
		m_deviceResources->GetD3DDevice()->CreateBuffer(
			&vertexBufferDesc,
			&vertexBufferData,
			&m_vertexBuffer
		)
	);

	// Create index buffer
	m_indexCount = indexCount; // Use the updated indexCount
	D3D11_SUBRESOURCE_DATA indexBufferData = { 0 };
	indexBufferData.pSysMem = Indices;
	indexBufferData.SysMemPitch = 0;
	indexBufferData.SysMemSlicePitch = 0;
	CD3D11_BUFFER_DESC indexBufferDesc(sizeof(unsigned int) * indexCount, D3D11_BIND_INDEX_BUFFER);
	DX::ThrowIfFailed(
		m_deviceResources->GetD3DDevice()->CreateBuffer(
			&indexBufferDesc,
			&indexBufferData,
			&m_indexBuffer
		)
	);


}



void Sample3DSceneRenderer::DragObjectToOrigin(int numVertices, int indexCount)
{
	Sample3DSceneRenderer::Vertices;
	Sample3DSceneRenderer::Indices;

	// Calculate the center of mass
	float centerOfMass[3] = { 0.0f, 0.0f, 0.0f };
	for (int i = 0; i < numVertices; ++i) {
		centerOfMass[0] += Vertices[i].pos.x;
		centerOfMass[1] += Vertices[i].pos.y;
		centerOfMass[2] += Vertices[i].pos.z;
	}
	centerOfMass[0] /= numVertices;
	centerOfMass[1] /= numVertices;
	centerOfMass[2] /= numVertices;

	// Subtract the center of mass from all the points
	for (int i = 0; i < numVertices; i++) {
		Vertices[i].pos.x -= centerOfMass[0];
		Vertices[i].pos.y -= centerOfMass[1];
		Vertices[i].pos.z -= centerOfMass[2];
	}

}





void Sample3DSceneRenderer::CalculateNormals(int vertexCount, int indexCount)
{
	// Initialize vertex normals
	for (int i = 0; i < vertexCount; i++) {
		Vertices[i].normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	}

	VertexPositionColor v0, v1, v2;
	// Calculate face normals and accumulate
	for (int i = 0; i < indexCount/3; i++) {
		v0 = Vertices[Indices[i * 3 + 0]];
		v1 = Vertices[Indices[i * 3 + 1]];
		v2 = Vertices[Indices[i * 3 + 2]];

		XMFLOAT3 edge1 = XMFLOAT3(v1.pos.x - v0.pos.x, v1.pos.y - v0.pos.y, v1.pos.z - v0.pos.z);
		XMFLOAT3 edge2 = XMFLOAT3(v2.pos.x - v0.pos.x, v2.pos.y - v0.pos.y, v2.pos.z - v0.pos.z);

		XMFLOAT3 faceNormal;
		XMStoreFloat3(&faceNormal, XMVector3Cross(XMLoadFloat3(&edge1), XMLoadFloat3(&edge2)));

		// Determine orientation
		XMFLOAT3 orientationVector;
		XMStoreFloat3(&orientationVector, XMVector3Cross(XMLoadFloat3(&v1.pos), XMLoadFloat3(&v2.pos)));
		if (XMVector3Dot(XMLoadFloat3(&orientationVector), XMLoadFloat3(&v0.pos)).m128_f32[0] < 0) {
			// Clockwise winding. Reverse the normal.
			faceNormal.x = -faceNormal.x;
			faceNormal.y = -faceNormal.y;
			faceNormal.z = -faceNormal.z;
		}

		Vertices[Indices[i * 3 + 0]].normal.x += faceNormal.x;
		Vertices[Indices[i * 3 + 0]].normal.y += faceNormal.y;
		Vertices[Indices[i * 3 + 0]].normal.z += faceNormal.z;

		Vertices[Indices[i * 3 + 1]].normal.x += faceNormal.x;
		Vertices[Indices[i * 3 + 1]].normal.y += faceNormal.y;
		Vertices[Indices[i * 3 + 1]].normal.z += faceNormal.z;

		Vertices[Indices[i * 3 + 2]].normal.x += faceNormal.x;
		Vertices[Indices[i * 3 + 2]].normal.y += faceNormal.y;
		Vertices[Indices[i * 3 + 2]].normal.z += faceNormal.z;
	}

	// Normalize normals
	for (int i = 0; i < vertexCount; i++) {
		XMStoreFloat3(&Vertices[i].normal, XMVector3Normalize(XMLoadFloat3(&Vertices[i].normal)));
	}
}





void Sample3DSceneRenderer::CalculateNormals1(int vertexCount, int indexCount)
{
	// Initialize vertex normals
	for (int i = 0; i < vertexCount; i++) {
		Vertices_table[i].normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	}

	VertexPositionColor v0, v1, v2;
	// Calculate face normals and accumulate
	for (int i = 0; i < indexCount; i++) {
		v0 = Vertices_table[Indices[i * 3 + 0]];
		v1 = Vertices_table[Indices[i * 3 + 1]];
		v2 = Vertices_table[Indices[i * 3 + 2]];

		XMFLOAT3 edge1 = XMFLOAT3(v1.pos.x - v0.pos.x, v1.pos.y - v0.pos.y, v1.pos.z - v0.pos.z);
		XMFLOAT3 edge2 = XMFLOAT3(v2.pos.x - v0.pos.x, v2.pos.y - v0.pos.y, v2.pos.z - v0.pos.z);

		XMFLOAT3 faceNormal;
		XMStoreFloat3(&faceNormal, XMVector3Cross(XMLoadFloat3(&edge1), XMLoadFloat3(&edge2)));

		// Determine orientation
		XMFLOAT3 orientationVector;
		XMStoreFloat3(&orientationVector, XMVector3Cross(XMLoadFloat3(&v1.pos), XMLoadFloat3(&v2.pos)));
		if (XMVector3Dot(XMLoadFloat3(&orientationVector), XMLoadFloat3(&v0.pos)).m128_f32[0] < 0) {
			// Clockwise winding. Reverse the normal.
			faceNormal.x = -faceNormal.x;
			faceNormal.y = -faceNormal.y;
			faceNormal.z = -faceNormal.z;
		}

		Vertices_table[Indices[i * 3 + 0]].normal.x += faceNormal.x;
		Vertices_table[Indices[i * 3 + 0]].normal.y += faceNormal.y;
		Vertices_table[Indices[i * 3 + 0]].normal.z += faceNormal.z;

		Vertices_table[Indices[i * 3 + 1]].normal.x += faceNormal.x;
		Vertices_table[Indices[i * 3 + 1]].normal.y += faceNormal.y;
		Vertices_table[Indices[i * 3 + 1]].normal.z += faceNormal.z;

		Vertices_table[Indices[i * 3 + 2]].normal.x += faceNormal.x;
		Vertices_table[Indices[i * 3 + 2]].normal.y += faceNormal.y;
		Vertices_table[Indices[i * 3 + 2]].normal.z += faceNormal.z;
	}

	// Normalize normals
	for (int i = 0; i < vertexCount; i++) {
		XMStoreFloat3(&Vertices_table[i].normal, XMVector3Normalize(XMLoadFloat3(&Vertices_table[i].normal)));
	}
}