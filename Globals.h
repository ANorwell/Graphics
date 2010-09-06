#pragma once
class Globals {
    static int pWidth, pHeight;
    static double pFovy;
public:
    static double PI;
    static int getWidth() { return pWidth; }
    static int getHeight() {return pHeight; }
    static double getFovy() { return pFovy; }
    static double getFovx() { return pWidth*pFovy/pHeight; }

    static void setWidth(int aWidth) { pWidth = aWidth; }
    static void setHeight(int aHeight) { pHeight = aHeight; }
    static void setFovy(double aFovy) { pFovy = aFovy; }
};