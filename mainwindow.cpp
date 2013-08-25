#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    filename = QFileDialog::getOpenFileName(this,
                                          tr("Open Image File"), ".",
                                          tr("Image Files (*.jpg *.png *.bmp *.jpeg *.tiff *.tif *.dib *.jp2 *.jpe *.ppm *.pgm *.pbm *.ras *.sr)"));
    if (!filename.isEmpty()){

        // now load the image
        image = imread(filename.toStdString(), CV_LOAD_IMAGE_COLOR);   // Read the file

        if(! image.data )                              // Check for invalid input
        {
            QMessageBox msgBox;
            msgBox.setText("The selected image could not be opened!");
            msgBox.show();
            msgBox.exec();
        }
        else // If image file is fine. Show it in the label pixmap
        {
            QImage qigm=MatToQImage(image);
            ui->label->setPixmap(QPixmap::fromImage(qigm).scaledToWidth(ui->label->size().width(),Qt::FastTransformation));
        }
    }
}

QImage MainWindow::MatToQImage(const Mat& mat)
{
    // 8-bits unsigned, NO. OF CHANNELS=1
    if(mat.type()==CV_8UC1)
    {
        // Set the color table (used to translate colour indexes to qRgb values)
        QVector<QRgb> colorTable;
        for (int i=0; i<256; i++)
            colorTable.push_back(qRgb(i,i,i));
        // Copy input Mat
        const uchar *qImageBuffer = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage img(qImageBuffer, mat.cols, mat.rows, mat.step, QImage::Format_Indexed8);
        img.setColorTable(colorTable);
        return img;
    }
    // 8-bits unsigned, NO. OF CHANNELS=3
    else if(mat.type()==CV_8UC3)
    {
        // Copy input Mat
        const uchar *qImageBuffer = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage img(qImageBuffer, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        return img.rgbSwapped();
    }
    else
    {
        //qDebug() << "ERROR: Mat could not be converted to QImage.";
        return QImage();
    }
}

void MainWindow::on_pushButton_2_clicked() // canny edge detection part
{
    if (!filename.isEmpty())
    {
        if(image.data )                              // Check for invalid input
        {
            Mat src_gray;
            Mat detected_edges;

//            int edgeThresh = 1;
            int lowThreshold = 40;
//            int const max_lowThreshold = 100;
            int ratio = 3;
            int kernel_size = 3;

            /// Convert the image to grayscale
            cvtColor( image, src_gray, CV_BGR2GRAY );

            /// Reduce noise with a kernel 3x3
            blur( src_gray, detected_edges, Size(3,3) );

            /// Canny detector
            Canny( detected_edges, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size );

            QImage qigm=MatToQImage(detected_edges);
            image=detected_edges.clone();
            ui->label->setPixmap(QPixmap::fromImage(qigm).scaledToWidth(ui->label->size().width(),Qt::FastTransformation));
        }
    }
}

void MainWindow::on_pushButton_3_clicked()
{
    if (!filename.isEmpty())
    {
        if(image.data )                              // Check for invalid input
        {
            QString fname = QFileDialog::getSaveFileName(this,tr("Save Image File"), "", tr("Image Files (*.jpg *.png *.bmp *.jpeg *.tiff *.tif *.dib *.jp2 *.jpe *.ppm *.pgm *.pbm *.ras *.sr)"));
            imwrite( fname.toStdString(), image );
        }
    }
}
