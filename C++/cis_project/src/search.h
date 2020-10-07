#ifndef CIS_PROJECT_SEARCH_H
#define CIS_PROJECT_SEARCH_H

#include <QtWidgets>

class searchWidget : public QWidget
{
    /* Краткое описание всех атрибутов и методов класса:
     *
     *  Методы:
     *      - searchWidget() - конструктор класса, инициализирует виджет;
     *      - ~searchWidget() - деструктор класса, уничтожает виджет;
     *
     *      - init_ui() - метод, инициализирующий графический интерфейс виджета;
     *
     *  Атрибуты:
     *      - criteriaLEditList - объект класса QList<QLineEdit*>, хранит в себе виджеты класса QLineEdit, которые
     *      предназначены для ввода критериев поиска;
     *      - wLEditList - объект класса QList<QLineEdit*>, хранит в себе виджеты класса QLineEdit, которые
     *      предназначены для ввода весовых коэффициентов для поиска;
     *
     *      - contentLayout - объект класса QVBoxLayout, содержит в себе все виджеты;
     *      - formLayout - объект класса QHBoxLayout, содержит в себе все виджеты для ввода параметров;
     *      - labelsLayout - объект класса QVBoxLayout, содержит в себе все названия критериев
     *      в виде виджетов класса QLabel;
     *      - wLabelsLayout - объект класса QVBoxLayout, содержит в себе подписи к весовым коэффициентам
     *      в виде виджетов класса QLabel;
     *      - criteriaEntryLayout - объект класса QVBoxLayout, содержит в себе все виджеты из объекта criteriaLEditList;
     *      - weightEntryLayout - объект класса QVBoxLayout, содержит в себе все виджеты из объекта wLEditList;
     *
     *      - setWeightBox - объект класса QCheckBox, при отмеченном состоянии активирует использование
     *      весовых коэффициентов во время поиска по заданным параметрам;
     *      - searchButton - объект класса QPushButton, кнопка запуска функции поиска по заданным параметрам;
     * */

public:
    searchWidget();
    ~searchWidget();

    void init_ui();

    QList<QLineEdit*> criteriaLEditList;
    QList<QLineEdit*> wLEditList;

    QVBoxLayout *contentLayout;
    QHBoxLayout *formLayout;
    QVBoxLayout *labelsLayout;
    QVBoxLayout *wLabelsLayout;
    QVBoxLayout *criteriaEntryLayout;
    QVBoxLayout *weightEntryLayout;

    QCheckBox *setWeightBox;
    QPushButton *searchButton;
};

#endif //CIS_PROJECT_SEARCH_H
