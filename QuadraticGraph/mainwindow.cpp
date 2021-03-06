#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <cmath>
#include <QMessageBox>
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //Setup the UI
    ui->setupUi(this);
    ui->customPlot->addGraph();
    //Set labels for the x and y axis
    ui->customPlot->xAxis->setLabel("𝓍");
    ui->customPlot->yAxis->setLabel("𝓎");
    //Set the initial range on the graph
    MainWindow::setRange(10);

}

MainWindow::~MainWindow()
{
    //Destructor
    delete ui;
}

void MainWindow::setRange(const double& xyrange) {
    //Set the range to the positive and negative values of the value passed to the function
    ui->customPlot->xAxis->setRange(-xyrange, xyrange);
    ui->customPlot->yAxis->setRange(-xyrange, xyrange);
    //Set a global variable to the range the graph has
    xyrangeAt=xyrange;
}
void MainWindow::plot(void) {
    //Clear old data, and calculate new equation and vertex
    clear();
    setEquation();
    //Get the numbers the user inputted with the UI
    const double a = ui->valueA->QDoubleSpinBox::value();
    const double b = ui->valueB->QDoubleSpinBox::value();
    const double c = ui->valueC->QDoubleSpinBox::value();
    //Iterate through the vector and assign the points of the curve
    const double iterations = xyrangeAt*100; //For proper disaply, the number of points generated is range times 100
    for (int i=0; i < iterations; ++i) //Recursively generate x and y pairs
    {
        if (i == 0){ //Because the we access index i-1, we need to set and initial value to push the index to 1
            x.append(-xyrangeAt);
            y.append(-xyrangeAt);
            std::cerr << "Index: " << i << " X and Y pair: " << x[i] << " , " << y[i] << std::endl; //Debug, outputs generated x y pair
        }else{
            x.append((x[i-1] + 0.02)); //Next x coordinate is 0.02 more than the previous
            y.append((((a*std::pow(-x[i], 2)))-(b*-x[i])+c)); // y=ax^2-bx+c
            std::cerr << "Index: " << i << " X and Y pair: " << x[i] << " , " << y[i] << std::endl; //Debug, outputs generated x y pair
        }

    }
    //Assign the vector to the graph's data and plot it
    ui->customPlot->graph(0)->setData(x, y);
    ui->customPlot->replot();
    setVertex();
}

void MainWindow::clear(void) {
    //Clear data container
    x.clear();
    y.clear();
    //Replot with the empty data container
    ui->customPlot->graph(0)->setData(x, y);
    ui->customPlot->replot();
    //Set the text on the equation to nothing
    ui->equationData->setText(nullptr);
    ui->vertexData->setText(nullptr);
}


void MainWindow::setEquation(void) {
    //Get the data the user entered into the UI
    const QString a = QString::number(ui->valueA->QDoubleSpinBox::value());
    const QString b = QString::number(ui->valueB->QDoubleSpinBox::value());
    const QString c = QString::number(ui->valueC->QDoubleSpinBox::value());
    //Format it into a "template" equation
    const QString equation = "𝓎=" + a + "𝓍²+" + b +"𝓍+" + c;
    //Set the text of the equation label to the formatted string
    ui->equationData->setText(equation);
}


const QString MainWindow::getVertex(void) {
    //Get the values the user inputs
    const double a = ui->valueA->QDoubleSpinBox::value();
    const double b = ui->valueB->QDoubleSpinBox::value();
    const double c = ui->valueC->QDoubleSpinBox::value();
    //Calculate x and y
    const double x = (-b/(-2*a)); //x=-b/2a
    const double y = a*std::pow(x, 2)-(b*x)+c; //y=ax^2-bx+c
    //Format to string
    const QString result = QString::number(-x) + " , " + QString::number(y);
    return result;
}

void MainWindow::setVertex(void) {
    //Get the vertex and then set the label text
    const QString vertex = getVertex();
    ui->vertexData->setText(vertex);
}

void MainWindow::invertX() {
    //Invert the "B" field and replot
    ui->valueB->setValue(-(ui->valueB->QDoubleSpinBox::value()));
    plot();
}

void MainWindow::invertY() {
    //Invert the "A" field and replot
    ui->valueA->setValue(-(ui->valueA->QDoubleSpinBox::value()));
    plot();
}

void MainWindow::on_invertYbtn_clicked()
{
    if (!x.empty() || !y.empty()){ //Only flip if there is data on the graph
        invertY();
    }
}

void MainWindow::on_invertXbtn_clicked()
{
    if (!x.empty() || !y.empty()){ //Only flip if there is data on the graph
        invertX();
    }
}


//Replot whenever any user input field changed

void MainWindow::on_valueRange_valueChanged(double arg1)
{
    setRange(arg1);
    clear();
    plot();
}

void MainWindow::on_valueA_valueChanged(double arg1)
{
    clear();
    plot();
}

void MainWindow::on_valueB_valueChanged(double arg1)
{
    clear();
    plot();
}

void MainWindow::on_valueC_valueChanged(double arg1)
{
    clear();
    plot();
}
