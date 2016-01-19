/**
* @file loginDialog.cpp
* @brief 登陆界面类实现
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-09-14
*/

#include "logindialog.h"
#include "ui_logindialog.h"
#include "profile.h"
#include "global.h"
#include "wbapp.h"
#include <QtSql/QSqlQuery>

/**
* @brief 构造函数
* @param [I] 父指针
*/
LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);

	QSqlQuery q(profile.database());
	if (!q.exec("select name from user where hide <> 1"))
//		TRACE_LAST_ERROR(q);
        qDebug() << "error login";

	while(q.next())
		ui->name->addItem(q.value(0).toString());

	int i = ui->name->findText(profile.value("user/last").toString());
	ui->name->setCurrentIndex(qMax(0, i));
	ui->name->lineEdit()->selectAll();

#if defined(NX_NO_SIM)
	ui->sim->hide();
#endif
}

/**
* @brief 析构函数
*/
LoginDialog::~LoginDialog()
{
    delete ui;
}

/**
* @brief 槽函数
*/
void LoginDialog::accept()
{
	QSqlQuery q(profile.database());
	q.exec(QString("select password, level, hide from user where name = '%1'").arg(ui->name->currentText()));
	if (q.next())
	{
		if (q.value(0).toString() == ui->password->text())
		{
#ifndef NX_NO_SIM
			qApp->setTmpValue("debug/sim", ui->sim->isChecked());
#endif
			qApp->setTmpValue("user/name", ui->name->currentText());
			qApp->setTmpValue("user/level", q.value(1).toInt());
			if (!q.value(2).toBool())
				profile.setValue("user/last", ui->name->currentText());
			QDialog::accept();
		}
		else
			information(tr("密码错误!"));
	}
	else
		information(tr("用户名错误!"));
}
