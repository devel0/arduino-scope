#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <fcntl.h>
#include <unistd.h>
#include <memory.h>
#include <termios.h>

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
int deltaV = 0;
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
                 (lst[i] - adcValueMid + deltaV) / adcValueMid * (heightMid - 2 * windowHeightMargin) * vFactor;
      glVertex2i(i * hFactor, y);
    }
    {
      double y = windowHeightMargin + heightMid +
                 (lst[i + 1] - adcValueMid + deltaV) / adcValueMid * (heightMid - 2 * windowHeightMargin) * vFactor;
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

const int BUFSIZE = 48;
uint8_t buf[BUFSIZE];

void thReadSerialFn()
{
  int USB = open(serialPortName.c_str(), O_RDWR | O_NOCTTY);

  {
    struct termios tty;
    memset(&tty, 0, sizeof tty);

    cfsetispeed(&tty, (speed_t)115200);

    tty.c_cflag &= ~PARENB;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;

    tty.c_cflag &= ~CRTSCTS;
    tty.c_cc[VMIN] = 1;
    tty.c_cc[VTIME] = 5;
    tty.c_cflag |= CREAD | CLOCAL;

    cfmakeraw(&tty);

    tcflush(USB, TCIFLUSH);
    tcsetattr(USB, TCSANOW, &tty);
  }

  int n = 0;

  while (true)
  {
    n = read(USB, buf, BUFSIZE);

    int i = 0;

    // seek start
    while (i < n && !(buf[i] & 0xc0))
      ++i;

    while (i < n)
    {
      if (i + 1 >= n)
        break;

      if (((buf[i] & 0xc0) != 0xc0) || ((buf[i + 1] & 0xf0) != 0))
      {
        cout << "SKIP" << endl;
        continue;
      }

      uint8_t low = buf[i] & 0x3f;
      uint8_t high = buf[i + 1] & 0xf;

      int value = (low | ((high & 3) << 6)) | (((high & 12) >> 2) << 8);

      lstMutex.lock();
      if (lst.size() > lstMaxSize)
        lst.pop_front();
      lst.push_back(value);
      lstMutex.unlock();

//      cout << "value = " << value << endl;

      i += 2;
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

void keyboardSpecial(int key, int x, int y)
{
  switch (key)
  {
  case GLUT_KEY_UP:
    deltaV += 50 / vFactor;
    break;
  case GLUT_KEY_DOWN:
    deltaV -= 50 / vFactor;
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
  glutSpecialFunc(keyboardSpecial);
  glutIdleFunc(idleFunc);
  glutReshapeWindow(800, 600);

  thReadSerial = thread(thReadSerialFn);

  glutMainLoop();

  return 0;
}