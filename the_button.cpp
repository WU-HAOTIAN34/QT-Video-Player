//
// Created by twak on 11/11/2019.
//

#include "the_button.h"

void TheButton::init(TheButtonInfo* i) {
    info =  i;
}

void TheButton::clicked() {
    emit jumpTo(info);
}

void TheButton::updateText(QString text){
    this->setText(text + "Run");
}
