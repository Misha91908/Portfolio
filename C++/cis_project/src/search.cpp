#include <QtWidgets>
#include "search.h"

searchWidget::searchWidget()
{
    this->setFixedSize(600, 250);
    this->setWindowTitle("Поиск по параметрам");
    this->init_ui();
}

void searchWidget::init_ui()
{
    QList<QString> criteria_names = {"Цена:\t\t\t\t  ", "Количество на складе:\t\t  ", "Время доставки:\t\t\t  ", "Количество клиентов:\t\t  ", "Время существования поставщика:"};

    setWeightBox = new QCheckBox("Использовать весовые коэффициенты");

    criteriaEntryLayout = new QVBoxLayout;
    weightEntryLayout = new QVBoxLayout;

    for (int i = 0; i < 5; ++i)
    {
        QLineEdit *lineedit = new QLineEdit;
        lineedit->setValidator(new QIntValidator);

        QLineEdit *lineedit1 = new QLineEdit;
        QDoubleValidator *dV = new QDoubleValidator(0.0, 1.0, 1);
        lineedit1->setValidator(dV);

        criteriaLEditList.append(lineedit);
        wLEditList.append(lineedit1);

        criteriaEntryLayout->addWidget(criteriaLEditList[i]);
        weightEntryLayout->addWidget(wLEditList[i]);
    }

    labelsLayout = new QVBoxLayout;
    wLabelsLayout = new QVBoxLayout;

    for (int i = 0; i < 5; ++i)
    {
        labelsLayout->addWidget(new QLabel(criteria_names[i]));
        wLabelsLayout->addWidget(new QLabel("Вес:"));
    }

    formLayout = new QHBoxLayout;

    formLayout->addLayout(labelsLayout);
    formLayout->addLayout(criteriaEntryLayout);
    formLayout->addLayout(wLabelsLayout);
    formLayout->addLayout(weightEntryLayout);

    searchButton = new QPushButton("Поиск");
    contentLayout = new QVBoxLayout;
    contentLayout->addWidget(new QLabel("Поиск по заданным параметрам..."));
    contentLayout->addLayout(formLayout);
    contentLayout->addWidget(setWeightBox);
    contentLayout->addWidget(searchButton);
    this->setLayout(contentLayout);
}

searchWidget::~searchWidget() {};