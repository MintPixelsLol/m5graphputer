/*
M5GraphPuter

status - just started
*/

#include <M5Cardputer.h>
#include "Org_01.h"
#include "Petme8x8.h"
#include "tinyexpr.h"
static const uint16_t PROGMEM image_graph_1_pixels[] = { 0x18C6, 0x18C6, 0xDFFF, 0xDFFF, 0xDFFF, 0xDFFF, 0xDBDE, 0xDBDE, 0xDBDE, 0xDBDE, 0xDBDE, 0x18C6, 0x18C6, 0x18C6, 0xDFFF, 0x5F04, 0x5F04, 0x5F04, 0x5F04, 0x7A6D, 0x5F04, 0x5F04, 0x5F04, 0x5F04, 0xDBDE, 0x18C6, 0xDFFF, 0x5F04, 0x5F04, 0xFFFF, 0x5F04, 0x5F04, 0x7A6D, 0x5F04, 0x5F04, 0x5F04, 0x9F03, 0x9F03, 0x0C63, 0xDFFF, 0x5F04, 0xFFFF, 0x5F04, 0xFFFF, 0x5F04, 0x7A6D, 0x5F04, 0x5F04, 0x5F04, 0x9F03, 0x9F03, 0x0C63, 0xDFFF, 0xFFFF, 0x5F04, 0x5F04, 0x5F04, 0xFFFF, 0x7A6D, 0x5F04, 0x5F04, 0x9F03, 0x9F03, 0x9F03, 0x0C63, 0xDFFF, 0xFFFF, 0x5F04, 0x5F04, 0x5F04, 0xFFFF, 0x7A6D, 0x5F04, 0x9F03, 0x9F03, 0x9F03, 0x9F03, 0x0C63, 0xDFFF, 0x7A6D, 0x7A6D, 0x7A6D, 0x7A6D, 0x7A6D, 0xFFFF, 0x7A6D, 0x7A6D, 0x7A6D, 0x7A6D, 0x7A6D, 0xE739, 0xDBDE, 0x5F04, 0x5F04, 0x5F04, 0x5F04, 0x5F04, 0x7A6D, 0xFFFF, 0x9F03, 0x9F03, 0x9F03, 0xFFFF, 0xE739, 0xDBDE, 0x5F04, 0x5F04, 0x5F04, 0x5F04, 0x9F03, 0x7A6D, 0xFFFF, 0x9F03, 0x9F03, 0x3D02, 0xFFFF, 0xE739, 0xDBDE, 0x5F04, 0x5F04, 0x5F04, 0x9F03, 0x9F03, 0x7A6D, 0x9F03, 0xFFFF, 0x3D02, 0xFFFF, 0x3D02, 0xE739, 0xDBDE, 0x5F04, 0x9F03, 0x9F03, 0x9F03, 0x9F03, 0x7A6D, 0x9F03, 0x3D02, 0xFFFF, 0x3D02, 0x3D02, 0xE739, 0x18C6, 0xDBDE, 0x9F03, 0x9F03, 0x9F03, 0x9F03, 0x7A6D, 0x3D02, 0x3D02, 0x3D02, 0x3D02, 0xE739, 0x18C6, 0x18C6, 0x18C6, 0x0C63, 0x0C63, 0x0C63, 0x0C63, 0x0C63, 0xE739, 0xE739, 0xE739, 0xE739, 0x18C6, 0x18C6 };


M5Canvas Canvas(&M5Cardputer.Lcd);


//functional variabled
int mode = 1;
bool graphDrawUI[2] = { true, true };  //X ruler, Y ruler
bool generalDrawUI[1] = { true };       //topbar
uint16_t graphColors[] = {0xF800};
double graphA[240];

int graphAsize = 10;
char* graphAtoeval = new char[graphAsize];
String graphAstr = "sin(x/10)*20-cos(x)"; //20*sin(x/4)

//assistive variables
int linesX = -120;
int drawX = 0;
int posX = 0;
int linesY = -65;
int drawY = 0;
int posY = 0;
int primeLineTempPosX = 0;
int primeLineTempPosY = 0;


void setup() {
  Serial.begin(115200);
  auto cfg = M5.config();
  M5Cardputer.begin(cfg);
  M5Cardputer.update();
  strcpy(graphAtoeval, graphAstr.c_str());
  Canvas.createSprite(240, 135);

  intro();
}

int lastX = 1;
int lastY = 1;
unsigned long offsTime = 0;
void loop() {
  switch (mode) {
    case 1: //graph preview
      drawGraph();
      M5Cardputer.update();
      if (lastX != linesX || lastY != linesY) {
        const int size = 240;
        double resultArray[size];
        bool isError;
        int err = evalToArray(graphA, size, 0.5, linesX, graphAtoeval, &isError);
      }
      if (M5Cardputer.Keyboard.isKeyPressed('/')) {linesX++;}
      if (M5Cardputer.Keyboard.isKeyPressed(',')) {linesX--;}
      if (M5Cardputer.Keyboard.isKeyPressed(';')) {linesY--;}
      if (M5Cardputer.Keyboard.isKeyPressed('.')) {linesY++;}
      break;
    case 2: //graph input
      break;
    default:
      mode = 0;
      break;
  }
}

int evalToArray(double *arr, int size, float increment, float start, char* toeval, bool *isErr) {
  double x;
  te_variable vars[] = {{"x", &x}};
  int err;
  int errArr[size];
  int howManyErr;
  *isErr = 0;
  te_expr *expr = te_compile(toeval, vars, 1, &err);
  // Serial.print(te_eval(expr));
  for (int i = 0; i<size; i++) {
    x=(i+start)*increment;
    arr[i] = te_eval(expr);
  }
  te_free(expr);
  return 0;
}

void drawGraph() {
  Canvas.fillSprite(0xFFFF);

  //ruler lines
  for (int iy = linesY; iy < 135 + linesY; iy++) {
    drawY = -(iy / 2);
    posY = iy - linesY;
    if (iy == 0) {
      Canvas.drawLine(0, posY, 240, posY, 0x4208);
      primeLineTempPosY = posY;
    } else if (iy % 20 == 0) {
      Canvas.drawLine(0, posY, 240, posY, 0xC5F7);
    } else if (iy % 10 == 0) {
      Canvas.drawLine(0, posY, 240, posY, 0xDEFB);
    }
  }
  for (int ix = linesX; ix < 240 + linesX; ix++) {
    drawX = ix / 2;
    posX = ix - linesX;
    if (ix == 0) {
      primeLineTempPosX = posX;
      Canvas.drawLine(posX, 0, posX, 135, 0x4208);
    } else if (ix % 20 == 0) {
      Canvas.drawLine(posX, 0, posX, 135, 0xC5F7);
    } else if (ix % 10 == 0) {
      Canvas.drawLine(posX, 0, posX, 135, 0xDEFB);
    }
  }
  Canvas.drawLine(primeLineTempPosX, 0, primeLineTempPosX, 135, 0x4208);
  Canvas.drawLine(0, primeLineTempPosY, 240, primeLineTempPosY, 0x4208);


  
  //the GRAPHS!!!!!

  for (int i = 0; i<1; i++) {
    for (int ix = linesX; ix < 240 + linesX; ix++) {
      drawX = ix / 2;
      posX = ix - linesX;
      Canvas.drawLine(posX, -graphA[posX]*2-linesY, posX-1, -graphA[posX-1]*2-linesY, graphColors[i]);
    }
  }


  Canvas.setTextColor(0x0);
  //topbar
  if (generalDrawUI[0]) {
    Canvas.fillRect(0, 0, 240, 15, 0xC618);
    Canvas.drawLine(0, 15, 239, 15, 0x8410);
    Canvas.fillRoundRect(188, 2 + 11 - 0.11 * M5Cardputer.Power.getBatteryLevel(), 7, 0.12 * M5Cardputer.Power.getBatteryLevel(), 1, 0x7E0);
    Canvas.drawRoundRect(187, 1, 9, 13, 3, 0x0);
    Canvas.setTextSize(2);
    Canvas.setFreeFont(&Org_01);
    Canvas.setCursor(199, 3);
    Canvas.print(M5Cardputer.Power.getBatteryLevel());
    Canvas.print("%");
    Canvas.pushImage(1, 1, 13, 13, image_graph_1_pixels);
    Canvas.setTextSize(1);
    Canvas.setFreeFont(&FreeMono9pt7b);
    Canvas.setCursor(17, 0);
    Canvas.print(1);
    Canvas.print(" graph");
  }
  //ruler x
  if (graphDrawUI[0]) {
    Canvas.fillRect(0, 120, 240, 15, 0xBDF7);
    Canvas.drawLine(0, 119, 238, 119, 0x8410);
    Canvas.setTextSize(1);
    Canvas.setFreeFont(&Org_01);

    for (int ix = linesX; ix < 240 + linesX; ix++) {
      drawX = ix / 2;
      posX = ix - linesX;
      if (ix == 0 || (ix % 20 == 0)) {
        Canvas.drawNumber(drawX, posX, 128);
        Canvas.drawLine(posX, 120, posX, 126, 0x0);
      } else if (ix % 10 == 0) {
        Canvas.drawLine(posX, 125, posX, 121, 0x0);
      } else if (ix % 2 == 0) {
        Canvas.drawLine(posX, 125, posX, 122, 0x0);
      }
    }
  }
  if (graphDrawUI[1]) {
    Canvas.fillRect(0, 16 * generalDrawUI[0], 18, 135 - (16 * generalDrawUI[0]) - (16 * graphDrawUI[0]), 0xD69A);
    Canvas.drawLine(18, 16 * generalDrawUI[0], 18, 135 - 17 * graphDrawUI[0], 0x9CD2);
    Canvas.fillRect(222, 16 * generalDrawUI[0], 18, 135 - (16 * generalDrawUI[0]) - (16 * graphDrawUI[0]), 0xD69A);
    Canvas.drawLine(221, 16 * generalDrawUI[0], 221, 135 - 17 * graphDrawUI[0], 0x9CD2);
    Canvas.setTextSize(1);
    Canvas.setFreeFont(&Org_01);
    //ruler y
    for (int iy = linesY + (16 * generalDrawUI[0]); iy < 135 - (16 * graphDrawUI[0]) + linesY; iy++) {
      drawY = -(iy / 2);
      posY = iy - linesY;
      if (iy == 0 || (iy % 20 == 0)) {
        Canvas.drawNumber(drawY, 1, posY - 2);
        Canvas.drawLine(223, posY, 238, posY, 0x0);
      } else if (iy % 10 == 0) {
        Canvas.drawLine(226, posY, 238, posY, 0x0);
      } else if (iy % 2 == 0) {
        Canvas.drawLine(229, posY, 238, posY, 0x0);
      }
    }
  }
  Canvas.pushSprite(0, 0);
}

void intro() {
  int linesIntro = 0;
  bool introLoop = true;
  uint16_t introColor = 0xFAA4;
  while (introLoop) {
    M5Cardputer.update();
    Canvas.fillSprite(0xFFFF);
    const int size = 240;
    double resultArray[size];
    bool isError;
    int err = evalToArray(graphA, size, 0.5, linesIntro, graphAtoeval, &isError);
    for (int iy = linesY; iy < 135 + linesY; iy++) {
      drawY = -(iy / 2);
      posY = iy - linesY;
      if (iy == 0) {
        Canvas.drawLine(0, posY, 240, posY, 0x4208);
        primeLineTempPosY = posY;
      } else if (iy % 20 == 0) {
        Canvas.drawLine(0, posY, 240, posY, 0xC5F7);
      } else if (iy % 10 == 0) {
        Canvas.drawLine(0, posY, 240, posY, 0xDEFB);
      }
    }
    for (int ix = linesIntro; ix < 240 + linesIntro; ix++) {
      drawX = ix / 2;
      posX = ix - linesIntro;
      if (ix % 20 == 0) {
        Canvas.drawLine(posX, 0, posX, 135, 0xC5F7);
      } else if (ix % 10 == 0) {
        Canvas.drawLine(posX, 0, posX, 135, 0xDEFB);
      }
    }
    Canvas.drawLine(0, primeLineTempPosY, 240, primeLineTempPosY, 0x4208);
    for (int i = 0; i<1; i++) {
      for (int ix = linesIntro; ix < 240 + linesIntro; ix++) {
        drawX = ix / 2;
        posX = ix - linesIntro;
        Canvas.drawLine(posX, -graphA[posX]*2-linesY, posX-1, -graphA[posX-1]*2-linesY, introColor);
      }
    }

    Canvas.fillRoundRect(31, 15, 178, 41, 4, 0xDEFB);
    Canvas.drawRoundRect(77, 57, 89, 17, 2, introColor);
    Canvas.fillRoundRect(79, 57, 85, 15, 1, introColor);
    Canvas.drawRoundRect(29, 13, 182, 45, 5, 0x5AEC);
    Canvas.setTextColor(0x0);
    Canvas.setTextSize(1);
    Canvas.setFreeFont(&FreeMonoBold12pt7b);
    Canvas.drawString("M5Graphputer", 37, 17);
    Canvas.setTextColor(0x39C8);
    Canvas.setFreeFont(&FreeMono9pt7b);
    Canvas.drawString("by MintPixels", 61, 39);
    Canvas.pushImage(37, 39, 13, 13, image_graph_1_pixels);
    Canvas.drawLine(56, 40, 56, 51, 0x9CD2);
    Canvas.setTextColor(0xFFFF);
    Canvas.setFreeFont();
    Canvas.drawString("dev-release", 89, 61);
    Canvas.fillRoundRect(31, 110, 178, 13, 4, 0xDEFB);
    Canvas.drawRoundRect(29, 108, 182, 17, 4, 0x5AEC);
    Canvas.setTextColor(0x0);
    Canvas.setFreeFont(&Petme8x8);
    Canvas.drawString("Press G0 to continue.", 36, 113);
    Canvas.pushSprite(0, 0);
    linesIntro++;
    introLoop = !M5Cardputer.BtnA.wasPressed();
  }
}