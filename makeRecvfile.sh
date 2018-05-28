#!/bin/bash
g++ -o recv -g src/recvfile_main.cpp src/cvtstring.cpp src/db.cpp src/producer.cpp src/student.cpp -Lseetalib/lib/ -lseeta_fa_lib -lseeta_facedet_lib -lviplnet -L/usr/local/lib/ -lopencv_core -lopencv_highgui -lopencv_imgproc -L/usr/lib64/mysql -lmysqlclient -std=c++11 -lopencv_imgcodecs 
