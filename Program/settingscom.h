#ifndef SETTINGSCOM_H
#define SETTINGSCOM_H

#include <QMainWindow>

namespace Ui {
class SettingsCOM;
}

class SettingsCOM : public QMainWindow
{
    Q_OBJECT

public:
    explicit SettingsCOM(QWidget *parent = 0);
    ~SettingsCOM();
    void applySettingsFromForm();

signals:

    void saveAndApplySettingsInForm();
private slots:


    void on_loadSettingsButton_clicked();

    void on_defaultSettingsButton_clicked();

    void on_applyButton_clicked();

    void on_checkBox_clicked();

private:
    Ui::SettingsCOM *ui;
};

#endif // SETTINGSCOM_H
