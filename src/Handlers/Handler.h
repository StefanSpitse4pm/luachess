//
// Created by stefanspitse on 1/31/26.
//

#ifndef LUACHESS_ACTION_H
#define LUACHESS_ACTION_H
#include <string>


class Action {
    public:
    virtual ~Action() = default;

    virtual void router(std::string action) = 0;
};


#endif //LUACHESS_ACTION_H