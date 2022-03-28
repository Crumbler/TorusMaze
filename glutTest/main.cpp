#include <iostream>
#include <time.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Loader.hpp"
#include "Renderer.hpp"
#include "RawModel.hpp"
#include "StaticShader.hpp"
#include "Constants.hpp"
#include "TorusLinesShader.hpp"
#include "TorusSpotShader.hpp"
#include "TorusSpotEntity.hpp"

int window, curWidth, curHeight;
int oldX = 0, oldY = 0, oldTime = 0;
bool mouseDown = false, movingPlayer = false, doubleOrbit = true;
Loader loader;
Renderer* renderer;
RawModel staticModel, torusModel, torusLinesModel, playerModel, endModel;
StaticShader shader;
TorusLinesShader torusLinesShader;
TorusSpotShader torusSpotShader;
TorusShader torusShader;
Entity *entity, *torusEntity, *torusLinesEntity;
TorusSpotEntity *playerEntity, *endEntity;
Camera camera;
Maze mainMaze;
glm::mat4 curProjMat, curViewMat, curProjMatInv, curViewMatInv;


std::vector<float> cubeColors =
{
    0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 1.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 1.0f
};

std::vector<float> cubeVertices =
{
    -0.5f, -0.5f, 0.5f,
    -0.5f, 0.5f, 0.5f,
    0.5f, 0.5f, 0.5f,
    0.5f, -0.5f, 0.5f,

    -0.5f, -0.5f, -0.5f,
    -0.5f, 0.5f, -0.5f,
    0.5f, 0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
};

std::vector<int> cubeIndices =
{
    0, 1, 2,
    0, 2, 3,
    1, 5, 6,
    1, 6, 2,
    5, 6, 7,
    5, 7, 4,
    5, 1, 0,
    5, 0, 4,
    3, 2, 6,
    3, 6, 7,
    4, 7, 3,
    4, 3, 0
};

float distToTorus(glm::vec3 p)
{
    using namespace Constants;

    p.y -= sinf(atan2f(p.x, p.z) * 4.0f + (float)oldTime * 0.01f) * 0.2;

    const glm::vec3 flatP = glm::normalize(glm::vec3(p.x, 0.0f, p.z)) * R;

    return glm::distance(p, flatP) - r;
}

glm::vec3 castToTorus(const int x, const int y, bool& hit)
{
    const glm::vec3 origin = Maths::orbitPos(camera);
    float rD = 0.0f, dist = 0.0f;
    const glm::vec3 rayDir = Maths::RayFromCam(camera, x, y, curWidth, curHeight, curProjMatInv, curViewMatInv);

    constexpr int MAX_STEPS = 30;

    int steps = 0;

    do
    {
        ++steps;
        dist = distToTorus(origin + rayDir * rD);
        rD += dist;
    } while(steps <= MAX_STEPS);

    hit = abs(dist) <= 0.01f;

    return origin + rayDir * rD;
}

void CalcProjMat(const int width, const int height)
{
    curProjMat = glm::perspective(Renderer::FOV_R, (float)width / (float)height, Renderer::NEAR_PLANE, Renderer::FAR_PLANE);
    curProjMatInv = glm::inverse(curProjMat);
}

void CalcViewMat()
{
    curViewMat = Maths::createViewMatrix(camera);
    curViewMatInv = glm::inverse(curViewMat);
}

void ResetMaze()
{
    camera.rotation = glm::vec3(0.0f);
    CalcViewMat();

    mainMaze.GenerateMaze();

    loader.MazeToAttribs(mainMaze, loader.mazeAttribs);

    torusLinesModel.updateVBO(1, loader.mazeAttribs);

    playerEntity->MoveTo(0, 0);

    movingPlayer = false;
}

void Resize(int width, int height)
{
    glViewport(0, 0, width, height);

    CalcProjMat(width, height);

    renderer->updateProjectionMatrix(shader, width, height);
    renderer->updateProjectionMatrix(torusShader, width, height);
    renderer->updateProjectionMatrix(torusLinesShader, width, height);
    renderer->updateProjectionMatrix(torusSpotShader, width, height);

    curWidth = width;
    curHeight = height;
}

void Display()
{
    renderer->Prepare();

    shader.Start();
    shader.loadViewMatrix(camera);
    renderer->RenderCube(*entity, shader);
    shader.Stop();

    torusShader.Start();
    torusShader.loadFloat(1, (float)oldTime);
    torusShader.loadViewMatrix(camera);
    renderer->RenderTorus(*torusEntity, torusShader);
    torusShader.Stop();

    torusLinesShader.Start();
    torusLinesShader.loadFloat(3, (float)oldTime);
    torusLinesShader.loadViewMatrix(camera);
    renderer->RenderTorusLines(*torusLinesEntity, torusLinesShader);
    torusLinesShader.Stop();

    torusSpotShader.Start();
    torusSpotShader.loadFloat(3, (float)oldTime);
    torusSpotShader.loadViewMatrix(camera);
    renderer->RenderTorusSpot(*playerEntity, torusSpotShader);
    renderer->RenderTorusSpot(*endEntity, torusSpotShader);
    torusSpotShader.Stop();

    glutSwapBuffers();
}



void Init()
{
    srand(time(nullptr));

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_MULTISAMPLE);
    glHint(GL_MULTISAMPLE_FILTER_HINT_NV, GL_NICEST);

    staticModel = loader.loadToVAO(cubeVertices, cubeIndices, cubeColors);
    shader.Init();

    torusModel = loader.loadTorus();
    torusShader.Init();

    mainMaze.GenerateMaze();

    torusLinesModel = loader.loadTorusLines(mainMaze);
    torusLinesShader.Init();

    playerModel = loader.loadTorusSpot(glm::vec3(0.0f, 1.0f, 0.0f));
    endModel = loader.loadTorusSpot(glm::vec3(0.0f));
    torusSpotShader.Init();

    renderer = new Renderer(shader);
    entity = new Entity(staticModel, glm::vec3(0.0f), glm::vec3(0.0f), 1.0f);
    torusEntity = new Entity(torusModel, glm::vec3(0.0f), glm::vec3(0.0f), 1.0f);
    torusLinesEntity = new Entity(torusLinesModel, glm::vec3(0.0f), glm::vec3(0.0f), 1.0f);
    playerEntity = new TorusSpotEntity(playerModel);
    endEntity = new TorusSpotEntity(endModel);

    endEntity->MoveTo(0, Constants::GRID_HEIGHT / 2);

    curViewMat = Maths::createViewMatrix(camera);
}

void CharPressed(unsigned char ch, int x, int y)
{

}

void CharReleased(unsigned char ch, int x, int y)
{
    switch (ch)
    {
    case 234:
    case 202:
    case 'R':
    case 'r':
        ResetMaze();
        break;

    case 236:
    case 204:
    case 'V':
    case 'v':
        if (doubleOrbit)
            camera.changeOrbitDistanceInstant(Constants::R);
        else
            camera.changeOrbitDistanceInstant(-Constants::R);

        doubleOrbit = !doubleOrbit;

        break;
    }
}

void SpecialKeyPressed(int key, int x, int y)
{

}

void SpecialKeyReleased(int key, int x, int y)
{

}

void MouseButton(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON)
    {
        mouseDown = state == GLUT_DOWN;
        if (mouseDown)
        {
            oldX = x;
            oldY = y;
        }
    }
    else if (button == GLUT_RIGHT_BUTTON)
    {
        movingPlayer = state == GLUT_DOWN;

        if (state == GLUT_DOWN)
        {
            bool hit;
            const glm::vec3 hitPos = castToTorus(x, y, hit);

            if (hit)
            {
                playerEntity->MoveTo(hitPos, mainMaze);

                if (playerEntity->curX == endEntity->curX &&
                    playerEntity->curY == endEntity->curY)
                    ResetMaze();
            }
        }
    }
}

void MouseMove(int x, int y)
{
    if (mouseDown)
    {
        int dx = x - oldX,
            dy = y - oldY;

        oldX = x;
        oldY = y;

        float upMult = 1.0f;

        if (!doubleOrbit && abs(camera.rotation.y) > Maths::PI / 2.0f && abs(camera.rotation.y) < Maths::PI * 1.5f)
            upMult = -1.0f;

        constexpr float multiplier = 0.005f;

        camera.changeRotation((float)dx * -multiplier * upMult, (float)dy * multiplier);

        CalcViewMat();
    }

    if (movingPlayer)
    {
        bool hit;
        const glm::vec3 hitPos = castToTorus(x, y, hit);

        if (hit)
        {
            playerEntity->MoveTo(hitPos, mainMaze);

            if (playerEntity->curX == endEntity->curX &&
                playerEntity->curY == endEntity->curY)
                ResetMaze();
        }
    }
}

void MouseWheel(int wheel, int dir, int x, int y)
{
    camera.changeOrbitDistance((float)dir * -0.25f);

    CalcViewMat();
}

void Close()
{
    shader.CleanUp();
    torusShader.CleanUp();
    torusLinesShader.CleanUp();
    torusSpotShader.CleanUp();

    loader.CleanUp();

    delete entity;
    delete torusEntity;
    delete torusLinesEntity;
    delete playerEntity;
    delete endEntity;

    delete renderer;
}

void Idle()
{
    int newTime = glutGet(GLUT_ELAPSED_TIME);
    float dt = (newTime - oldTime) / 1000.0f;

    camera.updateSmooth(dt);

    CalcViewMat();

    oldTime = newTime;

    glutPostRedisplay();
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutSetOption(GLUT_MULTISAMPLE, 4);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE | GLUT_MULTISAMPLE);
    glutInitContextVersion(3, 3);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutInitContextFlags(GLUT_FORWARD_COMPATIBLE | GLUT_DEBUG);

    glutInitWindowPosition(400, 200);
    glutInitWindowSize(1280, 720);
    window = glutCreateWindow("Init window");

    glewExperimental = true;
    if (glewInit() != GLEW_OK)
    {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return -1;
    }

    glutReshapeFunc(Resize);
    glutDisplayFunc(Display);
    glutIdleFunc(Idle);
    glutKeyboardFunc(CharPressed);
    glutKeyboardUpFunc(CharReleased);
    glutSpecialFunc(SpecialKeyPressed);
    glutSpecialUpFunc(SpecialKeyReleased);
    glutMouseFunc(MouseButton);
    glutMotionFunc(MouseMove);
    glutCloseFunc(Close);
    glutPassiveMotionFunc(MouseMove);
    glutMouseWheelFunc(MouseWheel);

    Init();

    glutMainLoop();

    return 0;
}
