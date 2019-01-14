#include "Includes/Renderer.h"

bool Renderer::Retro_Mode = false;	//	Used to control retro mode

Renderer::Renderer ( std::shared_ptr < Window > window ) : window ( window )
{
	if ( glewInit (  ) != GLEW_OK )
		std::cerr << "Failed To Initialize GLEW" << std::endl;

	glEnable ( GL_DEPTH_TEST );
	
	glEnable ( GL_CULL_FACE );
	glCullFace ( GL_BACK );

	glEnable ( GL_POLYGON_SMOOTH );
	
	glClearColor ( 0.0, 0.0, 0.2, 1.0 );
}

Renderer::~Renderer (  )
{
}

/*
	Initialize

	Initialize the renderer, by setting up the shaders and perspective matrix.
*/
void Renderer::Initialize (  )
{
	ecs = ServiceLocator::LocateEntityComponentSystem (  );

	program = std::make_unique < ShaderProgram > (  );
	program->CreateShader ( "resources/vertex.glsl", GL_VERTEX_SHADER );
	program->CreateShader ( "resources/fragment.glsl", GL_FRAGMENT_SHADER );
	program->CompileShaders (  );
	program->LinkProgram (  );

	projection = glm::perspective ( glm::radians ( 60.0f ), 1280.0f / 720.0f, 0.1f, 100.0f );
	player = ecs->GetEntity ( 0 );

	SetupColliderData (  );
}

/*
	SetupColliderData

	Buffers the data used to render a collider.
*/
void Renderer::SetupColliderData (  )
{

	//	Collider Vertices
	std::vector < float > vertices =
	{
		-0.5, -0.5, -0.5,
		0.5, -0.5, -0.5, 
		0.5,  0.5, -0.5, 
		-0.5,  0.5, -0.5, 
		-0.5, -0.5,  0.5, 
		0.5, -0.5,  0.5, 
		0.5,  0.5,  0.5, 
		-0.5,  0.5,  0.5
	};

	//	Collider Indices
	std::vector < unsigned int > indices = 
	{
		0, 1, 2, 3, 
		4, 5, 6, 7,
		0, 4, 1, 5, 
		2, 6, 3, 7
	};

	//	Generate vertex array
	glGenVertexArrays ( 1, &collider_vao );
	glBindVertexArray ( collider_vao );

	//	Generate vertex buffer
	glGenBuffers ( 1, &collider_vbo );
	glBindBuffer ( GL_ARRAY_BUFFER, collider_vbo );
	glBufferData ( GL_ARRAY_BUFFER, vertices.size (  ) * sizeof ( float ), &vertices [ 0 ], GL_STATIC_DRAW );

	//	Assign pointer to shader attribute
	glVertexAttribPointer ( 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof ( float ), ( void* ) 0 );

	//	Generate index buffer
	glGenBuffers ( 1, &collider_ebo );
	glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, collider_ebo );
	glBufferData ( GL_ELEMENT_ARRAY_BUFFER, indices.size (  ) * sizeof ( unsigned int ), &indices [ 0 ], GL_STATIC_DRAW );

	//	Unbind buffers and vertex array
	glBindBuffer ( GL_ARRAY_BUFFER, 0 );
	glBindVertexArray ( 0 );
}

/*
   Render
   Iterates through each model in the entity-component system,
   and renders it
   */
void Renderer::Render (  )
{
	//	Clear the screen
	Clear (  );

	//	Use the shader program
	program->UseProgram (  );

	//	Retrieve the player transform for the camera
	Transform* player_transform = ecs->GetTransform ( player->id );
	glm::mat4 view = CreateView ( player_transform );

	//	Set the projection matrix in the shader
	int proj_loc = glGetUniformLocation ( program->GetProgramID (  ), "projection" );
	glUniformMatrix4fv ( proj_loc, 1, GL_FALSE, &projection [ 0 ] [ 0 ] );

	//	Set the view matrix in the shader
	int view_loc = glGetUniformLocation ( program->GetProgramID (  ), "view" );
	glUniformMatrix4fv ( view_loc, 1, GL_FALSE, &view [ 0 ] [ 0 ] );

	//	Retrieve the location of the model matrix from the vertex shader
	int model_loc = glGetUniformLocation ( program->GetProgramID (  ), "model" );

	//	Iterate through each model
	for ( Model model : ecs->GetModels (  ) )	
	{
		//	Retrieve the transform for the entity
		Transform* entity_transform = ecs->GetTransform ( model.entity );

		//	Create the transformation
		glm::mat4 m = CreateTransformation ( entity_transform );

		glUniform1i ( glGetUniformLocation ( program->GetProgramID (  ), "debug" ), 0 );	//	TESTING

		//	Send the matrix to the shader
		glUniformMatrix4fv ( model_loc, 1, GL_FALSE, &m [ 0 ] [ 0 ] );

		//	Render the model
		RenderModel ( model );
	}

	//	Render the box colliders
	RenderBoxColliders (  );

	//	Swaps the window buffers
	SDL_GL_SwapWindow ( window->GetWindowHandle (  ) );
}

/*
   Clear
   Clears the color and the depth buffer
   */
void Renderer::Clear (  )
{
	glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}

/*
	RenderModel

	Renders a model in the game world.
*/
void Renderer::RenderModel ( Model& model )
{
	// Cache model data
	const ModelData data = ServiceLocator::LocateResourceManager (  )->GetModel ( model.filename );

	for ( int i = 0; i < data.meshes.size (  ); i++ )
	{
		// Cache the mesh
		Mesh mesh = data.meshes [ i ];

		// Bind textures
		for ( int i = 0; i < mesh.textures.size (  ); i++ )
		{
			//	Activate the corresponding texture unit - GL_TEXTURE0 = 0
			glActiveTexture ( GL_TEXTURE0 + i );

			//	The texture name
			std::string name;

			//	Checks the texture type
			if ( mesh.textures [ i ].type == DIFFUSE )
			{
				name = "diffuse" + i;
			}

			// Set Texture Unit For the sampler
			glUniform1i ( glGetUniformLocation ( program->GetProgramID (  ), name.c_str (  ) ), i );

			//	Bind the texture to the activated texture unit
			glBindTexture ( GL_TEXTURE_2D, mesh.textures [ i ].texture_id );
		}

		// Render Mesh
		glBindVertexArray ( mesh.vertex_array );

		glEnableVertexAttribArray ( 0 );
		glEnableVertexAttribArray ( 1 );
		glEnableVertexAttribArray ( 2 );

		//	Determine whether to draw filled polygons or lines
		glPolygonMode ( GL_FRONT_AND_BACK, ( Retro_Mode ) ? GL_LINE : GL_FILL );
		glDrawElements ( GL_TRIANGLES, mesh.indices.size (  ), GL_UNSIGNED_INT, NULL );

		glDisableVertexAttribArray ( 2 );
		glDisableVertexAttribArray ( 1 );
		glDisableVertexAttribArray ( 0 );

		glBindVertexArray ( 0 );

		glActiveTexture ( GL_TEXTURE0 );
	}
}

/*
	RenderBoxColliders

	Renders each of the box colliders in the game world.
*/
void Renderer::RenderBoxColliders (  )
{
	glBindVertexArray ( collider_vao );
	glEnableVertexAttribArray ( 0 );

	glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, collider_ebo );

	for ( BoxCollider collider : ecs->GetBoxColliders (  ) )
	{
		Transform* t = ecs->GetTransform ( collider.entity );

		glm::mat4 transformation = glm::mat4 ( 1.0f );
		transformation = glm::translate ( transformation, t->position );
		transformation = glm::rotate ( transformation, glm::radians ( t->rotation.y ), glm::vec3 ( 0, 1, 0 ) );
		transformation = glm::scale ( transformation, t->scale );
		transformation = transformation * glm::translate ( glm::mat4 ( 1 ), glm::vec3 ( collider.local_center.x, collider.local_center.y, collider.local_center.z ) ) 
			* glm::scale ( glm::mat4 ( 1 ), collider.size );

		glUniformMatrix4fv ( glGetUniformLocation ( program->GetProgramID (  ), "model" ), 1, GL_FALSE, &transformation [ 0 ] [ 0 ] );
		glUniform1i ( glGetUniformLocation ( program->GetProgramID (  ), "debug" ), 1 );

		glDrawElements ( GL_LINE_LOOP, 4, GL_UNSIGNED_INT, 0 );
		glDrawElements ( GL_LINE_LOOP, 4, GL_UNSIGNED_INT, ( void* ) ( 4 * sizeof ( unsigned int ) ) );
		glDrawElements ( GL_LINES, 8, GL_UNSIGNED_INT, ( void* ) ( 8 * sizeof ( unsigned int ) ) );

	}

	glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, 0 );

	glDisableVertexAttribArray ( 0 );
	glBindVertexArray ( 0 );
}

/*
   CreateTransformation
   Uses the supplied transform to create a transformation matrix
   */
glm::mat4 Renderer::CreateTransformation ( Transform* transform )
{
	glm::mat4 transformation = glm::mat4 ( 1 );

	transformation = glm::translate ( transformation, transform->position );
	transformation = glm::rotate ( transformation, glm::radians ( transform->rotation.z ), glm::vec3 ( 0, 0, 1 ) );
	transformation = glm::rotate ( transformation, glm::radians ( transform->rotation.y ), glm::vec3 ( 0, 1, 0 ) );
	transformation = glm::rotate ( transformation, glm::radians ( transform->rotation.x ), glm::vec3 ( 1, 0, 0 ) );
	transformation = glm::scale ( transformation, transform->scale );

	//	Extract forward vector for movement
	transform->forward = glm::vec3 ( transformation [ 2 ][ 0 ], transformation [ 2 ][ 1 ], transformation [ 2 ][ 2 ] );

	return transformation;
}

/*
   CreateView
   Uses the supplied transform to create a view matrix
   */
glm::mat4 Renderer::CreateView ( Transform* transform )
{
	// Derived From Rotation Matrix - Enables player to walk in the direction they are looking
	transform->forward = glm::vec3 ( glm::sin ( glm::radians ( transform->rotation.y ) ), 0, glm::cos ( glm::radians ( transform->rotation.y ) ) );

	// Rotates for Look Matrix - Derived From Rotation Around X and Y Axes
	glm::vec3 forward = glm::vec3 ( transform->forward.x * glm::cos ( glm::radians ( transform->rotation.x ) ), -glm::sin ( glm::radians ( transform->rotation.x ) ), transform->forward.z * glm::cos ( glm::radians ( transform->rotation.x ) ) );

	// Look in direction
	glm::mat4 view = glm::lookAt ( transform->position, transform->position + forward, glm::vec3 ( 0, 1, 0 ) );

	return view;
}
