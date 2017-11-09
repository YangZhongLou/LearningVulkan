#include "SkyBox.h"
#include <Vulkan/VulkanRenderer.h>
#include <Mesh.h>

int main()
{
	using namespace yzl;
	yzl::SkyBox app("First", 200, 200, 800, 600);
	auto cube = app.LoadMesh("Resources/Models/cube.obj", false, false, false, false);
	VulkanRenderer * render = static_cast<VulkanRenderer*>(app.GetRenderer());
	render->CreateVertexBuffer(sizeof(cube->GetData()[0]) * cube->GetData().size());

	app.Run();

	return 0;
}