
#include "video_play.h"

void VideoPlay::init(ThePlayer *player, QPushButton *button){
    thePlayer = player;
    theButton = button;
}

void VideoPlay::clickPlay(){
    emit playVideo(thePlayer,theButton);
}

void VideoPlay::clickSkip(){
    emit fastForward(thePlayer);
}

void VideoPlay::clickBack(){
    emit fastBackward(thePlayer);
}

void VideoPlay::initInfo(vector<TheButtonInfo> vector, ThePlayer* player){
    videosList = vector;
    thePlayer = player;
}

void VideoPlay::clickFull(){
    emit full(thePlayer);
}

void VideoPlay::clickNext(){

    for ( int i = 0; i < (int)videosList.size(); i++ ) {
        if(thePlayer->initialName == videosList.at(i).url->fileName()){
            if(i!=(int)videosList.size()-1)
                info = &videosList.at(i+1);
            else
                info = &videosList.at(0);
        }
    }
    emit jumpTo(info);
}

void VideoPlay::clickPrev(){
    for ( int i = 0; i < (int)videosList.size(); i++ ) {
        if(thePlayer->initialName == videosList.at(i).url->fileName()){
            if(i!=0)
                info = &videosList.at(i-1);
            else
                info = &videosList.at((int)videosList.size()-1);
        }
    }
    emit jumpTo(info);
}





