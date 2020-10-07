#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include <QtCore>
#include <QJsonDocument>
#include "widgets/graphs.h"
#include "project/project.h"

// TODO завершить функционал менюбара
// TODO реализовать функционал, связанный с возможностью отката/возврата изменений(для бета версии)
// TODO сделать код более читабельным, попутный багфикс

class Project;

class editorWindow : public QMainWindow
{
    Q_OBJECT

public:
    editorWindow();
    ~editorWindow();
    void update_tab();

    QList<Project*> projectList;

    QTabWidget *GraphsTab;
    QVBoxLayout *lineParametersLayout;

    QLineEdit *projectName;
    QPushButton *loadDataButton;
    QPushButton *clearDataButton;
    QCheckBox *oxAddDimension;
    QComboBox *oxChooseDimension;
    QComboBox *oxChangeDimension;
    QLineEdit *oxAxisCaption;
    QCheckBox *oyAddDimension;
    QComboBox *oyChooseDimension;
    QComboBox *oyChangeDimension;
    QLineEdit *oyAxisCaption;
    QCheckBox *addLegend;

    QMenu *fileMenu;

    QMetaObject::Connection switch_tab_connection;

signals:
    void change_legend_state(int index, bool state);

public slots:
    void delete_line_data(int index);
    void switch_tab_slot(int index);

private slots:
    void newProject();
    void openProject();
    void saveProject();
    void saveAsImageProject();
    void close_tab();
    void load_data();
    void clear_data();
    void update_ox_choose_checkbox();
    void update_ox_change_checkbox();
    void update_oy_choose_checkbox();
    void update_oy_change_checkbox();
    void ox_dimension_activate();
    void oy_dimension_activate();
    void change_project_name();
    void change_ox_title();
    void change_oy_title();
    void call_change_legend_state();

private:
    void clear_line_parameters_widget(int index);
    void init_actions();
    void init_menubar();
    void init_content();
    void init_dimensions();
    bool is_number(const std::string& s);

    QWidget *contentWidget;
    QWidget *lineParametersWidget;
    QScrollArea *lineParameters;
    QHBoxLayout *contentLayout;
    QVBoxLayout *screenPlusLinesParametersLayout;
    QVBoxLayout *projectParametersLayout;
    QVBoxLayout *projectBasicLayout;
    QHBoxLayout *nameProjectLayout;
    QHBoxLayout *projectBasicButtonsLayout;
    QVBoxLayout *xAxisParametersLayout;
    QVBoxLayout *yAxisParametersLayout;
    QMessageBox *closeTabMessage;

    QMap<QString, int> dimensions_pointer;
    QMap<QString, double> time;
    QMap<QString, double> distance;
    QMap<QString, double> weight;
    QMap<QString, double> velocity;
    QMap<QString, double> voltage;
    QMap<QString, double> current;

    QAction *newAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *saveAsImgAct;

    QString oxPreviousDimension;
    QString oyPreviousDimension;
    bool oxDimensionChanged;
    bool oyDimensionChanged;
    bool oxDimension = false;
    bool oyDimension = false;
};

#endif // MAINWINDOW_H
