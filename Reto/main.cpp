//https://open.gl/textures
#include <GL/glut.h>
#include <stdlib.h>
#include <random>
#include <iomanip>
// #include "SOIL/SOIL.h"
#include "Cubo.h"
#include <string>
#include "RgbImage.h"

#define NTextures 1
GLuint texture[NTextures];

//Variables dimensiones de la pantalla
int WIDTH=500;
int HEIGTH=500;
//Variables para establecer los valores de gluPerspective
float FOVY=100.0;
float ZNEAR=0.01;
float ZFAR=900.0;
//Variables para definir la posicion del observador
//gluLookAt(EYE_X,EYE_Y,EYE_Z,CENTER_X,CENTER_Y,CENTER_Z,UP_X,UP_Y,UP_Z)
float EYE_X=0.0;
float EYE_Y=200.0;
float EYE_Z=0.0;
float CENTER_X=10;
float CENTER_Y=0;
float CENTER_Z=10;
float UP_X=0;
float UP_Y=0;
float UP_Z=-1;
//Variables para dibujar los ejes del sistema
float X_MIN=-500;
float X_MAX=500;
float Y_MIN=-500;
float Y_MAX=500;
float Z_MIN=-500;
float Z_MAX=500;
//Size del tablero
int DimBoard = 200;
const int NUMNODES = 32;
// Localizacion de los nodos
float LocNodos[NUMNODES][2];
int TransitionMatrix[NUMNODES][NUMNODES];
int NumberOfAgents = 10;
//
float speed = 2;
int LightColor = 0;
int LightCTRL = 0;
Cubo Agents[16];
int AnextNode[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

char* fondo = "texturene24.bmp";


using namespace std;

void loadTextureFromFile(char *filename, int index)
{
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT);
	//glEnable(GL_DEPTH_TEST);

	RgbImage theTexMap( filename );

    //generate an OpenGL texture ID for this texture
    glGenTextures(1, &texture[index]);
    //bind to the new texture ID
    glBindTexture(GL_TEXTURE_2D, texture[index]);


    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, theTexMap.GetNumCols(), theTexMap.GetNumRows(), 0,
                     GL_RGB, GL_UNSIGNED_BYTE, theTexMap.ImageData());
    theTexMap.Reset();
}

void drawAxis()
{
     glLineWidth(3.0);
     //X axis in red
     glColor3f(1.0f,0.0f,0.0f);
     glBegin(GL_LINES);
       glVertex3f(X_MIN,0.0,0.0);
       glVertex3f(X_MAX,0.0,0.0);
     glEnd();
     //Y axis in green
     glColor3f(0.0f,1.0f,0.0f);
     glBegin(GL_LINES);
       glVertex3f(0.0,Y_MIN,0.0);
       glVertex3f(0.0,Y_MAX,0.0);
     glEnd();
     //Z axis in blue
     glColor3f(0.0f,0.0f,1.0f);
     glBegin(GL_LINES);
       glVertex3f(0.0,0.0,Z_MIN);
       glVertex3f(0.0,0.0,Z_MAX);
     glEnd();
     glLineWidth(1.0);
 }

void drawString(int x, int y, int z, const char* text) {
  //glEnable(GL_TEXTURE_3D);
  glColor3f(1.0f, 1.0f, 1.0f);
  // Render the text
  glRasterPos3i(x, y, z);
  string s = text;
  void * font = GLUT_BITMAP_9_BY_15;
  for (string::iterator i = s.begin(); i != s.end(); ++i){
    char c = *i;
    glutBitmapCharacter(font, c);
  }
}

void RandNodes(int NodeSeq[]){
  NodeSeq[0] = 0;
  for(int i = 1; i < 100; i ++){
    NodeSeq[i] = rand() % 16;
  }

}

void PopulateLocNodes(){
  LocNodos[0][0] = -192;LocNodos[0][1] = -174;
  LocNodos[1][0] = -150;LocNodos[1][1] = -176;
  LocNodos[2][0] = -192;LocNodos[2][1] = -144;
  LocNodos[3][0] = -148;LocNodos[3][1] = -145;
  LocNodos[4][0] = 0;LocNodos[4][1] = -179;
  LocNodos[5][0] = 1;LocNodos[5][1] = -149;
  LocNodos[6][0] = 76;LocNodos[6][1] = -176;
  LocNodos[7][0] = 77;LocNodos[7][1] = -149;
  LocNodos[8][0] = 179;LocNodos[8][1] = -176;
  LocNodos[9][0] = 179;LocNodos[9][1] = -146;
  LocNodos[10][0] = -182;LocNodos[10][1] = -20;
  LocNodos[11][0] = -152;LocNodos[11][1] = -20;
  LocNodos[12][0] = 18;LocNodos[12][1] = -13;
  LocNodos[13][0] = 51;LocNodos[13][1] = -16;
  LocNodos[14][0] = 73;LocNodos[14][1] = 12;
  LocNodos[15][0] = 71;LocNodos[15][1] = 43;
  LocNodos[16][0] = 168;LocNodos[16][1] = 12;
  LocNodos[17][0] = 173;LocNodos[17][1] = 44;
  LocNodos[18][0] = -23;LocNodos[18][1] = 13;
  LocNodos[19][0] = -13;LocNodos[19][1] = 42;
  LocNodos[20][0] = -132;LocNodos[20][1] = 11;
  LocNodos[21][0] = -130;LocNodos[21][1] = 44;
  LocNodos[22][0] = -183;LocNodos[22][1] = 77;
  LocNodos[23][0] = -148;LocNodos[23][1] = 75;
  LocNodos[24][0] = -184;LocNodos[24][1] = 189;
  LocNodos[25][0] = -151;LocNodos[25][1] = 189;
  LocNodos[26][0] = 23;LocNodos[26][1] = 76;
  LocNodos[27][0] = 56;LocNodos[27][1] = 74;
  LocNodos[28][0] = 25;LocNodos[28][1] = 186;
  LocNodos[29][0] = 58;LocNodos[29][1] = 185;
  LocNodos[30][0] = 20;LocNodos[30][1] = -114;
  LocNodos[31][0] = 51;LocNodos[31][1] = -116;
}

void PopulateTMatrix(){


  for (int i = 0; i < NUMNODES; i++){
    for (int j = 0; j < NUMNODES; j++){
      TransitionMatrix[i][j] = 0;
    }
  }

  TransitionMatrix[0][2] = 1;
  TransitionMatrix[1][0] = 1;
  TransitionMatrix[2][3] = 1;
  TransitionMatrix[2][10] = 1;
  TransitionMatrix[3][5] = 1;
  TransitionMatrix[4][1] = 1;
  TransitionMatrix[5][7] = 1;
  TransitionMatrix[5][30] = 1;
  TransitionMatrix[6][4] = 1;
  TransitionMatrix[6][30] = 1;
  TransitionMatrix[7][9] = 1;
  TransitionMatrix[8][6] = 1;
  TransitionMatrix[9][8] = 1;
  TransitionMatrix[10][21] = 1;
  TransitionMatrix[10][22] = 1;
  TransitionMatrix[11][3] = 1;
  TransitionMatrix[12][15] = 1;
  TransitionMatrix[12][18] = 1;
  TransitionMatrix[12][26] = 1;
  TransitionMatrix[13][31] = 1;
  TransitionMatrix[14][13] = 1;
  TransitionMatrix[14][18] = 1;
  TransitionMatrix[14][26] = 1;
  TransitionMatrix[15][17] = 1;
  TransitionMatrix[16][14] = 1;
  TransitionMatrix[17][16] = 1;
  TransitionMatrix[18][20] = 1;
  TransitionMatrix[19][13] = 1;
  TransitionMatrix[19][15] = 1;
  TransitionMatrix[19][26] = 1;
  TransitionMatrix[20][11] = 1;
  TransitionMatrix[20][22] = 1;
  TransitionMatrix[21][19] = 1;
  TransitionMatrix[22][24] = 1;
  TransitionMatrix[23][11] = 1;
  TransitionMatrix[23][21] = 1;
  TransitionMatrix[24][25] = 1;
  TransitionMatrix[25][23] = 1;
  TransitionMatrix[26][28] = 1;
  TransitionMatrix[27][13] = 1;
  TransitionMatrix[27][15] = 1;
  TransitionMatrix[27][18] = 1;
  TransitionMatrix[28][29] = 1;
  TransitionMatrix[29][27] = 1;
  TransitionMatrix[30][12] = 1;
  TransitionMatrix[31][4] = 1;
  TransitionMatrix[31][7] = 1;
}


void DrawImage(){

/*
  int width, height;
  unsigned char* image =
    SOIL_load_image("0.png", &width, &height, 0, SOIL_LOAD_RGB);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
              GL_UNSIGNED_BYTE, image);
//glGenerateMipmap(GL_TEXTURE_2D);*/
}

void init(){

  for(int i = 0; i < NumberOfAgents; i++){
    Agents[i] = Cubo();
  }

  PopulateLocNodes();
  PopulateTMatrix();

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(FOVY, (GLfloat)WIDTH/HEIGTH, ZNEAR, ZFAR);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(EYE_X,EYE_Y,EYE_Z,CENTER_X,CENTER_Y,CENTER_Z,UP_X,UP_Y,UP_Z);
  glClearColor(0,0,0,0);
  glEnable(GL_DEPTH_TEST);
  srand(time(nullptr));
  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  loadTextureFromFile(fondo, 0);

}

void delay(float secs)
{
	float end = clock() / CLOCKS_PER_SEC + secs;
	while ((clock() / CLOCKS_PER_SEC) < end);
}

void TrafficLight(int LightColor){
  glPushMatrix();
  glTranslatef(-40,20,20);
  if(LightColor == 0){
    glColor3f(1.0,0.0,0.0);
  }
  if(LightColor == 1){
    glColor3f(0.0,1.0,0.0);
  }
  if(LightColor == 2){
    glColor3f(1.0,1.0,0.0);
  }
  glutSolidSphere(5, 50, 50);
  glPopMatrix();
}

int LightControl(int &LightCTRL){
  if(LightCTRL == 20){
    LightCTRL = 0;
    LightColor ++;
  }
  else{
    LightCTRL ++;
  }

  if(LightColor == 3){
    LightColor = 0;
  }
  return LightColor;
}

void display(){
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  //drawAxis();
  glColor3f(0.5, 0.5, 0.5);
  //Floor

  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texture[0]);
  glBegin(GL_QUADS);
  glTexCoord2f(0.0 ,1.0);
  glVertex3d(-DimBoard, 0.0, -DimBoard);
  glTexCoord2f(0.0 ,0.0);
  glVertex3d(-DimBoard, 0.0, DimBoard);
  glTexCoord2f(1.0 ,0.0);
  glVertex3d(DimBoard, 0.0, DimBoard);
  glTexCoord2f(1.0 ,1.0);
  glVertex3d(DimBoard, 0.0, -DimBoard);
  glEnd();
  glDisable(GL_TEXTURE_2D);

  for (int i = 0; i < NUMNODES; i++){
    std::string s = std::to_string(i);
    char const *pchar = s.c_str();
    drawString(LocNodos[i][0],10,LocNodos[i][1], pchar);
  }

  DrawImage();

  TrafficLight(LightColor);

  LightColor = LightControl(LightCTRL);


  for(int i = 0; i < NumberOfAgents; i++){
    Agents[i].draw();
    AnextNode[i] = Agents[i].update(LocNodos, TransitionMatrix, AnextNode[i], speed);
  }

  //Agents[0].draw();
  //Agents[1].draw();


  //A1nextNode = Agents[0].update(LocNodos, TransitionMatrix, A1nextNode, A2nextNode, speed);
  //A2nextNode = Agents[1].update(LocNodos, TransitionMatrix, A2nextNode, A1nextNode, speed);

  glutSwapBuffers();
    //delay(0.25);
}



void idle()
{
     display();
}

void keyboard(unsigned char key, int x, int y)
{
    switch(key)
    {//SOLID
    case 's':
    case 'S':
            glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
            glShadeModel(GL_FLAT);
      break;
    //INTERPOL
    case 'i':
    case 'I':
            glShadeModel(GL_SMOOTH);
            glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
      break;

    case 'w':
    case 'W':
            glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
      break;

    case 27:   // escape
      exit(0);
      break;
    }
    glutPostRedisplay();
}

int main(int argc, char **argv)
{

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
  glutInitWindowPosition(100, 100);
  glutInitWindowSize(WIDTH, HEIGTH);
  glutCreateWindow("ActividadM1");
  init();
  glutDisplayFunc(display);
  glutIdleFunc(idle);
  glutKeyboardFunc(keyboard);
  glutMainLoop();

  return 0;
}
