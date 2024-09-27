#include<iostream>

using namespace std;

enum game{paper=1,sci=2,stone=3};
enum users{player=1,computer=2,draw=3};
struct roundinfo{
  int round=0;
  game player1choice;
  game computerchoice; 
  users winner;
  string winnername; 
  
};
struct gameover{
  int totalround=0;
 int player1=0; 

int computer= 0; 
int draw= 0; 
  users gamewinner;
  string gamewinnername; 
  
};

int randnum(int to,int from){
  return rand()%(to-from+1)+from;
}

string winnername(users user){
  string arr[3]= {"player","computer","draw"};
  return arr[user-1];
}
users whowontheround(roundinfo round){
  if(round.player1choice == round.computerchoice){
    return users::draw;
  }
  
  switch(round.player1choice){
    case game::paper:
      if(round.computerchoice == game::stone){
        return users::player;
      }
      break;
   
    case game::sci:
      if(round.computerchoice == game::paper){
        return users::player;
      }
      break;

    case game::stone:
      if(round.computerchoice == game::sci){
        return users::player;
      }
      break;
  }

  return users::computer;
}
string gameuser(game user){
  string arr[3]= {"paper","sci","stone"};
  return arr[user-1];
}
void printresults(roundinfo round) { 
      cout << "\n____________Round [" << round.round << "] ____________\n\n";
           cout << "Player1  Choice: " << gameuser(round.player1choice) << endl; 
               cout << "Computer Choice: " << gameuser(round.computerchoice) << endl;  
                  cout << "Round Winner   : [" << round.winnername << "] \n";     cout << "__________________________________\n" << endl;  
                    }
users whowonthegame(int player1,int computer1){
if(player1>computer1){
  return users::player;
}
else if(computer1>player1){
  return users::computer;
}
else{
  return users::draw;
}
}
gameover fillgameresults(int totalround=0,int player1=0, int computer1= 0, int draw1= 0 ){
gameover over; 
over.totalround =totalround;
over.player1 =player1;
over.computer =computer1;
over.draw =draw1;
return over; 


}
game playerchoice(){
  int choice; 
choice= 1; 
do{
  cout<<"enter your choice from 1,3"<<endl;
  cin>>choice;

}while(choice<1||choice>3);
return (game)choice;

}
game computerchoice(){

return (game)randnum(1,3);
}
int user(){
  int  n; 
  cin>>n; 
 return n; 
}
gameover playgame(int howmanyround){
  roundinfo round; 
  int player1=0,computer=0,draw=0;
  for(int i=1;i<=howmanyround;i++){
    cout<<i; 
round.round = i; 
round.player1choice = playerchoice();
round.computerchoice=computerchoice();
round.winner =whowontheround(round);
round.winnername =winnername(round.winner);

  
if(round.winner==users::player)
player1++;

else if(round.winner==users::computer)
  computer++;


else
  draw++;


printresults(round);

  }
    return fillgameresults(howmanyround, player1, computer, draw);
  
}
string Tabs(short numberoftabs){
  string t="";
  for(int i=0;i<numberoftabs;i++){
    t+="\t";
    cout<<t;
  }
  return t;
}
void ShowGameOverScreen() {  
     cout << Tabs(2) << "__________________________________________________________\n\n"; 
    cout << Tabs(2) << "                 +++ G a m e  O v e r +++\n"; 
        cout << Tabs(2) << "__________________________________________________________\n\n"; 
}
void showfinalgameresults(gameover over){
    cout << Tabs(2) << "Game Rounds: " << over.totalround << endl;
    cout << Tabs(2) << "Player1 Wins: " << over.player1 << endl;
    cout << Tabs(2) << "Computer Wins: " << over.computer << endl;
    cout << Tabs(2) << "Draws: " << over.draw << endl;
    
    // Determine the final winner based on the game results
    over.gamewinner = whowonthegame(over.player1, over.computer);
    over.gamewinnername = winnername(over.gamewinner);
    
    cout << Tabs(2) << "Final Winner: " << over.gamewinnername << endl;
}

int  ReadHowManyRounds() {
   short GameRounds = 1; 
   do    { 
            cout << "How Many Rounds 1 to 10 ? \n";
                     cin >> GameRounds;     
            } while (GameRounds < 1 || GameRounds >10); return GameRounds; } 
            void startgame(){
              char playagain= 'y';
              do{
                gameover over= playgame(ReadHowManyRounds());
                showfinalgameresults(over);
                cout<<Tabs<<"do you wanna playagain"<<endl;
                cin>>playagain;
              }while(playagain=='y'||playagain=='Y');
            }

int main(){
srand((unsigned)time(NULL));

startgame();
}