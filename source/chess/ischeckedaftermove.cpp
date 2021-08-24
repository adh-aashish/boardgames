#include "../../include/chess/boardstate.h"

bool boardstate::ischeckedaftermove(boardstate b,unsigned int m,bool player){
    b.makemove(m);
    return b.ischecked(player);
}
