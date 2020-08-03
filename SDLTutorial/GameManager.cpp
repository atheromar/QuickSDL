//---------------------------------------------------------------------//
// GameManager.cpp                                                     //
// Used to intialize and release all other manager                     //
// Contains the game loop as well as the Update and Render functions   //
// Used to make sure all functions are called in the correct order     //
//                                                                     //
// By: Ather Omar                                                      //
//---------------------------------------------------------------------//
#include "GameManager.h"
#include <time.h>
#include <iostream>
#include "PerformanceTimer.h"
#include "InsertionSortAlg.h"
#include "SelectionSortAlg.h"
#include "BubbleSortAlg.h"
//-----------------------------------------------------------
// QuickSDL
//-----------------------------------------------------------
namespace QuickSDL {
	//Initializing to NULL
	GameManager* GameManager::sInstance = NULL;

	GameManager* GameManager::Instance() {

		//Create a new instance if no instance was created before
		if(sInstance == NULL)
			sInstance = new GameManager();

		return sInstance;
	}

	void GameManager::Release() {

		delete sInstance;
		sInstance = NULL;
	}

	GameManager::GameManager() {

		srand((unsigned int)time(0));

		mQuit = false;

		//Initialize SDL
		mGraphics = Graphics::Instance();

		// Quits the game if SDL fails to initialize
		if(!Graphics::Initialized())
			mQuit = true;

		//Initialize AssetManager
		mAssetMgr = AssetManager::Instance();

		//Initialize InputManager
		mInputMgr = InputManager::Instance();

		//Initialize AudioManager
		mAudioMgr = AudioManager::Instance();

		//Initialize Timer
		mTimer = Timer::Instance();

		mPhysMgr = PhysicsManager::Instance();
		mPhysMgr->SetLayerCollisionMask(PhysicsManager::CollisionLayer::Friendly, PhysicsManager::CollisionFlags::Hostile | PhysicsManager::CollisionFlags::HostileProjectiles);
		mPhysMgr->SetLayerCollisionMask(PhysicsManager::CollisionLayer::FriendlyProjectiles, PhysicsManager::CollisionFlags::Hostile);
		mPhysMgr->SetLayerCollisionMask(PhysicsManager::CollisionLayer::Hostile, PhysicsManager::CollisionFlags::Friendly | PhysicsManager::CollisionFlags::FriendlyProjectiles);
		mPhysMgr->SetLayerCollisionMask(PhysicsManager::CollisionLayer::HostileProjectiles, PhysicsManager::CollisionFlags::Friendly);


		std::vector<int> randomNums;

		for (int i = 0; i < 100; i++)
		{
			randomNums.push_back(rand() % 1000 + 1);
		}

		mVisualizers.push_back(new Visualizer<InsertionSortAlg>());
		mVisualizers.push_back(new Visualizer<SelectionSortAlg>());
		mVisualizers.push_back(new Visualizer<BubbleSortAlg>());

		mVisualizerGridDivisions = ceil(sqrt(mVisualizers.size()));
		mVisualizerPanelWidth = Graphics::Instance()->SCREEN_WIDTH / mVisualizerGridDivisions;
		mVisualizerPanelHeight = Graphics::Instance()->SCREEN_HEIGHT / mVisualizerGridDivisions;

		for (unsigned int i = 0; i < mVisualizerGridDivisions; i++)
		{
			for (unsigned int j = 0; j < mVisualizerGridDivisions; j++)
			{
				unsigned int index = i * mVisualizerGridDivisions + j;
				if (index < mVisualizers.size())
				{
					mVisualizers[index]->Initialize(randomNums);
					mVisualizers[index]->Scale(VEC2_ONE * (MAX_VISUALIZER_SCALE / mVisualizerGridDivisions));
					mVisualizers[index]->Pos(Vector2(mVisualizerPanelWidth * 0.5f + mVisualizerPanelWidth * j, mVisualizerPanelHeight * 0.5f + i * mVisualizerPanelHeight));
				}
			}
		}
		mSelectedVisualizer = -1;
	}

	GameManager::~GameManager() {

		for (unsigned int i = 0; i < mVisualizers.size(); i++)
		{
			delete mVisualizers[i];
			mVisualizers[i] = nullptr;
		}

		PhysicsManager::Release();
		mPhysMgr = nullptr;

		AudioManager::Release();
		mAudioMgr = NULL;

		AssetManager::Release();
		mAssetMgr = NULL;

		Graphics::Release();
		mGraphics = NULL;

		InputManager::Release();
		mInputMgr = NULL;

		Timer::Release();
		mTimer = NULL;
	}

	void GameManager::EarlyUpdate() {

		//Updating the input state before any other updates are run to make sure the Input check is accurate
		mInputMgr->Update();
	}

	void GameManager::Update() {

		//GameEntity Updates should happen here
		for (unsigned int i = 0; i < mVisualizers.size(); i++)
		{
			mVisualizers[i]->Update();
		}

		if (mSelectedVisualizer == -1)
		{
			if (mInputMgr->MouseButtonPressed(InputManager::left))
			{
				Vector2 mousePos = mInputMgr->MousePos();
				
				int row = (int)(mousePos.y / mVisualizerPanelHeight);
				int col = (int)(mousePos.x / mVisualizerPanelWidth);

				unsigned int index = (unsigned int)(row * mVisualizerGridDivisions + col);

				if (index < mVisualizers.size())
				{
					mSelectedVisualizer = index;
					mVisualizers[mSelectedVisualizer]->Scale(VEC2_ONE * (MAX_VISUALIZER_SCALE));
					mVisualizers[index]->Pos(Vector2(mGraphics->SCREEN_WIDTH * 0.5f, mGraphics->SCREEN_HEIGHT * 0.5f));
				}
			}
		}
		else
		{
			if (mInputMgr->MouseButtonPressed(InputManager::right))
			{
				int row = (int)(mSelectedVisualizer / mVisualizerGridDivisions);
				int col = mSelectedVisualizer % mVisualizerGridDivisions;
				mVisualizers[mSelectedVisualizer]->Scale(VEC2_ONE * (MAX_VISUALIZER_SCALE / mVisualizerGridDivisions));
				mVisualizers[mSelectedVisualizer]->Pos(Vector2(mVisualizerPanelWidth * 0.5f + mVisualizerPanelWidth * col, mVisualizerPanelHeight * 0.5f + row * mVisualizerPanelHeight));

				mSelectedVisualizer = -1;
			}
		}
	}

	void GameManager::LateUpdate() {

		//Any collision detection should happen here

		mPhysMgr->Update();
		mInputMgr->UpdatePrevInput();
		mTimer->Reset();
	}

	void GameManager::Render() {

		//Clears the last frame's render from the back buffer
		mGraphics->ClearBackBuffer();

		//All other rendering is to happen here
		for (unsigned int i = 0; i < mVisualizers.size(); i++)
		{
			if (mSelectedVisualizer == -1 || mSelectedVisualizer == i)
			{
				mVisualizers[i]->Render();
			}
		}
		//mInsertionVisualizer.Render();

		//Renders the current frame
		mGraphics->Render();
	}

	void GameManager::Run() {

		while(!mQuit) {

			mTimer->Update();

			while(SDL_PollEvent(&mEvents) != 0) {
				//Checks if the user quit the game
				if(mEvents.type == SDL_QUIT) {

					mQuit = true;
				}
			}

			//Limits the frame rate to FRAME_RATE
			if (mTimer->DeltaTime() >= (1.0f / FRAME_RATE)) {

				EarlyUpdate();
				Update();
				LateUpdate();
				Render();
			}
		}
	}
}