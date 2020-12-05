#include <iostream>
#include <math.h>
#include <windows.h>
#include <conio.h>
#include <iomanip>
#include <iostream>
#include <fstream>

using namespace std;

string menuName[5] = {"Просмотр базы","Регистрация поступления товара", "Оформление покупки", "Редактирование наименований",
 "Сохранение базы / Инвентаризация остатков с вычислением суммарной стоимости"};

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
    cout << setw( 20 ) << "НАИМЕНОВАНИЕ" << setw( 10 ) << "ЦЕНА"
                        << setw( 10 ) << "КОЛ-ВО"
                        << setw( 10 ) << "ЕДИНИЦЫ"<<endl;
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
    //навигация по меню
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
    cout << "Выбран пункт - "<<menuName[menuPos]<<endl<<endl;
    switch (menuPos+1)
    {
    case 1:
    {

        cout << "Просмотр базы товаров:"<<endl<<endl;
        dataList.Show();
        break;
    }

    case 2:
    {
        string findName;
        int addCount;

        cout << "Поступление товара"<<endl;
        cout << "Введите наименование поступившего товара: ";
        cin >> findName;
        if (dataList.GetItem(findName) == 0)
        {
            string units;
            float price;

            cout << "Товара с данным названием не существует в базе"<<endl;
            cout << "Создание новой позиции с названием - "<< findName <<endl;
            cout << "Введите единицы измерения товара: ";cin >> units;
            do
            {
                cout << "Введите цену нового товара: ";
                cin >> price;
                if (cin.fail())
                {
                    cin.clear();
                    cin.ignore(32767,'\n');
                    cout << "Вы ввели некорректные символы"<<endl;
                    cout << "-------ПОВТОРИТЕ ВВОД-------"<<endl;
                }
                else
                {
                    if (price > 0)
                        break;
                    else
                    {
                      cout << "Цена должна быть больше нуля"<<endl;
                      cout << "-------ПОВТОРИТЕ ВВОД-------"<<endl;
                    }
                }
                }while(true);

            dataList.Add(findName,price,units,0);
        }

        do
        {
            cout << "Введите количество поступившего товара: ";
            cin >> addCount;
            if (cin.fail())
            {
                cin.clear();
                cin.ignore(32767,'\n');
                cout << "Вы ввели некорректные символы"<<endl;
                cout << "-------ПОВТОРИТЕ ВВОД-------"<<endl;
            }
            else
            {
                if (addCount > 0)
                    break;
                else
                {
                  cout << "Количество должно быть больше 0"<<endl;
                  cout << "-------ПОВТОРИТЕ ВВОД-------"<<endl;
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
        cout << "Продажа товара"<<endl;
        cout << "Введите наименование товара для продажи: ";
        cin >> findName;
        if (dataList.GetItem(findName) == 0)
        {
            cout << "Товара с данным названием не существует в базе"<<endl;
            break;
        }

        do
        {
            cout << "Введите количество единиц для продажи: ";
            cin >> addCount;
            if (cin.fail())
            {
                cin.clear();
                cin.ignore(32767,'\n');
                cout << "Вы ввели некорректные символы"<<endl;
                cout << "-------ПОВТОРИТЕ ВВОД-------"<<endl;
            }
            else
            {
                if (addCount > 0)
                {
                    if (dataList.GetItem(findName)->record.count >= addCount)
                        break;
                    else
                        cout << "Данного товара на складе только "<<dataList.GetItem(findName)->record.count<<endl;
                }

                else
                {
                  cout << "Количество должно быть больше 0"<<endl;
                  cout << "-------ПОВТОРИТЕ ВВОД-------"<<endl;
                }
            }
        }while(true);

        dataList.GetItem(findName)->record.count-=addCount;
        price = dataList.GetItem(findName)-> record.price;

        cout << "Совершена покупка товара: "<<  findName << " ("<< addCount<<" единиц)"<<endl;
        cout << "На сумму - "<<addCount*price<<" р."<<endl;
        break;
    }

    case 4:
    {
        string findName,name, units;
        float price;
        int count,pos;
        bool cont = true;
        cout << "Редактирование базы"<<endl;
        cout << "Введите наименование товара для изменения: ";
        cin >> findName;
        if (dataList.GetItem(findName) == 0)
        {
            cout << "Товара с данным названием не существует в базе"<<endl;
            break;
        }
        while(cont)
        {
            cout << endl << "Для изменения НАЗВАНИЯ - 1 "<<endl;
            cout << "Для изменения ЕД ИЗМЕРЕНИЯ - 2 "<<endl;
            cout << "Для изменения ЦЕНЫ ЕДИНИЦЫ - 3 "<<endl;
            cout << "Для изменения КОЛИЧЕСТВА - 4 "<<endl;
            cout << "Для выхода из редактирования - 0 "<<endl<<endl;
            cout << "Введите число ->";cin >> pos;

            switch(pos)
            {
            case 1:
            {
                cout << "Введите новое название товара: ";
                cin >> name;
                dataList.GetItem(findName)->record.name=name;
                findName=name;
                break;
            }
            case 2:
            {
                cout << "Введите новые единицы измерения товара: ";
                cin >> units;
                dataList.GetItem(findName)->record.units=units;
                break;
            }
            case 3:
            {
                do
                {
                  cout << "Введите новую цену товара: ";
                  cin >> price;
                  if (cin.fail())
                  {
                      cin.clear();
                      cin.ignore(32767,'\n');
                      cout << "Вы ввели некорректные символы"<<endl;
                      cout << "-------ПОВТОРИТЕ ВВОД-------"<<endl;
                  }
                  else
                  {
                    if (price > 0)
                        break;
                    else
                    {
                      cout << "Цена должна быть больше нуля"<<endl;
                      cout << "-------ПОВТОРИТЕ ВВОД-------"<<endl;
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
                  cout << "Введите новое количество товара: ";
                  cin >> count;
                  if (cin.fail())
                  {
                      cin.clear();
                      cin.ignore(32767,'\n');
                      cout << "Вы ввели некорректные символы"<<endl;
                      cout << "-------ПОВТОРИТЕ ВВОД-------"<<endl;
                  }
                  else
                  {
                    if (count >= 0)
                        break;
                    else
                    {
                      cout << "Количество не может быть отрицательным"<<endl;
                      cout << "-------ПОВТОРИТЕ ВВОД-------"<<endl;
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
                cout << "Выбор производится в диапазоне от 1 до 4"<<endl;
                cout << "-------ПОВТОРИТЕ ВВОД-------"<<endl;
                break;
            }
            }
        }
        break;
    }

    case 5:
    {
        cout << "Сохранение данных. Вывод базы в файл. Название файла:database.txt "<<endl;
        cout << "Полная стоимость склада - "<<dataList.SaveDatabase()<<" руб.";
        break;
    }
    }
    cout <<endl<< "# Для возврата в меню нажмите любую клавишу... "<<endl;
    getch();
    menuDraw(menuPos);
}

int menuDraw(int menuPos)
{
    clearscreen();
    cout << "   АиСД - Индивидуальные задания (Шевцов Д.О)"<<endl<<endl;
    for (int i=0;i<5;i++)
    {
        if (menuPos==i)
            cout <<" > "<<menuName[i]<<endl;
        else
            cout <<"   " << menuName[i]<<endl;
        cout<<endl;
    }
}
