#include <QLayout>
#include <QLabel>
#include "launcher.h"
#include "project/project.h"
#include "project/serialization.h"
#include <iostream>
#include <string>

using namespace std;

launcher::launcher()
{
    app = new editorWindow();
    setWindowTitle(QString("Graph Editor v. 1.0.0 alpha"));
    setFixedSize(400, 150);
    init_ui();
    init_actions();
}

void launcher::init_ui()
{
    newProjectButton = new QPushButton(QString("New project..."));
    openProjectButton = new QPushButton(QString("Open project"));
    closeButton = new QPushButton(QString("Close"));
    label = new QLabel(QString("Welcome to Graph Editor!\n Please, create a new project or open to use that soft."));
    label->setAlignment(Qt::AlignCenter);
    buttons = new QHBoxLayout;
    buttons->addWidget(newProjectButton);
    buttons->addWidget(openProjectButton);
    buttons->addWidget(closeButton);
    content = new QVBoxLayout;
    content->addWidget(label);
    content->addLayout(buttons);
    setLayout(content);
}

void launcher::init_actions()
{
    connect(newProjectButton, &QPushButton::clicked, this, &launcher::newProject);
    connect(openProjectButton, &QPushButton::clicked, this, &launcher::openProject);
    connect(closeButton, &QPushButton::clicked, this, &launcher::close);
}

void launcher::newProject()
{
    Project *p = new Project(*app);
    app->projectList.append(p);
    app->projectName->setText(app->projectList[app->projectList.length() - 1]->title());
    app->GraphsTab->addTab(app->projectList[app->projectList.length() - 1]->plot(), app->projectList[app->projectList.length() - 1]->title());
    close();
    this->app->projectList[0]->plot()->set_serial_number(0);
    app->show();
    this->app->switch_tab_connection = connect(this->app->GraphsTab, &QTabWidget::currentChanged, this->app, &editorWindow::switch_tab_slot);
}

void launcher::openProject()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open JSON file", "", "JSON file (*.json);;All Files (*)");
    Project *p = new Project(*app);
    if (fileName.isEmpty())
        return;
    else
    {
        this->close();
        QFile openFile(fileName);

        if (!openFile.open(QIODevice::ReadOnly))
        {
            QMessageBox::information(this, "Unable to open file", openFile.errorString());
            return;
        }
        ProjectJsonSerializer::parse(QString(QByteArray(openFile.readAll())), p);
    }
    app->projectList.append(p);
    app->GraphsTab->addTab(app->projectList[0]->plot(), app->projectList[0]->title());
    this->app->GraphsTab->setTabText(0, this->app->projectList[0]->title());
    this->app->projectName->setText(this->app->projectList[0]->title());
    this->app->projectList[0]->plot()->set_serial_number(0);
    app->show();

    for (int i = 0; i < this->app->projectList[0]->plot()->Lines().length(); ++i)
    {
        connect(this->app->projectList[0]->plot()->lines_array_pointer()->value(i), &LineParameters::delete_line_emitter,
                this->app, &editorWindow::delete_line_data);
        this->app->projectList[0]->plot()->lines_array_pointer()->value(i)->set_serial_number(i);
        this->app->projectList[0]->plot()->update_connections();
        this->app->lineParametersLayout->addWidget(app->projectList[0]->plot()->lines_array_pointer()->value(i));
        this->app->projectList[0]->plot()->draw_line(this->app->projectList[0]->data_x()[i], this->app->projectList[0]->data_y()[i]);
        this->app->projectList[0]->plot()->set_line_state(*this->app->projectList[0]->plot()->points[i], this->app->projectList[0]->plot()->Lines()[i]->is_active(), i);
        this->app->projectList[0]->plot()->set_line_type(*this->app->projectList[0]->plot()->points[i], this->app->projectList[0]->plot()->Lines()[i]->
                                                                                                  lineTypes[this->app->projectList[0]->plot()->Lines()[i]->lineType->currentText()], i);
        this->app->projectList[0]->plot()->set_line_color(*this->app->projectList[0]->plot()->points[i], this->app->projectList[0]->plot()->Lines()[i]->lineColor->currentText(), i);
        this->app->projectList[0]->plot()->set_dot_state(*this->app->projectList[0]->plot()->points[i], this->app->projectList[0]->plot()->Lines()[i]->dots(), i);
        this->app->projectList[0]->plot()->set_line_width(*this->app->projectList[0]->plot()->points[i], this->app->projectList[0]->plot()->Lines()[i]->line_width(), i);
    }
    this->app->switch_tab_connection = connect(this->app->GraphsTab, &QTabWidget::currentChanged, this->app, &editorWindow::switch_tab_slot);
}


launcher::~launcher() {}
