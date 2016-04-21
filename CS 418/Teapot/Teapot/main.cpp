//
//  main.cpp
//  Teapot
//
//  Created by Mustafa Shikora on 4/4/16.
//  Copyright © 2016 Shikora. All rights reserved.
//
#include <stdio.h>
#include <stdlib.h>
#include <GLUT/glut.h>
#include <ctime>
#include <math.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>

using namespace std;

//STRUCTS
struct Vertex {
    float x;
    float y;
    float z;
    float norm_x;
    float norm_y;
    float norm_z;
    float s;
    float t;
};
struct faces {
    Vertex* first;
    Vertex* second;
    Vertex* third;
};

//Global Variables
float x_avg;
float y_avg;
float z_avg;
faces face[2256];
Vertex vertex[1213];

float rotation_iterator = .01;
bool rot = 1;
bool envOrTex = 1;
bool environment = 0;
bool light1 = 1;
bool light2 = 0;
float height = 1.0;
float rotation_value = 0.0;
const int max_char = 512; //Dictates how many characters are allowed per line when reading the .obj file
const int max_tokens = 4; //Dictates maximum number of tokens we will be needing (4 because "v xx xx xx" and "f xx xx xx" is 4 terms)
const char* space = " ";
//

//function definitions
void parseFile(char const* file);
void drawObject(const char * file);
void calculateAverages();
void calculateVertexNorm();
void setTextureCoordinates();
GLuint LoadTexture( const char * filename );

void init()
{
    glClearColor(0.9411,0.91764,0.839215,0.4); //nice eggshell color
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
    
    GLuint texture;
    if(environment){
        //enables the environment photo taken from http://debevec.org/Probes/
        texture= LoadTexture("stpeters_probe.bmp");
        glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
        glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
        glEnable(GL_TEXTURE_GEN_S);
        glEnable(GL_TEXTURE_GEN_T);
    }
    else{
        glDisable(GL_TEXTURE_GEN_S);
        glDisable(GL_TEXTURE_GEN_T);
        //enables the photo I chose
        texture= LoadTexture("squares.bmp");
    }
    glBindTexture (GL_TEXTURE_2D, texture);
}
void reshape( int width, int height )
{
    glViewport(0, 0, (GLsizei)width, (GLsizei)height );
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (GLfloat)width / (GLfloat)height, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

void display(void){
    GLfloat amb_Dif[] = {1.0,1.0,1.0,1.0};
    GLfloat specular[] = {1.0,1.0,1.0, 1.0};
    GLfloat lighting[] = {100, 100, 3.0};
    GLfloat lighting2[] = {-800, 500, -3.0};
    
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
    if(light1){
        //different light positions
        glLightfv(GL_LIGHT0, GL_POSITION, lighting);
    }
    else{
        glLightfv(GL_LIGHT0, GL_POSITION, lighting2);
    }
    glLightfv(GL_LIGHT0, GL_AMBIENT, amb_Dif);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, amb_Dif);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
    
    glColor3f (1.0, 0.0, 0.0);
    glLoadIdentity ();
    gluLookAt(4.2 * sin(rotation_value), height + 2.5, -4.2 * cos(rotation_value), x_avg, y_avg, z_avg, 0.0, 1.0, 0.0);
    
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //goes through the file and populates all vertices in vertex[]
    parseFile("teapot.obj");
    //modify rotation
    if (rot)
        rotation_value += rotation_iterator;
    
    glBegin(GL_TRIANGLES);
    for (int i = 0; i < 2256; ++i)
    {
        //displaying faces
        glNormal3f(face[i].first->norm_x, face[i].first->norm_y, face[i].first->norm_z);
        glTexCoord2f(face[i].first->s, face[i].first->t);
        glVertex3f(face[i].first->x, face[i].first->y, face[i].first->z);
        glNormal3f(face[i].second->norm_x, face[i].second->norm_y, face[i].second->norm_z);
        if (envOrTex)
            glTexCoord2f(face[i].second->s, face[i].second->t);
        glVertex3f(face[i].second->x, face[i].second->y, face[i].second->z);
        glNormal3f(face[i].third->norm_x, face[i].third->norm_y, face[i].third->norm_z);
        if (envOrTex)
            glTexCoord2f(face[i].third->s, face[i].third->t);
        glVertex3f(face[i].third->x, face[i].third->y, face[i].third->z);
    }
    glEnd();
    
    glutSwapBuffers();
    glFlush ();
    
    glutPostRedisplay();
}

void drawObject(const char * file){
    ifstream file2;
    file2.open(file);
    char* token[max_tokens] = {}; //allows for storage of each "term" line by line
    char buf[max_char];
    std::string::size_type sz;
    file2.getline(buf, max_char);
    int n = 0;
    token[0] = strtok(buf, space); // start with the first token
    while(std::strcmp(token[0],"v")==0 || std::strcmp(token[0],"g")==0 || std::strcmp(token[0],"#")==0){
        //making sure random comments don't trip up the process
        file2.getline(buf,max_char);
        token[0] = strtok(buf, space); //iterating after the comments
    }
    for(int i=0; i<2256;i++){
        for (n = 1; n < max_tokens; n++)
        {
            token[n] = strtok(0, space); // populate tokens with the rest of the terms
        }
        
        //some tricky maneuvering for type matching.. found online
        string token1 = string(token[1]);
        int one = std::stoi(token1,&sz);
        string token2 = string(token[2]);
        int two = std::stoi(token2,&sz);
        string token3 = string(token[3]);
        int three = std::stoi(token3,&sz);
        
        
        face[i].first = &vertex[one-1];
        face[i].second = &vertex[two-1];
        face[i].third = &vertex[three-1];
        
        
        file2.getline(buf,max_char);
        token[0] = strtok(buf, space); //iterate to next line and initialize the next token
    }
    
}
void parseFile(char const* file){
    ifstream file2;
    file2.open(file);
    char* token[max_tokens] = {};
    char buf[max_char];
    int a = 0;
    
    std::string::size_type sz;
    while(!file2.eof()){
        file2.getline(buf, max_char);
        int n = 0;
        token[0] = strtok(buf, space);
        if(!token[0]){
            //incase there is a line with no content or a space for the first term
            break;
        }
        for (n = 1; n < max_tokens; n++)
        {
            token[n] = strtok(0, space); // subsequent token_vertexs
            if (!token[n]) break; // no more token_vertexs
        }
        if(std::strcmp(token[0],"v")==0){
            while(true)
            {
                string token_vertex1 = string(token[1]);
                vertex[a].x = std::stof(token_vertex1,&sz);
                string token_vertex2 = string(token[2]);
                vertex[a].y = std::stof(token_vertex2,&sz);
                string token_vertex3 = string(token[3]);
                vertex[a].z = std::stof(token_vertex3,&sz);
                break;
            }
            a++;
        }
    }
    calculateAverages(); //calculates x_avg, y_avg, and z_avg
    drawObject(file); //draws the actual teapot immediately
    calculateVertexNorm(); //calculates vertex normals using the face[] array
    setTextureCoordinates(); //sets texture coordinates
}
void calculateAverages(){
    float x_sum;
    float y_sum;
    float z_sum;
    
    for(int i=0; i<1213; i++){
        x_sum+=vertex[i].x;
        y_sum+=vertex[i].y;
        z_sum+=vertex[i].z;
    }
    x_avg = x_sum/1213;
    y_avg = y_sum/1213;
    z_avg = z_sum/1213;
}
void calculateVertexNorm(){
    //
    float magnitude;
    for(int i = 0; i<2256; i++){
        magnitude = sqrt(face[i].first->x*face[i].first->x + face[i].first->y*face[i].first->y + face[i].first->z*face[i].first->z);
        face[i].first->norm_x = face[i].first->x/magnitude;
        face[i].first->norm_y = face[i].first->y/magnitude;
        face[i].first->norm_z = face[i].first->z/magnitude;
        
        magnitude = sqrt(face[i].second->x*face[i].second->x + face[i].second->y*face[i].second->y + face[i].second->z*face[i].second->z);
        face[i].second->norm_x = face[i].second->x/magnitude;
        face[i].second->norm_y = face[i].second->y/magnitude;
        face[i].second->norm_z = face[i].second->z/magnitude;
        
        magnitude = sqrt(face[i].third->x*face[i].third->x + face[i].third->y*face[i].third->y + face[i].third->z*face[i].third->z);
        face[i].third->norm_x = face[i].third->x/magnitude;
        face[i].third->norm_y = face[i].third->y/magnitude;
        face[i].third->norm_z = face[i].third->z/magnitude;
    }
}
void setTextureCoordinates(){
    float theta;
    for(int i=0; i<2256; i++){
        theta = atan2(face[i].first->z,face[i].first->x);
        face[i].first->s = (theta + M_PI)/(2*M_PI);
        face[i].first->t = (face[i].first->y/3.12708);
        
        theta = atan2(face[i].second->z,face[i].second->x);
        face[i].second->s = (theta + M_PI)/(2*M_PI);
        face[i].second->t = (face[i].second->y/3.12708);
        
        theta = atan2(face[i].third->z,face[i].third->x);
        face[i].third->s = (theta + M_PI)/(2*M_PI);
        face[i].third->t = (face[i].third->y/3.12708);
    }
}
GLuint LoadTexture( const char * filename )
{
    //////////////DISCLAIMER: THIS IS NOT MY CODE. http://stackoverflow.com/questions/12518111/how-to-load-a-bmp-on-glut-to-use-it-as-a-texture
    
    GLuint texture;
    
    int width, height;
    
    unsigned char * data;
    
    FILE * file;
    
    file = fopen( filename, "rb" );
    
    if ( file == NULL ) return 0;
    if(environment){
        width = 1500;
        height = 1500;
    }
    else{
        width = 1000;
        height=1000;
    }
    data = (unsigned char *)malloc(width * height * 3);
    //int size = fseek(file,);
    fread( data, width * height * 3, 1, file );
    fclose( file );
    
    for(int i = 0; i < width * height ; ++i)
    {
        int index = i*3;
        unsigned char B,R;
        B = data[index];
        R = data[index+2];
        
        data[index] = R;
        data[index+2] = B;
        
    }
    
    
    glGenTextures( 1, &texture );
    glBindTexture( GL_TEXTURE_2D, texture );
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_MODULATE );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST );
    
    
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT );
    gluBuild2DMipmaps( GL_TEXTURE_2D, 3, width, height,GL_RGB, GL_UNSIGNED_BYTE, data );
    free( data );
    
    return texture;
}
void keyboard(unsigned char key, int x, int y)
{
    switch (key) {
        case 't':
            envOrTex = !envOrTex;
            break;
        case 'l':
            light1 = !light1;
            break;
        case '=':
            rotation_iterator+=0.01;
            break;
        case '-':
            rotation_iterator-=0.01;
            break;
        case 'z':
            environment = !environment;
            init();
            break;
    }
}
int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize (600, 600);
    glutInitWindowPosition (100, 100);
    glutCreateWindow (argv[0]);
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(display);
    glutMainLoop();
    return 0;
}