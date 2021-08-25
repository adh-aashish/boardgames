#include "../../include/buddhichal/Buddhichal.hpp"

int Buddhichal::evaluate(char b[3][3])
{
    if (crossCount==0 && cirCount==0)
    {
      if (b[1][1]== ai)
        return 5;
      else if (b[1][1]==opponent)
        return -5;
    }

    if (b[0][0]==b[1][1] && b[1][1]==b[2][2] && b[0][0]!=' ')
    {
        if (b[0][0]==ai)
            return 10;
        else if (b[0][0]==opponent)
            return -10;
    }
 
    if (b[0][2]==b[1][1] && b[1][1]==b[2][0] && b[0][2]!=' ')
    {
        if (b[0][2]==ai)
            return 10;
        else if (b[0][2]==opponent)
            return -10;
    }

    // Checking for Rows for X or O victory.
    for (int row = 0; row<3; row++)
    {
        if (b[row][0]==b[row][1] &&
            b[row][1]==b[row][2] && b[row][0]!= ' ')
        {
            if (b[row][0]== ai)
                return 10;
            else if (b[row][0]==opponent)
                return -10;
        }
    }
 
    // Checking for Columns for X or O victory.
    for (int col = 0; col<3; col++)
    {
        if (b[0][col]==b[1][col] &&
            b[1][col]==b[2][col] && b[0][col]!=' ')
        {
            if (b[0][col]==ai)
                return 10;
 
            else if (b[0][col]==opponent)
                return -10;
        }
    }
 
    // Checking for Diagonals for X or O victory.
 
    // Else if none of them have won then return 0
    return 0;
}
 
int Buddhichal::minimax(char b[3][3], int depth, bool maximizingPlayer, int count, int alpha, int beta)
{
  int score = evaluate(b);
  if (depth == 0)
    return score+depth;

  if(score == 10)
    return score+depth;

  if(score == -10)
    return score-depth;

  if (maximizingPlayer)
  {
    int best = -1000;
    for(int i=0; i<3; i++)
    {
      for (int j=0; j<3; j++)
      {
        if(!noMarkLeft)
        {
          if(b[j][i]==' ')
          {
            b[j][i] = ai;
            best = std::max(best,minimax(b,depth-1,false,count++, alpha, beta));
            b[j][i] = ' ';
            alpha = std::max(alpha, best);
 
            // Alpha Beta Pruning
            if (beta <= alpha)
                break;
          }
        }
        else if(noMarkLeft || count>2)
        {
          if(b[j][i] == ai)
          {
            b[j][i] = 'D';
            for(int m=0; m<3; m++)
            {
              for(int n=0; n<3; n++)
              {
                if(b[n][m] == ' ')
                {
                  if(isValidMove(b,m,n))
                  {
                    b[n][m] = ai;
                    b[j][i] = ' ';
                    best = std::max(best,minimax(b,depth-1,false,count++, alpha, beta));
                    b[j][i] = 'D';
                    b[n][m] = ' ';
                    alpha = std::max(alpha, best);
                    // Alpha Beta Pruning
                    if (beta <= alpha)
                        break;
                  }
                }
              }
            }
            b[j][i] = ai;
          }
        }
      }
    }
    return best;
  }
  else
  {
    int best = 1000;
    for(int i=0; i<3; i++)
    {
      for (int j=0; j<3; j++)
      {
        if(!noMarkLeft)
        {
          if(b[j][i]==' ')
          {
            b[j][i] = opponent;
            best = std::min(best , minimax(b,depth-1,true, count++, alpha, beta));
            b[j][i] = ' ';
            beta = std::min(beta, best);
 
            // Alpha Beta Pruning
            if (beta <= alpha)
                break;
          }
        }
        else if(noMarkLeft || count>2)
        {
          if(b[j][i] == opponent)
          {
            b[j][i] = 'E';
            for(int m=0; m<3; m++)
            {
              for(int n=0; n<3; n++)
              {
                if(b[n][m] == ' ')
                {
                  if(isValidMove(b,m,n))
                  {
                    b[n][m] = opponent;
                    b[j][i] = ' ';
                    best = std::min(best,minimax(b,depth-1,true,count++, alpha, beta));
                    b[j][i] = 'E';
                    b[n][m] = ' ';
                    beta = std::min(beta, best);
                    // Alpha Beta Pruning
                    if (beta <= alpha)
                        break;
                  }
                }
              }
            }
            b[j][i] = opponent;
          }
        }
      }
    }
    return best;
  }
}

void Buddhichal::findBestMove(sf::Vector2i pos[2], char b[3][3]) 
{
  int bestVal = -1000;
  for(int i=0; i<3; i++)
  {
    for (int j=0; j<3; j++)
    {
      if(!noMarkLeft)
      {
        if(b[j][i]==' ')
        {
          b[j][i] = ai;
          int moveVal = minimax(b,8,false,crossCount,-1000,1000);
          b[j][i] = ' ';
          if(moveVal > bestVal)
          {
            pos[0].x = i;
            pos[0].y = j;
            bestVal = moveVal;
          }
        }
      }
      else
      {
        if(b[j][i] == ai)
        {
          b[j][i] = 'D';
          for(int m=0; m<3; m++)
          {
            for(int n=0; n<3; n++)
            {
              if(b[n][m] == ' ')
              {
                if(isValidMove(b,m,n))
                {
                  b[n][m] = ai;
                  b[j][i] = ' ';
                  int moveVal = minimax(b,8,false,crossCount,-1000,1000);
                  b[j][i] = 'D';
                  b[n][m] = ' ';
                  if(moveVal > bestVal)
                  {
                    pos[0].x = m;
                    pos[0].y = n;
                    pos[1].x = i;
                    pos[1].y = j;
                    bestVal = moveVal;
                  }
                }
              }
            }
          }
          b[j][i] = ai;
        }
      }
    }
  }
}
