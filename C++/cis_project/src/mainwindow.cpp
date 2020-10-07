#include <iostream>
#include <QtWidgets>
#include <QModelIndex>
#include <QtSql>
#include <QtCore>
#include <sstream>
#include "mainwindow.h"
#include "search.h"

using namespace std;

mainwindow::mainwindow()
{
    is_db_connect = this->connect_to_db();
    if (is_db_connect)
    {
        this->setFixedSize(1200, 700);
        this->init_ui();
        this->setCentralWidget(contentWidget);
        search = new searchWidget;

        connect(addCompanyButton, &QPushButton::clicked, this, &mainwindow::load_data);
        connect(deleteCompanyButton, &QPushButton::clicked, this, &mainwindow::remove_selected_data);
        connect(showAllListButton, &QPushButton::clicked, this, &mainwindow::get_all_data);
        connect(deleteAllCompaniesButton, &QPushButton::clicked, this, &mainwindow::remove_all_data);
        connect(clearListButton, &QPushButton::clicked, this, &mainwindow::clear_list);
        connect(searchButton, &QPushButton::clicked, this, &mainwindow::show_search);
        connect(search->searchButton, &QPushButton::clicked, this, &mainwindow::search_data);
        connect(tableWidget, &QTableWidget::cellDoubleClicked, this, &mainwindow::show_selected_provider_data);
    }
    else
    {
        QMessageBox::critical(this, "Ошибка запуска программы",  "Не удалось подключиться к базе данных.");
        return;
    }
}

bool mainwindow::connect_to_db()
{
    db = QSqlDatabase::addDatabase("QPSQL", "cis");
    db.setHostName("wararch");
    db.setDatabaseName("cis");
    db.setUserName("cis");
    db.setPassword("cis");

    return  db.open();
}

void mainwindow::init_ui()
{
    addCompanyButton = new QPushButton("&Добавить");
    deleteCompanyButton = new QPushButton("&Удалить");
    deleteAllCompaniesButton = new QPushButton("&Удалить все");
    showAllListButton = new QPushButton("&Показать все");
    clearListButton = new QPushButton("&Очистить список");
    searchButton = new QPushButton("&Поиск...");
    buttonsLayout = new QVBoxLayout;
    buttonsLayout->setMargin(0);
    buttonsLayout->setSpacing(2);
    buttonsLayout->addWidget(addCompanyButton);
    buttonsLayout->addWidget(deleteCompanyButton);
    buttonsLayout->addWidget(deleteAllCompaniesButton);
    buttonsLayout->addWidget(showAllListButton);
    buttonsLayout->addWidget(clearListButton);
    buttonsLayout->addWidget(searchButton);
    buttonsLayout->addWidget(new QWidget);
    buttonsLayout->setStretch(6, 6);


    listOfCompaniesWidget = new QFrame;
    listOfCompaniesWidget->setFrameStyle(QFrame::Box);
    QHBoxLayout *l = new QHBoxLayout;

    tableWidget = new QTableWidget;
    QList<QString> columns = {"Название", "Цена", "Количество", "Срок доставки", "Число клиентов", "Срок фирмы"};
    for (int i = 0; i < 6; ++i)
    {
        tableHeader << columns[i];
    }
    tableWidget->setColumnCount(6);
    tableWidget->setHorizontalHeaderLabels(tableHeader);
    tableWidget->verticalHeader()->setVisible(false);
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    l->addWidget(tableWidget);
    listOfCompaniesWidget->setLayout(l);

    contentLayout = new QHBoxLayout;
    contentLayout->addWidget(listOfCompaniesWidget);
    contentLayout->addLayout(buttonsLayout);
    contentLayout->setStretch(0, 6);
    contentLayout->setStretch(1, 1);
    contentWidget = new QWidget;
    contentWidget->setLayout(contentLayout);
}

void mainwindow::show_search()
{
    search->show();
}

void mainwindow::get_all_data()
{
    while(this->tableWidget->rowCount() != 0)
    {
        this->tableWidget->removeRow(0);
    }
    QSqlQuery q(db);
    q.exec("SELECT providers.name_organization , criteria.* FROM providers INNER JOIN criteria USING (id)");
    for(int i = 0; q.next(); i++)
    {
        this->tableWidget->insertRow(i);

        this->tableWidget->setItem(i,0, new QTableWidgetItem(q.value(0).toString()));
        this->tableWidget->setItem(i,1, new QTableWidgetItem(q.value(2).toString()));
        this->tableWidget->setItem(i,2, new QTableWidgetItem(q.value(3).toString()));
        this->tableWidget->setItem(i,3, new QTableWidgetItem(q.value(4).toString()));
        this->tableWidget->setItem(i,4, new QTableWidgetItem(q.value(5).toString()));
        this->tableWidget->setItem(i,5, new QTableWidgetItem(q.value(6).toString()));
    }
    this->tableWidget->setHorizontalHeaderLabels(tableHeader);
}

void mainwindow::remove_all_data()
{
    QSqlQuery q(db);
    q.exec("DELETE FROM providers");
    q.exec("DELETE FROM criteria");
    while(this->tableWidget->rowCount() != 0)
    {
        this->tableWidget->removeRow(0);
    }
}

void mainwindow::clear_list()
{
    while(this->tableWidget->rowCount() != 0)
    {
        this->tableWidget->removeRow(0);
    }
}

void mainwindow::remove_selected_data()
{
    this->tableWidget->removeRow(this->tableWidget->currentRow());
}

void mainwindow::load_data()
{
    QString dataFile = QFileDialog::getOpenFileName(this, "Открыть файл с данными о компаниях", "/home", "csv файл (*.csv)");
    QString dataFile1 = QFileDialog::getOpenFileName(this, "Открыть файл с параметрамии компании", "/home", "csv файл (*.csv)");

    QFile file(dataFile);
    QFile file1(dataFile1);
    QSqlQuery q(db);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    if (!file1.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    else
    {
        if (dataFile.split(".")[1] == QString("csv") && dataFile1.split(".")[1] == QString("csv"))
        {
            int k = 0, k1 = 0;
            while(!file.atEnd())
            {
                QString line = file.readLine();
                k++;
            }
            while(!file1.atEnd())
            {
                QString line = file1.readLine();
                k1++;
            }
            file.seek(0);
            file1.seek(0);
            if (k == k1)
            {
                while(!file1.atEnd())
                {
                    QString line = file1.readLine();
                    if (line.split(",").length() == 6)
                    {
                        if(is_number(string(line.split(",")[0].toStdString())) &&
                           is_number(string(line.split(",")[1].toStdString())) &&
                           is_number(string(line.split(",")[2].toStdString())) &&
                           is_number(string(line.split(",")[3].toStdString())) &&
                           is_number(string(line.split(",")[4].toStdString())) &&
                           is_number(string(line.split(",")[5].toStdString())))
                        {
                            q.exec("INSERT INTO criteria values (" + line.split(",")[0] + ", " +
                                   line.split(",")[1] + ", " + line.split(",")[2] + ", " + line.split(",")[3] +
                                   ", " + line.split(",")[4] + ", " + line.split(",")[5] + ")");
                        }
                    }
                }
                while(!file.atEnd())
                {
                    QString line = file.readLine();
                    if (line.split(";").length() == 6)
                    {
                        if(is_number(string(line.split(";")[0].toStdString())) &&
                           is_number(string(line.split(";")[3].toStdString())) &&
                           is_number(string(line.split(";")[5].toStdString())))
                        {
                           q.exec("INSERT INTO providers values (" + line.split(";")[0] + ", '" +
                                  line.split(";")[1] + "', '" + line.split(";")[2] + "', " + line.split(";")[3] +
                                          ", '" + line.split(";")[4] + "', " + line.split(";")[5].split("\n")[0] + ")");
                        }
                    }
                }
            }
        }
        file.close();
        file1.close();
    }
}

void mainwindow::search_data()
{
    QList<string> column_names = {"criteria.price < ", "criteria.quantity_in_stock > ", "criteria.delivery_time < ",
                                   "criteria.number_of_clients > ", "criteria.provider_lifetime > "};
    QList<int> entered_criteria_list;
    for (int i = 0; i < 5; ++i)
    {
        if(this->search->criteriaLEditList[i]->text() != "")
            entered_criteria_list.append(stoi(this->search->criteriaLEditList[i]->text().toStdString()));
        else
            entered_criteria_list.append(-1);
    }

    int k = 0;
    for (int i = 0; i < 5; ++i)
    {
        if(entered_criteria_list[i] != -1)
            k = i;
    }

    QString filter = "";
    for (int i = 0; i < 5; ++i)
    {
        if(entered_criteria_list[i] != -1 && i < k)
            filter += QString::fromStdString(column_names[i] + std::to_string(entered_criteria_list[i]) + " AND ");
        else
        {
            if(entered_criteria_list[i] != -1 && i == k)
                filter += QString::fromStdString(column_names[i] + std::to_string(entered_criteria_list[i]));
        }
    }
    if(filter != "")
    {
        while(this->tableWidget->rowCount() > 0)
        {
            this->tableWidget->removeRow(0);
        }
        QSqlQuery q(this->db);
        q.exec(QString("SELECT providers.name_organization , criteria.* FROM providers "
                       "INNER JOIN criteria USING (id) WHERE ") + filter);

        if(this->search->setWeightBox->isChecked())
        {
            QList<double> entered_weight_list;
            QList<double> best_results;
            QList<double> rating;
            QList<QSqlRecord> result;

            double checksum = 0.0;
            for (int i = 0; i < 5; ++i)
            {
                if(this->search->wLEditList[i]->text() != "")
                {
                    if (stod(this->search->wLEditList[i]->text().toStdString()) >= 1.0)
                    {
                        QMessageBox::warning(this, "Ошибка!",  "Некорректно введены коэффициенты.");
                        return;
                    }
                    else
                        entered_weight_list.append(stod(this->search->wLEditList[i]->text().toStdString()));
                        checksum += entered_weight_list[i];
                }
                else
                    entered_weight_list.append(-1.0);
            }

            if (checksum != 1.0)
            {
                QMessageBox::warning(this, "Ошибка!",  "Некорректно введены коэффициенты.\n Из сумма должна быть равна 1");
                return;
            }

            QList<double> data;
            for (int i = 0; i < 5; ++i)
            {
                q.seek(-1);
                for (int j = 0; q.next(); ++j)
                {
                    data.append(q.value(i + 2).toInt());
                }
                q.seek(-1);
                if(entered_criteria_list[i] != -1)
                    qSort(data.begin(), data.end(), int_comparator);
                if (data.length() == 0)
                    return;
                switch (i)
                {
                    case 0:
                        if(entered_criteria_list[i] != -1)
                            best_results.append(data[0]);
                        else
                            best_results.append(0.0);
                        break;
                    case 1:
                        if(entered_criteria_list[i] != -1)
                            best_results.append(data[data.length() - 1]);
                        else
                            best_results.append(0.0);
                        break;
                    case 2:
                        if(entered_criteria_list[i] != -1)
                            best_results.append(data[0]);
                        else
                            best_results.append(0.0);

                        break;
                    case 3:
                        if(entered_criteria_list[i] != -1)
                            best_results.append(data[data.length() - 1]);
                        else
                            best_results.append(0.0);
                        break;
                    case 4:
                        if(entered_criteria_list[i] != -1)
                            best_results.append(data[data.length() - 1]);
                        else
                            best_results.append(0.0);
                        break;
                }
                data.clear();
            }

            for (int i = 0; q.next(); ++i)
            {
                rating.append(0.0);
                for (int j = 0; j < 5; ++j)
                {
                    if(entered_criteria_list[j] != -1 && j != 1 && j != 3 && j != 4)
                        rating[i] += ((entered_criteria_list[j] - q.value(j + 2).toInt()) * 100.0) /
                                (entered_criteria_list[j] - best_results[j]) * entered_weight_list[j];
                    else
                    {
                        if(entered_criteria_list[j] != -1)
                            rating[i] += ((entered_criteria_list[j] + q.value(j + 2).toInt()) * 100.0) /
                                (entered_criteria_list[j] + best_results[j]) * entered_weight_list[j];
                    }
                }
            }
            q.seek(-1);

            for (int i = 0; q.next(); i++)
            {
                result.append(q.record());
                result[i].append(QSqlField("rate", QVariant::Double));
                result[i].setValue(7, rating[i]);
            }

            qSort(result.begin(), result.end(), mainwindow::comparator);

            for (int i = 0; i < result.length(); i++)
            {
                this->tableWidget->insertRow(i);

                this->tableWidget->setItem(i, 0, new QTableWidgetItem(result[i].value(0).toString()));
                this->tableWidget->setItem(i, 1, new QTableWidgetItem(result[i].value(2).toString()));
                this->tableWidget->setItem(i, 2, new QTableWidgetItem(result[i].value(3).toString()));
                this->tableWidget->setItem(i, 3, new QTableWidgetItem(result[i].value(4).toString()));
                this->tableWidget->setItem(i, 4, new QTableWidgetItem(result[i].value(5).toString()));
                this->tableWidget->setItem(i, 5, new QTableWidgetItem(result[i].value(6).toString()));
            }
        }
        else
        {
            for (int i = 0; q.next(); i++)
            {
                this->tableWidget->insertRow(i);

                this->tableWidget->setItem(i, 0, new QTableWidgetItem(q.value(0).toString()));
                this->tableWidget->setItem(i, 1, new QTableWidgetItem(q.value(2).toString()));
                this->tableWidget->setItem(i, 2, new QTableWidgetItem(q.value(3).toString()));
                this->tableWidget->setItem(i, 3, new QTableWidgetItem(q.value(4).toString()));
                this->tableWidget->setItem(i, 4, new QTableWidgetItem(q.value(5).toString()));
                this->tableWidget->setItem(i, 5, new QTableWidgetItem(q.value(6).toString()));
            }
        }
        this->tableWidget->setHorizontalHeaderLabels(tableHeader);

    }
}

void mainwindow::show_selected_provider_data()
{
    QScrollArea *area = new QScrollArea;
    QSqlQuery q(this->db);
    q.exec("SELECT * FROM providers WHERE name_organization = '" +
           QString(tableWidget->item(tableWidget->currentRow(), 0)->text()) + "'");
    q.next();
    QVBoxLayout *content = new QVBoxLayout;
    content->addWidget(new QLabel("Название фирмы поставщика: "));
    content->addWidget(new QLabel(q.value(1).toString()));
    content->addWidget(new QLabel("=================================================================="));
    content->addWidget(new QLabel("Адрес : "));
    content->addWidget(new QLabel(q.value(2).toString()));
    content->addWidget(new QLabel("=================================================================="));
    content->addWidget(new QLabel("Телефон : " + q.value(3).toString()));
    content->addWidget(new QLabel("Страна : " + q.value(4).toString()));
    content->setAlignment(Qt::AlignCenter);

    area->setLayout(content);
    area->setFixedSize(550, 200);
    area->setWindowTitle("Информация о поставщике");

    area->show();
}

bool mainwindow::is_number(const std::string& s)
{
    long double ld;
    return((std::istringstream(s) >> ld >> std::ws).eof());
}

bool mainwindow::comparator(const QSqlRecord &first, const QSqlRecord &second)
{
    return first.value(7).toDouble() > second.value(7).toDouble();
}

bool mainwindow::int_comparator(const int &first, const int &second)
{
    return first < second;
}

mainwindow::~mainwindow() {};