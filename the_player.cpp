//
// Created by twak on 11/11/2019.
//

#include "the_player.h"
#include "qvideowidget.h"
#include <iostream>
#include <QTime>
#include <QList>
using namespace std;


void ThePlayer::init(QVideoWidget *v){
    myVideo=v;
}

// all buttons have been setup, store pointers here

void ThePlayer::setContent(std::vector<TheButton*>* b, std::vector<TheButtonInfo>* i) {
    buttons = b;
    infos = i;
    jumpTo(buttons -> at(0) -> info);
}

void ThePlayer::changePlayState (QMediaPlayer::State ms) {
    switch (ms) {
        case QMediaPlayer::State::StoppedState:
            play(); // starting playing again...
            break;
    default:
        break;
    }
}

// change the video to play
void ThePlayer::jumpTo (TheButtonInfo* button) {
    initialName = button->url->fileName();
    emit nameChanged(button->url->fileName());
    setMedia( * button -> url);
    play();
}

// convert the format of time
void ThePlayer::setTimeTotal(){
   qint64 duration = this->duration();
   int s = (duration/1000) % 60;
   int m = (duration/60000) % 60;
   int h = (duration/3600000) % 24;
   QTime time(h, m,s);
   timeTotal = time;
}

// convert the format of time
void ThePlayer::setTimePlayed(){
    qint64 elapsed = this->position();
    int s = (elapsed/1000) % 60;
    int m = (elapsed/60000) % 60;
    int h = (elapsed/3600000) % 24;
    QTime time(h, m,s);
    timePlayed = time;

}

// change play or pause state
void ThePlayer::playVideo(ThePlayer *player, QPushButton *button){
    if(player->state()==PausedState){
        player->play();
        button->setIcon(QIcon(":/new/icons/resources/pause.png"));
    }
    else{
        button->setIcon(QIcon(":/new/icons/resources/play.png"));
        player->pause();
    }
}

// full screen play
void ThePlayer::full(ThePlayer *player){
    player->myVideo->setFullScreen(true);
}

// convert the format of time
QString ThePlayer::getTimeTotal(){
    if (timeTotal.hour()>0)
        return timeTotal.toString();
    else
        return timeTotal.toString("mm:ss");
}

// convert the format of time
QString ThePlayer::gettimePlayed(){
    if (timePlayed.hour()>0)
        return timePlayed.toString();
    else
        return timePlayed.toString("mm:ss");
}

// automatically play
void ThePlayer::autoPlay(){
    setContent(buttons, infos);
}

// fast forward 2 seconds
void ThePlayer::fastForward(ThePlayer *player){
    player->setPosition(player->position()+2000);
}

// fast rewind for 2 seconds
void ThePlayer::fastBackward(ThePlayer *player){
    player->setPosition(player->position()-2000);
}

// set position
void ThePlayer::changePosition(int time){
    this->setPosition(time);
}

// set slider
void ThePlayer::setSlider(){
    slider->setRange(0, this->duration() );
    slider->setValue(this->position() );
}

// set speed rate
void ThePlayer::setRate(int index){
    QList<qreal> speeds = {0.5, 1, 1.5, 2};
    setPlaybackRate(speeds[index]);
}

// set a new volume
void ThePlayer::changeVolume(int value){
    setVolume(value);
}

// add a new video
void ThePlayer::addVideo(std::string dir){
    vector<TheButtonInfo> newvideos=getInfoIn2 (dir);
}

// set the time label
void Timer::setTime(qint64){
    this->setText(player->gettimePlayed() + " / " + player->getTimeTotal());
    setFixedHeight(35);
}

// set the duration label
void Timer::setTimeTotal(qint64){
    this->setText(player->getTimeTotal());
    setFont(QFont ("Arial", 10));
}
