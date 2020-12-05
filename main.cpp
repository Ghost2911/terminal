#include <iostream>
#include <math.h>
#include <windows.h>
#include <conio.h>
#include <iomanip>
#include <iostream>
#include <fstream>

using namespace std;

string menuName[5] = {"�������� ����","����������� ����������� ������", "���������� �������", "�������������� ������������",
 "���������� ���� / �������������� �������� � ����������� ��������� ���������"};

struct Record
{
    string name,units;
    float price;
    int count;
};

struct Node
{
    Record record;
    Node *Next, *Prev;
};

class List
{
    Node *Head, *End;

    public:
	List():Head(NULL),End(NULL){};
	~List();

	void Add(string name,float price, string units, int count);
	Node* GetItem(string findName);
	void Show();
	float SaveDatabase();
};

List::~List()
{
    while(Head)
    {
        End=Head->Next;
        delete Head;
        Head=End;
    }
}

void List::Add(string name,float price, string units, int count)
{
   Node *temp = new Node;
   temp->Next = temp->Prev = NULL;
   temp->record.name = name;
   temp->record.price = price;
   temp->record.units = units;
   temp->record.count = count;
   if (Head!=NULL)
   {
	temp->Prev = End;
	End->Next = temp;
	End = temp;
   }
   else
   {
	temp->Prev = NULL;
	Head = End = temp;
   }
}

Node * List::GetItem(string findName)
{
   Node *temp = Head;
   while(temp!=NULL)
   {
      if (temp->record.name==findName)
        return temp;
      temp = temp->Next;
   }

   return 0;
}


void List::Show()
{
    Node *temp = Head;
    cout << setw( 20 ) << "������������" << setw( 10 ) << "����"
                        << setw( 10 ) << "���-��"
                        << setw( 10 ) << "�������"<<endl;
    while(temp!=NULL)
    {
        cout << setw( 20 ) << temp->record.name << setw( 10 ) << temp->record.price
                        << setw( 10 ) << temp->record.count
                        << setw( 10 ) << temp->record.units<<endl;
        temp = temp->Next;
    }
}

float List::SaveDatabase()
{
    float fullPrice=0;
    Node *temp = Head;
    ofstream out ("database.txt");
    while(temp!=NULL)
    {
        out<<temp->record.name<<";"<<temp->record.price<<";"<<temp->record.units<<";"<<temp->record.count<<endl;
        fullPrice+=temp->record.price*temp->record.count;
        temp = temp->Next;
    }
    out.close();
    return fullPrice;
}


int menuDraw(int menuPos);
int loadTask(int menuPos);
int clearscreen();
int readData();

List dataList;

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    int menuPos=0;
    bool cont=true;

    readData();
    menuDraw(menuPos);


    while(cont) {
    int key = getch();
    //��������� �� ����
    switch (key)
        {
            case 72:
                if (menuPos==0)
                    menuPos=4;
                else
                    menuPos--;
                menuDraw(menuPos);
                break;
            case 80:
                if (menuPos==4)
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

int readData()
{
    string name;
    string units;
    float price;
    int count=1;

    string line;

    ifstream in("database.txt");

    if (in.is_open())
    {
        while (getline(in, line))
        {
            int pos = 0;
            string temp;

            pos = line.find(';');
            temp = line.substr(0, pos);
            name = temp;

            line.erase(0, pos + 1);
            pos = line.find(';');
            temp = line.substr(0, pos);
            price = stof(temp);

            line.erase(0, pos + 1);
            pos = line.find(';');
            temp = line.substr(0, pos);
            units = temp;

            line.erase(0, pos + 1);
            count = stoi(line);

            dataList.Add(name,price,units,count);
        }
    }
    in.close();
}

int clearscreen()
{
    COORD startCoord  = {0,0};
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO screen;
    DWORD written;

    GetConsoleScreenBufferInfo(console, &screen);
    FillConsoleOutputCharacterA(
        console, ' ', screen.dwSize.X * screen.dwSize.Y, startCoord, &written
    );
    SetConsoleCursorPosition(console, startCoord);
}

int loadTask(int menuPos)
{
    cout << "������ ����� - "<<menuName[menuPos]<<endl<<endl;
    switch (menuPos+1)
    {
    case 1:
    {

        cout << "�������� ���� �������:"<<endl<<endl;
        dataList.Show();
        break;
    }

    case 2:
    {
        string findName;
        int addCount;

        cout << "����������� ������"<<endl;
        cout << "������� ������������ ������������ ������: ";
        cin >> findName;
        if (dataList.GetItem(findName) == 0)
        {
            string units;
            float price;

            cout << "������ � ������ ��������� �� ���������� � ����"<<endl;
            cout << "�������� ����� ������� � ��������� - "<< findName <<endl;
            cout << "������� ������� ��������� ������: ";cin >> units;
            do
            {
                cout << "������� ���� ������ ������: ";
                cin >> price;
                if (cin.fail())
                {
                    cin.clear();
                    cin.ignore(32767,'\n');
                    cout << "�� ����� ������������ �������"<<endl;
                    cout << "-------��������� ����-------"<<endl;
                }
                else
                {
                    if (price > 0)
                        break;
                    else
                    {
                      cout << "���� ������ ���� ������ ����"<<endl;
                      cout << "-------��������� ����-------"<<endl;
                    }
                }
                }while(true);

            dataList.Add(findName,price,units,0);
        }

        do
        {
            cout << "������� ���������� ������������ ������: ";
            cin >> addCount;
            if (cin.fail())
            {
                cin.clear();
                cin.ignore(32767,'\n');
                cout << "�� ����� ������������ �������"<<endl;
                cout << "-------��������� ����-------"<<endl;
            }
            else
            {
                if (addCount > 0)
                    break;
                else
                {
                  cout << "���������� ������ ���� ������ 0"<<endl;
                  cout << "-------��������� ����-------"<<endl;
                }
            }
        }while(true);
        dataList.GetItem(findName)->record.count+=addCount;
        break;
    }

    case 3:
    {
        string findName;
        int addCount;
        float price;
        cout << "������� ������"<<endl;
        cout << "������� ������������ ������ ��� �������: ";
        cin >> findName;
        if (dataList.GetItem(findName) == 0)
        {
            cout << "������ � ������ ��������� �� ���������� � ����"<<endl;
            break;
        }

        do
        {
            cout << "������� ���������� ������ ��� �������: ";
            cin >> addCount;
            if (cin.fail())
            {
                cin.clear();
                cin.ignore(32767,'\n');
                cout << "�� ����� ������������ �������"<<endl;
                cout << "-------��������� ����-------"<<endl;
            }
            else
            {
                if (addCount > 0)
                {
                    if (dataList.GetItem(findName)->record.count >= addCount)
                        break;
                    else
                        cout << "������� ������ �� ������ ������ "<<dataList.GetItem(findName)->record.count<<endl;
                }

                else
                {
                  cout << "���������� ������ ���� ������ 0"<<endl;
                  cout << "-------��������� ����-------"<<endl;
                }
            }
        }while(true);

        dataList.GetItem(findName)->record.count-=addCount;
        price = dataList.GetItem(findName)-> record.price;

        cout << "��������� ������� ������: "<<  findName << " ("<< addCount<<" ������)"<<endl;
        cout << "�� ����� - "<<addCount*price<<" �."<<endl;
        break;
    }

    case 4:
    {
        string findName,name, units;
        float price;
        int count,pos;
        bool cont = true;
        cout << "�������������� ����"<<endl;
        cout << "������� ������������ ������ ��� ���������: ";
        cin >> findName;
        if (dataList.GetItem(findName) == 0)
        {
            cout << "������ � ������ ��������� �� ���������� � ����"<<endl;
            break;
        }
        while(cont)
        {
            cout << endl << "��� ��������� �������� - 1 "<<endl;
            cout << "��� ��������� �� ��������� - 2 "<<endl;
            cout << "��� ��������� ���� ������� - 3 "<<endl;
            cout << "��� ��������� ���������� - 4 "<<endl;
            cout << "��� ������ �� �������������� - 0 "<<endl<<endl;
            cout << "������� ����� ->";cin >> pos;

            switch(pos)
            {
            case 1:
            {
                cout << "������� ����� �������� ������: ";
                cin >> name;
                dataList.GetItem(findName)->record.name=name;
                findName=name;
                break;
            }
            case 2:
            {
                cout << "������� ����� ������� ��������� ������: ";
                cin >> units;
                dataList.GetItem(findName)->record.units=units;
                break;
            }
            case 3:
            {
                do
                {
                  cout << "������� ����� ���� ������: ";
                  cin >> price;
                  if (cin.fail())
                  {
                      cin.clear();
                      cin.ignore(32767,'\n');
                      cout << "�� ����� ������������ �������"<<endl;
                      cout << "-------��������� ����-------"<<endl;
                  }
                  else
                  {
                    if (price > 0)
                        break;
                    else
                    {
                      cout << "���� ������ ���� ������ ����"<<endl;
                      cout << "-------��������� ����-------"<<endl;
                    }
                  }
                }while(true);

                dataList.GetItem(findName)->record.price=price;
                break;
            }
            case 4:
            {
                do
                {
                  cout << "������� ����� ���������� ������: ";
                  cin >> count;
                  if (cin.fail())
                  {
                      cin.clear();
                      cin.ignore(32767,'\n');
                      cout << "�� ����� ������������ �������"<<endl;
                      cout << "-------��������� ����-------"<<endl;
                  }
                  else
                  {
                    if (count >= 0)
                        break;
                    else
                    {
                      cout << "���������� �� ����� ���� �������������"<<endl;
                      cout << "-------��������� ����-------"<<endl;
                    }
                  }

                }while(true);

                dataList.GetItem(findName)->record.count=count;
                break;
            }
            case 0:
            {
                cont=false;
                break;
            }

            default:
            {
                cout << "����� ������������ � ��������� �� 1 �� 4"<<endl;
                cout << "-------��������� ����-------"<<endl;
                break;
            }
            }
        }
        break;
    }

    case 5:
    {
        cout << "���������� ������. ����� ���� � ����. �������� �����:database.txt "<<endl;
        cout << "������ ��������� ������ - "<<dataList.SaveDatabase()<<" ���.";
        break;
    }
    }
    cout <<endl<< "# ��� �������� � ���� ������� ����� �������... "<<endl;
    getch();
    menuDraw(menuPos);
}

int menuDraw(int menuPos)
{
    clearscreen();
    cout << "   ���� - �������������� ������� (������ �.�)"<<endl<<endl;
    for (int i=0;i<5;i++)
    {
        if (menuPos==i)
            cout <<" > "<<menuName[i]<<endl;
        else
            cout <<"   " << menuName[i]<<endl;
        cout<<endl;
    }
}
