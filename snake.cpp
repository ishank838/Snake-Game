#include<iostream>
#include<thread>
#include <sys/ioctl.h>
#include <termios.h>
using namespace std;
bool gameover=false;
const int width=20;
const int height=20;
struct position{
	int x;
	int y;
};
int tailx[100],taily[100];
int t_len=0;
position snake_head,fruit;
enum direction { STOP=0,LEFT,RIGHT,UP,DOWN};
direction dir;
int score;
bool kbhit()
{
    termios term;
    tcgetattr(0, &term);

    termios term2 = term;
    term2.c_lflag &= ~ICANON;
    tcsetattr(0, TCSANOW, &term2);

    int byteswaiting;
    ioctl(0, FIONREAD, &byteswaiting);

    tcsetattr(0, TCSANOW, &term);

    return byteswaiting > 0;
}

void setup()
{
	dir=STOP;
	snake_head.x=width/2;
	snake_head.y=height/2;
	fruit.x=rand()%width;
	fruit.y=rand()%height;
	score=0;
	//tailx[0]=snake_head.x;
	//taily[0]=snake_head.y;
}
void draw()
{
	system("clear");
	for(int i=0;i<height;i++)
	{
		for(int j=0;j<width;j++)
		{
			if(i==0 || j==0)
			{
				cout<<"#";
			}
			else if(i==height-1 || j==width-1)
			{
				cout<<"#";
			}
			else
			{

				if(i==snake_head.y && j==snake_head.x)
				{
					cout<<"0";
				}
				else if(i==fruit.y && j==fruit.x)
				{
					cout<<"F";
				} 
				else
				{
					int print=0;
					for(int p=0;p<t_len;p++)
					{
						if(tailx[p]==j && taily[p]==i)
						{
							cout<<"o";
							print=1;
						}
					}
					if(print==0)
					{
						cout<<" ";
					}
				}
			}

		}
		cout<<endl;
	}
	cout<<"SCORE:"<<score<<endl;
}
void input()
{
	if(kbhit())
	{
		char c=getchar();
		switch(c)
		{
			case 'w':dir=UP;
					break;
			case 'a':dir=LEFT;
					break;
			case 's':dir=DOWN;
					break;
			case 'd':dir=RIGHT;
				break;
			default:break;
		}
	}
}
void logic()
{
	int prex1=tailx[0],prey1=taily[0];
	int prex2,prey2;
	tailx[0]=snake_head.x;
	taily[0]=snake_head.y;
	for(int i=1;i<t_len;i++)
	{
		prex2=tailx[i];
		prey2=taily[i];
		tailx[i]=prex1;
		taily[i]=prey1;
		prex1=prex2;
		prey1=prey2;
	}
	switch(dir)
	{
		case UP:snake_head.y--;
				break;
		case DOWN:snake_head.y++;
				break;
		case LEFT:snake_head.x--;
				break;
		case RIGHT:snake_head.x++;
				break;
		default:break;
	}
	for(int i=0;i<t_len;i++)
	{
		if(tailx[i]==snake_head.x && taily[i]==snake_head.y)
		{
			gameover=true;
			return;
		}
	}
	if(snake_head.x<=0 || snake_head.x>=width-1 || snake_head.y<=0 || snake_head.y>=height-1)
	{
		gameover=true;
	}
	if(snake_head.x==fruit.x && snake_head.y==fruit.y)
	{
		score++;
		fruit.x=rand()%width;
		fruit.y=rand()%height;
		t_len++;
	}

}
int main(void)
{
	setup();
	while(gameover!=true)
	{
		draw();
		input();
		logic();
		std::this_thread::sleep_for(100ms);
	}
}