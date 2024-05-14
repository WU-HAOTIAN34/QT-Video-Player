//
// Created by twak on 11/11/2019.
//

#ifndef CW2_THE_PLAYER_H
#define CW2_THE_PLAYER_H

#include <QApplication>
#include <QMediaPlayer>
#include "qdir.h"
#include "qdiriterator.h"
#include "the_button.h"
#include <vector>
#include <QTimer>
#include <QTime>
#include <QLabel>
#include <QSlider>

using namespace std;

class ThePlayer : public QMediaPlayer {

Q_OBJECT

private:
    vector<TheButtonInfo>* infos;
    vector<TheButton*>* buttons;
    QTimer* mTimer;
    long updateCount = 0;
    QTime timePlayed;

public:
    QTime timeTotal;
    QSlider *slider;
    QString initialName;
    QVideoWidget *myVideo;
    ThePlayer() : QMediaPlayer(NULL) {
        setVolume(0);
        connect (this, SIGNAL (stateChanged(QMediaPlayer::State)), this, SLOT (changePlayState(QMediaPlayer::State)) );
        connect(this, SIGNAL(durationChanged(qint64)), this, SLOT(setTimeTotal()));
        connect(this, SIGNAL(positionChanged(qint64)), this, SLOT(setTimePlayed()));

    }

    void init(QVideoWidget *v);

    // all buttons have been setup, store pointers here
    void setContent(vector<TheButton*>* b, vector<TheButtonInfo>* i);

    //allow access to player timings in string form
    QString getTimeTotal();
    QString gettimePlayed();

    vector<TheButtonInfo> getInfoIn2 (string loc) {

        vector<TheButtonInfo> out =  vector<TheButtonInfo>();
        QDir dir(QString::fromStdString(loc) );
        QDirIterator it(dir);

        while (it.hasNext()) { // for all files

            QString f = it.next();

                if (f.contains("."))

    #if defined(_WIN32)
                if (f.contains(".wmv"))  { // windows
    #else
                if (f.contains(".mp4") || f.contains("MOV"))  { // mac/linux
    #endif
                            QUrl* url = new QUrl(QUrl::fromLocalFile( f ));
                            out . push_back(TheButtonInfo( url ) );
            }
        }

        return out;
    }

signals:
    void nameChanged(QString);
    void durationChanged();
    void positionChanged();

private slots:

    // change the position
    void changePosition(int);

    // change volume
    void changeVolume(int);

    // play or pause
    void changePlayState (QMediaPlayer::State ms);

public slots:

    // start playing this ButtonInfo
    void jumpTo (TheButtonInfo* button);
    void setTimeTotal();
    void setTimePlayed();
    void setSlider();
    void setRate(int);

    void playVideo(ThePlayer* player, QPushButton* button);
    void fastForward(ThePlayer *player);
    void fastBackward(ThePlayer *player);
    void autoPlay();
    void full(ThePlayer *player);
    void addVideo(std::string dir);
};

using namespace std;

class Timer : public QLabel {  //label for displaying updated time elapsed and duration

Q_OBJECT

private:
    ThePlayer * player;

public:
    //create label with starting times 0
    Timer(ThePlayer* inputPlayer) : QLabel(NULL) {
        player = inputPlayer;
        setFont(QFont ("Arial", 14));
        setText("00:00:00/00:00:00");
    }

public slots:
    //updates timings
    void setTime(qint64);
    void setTimeTotal(qint64);
};


#endif //CW2_THE_PLAYER_H
