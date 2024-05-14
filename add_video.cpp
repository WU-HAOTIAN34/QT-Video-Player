#include "add_video.h"

#include <QtWidgets/QFileDialog>
#include <QUrl>
#include <QtDebug>

#include <string>
#include <fstream>
#include <iostream>

void AddVideo::clicked() {
    QString result = QFileDialog::getOpenFileName(
            this, "Choose Video", "", "Video Files (*.mp4, *.mov, *.wmv)");
    std::string filepath = result.toStdString();

    // tell the player to add the video
    emit addVideo(filepath);
}
