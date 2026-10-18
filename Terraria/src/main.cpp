#include <Engine.h>
#include "TerrariaApp.h"

int main() {
	Engine::Log::Init();
	Engine::App* Terraria = new Engine::App();
	Terraria->PushLayer(new ::Terraria::TerrariaLayer());
	Terraria->Run();
	delete Terraria;
}