#include "stdafx.h"

#include "HexagonPrefab.h"
#include "../Components/Components.h"

HexagonPrefab::HexagonPrefab(float length, XMFLOAT4 color):
	m_Length(length),
	m_Color(color)
{
}


HexagonPrefab::~HexagonPrefab()
{
}

void HexagonPrefab::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	//Create Cube
	auto mesh = new MeshDrawComponent(6);

	float innerRadius = m_Length * 0.866025404f; // Sqrt(3)/2 -> precalculated so no sqrt operation
	
	VertexPosNormCol p1 = VertexPosNormCol(XMFLOAT3(0.0f, m_Length, 0.0f), XMFLOAT3(0.f, 0.f, 1.f), m_Color);
	VertexPosNormCol p2 = VertexPosNormCol(XMFLOAT3(innerRadius, 0.5f * m_Length, 0), XMFLOAT3(0.f, 0.f, 1.f), m_Color);
	VertexPosNormCol p3 = VertexPosNormCol(XMFLOAT3(innerRadius, -0.5f * m_Length, 0), XMFLOAT3(0.f, 0.f, 1.f), m_Color);
	VertexPosNormCol p4 = VertexPosNormCol(XMFLOAT3(0.0f, -m_Length, 0.0f), XMFLOAT3(0.f, 0.f, 1.f), m_Color);
	VertexPosNormCol p5 = VertexPosNormCol(XMFLOAT3(-innerRadius, -0.5f * m_Length, 0), XMFLOAT3(0.f, 0.f, 1.f), m_Color);
	VertexPosNormCol p6 = VertexPosNormCol(XMFLOAT3(-innerRadius, 0.5f * m_Length, 0), XMFLOAT3(0.f, 0.f, 1.f), m_Color);
	VertexPosNormCol center = VertexPosNormCol(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.f, 0.f, 1.f), m_Color);
	
	mesh->AddTriangle(center, p1, p2);
	mesh->AddTriangle(center, p2, p3);
	mesh->AddTriangle(center, p3, p4);
	mesh->AddTriangle(center, p4, p5);
	mesh->AddTriangle(center, p5, p6);
	mesh->AddTriangle(center, p6, p1);

	AddComponent(mesh);

}