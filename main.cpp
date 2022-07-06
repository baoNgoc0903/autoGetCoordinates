#include <QTextStream>
#include <QFile>
#include <QList>
#include <QDebug>

typedef struct toado
{
    double px;
    double py;
    double pz;
    double ox;
    double oy;
    double oz;
    double ow;

    void setCoordinates(int type, int stt, double value){
        switch (type) {
        case 0: // position
//            qDebug() << "position";
            switch(stt){
                case 0: px = value; break;
                case 1: py = value; break;
                case 2: pz = value; break;
            }
            break;
        case 1: // orientation
//            qDebug() << "orientation";
            switch(stt){
                case 0: ox = value; break;
                case 1: oy = value; break;
                case 2: oz = value; break;
                case 3: ow = value; break;
            }
            break;
        }
    }
}toado;
int main(int argc, char *argv[])
{
    QFile file("/home/chaelisa/coffee-qmake-again/autoGetCoordinates/toado.txt");
    QTextStream in(&file);
    system("cd /home/chaelisa/coffee-qmake-again/autoGetCoordinates && touch savetoado.txt");
    QFile fileSave("/home/chaelisa/coffee-qmake-again/autoGetCoordinates/savetoado.txt");
    QTextStream out(&fileSave);

    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Cannot open file for reading";
        return 1;
    }

    if (!fileSave.open(QIODevice::WriteOnly)) {
        qDebug() << "Cannot open file for reading";
        return 1;
    }

    toado td;
    QString readData;

    readData = in.readAll(); // đọc all data

    QStringList allCoordinates = readData.split("---");
    QStringList eachCoordinates;
    QStringList eachCoordinates_first;

    for(int i = 0; i<allCoordinates.count()-1; i++){
        eachCoordinates = allCoordinates[i].split("  position: ");
        eachCoordinates_first = eachCoordinates[1].split("  orientation: ");

        eachCoordinates_first[0] = eachCoordinates_first[0].replace("\n    ","|");
        eachCoordinates_first[1] = eachCoordinates_first[1].replace("\n    ","|");

        eachCoordinates_first[0].remove(0,1);
        eachCoordinates_first[1].remove(0,1);

        eachCoordinates_first[0] = eachCoordinates_first[0].replace("\n","");
        eachCoordinates_first[1] = eachCoordinates_first[1].replace("\n","");
        eachCoordinates_first[0] = eachCoordinates_first[0].replace(" ","");
        eachCoordinates_first[1] = eachCoordinates_first[1].replace(" ","");

        QStringList datas_position =  eachCoordinates_first[0].split("|");
        QStringList datas_orientation =  eachCoordinates_first[1].split("|");

        for(int i = 0; i < datas_position.count(); i++){
//            qDebug() << datas_position[i];
            QStringList detailxyz = datas_position[i].split(":");
            out << detailxyz[1].toDouble() << ":";
            td.setCoordinates(0, i, detailxyz[1].toDouble());
        }

        for(int i = 0; i < datas_orientation.count(); i++){
//            qDebug() << datas_orientation[i];
            QStringList detailxyzw = datas_orientation[i].split(":");
            out << detailxyzw[1].toDouble();
            if(i != datas_orientation.count()-1)
                out << ":";
            td.setCoordinates(1, i, detailxyzw[1].toDouble());
        }

        if(i != allCoordinates.count()-2)
            out << "|";

//        qDebug() << eachCoordinates_first[0];
//        qDebug() << eachCoordinates_first[1];
    }

    file.close();
    fileSave.close();
    return 0;
}
