#include "stdafx.h"
#include "BiomGenerator.h"

#include "Scenegraph\GameObject.h"
#include "Components\Components.h"
#include "Prefabs\Prefabs.h"
#include <ctime>

BiomGenerator::BiomGenerator() :
	GameScene(L"BiomGenerator")
{
}

BiomGenerator::~BiomGenerator()
{
}

void BiomGenerator::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	//More random
	srand(unsigned int(time(nullptr)));
	
	//--- CAMERA ---//
	m_pCamera = GetGameContext().pCamera->GetGameObject();

	if (LENGTH_COLS > LENGTH_ROWS)
	{
		m_CamPosY = LENGTH_COLS + (m_Size * 1.5) * 5;
	}
	else
	{
		m_CamPosY = LENGTH_ROWS + (m_Size * 1.5) * 5;
	}

	//Hexagon test
	//auto pHexagon = new HexagonPrefab(12, {1,0,0,1});
	//pHexagon->GetTransform()->Rotate(90,0,0,true);
	//pHexagon->GetTransform()->Translate(0, 10, 0);
	//AddChild(pHexagon);

	//--- BIOME DECLERATION ---//
	m_vColors.push_back({ 0,0,1,1 });
	m_vColors.push_back({ 0,1,0,1 });
	m_vColors.push_back({ 1,0,0,1 });
	m_vColors.push_back({ 1,1,1,1 });

	//--- NOISE ---//
	for (auto i = 0; i < m_Cols * m_Rows; i++)
	{
		m_vBiomes.push_back(XMFLOAT4(0, 0, 0, 0)); //Initialize w at 0, for valid check
	}
	Noise();

	//--- BIOME EXPENSION ---//
	float offsetX = 0;
	bool option1 = true;
	while (m_GeneratedTiles != m_Cols * m_Rows)
	{
		SetUpGrid(offsetX);
		if (option1)
		{
			ExpandBiome(m_vOpenBiomes, m_vOpenBiomes2);
			option1 = !option1;
		}
		else
		{
			ExpandBiome(m_vOpenBiomes2, m_vOpenBiomes);
			option1 = !option1;
		}
		offsetX += LENGTH_COLS + (m_Size * 1.5) * 12;
	} 

	//--- CAMERA INPUT ---//

	InputAction left = InputAction(0, Pressed, 'A');
	gameContext.pInput->AddInputAction(left);

	InputAction right = InputAction(1, Pressed, 'D');
	gameContext.pInput->AddInputAction(right);
}

void BiomGenerator::Noise()
{
	//Amount can't be higher then rows * cols
	int amountRand =  rand() % ((m_Cols * m_Rows) / 7) + 2;

	//Create random start nodes
	for (auto i = 0; i < amountRand; i++)
	{
		m_vOpenBiomes.push_back(RandomNumber());
	}

	//Create random Biomes for those nodes
	for (auto number : m_vOpenBiomes)
	{
		//Log in vector
		//Logger::LogInfo(std::to_wstring(number), false);

		//Random Biomes
		m_vBiomes[number] = m_vColors[rand() % m_vColors.size()];
	}
}

void BiomGenerator::ExpandBiome(vector<int>& vecInRef, vector<int>& vecOutRef)
{
	auto checkNode = [](float toCheck)//Lambda to check if a biom is already assigned
	{
		if (toCheck == 1)
		{
			return false;
		}
		return true;
	};

	for(auto number: vecInRef)
	{
		//Top
		if (!(number + m_Cols > m_Rows*m_Cols - 1)) //-1 for vector -> [0, (m_Cols*m_Rows) - 1]
		{
			if (checkNode(m_vBiomes[number + m_Cols].w))
			{
				m_vBiomes[number + m_Cols] = m_vBiomes[number];
				vecOutRef.push_back(number + m_Cols);
			}

			if ((number / m_Cols) % 2 == 0 && checkNode(m_vBiomes[number + m_Cols - 1].w))
			{
				m_vBiomes[number + m_Cols - 1] = m_vBiomes[number];
				vecOutRef.push_back(number + m_Cols - 1);
			}
			else if ((number / m_Cols) % 2 == 1 && number != (m_Cols * m_Rows) - (1 + m_Cols) && checkNode(m_vBiomes[number + m_Cols + 1].w))
			{
				//number check here so that the + 1 doesn't go out of range -> ex. 100 tiles, 89 passes top check but 89 + 1 + 10(cols) = 100 
				m_vBiomes[number + m_Cols + 1] = m_vBiomes[number];
				vecOutRef.push_back(number + m_Cols + 1);
			}
		}
		//Bottom
		if (!(number - m_Cols < 0))
		{
			if (checkNode(m_vBiomes[number - m_Cols].w))
			{
				m_vBiomes[number - m_Cols] = m_vBiomes[number];
				vecOutRef.push_back(number - m_Cols);
			}
			if ((number / m_Cols) % 2 == 0 && number != 1 + m_Cols && checkNode(m_vBiomes[number - m_Cols - 1].w))
			{
				//number check here so that the - 1 doesn't go out of range -> ex. 100 tiles, 11 passes top check but 11 - 1 - 10(cols) = -1 
				m_vBiomes[number - m_Cols - 1] = m_vBiomes[number];
				vecOutRef.push_back(number - m_Cols - 1);
			}
			else if((number / m_Cols) % 2 == 1 && checkNode(m_vBiomes[number - m_Cols + 1].w))
			{
				m_vBiomes[number - m_Cols + 1] = m_vBiomes[number];
				vecOutRef.push_back(number - m_Cols + 1);
			}
		}
		//Right
		if (!(number >= m_Rows*m_Cols - 2) && checkNode(m_vBiomes[number + 1].w))
		{
			m_vBiomes[number + 1] = m_vBiomes[number];
			vecOutRef.push_back(number + 1);
		}
		//Left
		if (!(number <= 0) && checkNode(m_vBiomes[number - 1].w))
		{
			m_vBiomes[number - 1] = m_vBiomes[number];
			vecOutRef.push_back(number - 1);
		}
	}

	m_GeneratedTiles += vecInRef.size();
	Logger::LogInfo(L"Tiles generated: " + std::to_wstring(m_GeneratedTiles));

	vecInRef.clear();
	std::random_shuffle(vecOutRef.begin(), vecOutRef.end());

	//Camera
	++m_TotalIterations;
}

int BiomGenerator::RandomNumber()
{
	int startNode = rand() % (m_Cols * m_Rows);

	//Log every generated number
	Logger::LogInfo(L"Generated number: " + std::to_wstring(startNode), false);

	//check if random number is already used
	for (auto it = 0; it < m_vOpenBiomes.size(); it++)
	{
		if (m_vOpenBiomes[it] == startNode) //if so generate anothre number
		{
			return RandomNumber();
		}
	}

	return startNode;
}

void BiomGenerator::SetUpGrid(float offsetX)
{
	float innerRadius = 2 * m_Size * 0.866025404f;
	for (auto col = 0; col < m_Cols; col++)
	{
		float posZ = 0;

		for (auto row = 0; row < m_Rows; row++)
		{
			float posX = (col + row * 0.5f - row / 2) * innerRadius;

			//Create new Hexagon with the right biome
			auto pHexagon = new HexagonPrefab(m_Size, m_vBiomes[col + (m_Cols * row)]);

			//Set the position rotation and add to scene
			pHexagon->GetTransform()->Rotate(90, 0, 0, true);
			pHexagon->GetTransform()->Translate(posX + offsetX, 5, posZ);
			AddChild(pHexagon);

			posZ = (row + 1) * m_Size * 1.5f;
		}
	}
}

void BiomGenerator::Update(const GameContext& gameContext)
{
	if (gameContext.pInput->IsActionTriggered(0)) //left
	{
		m_CurrentIteration -= 1;
		if (m_CurrentIteration < 0)
		{
			m_CurrentIteration = m_TotalIterations - 1;
		}
	}
	if (gameContext.pInput->IsActionTriggered(1)) //right
	{
		m_CurrentIteration += 1;
		if (m_CurrentIteration >= m_TotalIterations)
		{
			m_CurrentIteration = 0;
		}
	}
	
	m_pCamera->GetTransform()->Translate(
		LENGTH_COLS / 2 + (m_CurrentIteration * LENGTH_COLS) + ((m_Size * 1.5) * 12) * m_CurrentIteration,
		m_CamPosY,
		LENGTH_ROWS/2 - (m_Size * 1.5));

	UNREFERENCED_PARAMETER(gameContext);
}

void BiomGenerator::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}
