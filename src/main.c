// main.c

#include <cbeta/main.h>

int main() {

	// make it use wayland by default
	SDL_SetHint(SDL_HINT_VIDEODRIVER, "wayland,x11");

	// Init
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return -1;
	}

	// Opengl setup
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	// Create window
	SDL_Window* window = SDL_CreateWindow(
		"cbeta",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		800, 600,
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
	);

	if (!window) {
		printf("failed to make window: %s\n", SDL_GetError());
		return -1;
	}

	// Create context
	SDL_GLContext ctx = SDL_GL_CreateContext(window);
	if (!ctx) {
		printf("OpenGL ES 3.0 error: %s\n", SDL_GetError());
		SDL_DestroyWindow(window);
		SDL_Quit();
		return -1;
	}

	SDL_GL_SetSwapInterval(1);

	// log
	printf("Video Driver: %s\n", SDL_GetCurrentVideoDriver());
	printf("GL Renderer: %s\n", glGetString(GL_RENDERER));
	printf("GL Version: %s\n", glGetString(GL_VERSION));

	// vertices
	GLfloat vertices[] = {
		// front
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		
		// back
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		// top
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,

		//bottom
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		// left
		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		// right
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f
	};

	// colors
	GLfloat colors[] = {
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,

		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,

		0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f,

		1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f,

		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
	};

	struct cb_camera camera;
	cb_camera_init(&camera);

	// event loop
	uint64_t lt = SDL_GetTicks64();
	bool running = true;
	SDL_Event event;
	while (running) {
		// time
		uint64_t ct = SDL_GetTicks64();
		uint64_t dt = ct - lt;
		lt = ct;

		// events
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT)
				running = false;
			else if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED) {
				glViewport(0, 0, event.window.data1, event.window.data2);
			} else if (event.type == SDL_MOUSEMOTION) {
				cb_camera_handle_mouse(&camera, event.motion.xrel, event.motion.yrel);
			}
		}

		const uint8_t* state = SDL_GetKeyboardState(NULL);

		cb_camera_handle_keys(&camera, state, dt);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glEnable(GL_DEPTH_TEST);

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);

		glVertexPointer(3, GL_FLOAT, 0, vertices);
		glColorPointer(3, GL_FLOAT, 0, colors);

		glDrawArrays(GL_QUADS, 0, 24);

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glFrustum(-1.0, 1.0, -0.75, 0.75, 1.0, 100.0);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glRotatef(camera.pitch, 1.0f, 0.0f, 0.0f);
		glRotatef(camera.yaw + 90, 0.0f, 1.0f, 0.0f);
		glTranslatef(-camera.x, -camera.y, -camera.z - 5.0f);

		SDL_GL_SwapWindow(window);
	}

	SDL_GL_DeleteContext(ctx);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
