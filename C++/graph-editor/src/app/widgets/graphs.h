#ifndef GRAPHS_H
#define GRAPHS_H

#include <QWidget>
#include <QtCharts>
#include "../editorwindow.h"
#include "lineparameters.h"

QT_CHARTS_USE_NAMESPACE

class editorWindow;

class Graphs : public QWidget
{
    Q_PROPERTY(int serial_number READ serial_number WRITE set_serial_number)
    Q_PROPERTY(bool legend_activate READ legend_activate WRITE set_legend)
    Q_PROPERTY(QString x_axis_name READ x_axis_name WRITE set_x_axis_name)
    Q_PROPERTY(QString x_axis_dimension READ x_axis_dimension WRITE set_x_axis_dimension)
    Q_PROPERTY(bool x_dimension_active READ x_dimension_active WRITE set_x_dimension_active)
    Q_PROPERTY(QString y_axis_name READ y_axis_name WRITE set_y_axis_name)
    Q_PROPERTY(QString y_axis_dimension READ y_axis_dimension WRITE set_y_axis_dimension)
    Q_PROPERTY(bool y_dimension_active READ y_dimension_active WRITE set_y_dimension_active)
    Q_PROPERTY(QList<LineParameters*> Lines READ Lines)

public:
    Graphs(editorWindow& parent);
    ~Graphs();

    QList<QLineSeries*> points;
    QChartView *cview;

    QValueAxis *axisX;
    QValueAxis *axisY;

    QList<LineParameters*> Lines() const;
    QList<LineParameters*>* lines_array_pointer();
    int lines_array_length();

    int serial_number() const;
    void set_serial_number(const int& serial_number);

    bool legend_activate() const;
    void set_legend(const bool& legend_activate);

    QString x_axis_name() const;
    void set_x_axis_name(const QString& x_axis_name);

    QString x_axis_dimension() const;
    void set_x_axis_dimension(const QString& x_axis_dimension);

    bool x_dimension_active() const;
    void set_x_dimension_active(const bool& x_dimension_active);

    QString y_axis_name() const;
    void set_y_axis_name(const QString& y_axis_name);

    QString y_axis_dimension() const;
    void set_y_axis_dimension(const QString& y_axis_dimension);

    bool y_dimension_active() const;
    void set_y_dimension_active(const bool& y_dimension_active);

    void draw_line(QList<double> data_x, QList<double> data_y);
    void clear_chart();

    void add_ox_caption(QString title);
    void add_oy_caption(QString title);
    void update_connections();

    void set_line_state(QLineSeries& series, bool state, int index);
    void set_line_type(QLineSeries& series, QPen type, int index);
    void set_line_color(QLineSeries& series, QString color, int index);
    void set_dot_state(QLineSeries& series, bool state, int index);
    void set_line_width(QLineSeries& series, int width, int index);

public slots:
    void change_legend_state(int index, bool state);
    void change_legend(int index);
    void change_line_state(int index);
    void change_line_type(int index);
    void change_line_color(int index);
    void change_dot_state(int index);
    void change_line_width(int index);
    void delete_line(int index);

private:
    editorWindow& _parent;
    QChart *chart;
    void init_cview();
    static bool qreal_comparator_min(const qreal &first, const qreal &second);
    static bool spesial_list_comparator_min(const QList<double> &first, const QList<double> &second);
    static bool qreal_comparator_max(const qreal &first, const qreal &second);
    static bool spesial_list_comparator_max(const QList<double> &first, const QList<double> &second);
    QList<LineParameters*> _Lines;
    int _serial_number;

    bool _legend_activate;

    QString _x_axis_name;
    QString _x_axis_dimension;
    bool _x_dimension_active;

    QString _y_axis_name;
    QString _y_axis_dimension;
    bool _y_dimension_active;

};

#endif // GRAPHS_H
