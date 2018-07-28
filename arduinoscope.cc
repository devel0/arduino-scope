#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <unistd.h>

#include <fstream>
#include <chrono>
#include <list>
#include <deque>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <algorithm>

using namespace std;

thread thReadSerial;
auto start = std::chrono::system_clock::now();
deque<unsigned long> lst;
const int lstMaxSize = 2048;
mutex lstMutex;
bool started = false;
const int ADCVALUEMAX = 1023;
const int windowHeightMargin = 20;
double vFactor = 1;
double hFactor = 1;
string serialPortName;

void reshape(int w, int h)
{
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, w, 0, h, -1, 1);
  glScalef(1, 1, 1);
  glTranslatef(0, 0, 0);
}

void display()
{
  if (!started)
    started = true;

  glClear(GL_COLOR_BUFFER_BIT);
  glPointSize(1);
  glColor3f(1, 1, 1);

  glBegin(GL_LINES);
  auto width = glutGet(GLUT_WINDOW_WIDTH);
  auto height = glutGet(GLUT_WINDOW_HEIGHT);
  auto heightMid = height / 2;

  auto adcValueMid = ADCVALUEMAX / 2.0;

  lstMutex.lock();
  int w = min((int)lst.size(), width);
  for (int i = 0; i < w - 1; ++i)
  {
    {
      double y = windowHeightMargin + heightMid +
                 (lst[i] - adcValueMid) / adcValueMid * (heightMid - 2 * windowHeightMargin) * vFactor;
      glVertex2i(i * hFactor, y);
    }
    {
      double y = windowHeightMargin + heightMid +
                 (lst[i + 1] - adcValueMid) / adcValueMid * (heightMid - 2 * windowHeightMargin) * vFactor;
      glVertex2i(i * hFactor + 1, y);
    }
  }
  lstMutex.unlock();

  glEnd();
  glFlush();
}

void idleFunc()
{
  auto cur = chrono::system_clock::now();
  auto diff = chrono::duration_cast<chrono::milliseconds>(cur - start);
  if (diff.count() > 20) // 50Hz
  {
    if (started)
    {
      glutPostRedisplay();
    }

    start = cur;
  }
}

void thReadSerialFn()
{
  ifstream infile(serialPortName);
  string line;

  while (getline(infile, line))
  {
    try
    {
      auto val = std::stoul(line.c_str(), nullptr, 16);

      lstMutex.lock();
      if (lst.size() > lstMaxSize)
        lst.pop_front();
      lst.push_back(val);
      lstMutex.unlock();
    }
    catch (...)
    {
    }
  }
}

void keyboard(unsigned char c, int x, int y)
{
  switch (c)
  {
  case ' ':
    started = !started;
    break;
  case 'v':
    vFactor /= 1.2;
    cout << "vFactor = " << vFactor << endl;
    break;
  case 'V':
    vFactor *= 1.2;
    cout << "vFactor = " << vFactor << endl;
    break;
  case 'h':
    hFactor /= 1.2;
    cout << "hFactor = " << hFactor << endl;
    break;
  case 'H':
    hFactor *= 1.2;
    cout << "hFactor = " << hFactor << endl;
    break;
  }
}

int main(int argc, char **argv)
{
  if (argc != 2)
  {
    cout << "Syntax: " << argv[0] << " <serial-port>" << endl;

    return 1;
  }

  serialPortName = argv[1];

  glutInit(&argc, argv);
  glutCreateWindow("arduinoscope");
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);
  glutIdleFunc(idleFunc);
  glutReshapeWindow(800, 600);

  thReadSerial = thread(thReadSerialFn);

  glutMainLoop();

  return 0;
}