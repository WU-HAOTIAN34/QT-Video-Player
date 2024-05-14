/*
 *
 *    ______
 *   /_  __/___  ____ ___  ___  ____
 *    / / / __ \/ __ `__ \/ _ \/ __ \
 *   / / / /_/ / / / / / /  __/ /_/ /
 *  /_/  \____/_/ /_/ /_/\___/\____/
 *              video for sports enthusiasts...
 *
 *  2811 cw3 : twak
 */

#include <iostream>
#include <QApplication>
#include <QtMultimediaWidgets/QVideoWidget>
#include <QMediaPlaylist>
#include <string>
#include <vector>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtCore/QFileInfo>
#include <QtWidgets/QFileIconProvider>
#include <QDesktopServices>
#include <QImageReader>
#include <QScrollArea>
#include <QMessageBox>
#include <QtCore/QDir>
#include <QtCore/QDirIterator>
#include <QSlider>
#include <QComboBox>
#include <QShortcut>
#include "the_player.h"
#include "the_button.h"
#include "video_play.h"
#include "add_video.h"

using namespace std;

// read in videos and thumbnails to this directory
vector<TheButtonInfo> getInfoIn (string loc) {

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
                        QUrl* url = new QUrl(QUrl::fromLocalFile( f )); // convert the file location to a generic url
                        out . push_back(TheButtonInfo( url ) ); // add to the output list
        }
    }

    return out;
}

// set up the sliders
void setSliders(QSlider* progress, QSlider* vSlider, ThePlayer *player){

    player->slider = progress;

    player->connect(progress, SIGNAL(sliderMoved(int)), player, SLOT(changePosition(int)));
    QObject::connect(player, SIGNAL(positionChanged(qint64)), player, SLOT(setSlider()));

    player->slider->setStyleSheet("QSlider::groove:horizontal{\
                                height: 18px;\
                                left: 0px;\
                                right: 0px;\
                                border-radius: 2px;\
                                background:rgba(192,192,192,50);\
                            }\
                            QSlider::handle:horizontal{\
                                width:  20px;\
                                height: 0px;\
                                list-style-type:disc;\
                                margin-top: -6px;\
                                margin-bottom: -6px;\
                                border-radius: 6px;\
                                background:rgba(25,25,112,1)\
                            }\
                            QSlider::sub-page:horizontal{\
                                border-radius: 2px;\
                                background:rgba(135,206,235,1)\
                            }");

    //connect volume slider with function to change the players volume
    player->connect(vSlider, SIGNAL(sliderMoved(int)), player, SLOT(changeVolume(int)));
    vSlider->setRange(0,100);
    vSlider->setMaximumWidth(100);

    vSlider->setStyleSheet("QSlider::groove:horizontal{\
                                height: 12px;\
                                left: 0px;\
                                right: 0px;\
                                border-radius: 2px;\
                                background:rgba(192,192,192,50);\
                            }\
                            QSlider::handle:horizontal{\
                                width:  10px;\
                                height: 0px;\
                                list-style-type:disc;\
                                margin-top: -4px;\
                                margin-bottom: -4px;\
                                border-radius: 2px;\
                                background:rgba(0,0,0,1)\
                            }\
                            QSlider::sub-page:horizontal{\
                                border-radius: 2px;\
                                background:rgba(135,206,235,1)\
                            }");
}

// set up the previous and next video button
void setNextPrevButtons(VideoPlay *next, VideoPlay *prev, ThePlayer *player){
    next->setIcon((QIcon(":/new/icons/resources/prev.png")));
    next->connect(next, SIGNAL(released()), next, SLOT (clickNext() ));
    next->connect(next,SIGNAL(jumpTo(TheButtonInfo*)),player,SLOT(jumpTo(TheButtonInfo*)));

    prev->setIcon((QIcon(":/new/icons/resources/next.png")));
    prev->connect(prev, SIGNAL(released()), prev, SLOT(clickPrev()));
    next->connect(prev,SIGNAL(jumpTo(TheButtonInfo*)),player,SLOT(jumpTo(TheButtonInfo*)));
}

// set up fast forward and backward button
void setFastButtons(VideoPlay *forward, VideoPlay *backward, ThePlayer *player){
    backward->setIcon((QIcon(":/new/icons/resources/fast backwards.png")));
    backward->setMaximumWidth(60);
    backward->setFixedHeight(24);
    backward->connect(backward, SIGNAL(released()), backward, SLOT (clickBack() ));
    backward->connect(backward,SIGNAL(fastBackward(ThePlayer*)),player,SLOT(fastBackward(ThePlayer*)));
    backward->init(player,backward);
    forward->setIcon((QIcon(":/new/icons/resources/fast forward.png")));
    forward->setMaximumWidth(60);
    forward->setFixedHeight(24);
    forward->connect(forward, SIGNAL(released()), forward, SLOT (clickSkip() ));
    forward->connect(forward,SIGNAL(fastForward(ThePlayer*)),player,SLOT(fastForward(ThePlayer*)));
    forward->init(player,forward);
}

// set up full screen button
void setFullButton(VideoPlay *full, ThePlayer *player){
    full->setIcon((QIcon(":/new/icons/resources/full.png")));
    full->setMaximumWidth(40);
    full->setFixedHeight(40);
    full->init(player,full);
    full->connect(full, SIGNAL(released()), full, SLOT (clickFull() ));
    full->connect(full,SIGNAL(full(ThePlayer*)),player,SLOT(full(ThePlayer*)));
}

// set up volume controls
QHBoxLayout * setVolumeControls(QSlider *vSlider){
    QLabel *vIcon = new QLabel();
    vIcon->setFixedSize(30, 30);
    vIcon->setPixmap(QPixmap(":/new/icons/resources/volume.png"));

    QHBoxLayout *volume = new QHBoxLayout();
    volume->addWidget(vIcon);
    volume->addWidget(vSlider);
    vSlider->setFixedWidth(200);
    return volume;
}

// set up play rate box
QComboBox * setPlayRateControls(ThePlayer *player){
    QComboBox *rate = new QComboBox();
    rate->addItem("x0.5");
    rate->addItem("x1.0");
    rate->addItem("x1.5");
    rate->addItem("x2.0");
    rate->setCurrentIndex(1);
    rate->setFixedHeight(24);

    QObject::connect(rate, SIGNAL(currentIndexChanged(int)), player, SLOT(setRate(int)));
    return rate;
}

// set up title layout
QHBoxLayout * setTitleLayout(ThePlayer *player){
    QLabel *fixed = new QLabel("Present Video: ");
    QLabel *title = new QLabel(player->initialName);
    fixed->setFixedHeight(35);
    fixed->setFont(QFont ("Arial", 13));
    title->connect(player, SIGNAL(nameChanged(QString)), title, SLOT(setText(QString)));
    title->setFixedHeight(35);
    title->setFont(QFont ("Arial", 13));

    QHBoxLayout *titleLayout = new QHBoxLayout();
    titleLayout->addWidget(fixed);
    titleLayout->addWidget(title);
    titleLayout->addStretch();
    return titleLayout;
}

// set up buttons of the first row
QHBoxLayout * setContorlLayout(ThePlayer *player, VideoPlay *prev, VideoPlay *next, VideoPlay *full, QSlider *vSlider, VideoPlay *tmp){
    QHBoxLayout *stateLayout = new QHBoxLayout();
    stateLayout->addWidget(prev);    // previus video button
    stateLayout->addWidget(tmp,4);   // beigin/stop button
    stateLayout->addWidget(next);    // next video button
    stateLayout->addWidget(setPlayRateControls(player));    // sound area
    stateLayout->addItem(setVolumeControls(vSlider));       // multiplay
    stateLayout->addWidget(full);

    return stateLayout;
}

// set up buttons of the second row
QHBoxLayout * setContorlLayout2(ThePlayer *player,   VideoPlay *backward, VideoPlay *forward, QSlider *progress){
    QHBoxLayout *stateLayout = new QHBoxLayout();
    stateLayout->addWidget(backward);   // previous second
    stateLayout->addWidget(progress);   // time line
    stateLayout->addWidget(forward);    // next second
    Timer *time = new Timer(player);
    QObject::connect(player, SIGNAL(durationChanged(qint64)), time, SLOT(setTime(qint64)));
    QObject::connect(player, SIGNAL(positionChanged(qint64)), time, SLOT(setTime(qint64)));
    stateLayout->addWidget(time);   //time

    return stateLayout;
}

// set up scroll area with videos list
QScrollArea * setScrollArea(QGridLayout *grid){
    QWidget *bottom_videos = new QWidget();
    bottom_videos->setLayout(grid);
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidget(bottom_videos);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setFixedWidth(300);
    return scrollArea;
}

// set up previews of videos
void setPreview(QWidget *window, TheButton *button, QGridLayout *grid, int i){
    QVideoWidget *previewWidget = new QVideoWidget();
    ThePlayer *previewPlayer = new ThePlayer();
    previewPlayer->setVideoOutput(previewWidget);
    previewPlayer->setMedia(*button->info->url);
    previewPlayer->play();
    previewPlayer->setVolume(0);
    previewWidget->setAspectRatioMode(Qt::KeepAspectRatio);
    previewWidget->setFixedHeight(0.2*window->geometry().height());
    previewWidget->setFixedWidth(float((0.2*16/9))*window->geometry().height());

    // show the videos' name
    button->setFont(QFont ("Arial", 16));
    button->setFixedWidth(float((0.15*16/9))*window->geometry().height());
    button->setText(button->info->url->fileName());

    // shwo total time
    Timer *duration = new Timer(previewPlayer);
    QObject::connect(previewPlayer, SIGNAL(durationChanged(qint64)), duration, SLOT(setTimeTotal(qint64)));

    // add the labels to the widget
    grid->addWidget(previewWidget, i*3+1, 0, Qt::AlignCenter);
    grid->addWidget(button,  i*3+2, 0, Qt::AlignCenter);
    grid->addWidget(duration, i*3+3, 0, Qt::AlignCenter);
}

//void initalNoVideos(){

//}

int main(int argc, char *argv[]) {

    // let's just check that Qt is operational first
    qDebug() << "Qt version: " << QT_VERSION_STR;

    // create the Qt Application
    QApplication app(argc, argv);

    // collect all the videos in the folder
    vector<TheButtonInfo> videos;

    if (argc == 2)
        videos = getInfoIn( string(argv[1]) );

    if (videos.size() == 0) {
        const int box = QMessageBox::question(
                    NULL,
                    QString("MVP"),
                    QString("no videos found! download, unzip, and add command line to this program. You can download videos from Tom's OneDrive or download video by yourself"),
                    QMessageBox::Yes |
                    QMessageBox::No );

        switch( box )
        {
        case QMessageBox::Yes:
          QDesktopServices::openUrl(QUrl("https://leeds365-my.sharepoint.com/:u:/g/personal/scstke_leeds_ac_uk/EcGntcL-K3JOiaZF4T_uaA4BHn6USbq2E55kF_BTfdpPag?e=n1qfuN"));
          break;
        default:
            break;
        }
        exit(-1);
    }

    // create the main window and layout
    QWidget window;
    QVBoxLayout *top = new QVBoxLayout();
    window.setLayout(top);
    window.setWindowTitle("MVP");
    window.setMinimumSize(930, 680);

    // the widget that will show the video
    QVideoWidget *videoWidget = new QVideoWidget;
    //videoWidget->showFullScreen();
    QShortcut *esc = new QShortcut(QKeySequence("Esc"), videoWidget);
    QAbstractButton::connect(esc, &QShortcut::activated,
            videoWidget, [&](){videoWidget->setFullScreen(false);});

    //the slider to show video progress
    QSlider *progress = new QSlider(Qt::Horizontal);

    //the volume slider
    QSlider *vSlider = new QSlider(Qt::Horizontal);

    // the QMediaPlayer which controls the playback
    ThePlayer *player = new ThePlayer;
    player->init(videoWidget);
    player->setVideoOutput(videoWidget);
    setSliders(progress, vSlider, player);

    // a row of buttons
    QWidget *buttonWidget = new QWidget();

    // a list of the buttons
    vector<TheButton*> buttons;

    //the buttons and videos are added in a grid layout
    QGridLayout *grid = new QGridLayout();

    AddVideo *addVideo = new AddVideo();
    addVideo->setIcon(QIcon(":/new/icons/resources/add.png"));
    addVideo->setFixedHeight(50);
    addVideo->setFixedWidth(240);
    addVideo->connect(addVideo,SIGNAL(addVideo(std::string)),player,SLOT(addVideo(std::string)));

    // create the preview video players for the bottom of the page
    grid->addWidget(addVideo, 0, 0, Qt::AlignCenter);
    for ( int i = 0; i < (int)videos.size(); i++ ) {

        //create buttons for each video in folders
        TheButton *button = new TheButton(buttonWidget);
        button->connect(button, SIGNAL(jumpTo(TheButtonInfo* )), player, SLOT (jumpTo(TheButtonInfo* ))); // when click, tell the player to play.
        buttons.push_back(button);
        button->init(&videos.at(i));

        //create video players and durations comprising preview and add to layout grid
        setPreview(&window, button, grid, i);
    }

    // tell the player what buttons and videos are available
    player->setContent(&buttons, & videos);

    VideoPlay * tmp = new VideoPlay();
    tmp->setIcon(QIcon(":/new/icons/resources/pause.png"));
    tmp->setStyleSheet("border-radius:50px");

    tmp->setIconSize(QSize(60,60));
    tmp->setMaximumWidth(60);
    tmp->setFixedHeight(60);
    tmp->connect(tmp, SIGNAL(released()), tmp, SLOT (clickPlay() ));
    tmp->connect(tmp,SIGNAL(playVideo(ThePlayer*,QPushButton*)),player,SLOT(playVideo(ThePlayer*,QPushButton*)));
    tmp->init(player,tmp);

    //Adding next and previous
    VideoPlay *next = new VideoPlay();
    VideoPlay *prev = new VideoPlay();
    VideoPlay *full = new VideoPlay();
    setNextPrevButtons(next, prev, player);
    setFullButton(full, player);
    next->initInfo(videos,player);
    prev->initInfo(videos,player);
    full->initInfo(videos,player);

    //Adding akip and backward buttons
    VideoPlay *forward = new VideoPlay();
    VideoPlay *backward = new VideoPlay();
    setFastButtons(forward, backward, player);

    //add elementto the window
    QGridLayout *background = new QGridLayout(); // the grid left is the video and right is main body
    QVBoxLayout *mainBody = new QVBoxLayout();   // main body consists of buttons and player
    background->addWidget(setScrollArea(grid), 0,0);
    mainBody->addLayout(setTitleLayout(player));
    mainBody->addWidget(videoWidget);
    mainBody->addLayout(setContorlLayout(player, prev, next, full, vSlider, tmp));
    mainBody->addLayout(setContorlLayout2(player, backward, forward, progress));
    background->addLayout(mainBody,0,4,Qt::AlignCenter);
    top->addLayout(background);

    // showtime!
    window.show();

    // wait for the app to terminate
    return app.exec();
}

