#include <QObject>
#include <QtWidgets>
#include <QtCore>
#include <QLayout>
#include <QtCharts>
#include "../editorwindow.h"
#include "graphs.h"
#include "lineparameters.h"
#include <iostream>

using namespace std;


Graphs::Graphs(editorWindow& parent) : _parent(parent)
{
    this->axisX = new QValueAxis;
    this->axisX->setRange(-10, 10);
    this->axisY = new QValueAxis;
    this->set_legend(false);
    this->set_x_axis_name(QString("untitled"));
    this->set_x_axis_dimension(QString("sec"));
    this->set_x_dimension_active(false);
    this->set_y_axis_name(QString("untitled"));
    this->set_y_axis_dimension(QString("m"));
    this->set_y_dimension_active(false);
    this->init_cview();
    this->update_connections();

    connect(&_parent, &editorWindow::change_legend_state, this, &Graphs::change_legend_state);
}

Graphs::~Graphs() {}

void Graphs::init_cview()
{
    this->chart = new QChart();
    this->chart->legend()->hide();
    this->chart->createDefaultAxes();

    this->cview = new QChartView(chart);
    this->cview->setRenderHint(QPainter::Antialiasing);
    QGridLayout *chartLayout = new QGridLayout();
    chartLayout->addWidget(cview, 0, 0);
    this->setLayout(chartLayout);
}

void Graphs::draw_line(QList<double> data_x, QList<double> data_y)
{
    this->chart->removeAxis(this->axisX);
    this->chart->removeAxis(this->axisY);
    for (int i = 0; i < this->points.length(); ++i)
    {
        this->chart->removeSeries(this->points[i]);
    }
    this->points.append(new QLineSeries());
    for(int i = 0; i < data_y.length(); i++)
    {
        this->points[this->points.length() - 1]->append(data_x[i], data_y[i]);
    }
    for (int i = 0; i < this->points.length(); ++i)
    {
        this->chart->addSeries(this->points[i]);
        this->points[i]->setName(this->Lines()[i]->legend());
    }
    QList<QList<double>> x_sorted_values;
    QList<QList<double>> y_sorted_values;
    QList<QPointF> _temp_pointf_data;
    QList<qreal> _temp_qreal_data;
    for (int i = 0; i < points.length(); ++i)
    {
        _temp_pointf_data = points[i]->points();
        for (int j = 0; j < _temp_pointf_data.length(); ++j)
        {
            _temp_qreal_data.append(_temp_pointf_data[j].x());
        }
        qSort(_temp_qreal_data.begin(), _temp_qreal_data.end(), qreal_comparator_min);
        x_sorted_values.append(_temp_qreal_data);
        _temp_qreal_data.clear();
        for (int j = 0; j < _temp_pointf_data.length(); ++j)
        {
            _temp_qreal_data.append(_temp_pointf_data[j].y());
        }
        qSort(_temp_qreal_data.begin(), _temp_qreal_data.end(), qreal_comparator_min);
        y_sorted_values.append(_temp_qreal_data);
        _temp_qreal_data.clear();
    }
    qSort(x_sorted_values.begin(), x_sorted_values.end(), spesial_list_comparator_min);
    qSort(y_sorted_values.begin(), y_sorted_values.end(), spesial_list_comparator_min);

    double x_min = x_sorted_values[0][0], y_min = y_sorted_values[0][0];

    for (int i = 0; i < x_sorted_values.length(); ++i)
    {
        qSort(x_sorted_values[i].begin(), x_sorted_values[i].end(), qreal_comparator_max);
        qSort(y_sorted_values[i].begin(), y_sorted_values[i].end(), qreal_comparator_max);
    }

    qSort(x_sorted_values.begin(), x_sorted_values.end(), spesial_list_comparator_max);
    qSort(y_sorted_values.begin(), y_sorted_values.end(), spesial_list_comparator_max);

    double x_max = x_sorted_values[0][0], y_max = y_sorted_values[0][0];

    this->axisX->setRange(x_min - ((x_max - x_min) * 0.05), x_max + ((x_max - x_min) * 0.05));
    this->axisY->setRange(y_min - ((y_max - y_min) * 0.05), y_max + ((y_max - y_min) * 0.05));

    this->chart->addAxis(this->axisX, Qt::AlignBottom);
    this->chart->addAxis(this->axisY, Qt::AlignLeft);
    //    chart->createDefaultAxes();
    foreach(QLineSeries* p, this->points)
    {
        p->attachAxis(this->axisX);
        p->attachAxis(this->axisY);
    }
}

void Graphs::clear_chart()
{
    chart->removeAxis(axisX);
    chart->removeAxis(axisY);
    for (int i = 0; i < points.length(); ++i)
    {
        chart->removeSeries(points[i]);
    }
    points.clear();
}

void Graphs::add_ox_caption(QString title)
{
    chart->removeAxis(axisX);
    axisX->setTitleText(title);
    chart->addAxis(axisX, Qt::AlignBottom);
    foreach(QLineSeries* p, points)
        {
            p->attachAxis(axisX);
        }
}

void Graphs::add_oy_caption(QString title)
{
    chart->removeAxis(axisY);
    axisY->setTitleText(title);
    chart->addAxis(axisY, Qt::AlignLeft);
            foreach(QLineSeries* p, points)
        {
            p->attachAxis(axisY);
        }
}

void Graphs::set_line_state(QLineSeries &series, bool state, int index)
{
    state ? series.show() : series.hide();
    this->lines_array_pointer()->value(index)->set_active_line(state);
}

void Graphs::set_line_type(QLineSeries &series, QPen type, int index)
{
    series.setPen(type);
    this->lines_array_pointer()->value(index)->set_line_type(this->lines_array_pointer()->value(index)->lineType->currentText());
}

void Graphs::set_line_color(QLineSeries &series, QString color, int index)
{
    series.setColor(color);
    this->lines_array_pointer()->value(index)->set_line_color(this->lines_array_pointer()->value(index)->lineColor->currentText());
}

void Graphs::set_dot_state(QLineSeries &series, bool state, int index)
{
    series.setPointsVisible(state);
    this->lines_array_pointer()->value(index)->set_dots(state);
}

void Graphs::set_line_width(QLineSeries &series, int width, int index)
{
    QPen pen = this->points[index]->pen();
    pen.setWidth(width);
    series.setPen(pen);
    this->lines_array_pointer()->value(index)->set_line_width(pen.width());
}

void Graphs::change_legend_state(int index, bool state)
{
    if(index == this->_serial_number)
        this->chart->legend()->setVisible(state);
}

void Graphs::change_legend(int index)
{
    this->points[index]->setName(this->Lines()[index]->legendEntryField->text());
    this->lines_array_pointer()->value(index)->set_legend(this->Lines()[index]->legendEntryField->text());
}

void Graphs::change_line_state(int index)
{
    if(this->Lines()[index]->lineActive->isChecked())
    {
        this->points[index]->show();
        this->lines_array_pointer()[0][index]->set_active_line(true);
        this->lines_array_pointer()->value(index)->set_active_line(true);
    }
    else
    {
        this->points[index]->hide();
        this->lines_array_pointer()[0][index]->set_active_line(false);
        this->lines_array_pointer()->value(index)->set_active_line(false);
    }
}

void Graphs::change_line_type(int index)
{
    this->points[index]->setPen(this->Lines()[index]->lineTypes[this->Lines()[index]->lineType->currentText()]);
    this->points[index]->setColor(this->Lines()[index]->lineColor->currentText());
    this->lines_array_pointer()->value(index)->set_line_type(this->Lines()[index]->lineType->currentText());
}

void Graphs::change_line_color(int index)
{
    this->points[index]->setColor(this->Lines()[index]->lineColor->currentText());
    this->lines_array_pointer()->value(index)->set_line_color(this->Lines()[index]->lineColor->currentText());
}

void Graphs::change_dot_state(int index)
{
    if(this->Lines()[index]->dotsActive->isChecked())
    {
        this->points[index]->setPointsVisible(true);
        this->lines_array_pointer()->value(index)->set_dots(true);
    }
    else
    {
        this->points[index]->setPointsVisible(false);
        this->lines_array_pointer()->value(index)->set_dots(false);
    }
}

void Graphs::change_line_width(int index)
{
    QPen pen = this->points[index]->pen();
    pen.setWidth(this->Lines()[index]->lineWidth->text().toInt());
    this->points[index]->setPen(pen);
    this->lines_array_pointer()->value(index)->set_line_width(pen.width());
}

void Graphs::delete_line(int index)
{
    this->chart->removeSeries(this->points[index]);
    this->points.removeAt(index);
    this->lines_array_pointer()->removeAt(index);
    for (int i = 0; i < this->Lines().length(); ++i)
    {
        this->lines_array_pointer()->value(i)->set_serial_number(i);
    }
}

void Graphs::update_connections()
{
    foreach(LineParameters *l, _Lines)
    {
        if(!l->hasConnection)
        {
            l->change_legend_connection = connect(l, &LineParameters::change_legend_emitter, this, &Graphs::change_legend);
            l->change_lice_state_connection = connect(l, &LineParameters::change_line_state_emitter, this, &Graphs::change_line_state);
            l->change_line_type_connection = connect(l, &LineParameters::change_line_type_emitter, this, &Graphs::change_line_type);
            l->change_line_color_connection = connect(l, &LineParameters::change_line_color_emitter, this, &Graphs::change_line_color);
            l->change_dot_state_connection = connect(l, &LineParameters::change_dot_state_emitter, this, &Graphs::change_dot_state);
            l->change_line_width_connection = connect(l, &LineParameters::change_line_width_emitter, this, &Graphs::change_line_width);
            l->delete_line_connection = connect(l, &LineParameters::delete_line_emitter, this, &Graphs::delete_line);
            l->hasConnection = true;
        }
    }
}

int Graphs::serial_number() const
{
    return _serial_number;
}

void Graphs::set_serial_number(const int &serial_number)
{
    _serial_number = serial_number;
}

QList<LineParameters*> Graphs::Lines() const
{
    return _Lines;
}

QList<LineParameters*>* Graphs::lines_array_pointer()
{
    return &_Lines;
}

int Graphs::lines_array_length()
{
    return _Lines.length();
}

bool Graphs::legend_activate() const
{
    return _legend_activate;
}

void Graphs::set_legend(const bool &legend_activate)
{
    _legend_activate = legend_activate;
}

QString Graphs::x_axis_name() const
{
    return _x_axis_name;
}

void Graphs::set_x_axis_name(const QString &x_axis_name)
{
    _x_axis_name = x_axis_name;
}

QString Graphs::x_axis_dimension() const
{
    return _x_axis_dimension;
}

void Graphs::set_x_axis_dimension(const QString &x_axis_dimension)
{
    _x_axis_dimension = x_axis_dimension;
}

bool Graphs::x_dimension_active() const
{
    return _x_dimension_active;
}

void Graphs::set_x_dimension_active(const bool &x_dimension_active)
{
    _x_dimension_active = x_dimension_active;
}

QString Graphs::y_axis_name() const
{
    return _y_axis_name;
}

void Graphs::set_y_axis_name(const QString &y_axis_name)
{
    _y_axis_name = y_axis_name;
}

QString Graphs::y_axis_dimension() const
{
    return _y_axis_dimension;
}

void Graphs::set_y_axis_dimension(const QString &y_axis_dimension)
{
    _y_axis_dimension = y_axis_dimension;
}

bool Graphs::y_dimension_active() const
{
    return _y_dimension_active;
}

void Graphs::set_y_dimension_active(const bool &y_dimension_active)
{
    _y_dimension_active = y_dimension_active;
}

bool Graphs::qreal_comparator_min(const qreal &first, const qreal &second)
{
    return first < second;
}

bool Graphs::qreal_comparator_max(const qreal &first, const qreal &second)
{
    return first > second;
}

bool Graphs::spesial_list_comparator_min(const QList<double> &first, const QList<double> &second)
{
    return first[0] < second[0];
}

bool Graphs::spesial_list_comparator_max(const QList<double> &first, const QList<double> &second)
{
    return first[0] > second[0];
}
