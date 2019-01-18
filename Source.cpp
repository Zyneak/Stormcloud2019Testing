#include <GL/gl3w.h>
#include <SFML/Window.hpp>
#include <Stormcloud.h>
#include <GameObject3D.h>
#include <vector>
#include <iostream>

// Shader sources
const GLchar* vertexSource = R"glsl(
    #version 330 core
    layout(location = 0)in vec3 position;
	uniform mat4 MVP;
    layout(location = 1) in vec3 color;
    out vec3 Color;
	layout(location = 2) in mat4 modelMatrix;
    void main()
    {
        Color = color;
        gl_Position = MVP * modelMatrix * vec4(position, 1.0);
    }
)glsl";
const GLchar* fragmentSource = R"glsl(
    #version 330 core
    in vec3 Color;
    out vec4 outColor;
    void main()
    {
        outColor = vec4(Color, 1.0);
    }
)glsl";


int main()
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 16;
	settings.depthBits = 24;
	settings.stencilBits = 8;

	sf::Window window(sf::VideoMode(800, 600, 32), "OpenGL", sf::Style::Titlebar | sf::Style::Close, settings);

	// Initialize GLEW
	if (gl3wInit()) {
		fprintf(stderr, "failed to initialize OpenGL\n");
		return -1;
	}
	glEnable(GL_DEPTH_TEST);

	// Create Vertex Array Object
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Create a Vertex Buffer Object and copy the vertex data to it
	GLuint vbo;
	glGenBuffers(1, &vbo);

	GLfloat vertices[] = {
			-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
			 0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
			 0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,

			-0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 0.0f,

			-0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
			-0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
			-0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f,

			 0.5f, 0.5f,  0.5f, 1.0f, 0.0f, 1.0f,
			 0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f,
			 0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 1.0f,
			 0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 1.0f,

			-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f,
			 0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f,
			 0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			 0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f,

			-0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 0.0f,

			1000.0f,0.f, 1000.0f,1.0f, 1.0f, 1.0f,
			1000.f, 0.f,-1000.0f,1.0f, 1.0f, 1.0f,
			-1000.0f,0.f, 1000.0f,1.0f, 1.0f, 1.0f,
			-1000.0f,0.f, 1000.0f, 1.0f, 1.0f, 1.0f,
			1000.f,0.f, -1000.0f, 1.0f, 1.0f, 1.0f,
			-1000.f,0.f, -1000.0f, 1.0f, 1.0f, 1.0f,
	};

	std::vector<glm::mat4> models = std::vector<glm::mat4>(); 
	for (int x = 0; x < 100; x++) {
		for (int y = 0; y < 100; y++) {
			models.push_back(glm::translate(glm::mat4(), glm::vec3(x*2- 100/2, 0, y * 2 - 100/2)));
		}
	}
	/*
	models.push_back(glm::rotate(glm::mat4(), glm::radians(30.0f), glm::vec3(1, 0, 0)));
	models.push_back(glm::rotate(glm::mat4(), glm::radians(40.0f), glm::vec3(1, 0, 0)));
	models.push_back(glm::rotate(glm::mat4(), glm::radians(50.0f), glm::vec3(1, 0, 0)));
	models.push_back(glm::rotate(glm::mat4(), glm::radians(60.0f), glm::vec3(1, 0, 0)));
	models.push_back(glm::rotate(glm::mat4(), glm::radians(60.0f), glm::vec3(1, 0, 0)));*/
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	GLuint modelVbos;
	glGenBuffers(1, &modelVbos);
	glBindBuffer(GL_ARRAY_BUFFER, modelVbos);
	glBufferData(GL_ARRAY_BUFFER, models.size() * sizeof(glm::mat4), &models[0], GL_STATIC_DRAW);


	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);
	GLint success = 0;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE) {
		GLint maxLength = 0;
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> errorLog(maxLength);
		glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &errorLog[0]);
		for (int i = 0; i < errorLog.size(); i++) {
			std::cout << errorLog[i];
		}
		std::cout << std::endl;
		// Provide the infolog in whatever manor you deem best.
		// Exit with failure.
		glDeleteShader(vertexShader); // Don't leak the shader.
	}

	// Create and compile the fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);
	success = 0;
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE) {
		GLint maxLength = 0;
		glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> errorLog(maxLength);
		glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &errorLog[0]);
		for (int i = 0; i < errorLog.size(); i++) {
			std::cout << errorLog[i];
		}
		std::cout << std::endl;
		// Provide the infolog in whatever manor you deem best.
		// Exit with failure.
		glDeleteShader(fragmentShader); // Don't leak the shader.
	}

	// Link the vertex and fragment shader into a shader program
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glBindFragDataLocation(shaderProgram, 0, "outColor");
	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);
	success = 0;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	GLchar infoLog[512];

	if (success == GL_FALSE) {
		GLint maxLength = 0;
		glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &maxLength);
		maxLength = 256;

		// The maxLength includes the NULL character
		std::vector<GLchar> errorLog(256);

		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		// Provide the infolog in whatever manor you deem best.
		// Exit with failure.
		glDeleteShader(fragmentShader); // Don't leak the shader.
		glDeleteShader(shaderProgram); // Don't leak the shader.
		glDeleteShader(vertexShader); // Don't leak the shader.
		system("pause");
		return 0;
	}

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	// Specify the layout of the vertex data
	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);

	GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));


	GLint pos = glGetAttribLocation(shaderProgram, "modelMatrix");
	std::cout << pos << std::endl;
	GLint pos1 = pos + 0;
	GLint pos2 = pos + 1;
	GLint pos3 = pos + 2;
	GLint pos4 = pos + 3;
	glEnableVertexAttribArray(pos1);
	glEnableVertexAttribArray(pos2);
	glEnableVertexAttribArray(pos3);
	glEnableVertexAttribArray(pos4);
	glBindBuffer(GL_ARRAY_BUFFER, modelVbos);
	glVertexAttribPointer(pos1, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(0));
	glVertexAttribPointer(pos2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
	glVertexAttribPointer(pos3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
	glVertexAttribPointer(pos4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));
	glVertexAttribDivisor(pos1, 1);
	glVertexAttribDivisor(pos2, 1);
	glVertexAttribDivisor(pos3, 1);
	glVertexAttribDivisor(pos4, 1);

	GLint uniProj = glGetUniformLocation(shaderProgram, "MVP");
	glm::mat4 proj = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 1.0f, 1000.0f);
	glm::mat4 view = glm::lookAt(glm::vec3(20.5f, 20.5f, 20.0f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR)
	{
		std::cout << err << std::endl;
	}
	float r = 0;

	bool running = true;
	sf::Vector2i lastPosition = sf::Mouse::getPosition(window);
	sf::Vector2i mouseDelta = sf::Vector2i();
	while (running)
	{
		
		mouseDelta = sf::Mouse::getPosition(window) - lastPosition;
		lastPosition = sf::Mouse::getPosition(window);
		std::cout << "Mouse delta x, " << mouseDelta.x << " y, " << mouseDelta.y << std::endl;
		sf::Event windowEvent;
		while (window.pollEvent(windowEvent))
		{
			switch (windowEvent.type)
			{
			case sf::Event::Closed:
				running = false;
				break;
			}
		}

		// Clear the screen to black
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



		// Draw a rectangle from the 2 triangles using 6 indices

		glm::mat4 model = glm::translate(glm::mat4(), glm::vec3(0, -0.5f, 0));

		glm::mat4 f = proj * view * glm::mat4();
		glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(f));
		glDrawArraysInstanced(GL_TRIANGLES, 0, 36, 10000);
		//glDrawArrays(GL_TRIANGLES, 0, 36);
		glm::mat4 f2 = proj * view * glm::mat4();
		glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(f2));
		glDrawArrays(GL_TRIANGLES, 36, 6);
		r += 0.0001;
		// Swap buffers
		window.display();
	}

	glDeleteProgram(shaderProgram);
	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);

	glDeleteBuffers(1, &vbo);

	glDeleteVertexArrays(1, &vao);

	window.close();

	return 0;
}