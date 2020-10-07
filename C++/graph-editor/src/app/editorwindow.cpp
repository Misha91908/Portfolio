#include <QtCore>
#include <QtCharts>
#include <QWidget>
#include <QLayout>
#include <QAction>
#include <QFileDialog>
#include <QMenuBar>
#include <QJsonDocument>
#include "editorwindow.h"
#include "project/serialization.h"
#include "widgets/graphs.h"
#include "project/project.h"
#include "widgets/lineparameters.h"
#include <iostream>
#include <sstream>

using namespace std;

editorWindow::editorWindow()
{
    this->setGeometry(200, 200, 1300, 700);
    this->init_dimensions();
    this->init_content();
    this->init_menubar();
    this->init_actions();
    this->setWindowTitle(QString("Graph Editor v.1.0.0 alpha"));
    this->setCentralWidget(contentWidget);
}

void editorWindow::init_content()
{
    GraphsTab = new QTabWidget;
    GraphsTab->setTabsClosable(true);
    this->GraphsTab->setMovable(true);

    this->closeTabMessage = new QMessageBox();
    this->closeTabMessage->setText("Save project before closing");
    this->closeTabMessage->setInformativeText("Do you want to save project?");
    this->closeTabMessage->setStandardButtons(QMessageBox::Yes | QMessageBox::No);

    projectName = new QLineEdit;
    projectName->setText(QString("untitled"));
    nameProjectLayout = new QHBoxLayout;
    nameProjectLayout->addWidget(new QLabel(QString("Name: ")));
    nameProjectLayout->addWidget(projectName);

    loadDataButton = new QPushButton(QString("Load data"));
    clearDataButton = new QPushButton(QString("Clear"));
    projectBasicButtonsLayout = new QHBoxLayout;
    projectBasicButtonsLayout->addWidget(loadDataButton);
    projectBasicButtonsLayout->addWidget(clearDataButton);

    projectBasicLayout = new QVBoxLayout;
    projectBasicLayout->addLayout(nameProjectLayout);
    projectBasicLayout->addLayout(projectBasicButtonsLayout);

    oxAddDimension = new QCheckBox(QString("Dimension"));
    oxChooseDimension = new QComboBox;
    foreach(QString key, dimensions_pointer.keys())
        {
            oxChooseDimension->addItem(key, dimensions_pointer.value(key));
        }
    oxChooseDimension->setCurrentText(dimensions_pointer.key(0));
    oxChangeDimension = new QComboBox;
    foreach(QString key, time.keys())
        {
            oxChangeDimension->addItem(key, time.value(key));
        }
    oxDimensionChanged = false;
    oxPreviousDimension = oxChangeDimension->currentText();
    oxAxisCaption = new QLineEdit;
    xAxisParametersLayout = new QVBoxLayout;
    xAxisParametersLayout->addWidget(new QLabel(QString("OX Axis")));
    xAxisParametersLayout->addWidget(oxAddDimension);
    xAxisParametersLayout->addWidget(oxChooseDimension);
    xAxisParametersLayout->addWidget(oxChangeDimension);
    xAxisParametersLayout->addWidget(oxAxisCaption);
    xAxisParametersLayout->setMargin(2);
    xAxisParametersLayout->setSpacing(2);

    oyAddDimension = new QCheckBox(QString("Dimension"));
    oyChooseDimension = new QComboBox;
    foreach(QString key, dimensions_pointer.keys())
        {
            oyChooseDimension->addItem(key, dimensions_pointer.value(key));
        }
    oyChooseDimension->setCurrentText(dimensions_pointer.key(1));
    oyChangeDimension = new QComboBox;
    foreach(QString key, distance.keys())
        {
            oyChangeDimension->addItem(key, distance.value(key));
        }
    oyDimensionChanged = false;
    oyPreviousDimension = oyChangeDimension->currentText();
    oyAxisCaption = new QLineEdit;
    yAxisParametersLayout = new QVBoxLayout;
    yAxisParametersLayout->addWidget(new QLabel(QString("OY Axis")));
    yAxisParametersLayout->addWidget(oyAddDimension);
    yAxisParametersLayout->addWidget(oyChooseDimension);
    yAxisParametersLayout->addWidget(oyChangeDimension);
    yAxisParametersLayout->addWidget(oyAxisCaption);
    yAxisParametersLayout->setMargin(2);
    yAxisParametersLayout->setSpacing(2);

    addLegend = new QCheckBox("Add legend");

    projectParametersLayout = new QVBoxLayout;
    projectParametersLayout->addLayout(projectBasicLayout);
    projectParametersLayout->addLayout(xAxisParametersLayout);
    projectParametersLayout->addLayout(yAxisParametersLayout);
    projectParametersLayout->addWidget(addLegend);
    projectParametersLayout->addWidget(new QWidget());
    projectParametersLayout->setMargin(2);
    projectParametersLayout->setSpacing(2);
    this->projectParametersLayout->setStretch(4, 4);


    lineParameters = new QScrollArea;
    lineParametersWidget = new QWidget();
    lineParameters->setWidget(lineParametersWidget);
    lineParametersWidget->resize(200, 250);
    lineParameters->setWidgetResizable(true);

    lineParametersLayout = new QVBoxLayout;
    lineParametersLayout->setMargin(1);
    lineParametersLayout->setSpacing(1);
    lineParametersWidget->setLayout(lineParametersLayout);

    screenPlusLinesParametersLayout = new QVBoxLayout;
    screenPlusLinesParametersLayout->addWidget(GraphsTab);
    screenPlusLinesParametersLayout->addWidget(lineParameters);
    screenPlusLinesParametersLayout->setStretch(0, 5);
    screenPlusLinesParametersLayout->setStretch(1, 1);

    contentLayout = new QHBoxLayout;
    contentLayout->addLayout(screenPlusLinesParametersLayout);
    contentLayout->addLayout(projectParametersLayout);
    contentLayout->setStretch(0, 5);
    contentLayout->setStretch(1, 1);
    contentWidget = new QWidget;
    contentWidget->setLayout(contentLayout);
}

void editorWindow::init_menubar()
{
    fileMenu = menuBar()->addMenu("&File");

    newAct = new QAction("&New &Project", this);
    fileMenu->addAction(newAct);
    connect(newAct, &QAction::triggered, this, &editorWindow::newProject);

    openAct = new QAction("&Open &Project", this);
    fileMenu->addAction(openAct);
    connect(openAct, &QAction::triggered, this, &editorWindow::openProject);

    saveAct = new QAction("&Save &Project", this);
    fileMenu->addAction(saveAct);
    connect(saveAct, &QAction::triggered, this, &editorWindow::saveProject);

    saveAsImgAct = new QAction("&Save &As &Image", this);
    fileMenu->addAction(saveAsImgAct);
    connect(saveAsImgAct, &QAction::triggered, this, &editorWindow::saveAsImageProject);


    fileMenu->addSeparator();
}

void editorWindow::init_actions()
{
    connect(this->loadDataButton, &QPushButton::clicked, this, &editorWindow::load_data);
    connect(this->clearDataButton, &QPushButton::clicked, this, &editorWindow::clear_data);
    connect(this->projectName, &QLineEdit::textChanged, this, &editorWindow::change_project_name);
    connect(this->oxChooseDimension, &QComboBox::currentTextChanged, this, &editorWindow::update_ox_choose_checkbox);
    connect(this->oyChooseDimension, &QComboBox::currentTextChanged, this, &editorWindow::update_oy_choose_checkbox);
    connect(this->oxChangeDimension, &QComboBox::currentTextChanged, this, &editorWindow::update_ox_change_checkbox);
    connect(this->oyChangeDimension, &QComboBox::currentTextChanged, this, &editorWindow::update_oy_change_checkbox);
    connect(this->oxAddDimension, &QCheckBox::stateChanged, this, &editorWindow::ox_dimension_activate);
    connect(this->oyAddDimension, &QCheckBox::stateChanged, this, &editorWindow::oy_dimension_activate);
    connect(this->oxAxisCaption, &QLineEdit::textChanged, this, &editorWindow::change_ox_title);
    connect(this->oyAxisCaption, &QLineEdit::textChanged, this, &editorWindow::change_oy_title);
    connect(this->addLegend, &QCheckBox::stateChanged, this, &editorWindow::call_change_legend_state);
    connect(this->GraphsTab, &QTabWidget::tabCloseRequested, this, &editorWindow::close_tab);
}

void editorWindow::init_dimensions()
{
    time["ns"] = 1E-9;
    time["us"] = 1E-6;
    time["ms"] = 1E-3;
    time["s"] = 1.0;
    time["min"] = 60.0;
    time["h"] = 3600.0;
    distance["nM"] = 1E-9;
    distance["uM"] = 1E-6;
    distance["mM"] = 1E-3;
    distance["dM"] = 0.1;
    distance["M"] = 1.0;
    distance["kM"] = 1E+3;
    weight["ug"] = 1E-9;
    weight["mg"] = 1E-6;
    weight["g"] = 1E-3;
    weight["kg"] = 1.0;
    weight["t"] = 1E+3;
    velocity["m/s"] = 1.0;
    velocity["km/h"] = 3.6;
    voltage["nV"] = 1E-9;
    voltage["uV"] = 1E-6;
    voltage["mV"] = 1E-3;
    voltage["V"] = 1.0;
    voltage["kV"] = 1E+3;
    voltage["MV"] = 1E+6;
    current["nA"] = 1E-9;
    current["uA"] = 1E-6;
    current["mA"] = 1E-3;
    current["A"] = 1.0;
    current["kA"] = 1E+3;
    dimensions_pointer["time"] = 0;
    dimensions_pointer["distance"] = 1;
    dimensions_pointer["weight"] = 2;
    dimensions_pointer["velocity"] = 3;
    dimensions_pointer["voltage"] = 4;
    dimensions_pointer["current"] = 5;

}

void editorWindow::newProject()
{
    this->projectList.append(new Project(*this));
    this->projectList[this->projectList.length() - 1]->plot()->set_serial_number(this->projectList.length() - 1);
    this->GraphsTab->addTab(this->projectList[this->projectList.length() - 1]->plot(),
                            this->projectList[this->projectList.length() - 1]->title());
    this->GraphsTab->setCurrentIndex(this->GraphsTab->count() - 1);
}

void editorWindow::openProject()
{
    disconnect(this->switch_tab_connection);
    QString fileName = QFileDialog::getOpenFileName(this, "Open JSON file", "", "JSON file (*.json)");
    if (fileName.isEmpty())
        return;
    else
    {
        QFile openFile(fileName);

        if (!openFile.open(QIODevice::ReadOnly))
        {
            QMessageBox::information(this, "Unable to open file", openFile.errorString());
            return;
        }

        Project *p = new Project(*this);

        ProjectJsonSerializer::parse(QString(QByteArray(openFile.readAll())), p);

        this->projectList.append(p);

        int index = this->projectList.length() - 1;

        this->projectList[index]->plot()->set_serial_number(index);
        this->GraphsTab->addTab(this->projectList[index]->plot(), this->projectList[index]->title());
        this->clear_line_parameters_widget(dynamic_cast<Graphs*>(this->GraphsTab->currentWidget())->serial_number());
        this->GraphsTab->setCurrentIndex(this->GraphsTab->count() - 1);
        this->projectList[index]->plot()->update_connections();
        this->projectName->setText(this->projectList[index]->title());

        this->projectList[index]->plot()->set_x_dimension_active(this->projectList[index]->plot()->x_dimension_active());
        this->projectList[index]->plot()->set_x_axis_dimension(this->projectList[index]->plot()->x_axis_dimension());
        this->projectList[index]->plot()->set_x_axis_name(this->projectList[index]->plot()->x_axis_name());

        this->projectList[index]->plot()->set_y_dimension_active(this->projectList[index]->plot()->y_dimension_active());
        this->projectList[index]->plot()->set_y_axis_dimension(this->projectList[index]->plot()->y_axis_dimension());
        this->projectList[index]->plot()->set_y_axis_name(this->projectList[index]->plot()->y_axis_name());

        for (int i = 0; i < this->projectList[index]->data_x().length(); ++i)
        {
            connect(this->projectList[index]->plot()->lines_array_pointer()->value(i),
                    &LineParameters::delete_line_emitter, this, &editorWindow::delete_line_data);
            this->projectList[index]->plot()->lines_array_pointer()->value(i)->set_serial_number(i);
            this->lineParametersLayout->addWidget(this->projectList[index]->plot()->lines_array_pointer()->value(i));
            this->projectList[index]->plot()->draw_line(this->projectList[index]->data_x()[i],
                                                                            this->projectList[index]->data_y()[i]);
            this->projectList[index]->plot()->set_line_state(*this->projectList[index]->plot()->points[i],
                                                                            this->projectList[index]->plot()->Lines()[i]->is_active(), i);
            this->projectList[index]->plot()->set_line_type(*this->projectList[index]->plot()->points[i], this->projectList[index]->plot()->Lines()[i]->
                                                                            lineTypes[this->projectList[index]->plot()->Lines()[i]->lineType->currentText()], i);
            this->projectList[index]->plot()->set_line_color(*this->projectList[index]->plot()->points[i], this->projectList[index]->plot()->Lines()[i]
                                                                            ->lineColor->currentText(), i);
            this->projectList[index]->plot()->set_dot_state(*this->projectList[index]->plot()->points[i], this->projectList[index]->plot()->Lines()[i]->dots(), i);
            this->projectList[index]->plot()->set_line_width(*this->projectList[index]->plot()->points[i], this->projectList[index]->plot()->Lines()[i]->line_width(), i);
        }
    }
    this->switch_tab_connection = connect(this->GraphsTab, &QTabWidget::currentChanged, this, &editorWindow::switch_tab_slot);
}

void editorWindow::saveProject()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save Project File", "", "JSON file (*.json)");
    fileName += QString(".json");

    if (fileName.isEmpty())
        return;
    else
    {
        QFile saveFile(fileName);
        if (!saveFile.open(QIODevice::WriteOnly))
        {
            QMessageBox::information(this, "Unable to open file", saveFile.errorString());
            return;
        }
        saveFile.write(ProjectJsonSerializer::serialize(projectList[GraphsTab->currentIndex()]));
    }
}

void editorWindow::saveAsImageProject()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save Image", "", "PNG Image (*.png)");

    if (fileName.isEmpty())
        return;
    else
    {
        QPixmap p = this->projectList[dynamic_cast<Graphs*>(this->GraphsTab->currentWidget())->serial_number()]->plot()->cview->grab();
        p.save(fileName + QString(".png"), "PNG");
    }
}

void editorWindow::load_data()
{
    QString dataFile = QFileDialog::getOpenFileName(this, "Open File", "/home", "json files (*.json);;csv files (*.csv)");
    QFile file(dataFile);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
          return;
    else
        {
            int index = dynamic_cast<Graphs*>(this->GraphsTab->currentWidget())->serial_number();
            if (dataFile.split(".")[1] == QString("csv"))
            {

                QList<double> datax, datay;
                while(!file.atEnd())
                {
                    QString line = file.readLine();
                    if (line.split(",").length() == 2)
                    {
                        if(is_number(string(line.split(",")[0].toStdString())) && is_number(string(line.split(",")[1].toStdString())))
                        {
                            datax.append(stod(string(line.split(",")[0].replace(".", ",").toStdString())));
                            datay.append(stod(string(line.split(",")[1].replace(".", ",").toStdString())));
                        }
                    }
                }

                this->projectList[index]->datax_array_pointer()->append(datax);
                this->projectList[index]->datay_array_pointer()->append(datay);
                this->projectList[index]->plot()->lines_array_pointer()->append(new LineParameters());
                connect(this->projectList[index]->plot()->lines_array_pointer()->last(), &LineParameters::delete_line_emitter,
                        this, &editorWindow::delete_line_data);
                this->projectList[index]->plot()->lines_array_pointer()->last()->set_serial_number(this->projectList[index]
                         ->plot()->lines_array_pointer()->length() - 1);
                this->projectList[index]->plot()->update_connections();
                this->lineParametersLayout->addWidget(this->projectList[index]->plot()->lines_array_pointer()->last());
                this->projectList[index]->plot()->draw_line(datax, datay);
                this->projectList[index]->plot()->set_line_state(*this->projectList[index]->plot()->points[this->projectList[index]
                                                                                     ->plot()->Lines().length() - 1],
                                                                                     this->projectList[index]->plot()->
                                                                                     Lines()[this->projectList[index]->plot()->Lines().length() - 1]->is_active(),
                                                                                     this->projectList[index]->plot()->Lines().length() - 1);
                this->projectList[index]->plot()->set_line_type(*this->projectList[index]->plot()->points[this->projectList[index]->
                                                                                     plot()->Lines().length() - 1], this->projectList[index]->plot()->
                                                                                     Lines()[this->projectList[index]->plot()->Lines().length() - 1]->
                                                                                     lineTypes[this->projectList[index]->
                                                                                     plot()->Lines()[this->projectList[index]->plot()->
                                                                                     Lines().length() - 1]->lineType->currentText()],
                                                                                     this->projectList[index]->plot()->Lines().length() - 1);
                this->projectList[index]->plot()->set_line_color(*this->projectList[index]->plot()->
                                                                                     points[this->projectList[index]->plot()->Lines().length() - 1],
                                                                                     this->projectList[index]->plot()->Lines()[this->projectList[index]
                                                                                     ->plot()->Lines().length() - 1]->lineColor->currentText(),
                                                                                     this->projectList[index]->plot()->Lines().length() - 1);
                this->projectList[index]->plot()->set_dot_state(*this->projectList[index]->plot()->points[this->projectList[index]->plot()->Lines().length() - 1],
                                                                                     this->projectList[index]->plot()->Lines()[this->projectList[index]->plot()->
                                                                                     Lines().length() - 1]->dots(), this->projectList[index]->plot()->Lines().length() - 1);
                this->projectList[index]->plot()->set_line_width(*this->projectList[index]->plot()->points[this->projectList[index]->plot()->Lines().length() - 1],
                                                                                     this->projectList[index]->plot()->Lines()[this->projectList[index]->plot()->
                                                                                     Lines().length() - 1]->line_width(), this->projectList[index]->plot()->Lines().length() - 1);
            }
            if (dataFile.split(".")[1] == QString("json"))
            {
                Project *p = new Project(*this);
                ProjectJsonSerializer::parse(QString(QByteArray(file.readAll())), p);
                for (int i = 0; i < p->data_x().length(); ++i)
                {
                    this->projectList[index]->datax_array_pointer()->append(p->data_x()[i]);
                    this->projectList[index]->datay_array_pointer()->append(p->data_y()[i]);
                    this->projectList[index]->plot()->lines_array_pointer()->append(new LineParameters());
                    connect(this->projectList[index]->plot()->lines_array_pointer()->last(),
                            &LineParameters::delete_line_emitter, this, &editorWindow::delete_line_data);
                    this->projectList[index]->plot()->lines_array_pointer()->last()->set_serial_number(this->projectList[index]->plot()->lines_array_pointer()->length() - 1);
                    this->lineParametersLayout->addWidget(projectList[index]->plot()->lines_array_pointer()->last());
                    this->projectList[index]->plot()->draw_line(p->data_x()[i], p->data_y()[i]);
                }
                for (int i = 0; i < this->projectList[index]->data_x().length(); ++i)
                {
                    this->projectList[index]->plot()->set_line_state(*this->projectList[index]->plot()->points[i],
                                                                                         this->projectList[index]->plot()->Lines()[i]->is_active(),
                                                                                         this->projectList[index]->plot()->Lines().length() - 1);
                    this->projectList[index]->plot()->set_line_type(*this->projectList[index]->plot()->points[i], this->projectList[index]->plot()->Lines()[i]->
                                                                                         lineTypes[this->projectList[index]->plot()->Lines()[i]->lineType->currentText()],
                                                                                         this->projectList[GraphsTab->currentIndex()]->plot()->Lines().length() - 1);
                    this->projectList[index]->plot()->set_line_color(*this->projectList[index]->plot()->points[i], this->projectList[index]->plot()->Lines()[i]->lineColor->currentText(),
                                                                                         this->projectList[index]->plot()->Lines().length() - 1);
                    this->projectList[index]->plot()->set_dot_state(*this->projectList[index]->plot()->points[i], this->projectList[index]->plot()->Lines()[i]->dots(),
                                                                                        this->projectList[index]->plot()->Lines().length() - 1);
                    this->projectList[index]->plot()->set_line_width(*this->projectList[index]->plot()->points[i], this->projectList[index]->plot()->Lines()[i]->line_width(),
                                                                                         this->projectList[index]->plot()->Lines().length() - 1);
                }
                this->projectList[index]->plot()->update_connections();
                delete p;
                p = nullptr;
            }
        }
}

void editorWindow::close_tab()
{
    int result = closeTabMessage->exec();

    if(result == QMessageBox::Yes)
    {
        this->saveProject();
    }

    if(this->GraphsTab->count() == 1)
    {
        this->close();
        return;
    }
    this->projectList.removeAt(dynamic_cast<Graphs*>(this->GraphsTab->currentWidget())->serial_number());
    this->GraphsTab->removeTab(this->GraphsTab->currentIndex());
}

void editorWindow::update_tab()
{
    int index = dynamic_cast<Graphs*>(this->GraphsTab->currentWidget())->serial_number();
    this->GraphsTab->setTabText(this->GraphsTab->currentIndex(), this->projectList[index]->title());
    this->projectName->setText(this->projectList[index]->title());
    for (int i = 0; i < projectList[index]->data_x().length(); ++i)
    {
        this->projectList[index]->plot()->draw_line(this->projectList[index]->data_x()[i], this->projectList[index]->data_y()[i]);
    }
}

void editorWindow::change_project_name()
{
    GraphsTab->setTabText(GraphsTab->currentIndex(), projectName->text());
    this->projectList[dynamic_cast<Graphs*>(this->GraphsTab->currentWidget())->serial_number()]->set_title(projectName->text());
}

void editorWindow::clear_data()
{
    while (lineParametersLayout->count())
    {
        QLayoutItem *item = lineParametersLayout->takeAt(0);
        QWidget *w = item->widget();
        w->deleteLater();

        delete item, w;
        item, w = nullptr;
    }
    projectList[GraphsTab->currentIndex()]->clear_data();
    projectList[GraphsTab->currentIndex()]->plot()->clear_chart();
}

void editorWindow::switch_tab_slot(int index)
{
    index = dynamic_cast<Graphs*>(this->GraphsTab->widget(index))->serial_number();

    this->clear_line_parameters_widget(dynamic_cast<Graphs*>(this->GraphsTab->currentWidget())->serial_number());

    this->projectName->setText(this->projectList[index]->title());

    this->projectList[index]->plot()->set_x_dimension_active(this->projectList[index]->plot()->x_dimension_active());
    this->projectList[index]->plot()->set_x_axis_dimension(this->projectList[index]->plot()->x_axis_dimension());
    this->projectList[index]->plot()->set_x_axis_name(this->projectList[index]->plot()->x_axis_name());

    this->projectList[index]->plot()->set_y_dimension_active(this->projectList[index]->plot()->y_dimension_active());
    this->projectList[index]->plot()->set_y_axis_dimension(this->projectList[index]->plot()->y_axis_dimension());
    this->projectList[index]->plot()->set_y_axis_name(this->projectList[index]->plot()->y_axis_name());

    for (int i = 0; i < this->projectList[index]->plot()->lines_array_length(); ++i)
    {
        this->lineParametersLayout->addWidget(this->projectList[index]->plot()->lines_array_pointer()->value(i));
    }
}

void editorWindow::update_ox_choose_checkbox()
{
    if (oxDimension)
    {
        oxChangeDimension->clear();
        oxDimensionChanged = true;
        if(oxChooseDimension->currentText() == QString("time"))
        {
            foreach(QString key, time.keys())
                {
                    oxChangeDimension->addItem(key, time.value(key));
                }
        }
        if(oxChooseDimension->currentText() == QString("distance"))
        {
                    foreach(QString key, distance.keys())
                {
                    oxChangeDimension->addItem(key, distance.value(key));
                }
        }
        if(oxChooseDimension->currentText() == QString("weight"))
        {
                    foreach(QString key, weight.keys())
                {
                    oxChangeDimension->addItem(key, weight.value(key));
                }
        }
        if(oxChooseDimension->currentText() == QString("velocity"))
        {
                    foreach(QString key, velocity.keys())
                {
                    oxChangeDimension->addItem(key, velocity.value(key));
                }
        }
        if(oxChooseDimension->currentText() == QString("voltage"))
        {
                    foreach(QString key, voltage.keys())
                {
                    oxChangeDimension->addItem(key, voltage.value(key));
                }
        }
        if(oxChooseDimension->currentText() == QString("current"))
        {
                    foreach(QString key, current.keys())
                {
                    oxChangeDimension->addItem(key, current.value(key));
                }
        }
        oxPreviousDimension = oxChangeDimension->currentText();
    }
}

void editorWindow::update_oy_choose_checkbox()
{
    if (oyDimension)
    {
        oyChangeDimension->clear();
        oyDimensionChanged = true;
        if(oyChooseDimension->currentText() == QString("time"))
        {
                    foreach(QString key, time.keys())
                {
                    oyChangeDimension->addItem(key, time.value(key));
                }
        }
        if(oyChooseDimension->currentText() == QString("distance"))
        {
                    foreach(QString key, distance.keys())
                {
                    oyChangeDimension->addItem(key, distance.value(key));
                }
        }
        if(oyChooseDimension->currentText() == QString("weight"))
        {
                    foreach(QString key, weight.keys())
                {
                    oyChangeDimension->addItem(key, weight.value(key));
                }
        }
        if(oyChooseDimension->currentText() == QString("velocity"))
        {
                    foreach(QString key, velocity.keys())
                {
                    oyChangeDimension->addItem(key, velocity.value(key));
                }
        }
        if(oyChooseDimension->currentText() == QString("voltage"))
        {
                    foreach(QString key, voltage.keys())
                {
                    oyChangeDimension->addItem(key, voltage.value(key));
                }
        }
        if(oyChooseDimension->currentText() == QString("current"))
        {
                    foreach(QString key, current.keys())
                {
                    oyChangeDimension->addItem(key, current.value(key));
                }
        }
        oyPreviousDimension = oyChangeDimension->currentText();
    }
}

void editorWindow::update_ox_change_checkbox()
{
    if (this->oxDimension)
    {
        int index = dynamic_cast<Graphs*>(this->GraphsTab->currentWidget())->serial_number();
        if (!this->oxDimensionChanged && this->oxChangeDimension->count() > 0)
        {
            double prev_x = this->oxChangeDimension->itemData(this->oxChangeDimension->findText(this->oxPreviousDimension)).toDouble();
            double scalar = prev_x / this->oxChangeDimension->itemData(this->oxChangeDimension->currentIndex()).toDouble();
            this->oxPreviousDimension = this->oxChangeDimension->currentText();
            for (int i = 0; i < projectList[index]->data_x().length(); ++i)
            {
                QList<double> ox_list = this->projectList[index]->data_x()[i];
                for (int j = 0; j < ox_list.length(); ++j)
                {
                    ox_list[j] *= scalar;
                }
                projectList[GraphsTab->currentIndex()]->datax_array_pointer()->replace(i, ox_list);
            }
            this->projectList[index]->plot()->clear_chart();
            this->update_tab();
            this->projectList[index]->plot()->set_x_axis_dimension(this->oxChangeDimension->currentText());
        }
        this->oxDimensionChanged = false;
    }
}

void editorWindow::update_oy_change_checkbox()
{
    if (this->oyDimension)
    {
        int index = dynamic_cast<Graphs*>(this->GraphsTab->currentWidget())->serial_number();
        if (!this->oyDimensionChanged && this->oyChangeDimension->count() > 0)
        {
            double prev_y = this->oyChangeDimension->itemData(this->oyChangeDimension->findText(this->oyPreviousDimension)).toDouble();
            double scalar = prev_y / this->oyChangeDimension->itemData(this->oyChangeDimension->currentIndex()).toDouble();
            this->oyPreviousDimension = this->oyChangeDimension->currentText();
            for (int i = 0; i < projectList[index]->data_y().length(); ++i)
            {
                QList<double> oy_list = this->projectList[index]->data_y()[i];
                for (int j = 0; j < oy_list.length(); ++j)
                {
                    oy_list[j] *= scalar;
                }
                this->projectList[index]->datay_array_pointer()->replace(i, oy_list);
            }
            this->projectList[index]->plot()->clear_chart();
            this->update_tab();
            this->projectList[index]->plot()->set_y_axis_dimension(this->oyChangeDimension->currentText());
        }
        this->oyDimensionChanged = false;
    }
}

void editorWindow::ox_dimension_activate()
{
    this->oxAddDimension->isChecked() ? this->oxDimension = true : this->oxDimension = false;
    this->projectList[dynamic_cast<Graphs*>(this->GraphsTab->currentWidget())->serial_number()]->plot()->set_x_dimension_active(this->oxAddDimension->isChecked());
}

void editorWindow::oy_dimension_activate()
{
    this->oyAddDimension->isChecked() ? this->oyDimension = true : this->oyDimension = false;
    this->projectList[dynamic_cast<Graphs*>(this->GraphsTab->currentWidget())->serial_number()]->plot()->set_y_dimension_active(this->oyAddDimension->isChecked());
}

void editorWindow::change_ox_title()
{
    int index = dynamic_cast<Graphs*>(this->GraphsTab->currentWidget())->serial_number();
    this->projectList[index]->plot()->add_ox_caption(this->oxAxisCaption->text());
    this->projectList[index]->plot()->set_x_axis_name(this->oxAxisCaption->text());
}

void editorWindow::change_oy_title()
{
    int index = dynamic_cast<Graphs*>(this->GraphsTab->currentWidget())->serial_number();
    this->projectList[index]->plot()->add_oy_caption(this->oyAxisCaption->text());
    this->projectList[index]->plot()->set_y_axis_name(this->oyAxisCaption->text());
}

void editorWindow::call_change_legend_state()
{
    emit change_legend_state(this->GraphsTab->currentIndex(), this->addLegend->isChecked());
}

void editorWindow::delete_line_data(int index)
{
    QLayoutItem *item = lineParametersLayout->takeAt(index);
    QWidget *w = item->widget();
    w->deleteLater();

    int _index = dynamic_cast<Graphs*>(this->GraphsTab->currentWidget())->serial_number();
    this->projectList[_index]->datax_array_pointer()->removeAt(index);
    this->projectList[_index]->datay_array_pointer()->removeAt(index);
}

void editorWindow::clear_line_parameters_widget(int index)
{
    while (QLayoutItem *item = this->lineParametersLayout->takeAt(0))
    {
        this->lineParametersLayout->removeItem(item);
        this->lineParametersLayout->removeWidget(item->widget());
        item->widget()->setParent(this->projectList[index]->plot());
        delete item;
        this->lineParametersLayout->update();
    }
}

bool editorWindow::is_number(const std::string& s)
{
    long double ld;
    return((std::istringstream(s) >> ld >> std::ws).eof());
}

editorWindow::~editorWindow() {}
