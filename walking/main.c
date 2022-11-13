//
//  main.c
//  walking
//
//  Created by 小田竜太郎 on 2020/01/17.
//  Copyright © 2020 小田竜太郎. All rights reserved.
//

#include <stdlib.h>
#include <math.h>
#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>

void scene(void)
{
    /* 物体の色 */
    static GLfloat red[] = { 0.8, 0.2, 0.2, 1.0 };
    static GLfloat green[] = { 0.2, 0.8, 0.2, 1.0 };
    static GLfloat blue[] = { 0.2, 0.2, 0.8, 1.0 };
    static GLfloat yellow[] = { 0.8, 0.8, 0.2, 1.0 };
    static GLfloat black[] = {0.0, 0.0, 0.0, 1.0};
    static GLfloat ground[][4] = {
        { 0.6, 0.6, 0.6, 1.0 },
        { 0.3, 0.3, 0.3, 1.0 }
    };
    
    int i, j;
    
    /* 赤い箱 */
    glPushMatrix();
    glTranslated(0.0, 0.0, -3.0);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, red);
    glutSolidCube(1.0);
    glPopMatrix();
    
    /* 緑の箱 */
    glPushMatrix();
    glTranslated(0.0, 0.0, 3.0);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, green);
    glutSolidCube(1.0);
    glPopMatrix();
    
    /* 青い箱 */
    glPushMatrix();
    glTranslated(-3.0, 0.0, 0.0);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, blue);
    glutSolidCube(1.0);
    glPopMatrix();
    
    /* 黄色い箱 */
    glPushMatrix();
    glTranslated(3.0, 0.0, 0.0);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, yellow);
    glutSolidCube(1.0);
    glPopMatrix();
    
    glPushMatrix();
    glTranslated(0.0, 3.0, 0.0);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, black);
    glutSolidCube(1.0);
    glPopMatrix();
    
    /* 地面 */
    glBegin(GL_QUADS);
    glNormal3d(0.0, 1.0, 0.0);
    for (j = -5; j < 5; ++j) {
        for (i = -5; i < 5; ++i) {
            glMaterialfv(GL_FRONT, GL_DIFFUSE, ground[(i + j) & 1]);
            glVertex3d((GLdouble)i, -0.5, (GLdouble)j);
            glVertex3d((GLdouble)i, -0.5, (GLdouble)(j + 1));
            glVertex3d((GLdouble)(i + 1), -0.5, (GLdouble)(j + 1));
            glVertex3d((GLdouble)(i + 1), -0.5, (GLdouble)j);
        }
    }
    glEnd();
}

void idle(void){
    glutPostRedisplay();
}

int flugUp = 0;
int flugDown = 0;
int flugLeft = 0;
int flagRight = 0;
double dXZ = 0;
double dYZ = 0;
double angleXZ = 0;
double angleYZ = 0;

void display(void){
    
    static GLfloat lightpos[] = { 3.0, 4.0, 5.0, 1.0 }; /* 光源の位置 */
    
    static double ex = 0.0, ez = 0.0; /* 視点の位置 */
    if(flugUp) ez += 0.1;
    if(flugDown) ez -= 0.1;
    if(flugLeft) ex += 0.1;
    if(flagRight) ex -= 0.1;
    
    /* 画面クリア */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    /* モデルビュー変換行列の初期化 */
    glLoadIdentity();

    /* 視点の移動 */
    if (dXZ) {
        glRotated(dXZ, 0.0, 1.0, 0.0);
        angleXZ = dXZ;//FIXME: +=
    }
    if (dYZ != 0 || dYZ < 90 || dYZ > -90) {
       // glRotated(dYZ, 1.0, 0.0, 0.0);
       // printf("%f\n", dYZ);
        angleYZ = dYZ;//FIXME: +=
    }
    if (angleXZ != 0){
        glTranslated(ex * cos(angleXZ / 180 * M_PI) + (ez * -sin(angleXZ / 180 * M_PI)) , 0.0, ex * sin(angleXZ / 180 * M_PI) + (ez * cos(angleXZ / 180 * M_PI)));
    }else{
        glTranslated(ex, 0.0, ez);
    }
    /* 光源の位置を設定 */
    glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
    
    /* シーンの描画 */
    scene();
    
    glFlush();
}

void resize(int w, int h){
    /* ウィンドウ全体をビューポートにする */
    glViewport(0, 0, w, h);
    
    /* 透視変換行列の指定 */
    glMatrixMode(GL_PROJECTION);
    
    /* 透視変換行列の初期化 */
    glLoadIdentity();
    gluPerspective(30.0, (double)w / (double)h, 1.0, 100.0);
    
    /* モデルビュー変換行列の指定 */
    glMatrixMode(GL_MODELVIEW);
}

void motion(int x, int y) {
    static int wrap = 0;

    if(wrap == 0) {
        int ww = glutGet(GLUT_WINDOW_WIDTH);
        int wh = glutGet(GLUT_WINDOW_HEIGHT);

        int dx = x - ww / 2;
        int dy = y - wh / 2;

        // Do something with dx and dy here
        dXZ = dx;
        dYZ = dy;
        glutIdleFunc(idle);
        // move mouse pointer back to the center of the window
        wrap = 1;
        glutWarpPointer(ww / 2, wh / 2);
    } else {
        wrap = 0;
    }
}


void specialKeyUp(int key, int x, int y){
    switch(key){
//        case GLUT_KEY_UP:
//            flugUp = 0;
//            glutIdleFunc(0);
//            break;
//        case GLUT_KEY_DOWN:
//            flugDown = 0;
//            glutIdleFunc(0);
//            break;
//        case GLUT_KEY_LEFT:
//            flugLeft = 0;
//            glutIdleFunc(0);
//            break;
//        case GLUT_KEY_RIGHT:
//            flagRight = 0;
//            glutIdleFunc(0);
//            break;
        case 'w':
            flugUp = 0;
            glutIdleFunc(0);
            break;
        case 's':
            flugDown = 0;
            glutIdleFunc(0);
            break;
        case 'a':
            flugLeft = 0;
            glutIdleFunc(0);
            break;
        case 'd':
            flagRight = 0;
            glutIdleFunc(0);
            break;
//        case '\013':
//            flugUp = 0;
//            glutIdleFunc(0);
//            break;
//        case 1:
//            flugDown = 0;
//            glutIdleFunc(0);
//            break;
//        case '0':
//            flugLeft = 0;
//            glutIdleFunc(0);
//            break;
//        case '2':
//            flagRight = 0;
//            glutIdleFunc(0);
//            break;
    }
}

void jump(void){
    //
    //printf("jump!\n");
}

void specialKeyDown(int key, int x, int y){
    switch(key){

        case  'w':
            flugUp = 1;
            glutIdleFunc(idle);
            break;
        case 's':
            flugDown = 1;
            glutIdleFunc(idle);
            break;
        case 'a':
            flugLeft = 1;
            glutIdleFunc(idle);
            break;
        case 'd':
            flagRight = 1;
            glutIdleFunc(idle);
            break;
        case '\033':
            exit(0);
            break;
        case ' ':
            jump();
            break;
    }
}

void init(void){
    /* 初期設定 */
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
}

int main(int argc, char *argv[]){

    glutInit(&argc, argv);
    glutInitWindowPosition(100, 100);//winの大きさ
    glutInitWindowSize(800, 600);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);
    glutCreateWindow(argv[0]);
    glutDisplayFunc(display);
    glutReshapeFunc(resize);
    glutSpecialFunc(specialKeyDown);
    glutSpecialUpFunc(specialKeyUp);
    glutPassiveMotionFunc(motion);//マウスのキャプチャ
    glutSetCursor(GLUT_CURSOR_NONE);//マウスカーソル無効
    init();
    glutMainLoop();
    return 0;
}
