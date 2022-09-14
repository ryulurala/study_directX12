#include "pch.h"
#include "Game.h"

#include "Engine.h"
#include "CommandQueue.h"

#include "Mesh.h"
#include "Shader.h"

shared_ptr<Mesh> mesh = make_shared<Mesh>();
shared_ptr<Shader> shader = make_shared<Shader>();

void Game::Init(const WindowInfo& window)
{
	GEngine->Init(window);

	// Vertex buffer
	vector<Vertex> vertexVec(4);
	vertexVec[0].pos = Vec3(-0.5f, 0.5f, 0.5f);
	vertexVec[0].color = Vec4(1.f, 0.f, 0.f, 1.f);
	vertexVec[1].pos = Vec3(0.5f, 0.5f, 0.5f);
	vertexVec[1].color = Vec4(0.f, 1.f, 0.f, 1.f);
	vertexVec[2].pos = Vec3(0.5f, -0.5f, 0.5f);
	vertexVec[2].color = Vec4(0.f, 0.f, 1.f, 1.f);
	vertexVec[3].pos = Vec3(-0.5f, -0.5f, 0.5f);
	vertexVec[3].color = Vec4(0.f, 1.f, 0.f, 1.f);

	// Index buffer
	vector<uint32> indexVec;
	{
		indexVec.push_back(0);
		indexVec.push_back(1);
		indexVec.push_back(2);
	}
	{
		indexVec.push_back(0);
		indexVec.push_back(2);
		indexVec.push_back(3);
	}

	mesh->Init(vertexVec, indexVec);

	shader->Init(L"..\\Resources\\Shader\\default.hlsli");

	GEngine->GetCmdQueue()->WaitSync();
}

void Game::Update()
{
	GEngine->RenderBegin();

	shader->Update();

	Transform t;
	t.offset = Vec4(0.f, 0.f, 0.f, 0.f);
	mesh->SetTransform(t);
	mesh->Render();

	//GEngine->Render();
	
	GEngine->RenderEnd();
}
