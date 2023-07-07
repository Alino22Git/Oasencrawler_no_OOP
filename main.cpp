#include <iostream>
#include <ctime>
#include <string>

struct gameworld {
    int field[5][5];
    int relics=0;
    int dangers=0;
    int springs=0;
};

struct player {
    int x=0;
    int y=0;
    int life=5;
    int relics=0;
};

struct villain {
    int x=rand()%4+1;
    int y=rand()%4+1;
};


void createfieldsIn(struct gameworld& gw) {
    int num=0;
    for(int i = 0; i<5; i++) {
        for(int x = 0; x<5; x++) {
            num=rand()%10;
            if(i==0 && x==0) {
                num=0;
            }
            if(num>=4 && num<=7) {
                gw.dangers+=1;
            } else if(num==8) {
                gw.springs+=1;
            } else if(num==9) {
                gw.relics+=1;
            }
            gw.field[i][x]=num;
        }
    }
    if(gw.relics==0) {
        createfieldsIn(gw);
    }
}

void printOutVisibleWorld(struct gameworld& gw) {
    using namespace std;
    for(int i = 0; i<5; i++) {
        for(int x = 0; x<5; x++) {
            cout <<  gw.field[i][x];
        }
        cout<<endl;
    }
    cout<<endl;
}

void printOutWorld(struct gameworld& gw,struct player& p, struct villain& v) {
    using namespace std;
    for(int i = 0; i<5; i++) {
        for(int x = 0; x<5; x++) {
            if(p.x==x&&p.y==i) {
                cout <<  "[P]";
            } else if(v.x==x&&v.y==i) {
                cout <<  "[V]";
            } else {
                cout <<  "[ ]";
            }
        }
        cout<<endl;
    }
    cout<<endl;
}

void gameInfo(struct gameworld& gw) {
    using namespace std;
    cout<<"Gameworld informations:"<<endl;
    cout<<"Relics:"<<gw.relics<<endl;
    cout<<"Springs:"<<gw.springs<<endl;
    cout<<"Dangers:"<<gw.dangers<<endl;

    cout<<"Move with WASD to gather the relics."<<endl<<endl;
}

int movePlayer(char step,struct player& p) {
    switch(step) {
    case 'w':
        if(p.y>0) {
            p.y-=1;
        } else {
            return 1;
        }
        break;
    case 'a':
        if(p.x>0) {
            p.x-=1;
        } else {
            return 1;
        }
        break;
    case 's':
        if(p.y<4) {
            p.y+=1;
        } else {
            return 1;
        }
        break;
    case 'd':
        if(p.x<4) {
            p.x+=1;
        } else {
            return 1;
        }
        break;
    default:
        std::cout<<"Switch-Case Error"<<std::endl;
        return 1;
    }
    return 0;
}

void input(struct player& p) {
    using namespace std;
    char direction;
    int error=0;
    do {
        error=0;
        cout<<"Move your player:";
        cin>> direction;
        if(direction != 'w' && direction != 'a' && direction != 's' && direction != 'd') {
            error=1;
            cout<<"Wrong input!"<<endl;
        }

        if(error!=1) {
            error=movePlayer(direction,p);
            if(error==1) {
                cout<<"Illegal move!"<<endl;
            }
        }


    } while(error==1);
//    cout<<"He is on the field"<<" x:"<<p.x<<" y:"<<p.y<<endl;
}

void playerReact(struct gameworld& gw, struct player& p,struct villain& v,std::string& log) {
    using namespace std;
//    cout<<"VILLAIN is on the field"<<" x:"<<v.x<<" y:"<<v.y<<endl;
    if(p.x==v.x&&p.y==v.y) {
        cout<<"You ran into the VILLAIN!"<<endl;
        log.append("You ran into the villain.\n");
        p.life=0;
        return;
    }
    int risk=0;
    int fieldtrait=gw.field[p.y][p.x];
//    cout<<"Fieldtrait:"<<fieldtrait<<endl;
//gw.field[p.y][p.x]=0;

    if(fieldtrait>=4 && fieldtrait<=7) {
        cout<<"Dangerfield!|"<<endl;
        risk=rand()%6;
        if(risk==0) {
            log.append("You stepped on a dangerfield and got hurt.\n");
            cout<<"You got hurt|"<<endl;
            gw.dangers-=1;
            p.life-=1;
        } else {
            log.append("You stepped on a danger field.\n");
        }
    } else if(fieldtrait==8) {
        log.append("You stepped on a spring field.\n");
        cout<<"Spring field!|"<<endl;
        p.life+=1;
    } else if(fieldtrait==9) {
        gw.field[p.y][p.x]=0;
        log.append("You gathered a relic!\n");
        cout<<"Relic field!|"<<endl;
        p.relics+=1;
    } else {
        cout<<"Empty field!|"<<endl;
    }
}

void villainMove(struct player& p,struct villain& v,std::string& log) {


    v.x=rand()%5;
    v.y=rand()%5;

    if(p.x==v.x&&p.y==v.y) {
        std::cout<<"You got Caught!"<<std::endl;
        log.append("You got caught by the villain!\n");
        p.life=0;
    }
}
int main() {
    srand(time(NULL));
    using namespace std;
    struct gameworld gw;
    struct player p;
    struct villain v;
    string log;

    createfieldsIn(gw);

    do {

        gameInfo(gw);
        printOutWorld(gw,p,v);
        input(p);
        system("cls");
        playerReact(gw,p,v,log);
        villainMove(p,v,log);

        if(p.relics==gw.relics) {
            system("cls");
            cout<<"Game won!"<<endl;
            log.append("You won the game!\n");
            break;
        }

        if(p.life==0) {
            system("cls");
            cout<<"Game lost!"<<endl;
            log.append("You lost the game!\n");
            break;
        }


        cout<<"------------"<<endl;
    } while(p.life!=0&&p.relics!=gw.relics);

    cout<<"\nLog events:\n";
    cout<<log;
    return 0;
}
