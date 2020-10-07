#include <QApplication>
#include "src/mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv); // Объект, отвечающий за работу приложения
    mainwindow w;               // Виджет главного окна приложения
    if (w.is_db_connect)        // Проверяется подключение к базе данных, в отсутствие подключения выводит оповещение
        w.show();               // и завершает работу приложения
    else
    {
        a.quit();               // Выход из приложения в случае отсутствия соединения с базой данных
        return 0;
    }
    return a.exec();            // Выход из приложения
}