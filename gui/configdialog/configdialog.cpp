/* BEGIN_COMMON_COPYRIGHT_HEADER
 * (c)LGPL2+
 *
 *
 * Copyright: 2012-2014 Boomaga team https://github.com/Boomaga
 * Authors:
 *   Alexander Sokoloff <sokoloff.a@gmail.com>
 *
 * This program or library is free software; you can redistribute it
 * and/or modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301 USA
 *
 * END_COMMON_COPYRIGHT_HEADER */


#include "configdialog.h"
#include "ui_configdialog.h"
#include "../settings.h"
#include <QDebug>


/************************************************

 ************************************************/
ConfigDialog *ConfigDialog::createAndShow(QWidget *parent)
{
    ConfigDialog *instance = parent->findChild<ConfigDialog*>();

    if (!instance)
        instance = new ConfigDialog(parent);

    instance->loadSettings();
    instance->show();
    instance->raise();
    instance->activateWindow();
    instance->setAttribute(Qt::WA_DeleteOnClose);

    return instance;
}


/************************************************

 ************************************************/
ConfigDialog::ConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigDialog)
{
    ui->setupUi(this);

    int width = settings->value("ConfigureDialog/Width").toInt();
    int height = settings->value("ConfigureDialog/Height").toInt();
    resize(width, height);

    connect(ui->bookletGroupBox, SIGNAL(clicked(bool)),
            this, SLOT(bookletGroupBoxClicked(bool)));
}


/************************************************

 ************************************************/
ConfigDialog::~ConfigDialog()
{
    delete ui;
}


/************************************************

 ************************************************/
void ConfigDialog::done(int res)
{
    settings->setValue("ConfigureDialog/Width",  size().width());
    settings->setValue("ConfigureDialog/Height", size().height());

    if (res)
        saveSettings();

    QDialog::done(res);
}


/************************************************

 ************************************************/
void ConfigDialog::bookletGroupBoxClicked(bool value)
{
    foreach (QWidget *widget, ui->bookletGroupBox->findChildren<QWidget*>())
    {
        widget->setEnabled(value);
    }
}


/************************************************

 ************************************************/
void ConfigDialog::loadSettings()
{
    ui->bookletGroupBox->setChecked(settings->value(Settings::SubBookletsEnabled).toBool());
    ui->bookletSheetsPerPageSpinBox->setValue(settings->value(Settings::SubBookletSize).toInt());
}


/************************************************

 ************************************************/
void ConfigDialog::saveSettings()
{
    bool changed = false;
    if (settings->value(Settings::SubBookletsEnabled).toBool() != ui->bookletGroupBox->isChecked())
    {
        changed = true;
        settings->setValue(Settings::SubBookletsEnabled, ui->bookletGroupBox->isChecked());
    }

    if (settings->value(Settings::SubBookletSize).toInt() != ui->bookletSheetsPerPageSpinBox->value())
    {
        changed = true;
        settings->setValue(Settings::SubBookletSize, ui->bookletSheetsPerPageSpinBox->value());
    }

    if (changed)
        project->update();
}
