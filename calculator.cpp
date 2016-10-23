#include "calculator.h"
#include "button.h"
using namespace std;

Calculator::Calculator(QWidget *parent) :
    QWidget(parent)
{
    QGridLayout *mainLayout = new QGridLayout;
    QGridLayout *tableLayout = new QGridLayout;
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);

    matrixList = new QListWidget();
    display = new QTextBrowser();
    matrixTable = new QTextBrowser();
    matrixList->addItem("haha");
    mainLayout->addWidget(display,0,0,1,2);
    mainLayout->addLayout(tableLayout,1,1,1,1);
    mainLayout->addWidget(matrixList,1,0,1,1);
    mainLayout->addWidget(matrixTable,1,1,1,1);
    setLayout(mainLayout);
    setFocusPolicy(Qt::StrongFocus);
    matrixList->installEventFilter(this);
    display->installEventFilter(this);
    QFont qf;
    qf.setFamily("Consolas");
    display->setFont(qf);
    connect(matrixList,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(matrixListItemEdit(QListWidgetItem*)));
    connect(display,SIGNAL(highlighted(QString)),this,SLOT(haha()));
    display->insertPlainText(">>");
    inputMode = 1;
    //uielement
    writeLog("Init Complete...");
}

bool Calculator::eventFilter(QObject *object, QEvent *event)
{
    if(event->type() == QEvent::KeyPress) {
        const QKeyEvent *key = static_cast<QKeyEvent *>(event);
        if (key->key() == Qt::Key_Backspace)
        {
            QString cc=display->toPlainText();
            if (currentLine.size())
            {
                cc=cc.left(cc.size()-1);
                currentLine=currentLine.left(currentLine.size()-1);
                display->setText(cc);
            }
            return true;
        }else if (key->key() == Qt::Key_Space)
        {
            display->setText(display->toPlainText()+key->text());
            currentLine+=" ";
            return true;
        }else if (key->key() == Qt::Key_Return || key->key() == Qt::Key_Enter)
        {
            finishLine();
            display->setText(display->toPlainText()+"\n"+(inputMode==1?">>":"..."));
            currentLine="";
            return true;
        }else
        {
            display->setText(display->toPlainText()+key->text());
            currentLine+=key->text();
            return true;
        }
        return QWidget::eventFilter(object,event);
    }
    return QWidget::eventFilter(object, event);
}


void Calculator::keyPressEvent(QKeyEvent  *event)
{

}

void Calculator::matrixListItemEdit(QListWidgetItem* item)
{
    Matrix *ptr;
    writeLog("Item Selected:"+item->text());
    QString mName=item->text().split("=")[0];
    map<QString,Matrix> :: iterator it=data.find(mName);
    if (it==data.end())
    {
        cout<<"Cannot find matrix <"<<mName.toStdString()<<">"<<endl;
        return ;
    }
    ptr = &(it->second);
    matrixTable->setHtml(QString::fromStdString(ptr -> toHTML()));
}

Calculator::~Calculator()
{
}

Button* Calculator::createButton(QString text,const char* event)
{
    Button * ret = new Button(text,this);
    connect(ret,SIGNAL(clicked()),this,event);
    return ret;
}

void Calculator::numbersKeyDown()
{
    //    Button *clickedButton = qobject_cast<Button *>(sender());
    Button* wsender = qobject_cast<Button*>(sender());
    writeLog("Click on button:"+wsender->text());
}

void Calculator::writeLog(QString log,QString color)
{
    std::cout<<log.toStdString()<<std::endl;
    return ;
    display->append("<span style='color:"+color+"'> LOG>>>"+log+"</span>");
    display->scroll(0,1);
}
void Calculator::finishLine()
{
    currentLine=currentLine.simplified();
    if (inputMode==1)
    {
        if (currentLine.left(7)=="matrix " || currentLine.left(5)=="calc ")
        {
            if (currentLine[0]=='m')
                matrixName=currentLine.right(currentLine.size()-7);
            else
                matrixName=currentLine.right(currentLine.size()-5);
            cout<<"Matrix Name:"<<matrixName.toStdString()<<endl;
            if (currentLine[0]=='m')
                inputMode=2;
            else
                inputMode=3;
        }else if (currentLine.left(7)=="import ")
        {
            QString fname = currentLine.split(" ")[1];
            ifstream fin(fname.toStdString());
            if (!fin)
                cout<<"No such file"<<endl;
            string mname;
            while (getline(fin,mname))
            {
                if (mname.size()==0)break;
                fin>>data[QString::fromStdString(mname)];
            }
            matrixListRefresh();
        }else if (currentLine.left(6)=="value ")
        {
            QStringList vars=currentLine.split(" ");
            if (vars.size()==3)
            {
                vdata[vars[1]]=Fraction(vars[2].toStdString());
            }
        }else if (currentLine.left(4)=="exit")
        {
            QApplication::exit();
        }else if (data.find(currentLine)!=data.end())
        {
            matrixTable->setHtml(QString::fromStdString(data[currentLine].toHTML()));
        }
    }else if (inputMode == 2)
    {
        if (currentLine.size()==0)
        {
            inputMode=1;
            matrixListRefresh();
            return ;
        }else
        {
            data[matrixName].appendInput(currentLine.toStdString());
        }
    }
    if (inputMode == 3 || inputMode == 1)
    {
        if (currentLine.left(7)=="inverse")
        {
            QString tm;
            tm=currentLine.right(currentLine.size()-7);
            if (data.find(tm) == data.end())
                cout<<"Cannot find <"<<tm.toStdString()<<">"<<endl;
            try
            {
                Matrix a=data[tm].inverse();
                if (inputMode == 3)
                    data[matrixName]=a;
                else
                    matrixTable->setHtml(QString::fromStdString(a.toHTML()));
            }catch(UnableEliminateException)
            {
                cout<<"Cannot Eliminate!"<<endl;
            }
            matrixListRefresh();
            inputMode=1;
        }else if (currentLine.left(4)=="add " || currentLine.left(1)=="+")
        {
            QStringList vars=currentLine.split(" ");
            if (vars.size()<3)return ;
            if (data.find(vars[1])==data.end() || data.find(vars[2])==data.end())
            {
                cout<<"Wrong Matrix Name!"<<endl;
                return ;
            }
            try{
                Matrix a = data[vars[1]]+data[vars[2]];
                if (inputMode==3)
                    data[matrixName]=a;
                else
                    matrixTable->setHtml(QString::fromStdString(a.toHTML()));
            }catch(MatrixCalculationExceptions){
                cout<<"Error Calculate Multiplication"<<endl;
            }
            matrixListRefresh();
            inputMode=1;
        }
        else if (currentLine.left(9)=="multiply " || currentLine.left(1)=="*")
        {
            QStringList vars=currentLine.split(" ");
            if (vars.size()<3)return ;
            Matrix a;
            if (isFraction(vars[2].toStdString()) && !isFraction(vars[1].toStdString()))
                a=Fraction(vars[2].toStdString())*data[vars[1]];
            else if (isFraction(vars[1].toStdString()) && !isFraction(vars[2].toStdString()))
                a=Fraction(vars[1].toStdString())*data[vars[2]];
            else
                a=data[vars[1]]*data[vars[2]];
            if (inputMode==3)
                data[matrixName]=a;
            else
                matrixTable->setHtml(QString::fromStdString(a.toHTML()));
            matrixListRefresh();
            inputMode=1;
        }
    }
}

void Calculator::matrixListRefresh()
{
    matrixList->clear();
    for (map<QString,Matrix>::iterator it=data.begin();it!=data.end();it++)
    {

        matrixList->insertItem(1,QString(it->first)+QString("=")+QString::fromStdString(it->second.getAbstract(40)));
    }
}

void Calculator::haha()
{
    writeLog("str");
}
