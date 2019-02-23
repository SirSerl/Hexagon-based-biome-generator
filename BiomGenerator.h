#pragma once
#include "Scenegraph/GameScene.h"

class HexagonPrefab;

class BiomGenerator : public GameScene
{
public:
	BiomGenerator();
	~BiomGenerator();

protected:

	virtual void Initialize(const GameContext& gameContext) override;
	virtual void Update(const GameContext& gameContext) override;
	virtual void Draw(const GameContext& gameContext) override;
	virtual void SceneActivated() override {};
	virtual void SceneDeactivated() override {};

private:

	int m_Rows = 10; //Amount of rows
	int m_Cols = 10; //Amount of columns
	float m_Size = 12; //Size of the hexagons

	int m_GeneratedTiles = 0;

	std::vector<XMFLOAT4> m_vColors; //Color of biomes that can be generated
	std::vector<int> m_vOpenBiomes; //First vec to generate
	std::vector<int> m_vOpenBiomes2; //Second vec to generate
	std::vector<XMFLOAT4> m_vBiomes; //The generated biomes

	void Noise(); //Create initial nodes
	void BiomGenerator::ExpandBiome(vector<int>& vecInRef, vector<int>& vecOutRef); //Expand the biome
	int RandomNumber(); //return a random number that isn't generated yet
	void SetUpGrid(float yPos); //draw the biomes

	//Camera vars
	GameObject* m_pCamera = nullptr;
	const float LENGTH_COLS = (m_Size * 1.5) * (m_Cols + 1);
	const float LENGTH_ROWS = (m_Size * 1.5) * (m_Rows + 1);
	int m_CurrentIteration = 0;
	int m_TotalIterations = 0;
	int m_CamPosY = 0;

	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	BiomGenerator(const BiomGenerator &obj) = delete;
	BiomGenerator& operator=(const BiomGenerator& obj) = delete;
};

