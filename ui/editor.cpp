//
// Created by andiw on 20/12/2023.
//

// You may need to build the project (run Qt uic code generator) to get "ui_Editor.h" resolved

#include "editor.h"
#include "ui_Editor.h"
#include "core/IntComputer.h"

#include <QTextBrowser>


Editor::Editor(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::Editor) {
    ui->setupUi(this);

    //connect(this, ui.
    //connect(ui->textInput, &QTextBrowser::textChanged,
        //this, &Editor::input_text);

    connect(ui->buttonCompile, &QPushButton::clicked,
        this, &Editor::compile);
}

Editor::~Editor() {
    delete ui;
}

void Editor::input_text() {
    ui->codeView->document()->setHtml(ui->textInput->document()->toRawText());
}

void Editor::compile() {
    IntComputer program{ui->textInput->document()->toRawText().toStdString()};

    ui->codeView->document()->setHtml(QString::fromStdString(program.print()));
}

void Editor::run() {

}
