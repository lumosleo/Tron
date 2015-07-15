/*-------------------------------------------------------------
Do Whatever the Hell You Want With It License
---------------------------------------------------------------
Created By Abhikalp Unakal

This is a fun little implementation of the arcade version of Tron
Feel Free to Use this program as needed
All I Ask is To give due credit and link back to this url

https://github.com/lumosleo/Tron

Cheers!
 
libraries used are 
SDL	      ---> initialising the audio stream 
SDL_mixer     ---> for audio playback   
freeglut3-dev ---> install this to get all the OpenGL libraries
Standard UNIX library functions
------------------------------------------------------------*/
#include"iostream"
#include"GL/glut.h"
#include"unistd.h"
#include"vector"
#include"string.h"
#include"SDL/SDL.h"
#include"SDL/SDL_mixer.h"
#include"sys/types.h"
#include"signal.h"


#define SIZE 100
#define LEFT  0
#define RIGHT 1
#define UP    2
#define DOWN  3
#define ESCAPE 27
#define BLUE 1
#define ORANGE 2


using namespace std;
int width=700, height =700; //window specs
GLint deviceheight,devicewidth;
GLfloat clipAreaXLeft,clipAreaXRight,clipAreaYTop,clipAreaYBottom;

int p1dir_state;  //direction state for player1
int p2dir_state;  //direction state for player2
int winner;
int loser;
int gameover = 0;
pid_t pid;
long child_id;   // for control of the background process that plays the sound
vector < vector <int> > gameboard; //initialize gameboard to 0. 0 = empty spaces ; 1 = player1 path ; 2 = player2 path;

int dx =10,dy =10; //size of light cycles
int i,j;


//function to display raster text
void displaytext(int x, int y,char const* s)
{
        int i,len;
        glRasterPos2d(x,y);
        len=strlen(s);
        for(i=0;i<len;i++)
        {
		glPushMatrix();
                glColor3f(0.0,1.0,1.0);
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,s[i]);
		glFlush();
		glPopMatrix();
        }
}


// the player class

class player{ 

public:
    int id;
    int posx;
    int posy;
    int xmin , ymin;
    int xmax , ymax;
    int score;
    float red , green , blue;
    int dir_state;
    int valid[4];

    player(int id_no,int x, int y ,float r , float g , float b){

        posx = x;  // logical x and y positions on game board that's mapped to opengl coordinates
        posy = y;
        xmin = 0;
        ymin = 0;
        xmax = xmin + dx;
        ymax = ymin + dy;
        red = r;
        green = g;
        blue= b;
        dir_state = -1;
        id = id_no;
        //initialize valid states to -1 at start of game;
        valid[LEFT] = -1;
        valid[RIGHT] = -1;
        valid[UP] = -1;
        valid[DOWN] = -1;
    }

//function to exit game when a player collides with itself opponent or the game grid
    void game_exit(){
	if(id == BLUE){
		gameover = 1;
		glColor3f(0,1,1);
		displaytext(760,70,"GAME OVER");
		displaytext(760,50,"WINNER  -  ORANGE");
		displaytext(760,30,"LOSER   -   BLUE");
					
	}
       if(id == ORANGE){
		gameover = 1;
		glColor3f(0,1,1);
		displaytext(760,70,"GAME OVER");
		displaytext(760,50,"WINNER  -  BLUE");
		displaytext(760,30,"LOSER   -   ORANGE");
	}

    }

//mapping the logical coordinates to that of OpenGL coordinates (a hugely irritating obstacle)
    void update(){
        xmin = posx*dx;
        ymin = height - 10 - (posy*dy);
        xmax = xmin + dx;
        ymax = ymin + dy;
        square(xmin,ymin,xmax,ymax,red,green,blue);
        gameboard[posy][posx] = id;  
    }

//optional function to draw a bounding box along player object 
    void border(int xmin,int ymin,int xmax,int ymax,float red, float green,float blue){
    glColor3f(red,green,blue);
    glBegin(GL_LINE_LOOP);
        glVertex2d(xmin,ymin);
        glVertex2d(xmax,ymin);
        glVertex2d(xmax,ymax);
        glVertex2d(xmin,ymax);
    glEnd();
    glFlush();
   }


void square(int xmin , int ymin,int xmax,int ymax,float red, float green,float blue){
    glColor3f(red,green,blue);
    glBegin(GL_POLYGON);
        glVertex2d(xmin,ymin);
        glVertex2d(xmax,ymin);
        glVertex2d(xmax,ymax);
        glVertex2d(xmin,ymax);
    glEnd();
//uncomment next line to draw the bounding boxes
// border(xmin,ymin,xmax,ymax,1,1,1);
    glFlush();
}

};

// start values for each run 
int orangex = 10;
int orangey = 30;
int bluex =  50;
int bluey = 40;

player orange(ORANGE,orangex,orangey,1,0.7,0);
player blue(BLUE,bluex,bluey,0,1,1);




void arrows(int key,int x,int y){

    if(!gameover){

        if(key == GLUT_KEY_RIGHT  && (blue.valid[RIGHT] == -1 || blue.valid[RIGHT] == 1) ){
                blue.posx++;
                blue.update();
                blue.dir_state = RIGHT;
                blue.valid[LEFT] = 0;
                blue.valid[RIGHT] = 0;
                blue.valid[UP] = 1;
                blue.valid[DOWN] = 1;

        }

        if(key == GLUT_KEY_DOWN  && (blue.valid[DOWN] == -1 || blue.valid[DOWN] == 1)){
                blue.posy++;
                blue.update();
                blue.dir_state = DOWN;
                blue.valid[LEFT] = 1;
                blue.valid[RIGHT] = 1;
                blue.valid[UP] = 0;
                blue.valid[DOWN] = 0;
        }

        if(key == GLUT_KEY_LEFT  && (blue.valid[LEFT] == -1 || blue.valid[LEFT] == 1) ){
                blue.posx--;
                blue.update();
                blue.dir_state = LEFT;
                blue.valid[LEFT] = 0;
                blue.valid[RIGHT] = 0;
                blue.valid[UP] = 1;
                blue.valid[DOWN] = 1;
        }

        if(key == GLUT_KEY_UP  && (blue.valid[UP] == -1 || blue.valid[UP] == 1) ){
                blue.posy--;
                blue.update();
                blue.dir_state = UP;
                blue.valid[LEFT] = 1;
                blue.valid[RIGHT] = 1;
                blue.valid[UP] = 0;
                blue.valid[DOWN] = 0;
        }

    }

}



//controls for the orange player 
void keyboard(unsigned char key,int x,int y){

    if(!gameover){


        if(key == 'd' && (orange.valid[RIGHT] == -1 || orange.valid[RIGHT] == 1) ){

                orange.posx++;
                orange.update();
                orange.dir_state = RIGHT;
                orange.valid[LEFT] = 0;
                orange.valid[RIGHT] = 0;
                orange.valid[UP] = 1;
                orange.valid[DOWN] = 1;

        }

        if(key == 's'  && (orange.valid[DOWN] == -1 || orange.valid[DOWN] == 1)){
                orange.posy++;
                orange.update();
                orange.dir_state = DOWN;
                orange.valid[LEFT] = 1;
                orange.valid[RIGHT] = 1;
                orange.valid[UP] = 0;
                orange.valid[DOWN] = 0;
        }

        if(key == 'a'  && (orange.valid[LEFT] == -1 || orange.valid[LEFT] == 1) ){
                orange.posx--;
                orange.update();
                orange.dir_state = LEFT;
                orange.valid[LEFT] = 0;
                orange.valid[RIGHT] = 0;
                orange.valid[UP] = 1;
                orange.valid[DOWN] = 1;
        }

        if(key == 'w'  && (orange.valid[UP] == -1 || orange.valid[UP] == 1) ){
                orange.posy--;
                orange.update();
                orange.dir_state = UP;
                orange.valid[LEFT] = 1;
                orange.valid[RIGHT] = 1;
                orange.valid[UP] = 0;
                orange.valid[DOWN] = 0;
        }

    }

    if(key == ESCAPE){
    	   kill(child_id,SIGKILL);
	   exit(0);

    }

}


//function to display the curent status of the game board 
void printBoard()
{
	cout << "Board" << endl;
	for (int a = 0; a < 70; a++)
	{
		for (int b = 0; b < 70; b++)
			cout << gameboard[a][b] << " ";
		cout << endl;
	}
	cout << endl;
}

void idle(){

// prints the board in ascii format uncomment it if you need it for debugging purposes
//printBoard();

if(!gameover){

    //Move Right
    if(blue.dir_state == RIGHT  ){


    if(gameboard[blue.posy][blue.posx+1] == -1 )
            blue.game_exit();

    else if(gameboard[blue.posy][blue.posx+1] == ORANGE || gameboard[blue.posy][blue.posx+1] == BLUE){
            blue.game_exit();
    }

    else {
            usleep(50000);
            blue.posx++;
            blue.update();
    }

    }

    //Move Left
    if(blue.dir_state == LEFT  ){

    if(gameboard[blue.posy][blue.posx-1] == -1)
           	blue.game_exit();

    else if(gameboard[blue.posy][blue.posx-1] == ORANGE || gameboard[blue.posy][blue.posx-1] == BLUE){
            blue.game_exit();
    }

    else{
            usleep(50000);
            blue.posx--;
            blue.update();
    }

    }

    //Move Up
    if(blue.dir_state == UP  ){

        if(gameboard[blue.posy-1][blue.posx] == -1)
               blue.game_exit();


        else if(gameboard[blue.posy-1][blue.posx] == ORANGE || gameboard[blue.posy-1][blue.posx] == BLUE){
                blue.game_exit();
        }

        else{
                usleep(50000);
                blue.posy--;
                blue.update();
        }

    }

    //Move Down
    if(blue.dir_state == DOWN  ){

        if(gameboard[blue.posy+1][blue.posx] == -1)
                blue.game_exit();


        else if(gameboard[blue.posy+1][blue.posx] == ORANGE || gameboard[blue.posy+1][blue.posx] == BLUE){
                blue.game_exit();
        }

        else{
                usleep(50000);
                blue.posy++;
                blue.update();
        }

    }

    //Move Right
    if(orange.dir_state == RIGHT){


        if(gameboard[orange.posy][orange.posx+1] == -1)
                orange.game_exit();


        else if(gameboard[orange.posy][orange.posx+1] == BLUE || gameboard[orange.posy][orange.posx+1] == ORANGE){
                orange.game_exit();
        }

        else{
                usleep(50000);
                orange.posx++;
                orange.update();
        }

    }

    //Move Left 
    if(orange.dir_state == LEFT){

        if(gameboard[orange.posy][orange.posx-1] == -1)
                orange.game_exit();


        else if(gameboard[orange.posy][orange.posx-1] == BLUE || gameboard[orange.posy][orange.posx-1] == ORANGE){
                orange.game_exit();
        }


        else{
                usleep(50000);
                orange.posx--;
                orange.update();
        }

    }

    //Move Up
    if(orange.dir_state == UP){

        if(gameboard[orange.posy-1][orange.posx] == -1)
                orange.game_exit();

        else if(gameboard[orange.posy-1][orange.posx] == BLUE || gameboard[orange.posy-1][orange.posx] == ORANGE){
                orange.game_exit();
        }



        else{
                usleep(50000);
                orange.posy--;
                orange.update();
        }

    }

    //Move Down
    if(orange.dir_state == DOWN){

        if(gameboard[orange.posy+1][orange.posx] == -1)
                orange.game_exit();


        else if(gameboard[orange.posy+1][orange.posx] == BLUE || gameboard[orange.posy+1][orange.posx] == ORANGE){
               orange.game_exit();
        }


        else{ 
                usleep(50000);
                orange.posy++;
                orange.update();
        }

    }

}


}


//Reshape function to change the clipping coordinates when we enter into the fullscreen mode 
void reshape(GLsizei width,GLsizei height){


if(height == 0) height = 1; // to prevent divide by 0
float aspect  = (float)width/(float)height;

::deviceheight = height;
::devicewidth = width;

//set the viewport to cover the new window
glViewport(0,0,width,height);
glMatrixMode(GL_PROJECTION); // to operate on the projection matrix
glLoadIdentity();   //reset the projection matrix
if(width >= height){
    clipAreaXLeft = -1.0*aspect;
    clipAreaXRight = 1.0*aspect;
    clipAreaYBottom = -1.0;
    clipAreaYTop = 1.0;

}

else{

        clipAreaXLeft = -1.0;
        clipAreaXRight = 1.0;
        clipAreaYBottom = -1.0/aspect;
        clipAreaYTop = 1.0/aspect;

}
gluOrtho2D(clipAreaXLeft,clipAreaXRight,clipAreaYBottom,clipAreaYTop);
glMatrixMode(GL_MODELVIEW);
}



//draw the game grid 
void draw_mesh(){

    //Drawing the boundary lines
    glColor3f(0.5,0.5,0.5);
    glBegin(GL_LINE_LOOP);
    glVertex2d(1,1);
    glVertex2d(699,1);
    glVertex2d(699,699);
    glVertex2d(1,699);
    glEnd();

glPushMatrix();

//Drawing the grid lines
for(int k = 1;k<700;k+=100){
		glBegin(GL_LINES);
		glVertex2d(1,k);
		glVertex2d(699,k);
		glEnd();
		glFlush();
}

for(int l=1;l<700;l+=100){
		glBegin(GL_LINES);
		glVertex2d(l,1);
		glVertex2d(l,699);
		glEnd();
		glFlush();
}
glPopMatrix();

}


void display(){
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0,0,0,1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluOrtho2D(0,width,0,height);
    glPushMatrix();
    glPointSize(2.0);
    glColor3f(0,1,1);
    //using negative co-ordinates casue the ortho values get changed when you use glutFulScreen API    
    displaytext(-260,70,"PRESS ESC TO EXIT GAME"); 
    draw_mesh();
    glPopMatrix();
    blue.update();
    orange.update();
        glFlush();
}




int main(int argc , char **argv){

    //creating a new fork to child process here
    //to enable the audio process to run in the background
    pid = fork();
    if(pid<0)
	perror("Fork");

    if(pid == 0){

        child_id = getpid();

        if (SDL_Init(SDL_INIT_AUDIO) != 0){
            std::cerr << "SDL_Init ERROR: " << SDL_GetError() << std::endl;
            return -1;
        }
        // Open Audio device with bit rate of audio
        if (Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 2048) != 0){
            std::cerr << "Mix_OpenAudio ERROR: " << Mix_GetError() << std::endl;
            return -1;
        }
        // Set Volume
        Mix_VolumeMusic(100);

        // Open Audio File
        //Audio file is passed as command line argument
        Mix_Music* music = Mix_LoadMUS(argv[1]);
        if (music){
            // Start Playback using fadein music that loops indefinitely
                if (Mix_FadeInMusic(music, -1,2000) == 0){

                        unsigned int startTime = SDL_GetTicks();
                        // Wait function to determine the time elapsed for the music track does nothing important
                        while (Mix_PlayingMusic()){
                               SDL_Delay(1000);
                                //std::cout << "Time: " << (SDL_GetTicks() - startTime) / 1000 << std::endl;
                        }
                }
        //an error occurs when the MIX_FadeInMusic API returns -1
        else{
                std::cerr << "Mix_PlayMusic ERROR: " << Mix_GetError() << std::endl;
        }

        // Free File
        Mix_FreeMusic(music);
        music = 0;
        }
        else{       
                std::cerr << "Mix_LoadMuS ERROR: " << Mix_GetError() << std::endl;
        }
    
         // End close the audio channel
      
         Mix_CloseAudio();

    }

if(pid > 0){

//setting game board boundary conditions    
//need to use the push_back method for the first time initialization of the memory for the 2D vector gameboard
for(i=0;i<70;i++)
{
	vector <int> temp;
	for (j=0;j<70;j++)
                {
                
			if(i == 0 || i == 69 || j==0 || j==69)
                        	temp.push_back(-1);
			else
				temp.push_back(0);
                }
                gameboard.push_back(temp);
}


    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
    glutInitWindowPosition(0,0);
    glutInitWindowSize(700,700);
    glutCreateWindow("TRON");
    glutFullScreen();
    //call the display mouse and keyboard functions only when the gameover flag is set to 0
    if(!gameover){
    glutDisplayFunc(display);
    glutSpecialFunc(arrows);
    glutIdleFunc(idle);
 }
    glutKeyboardFunc(keyboard);
   glutReshapeFunc(reshape);
    glutMainLoop();
    return 0;
}


}

