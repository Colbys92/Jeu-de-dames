
#pragma once


bool checkEdges(int i, int position){
    if(i==0 || i==3){
        if(position%10==5){
            return true;
        }
    }
    else{
        if(position%10==4){
            return true;
        }
    }
    if(i==0|| i==1){
        if(position<=4){
            return true;
        }
    }
    else{
        if(position>=45){
            return true;
        }
    }
    return false;
}


