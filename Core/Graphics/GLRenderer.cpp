﻿#include ".//GLRenderer.h"


RGP_CORE::GLShaderProgramsManager::GLShaderProgramsManager(): m_LoadedProgramsList(NULL) , m_NumPrograms(0), m_Size(0),
															m_boundProgramID(0)
{};
RGP_CORE::GLShaderProgramsManager::~GLShaderProgramsManager()
{
	this->Destroy();
};
void RGP_CORE::GLShaderProgramsManager::Destroy()
{
	if (m_LoadedProgramsList) {
		for (_u32b i = 0; i < m_NumPrograms; ++i) {
			if (m_LoadedProgramsList[i].VertexShaderFileName)
				free(m_LoadedProgramsList[i].VertexShaderFileName);
			if (m_LoadedProgramsList[i].GeometryShaderFileName)
				free(m_LoadedProgramsList[i].GeometryShaderFileName);
			if (m_LoadedProgramsList[i].FragmentShaderFileName)
				free(m_LoadedProgramsList[i].FragmentShaderFileName);
			if (m_LoadedProgramsList[i].OneForAllFileName)
				free(m_LoadedProgramsList[i].OneForAllFileName);
			if(m_LoadedProgramsList[i].GLProgramID)
				glDeleteProgram(m_LoadedProgramsList[i].GLProgramID);
		}
		free(m_LoadedProgramsList);
		m_LoadedProgramsList = NULL;
	}
	m_NumPrograms = 0;
	m_Size = 0;
};
_u32b RGP_CORE::GLShaderProgramsManager::CreateProgramFromOneFile(const _s8b* Filename)
{
	return 0;
};
_u32b RGP_CORE::GLShaderProgramsManager::CreateProgramFromFiles(const _s8b* VertexFile,const  _s8b* FrgamentFile)
{
	return CreateProgramFromFiles(VertexFile, NULL, FrgamentFile);
};
_u32b RGP_CORE::GLShaderProgramsManager::CreateProgramFromFiles(const _s8b* VertexFile,const  _s8b* GeometryFile,const  _s8b* FragmentFile)
{
	if (VertexFile==NULL)
		return 0;
	_u32b ID=0;
	if (ID = doExiste(VertexFile, GeometryFile, FragmentFile))
		return ID;
	if (m_Size == m_NumPrograms) {
		ShaderProgram* tmp = (ShaderProgram*)malloc((m_Size + 1) * sizeof(ShaderProgram));
		if (!tmp)
			return 0;
		for (_u32b i = 0; i < m_NumPrograms; ++i) 
			tmp[i] = m_LoadedProgramsList[i];
		free(m_LoadedProgramsList);
		m_LoadedProgramsList = tmp;
		++m_Size;
	}
	m_LoadedProgramsList[m_NumPrograms].OneForAllFileName = NULL;
	m_LoadedProgramsList[m_NumPrograms].VertexShaderFileName = NULL;
	m_LoadedProgramsList[m_NumPrograms].GeometryShaderFileName = NULL;
	m_LoadedProgramsList[m_NumPrograms].FragmentShaderFileName = NULL;
	m_LoadedProgramsList[m_NumPrograms].GLProgramID = 0;
	++m_NumPrograms;
	m_LoadedProgramsList[m_NumPrograms-1].VertexShaderFileName = (_s8b*)malloc((strlen(VertexFile) + 1) * sizeof(_s8b));
	if (!m_LoadedProgramsList[m_NumPrograms - 1].VertexShaderFileName) {
		return 0;
	}
	else {
		strcpy(m_LoadedProgramsList[m_NumPrograms - 1].VertexShaderFileName, VertexFile);
	}
	if (GeometryFile) {
		m_LoadedProgramsList[m_NumPrograms - 1].GeometryShaderFileName = (_s8b*)malloc((strlen(GeometryFile) + 1) * sizeof(_s8b));
		if (!m_LoadedProgramsList[m_NumPrograms - 1].GeometryShaderFileName) {
			this->DeleteProgram(m_NumPrograms - 1);
			return 0;
		}
		else {
			strcpy(m_LoadedProgramsList[m_NumPrograms - 1].GeometryShaderFileName, GeometryFile);
		}
	}
	if (FragmentFile) {
		m_LoadedProgramsList[m_NumPrograms - 1].FragmentShaderFileName = (_s8b*)malloc((strlen(FragmentFile) + 1) * sizeof(_s8b));
		if (!m_LoadedProgramsList[m_NumPrograms - 1].FragmentShaderFileName) {
			this->DeleteProgram(m_NumPrograms);
			return 0;
		}
		else {
			strcpy(m_LoadedProgramsList[m_NumPrograms - 1].FragmentShaderFileName, FragmentFile);
		}
	}
	m_LoadedProgramsList[m_NumPrograms-1].GLProgramID = CreateGLProgram(VertexFile, GeometryFile, FragmentFile);
	if (!m_LoadedProgramsList[m_NumPrograms - 1].GLProgramID) {
		this->DeleteProgram(m_NumPrograms);
		return 0;
	}
	return m_NumPrograms;
};
void  RGP_CORE::GLShaderProgramsManager::BindProgram(_u32b ID)
{
	if (ID != m_boundProgramID) {
		if (ID == 0) {
			glUseProgram(0);
			m_boundProgramID = 0;
			return;
		}
		if (ID <= m_NumPrograms) {
			glUseProgram(m_LoadedProgramsList[ID - 1].GLProgramID);
			m_boundProgramID = ID;
		}
	}
};
void  RGP_CORE::GLShaderProgramsManager::DeleteProgram(_u32b ID)
{
	if (ID > 0 && ID <= m_NumPrograms) {
		
		if (m_LoadedProgramsList[ID - 1].FragmentShaderFileName) {
			free(m_LoadedProgramsList[ID - 1].FragmentShaderFileName);
			m_LoadedProgramsList[ID - 1].FragmentShaderFileName = NULL;
		}
		if (m_LoadedProgramsList[ID - 1].GeometryShaderFileName) {
			free(m_LoadedProgramsList[ID - 1].GeometryShaderFileName);
			m_LoadedProgramsList[ID - 1].GeometryShaderFileName = NULL;
		}
		if (m_LoadedProgramsList[ID - 1].VertexShaderFileName) {
			free(m_LoadedProgramsList[ID - 1].VertexShaderFileName);
			m_LoadedProgramsList[ID - 1].VertexShaderFileName = NULL;
		}
		if (m_LoadedProgramsList[ID - 1].OneForAllFileName) {
			free(m_LoadedProgramsList[ID - 1].OneForAllFileName);
			m_LoadedProgramsList[ID - 1].OneForAllFileName = NULL;
		}
		if (m_LoadedProgramsList[ID - 1].GLProgramID) {
			glDeleteProgram(m_LoadedProgramsList[ID - 1].GLProgramID);
			m_LoadedProgramsList[ID - 1].GLProgramID = 0;
		}
		for (_u32b i = ID - 1; i < m_NumPrograms-1; ++i) {
			m_LoadedProgramsList[i] = m_LoadedProgramsList[i + 1];
		}
		if (m_boundProgramID == ID) {
			m_boundProgramID = 0;
			glUseProgram(0);
		}
		else if (m_boundProgramID > ID)
			--m_boundProgramID; 
		--m_NumPrograms;
		m_LoadedProgramsList[m_NumPrograms].FragmentShaderFileName = NULL;
		m_LoadedProgramsList[m_NumPrograms].GeometryShaderFileName = NULL;
		m_LoadedProgramsList[m_NumPrograms].VertexShaderFileName = NULL;
		m_LoadedProgramsList[m_NumPrograms].OneForAllFileName = NULL;
		m_LoadedProgramsList[m_NumPrograms].GLProgramID = 0;
	}
};
_u32b RGP_CORE::GLShaderProgramsManager::LoadShaderBuffer(GLenum type, const _s8b* Buffer, int buffersize) {
	GLuint Shader = 0;
	if (!Buffer || !buffersize) {
		printf("error no shader buffer ");
		return Shader;
	}

	Shader = glCreateShader(type);
	if (Shader) {
		glShaderSource(Shader, 1, (const _s8b**)(&Buffer), &buffersize);
		glCompileShader(Shader);
		GLint compiled;
		glGetShaderiv(Shader, GL_COMPILE_STATUS, &compiled);
		if (!compiled) {
			printf("error compiling shader\n");
			printf("Source: %s \n", Buffer);
			GLint blen = 0;
			GLsizei slen = 0;

			glGetShaderiv(Shader, GL_INFO_LOG_LENGTH, &blen);
			if (blen > 1)
			{
				GLchar* compiler_log = (GLchar*)malloc(blen);
				glGetShaderInfoLog(Shader, blen, &slen, compiler_log);
				printf("compiler_log: %s\n", compiler_log);
				free(compiler_log);
			}
			glDeleteShader(Shader);
		}
	}
	else {
		printf("Cannot create shader \n");
	}
	return Shader;
};
_u32b RGP_CORE::GLShaderProgramsManager::LoadShaderFile(GLenum Type,const  _s8b* filename)
{
	GLuint Shader = 0;
	_u32b buffersize = 0;
	_s8b* buffer = NULL;
	FILE* Shaderfile = NULL;
	if (!filename)
		return 0;
	fflush(stdin);
	if (!(Shaderfile = fopen(filename, "rb"))) {
		printf("shader file %s not found \n", filename);
		return Shader;
	}
	
	fseek(Shaderfile, 0, SEEK_END);
	buffersize = ftell(Shaderfile);
	if (buffersize == 0) {
		printf("empty file\n");
		fclose(Shaderfile);
		return 0;
	}
	fseek(Shaderfile, 0, SEEK_SET);
	buffer = (_s8b*)malloc((buffersize + 1) * sizeof(_s8b));
	if (!buffer) {
		fclose(Shaderfile);
		return Shader;
	}
	fflush(stdin);
	fread(buffer, buffersize, 1, Shaderfile);
	buffer[buffersize] = '\0';
	fclose(Shaderfile);
	Shader = LoadShaderBuffer(Type, buffer, buffersize);
	free(buffer);
	return Shader;
};

_u32b RGP_CORE::GLShaderProgramsManager::CreateGLProgram(const _s8b* VertexFile,const  _s8b* GeometryFile,const  _s8b* FragmentFile)
{
	GLuint program = 0, vs = 0, gs = 0, fs = 0;
	GLuint error = 0;
	vs = LoadShaderFile(GL_VERTEX_SHADER, VertexFile);
	if (!vs) {
		printf("error loading vertex shader\n");
		return program;
	}
	if (GeometryFile) {
		if (strlen(GeometryFile) != 0) {
			gs = LoadShaderFile(GL_GEOMETRY_SHADER, GeometryFile);
			if (!gs) {
				glDeleteShader(vs);
				return program;
			}
		}
	}
	if (FragmentFile)
		if (strlen(FragmentFile) != 0) {
			fs = LoadShaderFile(GL_FRAGMENT_SHADER, FragmentFile);
			if (!fs) {
				glDeleteShader(vs);
				if (gs) {
					glDeleteShader(gs);
				}
				return program;
			}
		}
	glGetError();
	if (program = glCreateProgram()) {
		error = glGetError();
		glAttachShader(program, vs);
		if (error = glGetError()) {
			printf("error attaching shader \n");
		}
		if (gs) {
			glAttachShader(program, gs);
			if (error = glGetError()) {
				printf("error attaching shader \n");
			}
		}
		if (fs) {
			glAttachShader(program, fs);
			if (error = glGetError()) {
				printf("error attaching shader \n");
			}
		}
		glLinkProgram(program);
		error = glGetError();
		GLint linked;
		glGetProgramiv(program, GL_LINK_STATUS, &linked);
		if (linked == GL_FALSE) {
			printf("error linking shader program %d  %d \n", linked, error);
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
			GLchar log[1024];
			glGetProgramInfoLog(program, maxLength, &maxLength, log);
			log[maxLength] = '\0';
			printf("\n%s\n", log);
			glDeleteProgram(program);
			program = 0;
		}
	}
	else {
		printf("cannot create shader program\n");
	}
	glDeleteShader(vs);
	glDeleteShader(fs);
	glDeleteShader(gs);
	return program;
};

_u32b RGP_CORE::GLShaderProgramsManager::doExiste(const _s8b* VertexFile,const  _s8b* GeometryFile,const  _s8b* FragmentFile)
{
	//IF THE SHADER PROGRM IS LOADED
	for (_u32b i = 0; i < m_NumPrograms; i++)
		if (strcmp(VertexFile, m_LoadedProgramsList[i].VertexShaderFileName) == 0)
			return i + 1;
	return 0;
};
RGP_CORE::GLShaderProgramsManager::ShaderProgram* 
				RGP_CORE::GLShaderProgramsManager::getShaderProgram(_u32b programID)
{
	if (programID > 0 && programID <= m_NumPrograms)
		return &m_LoadedProgramsList[programID-1];
	return 0;
};




RGP_CORE::GLRenderer::GLRenderer():m_Target(NULL),
											m_isInitialized(false) ,
											m_SelectedScene(NULL),m_ShaderManager(NULL),

											m_FBO(0),
                                            m_AttachmentTextures(NULL),

                                            m_FinalRenderSurface(NULL),
                                            m_FinalRenderProgram(0),										
											
											m_LightAccumProgram(0),
											m_LightAccumBuffer(0),
											m_LightAccumDiffuseTexture(0),
											m_LightAccumSpecularTexture(0),

											m_ShadowVectorSize(0),m_NumShadowFBOs(0),
											m_ShadowRenderingProgram(0),
											m_ShadowFBOs(NULL),
											m_ShadowAttachmentTexture(NULL),
											m_ShadowAccumBuffer(0),
											m_ShadowAccumTexture(0),
											m_ShadowAccumProgram(0),
											m_noLightMode(false),

											
							
											m_Materials(NULL),
											m_GPUMaterials(NULL),
											m_NumMaterials(0),
											m_SizeofMaterialVector(0),
											m_CurrentShaderProgram(0),
											m_AllMaterialUBO(0),
											m_CameraMtxUBO(0),
											m_LightDataUBO(0),
											m_NumRegisteredLights(0),
				
											m_Emissionintensity(1.0f),
			
											m_Timer(),
											m_Time(0.0),
											num_exts_i(0),
											exts_i(NULL)
											
{
   

	DrawBuff[0] = GL_COLOR_ATTACHMENT0;//Diffuse
	DrawBuff[1] = GL_COLOR_ATTACHMENT1;//specular
	DrawBuff[2] = GL_COLOR_ATTACHMENT2;// normal
	DrawBuff[3] = GL_COLOR_ATTACHMENT3;//Material ID
	DrawBuff[4] = GL_COLOR_ATTACHMENT4;//World Postion
	DrawBuff[5] = GL_COLOR_ATTACHMENT5; // transParency
	DrawBuff[6] = GL_COLOR_ATTACHMENT6; //Emission

};
RGP_CORE::GLRenderer::~GLRenderer(){
    this->Destroy();
};
void RGP_CORE::GLRenderer::Destroy(){
	
	ClearMaterials();
	if (exts_i) {
		for (_u32b index = 0; index < num_exts_i; ++index) {
			free( exts_i[index]);
		}
		free(exts_i);
		exts_i = NULL;
		num_exts_i = 0;
	}
	if (m_CameraMtxUBO) {
		this->DeleteBuffers(1, &m_CameraMtxUBO);
		m_CameraMtxUBO = 0;
	}
	if (m_LightDataUBO) {
		this->DeleteBuffers(1, &m_LightDataUBO);
		m_LightDataUBO = 0;
	}
	if (m_GPUMaterials) {
		this->DeleteBuffers(1, &m_AllMaterialUBO);
		m_GPUMaterials = 0;
	}

	

	if(m_FBO){
        glDeleteFramebuffers(1,(GLuint*)&m_FBO);
        m_FBO=0 ;
    }
    if(m_AttachmentTextures){
         glDeleteTextures(NUMTEXTUREGBUFFER, (GLuint*)m_AttachmentTextures);
        free(m_AttachmentTextures);
        m_AttachmentTextures=NULL ;
    }
	if (m_LightAccumProgram) {
		glDeleteProgram(m_LightAccumProgram);
		m_LightAccumProgram = 0;
	}
	if (m_LightAccumBuffer) {
		glDeleteFramebuffers(1, (GLuint*)&m_LightAccumBuffer);
		m_LightAccumBuffer = 0;
	}
	if (m_LightAccumDiffuseTexture) {
		glDeleteTextures(1, (GLuint*)&m_LightAccumDiffuseTexture);
		m_LightAccumDiffuseTexture = 0;
	}
	if (m_LightAccumSpecularTexture) {
		glDeleteTextures(1, (GLuint*)&m_LightAccumSpecularTexture);
		m_LightAccumSpecularTexture = 0;
	}
	if (m_ShadowFBOs){
		if (m_ShadowAttachmentTexture){
			glDeleteTextures(m_NumShadowFBOs, (GLuint*)m_ShadowAttachmentTexture);
			free(m_ShadowAttachmentTexture);
			m_ShadowAttachmentTexture = NULL;
		}
		glDeleteFramebuffers(m_ShadowVectorSize, (GLuint*)m_ShadowFBOs);
		free(m_ShadowFBOs);
		m_ShadowFBOs = NULL;
		m_NumShadowFBOs = 0;
		m_ShadowVectorSize = 0;
	}
	if (m_ShadowAccumBuffer){
		if (m_ShadowAccumTexture){
			glDeleteTextures(1, (GLuint*)&m_ShadowAccumTexture);
			m_ShadowAccumTexture = 0;
		}
		glDeleteFramebuffers(1, (GLuint*)&m_ShadowAccumBuffer);
		m_ShadowAccumBuffer = 0;
	}
	

    if(m_FinalRenderSurface){
		
        glDeleteBuffers(3,(GLuint*)m_FinalRenderSurface->Wrappers);
        if(m_FinalRenderSurface->VertexArrayObject){
            glDeleteVertexArrays(1,(GLuint*)&(m_FinalRenderSurface->VertexArrayObject));
        }
        free(m_FinalRenderSurface);
        m_FinalRenderSurface=NULL ;
    }
	if (m_ShaderManager) {
		delete m_ShaderManager;
		m_ShaderManager = NULL;
	}
    if(m_Target){
        m_Target->Destroy();
        delete m_Target;
        m_Target=NULL ;
    }
	
    m_SelectedScene=NULL ;
    m_isInitialized =false;
	FreeImage_DeInitialise();
	glfwTerminate();


};
_bool RGP_CORE::GLRenderer::InitRenderer(gfxConfig Config){

    ///initializinf GLFW and creating window ;
    if(!glfwInit()){
        printf("errro initializing GLFW\n");
        return false ;
    }
	glfwWindowHint(GLFW_STEREO, 0);
	glfwWindowHint(GLFW_DOUBLEBUFFER, 1);
	glfwWindowHint(GLFW_RESIZABLE, 0);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    m_Target=RGP_CORE::MyWindow::Create(Config.Title,Config.Witdh,Config.Height);
    if(!m_Target){
        printf("error creating Window\n");
        return false ;
    }
	
	this->MakeContext();

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glfwSwapInterval(0);
	FreeImage_Initialise();
	m_ShaderManager = new GLShaderProgramsManager();
	if (!m_ShaderManager) {
		printf("Fatal error\n");
		return false;
	}
	m_Config = Config;
     if(!CreateNeededObjects()){
        printf("error Creating FBOs and Render Textures\n");
        return false ;
     }

	 this->CreateDefaultMaterial();

   //final step to initialize the renderer
    if(!InitFinalPhase()){
        printf("error initializing final phase\n");
        return false ;
    }
    //init rendering thread 
	
    m_isInitialized=true ;
	m_Timer.Reset();
    return true ;
};
_bool RGP_CORE::GLRenderer::CreateNeededObjects(){
	if (!CreateColorsObjects())
		return false;
	if (!CreateLightObjects())
		return false;
	if (!CreateShadowsObjects())
		return false;
	
	this->GenBuffers(1, &m_CameraMtxUBO);
	this->BindBuffer(GL_UNIFORM_BUFFER, m_CameraMtxUBO);
	this->setBufferData(GL_UNIFORM_BUFFER, 32 * sizeof(_float), NULL, GL_DYNAMIC_DRAW);
	this->GenBuffers(1, &m_AllMaterialUBO);
	this->BindBuffer(GL_UNIFORM_BUFFER, m_AllMaterialUBO);
	this->setBufferData(GL_UNIFORM_BUFFER, MAXNUMAMTERIALS * sizeof(GPUMaterial), NULL, GL_DYNAMIC_DRAW);
	this->GenBuffers(1, &m_LightDataUBO);
	this->BindBuffer(GL_UNIFORM_BUFFER, m_LightDataUBO);
	this->setBufferData(GL_UNIFORM_BUFFER, MAXNUMLIGHTSOURCES*25 * sizeof(_float), NULL, GL_DYNAMIC_DRAW);
	this->BindBuffer(GL_UNIFORM_BUFFER, 0);



	return true;
};

_bool	RGP_CORE::GLRenderer::CreateColorsObjects()
{
	int error;
	//generating textures(Diffuse1,Diffuse2,Specular+Roughness(RGBA),Normal,Depth+Stencil,Material ID,position,trancprency)==>8maps;
	m_AttachmentTextures = (_u32b*)malloc(NUMTEXTUREGBUFFER * sizeof(_u32b));
	if (!m_AttachmentTextures) {
			printf("error allocation \n");
			return false;
	}
	for (_s16b k = 0; k<NUMTEXTUREGBUFFER; ++k) {
		m_AttachmentTextures[k] = 0;
	}
	glGetError();// emptying the error log
	glGenTextures(NUMTEXTUREGBUFFER, (GLuint*)m_AttachmentTextures);
	if (error = glGetError())
		printf("error generating textures for GBbuffer\n");
	//Depth texture+stencil
	glBindTexture(GL_TEXTURE_2D, m_AttachmentTextures[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glGetError();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24,
			m_Target->getWidth(),
			m_Target->getHeight(),
			0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

		///THE OTHER TEXTURES
	for (_s16b k = 1; k<NUMTEXTUREGBUFFER; ++k) {
			glBindTexture(GL_TEXTURE_2D, m_AttachmentTextures[k]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F,
				m_Target->getWidth(),
				m_Target->getHeight(),
				0, GL_RGBA, GL_FLOAT, 0);
	}
		glBindTexture(GL_TEXTURE_2D, 0);

	//generating Colors FBO
	glGetError();
	glGenFramebuffers(1, (GLuint*)&m_FBO);
	if (error = glGetError()) {
		printf("error generating Framebuffers %d\n", error);
		return false;
	}
	if (!AttachColorsTextures())
		return false;
	return true;
};

_bool RGP_CORE::GLRenderer::AttachColorsTextures() {
	int error;
	///attaching texture to its FBO
	glGetError();
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	if (error = glGetError()) {
		printf("error binding framebuffer = %u error %d\n", m_FBO, error);
	}
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_AttachmentTextures[DIFFUSE_TEXTURE], 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_AttachmentTextures[SPECULAR_TEXTURE], 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_AttachmentTextures[NORMAL_TEXTURE], 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, m_AttachmentTextures[MATERIAL_TEXTURE], 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, m_AttachmentTextures[POSITION_TEXTURE], 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT5, GL_TEXTURE_2D, m_AttachmentTextures[TRANSPARENCY_TEXTURE], 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT6, GL_TEXTURE_2D, m_AttachmentTextures[EMISSION_TEXTURE], 0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_AttachmentTextures[DEPTH_TEXTURE], 0);
	error = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (GL_FRAMEBUFFER_COMPLETE != error) {
		printf("frameBuffer attacehment error code %x\n", error);
	}
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return true;
};

_bool	RGP_CORE::GLRenderer::CreateShadowsObjects()
{

	m_ShadowRenderingProgram = CreateGLProgramFromFile("..//Shaders//ShadowMapProgram.vs", NULL);
	if (!m_ShadowRenderingProgram) {
		printf("error loading Shadow Mapping program");
		return false;
	}

	m_ShadowAccumProgram = CreateGLProgramFromFile("..//Shaders//ShadowAccumProgram.vs", "..//Shaders//ShadowAccumProgram.fs");
	if (!m_ShadowAccumProgram) {
		printf("error loading Shadow Maps Combiner program");
		return false;
	}

	glGenFramebuffers(1, (GLuint*)&m_ShadowAccumBuffer);
	if (m_ShadowAccumBuffer == 0) {
		printf("error generating FBO for Shadows \n");
		return false;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, m_ShadowAccumBuffer);
	glGenTextures(1, (GLuint*)&m_ShadowAccumTexture);
	glBindTexture(GL_TEXTURE_2D, m_ShadowAccumTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F,
		m_Target->getWidth(),
		m_Target->getHeight(),
		0, GL_RGBA, GL_FLOAT, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ShadowAccumTexture, 0);
	glViewport(0, 0, m_Target->getWidth(), m_Target->getHeight());
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	return true;
};
_bool	RGP_CORE::GLRenderer::CreateLightObjects()
{
	int error;
	m_LightAccumProgram = CreateGLProgramFromFile("..//Shaders//LightAccumProgram.vs", "..//Shaders//LightAccumProgram.fs");
	if (!m_LightAccumProgram) {
		printf("error loading Light program");
		return false;
	}
	glGenFramebuffers(1, (GLuint*)&m_LightAccumBuffer);
	if (m_LightAccumBuffer == 0) {
		printf("error generating FBO for light accumulation \n");
		return false;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, m_LightAccumBuffer);
	glGenTextures(1, (GLuint*)&m_LightAccumDiffuseTexture);
	glBindTexture(GL_TEXTURE_2D, m_LightAccumDiffuseTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F,
		m_Target->getWidth(),
		m_Target->getHeight(),
		0, GL_RGBA, GL_FLOAT, 0);
	glGenTextures(1, (GLuint*)&m_LightAccumSpecularTexture);
	glBindTexture(GL_TEXTURE_2D, m_LightAccumSpecularTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F,
		m_Target->getWidth(),
		m_Target->getHeight(),
		0, GL_RGBA, GL_FLOAT, 0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_LightAccumDiffuseTexture, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_LightAccumSpecularTexture, 0);
	error = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (GL_FRAMEBUFFER_COMPLETE != error) {
		printf("frameBuffer attacehment error code %x\n", error);
	}
	glViewport(0, 0, m_Target->getWidth(), m_Target->getHeight());
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(1.0, 1.0, 1.0,0.0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	return true;
}
_bool RGP_CORE::GLRenderer::CreateDefaultMaterial()
{
	defaultMAaterial.Name = CreateStringCopy("DefaultMaterial");
	defaultMAaterial.IOR = 1.0f;
	defaultMAaterial.Opacity = 1.0f;
	defaultMAaterial.DiffuseMap = (Image*)malloc(sizeof(Image));
	defaultMAaterial.DiffuseMap->Height = 1;
	defaultMAaterial.DiffuseMap->Width = 1;
	defaultMAaterial.DiffuseMap->Pixels = (_u8b*)malloc(4 * sizeof(_u8b));
	defaultMAaterial.DiffuseMap->Pixels[0] = 128;
	defaultMAaterial.DiffuseMap->Pixels[1] = 128;
	defaultMAaterial.DiffuseMap->Pixels[2] = 128;
	defaultMAaterial.DiffuseMap->Pixels[3] = 0;

	defaultMAaterial.NormalsMap = (Image*)malloc(sizeof(Image));
	defaultMAaterial.NormalsMap->Height = 1;
	defaultMAaterial.NormalsMap->Width = 1;
	defaultMAaterial.NormalsMap->Pixels = (_u8b*)malloc(4 * sizeof(_u8b));
	defaultMAaterial.NormalsMap->Pixels[0] = 128;
	defaultMAaterial.NormalsMap->Pixels[1] = 127;
	defaultMAaterial.NormalsMap->Pixels[2] = 255;
	defaultMAaterial.NormalsMap->Pixels[3] = 0;

	defaultMAaterial.SpecularMap = (Image*)malloc(sizeof(Image));
	defaultMAaterial.SpecularMap->Height = 1;
	defaultMAaterial.SpecularMap->Width = 1;
	defaultMAaterial.SpecularMap->Pixels = (_u8b*)malloc(4 * sizeof(_u8b));
	defaultMAaterial.SpecularMap->Pixels[0] = 120;
	defaultMAaterial.SpecularMap->Pixels[1] = 120;
	defaultMAaterial.SpecularMap->Pixels[2] = 120;
	defaultMAaterial.SpecularMap->Pixels[3] = 0;

	defaultMAaterial.EmissiveMap = (Image*)malloc(sizeof(Image));
	defaultMAaterial.EmissiveMap->Height = 1;
	defaultMAaterial.EmissiveMap->Width = 1;
	defaultMAaterial.EmissiveMap->Pixels = (_u8b*)malloc(4 * sizeof(_u8b));
	defaultMAaterial.EmissiveMap->Pixels[0] = 255;
	defaultMAaterial.EmissiveMap->Pixels[1] = 255;
	defaultMAaterial.EmissiveMap->Pixels[2] = 255;
	defaultMAaterial.EmissiveMap->Pixels[3] = 0;

	this->CreateMaterial(&defaultMAaterial);

	return true;
};
_bool RGP_CORE::GLRenderer::InitFinalPhase(){
    _float vertexBuffer[8]={    -1.0f,1.0f,
                                1.0f,1.0f,
                                1.0f,-1.0f,
                                -1.0f,-1.0f,};
    _float TexCoordBuffer[8]={0.0f,1.0f,
                            1.0f,1.0f,
                            1.0f,0.0f,
                            0.0f,0.0f};
    _u32b IndexBuffer[6]={0,2,1,
                         0,3,2};
    m_FinalRenderSurface=(MeshBuffers*)malloc(sizeof(MeshBuffers));
    if(!m_FinalRenderSurface)
        return false ;
    m_FinalRenderSurface->VertexArrayObject=0;
	m_FinalRenderSurface->Wrappers[0]=0;
    m_FinalRenderSurface->Wrappers[1] =0;
    m_FinalRenderSurface->Wrappers[2] =0;
    glGetError();

    glGenBuffers(3, (GLuint*)(m_FinalRenderSurface-> Wrappers));
    glBindBuffer(GL_ARRAY_BUFFER, m_FinalRenderSurface-> Wrappers[0]);
    glBufferData(GL_ARRAY_BUFFER,8*sizeof(_float),vertexBuffer,GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, m_FinalRenderSurface-> Wrappers[1]);
    glBufferData(GL_ARRAY_BUFFER,8*sizeof(_float),TexCoordBuffer,GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_FinalRenderSurface->Wrappers[2]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6*sizeof(_u32b) , IndexBuffer, GL_STATIC_DRAW);


    glGenVertexArrays(1, (GLuint*)&(m_FinalRenderSurface->VertexArrayObject));
    glBindVertexArray(m_FinalRenderSurface->VertexArrayObject);
    glBindBuffer(GL_ARRAY_BUFFER,m_FinalRenderSurface->Wrappers[0]);
    glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,0,0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER,m_FinalRenderSurface->Wrappers[1]);
    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,0,0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_FinalRenderSurface->Wrappers[2]);


    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
	

	m_FinalRenderProgram = this->CreateGLProgramFromFile("..//Shaders//FinalDeferred.vs", "..//Shaders//FinalDeferred.fs");
	if (!m_FinalRenderProgram){
		printf("error loading render program \n");
		return false;
	}
	

    return true ;
}
_bool RGP_CORE::GLRenderer::UpdateCameraMtxUBO()
{
	if (!m_SelectedScene->getCamera())
		return false;
	this->setBufferSubData(GL_UNIFORM_BUFFER, 0, 16 * sizeof(_float), m_SelectedScene->getCamera()->getViewMtx(), m_CameraMtxUBO);
	this->setBufferSubData(GL_UNIFORM_BUFFER, 16 * sizeof(_float), 16 * sizeof(_float), m_SelectedScene->getCamera()->getProjectionMtx(), m_CameraMtxUBO);

	return true;
}
_bool RGP_CORE::GLRenderer::UpdateLightDataUBO()
{
	_u32b light = 0;
	_u32b lightdatasize = 28 * sizeof(_float);
	LightSource* source = NULL;
	_float data[3] = { 0.0f,0.0f,0.0f };
	this->BindBuffer(GL_UNIFORM_BUFFER,m_LightDataUBO);
	for (_u32b i = 0; i < m_SelectedScene->getNumActors(); ++i) {
		if (m_SelectedScene->getActor(i)->getID() & LIGHTSOURCE) {
			source = dynamic_cast<LightSource*>(m_SelectedScene->getActor(i));
			this->setBufferSubData(GL_UNIFORM_BUFFER, light*lightdatasize                       , 16 * sizeof(_float), source->getTransMtx());
			this->setBufferSubData(GL_UNIFORM_BUFFER, light*lightdatasize+ 16 * sizeof(_float)  , 3 * sizeof(_float),(void*) source->getLightDiffuseColor());
			this->setBufferSubData(GL_UNIFORM_BUFFER, light*lightdatasize+ 20 * sizeof(_float)  , 3 * sizeof(_float),(void*) source->getLightSpecularColor());
			data[0] = source->getLightDistance(); data[1] = source->getLightCutoffAngle(); data[2] = source->getLightShiness();
			this->setBufferSubData(GL_UNIFORM_BUFFER, light*lightdatasize + 24 * sizeof(_float) , 3 * sizeof(_float),data );
			++light;
		}
	}
	return true;
}
_bool RGP_CORE::GLRenderer::UpdateMaterialsUBO()
{
	this->setBufferSubData(GL_UNIFORM_BUFFER, 0, m_NumMaterials * sizeof(GPUMaterial), m_GPUMaterials, m_AllMaterialUBO);
	return true;
}
;

_bool RGP_CORE::GLRenderer::isInitialized(){ return m_isInitialized ;};

RGP_CORE::MyWindow*   RGP_CORE::GLRenderer::getTarget(){ return m_Target ;};


void		RGP_CORE::GLRenderer::MakeContext()
{
	glfwMakeContextCurrent(m_Target->getglfwWindow());
};


void  RGP_CORE::GLRenderer::setScene(GameScene*   Scene){ 
	m_SelectedScene= Scene ;
	reRegisterLightSources();
};

_bool RGP_CORE::GLRenderer::reRegisterLightSources()
{

	_u32b NumShadowmaps = 0;
	_u32b* tmp = NULL;
	LightSource* Source = NULL;
	int error;
	m_NumRegisteredLights = 0;
	if (m_SelectedScene) {
		//determine how many shadow map needed
		
		for (_u32b i = 0; i < m_SelectedScene->getNumActors(); ++i) {
			if (m_SelectedScene->getActor(i)->getID() & LIGHTSOURCE) {
				++NumShadowmaps;
				Source = dynamic_cast<LightSource*>(m_SelectedScene->getActor(i));
				if (Source->getLightCutoffAngle() < 0.0)
					NumShadowmaps += 5;
				++m_NumRegisteredLights;
			}
		}

		if (NumShadowmaps > m_ShadowVectorSize) {
			tmp = (_u32b*)malloc(NumShadowmaps * sizeof(_u32b));//first we need new FBos
			if (!tmp)
				return false;
			for (_u32b i = 0; i < m_NumShadowFBOs; ++i)
				tmp[i] = m_ShadowFBOs[i];
			free(m_ShadowFBOs);
			m_ShadowFBOs = tmp;

			tmp = (_u32b*)malloc(NumShadowmaps * sizeof(_u32b));//next Attachent texture for eac fbo
			if (!tmp)
				return false;
			for (_u32b i = 0; i < m_NumShadowFBOs; ++i)
				tmp[i] = m_ShadowAttachmentTexture[i];
			free(m_ShadowAttachmentTexture);
			m_ShadowAttachmentTexture = tmp;

			//generate and attach
			glGenFramebuffers(NumShadowmaps - m_NumShadowFBOs, (GLuint*)&m_ShadowFBOs[m_NumShadowFBOs]);
			glGenTextures(NumShadowmaps - m_NumShadowFBOs, (GLuint*)&m_ShadowAttachmentTexture[m_NumShadowFBOs]);

			for (_u32b i = m_NumShadowFBOs; i < NumShadowmaps; ++i) {
				glBindFramebuffer(GL_FRAMEBUFFER, m_ShadowFBOs[i]);
				glBindTexture(GL_TEXTURE_2D, m_ShadowAttachmentTexture[i]);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24,
					m_Config.ShadowResolution,
					m_Config.ShadowResolution,
					0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_ShadowAttachmentTexture[i], 0);
				glDrawBuffer(GL_NONE);
				glReadBuffer(GL_NONE);
				error = glCheckFramebufferStatus(GL_FRAMEBUFFER);
				if (GL_FRAMEBUFFER_COMPLETE != error) {
					printf("frameBuffer attacehment error code %x\n", error);
				}
				glClear(GL_DEPTH_BUFFER_BIT);
			}
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			m_ShadowVectorSize = NumShadowmaps;
		}
		m_NumShadowFBOs = NumShadowmaps;
	}
	this->UpdateLightDataUBO();
	return true;
}

void	RGP_CORE::GLRenderer::RenderScene(_u32b FBO_Target,Camera* camera)
{
		//RENDER SCENE Colors to the SelectedFBO
		this->RenderSceneColors(m_FBO, camera);
		if (!m_noLightMode) {
			////if should draw shadows
			if (m_Config.EnableShadows == true) {
				this->RenderSceneShadows(m_ShadowAccumBuffer, camera);
			}
			this->RenderSceneLightAccum();
		}
	///combine results and render to the screen ;
	this->RenderToTarget(FBO_Target);
};

void  RGP_CORE::GLRenderer::RenderCurrentScene()
{
	this->RenderScene(0);
};
void	RGP_CORE::GLRenderer::LoadShadowProgram()
{
	m_ShaderManager->BindProgram(m_ShadowRenderingProgram);
};
void	RGP_CORE::GLRenderer::UnloadShadowProgram()
{
	m_ShaderManager->BindProgram(0);
};
void RGP_CORE::GLRenderer::RenderSceneColors(_u32b FBO,Camera *camera)
{
	_u32b nbActors = 0;
	BaseActor*  actor = NULL;
	Camera* Eye=NULL;
	
	glGetError();
	if (m_SelectedScene){
		if (camera) {
			Eye = camera;
		}
		else {
			Eye = m_SelectedScene->getCamera();
		}
		if (Eye->hasChanged()) {
			Eye->ApplyChanges();
			this->UpdateCameraMtxUBO();
		}
		nbActors = m_SelectedScene->getNumActors();
		///first step render to current FBO
		glGetError();
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);

		glViewport(0, 0, m_Target->getWidth(), m_Target->getHeight());
		glDrawBuffers(NUMTEXTUREGBUFFER-1, DrawBuff);
		
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glEnable(GL_DEPTH_TEST);
		///glClear attachements
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		_float Ones[4] = { 1.0f,1.0f,1.0f,1.0f };
		_float Zeros[4] = { 0.0f,0.0f,0.0f,0.0f };
		glClearBufferfv(GL_COLOR, 0, Zeros);
		glClearBufferfv(GL_COLOR, 1, Zeros);
		glClearBufferfv(GL_COLOR, 3, Zeros);
		glClearBufferfv(GL_COLOR, 5, Ones);
		glClearBufferfv(GL_COLOR, 6, Zeros);
		///start searching for renderable objects and rendering
		for (_u32b i = 0; i < nbActors; ++i){
			actor = m_SelectedScene->getActor(i);
			if (actor->getID() & RENDERABLE) {
				dynamic_cast<Renderable*>(actor)->Render(Eye);
			}
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);	
	}
};
void RGP_CORE::GLRenderer::RenderSceneShadows(_u32b FBO, Camera* camera)
{
	Camera* Eye = NULL ;
	
	if (m_SelectedScene && m_Config.EnableShadows){
		_u32b NumActors = m_SelectedScene->getNumActors();
		_u32b ShadowIndex = 0;
		_s32b Location1 = -1, Location2 = -1, Location3 = -1;
		LightSource*	Source = NULL;
		Renderable*		actor = NULL;
		

		if (camera) {
			Eye = camera;
		}
		else {
			Eye = m_SelectedScene->getCamera();
		}
		
		m_ShaderManager->BindProgram(m_ShadowRenderingProgram);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		Location1 = this->GetUniformLocation(m_ShadowRenderingProgram, "World");
		Location2 = this->GetUniformLocation(m_ShadowRenderingProgram, "View");
		Location3 = this->GetUniformLocation(m_ShadowRenderingProgram, "Projection");
	
		for(_u32b Index = 0 ; Index < m_SelectedScene->getNumActors(); ++Index){
			if (!(m_SelectedScene->getActor(Index)->getID() & LIGHTSOURCE))
				continue;

			Source = dynamic_cast<LightSource*>( m_SelectedScene->getActor(Index));
					glBindFramebuffer(GL_FRAMEBUFFER, m_ShadowFBOs[ShadowIndex]);
					glDrawBuffer(GL_NONE);
					glReadBuffer(GL_NONE);
					glViewport(0, 0, m_Config.ShadowResolution, m_Config.ShadowResolution);
					glClear(GL_DEPTH_BUFFER_BIT);
					this->SetUniformvMtx(Location2, Source->getLightViewMtx());
					this->SetUniformvMtx(Location3, Source->getLightProjectionMtx(Eye));

					for (_u32b j = 0; j < NumActors; ++j)
						if (m_SelectedScene->getActor(j)->getID() & RENDERABLE) {
							actor = dynamic_cast<Renderable*>(m_SelectedScene->getActor(j));
							this->SetUniformvMtx(Location1, actor->getTransMtx());
							actor->CastShadow();
						}
					++ShadowIndex;
					if (Source->getLightCutoffAngle() < 0.0) {
						for (_u8b k = 1; k <6; ++k) {
							glBindFramebuffer(GL_FRAMEBUFFER, m_ShadowFBOs[ShadowIndex]);
							glDrawBuffer(GL_NONE);
							glReadBuffer(GL_NONE);
							glViewport(0, 0, m_Config.ShadowResolution, m_Config.ShadowResolution);
							glClear(GL_DEPTH_BUFFER_BIT);
							this->SetUniformvMtx(Location2, Source->getLightViewMtx()+k*16);
							for (_u32b j = 0; j < NumActors; ++j)
								if (m_SelectedScene->getActor(j)->getID() & RENDERABLE) {
									actor = dynamic_cast<Renderable*>(m_SelectedScene->getActor(j));
									this->SetUniformvMtx(Location1, actor->getTransMtx());
									actor->CastShadow();
								}
							++ShadowIndex;
						}
					}

		}
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);


		//combine Shadow map to a single one
		m_ShaderManager->BindProgram(m_ShadowAccumProgram);
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.0f, 0.0f, 0.0f,0.0f);
		glViewport(0, 0, m_Target->getWidth(), m_Target->getHeight());
		glEnable(GL_BLEND);
		glBlendEquation(GL_MAX);

		ShadowIndex = 0;
		Location1 = this->GetUniformLocation(m_ShadowAccumProgram, "PositionMap");
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_AttachmentTextures[POSITION_TEXTURE]);
		glUniform1i(Location1, 0);

		for (_u32b Index = 0; Index < m_SelectedScene->getNumActors(); ++Index) {
			if (!(m_SelectedScene->getActor(Index)->getID() & LIGHTSOURCE))
				continue;

			Source = dynamic_cast<LightSource*>(m_SelectedScene->getActor(Index));

			Location1 = this->GetUniformLocation(m_ShadowAccumProgram, "lighttype");
			if (Source->getLightDistance() < 0.0f)//Directionnal light
				glUniform1i(Location1, 1);
			else if (Source->getLightCutoffAngle()<0.0f)// Point light
				glUniform1i(Location1, 2);
			else                              //Spot light
				glUniform1i(Location1, 3);
			

			Location1 = this->GetUniformLocation(m_ShadowAccumProgram, "shadowStrengh");
			this->SetUniformF(Location1, Source->getShadowStrengh());

			Location1 = this->GetUniformLocation(m_ShadowAccumProgram, "LightProjMatrix");
			this->SetUniformvMtx(Location1, Source->getLightProjectionMtx(Eye));

			Location1 = this->GetUniformLocation(m_ShadowAccumProgram, "LightViewMtx");
			this->SetUniformvMtx(Location1, Source->getLightViewMtx());

			
			Location1 = this->GetUniformLocation(m_ShadowAccumProgram, "ShadowMap");
			//TODO :: pass handle instead
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, m_ShadowAttachmentTexture[ShadowIndex]);
			glUniform1i(Location1, 1);


			///VAOs
			glBindVertexArray(m_FinalRenderSurface->VertexArrayObject);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			++ShadowIndex;
			if (Source->getLightCutoffAngle() < 0.0) {
				for (_u8b k = 1; k < 6; ++k) {
					Location1 = this->GetUniformLocation(m_ShadowAccumProgram, "LightViewMtx");
					this->SetUniformvMtx(Location1, Source->getLightViewMtx() + k * 16);

					Location1 = this->GetUniformLocation(m_ShadowAccumProgram, "ShadowMap");
					glActiveTexture(GL_TEXTURE1);
					glBindTexture(GL_TEXTURE_2D, m_ShadowAttachmentTexture[ShadowIndex]);
					glUniform1i(Location1, 1);

					///VAOs
					glBindVertexArray(m_FinalRenderSurface->VertexArrayObject);
					glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
					++ShadowIndex;
				}
			}
		}

		//end
		glDisable(GL_BLEND);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindVertexArray(0);
		glActiveTexture(GL_NONE);
		m_ShaderManager->BindProgram(0);
	}
};
void	RGP_CORE::GLRenderer:: RenderSceneLightAccum(Camera* camera)
{
	GLint Location = -1;
	LightSource* Source = NULL;
	Camera *Eye = camera;
	if (!Eye)
		Eye = m_SelectedScene->getCamera();
	if (m_SelectedScene) {
		
		this->BindFrameBuffer(m_LightAccumBuffer);
		this->SetShaderProgram(m_LightAccumProgram);
		glDrawBuffers(2, DrawBuff);
		glDisable(GL_DEPTH_TEST);
		glViewport(0, 0, m_Target->getWidth(), m_Target->getHeight());
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		
		
		//common uniform variables
		this->BindBufferBase(GL_UNIFORM_BUFFER, 0, m_LightDataUBO);
		Location = this->GetUniformLocation(m_LightAccumProgram, "NumLights");
		this->SetUniformI(Location, m_NumRegisteredLights);

		Location = this->GetUniformLocation(m_LightAccumProgram, "NormalMap");
		this->BindTexture(m_AttachmentTextures[NORMAL_TEXTURE],0);
		this->SetUniformSample(Location, 0);
		Location = this->GetUniformLocation(m_LightAccumProgram, "PositionMap");
		this->BindTexture(m_AttachmentTextures[POSITION_TEXTURE],1);
		this->SetUniformSample(Location, 1);
		Location = this->GetUniformLocation(m_LightAccumProgram, "CameraPos");
		this->SetUniform3F(Location, Eye->getPosition().x,
			Eye->getPosition().y,
			Eye->getPosition().z);
		//render
		glBindVertexArray(m_FinalRenderSurface->VertexArrayObject);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	
		//after render
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindVertexArray(0);
		this->SetShaderProgram(0);

	}
};

void RGP_CORE::GLRenderer::RenderToTarget(_u32b FBO_Target){
        _s32b location = -1 ;
        _s8b shaderstring[50]="";
		glGetError();
        glBindFramebuffer(GL_FRAMEBUFFER, FBO_Target);

		glDrawBuffers(1, DrawBuff);
        glViewport(0,0,m_Target->getWidth(),m_Target->getHeight());
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.8f,0.8f,0.8f,0.0f);
        ///construct and render the final result
		m_ShaderManager->BindProgram(m_FinalRenderProgram);
		
		//TODO use texture handles instead
        ///other uniforms
        location= this->GetUniformLocation(m_FinalRenderProgram,"Diffuse");
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,m_AttachmentTextures[DIFFUSE_TEXTURE]);
        glUniform1i(location,0);
		

        location= this->GetUniformLocation(m_FinalRenderProgram,"Specular");
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D,m_AttachmentTextures[SPECULAR_TEXTURE]);
        glUniform1i(location,1);

		location = this->GetUniformLocation(m_FinalRenderProgram, "Normal");
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, m_AttachmentTextures[NORMAL_TEXTURE]);
		glUniform1i(location, 2);

        location= this->GetUniformLocation(m_FinalRenderProgram,"Depth");
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D,m_AttachmentTextures[DEPTH_TEXTURE]);
        glUniform1i(location,3);

		location = this->GetUniformLocation(m_FinalRenderProgram, "Shadow");
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, m_ShadowAccumTexture);
		glUniform1i(location,4);

		location = this->GetUniformLocation(m_FinalRenderProgram, "LightDiffuse");
		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_2D, m_LightAccumDiffuseTexture);
		glUniform1i(location, 5);

		location = this->GetUniformLocation(m_FinalRenderProgram, "LightSpecular");
		glActiveTexture(GL_TEXTURE6);
		glBindTexture(GL_TEXTURE_2D, m_LightAccumSpecularTexture);
		glUniform1i(location, 6);

		location = this->GetUniformLocation(m_FinalRenderProgram, "MaterialID");
		glActiveTexture(GL_TEXTURE7);
		glBindTexture(GL_TEXTURE_2D, m_AttachmentTextures[MATERIAL_TEXTURE]);
		glUniform1i(location, 7);

		location = this->GetUniformLocation(m_FinalRenderProgram, "Emissive");
		glActiveTexture(GL_TEXTURE8);
		glBindTexture(GL_TEXTURE_2D, m_AttachmentTextures[EMISSION_TEXTURE]);
		glUniform1i(location, 8);

		location = this->GetUniformLocation(m_FinalRenderProgram, "intensity");
		glUniform1f(location, m_Emissionintensity);


		location = this->GetUniformLocation(m_FinalRenderProgram, "noLight");
		if (m_noLightMode)
			glUniform1i(location, 1);
		else
			glUniform1i(location, 0);
        ///
        ///VAOs
        glBindVertexArray(m_FinalRenderSurface->VertexArrayObject);
        glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);


        glBindVertexArray(0);
		m_ShaderManager->BindProgram(0);

};


void	RGP_CORE::GLRenderer::UpdateEnvironmentMaps()
{

};


void	RGP_CORE::GLRenderer::SwitchNoLightMode()
{
	if (m_noLightMode)
		m_noLightMode = false;
	else
		m_noLightMode = true;
}

_u32b RGP_CORE::GLRenderer::getCameratransformsUBO()
{
	return m_CameraMtxUBO;
}

_u32b RGP_CORE::GLRenderer::getMaterialsUBO()
{
	return m_AllMaterialUBO;
}

_u32b RGP_CORE::GLRenderer::getCurrentShaderProgram()
{
	return m_CurrentShaderProgram;
}

_bool RGP_CORE::GLRenderer::DoesSupportBindlessTexture()
{
	if(glfwExtensionSupported("GL_ARB_bindless_texture")!= GLFW_TRUE)
		return false;
	return true;
}

void RGP_CORE::GLRenderer::printExtension()
{
	if (!exts_i) {
		num_exts_i = 0;
		glGetIntegerv(GL_NUM_EXTENSIONS, (GLint*)&num_exts_i);
		if (num_exts_i > 0) {
			exts_i = (_s8b **)malloc((size_t)num_exts_i * (sizeof *exts_i));
		}

		if (exts_i == NULL) {
			return;
		}

		for (_u32b index = 0; index < num_exts_i; index++) {
			const char *gl_str_tmp = (const char*)glGetStringi(GL_EXTENSIONS, index);
			exts_i[index] = CreateStringCopy(gl_str_tmp);
		}
	}
	for (_u32b index = 0; index < num_exts_i; ++index) {
		printf("%s\n", exts_i[index]);
	}
}

void    RGP_CORE::GLRenderer::setEmissionIntensity(_float value) 
{
	if (value > 0.0f)
		m_Emissionintensity = value;
	else m_Emissionintensity = 0.0f;
};
_float    RGP_CORE::GLRenderer::getEmissionIntensity() 
{
	return m_Emissionintensity;
};

_u32b RGP_CORE::GLRenderer::CreateMaterial(const Material* ptrData)
{
	OGLMaterial* tmp1 = NULL;
	GPUMaterial* tmp2 = NULL;
	_u32b index = 0;
	if (!(index=GetMaterialIndex(ptrData->Name))) {
		
		if (m_SizeofMaterialVector == m_NumMaterials) {
			
			tmp1 = (OGLMaterial*)malloc((m_SizeofMaterialVector + 5) * sizeof(OGLMaterial));
			if (!tmp1)
				return 0;
			if (DoesSupportBindlessTexture()) {
				tmp2 = (GPUMaterial*)malloc((m_SizeofMaterialVector + 5) * sizeof(GPUMaterial));
				if (!tmp2) {
					free(tmp1);
					return 0;
				}
			}

			
			if (DoesSupportBindlessTexture() && !tmp2)
				return 0;
			
			for (_u32b i = 0; i < m_NumMaterials; ++i) {
				tmp1[i] = m_Materials[i];
				if (DoesSupportBindlessTexture())
					tmp2[i] = m_GPUMaterials[i];
			}

			free(m_Materials);
			m_Materials = tmp1;
			if (DoesSupportBindlessTexture()) {
				free(m_GPUMaterials);
				m_GPUMaterials = tmp2;
			}
			m_SizeofMaterialVector += 5;
		}
		//copy material data 
		m_Materials[m_NumMaterials].Name = CreateStringCopy(ptrData->Name);
		m_Materials[m_NumMaterials].DiffuseMap = 0;
		m_Materials[m_NumMaterials].SpecularMap = 0;
		m_Materials[m_NumMaterials].NormalsMap = 0;
		m_Materials[m_NumMaterials].EmissiveMap = 0;
		m_Materials[m_NumMaterials].IOR = ptrData->IOR;
		m_Materials[m_NumMaterials].Opacity = ptrData->Opacity;
		if (DoesSupportBindlessTexture()) {
			m_GPUMaterials[m_NumMaterials].DiffuseBindless = 0;
			m_GPUMaterials[m_NumMaterials].SpecularBindless = 0;
			m_GPUMaterials[m_NumMaterials].NormalBindless = 0;
			m_GPUMaterials[m_NumMaterials].EmissiveBindless = 0;
			m_GPUMaterials[m_NumMaterials].IOR = ptrData->IOR;
			m_GPUMaterials[m_NumMaterials].Opacity = ptrData->Opacity;
		}
		if (ptrData->DiffuseMap && ptrData->DiffuseMap->Pixels) {
			this->GenTextures2D(1, &(m_Materials[m_NumMaterials].DiffuseMap));
			this->BindTexture(m_Materials[m_NumMaterials].DiffuseMap);
			this->SetImageData2D(ptrData->DiffuseMap);
			
			if (DoesSupportBindlessTexture()) {
				m_GPUMaterials[m_NumMaterials].DiffuseBindless = this->GetTextureHandle(m_Materials[m_NumMaterials].DiffuseMap);
				this->MakeTextureHandleResidant(m_GPUMaterials[m_NumMaterials].DiffuseBindless);
			}
			this->BindTexture(0);
		}
		
		if (ptrData->SpecularMap && ptrData->SpecularMap->Pixels) {
			this->GenTextures2D(1, &(m_Materials[m_NumMaterials].SpecularMap));
			this->BindTexture(m_Materials[m_NumMaterials].SpecularMap);
			this->SetImageData2D(ptrData->SpecularMap);
			this->BindTexture(0);
			if (DoesSupportBindlessTexture()) {
				m_GPUMaterials[m_NumMaterials].SpecularBindless = this->GetTextureHandle(m_Materials[m_NumMaterials].SpecularMap);
				this->MakeTextureHandleResidant(m_GPUMaterials[m_NumMaterials].SpecularBindless);
			}
		}
		if (ptrData->NormalsMap && ptrData->NormalsMap->Pixels) {
			this->GenTextures2D(1, &(m_Materials[m_NumMaterials].NormalsMap));
			this->BindTexture(m_Materials[m_NumMaterials].NormalsMap);
			this->SetImageData2D(ptrData->NormalsMap);
			this->BindTexture(0);
			if (DoesSupportBindlessTexture()) {
				m_GPUMaterials[m_NumMaterials].NormalBindless = this->GetTextureHandle(m_Materials[m_NumMaterials].NormalsMap);
				this->MakeTextureHandleResidant(m_GPUMaterials[m_NumMaterials].NormalBindless);
			}
		}
		if (ptrData->EmissiveMap && ptrData->EmissiveMap->Pixels) {
			this->GenTextures2D(1, &(m_Materials[m_NumMaterials].EmissiveMap));
			this->BindTexture(m_Materials[m_NumMaterials].EmissiveMap);
			this->SetImageData2D(ptrData->EmissiveMap);
			this->BindTexture(0);
			if (DoesSupportBindlessTexture()) {
				m_GPUMaterials[m_NumMaterials].EmissiveBindless = this->GetTextureHandle(m_Materials[m_NumMaterials].EmissiveMap);
				this->MakeTextureHandleResidant(m_GPUMaterials[m_NumMaterials].EmissiveBindless);
			}
		}
		++m_NumMaterials;
		//TODO:: update material UBO
		//this->UpdateMaterialsUBO();
		return m_NumMaterials-1;
	}
	else {
		return index;
	}

}
_u32b RGP_CORE::GLRenderer::GetMaterialIndex( const _s8b * Name)
{
	if (!m_NumMaterials)
		return 0;
	for(_u32b i=0 ; i< m_NumMaterials ; ++i )
		if (strcmp(m_Materials[i].Name, Name) == 0) {
			return i;
		}
	return 0;
}
RGP_CORE::OGLMaterial * RGP_CORE::GLRenderer::GetMaterial(_u32b index)
{
	if (index >= m_NumMaterials)
		return NULL;
	return &(m_Materials[index]);
}
_bool RGP_CORE::GLRenderer::RemoveMaterial(const _s8b * materialname)
{
	return this->RemoveMaterialAt(GetMaterialIndex(materialname));
}
_bool RGP_CORE::GLRenderer::RemoveMaterialAt(_u32b index)
{
	if (index < 1 || index >= m_NumMaterials)
		return false;
	if (m_Materials[index].Name) {
		free(m_Materials[index].Name);
	}
	if (m_Materials[index].DiffuseMap) {
		this->DeleteTextures(1, &(m_Materials[index].DiffuseMap));
	}
	if (m_Materials[index].SpecularMap) {
		this->DeleteTextures(1, &(m_Materials[index].SpecularMap));
	}
	if (m_Materials[index].NormalsMap) {
		this->DeleteTextures(1, &(m_Materials[index].NormalsMap));
	}
	if (m_Materials[index].EmissiveMap) {
		this->DeleteTextures(1, &(m_Materials[index].EmissiveMap));
	}
	for (_u32b i = index; i < m_NumMaterials-1; ++i) {
		m_Materials[i] = m_Materials[i + 1];
	}
	--m_NumMaterials;
	this->UpdateMaterialsUBO();
	return true;
}

void RGP_CORE::GLRenderer::ClearMaterials()
{
	if (m_Materials) {
		for (_u32b i =0; i<m_NumMaterials; ++i) {
			if (m_Materials[i].Name) {
				free(m_Materials[i].Name);
			}
			if (m_Materials[i].DiffuseMap) {
				this->DeleteTextures(1, &(m_Materials[i].DiffuseMap));
			}
			if (m_Materials[i].SpecularMap) {
				this->DeleteTextures(1, &(m_Materials[i].SpecularMap));
			}
			if (m_Materials[i].NormalsMap) {
				this->DeleteTextures(1, &(m_Materials[i].NormalsMap));
			}
			if (m_Materials[i].EmissiveMap) {
				this->DeleteTextures(1, &(m_Materials[i].EmissiveMap));
			}
		}
		free(m_Materials);
		m_Materials = NULL;
		m_NumMaterials = 0;
		m_SizeofMaterialVector = 0;
	}
	if (m_GPUMaterials) {
		free(m_GPUMaterials);
		m_GPUMaterials = NULL;
	}

	if (defaultMAaterial.DiffuseMap) {
		if (defaultMAaterial.DiffuseMap->Pixels) {
			free(defaultMAaterial.DiffuseMap->Pixels);
			defaultMAaterial.DiffuseMap->Pixels = NULL;
		}
		free(defaultMAaterial.DiffuseMap);
		defaultMAaterial.DiffuseMap = NULL;
	}
	if (defaultMAaterial.SpecularMap) {
		if (defaultMAaterial.SpecularMap->Pixels) {
			free(defaultMAaterial.SpecularMap->Pixels);
			defaultMAaterial.SpecularMap->Pixels = NULL;
		}
		free(defaultMAaterial.SpecularMap);
		defaultMAaterial.SpecularMap = NULL;
	}
	if (defaultMAaterial.NormalsMap) {
		if (defaultMAaterial.NormalsMap->Pixels) {
			free(defaultMAaterial.NormalsMap->Pixels);
			defaultMAaterial.NormalsMap->Pixels = NULL;
		}
		free(defaultMAaterial.NormalsMap);
		defaultMAaterial.NormalsMap = NULL;
	}
	if (defaultMAaterial.EmissiveMap) {
		if (defaultMAaterial.EmissiveMap->Pixels) {
			free(defaultMAaterial.EmissiveMap->Pixels);
			defaultMAaterial.EmissiveMap->Pixels = NULL;
		}
		free(defaultMAaterial.EmissiveMap);
		defaultMAaterial.EmissiveMap = NULL;
	}

}

_u32b RGP_CORE::GLRenderer::getNumMetrials()
{
	return m_NumMaterials;
}

void RGP_CORE::GLRenderer::Color(_float red, _float green, _float blue)
{
	glColor3f(0.5f, 0.5f, 0.5f);
}







_bool RGP_CORE::GLRenderer::GenBuffers(_u32b numBuffers,_u32b*    target){
    if(!target)
        return false ;
	glGetError();
	glGenBuffers(numBuffers,(GLuint*)target);
    if(glGetError()){
        printf("error generating Buffers\n");
        return false ;
    }
    return true ;
};
void  RGP_CORE::GLRenderer::DeleteBuffers(_u32b numBuffers, _u32b*    target){
    if(!target)
        return ;
    glDeleteBuffers(numBuffers,(GLuint*)target);
    for(_u32b i=0 ; i<numBuffers;++i)
        target[i]=0;
};
void  RGP_CORE::GLRenderer::setBufferData(_u32b Target,_u32b SizeinByte, void* Data, _u32b flag, _u32b bufferwrapper){
	
	glGetError();
	if (bufferwrapper)
		glNamedBufferData(bufferwrapper, SizeinByte, Data, flag);
	else
		glBufferData(Target,SizeinByte,Data,flag);
    if(glGetError())
        printf("error filling buffer with Data\n");
};
void   RGP_CORE::GLRenderer::setBufferSubData(_u32b Target, _u32b Offset, _u32b Size, void* Data, _u32b bufferwrapper)
{
	glGetError();
	if (bufferwrapper)
		glNamedBufferSubData(bufferwrapper, Offset, Size, Data);
	else
		glBufferSubData(Target, Offset, Size, Data);
	if (glGetError())
		printf("error filling buffer with sub Data\n");
};
void  RGP_CORE::GLRenderer::BindBuffer(_u32b Bindtype,_u32b BufferID ){
	
	glGetError();
    glBindBuffer(Bindtype,BufferID);
	if (glGetError())
		printf("error binding buffer\n");
}
void RGP_CORE::GLRenderer::BindBufferBase(_u32b Target, _u32b index, _u32b BufferID)
{
	glGetError();
	glBindBufferBase(Target, index, BufferID);
	if (glGetError()) {
		printf("NOPE");
	}
}
;

///VAOs
_bool RGP_CORE::GLRenderer::GenVertexArrays(_u32b numBuffers,_u32b*    target){
    if(!target)
        return false ;
    glGenVertexArrays(numBuffers, (GLuint*)target);
    if(glGetError())
        return false ;
    return true ;
};
void  RGP_CORE::GLRenderer::DeleteVertexArrays(_u32b numBuffers,_u32b*    target){
	if (!target){
		return;
	}
    glDeleteVertexArrays(numBuffers, (GLuint*)target);
    for(_u32b i=0; i< numBuffers; ++i)
        target[i]=0;
};
_bool  RGP_CORE::GLRenderer::BindVertexArray(_u32b BufferID){
    glBindVertexArray(BufferID);
    if(glGetError())
        return false ;
    return true ;
};
void RGP_CORE::GLRenderer::DrawElements(_u32b mode,_u32b Count,GLenum Type,void* Offset,_u32b numInstances){
    glDrawElementsInstanced(mode,Count,Type,Offset,numInstances);
};
void RGP_CORE::GLRenderer::DrawArrays(_u32b mode, _u32b first, _u32b count,  _u32b numInstances)
{
	glDrawArraysInstanced(mode, first, count,numInstances);
}
_bool RGP_CORE::GLRenderer::DrawElementsIndirect(_u32b mode, _u32b Type, const DrawElementsIndirectCommand * command)
{
	int error;
	glGetError();
	glDrawElementsIndirect(mode, Type, command);
	if (error=glGetError()) {
		return false;
	}
	return true;
}
_bool RGP_CORE::GLRenderer::DrawArraysIndirect(_u32b mode, const DrawArraysIndirectCommand * command)
{
	glGetError();
	glDrawArraysIndirect(mode, command);
	if (glGetError())
		return false;
	return true;
}
_bool RGP_CORE::GLRenderer::MultiDrawElementsIndirect(_u32b mode, _u32b Type, const void * commands_or_Offset, _u32b count, _u32b stride)
{
	if (!glMultiDrawElementsIndirect)
		return false;
	else
		glMultiDrawElementsIndirect(mode, Type, commands_or_Offset, count, stride);
}
_bool RGP_CORE::GLRenderer::MultiDrawArraysIndirect(_u32b mode, const void * indirect, _u32b drawcount, _u32b stride)
{
	if (!glMultiDrawArraysIndirect)
		return false;
	else
		glMultiDrawArraysIndirect(mode,indirect,drawcount,stride);
}
;
///Textures
_bool RGP_CORE::GLRenderer::GenTextures2D(_u32b numTextures,_u32b*    target){
    if(!target)
        return false ;
    glGenTextures(numTextures, (GLuint*)target);
    if(glGetError()){
        return false ;
    }
    for(_u32b i = 0 ; i< numTextures ; ++i){
        glBindTexture(GL_TEXTURE_2D,target[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    }
	if (glGetError()) {
		printf("error while creating texture \n");
	}
    glBindTexture(GL_TEXTURE_2D,0);
    return true ;
};
_bool RGP_CORE::GLRenderer::GenTexturesCube(_u32b numTextures, _u32b*    target)
{
	if (!target)
		return false;
	glGenTextures(numTextures, (GLuint*)target);
	if (glGetError()) {
		return false;
	}
	for (_u32b i = 0; i< numTextures; ++i) {
		glBindTexture(GL_TEXTURE_CUBE_MAP, target[i]);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	return true;
};
void  RGP_CORE::GLRenderer::DeleteTextures(_u32b numTextures,_u32b*    target){
    if(!target)
        return ;
    glDeleteTextures(numTextures, (GLuint*)target);
    for(_u32b i=0 ;i<numTextures;++i)
        target[i]=0;
};
void  RGP_CORE::GLRenderer::SetImageData2D(RGP_CORE::Image* ImageSource){
	glGetError();
	if (ImageSource) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,
			ImageSource->Width, ImageSource->Height,
			0, GL_BGRA, GL_UNSIGNED_BYTE, ImageSource->Pixels);
	}
	if (glGetError()) {
		printf("error loading data in textures  \n");
	}

};
void  RGP_CORE::GLRenderer::SetImageDataCube(RGP_CORE::Image* right , RGP_CORE::Image* left, RGP_CORE::Image* front,
													RGP_CORE::Image* back, RGP_CORE::Image* top, RGP_CORE::Image* bottom) 
{
		
};
void  RGP_CORE::GLRenderer::SetImageDataCube(_s32b Width, _s32b Height) 
{
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X  , 0, GL_RGBA32F, Width, Height, 0, GL_RGBA, GL_FLOAT, 0);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+1, 0, GL_RGBA32F, Width, Height, 0, GL_RGBA, GL_FLOAT, 0);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+2, 0, GL_RGBA32F, Width, Height, 0, GL_RGBA, GL_FLOAT, 0);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+3, 0, GL_RGBA32F, Width, Height, 0, GL_RGBA, GL_FLOAT, 0);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+4, 0, GL_RGBA32F, Width, Height, 0, GL_RGBA, GL_FLOAT, 0);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+5, 0, GL_RGBA32F, Width, Height, 0, GL_RGBA, GL_FLOAT, 0);
};
void  RGP_CORE::GLRenderer::SetActiveTexture(_u16b index)
{
    glActiveTexture(GL_TEXTURE0+index);
};
_bool RGP_CORE::GLRenderer::BindTexture(_u32b textureID, _u32b unit ,_bool Texture2D){
	glGetError();
	this->SetActiveTexture(unit);
    if(Texture2D)
		glBindTexture(GL_TEXTURE_2D,textureID);
	else
		glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	if (glGetError()) {
		printf("error binding textures\n");
		return false;
	}
	this->SetActiveTexture(0);
    return true ;
};


_u64b RGP_CORE::GLRenderer::GetTextureHandle(_u32b textureID)
{
	
	if (glGetTextureHandleARB(textureID))
		return  glGetTextureHandleARB(textureID);
	else if (glGetTextureHandleNV)
		return glGetTextureHandleNV(textureID);
	else
		return 0;
};
void  RGP_CORE::GLRenderer::MakeTextureHandleResidant(_u64b texHandle,_bool makeResidant )
{
	if (glMakeTextureHandleResidentARB) {
		if (makeResidant) {
			glMakeTextureHandleResidentARB(texHandle);
		}
		else {
			glMakeTextureHandleNonResidentARB(texHandle);
		}
		return;
	}
	if (glMakeTextureHandleResidentNV) {
		if (makeResidant) {
			glMakeTextureHandleResidentNV(texHandle);
		}
		else {
			glMakeTextureHandleNonResidentNV(texHandle);
		}
		return;
	}
	printf("make resident is not supported\n");
	
};


//FBOs
_bool RGP_CORE::GLRenderer::GenFrameBuffers(_u32b numFrameBuffers, _u32b* target)
{
	if (!target)
		return false;
	int error =glGetError();
	glGenFramebuffers(numFrameBuffers, (GLuint*)target);
	if (error = glGetError()) {
		
		return false;
	}
	return true;
};
void RGP_CORE::GLRenderer::DeleteFrameBuffers(_u32b numFrameBuffers, _u32b* target)
{
	if (target) {
		glDeleteFramebuffers(numFrameBuffers, (GLuint*)target);
	}
};
_bool RGP_CORE::GLRenderer::BindFrameBuffer(_u32b BufferID)
{
	int error = glGetError();
	glBindFramebuffer(GL_FRAMEBUFFER, BufferID);
	if (error = glGetError()) {
		printf("error binding FBO\n");
		return false;
	}
	return true;
};
_bool RGP_CORE::GLRenderer::AttachTexturetoFrameBuffer(GLenum AttachementID, GLenum TextureTarget, _u32b TextureID, _s32b Level)
{
	glFramebufferTexture2D(GL_FRAMEBUFFER, AttachementID, TextureTarget, TextureID, Level);
	return true;
};

void  RGP_CORE::GLRenderer::setDrawBuffers(GLenum* bufferenum, _s32b numBuffers)
{
	if (numBuffers == 0)
		glDrawBuffer(GL_NONE);
	else if(numBuffers > 0)
		glDrawBuffers(numBuffers, bufferenum);
};
void  RGP_CORE::GLRenderer::setReadBuffers(GLenum bufferenum) 
{
	glReadBuffer(bufferenum);
};

///ShaderProgramManagement

_u32b   RGP_CORE::GLRenderer::CreateGLProgramFromFile(const _s8b* VertexFile, const  _s8b* FragmentFile){
	return CreateGLProgramFromFile(VertexFile, NULL, FragmentFile);
};
_u32b  RGP_CORE::GLRenderer::CreateGLProgramFromFile(const  _s8b* VertexFile,const   _s8b* GeometryFile,const  _s8b* FragmentFile)
{
	if (m_ShaderManager) {
		return m_ShaderManager->CreateProgramFromFiles(VertexFile, GeometryFile, FragmentFile);
	}
	return 0;
};
void    RGP_CORE::GLRenderer::DeleteGLProgram(_u32b ProgramID){
		m_ShaderManager->DeleteProgram(ProgramID);
};
_s32b   RGP_CORE::GLRenderer::GetUniformLocation(_u32b programID ,_s8b*  Name){
	GLShaderProgramsManager::ShaderProgram* program = NULL;
	program = m_ShaderManager->getShaderProgram(programID);
	GLint location;
	if (program) {
		location= glGetUniformLocation(program->GLProgramID, Name);
		return location;
	}
	return -1;
};
_bool   RGP_CORE::GLRenderer::SetUniformF(_s32b Location,_float data )
{
     glUniform1f(Location,data);
     return true ;
};
_bool    RGP_CORE::GLRenderer::SetUniform3F(_s32b Location, _float data1, _float data2, _float data3)
{
	glUniform3f(Location, data1, data2, data3);
	return true;
};
_bool   RGP_CORE::GLRenderer::SetUniformI(_s32b Location, _u32b data)
{
	glUniform1i(Location, data);
	return true;
}
_bool RGP_CORE::GLRenderer::SetUniform64I(_s32b Location, _u64b data)
{
	if (glUniform1ui64ARB) {
		glUniform1ui64ARB(Location, data);
		return true;
	}
	else if (glUniform1ui64NV) {
		glUniform1ui64NV(Location, data);
		return true;
	}
	printf("not supported func\n");
	return true;
}
;
_bool   RGP_CORE::GLRenderer::SetUniformFv(_s32b Location,_float* data,_u32b numElements )
{
    glUniform1fv(Location,numElements,data);
    if(glGetError())
        return false ;
    return true ;
};
_bool   RGP_CORE::GLRenderer::SetUniformvMtx(_s32b Location,_float* Matrix_4x4 ){
    glUniformMatrix4fv(Location,1,GL_FALSE,Matrix_4x4);
    if(glGetError())
        return false ;
    return true ;
};
_bool   RGP_CORE::GLRenderer::SetUniformSample(_s32b Location, _u32b TextureUnit){
    glUniform1i(Location,TextureUnit);
     if(glGetError()){
        return false ;
     }
    return true ;
};
_bool	RGP_CORE::GLRenderer::SetUniformHandleu64(_s32b Location, _u64b Handle)
{
	if (glUniformHandleui64ARB) {
		glUniformHandleui64ARB(Location, Handle);
		return true;
	}
	else if (glUniformHandleui64NV) {
		glUniformHandleui64NV(Location, Handle);
		return true;
	}
	return false;
};
_bool	RGP_CORE::GLRenderer::SetUniformHandleu64v(_s32b Location, _u32b Count, _u64b* Handle) 
{
	if (glUniformHandleui64vARB) {
		glUniformHandleui64vARB(Location, Count,Handle);
		return true;
	}
	else if (glUniformHandleui64vNV) {
		glUniformHandleui64vNV(Location, Count ,Handle);
		return true;
	}
	return false;
};


_bool   RGP_CORE::GLRenderer::SetVertexAttribPointerF(_u32b Index,_u32b NumElemntsPerVertex,
                               _u32b offsetBetweenElements,void* offsetFromFirst)
{
    glVertexAttribPointer(Index,NumElemntsPerVertex,GL_FLOAT,GL_FALSE,offsetBetweenElements,offsetFromFirst);
     if(glGetError())
        return false ;
    return true ;
};
_bool   RGP_CORE::GLRenderer::SetVertexAttribPointerI(_u32b Index, _u32b NumElemntsPerVertex,
	_u32b offsetBetweenElements, void* offsetFromFirst)
{
	glVertexAttribPointer(Index, NumElemntsPerVertex, GL_UNSIGNED_INT, GL_FALSE, offsetBetweenElements, offsetFromFirst);
	if (glGetError())
		return false;
	return true;
};
_bool   RGP_CORE::GLRenderer::EnableVertexAttribArray(_u32b index){
    glEnableVertexAttribArray(index);
     if(glGetError())
        return false ;
    return true ;
};
_bool   RGP_CORE::GLRenderer::DisableVertexAttribArray(_u32b index){
    glDisableVertexAttribArray(index);
     if(glGetError())
        return false ;
    return true ;
};
void    RGP_CORE::GLRenderer::SetShaderProgram(_u32b programID){
	
		m_CurrentShaderProgram = programID;
		m_ShaderManager->BindProgram(programID);

};

 
