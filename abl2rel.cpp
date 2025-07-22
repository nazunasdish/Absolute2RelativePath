#include<QApplication>
#include<QWidget>
#include<QPushButton>
#include<QVBoxLayout>
#include<QHBoxLayout>
#include<QLineEdit>
#include<QLabel>
#include<QFileDialog>
#include<QFile>
#include<QTextStream>
#include<QSpinBox>
#include<QMessageBox>
#include<QPlainTextEdit>
#include<QIcon>
#include<QDir>
#include "resource.h"
class M3UEditor : public QWidget {
    Q_OBJECT

public:
    M3UEditor(QWidget *parent = nullptr) : QWidget(parent) {
        setWindowTitle("Nazuna M3U Editor 😎");
        QIcon icon("icon.ico");
        if (!icon.isNull()) {
            setWindowIcon(icon);
        }
        resize(800, 600);
        setMinimumSize(600, 400);

        auto *mainLayout = new QVBoxLayout(this);

        auto *fileLayout = new QHBoxLayout();
        filePathEdit = new QLineEdit(this);
        filePathEdit->setReadOnly(true);
        auto *browseBtn = new QPushButton("Chọn file", this);
        fileLayout->addWidget(filePathEdit);
        fileLayout->addWidget(browseBtn);

        auto *spinLayout = new QHBoxLayout();
        auto *spinLabel = new QLabel("Số ký tự cần xóa:", this);
        spinBox = new QSpinBox(this);
        spinBox->setRange(0, 999);
        auto *previewBtn = new QPushButton("Xem trước", this);
        spinLayout->addWidget(spinLabel);
        spinLayout->addWidget(spinBox);
        spinLayout->addWidget(previewBtn);

        auto *previewLayout = new QHBoxLayout();
        inputPreview = new QPlainTextEdit(this);
        outputPreview = new QPlainTextEdit(this);
        inputPreview->setReadOnly(true);
        outputPreview->setReadOnly(true);
        inputPreview->setPlaceholderText("Nội dung file gốc...");
        outputPreview->setPlaceholderText("Sau khi đã xóa ký tự đầu dòng...");
        previewLayout->addWidget(inputPreview);
        previewLayout->addWidget(outputPreview);

        auto *exportBtn = new QPushButton("Xuất 😝", this);

        mainLayout->addLayout(fileLayout);
        mainLayout->addLayout(spinLayout);
        mainLayout->addLayout(previewLayout);
        mainLayout->addWidget(exportBtn);
        
        connect(browseBtn, &QPushButton::clicked, this, &M3UEditor::chooseFile);
        connect(previewBtn, &QPushButton::clicked, this, &M3UEditor::updatePreview);
        connect(exportBtn, &QPushButton::clicked, this, &M3UEditor::exportFile);
    }

private slots:
    void chooseFile() {
        QString fileName = QFileDialog::getOpenFileName(this, "Chọn file M3U", "", "M3U Files (*.m3u *.m3u8);;All Files (*)");
        if (!fileName.isEmpty()) {
            filePathEdit->setText(fileName);
            updatePreview();
        }
    }

    void updatePreview() {
        QString inputPath = filePathEdit->text();
        if (inputPath.isEmpty()) return;

        QFile inFile(inputPath);
        if (!inFile.open(QIODevice::ReadOnly | QIODevice::Text)) return;

        QTextStream in(&inFile);
        int charsToRemove = spinBox->value();
        QString inputText, outputText;

        while (!in.atEnd()) {
            QString line = in.readLine();
            inputText += line + "\n";
            outputText += line.mid(charsToRemove) + "\n";
        }

        inputPreview->setPlainText(inputText);
        outputPreview->setPlainText(outputText);
        inFile.close();
    }

    void exportFile() {
        QString outputText = outputPreview->toPlainText();
        if (outputText.isEmpty()) {
            QMessageBox::warning(this, "Lỗi", "Chưa có nội dung để xuất.");
            return;
        }

        QString savePath = QFileDialog::getSaveFileName(this, "Lưu file mới", "", "M3U Files (*.m3u *.m3u8);;All Files (*)");
        if (savePath.isEmpty()) return;

        QFile outFile(savePath);
        if (!outFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QMessageBox::critical(this, "Lỗi", "Không ghi được file.");
            return;
        }

        QTextStream out(&outFile);
        out << outputText;
        outFile.close();

        QMessageBox::information(this, "Xong rồi", "Đã xuất file thành công 😎");
    }

private:
    QLineEdit *filePathEdit;
    QSpinBox *spinBox;
    QPlainTextEdit *inputPreview;
    QPlainTextEdit *outputPreview;
};

#include "abl2rel.moc"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QDir::setCurrent(QCoreApplication::applicationDirPath());

    M3UEditor editor;
    editor.show();
    return app.exec();
}
