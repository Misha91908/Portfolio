#ifndef LAUNCHER_H
#define LAUNCHER_H

#include <QWidget>
#include <QPushButton>
#include "editorwindow.h"
#include"project/project.h"

class launcher : public QWidget
{
public:
    launcher();
    ~launcher();
    bool launch = false;
    editorWindow *app;

private slots:
    void openProject();
    void newProject();

private:
    void init_ui();
    void init_actions();
    QPushButton *newProjectButton;
    QPushButton *openProjectButton;
    QPushButton *closeButton;
    QHBoxLayout *buttons;
    QVBoxLayout *content;
    QLabel *label;
};

#endif // LAUNCHER_H
