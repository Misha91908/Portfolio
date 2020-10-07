#ifndef CIS_PROJECT_MAINWINDOW_H
#define CIS_PROJECT_MAINWINDOW_H

#include <QtWidgets>
#include <QtSql>
#include "search.h"

class mainwindow : public QMainWindow
{

    /* Краткое описание всех атрибутов и методов класса:
     *
     *  Методы:
     *      - mainwindow() - конструктор класса, инициализирует виджет;
     *      - ~mainwindow() - деструктор класса, уничтожает виджет;
     *
     *      - init_ui() - метод, инициализирующий графический интерфейс виджета;
     *      - connect_to_db() - метод, отвечающий за подключение к базе данных, в случае
     *      отсутствия или присутствия соединения возвращает false или true соответственно;
     *
     *      - load_data() - слот виджета addCompanyButton, выгружает данные из csv файлов в базу данных;
     *      - remove_selected_data() - слот виджета deleteCompanyButton, удаляет ячейку, выделенную в
     *      tableWidget, не стирая данные о ней в базе данных;
     *      - show_search() - слот виджета searchButton, открывает виджет search;
     *      - remove_all_data() - слот виджета deleteAllCompaniesButton, удаляет все данные как с tableWidget,
     *      так и с базы данных;
     *      - get_all_data() - слот виджета showAllListButton, выводит все данные, хранящиеся в базе данных;
     *      - clear_list() - слот виджета clearListButton, очищает все данные с tableWidget, не удаляя их из
     *      базы данных;
     *      - search_data() - слот виджета search->searchButton, выполняет поиск данных согласно заданным критериям,
     *      в случае использования весовых коэффициентов, сортирует данные в порядке убывания,
     *      согласно составленному рейтингу;
     *      - show_selected_provider_data() - слот виджета tableWidget, выводит в отдельном окне данные
     *      о выделенном поставщике
     *
     *      - is_number() - метод, проверяющий, являектся ли введенная строка числом, если да, то
     *      возвращает true;
     *      - comparator() - метод, используемый функцией qSort() для сортировки записей QSqlRecord:
     *      - int_comparator() - метод, используемый функцией qSort() для сортировки целых чисел.
     *
     *  Атрибуты:
     *      - search - объект класса searchWidget, выполняет роль окна поиска для приложения;
     *
     *      - is_db_connect - логическая переменная, хранящее текущее состояние подключения к базе данных;
     *      - db - объект класса QSqlDatabase, отвечает за взаимодействие с базой данных;
     *
     *      - tableWidget - объект класса QTableWidget, отвечает за визуализацию данных в виде таблицы;
     *      - tableHeader - объект класса QStringList, хранит названия столбцов таблицы;
     *
     *      - contentWidget - объект класса QWidget, используется для установки контейнера с остальными виждетами;
     *      - contentLayout - объект класса QHBoxLayout, содержит в себе все виджеты;
     *      - buttonsLayout - объект класса QVBoxLayout, содержит в себе все кнопочные виджеты;
     *      - listOfCompaniesWidget - объект класса QFrame, содержит в себе контейнер с виджетом tableWidget;
     *
     *      - addCompanyButton - объект класса QPushButton, кнопка добавления данных в базу данных;
     *      - deleteCompanyButton - объект класса QPushButton, кнопка удаления выделенной строки из tableWidget;
     *      - deleteAllCompaniesButton - объект класса QPushButton, кнопка удаления всех данных из базы данных
     *      и tableWidget;
     *      - showAllListButton - объект класса QPushButton, кнопка вывода всех данных с базы данных;
     *      - clearListButton - объект класса QPushButton, кнопка очистки tableWidget;
     *      - searchButton - объект класса QPushButton, кнопка открытия виджета search.
     * */

public:
    mainwindow();
    ~mainwindow();

    bool connect_to_db();

    bool is_db_connect = false;


private slots:
    void load_data();
    void remove_selected_data();
    void show_search();
    void remove_all_data();
    void get_all_data();
    void clear_list();
    void search_data();
    void show_selected_provider_data();

private:
    void init_ui();

    static bool is_number(const std::string& s);
    static bool comparator(const QSqlRecord& first, const QSqlRecord& second);
    static bool int_comparator(const int& first, const int& second);

    searchWidget *search;
    QSqlDatabase db;
    QTableWidget *tableWidget;
    QStringList tableHeader;

    QWidget *contentWidget;
    QHBoxLayout *contentLayout;
    QVBoxLayout *buttonsLayout;

    QFrame *listOfCompaniesWidget;
    QPushButton *addCompanyButton;
    QPushButton *deleteCompanyButton;
    QPushButton *deleteAllCompaniesButton;
    QPushButton *showAllListButton;
    QPushButton *clearListButton;
    QPushButton *searchButton;
};

#endif //CIS_PROJECT_MAINWINDOW_H
