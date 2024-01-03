//
// Created by andiw on 20/12/2023.
//

#ifndef EDITOR_H
#define EDITOR_H

#include <QMainWindow>

#include "core/ColorPalette.h"
#include "core/IntComputer.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Editor;
}
QT_END_NAMESPACE

class Editor : public QMainWindow
{
    Q_OBJECT

  public:
    explicit Editor(QWidget *parent = nullptr);
    ~Editor() override;


  private:
    Ui::Editor *ui;
    ColorPalette palette_;
    IntComputer computer_;

    void input_text();
    void step_program();

  public slots:
    void compile();
    void run();
    void step();
};


#endif// EDITOR_H
