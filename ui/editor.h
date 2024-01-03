//
// Created by andiw on 20/12/2023.
//

#ifndef EDITOR_H
#define EDITOR_H

#include "core/ColorPalette.h"
#include <QMainWindow>


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
    ColorPalette m_palette;

    void input_text();
    void compile();
    void run();
};


#endif// EDITOR_H
