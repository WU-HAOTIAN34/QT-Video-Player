#ifndef ADD_VIDEO_H
#define ADD_VIDEO_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QPushButton>

#include <string>

class AddVideo : public QPushButton {
    Q_OBJECT

private:
    std::string videosDir;
public:
    AddVideo() : QPushButton(NULL) {

        // handle click
        connect(
            this, SIGNAL( released() ),
            this, SLOT( clicked() ));
    }
    ~AddVideo() {}
private slots:
    void clicked();
signals:
    void addVideo(std::string dir);
};

#endif // ADD_VIDEO_H
