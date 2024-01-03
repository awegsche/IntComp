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


    palette_.text = palette.text().color().name().toStdString();
    palette_.highlighted_background = palette.highlight().color().name().toStdString();

    qDebug() << QString::fromStdString(std::format("{}", palette_));
    // connect(this, ui.
    // connect(ui->textInput, &QTextBrowser::textChanged,
    // this, &Editor::input_text);

    connect(ui->buttonCompile, &QPushButton::clicked, this, &Editor::compile);
    connect(ui->buttonRun, &QPushButton::clicked, this, &Editor::run);
    connect(ui->buttonStep, &QPushButton::clicked, this, &Editor::step);
}

Editor::~Editor() { delete ui; }

void Editor::input_text() { ui->codeView->document()->setHtml(ui->textInput->document()->toRawText()); }

void Editor::step_program()
{
    if (computer_.get_state() == State::INPUT) {
        const CodeType input = ui->terminalIn->text().toLongLong();
        ui->terminalIn->setText("");
        computer_.set_input(input);
    }
    computer_.step();

    switch (computer_.get_state()) {
    case State::OUTPUT:
        ui->terminalOut->append(QString::number(computer_.get_output()));
        break;
    case State::ATEND:
        ui->terminalOut->append("END");
        break;
    case State::INPUT:
        ui->terminalOut->append(">");
        break;
    case State::RUN:
        // this is the default case, just continue
        break;
    default:
        ui->terminalOut->append("ERROR");
        break;
    }
}

void Editor::compile()
{
    computer_ = IntComputer{ ui->textInput->document()->toRawText().toStdString() };
    computer_.set_color_palette(palette_);

    ui->terminalOut->setText("");
    ui->codeView->document()->setHtml(QString::fromStdString(computer_.print()));
}

void Editor::run()
{
    do {
        step_program();
    } while (computer_.get_state() != State::INPUT && computer_.get_state() != State::ATEND);
}

void Editor::step()
{
    step_program();
    ui->codeView->document()->setHtml(QString::fromStdString(computer_.print()));
}
