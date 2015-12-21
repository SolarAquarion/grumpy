//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2015

#include "../libcore/definitions.h"

#include <QMenu>
#include <QColorDialog>
#include "../libcore/autocompletionengine.h"
#include "../libcore/commandprocessor.h"
#include "../libcore/exception.h"
#include "../libcore/configuration.h"
#include "../libcore/core.h"
#include "../libcore/highlighter.h"
#include "../libcore/generic.h"
#include "preferenceswin.h"
#include "skin.h"
#include "ui_preferenceswin.h"
#include "corewrapper.h"
#include "grumpyconf.h"

using namespace GrumpyIRC;

PreferencesWin::PreferencesWin(QWidget *parent) : QDialog(parent), ui(new Ui::PreferencesWin)
{
    this->ui->setupUi(this);
    this->ui->leIdent->setText(CONF->GetIdent());
    this->ui->leNick->setText(CONF->GetNick());
    this->ui->leNickFix->setText(CONF->GetAlterNick());
    this->ui->lineEdit->setText(CONF->GetRawQuitMessage());
    this->ui->lineEdit_2->setText(CONF->GetName());
    this->ui->lineEdit_3->setText(QString::number(CONF->GetSplitMaxSize()));
    this->ui->checkBox->setChecked(CONF->GetIgnoreSSLProblems());
    this->ui->checkBoxSplitMs->setChecked(CONF->GetSplit());
    this->ui->plainTextEditAutorun->setPlainText(CONF->GetAutorun());
    this->ui->lineEdit_ChannelH->setText(CONF->GetChannelHeader());
    this->ui->lineEdit_LabeledH->setText(CONF->GetLabeledHeader());
    this->ui->lineEdit_StandardH->setText(CONF->GetStandardHeader());
    QString ignored;
    foreach (int numeric, CONF->IgnoredNums())
    {
        ignored += QString::number(numeric) + ", ";
    }
    ignored = ignored.trimmed();
    this->ui->lineEdit_4->setText(ignored);

    QStringList heading_1;
    heading_1 << "Highlighted text" << "Is regex" << "Matching" << "Enabled";
    this->ui->tableWidget->verticalHeader()->setVisible(false);
    this->ui->tableWidget->setColumnCount(heading_1.size());
    this->ui->tableWidget->setShowGrid(false);
    this->ui->tableWidget->setHorizontalHeaderLabels(heading_1);

    QStringList heading_2;
    heading_2 << "Ignored text" << "Is regex" << "Matching" << "Enabled";
    this->ui->tableWidget_2->verticalHeader()->setVisible(false);
    this->ui->tableWidget_2->setColumnCount(heading_2.size());
    this->ui->tableWidget_2->setShowGrid(false);
    this->ui->tableWidget_2->setHorizontalHeaderLabels(heading_2);

    this->highlights_reload();
    this->ui->tableWidget->resizeColumnsToContents();
    this->ui->tableWidget->resizeRowsToContents();
    this->ui->tableWidget_2->resizeColumnsToContents();
    this->updateSkin();
}

PreferencesWin::~PreferencesWin()
{
    delete this->ui;
}

void GrumpyIRC::PreferencesWin::on_buttonBox_rejected()
{
    this->close();
}

void GrumpyIRC::PreferencesWin::on_buttonBox_accepted()
{
    CONF->SetAlterNick(this->ui->leNickFix->text());
    CONF->SetNick(this->ui->leNick->text());
    CONF->SetIdent(this->ui->leIdent->text());
    CONF->SetQuitMessage(this->ui->lineEdit->text());
    CONF->SetName(this->ui->lineEdit_2->text());
    CONF->SetIgnoreSSLProblems(this->ui->checkBox->isChecked());
    CONF->SetSplitMaxSize(this->ui->lineEdit_3->text().toInt());
    CONF->SetSplit(this->ui->checkBoxSplitMs->isChecked());
    CONF->SetAutorun(this->ui->plainTextEditAutorun->toPlainText());
    CoreWrapper::GrumpyCore->GetCommandProcessor()->LongSize = CONF->GetSplitMaxSize();
    CoreWrapper::GrumpyCore->GetCommandProcessor()->SplitLong = CONF->GetSplit();
    CONF->SetChannelH(this->ui->lineEdit_ChannelH->text());
    CONF->SetLabeledH(this->ui->lineEdit_LabeledH->text());
    CONF->SetStandardH(this->ui->lineEdit_StandardH->text());
    QList<int> ignored_nums;
    QList<QString> ignored = this->ui->lineEdit_4->text().split(",");
    foreach (QString numeric, ignored)
    {
        numeric = numeric.trimmed();
        if (numeric.isEmpty())
            continue;
        ignored_nums.append(numeric.toInt());
    }
    Skin::Current = this->highlighted_skin;
    CONF->SetIRCIgnoredNumerics(ignored_nums);
    CONF->Save();
    this->close();
}

void PreferencesWin::highlights_reload()
{
    // This will leak until the window is closed, but that's most likely not a big problem
    this->highlights_enabled.clear();
    this->highlights_regex.clear();
    this->highlights_type.clear();
    int row = 0;
    foreach (Highlighter *hl, Highlighter::Highlighter_Data)
    {
        this->highlights_append_row(row++, hl);
    }
}

void PreferencesWin::highlights_append_row(int row, Highlighter *hl)
{
    this->ui->tableWidget->insertRow(row);
    QTableWidgetItem *item = new QTableWidgetItem(hl->GetDefinition());
    this->highlights_source.insert(item, hl);
    this->ui->tableWidget->setItem(row, 0, item);

    // Create a check box for regex item
    QCheckBox *regex = new QCheckBox(this);
    regex->setChecked(hl->IsRegex);
    this->highlights_regex.insert(regex, hl);
    this->ui->tableWidget->setCellWidget(row, 1, regex);
    connect(regex, SIGNAL(toggled(bool)), this, SLOT(OnHLRegex(bool)));

    // Combo box for highlight type
    QComboBox *hlt = new QComboBox(this);
    hlt->addItem("Text of message");
    hlt->setCurrentIndex(0);
    this->highlights_type.insert(hlt, hl);
    this->ui->tableWidget->setCellWidget(row, 2, hlt);

    QCheckBox *enabled = new QCheckBox(this);
    enabled->setChecked(hl->Enabled);
    this->highlights_enabled.insert(enabled, hl);
    connect(enabled, SIGNAL(toggled(bool)), this, SLOT(OnHLEnable(bool)));
    this->ui->tableWidget->setCellWidget(row, 3, enabled);
    this->ui->tableWidget->resizeRowToContents(row);
}

QList<int> PreferencesWin::selectedHLRows()
{
    QList<int> results;
    QList<QTableWidgetItem*> items = this->ui->tableWidget->selectedItems();
    foreach (QTableWidgetItem *xx, items)
    {
        if (!results.contains(xx->row()))
            results.append(xx->row());
    }

    return results;
}

void PreferencesWin::updateSkin()
{
    int selectedSkin = Skin::SkinList.indexOf(Skin::GetCurrent());
    if (this->ui->comboBox->count() > 0)
        selectedSkin = this->ui->comboBox->currentIndex();
    this->ui->comboBox->clear();
    foreach (Skin *skin, Skin::SkinList)
    {
        this->ui->comboBox->addItem(skin->Name);
    }
    while (selectedSkin >= this->ui->comboBox->count())
        selectedSkin -= 1;
    this->ui->comboBox->setCurrentIndex(selectedSkin);
}

void PreferencesWin::refreshSkin(bool enabled)
{
    this->ui->pushButton_4->setEnabled(enabled);
    this->ui->lineEdit_SkinFont->setEnabled(enabled);
    this->ui->lineEdit_SkinName->setEnabled(enabled);
    this->ui->pushButton_5->setEnabled(enabled);
    this->ui->lineEdit_SkinSz->setEnabled(enabled);
}

void PreferencesWin::on_tableWidget_cellChanged(int row, int column)
{
    QTableWidgetItem *item = this->ui->tableWidget->item(row, column);
    if (!this->highlights_source.contains(item))
        return;

    this->highlights_source[item]->SetDefinition(item->text());
}

void PreferencesWin::on_tableWidget_2_cellChanged(int row, int column)
{

}

void PreferencesWin::on_tableWidget_customContextMenuRequested(const QPoint &pos)
{
    QPoint globalPos = this->ui->tableWidget->viewport()->mapToGlobal(pos);
    QMenu Menu;
    // Items
    QAction *menuInsert = new QAction("Insert", &Menu);
    Menu.addAction(menuInsert);
    QAction *menuRemove = new QAction("Remove", &Menu);
    Menu.addAction(menuRemove);

    QAction* selectedItem = Menu.exec(globalPos);
    if (!selectedItem)
        return;
    if (selectedItem == menuInsert)
    {
        Highlighter *hx = new Highlighter("");
        hx->Enabled = false;
        this->highlights_append_row(this->ui->tableWidget->rowCount(), hx);
    }
    else if (selectedItem == menuRemove)
    {
        // We need to remove them from last to first
        QList<int> items = this->selectedHLRows();
        qSort(items);
        while (!items.isEmpty())
        {
            int row = items.last();
            items.removeLast();
            QTableWidgetItem *item = this->ui->tableWidget->item(row, 0);
            if (this->highlights_source.contains(item))
            {
                Highlighter *highlight_sel = this->highlights_source[item];
                // Remove the row
                this->ui->tableWidget->removeRow(row);
                // Nuke the highlight
                delete highlight_sel;
            }
        }
    }
}

void PreferencesWin::OnHLEnable(bool checked)
{
    QCheckBox *source = (QCheckBox*)QObject::sender();
    if (!this->highlights_enabled.contains(source))
        throw new Exception("Checkbox not found", BOOST_CURRENT_FUNCTION);

    this->highlights_enabled[source]->Enabled = checked;
}

static void UpdateButton(QPushButton *push, QColor color)
{
    QPalette temp = push->palette();
    QColor inverted;
    inverted.setBlue(255 - color.blue());
    inverted.setRed(255 - color.red());
    inverted.setGreen(255 - color.green());
    temp.setColor(QPalette::ButtonText, inverted);
    push->setStyleSheet("background-color: " + color.name() + ";");
    push->setPalette(temp);
}

void PreferencesWin::OnHLRegex(bool checked)
{
    QCheckBox *source = (QCheckBox*)QObject::sender();
    if (!this->highlights_regex.contains(source))
        throw new Exception("Checkbox not found", BOOST_CURRENT_FUNCTION);

    this->highlights_regex[source]->IsRegex = checked;
}

void GrumpyIRC::PreferencesWin::on_comboBox_currentIndexChanged(int index)
{
    if (index < 0)
        return;

    if (index >= Skin::SkinList.count())
        throw new Exception("Invalid skin_ptr", BOOST_CURRENT_FUNCTION);

    this->highlighted_skin = Skin::SkinList[index];
    this->ui->lineEdit_SkinFont->setText(this->highlighted_skin->TextFont.toString());
    this->ui->lineEdit_SkinSz->setText(QString::number(this->highlighted_skin->TextFont.pixelSize()));
    this->ui->lineEdit_SkinName->setText(this->highlighted_skin->Name);
    UpdateButton(this->ui->pushButton_4, this->highlighted_skin->BackgroundColor);
    UpdateButton(this->ui->pushButton_5, this->highlighted_skin->TextColor);
    UpdateButton(this->ui->pushButton_SC, this->highlighted_skin->SystemColor);
    this->refreshSkin(!this->highlighted_skin->IsDefault());
    //this->ui->lineEdit_SkinBk->setText(skin->BackgroundColor);
}

void GrumpyIRC::PreferencesWin::on_pushButton_clicked()
{
    if (this->highlighted_skin->IsDefault())
        return;

    this->highlighted_skin->Name = this->ui->lineEdit_SkinName->text();
    this->highlighted_skin->TextFont = QFont(this->ui->lineEdit_SkinFont->text());
    this->highlighted_skin->TextFont.setPixelSize(this->ui->lineEdit_SkinSz->text().toInt());
}

void GrumpyIRC::PreferencesWin::on_pushButton_3_clicked()
{
    if (this->highlighted_skin->IsDefault())
        return;

    delete this->highlighted_skin;
    this->highlighted_skin = NULL;
    this->updateSkin();
}

void GrumpyIRC::PreferencesWin::on_pushButton_2_clicked()
{
    new Skin(this->highlighted_skin);
    this->updateSkin();

    // Now that we created a new skin we can switch it
    this->ui->comboBox->setCurrentIndex(this->ui->comboBox->count() - 1);
}

static QColor GetColorUsingPicker(QColor color)
{
    QColor original = color;
    QColorDialog *picker = new QColorDialog(color, NULL);
    picker->exec();
    color = picker->selectedColor();
    delete picker;
    if (!color.isValid())
        return original;
    return color;
}

void GrumpyIRC::PreferencesWin::on_pushButton_4_clicked()
{
    if (!this->highlighted_skin)
        return;

    this->highlighted_skin->BackgroundColor = GetColorUsingPicker(this->highlighted_skin->BackgroundColor);
    UpdateButton(this->ui->pushButton_4, this->highlighted_skin->BackgroundColor);
}

void GrumpyIRC::PreferencesWin::on_pushButton_5_clicked()
{
    if (!this->highlighted_skin)
        return;

    this->highlighted_skin->TextColor = GetColorUsingPicker(this->highlighted_skin->TextColor);
    UpdateButton(this->ui->pushButton_5, this->highlighted_skin->TextColor);
}

void GrumpyIRC::PreferencesWin::on_pushButton_SC_clicked()
{
    if (!this->highlighted_skin)
        return;

    this->highlighted_skin->SystemColor = GetColorUsingPicker(this->highlighted_skin->SystemColor);
    UpdateButton(this->ui->pushButton_SC, this->highlighted_skin->SystemColor);
}
