//
//
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>

#include "newprojectdialog.hpp"

NewProjectDialog::NewProjectDialog(QWidget *parent)
	: QDialog(parent)
	, m_dir(QDir::homePath())
{
	setWindowTitle("New Project");

	m_nameLabel.setText("Name:");
	m_nameBox.addWidget(&m_nameLabel);

	connect(&m_nameEdit, &QLineEdit::textChanged, this, &NewProjectDialog::nameChanged);
	m_nameBox.addWidget(&m_nameEdit);

	m_layout.addLayout(&m_nameBox);

	m_pathLabel.setText("Path:");
	m_pathBox.addWidget(&m_pathLabel);

	m_pathEdit.setMinimumWidth(500);
	m_pathEdit.setText(m_dir.path());
	connect(&m_pathEdit, &QLineEdit::textChanged, this, &NewProjectDialog::pathChanged);

	m_pathBox.addWidget(&m_pathEdit);

	m_browseButton.setText("Browse...");
	connect(&m_browseButton, &QPushButton::released, this, &NewProjectDialog::browse);
	m_pathBox.addWidget(&m_browseButton);
	m_layout.addLayout(&m_pathBox);

	m_projectPathLabel.setText("Project Path:");
	m_projectPathBox.addWidget(&m_projectPathLabel);
	m_projectPathEdit.setReadOnly(true);
	m_projectPathEdit.setText(m_dir.path());
	m_projectPathBox.addWidget(&m_projectPathEdit);
	m_projectPathBox.setAlignment(Qt::AlignLeft);
	m_layout.addLayout(&m_projectPathBox);

	m_buttonBox.addButton(QDialogButtonBox::Ok);
	m_buttonBox.addButton(QDialogButtonBox::Cancel);
	connect(&m_buttonBox, &QDialogButtonBox::accepted, this, &NewProjectDialog::accept);
	connect(&m_buttonBox, &QDialogButtonBox::rejected, this, &NewProjectDialog::reject);
	m_layout.addWidget(&m_buttonBox);

	setLayout(&m_layout);
}

NewProjectDialog::~NewProjectDialog(void)
{
}

void NewProjectDialog::browse(void)
{
	QString path = QFileDialog::getExistingDirectory(this, "Select Project Directory", path);
	m_dir.setPath(path);
	m_pathEdit.setText(path);
}

void NewProjectDialog::nameChanged(const QString &text)
{
	m_name = text;
	m_path = QDir::cleanPath(m_dir.path() + "/" + m_name);
	m_projectPathEdit.setText(m_path);
}

void NewProjectDialog::pathChanged(const QString &text)
{
	m_dir.setPath(text);
	m_path = QDir::cleanPath(m_dir.path() + "/" + m_name);
	m_projectPathEdit.setText(m_path);
}

void NewProjectDialog::accept(void)
{
	if (m_name.isEmpty()) {
		QMessageBox::warning(this, "Waring", "Empty project name.");
		return;
	}

	//if (!m_name.contains("^[\\w\\d]+$")) {
	//	QMessageBox::warning(this, "Waring", "Invalid project name.");
	//	return;
	//}

	if (!m_dir.exists()) {
		QMessageBox::warning(this, "Warning", "Invalid project path: " + m_dir.path());
		return;
	}

	QDialog::accept();
}
