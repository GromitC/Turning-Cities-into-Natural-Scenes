#include "Display.h"
#include "time.h"

//GLOBAL VARIABLE
string terrainFile;
int mode=1;
bool tex=true;
double MouseWheel=0;
double x=0,y=0,z=0;
float lightX=0,lightY=10,lightZ=0;
vector<string> terTexts1;
vector<string> terTexts2;
int textureChoice=2;
int currentChoice=2;


void KeyboardCallback(GLFWwindow* window, int key, int code, int action, int mods) {

    if(action == GLFW_PRESS){
    switch (key) {
        case GLFW_KEY_ESCAPE:
          glfwSetWindowShouldClose(window, GL_TRUE);
          break;
        case GLFW_KEY_1:
            mode=1;
            terrainFile = "Objects/Manhattan-2.obj";
            break;
        case GLFW_KEY_2:
            mode=2;
            terrainFile = "Objects/manhattan.png";
            break; 
        case GLFW_KEY_3:
            textureChoice=2;
            break; 
        case GLFW_KEY_4:
            textureChoice=1;
            break;  
        case GLFW_KEY_T:
            tex=!tex;
            cout<<tex<<endl;
            break;
        case GLFW_KEY_A:
            z-=10;
            break;
        case GLFW_KEY_D:
            z+=10;
            break;
        case GLFW_KEY_W:
            x-=10;
            break;
        case GLFW_KEY_S:
            x+=10;
            break; 
        case GLFW_KEY_Q:
            y-=10;
            break;
        case GLFW_KEY_E:
            y+=10;
            break; 
        case GLFW_KEY_UP:
            lightY+=1;
            break;
        case GLFW_KEY_DOWN:
            lightY-=1;
            break;
        case GLFW_KEY_LEFT:
            lightX-=1;
            break;
        case GLFW_KEY_RIGHT:
            lightX+=1;
            break;              

    }
    }
}

void MouseWheelCallback(GLFWwindow *w, double xoffset, double yoffest) {
    MouseWheel = yoffest;
}

Display::Display(int w, int h, const char* title) :
	MainCam(20, Vec3(0, 0, 0)) {
    mode=1;

    // start GL context and O/S window using the GLFW helper library
    if (!glfwInit ()) {
    	fprintf (stderr, "ERROR: could not start GLFW3\n");
    	// exit(1);
    } 

    glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    _windowHeight = h;
    _windowWidth = w;
    _window = glfwCreateWindow(w, h, title, NULL, NULL);
    if (!_window)
    {
        fprintf (stderr, "ERROR: could not open window with GLFW3\n");
        glfwTerminate();
    }

    glfwMakeContextCurrent(_window);

    glewExperimental = GL_TRUE;
    glewInit();

	_terrainShader = GLSLShader("Shaders/vertex.glsl", "Shaders/fragment.glsl");
   



    glClearColor(28.0f/255.0f,144.0f/255.0f,153.0f/255.0f, 0);
    //glClearColor(1,1,1, 0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    glfwSetKeyCallback(_window, KeyboardCallback);
    glfwSetScrollCallback(_window, MouseWheelCallback);
}

Display::~Display() {
	glfwTerminate();
}

void Display::Run() {

	displayLoop();
}


void Display::displayLoop() { 
        projection = Mat4::Identity();
    view = Mat4::Identity();
    PMVmatrix = Mat4::Identity();


    float fov = 45.0f;
    projection = Perspective(fov, _windowWidth/(float)_windowHeight, 0.1f, 10000.0f);
    view = MainCam.View();

    PMVmatrix = projection * view;

	terrainFile = "Objects/Manhattan-2.obj";
    //string terrainFile = "Objects/manhattan.png";
	
	terTexts2.push_back("Textures/mountain1.png");
	terTexts2.push_back("Textures/mountain2.png");
	terTexts2.push_back("Textures/mountain3.png");
	terTexts2.push_back("Textures/mountain4.png");

    terTexts1.push_back("Textures/water1.png");
    terTexts1.push_back("Textures/water2.png");
    terTexts1.push_back("Textures/water3.png");
    terTexts1.push_back("Textures/water4.png");
	vector<string> terLocs;
	terLocs.push_back("text1");
	terLocs.push_back("text2");
	terLocs.push_back("text3");
	terLocs.push_back("text4");

	_terrainShader.Use();
	_resManager.GetObject(terrainFile, &_terrainShader, tex&&terTexts2.size() > 0, terTexts2, terLocs,mode,false);
	if (!_resManager.ScaleObject(terrainFile, Vec3(100, 100, 100))) {
		printf("Object not found\n");
	}

    if (mode==1){
	   if (!_resManager.TranslateObject(terrainFile, Vec3(0, -10, 20))) {
		  printf("Object not found\n");
	   }
    }
    else{
        if (!_resManager.TranslateObject(terrainFile, Vec3(0, 0, 20))) {
        printf("Object not found\n");
    }
    }

	GLuint mtex_AP, mtex_DP, mtex_SP, mtex_Shine, mtex_LP;

    mtex_AP = glGetUniformLocation(_terrainShader.GetId(), "AmbientProduct");
    mtex_DP = glGetUniformLocation(_terrainShader.GetId(), "DiffuseProduct");
    mtex_SP = glGetUniformLocation(_terrainShader.GetId(), "SpecularProduct");
    mtex_Shine = glGetUniformLocation(_terrainShader.GetId(), "Shininess");
    mtex_LP = glGetUniformLocation(_terrainShader.GetId(), "LightPosition");

	glUniform4f(mtex_AP, Ambient.v[0], Ambient.v[1], Ambient.v[2], Ambient.v[3]);
    glUniform4f(mtex_DP, Diffuse.v[0], Diffuse.v[1], Diffuse.v[2], Diffuse.v[3]);
    glUniform4f(mtex_SP, Specular.v[0], Specular.v[1], Specular.v[2], Specular.v[3]);
    glUniform1f(mtex_Shine, shininess);
    

	float lastUpdate = clock();

	while (!glfwWindowShouldClose (_window)) {
        glUniform4f(mtex_LP, lightX, lightY, lightZ, 1);

	    glClearDepth(1.0);
	
	    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	    while (clock() - 16 < lastUpdate);
	    lastUpdate = clock();

	    updateCam();

        _resManager.TranslateObject(terrainFile, Vec3(x, y, z));

        if(textureChoice==1){
            if (textureChoice!=currentChoice){
                _resManager.GetObject(terrainFile, &_terrainShader, tex&&terTexts1.size() > 0, terTexts1, terLocs,mode,true);
                currentChoice=textureChoice;
            }

            renderObj(_terrainShader, terrainFile, terTexts1, terLocs);            
        }

        else{
            if (textureChoice!=currentChoice){
                _resManager.GetObject(terrainFile, &_terrainShader, tex&&terTexts2.size() > 0, terTexts2, terLocs,mode,true);
                currentChoice=textureChoice;
            }
            renderObj(_terrainShader, terrainFile, terTexts2, terLocs);
        }


		glfwSwapBuffers(_window);

    	glfwPollEvents();
	}

	glfwTerminate();
}

void Display::updateCam() {

	double xpos, ypos;
    glfwGetCursorPos(_window, &xpos, &ypos);

    if (glfwGetMouseButton(_window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS)
    {
    	MainCam.Update(xpos - MouseLastX, ypos - MouseLastY);
    }

    MainCam.DeltaDistance(MouseWheel);
    MouseWheel = 0;

    view = MainCam.View();

    MouseLastX = xpos;
   	MouseLastY = ypos;
}

void Display::renderObj(GLSLShader shader, string objPath, vector<string> textureFiles, vector<string> textureLocs) {

	GLObj model = _resManager.GetObject(objPath, &shader, tex&&textureFiles.size() > 0, textureFiles, textureLocs,mode,false);
    _resManager.ScaleObject(terrainFile, Vec3(100, 100, 100));
    if(mode==1){
        _resManager.TranslateObject(terrainFile, Vec3(0, -10, 20));
    }else{
        _resManager.TranslateObject(terrainFile, Vec3(-10, 0, -30));
    }
	glUseProgram(model.shader);

	for (int i = 0; i < textureFiles.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, model.textures[i]);
		glUniform1i(model.textureLocs[i], i);
	}

	glBindVertexArray(model.vaoId);
	glEnableVertexAttribArray(0);

	glUniformMatrix4fv(shader.GetMV(), 1, GL_FALSE, (view * Translate(model.Position) * Rotate(model.Rotation) * Scale(model.Scale)).matrix);
    glUniformMatrix4fv(shader.GetProj(), 1, GL_FALSE,  projection.matrix);

    if(mode==1){
	glDrawArrays(GL_TRIANGLES, 0, model.size);
    }else{
         for (int i=0;i<512*2;i++)
        //     glDrawArrays(GL_TRIANGLE_STRIP, i*512, 512);  
        glDrawArrays(GL_LINE_STRIP, i*512, 512);
    }
	
	// Unbind
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
}
