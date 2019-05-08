//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

#include <string.h>
#include <vector>
#include<math.h>



void Draw();
void AddBall(double x, double y, double r);

struct Vector2
{
double x,y;
} ;


struct Ball
{
Vector2 pos;
Vector2 speed;
Vector2 acc;
double radius;
double mass;
bool m;
int id;
};


struct pair
{
Ball *first;
Ball *second;

pair(Ball *b1,Ball *b2)
{
first = b1;
second = b2;
}
};

std::vector <pair> colVec;
std::vector <Ball> allBalls;

TForm1 *Form1;

Graphics::TBitmap *buffer,*screen;
int nBalls;
Ball *selectedBall= NULL;
int button=-1;
//-1 empty, 0 - left,1 -midle,2- right

float xo,yo;


void AddBall(double x, double y, double r = 5.0)
{
Ball tmp;
Vector2 t;
t.x=0;
t.y=0;

tmp.pos.x =x;
tmp.pos.y = y;

tmp.acc = t;
tmp.speed = t;

tmp.radius = r;
tmp.mass = r * 2;
tmp.id = allBalls.size();
allBalls.push_back(tmp);
}


void Draw(){

//clear
buffer->Canvas->Brush->Style = bsSolid;
buffer->Canvas->Brush->Color = clBlack;
buffer->Canvas->FillRect(Rect(0,0,Form1->ClientWidth,Form1->ClientHeight));

//new Frame
buffer->Canvas->Pen->Width = 1;
buffer->Canvas->Pen->Color = clLime;
buffer->Canvas->Brush->Color = clAqua;
for(unsigned int i = 0;i<allBalls.size();i++)
{
if ( allBalls[i].speed.x!=0|| allBalls[i].speed.y!=0){
buffer->Canvas->Brush->Color =(TColor) random(16000000);
}
else
buffer->Canvas->Brush->Color=clAqua;


buffer->Canvas->Ellipse(allBalls[i].pos.x-(allBalls[i].radius),allBalls[i].pos.y-allBalls[i].radius,allBalls[i].pos.x+allBalls[i].radius,allBalls[i].pos.y+allBalls[i].radius );
buffer->Canvas->Brush->Style=bsClear;
//buffer->Canvas->TextOut(allBalls[i].pos.x-5,allBalls[i].pos.y - 5,FloatToStr(allBalls[i].speed.x)+":"+FloatToStr(allBalls[i].speed.x));
}

//Vector of collisions
buffer->Canvas->Pen->Color = clRed;

if (selectedBall!=NULL && button==2){
buffer->Canvas->MoveTo(selectedBall->pos.x,selectedBall->pos.y);
buffer->Canvas->LineTo(xo,yo);
}



screen->Canvas->CopyRect(Rect(0,0,Form1->ClientWidth,Form1->ClientHeight),buffer->Canvas,Rect(0,0,Form1->ClientWidth,Form1->ClientHeight)) ;
//show
Form1->Canvas->Draw(0,0,screen);
}

void BallPhys(){

for (unsigned  int i=0;i<allBalls.size();i++)
{
 allBalls[i].m = false;



 if (fabs(allBalls[i].acc.x +allBalls[i].acc.y) <0.001)
 {
  allBalls[i].acc.x=0;
  allBalls[i].acc.y=0;
 }

 allBalls[i].acc.x = -allBalls[i].speed.x * 0.05f;
 allBalls[i].acc.y = -allBalls[i].speed.y * 0.05f;

 allBalls[i].speed.x += allBalls[i].acc.x;
 allBalls[i].speed.y +=allBalls[i].acc.y;

 allBalls[i].pos.x += allBalls[i].speed.x;
 allBalls[i].pos.y +=allBalls[i].speed.y;

 if (allBalls[i].pos.x-allBalls[i].radius<0) {
 allBalls[i].pos.x -= allBalls[i].pos.x-allBalls[i].radius-1;
 allBalls[i].acc.x*=-1;
 allBalls[i].speed.x*=-1;

 }
 if (allBalls[i].pos.x+allBalls[i].radius >Form1->ClientWidth) {
 allBalls[i].pos.x -=(allBalls[i].pos.x - Form1->ClientWidth +allBalls[i].radius+1);
 allBalls[i].acc.x*=-1;
 allBalls[i].speed.x*=-1;

 }
  if (allBalls[i].pos.y-allBalls[i].radius<0) {
 allBalls[i].pos.y -= allBalls[i].pos.y - allBalls[i].radius-1;
 allBalls[i].acc.y*=-1;
 allBalls[i].speed.y*=-1;

 }

 if (allBalls[i].pos.y+allBalls[i].radius>Form1->ClientHeight) {
 allBalls[i].pos.y -=(allBalls[i].pos.y - Form1->ClientHeight)+allBalls[i].radius+1;
 allBalls[i].acc.y*=-1;
 allBalls[i].speed.y*=-1;

 }




 if (fabs(allBalls[i].speed.x +allBalls[i].speed.y) <0.001)
 {
  allBalls[i].speed.x=0;
  allBalls[i].speed.y=0;
 }

}

}

void Collision(){

colVec.clear();
BallPhys();

for (unsigned  int i=0;i<allBalls.size()-1;i++)
{
 for (unsigned  int j=i+1;j<allBalls.size();j++)
 {

   {
   double x1 =(allBalls[i].pos.x);
   double x2 =(allBalls[j].pos.x);
   double y1 =(allBalls[i].pos.y);
   double y2 =(allBalls[j].pos.y);
   double r1 =(allBalls[i].radius);
   double r2 =(allBalls[j].radius);


    double distance = abs((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2)) + 0.00001;
    bool d = distance <=((r1+r2)*(r1+r2));
    if (d)
    {


       colVec.push_back(pair(&allBalls[i],&allBalls[j]));


     distance = sqrt(distance);
     double overlap = 0.5 *(distance - r1 - r2);

     //move i ball
     allBalls[i].pos.x-=overlap * (x1-x2)/distance;
     allBalls[i].pos.y-=overlap * (y1-y2)/distance;
     //move j ball
     allBalls[j].pos.x+=overlap * (x1-x2)/distance;
     allBalls[j].pos.y+=overlap * (y1-y2)/distance;


    }
   }
 }
}

for (unsigned  int i =0; i<colVec.size();i++){
Ball *t1 = colVec[i].first;
Ball *t2 = colVec[i].second;


double distance = sqrt((t1->pos.x-t2->pos.x)*(t1->pos.x-t2->pos.x) + (t1->pos.y-t2->pos.y)*(t1->pos.y-t2->pos.y)) + 0.00001;


double nx = (t1->pos.x - t2->pos.x)/distance;
double ny = (t1->pos.y - t2->pos.y)/distance;

double tx = -ny;
double ty = nx;


double dpTan1 = t1->speed.x * tx+ t1->speed.y * ty;
double dpTan2 = t2->speed.x *tx +t2->speed.y * ty;


double dpNorm1 = t1->speed.x * nx+ t1->speed.y * ny;
double dpNorm2 = t2->speed.x *nx +t2->speed.y * ny;


double m1 = (dpNorm1 * (t1->mass - t2->mass) +2.0 * t2->mass *dpNorm2)/(t1->mass +t2->mass);
double m2 = (dpNorm2 * (t2->mass - t1->mass) +2.0 * t1->mass *dpNorm1)/(t1->mass +t2->mass);



t1->speed.x = tx*dpTan1+  nx * m1;
t1->speed.y = ty*dpTan1+  ny * m1;
t2->speed.x = tx*dpTan2+  nx * m2;
t2->speed.y = ty*dpTan2+  ny * m2;
}
}
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormCreate(TObject *Sender)
{

randomize();
screen = new Graphics::TBitmap();
buffer = new Graphics::TBitmap();

screen->Width = ClientWidth;
screen->Height = ClientHeight;

buffer->Width = ClientWidth;
buffer->Height = ClientHeight;


double defaultRad = 2.0;
int nr = 1000;
for (int i =0;i<nr;i++)
AddBall(random(ClientWidth),random(ClientHeight),defaultRad + (random(30)/5)) ;

}
//---------------------------------------------------------------------------







void __fastcall TForm1::FormMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
		  int X, int Y)
{

selectedBall = NULL;
 for(unsigned int i =0;i<allBalls.size();i++)
 {
   double x1 =(allBalls[i].pos.x);
   double x2 =(X);
   double y1 =(allBalls[i].pos.y);
   double y2 =(Y);
   double r1 =(allBalls[i].radius);
   double r2 =(0);
    double distance = abs((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));
    bool d = distance <=((r1+r2)*(r1+r2));
    if (d){
     selectedBall = &allBalls[i];
     if (Button==mbLeft) button=0;
     else
     button=2;
     break;
    }
 }



		}

//---------------------------------------------------------------------------
void __fastcall TForm1::FormMouseMove(TObject *Sender, TShiftState Shift, int X, int Y)

{
xo = X;
yo = Y;
if(selectedBall!=NULL && button == 0){
selectedBall->pos.x = X;
selectedBall->pos.y = Y;
}
}

//---------------------------------------------------------------------------
void __fastcall TForm1::FormMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
		  int X, int Y)
{

if (Button == mbLeft)
{
selectedBall=NULL;
}
else
{
if (selectedBall!=NULL)
{
selectedBall->speed.x = (selectedBall->pos.x- X)/10;
selectedBall->speed.y = (selectedBall->pos.y- Y)/10;
}
selectedBall=NULL;
}
}

void __fastcall TForm1::Timer1Timer(TObject *Sender)
{

Collision();
Draw();
}
//---------------------------------------------------------------------------



void __fastcall TForm1::FormResize(TObject *Sender)
{
screen->Width = ClientWidth;
screen->Height = ClientHeight;

buffer->Width = ClientWidth;
buffer->Height = ClientHeight;
}
//---------------------------------------------------------------------------

