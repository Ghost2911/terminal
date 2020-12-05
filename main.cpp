#include <iostream>
#include <math.h>
#include <windows.h>
#include <conio.h>
#include <iomanip>

using namespace std;

string menuName[3] = {"Регистрация поступления товара", "Оформление покупки",
 "Инвентаризация остатков товара с вычислением суммарной стоимости"};


struct Node
{
    float data;
    Node *Next, *Prev;
};

class List
{
    Node *Head, *Tail;

    public:
	List():Head(NULL),Tail(NULL){};
	~List();

	void Add(float data);
	void Show();


List::~List()
{
    while(Head)
    {
        Tail=Head->Next;
        delete Head;
        Head=Tail;
    }
}

void List::Add(float data)
{
   Node *temp = new Node; .
   temp->Next = temp->Prev = NULL;
   temp->data = data;
   if (Head!=NULL)
   {
	temp->Prev = Tail;
	Tail->Next = temp;
	Tail = temp;
   }
   else
   {
	temp->Prev = NULL;
	Head=Tail=temp;
   }
}

void List::Show()
{
    Node *temp = Head;
    while(temp!=NULL)
    {
	Form1->Memo2->Lines->Add(FloatToStr(temp->data));
	temp = temp->Next;
    }
}


int menuDraw(int menuPos);
int loadTask(int menuPos);
int clearscreen();

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    int menuPos=0;
    bool cont=true;

    menuDraw(menuPos);

    while(cont) {
    int key = getch();
    switch (key)
        {
            case 72:
                if (menuPos==0)
                    menuPos=2;
                else
                    menuPos--;
                menuDraw(menuPos);
                break;
            case 80:
                if (menuPos==2)
                    menuPos=0;
                else
                    menuPos++;
                menuDraw(menuPos);
                break;
            case 13:
                clearscreen();
                loadTask(menuPos);
                break;
            case 27:
                clearscreen();
                cont = false;
                break;
        }
    }
}

int clearscreen()
{
    COORD topLeft  = {0,0};
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO screen;
    DWORD written;

    GetConsoleScreenBufferInfo(console, &screen);
    FillConsoleOutputCharacterA(
        console, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written
    );
    SetConsoleCursorPosition(console, topLeft);
}

int loadTask(int menuPos)
{
    cout << "Выбран пункт - "<<menuName[menuPos]<<endl<<endl;
    switch (menuPos+1)
    {
    case 1:
    {

        cout << "Пункт 1";
        List a;
        a.Add("Дата 1");
        a.Add("Дата 2");
        a.Add("Дата 3");
        a.Add("Дата 4");
        a.Add("Дата 5");
        a.Show();
        break;
    }

    case 2:
    {
        cout << "Пункт 2";
        break;
    }

    case 3:
    {
        cout << "Пункт 3";
        break;
    }
    }
    cout <<endl<< "# Для возврата в меню нажмите любую клавишу..."<<endl;
    getch();
    menuDraw(menuPos);
}twitch-app

int menuDraw(int menuPos)
{
    clearscreen();twitch-app
    cout << "                      Технологии програмирования (Шевцов Д.О)"<<endl<<endl;
    for (int i=0;i<3;i++)
    {twitch-app
        if (menuPos==i)twitch-app
            cout <<" > "<<menuName[i]<<endl;
        else
            cout <<"   " << menuName[i]<<endl;
        cout<<endl;
    }
}
