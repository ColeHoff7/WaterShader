#include "SharedMaterialProperties.h"

GLuint SharedMaterialProperties::ambientMatLocation; // Byte offset of the projection matrix

GLuint SharedMaterialProperties::diffuseMatLocation; // Byte offset of the viewing matrix

GLuint SharedMaterialProperties::specularMatLocation; // Byte offset of the modeling matrix

GLuint SharedMaterialProperties::emmissiveMatLocation; // Byte offset of the modeling matrix to transform normal vectors

GLuint SharedMaterialProperties::specularExpLocation;  // Byte offset of the eye position

GLuint SharedMaterialProperties::textureModeLocation;  // Byte offset of the eye position

GLint SharedMaterialProperties::gSamplerLocation = 0;

GLuint SharedMaterialProperties::materialBlockBuffer; // Identifier for the buffer. There would only be one buffer to feed several uniform blocks.
GLint SharedMaterialProperties::materialBlockSize = 0; // Size in bytes of both the buffer and the uniform blocks in all the the shaders. 
GLuint SharedMaterialProperties::materialBlockIndex; // Identifier for the uniform block.

bool SharedMaterialProperties::blockSizeAndOffetsSet = false; // Indicates whether or not the buffer for the blocks has been setup

const string SharedMaterialProperties::materialBlockName = "MaterialBlock";

bool checkLocFound(const GLchar* locationName, GLuint indice)
{
	if (indice == GL_INVALID_INDEX) {
		cout << locationName << " not found in shader." << endl;

		return false;
	}
	else {

		//cout << locationName << " index is " <<  indice << endl; 
		return true;
	}

} // end checkLocationFound


void SharedMaterialProperties::setUniformBlockForShader(GLuint shaderProgram)
{
	// Determine the size of the block and set the binding point for the block(s)
	determineBlockSizeSetBindingPoint(shaderProgram);

	// Has the buffer been created and have the byte offset been found?
	if (blockSizeAndOffetsSet == false) {

		// Set up the buffers and bind to binding points
		allocateBuffers(shaderProgram);

		// Find the byte offsets of the uniform block variables
		findOffsets(shaderProgram);
	}

	checkOpenGLErrors("setUniformBlockForShader");

} // end setUniformBlockForShader


void SharedMaterialProperties::allocateBuffers(GLuint shaderProgram)
{
	if (materialBlockSize > 0) {

		glGenBuffers(1, &materialBlockBuffer);

		glBindBuffer(GL_UNIFORM_BUFFER, materialBlockBuffer);

		glBufferData(GL_UNIFORM_BUFFER, materialBlockSize, NULL, GL_DYNAMIC_DRAW);

		// Assign the buffer to a binding point 
		glBindBufferBase(GL_UNIFORM_BUFFER, materialBlockBindingPoint, materialBlockBuffer);

		blockSizeAndOffetsSet = true;
	}

	checkOpenGLErrors("findOffets");

} // end allocateBuffers


void SharedMaterialProperties::findOffsets(GLuint shaderProgram)
{
	const int numberOfNames = 6;

	GLuint uniformIndices[numberOfNames] = { 0 };
	GLint uniformOffsets[numberOfNames] = { 0 };

	const GLchar * charStringNames[] = { "object.ambientMat", "object.diffuseMat", "object.specularMat", 
										 "object.emmissiveMat", "object.specularExp", "object.textureMode" };

	glGetUniformIndices(shaderProgram, numberOfNames, (const GLchar **)charStringNames, uniformIndices);

	if (uniformIndices[0] != GL_INVALID_INDEX && uniformIndices[1] != GL_INVALID_INDEX) {
		for (int i = 0; i < numberOfNames; i++) {

			checkLocFound(charStringNames[i], uniformIndices[i]);
		}

		//Get the offsets of the uniforms
		glGetActiveUniformsiv(shaderProgram, numberOfNames, uniformIndices, GL_UNIFORM_OFFSET, uniformOffsets);

		for (int i = 0; i < numberOfNames; i++) {

			cout << '\t' << charStringNames[i] << " offset is " << uniformOffsets[i] << endl;
		}

		// Save locations
		ambientMatLocation = uniformOffsets[0];
		diffuseMatLocation = uniformOffsets[1];
		specularMatLocation = uniformOffsets[2];
		emmissiveMatLocation = uniformOffsets[3];
		specularExpLocation = uniformOffsets[4];
		textureModeLocation = uniformOffsets[5];
	}
	else {

		cout << " Did not find names in " << materialBlockName.c_str() << endl;
	}

	checkOpenGLErrors("findOffets");

} // findOffsets


void SharedMaterialProperties::determineBlockSizeSetBindingPoint(GLuint shaderProgram)
{
	materialBlockIndex = glGetUniformBlockIndex(shaderProgram, materialBlockName.c_str());

	cout << materialBlockName.c_str() << " index is " << materialBlockIndex << endl;

	if (checkLocFound(materialBlockName.c_str(), materialBlockIndex)) {

		// Determine the size in bytes
		glGetActiveUniformBlockiv(shaderProgram, materialBlockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &materialBlockSize);
		cout << materialBlockName.c_str() << " size is " << materialBlockSize << endl;

		// Assign the block to a binding point
		glUniformBlockBinding(shaderProgram, materialBlockIndex, materialBlockBindingPoint);
	}

	checkOpenGLErrors("findBlockSizeSetBindingPoint");

} // end determineBlockSizeSetBindingPoint


void SharedMaterialProperties::setShaderMaterialProperties(GLuint shaderProgram, Material material)
{
	// Bind the buffer. 
	glBindBuffer(GL_UNIFORM_BUFFER, materialBlockBuffer);

	glBufferSubData(GL_UNIFORM_BUFFER, ambientMatLocation, sizeof(glm::vec4), glm::value_ptr(material.ambientMat));
	glBufferSubData(GL_UNIFORM_BUFFER, diffuseMatLocation, sizeof(glm::vec4), glm::value_ptr(material.diffuseMat));
	glBufferSubData(GL_UNIFORM_BUFFER, specularMatLocation, sizeof(glm::vec4), glm::value_ptr(material.specularMat));
	glBufferSubData(GL_UNIFORM_BUFFER, emmissiveMatLocation, sizeof(glm::vec4), glm::value_ptr(material.emissiveMat));
	glBufferSubData(GL_UNIFORM_BUFFER, specularExpLocation, sizeof(float), &material.specularExpMat);
	

	GLint diffuse_sampler_loc = glGetUniformLocation(shaderProgram, "diffuse_sampler");
	// Set it to 0
	glUniform1i(diffuse_sampler_loc, 0);
	// Select texture unit 0
	glActiveTexture(GL_TEXTURE0);
	// Bind a texture to it
	
	if (material.textureMode != NO_TEXTURE && material.textureObject > 0) {

		int t = (int)material.textureMode;
		glBufferSubData(GL_UNIFORM_BUFFER, textureModeLocation, sizeof(int), &t);

		// Bind a texture 
		glBindTexture(GL_TEXTURE_2D, material.textureObject);
	}
	else {

		int t = 0;
		glBufferSubData(GL_UNIFORM_BUFFER, textureModeLocation, sizeof(int), &t);
		glBindTexture(GL_TEXTURE_2D, 0);
	}


	glBindBuffer(GL_UNIFORM_BUFFER, 0);

} // end setShaderMaterialProperties


GLint TextureFromFile(const string textureFileName)
{
	checkOpenGLErrors("TextureFromFile0");

	FREE_IMAGE_FORMAT formato = FreeImage_GetFileType(textureFileName.c_str(), 0);

	FIBITMAP* imagen = FreeImage_Load(formato, textureFileName.c_str());

	FIBITMAP* temp = imagen;
	imagen = FreeImage_ConvertTo32Bits(imagen);
	FreeImage_Unload(temp);

	int width = FreeImage_GetWidth(imagen);
	int height = FreeImage_GetHeight(imagen);

	if (imagen == NULL || width == 0 || height == 0) {
		std::cerr << "ERROR: Unable to load " << textureFileName << "!" << endl;
		return -1;
	}

	GLubyte* textura = new GLubyte[4 * width*height];
	char* pixeles = (char*)FreeImage_GetBits(imagen);

	//FreeImage loads in BGR format, so swap some bytes
	for (int j = 0; j<width*height; j++){
		textura[j * 4 + 0] = pixeles[j * 4 + 2];
		textura[j * 4 + 1] = pixeles[j * 4 + 1];
		textura[j * 4 + 2] = pixeles[j * 4 + 0];
		textura[j * 4 + 3] = pixeles[j * 4 + 3];
	}

	GLuint textureID;
	glGenTextures(1, &textureID);

	glBindTexture(GL_TEXTURE_2D, textureID);

	checkOpenGLErrors("TextureFromFile1");

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)textura);

	cout << "Loaded: " << textureFileName.c_str() << " texture. width " << width << " height " << height << endl;

	checkOpenGLErrors("TextureFromFile2");

	glGenerateMipmap(GL_TEXTURE_2D);

	// Parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);

	checkOpenGLErrors("TextureFromFile3");

	FreeImage_Unload(imagen);

	return textureID;

} // end TextureFromFile
