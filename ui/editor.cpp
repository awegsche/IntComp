//
// Created by andiw on 20/12/2023.
//

// You may need to build the project (run Qt uic code generator) to get "ui_Editor.h" resolved

#include "editor.h"
#include "core/IntComputer.h"
#include "ui_Editor.h"

#include <fmttools.h>
#include <format>

#include <QDebug>
#include <QTextBrowser>

Editor::Editor(QWidget *parent) : QMainWindow(parent), ui(new Ui::Editor)
{
    ui->setupUi(this);

    // m_currentline_clr = QString:: QPalette().highlightedText().color();
    // QPalette().HighlightedText;
    //
    const auto palette = QGuiApplication::palette();

    qDebug() << palette.highlightedText().color();
    qDebug() << palette.text().color().name();


    m_palette.text = palette.text().color().name().toStdString();
    m_palette.highlighted_background = palette.highlight().color().name().toStdString();

    qDebug() << QString::fromStdString(std::format("{}", m_palette));
    // connect(this, ui.
    // connect(ui->textInput, &QTextBrowser::textChanged,
    // this, &Editor::input_text);

    connect(ui->buttonCompile, &QPushButton::clicked, this, &Editor::compile);
}

Editor::~Editor() { delete ui; }

void Editor::input_text() { ui->codeView->document()->setHtml(ui->textInput->document()->toRawText()); }

void Editor::compile()
{
    IntComputer program{ ui->textInput->document()->toRawText().toStdString() };

    ui->codeView->document()->setHtml(QString::fromStdString(program.print()));
}

void Editor::run() {}
