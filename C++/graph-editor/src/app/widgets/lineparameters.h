#ifndef GRAPHIC_EDITOR_LINEPARAMETERS_H
#define GRAPHIC_EDITOR_LINEPARAMETERS_H

#include <QObject>
#include <QtWidgets>
#include <QtCore>
#include <QLayout>


class LineParameters : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(int serial_number READ serial_number WRITE set_serial_number)
    Q_PROPERTY(QString legend READ legend WRITE set_legend)
    Q_PROPERTY(bool is_active READ is_active WRITE set_active_line)
    Q_PROPERTY(QString line_type READ line_type WRITE set_line_type)
    Q_PROPERTY(QString line_color READ line_color WRITE set_line_color)
    Q_PROPERTY(bool dots READ dots WRITE set_dots)
    Q_PROPERTY(int line_width READ line_width WRITE set_line_width)

public:
    explicit LineParameters(QWidget *parent = 0);
    ~LineParameters();

    int serial_number() const;
    void set_serial_number(const int& serial_number);

    QString legend() const;
    void set_legend(const QString& legend);

    bool is_active() const;
    void set_active_line(const bool& is_active);

    QString line_type() const;
    void set_line_type(const QString& line_type);

    QString line_color() const;
    void set_line_color(const QString& line_color);

    bool dots() const;
    void set_dots(const bool& dots);

    int line_width() const;
    void set_line_width(const int& line_width);


    QHBoxLayout *contentLayout;
    QLabel *line;
    QLabel *addLegendLabel;
    QLineEdit *legendEntryField;
    QCheckBox *lineActive;
    QComboBox *lineType;
    QComboBox *lineColor;
    QCheckBox *dotsActive;
    QLabel *lineWidthLabel;
    QLineEdit *lineWidth;
    QPushButton *deleteLine;

    bool hasConnection = false;
    QMetaObject::Connection change_legend_connection;
    QMetaObject::Connection change_lice_state_connection;
    QMetaObject::Connection change_line_type_connection;
    QMetaObject::Connection change_line_color_connection;
    QMetaObject::Connection change_dot_state_connection;
    QMetaObject::Connection change_line_width_connection;
    QMetaObject::Connection delete_line_connection;

    QMap<QString, QPen> lineTypes;

signals:
    void change_legend_emitter(int index);
    void change_line_state_emitter(int index);
    void change_line_type_emitter(int index);
    void change_line_color_emitter(int index);
    void change_dot_state_emitter(int index);
    void change_line_width_emitter(int index);
    void delete_line_emitter(int index);

private slots:
    void call_change_legend_emitter();
    void call_change_line_state_emitter();
    void call_change_line_type_emitter();
    void call_change_line_color_emitter();
    void call_change_dot_state_emitter();
    void call_change_line_width_emitter();
    void call_delete_line_emitter();

private:
    int _serial_number;
    QString _legend;
    bool _is_active;
    QString _line_type;
    QString _line_color;
    bool _dots;
    int _line_width;
};


#endif //GRAPHIC_EDITOR_LINEPARAMETERS_H
