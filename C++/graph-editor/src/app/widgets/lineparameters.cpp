#include <QObject>
#include <QtWidgets>
#include <QtCore>
#include <QLayout>
#include <iostream>
#include <string>
#include "lineparameters.h"

using namespace std;


LineParameters::LineParameters(QWidget *parent) :
        QWidget(parent)
{
    set_legend(QString("untitled"));
    set_active_line(true);
    set_line_type(QString("line"));
    set_line_color(QString("red"));
    set_dots(false);
    this->set_line_width(3);

    lineTypes["line"] = QPen(Qt::SolidLine);
    lineTypes["puncture"] = QPen(Qt::DashLine);
    lineTypes["dots"] = QPen(Qt::DotLine);
    lineTypes["dashdot"] = QPen(Qt::DashDotLine);
    lineTypes["dashdotdot"] = QPen(Qt::DashDotDotLine);

    line = new QLabel(QString("Line Parameters"));
    addLegendLabel = new QLabel("Legend:");
    legendEntryField = new QLineEdit;
    lineActive = new QCheckBox(QString("Line visibility"));
    lineType = new QComboBox;
    dotsActive = new QCheckBox;
    lineColor = new QComboBox;
    dotsActive = new QCheckBox(QString("Add dots"));
    lineWidthLabel = new QLabel("Line width:");
    lineWidth = new QLineEdit;
    lineWidth->setFixedSize(100, 25);
    deleteLine = new QPushButton("X");
    deleteLine->setFixedSize(30, 30);

    connect(legendEntryField, &QLineEdit::textChanged, this, &LineParameters::call_change_legend_emitter);
    connect(lineActive, &QCheckBox::stateChanged, this, &LineParameters::call_change_line_state_emitter);
    connect(lineType, &QComboBox::currentTextChanged, this, &LineParameters::call_change_line_type_emitter);
    connect(lineColor, &QComboBox::currentTextChanged, this, &LineParameters::call_change_line_color_emitter);
    connect(dotsActive, &QCheckBox::stateChanged, this, &LineParameters::call_change_dot_state_emitter);
    connect(lineWidth, &QLineEdit::textChanged, this, &LineParameters::call_change_line_width_emitter);
    connect(deleteLine, &QPushButton::clicked, this, &LineParameters::call_delete_line_emitter);

    contentLayout = new QHBoxLayout;

    contentLayout->addWidget(line);
    contentLayout->addWidget(addLegendLabel);
    contentLayout->addWidget(legendEntryField);
    contentLayout->addWidget(lineActive);
    contentLayout->addWidget(lineType);
    contentLayout->addWidget(lineColor);
    contentLayout->addWidget(dotsActive);
    contentLayout->addWidget(lineWidthLabel);
    contentLayout->addWidget(lineWidth);
    contentLayout->addWidget(deleteLine);
    this->setLayout(contentLayout);

    legendEntryField->setText(this->legend());
    lineActive->setChecked(this->is_active());

    foreach(QString name, lineTypes.keys())
        {
            lineType->addItem(name, lineTypes[name]);
        }
    lineType->setCurrentText(this->line_type());

    lineColor->addItem("red", 0);
    lineColor->addItem("green", 1);
    lineColor->addItem("blue", 2);
    lineColor->addItem("black", 3);
    lineColor->addItem("yellow", 4);
    lineColor->addItem("white", 5);
    lineColor->addItem("brown", 6);
    lineColor->setStatusTip(this->line_color());

    dotsActive->setChecked(this->dots());

    lineWidth->setValidator(new QIntValidator);
    lineWidth->setText(QString::number(this->line_width()));
}

LineParameters::~LineParameters() {}

void LineParameters::call_change_legend_emitter()
{
    emit change_legend_emitter(this->serial_number());
}

void LineParameters::call_change_line_state_emitter()
{
    emit change_line_state_emitter(this->serial_number());
}

void LineParameters::call_change_line_type_emitter()
{
    emit change_line_type_emitter(this->serial_number());
}

void LineParameters::call_change_line_color_emitter()
{
    emit change_line_color_emitter(this->serial_number());
}

void LineParameters::call_change_dot_state_emitter()
{
    emit change_dot_state_emitter(this->serial_number());
}

void LineParameters::call_change_line_width_emitter()
{
    emit change_line_width_emitter(this->serial_number());
}

void LineParameters::call_delete_line_emitter()
{
    emit delete_line_emitter(this->serial_number());
}

int LineParameters::serial_number() const
{
    return _serial_number;
}

void LineParameters::set_serial_number(const int &serial_number)
{
    _serial_number = serial_number;
}

QString LineParameters::legend() const
{
    return _legend;
}

void LineParameters::set_legend(const QString &legend)
{
    _legend = legend;
}

bool LineParameters::is_active() const
{
    return _is_active;
}

void LineParameters::set_active_line(const bool &is_active)
{
    _is_active = is_active;
}

QString LineParameters::line_type() const
{
    return _line_type;
}

void LineParameters::set_line_type(const QString &line_type)
{
    _line_type= line_type;
}

QString LineParameters::line_color() const
{
    return _line_color;
}

void LineParameters::set_line_color(const QString &line_color)
{
    _line_color = line_color;
}

bool LineParameters::dots() const
{
    return _dots;
}

void LineParameters::set_dots(const bool &dots)
{
    _dots = dots;
}

int LineParameters::line_width() const
{
    return _line_width;
}

void LineParameters::set_line_width(const int &line_width)
{
    _line_width = line_width;
}
