#pragma once

#include "BasicIncludesAndDefines.h"
#include "VisibleObject.h"

struct Texture {

	GLuint id;
	string type;
	aiString path;
};

class Model :public VisibleObject
{
public :

	Model(const string filename);
	~Model(void);

	virtual void initialize();

	virtual void draw();

	struct MeshEntry  {

		enum BUFFERS { VERTEX_BUFFER, TEXCOORD_BUFFER, NORMAL_BUFFER, INDEX_BUFFER };
		GLuint vertexArrayObject;
		GLuint vbo[4];

		unsigned int numberOfIndices;

		// Constructor - Uses
		MeshEntry(aiMesh *mesh, const aiScene* scene, const string directory);
		~MeshEntry();

		// Loads an individual mesh and its material properties
		void load(aiMesh *mesh);

		void draw(GLint shaderProgram);

		// Loads the texture associate with the mesh
		GLuint loadMaterialTexture(aiMaterial* mat, aiTextureType type, string typeName);

		// Holds the Material properties and textures for the MeshEntry
		Material meshMaterial;

		// Stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
		static vector<Texture> textures_loaded;

		// Store the directory in which the model is loaded
		const string directory;
	};

	// Vector of all the individual meshes contained in the model
	std::vector<MeshEntry*> meshEntries;

	// Name and path for the file containig the model
	const string fileName;
};

