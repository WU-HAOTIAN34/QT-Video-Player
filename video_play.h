#ifndef VIDEO_PLAY_H
#define VIDEO_PLAY_H

#include <QPushButton>
#include <QMediaPlayer>
#include "the_player.h"

class VideoPlay: public QPushButton
{
    Q_OBJECT
public:
    ThePlayer *thePlayer;
    QPushButton *theButton;
    TheButtonInfo* info;
    vector<TheButtonInfo> videosList;


    void init(ThePlayer *player, QPushButton *button);
    void initInfo(vector<TheButtonInfo> vector, ThePlayer* player);

private slots:
    void clickPlay();
    void clickSkip();
    void clickBack();
    void clickNext();
    void clickPrev();
    void clickFull();


signals:
    void playVideo(ThePlayer *player, QPushButton *button);
    void fastForward(ThePlayer *player);
    void fastBackward(ThePlayer *player);
    void full(ThePlayer *player);
    void jumpTo(TheButtonInfo* button);
};

#endif // VIDEO_PLAY_H
