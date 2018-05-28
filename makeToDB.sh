#!/bin/bash
g++ -g -o toDB src/inputToDB_main.cpp src/cvtstring.cpp src/db.cpp src/producer.cpp src/student.cpp -Lseetalib/lib -lseeta_fa_lib -lseeta_facedet_lib -lviplnet -L/usr/local/lib/ -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_imgcodecs -lmysqlclient -std=c++11
