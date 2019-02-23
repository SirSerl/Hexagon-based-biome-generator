#pragma once
#include "..\Scenegraph\GameObject.h"

class HexagonPrefab: public GameObject
{
public:
	HexagonPrefab(float length, XMFLOAT4 color);
	~HexagonPrefab();

protected:

	void Initialize(const GameContext& gameContext) override;

private:

	float m_Length;
	XMFLOAT4 m_Color = {1,1,1,1};

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	HexagonPrefab(const HexagonPrefab& yRef) = delete;
	HexagonPrefab& operator=(const HexagonPrefab& yRef) = delete;
};

