#include <iostream> // Для ввода/вывода через консоль (cin, cout)
#include <fstream>  // Для работы с файлами (ifstream, ofstream)
#include <cstring>  // Для работы со строками (char[])
#include <iomanip>  // Для красивого форматирования вывода (setw, right)
#include <windows.h>


using namespace std; // чтобы не писать каждый раз перед cin и cout

const int MAX_BOOK = 1000;

struct Book
{
    char name[100];
    char author[50];
    int year;
    float grade;
};

Book books[MAX_BOOK];
int book_count = 0;

void createNewBookFile(const char *filename)
{
    ofstream fout(filename);
    if(fout)
    {
        fout.close();
        cout << "Создан новый файл: " << filename << endl;
        book_count = 0; // Сбрасываем счетчик книг, если нужно
    }
    else
    {
        cout << "Ошибка создания файла: " << filename << endl;
    }
}

// Для загрузки записей файла
void loadBook(const char *filename)
{
    ifstream fin(filename);
    if(!fin)
        return;
    
    string line;
    while(getline(fin, line))
    {
        // Пропускаем пустые строки
        if(line.empty()) continue;
        
        size_t pos1 = line.find(';');
        size_t pos2 = line.find(';', pos1 + 1);
        size_t pos3 = line.find(';', pos2 + 1);
        
        // Проверяем, что найдены все разделители
        if(pos1 != string::npos && pos2 != string::npos && pos3 != string::npos)
        {
            // Копируем строки в массивы символов
            strncpy(books[book_count].name, line.substr(0, pos1).c_str(), 99);
            strncpy(books[book_count].author, line.substr(pos1 + 1, pos2 - pos1 - 1).c_str(), 49);
            books[book_count].year = stoi(line.substr(pos2 + 1, pos3 - pos2 - 1));
            books[book_count].grade = stod(line.substr(pos3 + 1));
            
            // Гарантируем нулевое завершение строк
            books[book_count].name[99] = '\0';
            books[book_count].author[49] = '\0';
            
            book_count++;
        }
    }
    fin.close();
}

// Сохранение добавленной книги
void saveBookToFile(const char *filename, Book b)
{
    ofstream fout(filename, ios::app);

    if (!fout)
    {
        cout << "Ошибка при сохранении файла!\n";
        return;
    }
    fout << b.name << ";" << b.author << ";" << b.year << ";" << b.grade << endl;
    fout.close();
}

// Добавить книгу
void addBook()
{
    if (book_count >= MAX_BOOK)
    {
        cout << "Превышен лимит количества книг.\n";
        return;
    }
    cin.ignore();

    Book b;
    cout << "Введите названия книги(используйте _ вместо пробелов): ";
    cin.getline(b.name, 100);
    
    cout << "Введите автора: ";
    cin.getline(b.author, 50);
    cout << "Год прочтения: ";
    cin >> b.year;
    cout << "Ваша оценка (0-10): ";
    cin >> b.grade;

    books[book_count++] = b;
    saveBookToFile("book.txt", b);
    cout << "Книга добавлена!\n";
    cin.ignore();
}

// Для показа всех книг
void showAllBooks()
{
    if (book_count == 0)
    {
        cout << "Нет данных о книгах.\n";
        return;
    }

    cout << "\n--------- Все прочитанные книги ---------\n";
    cout << left << setw(70) << "Назавание произведения"
        <<  setw(40) << "Автор произведения"
        << setw(10) << "Год"
        << "Оценка(0-10)\n";
    cout << "------------------------------------------\n";

    for (int i = 0; i < book_count; i++)
    {
            cout << left << setw(70) << books[i].name
            <<  setw(40) << books[i].author
            <<  setw(10) << books[i].year
            << books[i].grade << "\n";
    }
}

void showStats()
{
    if (book_count == 0)
    {
        cout << "Нет данных для анализа.\n";
        return;
    }
    
    float total = 0;
    float average;
    float max = books[0].grade;
    char maxBook[100];
    strcpy(maxBook, books[0].name);
    for (int i = 0; i < book_count; i++)
    {
        total += books[i].grade;

        if (books[i].grade > max)
        {
            max = books[i].grade;
            strcpy(maxBook, books[i].name);
        }
    }
    average = total / book_count;
    cout << "Количество прочитанных книг: " << book_count << "\n";
    cout << "Средняя оценка: " << average << "\n";
    cout << "Самая высокая оценённая книга - " << maxBook << ": " << max << "\n";
}

void showMenu()
{
    cout << "\n=== Трекер книг ===\n";
    cout << "1. Добавить книгу\n";
    cout << "2. Показать все книги\n";
    cout << "3. Показать статистику\n";
    cout << "4. Выход\n";
    cout << "Выберите пункт: ";
}

int main()
{
    // Переключаем консоль в UTF-8
    SetConsoleOutputCP(65001); // 65001 = UTF-8
    SetConsoleCP(65001);

    char ans[100];
    cout << "Вы хотите создать новый файл: ";
    cin >> ans;
    if (strcmp(ans, "yes") == 0 || strcmp(ans, "Yes") == 0 || 
        strcmp(ans, "да") == 0 || strcmp(ans, "Да") == 0){
        createNewBookFile("book.txt");
        cout << ans;}
    else
        loadBook("book.txt");

    int choice; // переменная для хранения выбора   
    do{
        showMenu();
        cin >> choice;

        switch(choice)
        {
        case 1:
            addBook();
            break;
        case 2:
            showAllBooks();
            break;
        case 3:
            showStats();
            break;
        case 4:
            cout << "Выход из программы.\n";
            break;
        default:
            cout << "Неверный выбор.\n";
        }
    }while(choice != 4);

    return 0;
}