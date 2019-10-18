//
//
#ifndef SGE_EDITOR_NEWPROJECTPAGES_HPP
#define SGE_EDITOR_NEWPROJECTPAGES_HPP

#include <QDialog>
#include <QDialogButtonBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QString>
#include <QDir>

#include <sge/editor/common.hpp>

SGE_EDITOR_BEGIN

class NewProjectDialog: public QDialog {
	Q_OBJECT

public:
	NewProjectDialog(QWidget *parent = Q_NULLPTR);
	virtual ~NewProjectDialog(void);

public:
	const QString &name(void) const;
	const QDir &dir(void) const;
	const QString &path(void) const;

protected slots:
	void browse(void);
	void nameChanged(const QString &text);
	void pathChanged(const QString &text);
	void accept(void) override;

private:
	QVBoxLayout m_layout;

	QHBoxLayout m_nameBox;
	QLabel m_nameLabel;
	QLineEdit m_nameEdit;

	QHBoxLayout m_pathBox;
	QLabel m_pathLabel;
	QLineEdit m_pathEdit;
	QPushButton m_browseButton;

	QHBoxLayout m_projectPathBox;
	QLabel m_projectPathLabel;
	QLineEdit m_projectPathEdit;

	QDialogButtonBox m_buttonBox;

	QString m_name;
	QDir m_dir;
	QString m_path;
};

inline const QString &NewProjectDialog::name(void) const
{
	return m_name;
}

inline const QDir &NewProjectDialog::dir(void) const
{
	return m_dir;
}

inline const QString &NewProjectDialog::path(void) const
{
	return m_path;
}

SGE_EDITOR_END

#endif // SGE_EDITOR_NEWPROJECTDIALOG_HPP
