#include "io.h"
#include <QFile>
#include <QTextStream>
IO::IO()
{

}

IO::~IO() {
}

void IO::open(const char *filename, QVector<QVector3D>& positions) {

    positions.clear();
    QFile inputFile(filename);
    if (inputFile.open(QIODevice::ReadOnly))
    {
       QTextStream in(&inputFile);
       int cnt = 0;

       while (!in.atEnd())
       {
          QString line = in.readLine();
          cnt++;
          if (cnt>=2) {
              QStringList lst = line.split(" ");
              QVector3D p(lst[1].toFloat(),lst[2].toFloat(),lst[3].toFloat());
              positions.append(p);


          }
       }
       inputFile.close();
    }

}




// This saves the current state to a file following the xyz-standard (see http://en.wikipedia.org/wiki/XYZ_file_format )
void IO::save(const char* filename, QVector<QVector3D>& positions) {
    ofstream file;

    file.open(filename,std::ofstream::out);

    if(file.is_open()) {
        file << positions.count() << endl;
        file << "The is an optional comment line that can be empty." << endl;
        for(QVector3D p : positions) {
            file << "Ar " << p.x() << " " << p.y() << " " << p.z() << endl;
        }
        file.close();

    }
}
