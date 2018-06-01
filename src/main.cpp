#include <iostream>
#include <iomanip>
#include <string>
#include <GL/glew.h>

#include "./KHeader.h"
#include "./Log.h"
#include "./Render/MazeRenderer.h"
#include "./Render/TestRenderer.h"

 int main() {

#if 1
	 auto mazeRenderer = new KEngine::KRenderer::MazeRenderer("maze");

	 mazeRenderer->exec();

	 delete mazeRenderer;

#else

	 auto test = new KEngine::KRenderer::TestRenderer();

	 test->exec();

	 delete test;

#endif

	return 0;
 }
