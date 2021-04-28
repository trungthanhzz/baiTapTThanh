
#include "SDL_2.h"

using namespace std;
//**************************************************************

//***************************************************************
SDL_Window* window;
SDL_Renderer* renderer;
Mix_Music* music = nullptr;
void drawBackground();
void DrawText(string text, int x, int y,int size);
bool Play_Game();
bool playAgain(bool win);
string chooseWord();
void renderGame(string guessedWord, int badGuessCount);
bool contains(const string& word, char c);
char readAGuess();
void update(string& guessedWord, const string& word, char guess);
void Music();
void close();

//**************************************************************

int main(int argc, char* argv[])
{
    initSDL(window, renderer);
    SDL_Texture *background = loadTexture("image/background1.bmp",renderer);

    while (true) {
        SDL_RenderClear(renderer);
        renderTexture(background, renderer, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        SDL_RenderPresent(renderer);
        bool win = Play_Game();
        if (!playAgain(win)) break;
    }
    waitUntilKeyPressed();
    SDL_DestroyTexture(background);
    close();
    return 0;
}
string getLowerCaseString(const string& s)
{
    string res = s;
    transform(s.begin(), s.end(), res.begin(), ::tolower);
    return res;
}
string chooseWord(const char* fileName)
{
	vector<string> wordList;
	ifstream file(fileName);
  	if (file.is_open()) {
	    string word;
		while (file >> word) {
            wordList.push_back(word);
        }
    	file.close();
  	}
  	if (wordList.size() > 0) {
  		int randomIndex = rand() % wordList.size();
    	return getLowerCaseString(wordList[randomIndex]);
	} else return "";
}



void renderGame(string guessedWord, int badGuessCount)
{
    vector <SDL_Texture* > figure;
    figure.push_back(loadTexture("Figure/man0.bmp",renderer));
    figure.push_back(loadTexture("Figure/man1.bmp",renderer));
    figure.push_back(loadTexture("Figure/man2.bmp",renderer));
    figure.push_back(loadTexture("Figure/man3.bmp",renderer));
    figure.push_back(loadTexture("Figure/man4.bmp",renderer));
    figure.push_back(loadTexture("Figure/man5.bmp",renderer));
    figure.push_back(loadTexture("Figure/man6.bmp",renderer));
    figure.push_back(loadTexture("Figure/man7.bmp",renderer));
    int imageNum = figure.size();
    for (int i = 0; i < imageNum; ++i) {
        if (figure[i] == nullptr) {
            for (int j = 0; j < imageNum; ++j) SDL_DestroyTexture(figure[j]);
            close();
        }
    }
    DrawText(guessedWord,370,210,80);
    DrawText("Bad guess count: "+to_string(badGuessCount),600,125,30);
    renderTexture(figure[badGuessCount],renderer,49,130,180,250);
    SDL_RenderPresent(renderer);
    for (int j = 0; j < imageNum; ++j) SDL_DestroyTexture(figure[j]);
}

bool contains(const string& word, char c)
{
	return (word.find_first_of(c) != string::npos);
}

void update(string& guessedWord, const string& word, char guess)
{
	for (int i = word.length()-1; i >= 0; i--) {
        if (word[i] == guess) {
            guessedWord[i] = guess;
        }
    }

}

void DrawText(string text, int x, int y,int size)
{
    TTF_Font* font = NULL;
    font = TTF_OpenFont("font.ttf", size);
    SDL_Color fg = { 255, 255, 255,255 };
    SDL_Surface* surface = NULL;
	SDL_Texture* texture = NULL;
    surface = TTF_RenderText_Solid(font, text.c_str(), fg);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    SDL_Rect srcRest;
    SDL_Rect desRect;
    TTF_SizeText(font, text.c_str(), &srcRest.w, &srcRest.h);

    srcRest.x = 0;
    srcRest.y =  0;

    desRect.x = x;
    desRect.y = y;

    desRect.w = srcRest.w;
    desRect.h = srcRest.h;
    SDL_SetRenderDrawColor(renderer,46,68,66,255);
    SDL_RenderFillRect(renderer, &desRect);
    SDL_RenderCopy(renderer, texture, &srcRest, &desRect);
    SDL_RenderPresent(renderer);
}
void Music()
{
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1)
    {
	printf("%s", Mix_GetError());
    }
}

bool Play_Game()
{


    SDL_Event a;
    string yourGuess;
    string badGuesses = "";
    char guess;
    srand(time(0));
	string word = chooseWord(DATA_FILE);
	if (word.length() < 1) {
		cout << "Error reading vocabulary file " << DATA_FILE;
		return -1;
	}
	string guessedWord = string(word.length(), '-');
	int badGuessCount = 0;
    do {
		renderGame(guessedWord, badGuessCount);
		if(badGuessCount >= 1){
        yourGuess = string("The ") + (badGuessCount == 1 ? "word" : "words") + " you have guessed" + ": " + badGuesses ;
        }
        DrawText(yourGuess,300, 350, 30);
		char guess = readAGuess();
		badGuesses+= guess;
		if (contains(word, guess))
        {
            update(guessedWord, word, guess);
            Music();
            music = Mix_LoadMUS("music/dung.mp3");
            Mix_PlayMusic(music, 0);
        }
        else
		{
            badGuessCount++;
            Music();
            music = Mix_LoadMUS("music/sai.mp3");
            Mix_PlayMusic(music, 0);
		}
	} while (badGuessCount < MAX_BAD_GUESSES && word != guessedWord);

	renderGame(guessedWord, badGuessCount);
    if (badGuessCount != MAX_BAD_GUESSES)
    {
        SDL_Delay(500);
        Music();
        music = Mix_LoadMUS("music/thang.mp3");
        Mix_PlayMusic(music, 0);
        SDL_RenderClear(renderer);
        SDL_Texture*Win = loadTexture("image/win.bmp",renderer);
        renderTexture(Win,renderer,0,0);
        SDL_RenderPresent(renderer);
        SDL_DestroyTexture(Win);
        SDL_Delay(1000);
        SDL_Texture* yesNo = loadTexture("image/yesNo.bmp", renderer);
        renderTexture(yesNo, renderer, 200, 370);
        SDL_RenderPresent(renderer);
        SDL_DestroyTexture(yesNo);
        return true;
    }
    else
    {
        SDL_Delay(500);
        Music();
        music = Mix_LoadMUS("music/thua.mp3");
        Mix_PlayMusic(music, 0);
        SDL_RenderClear(renderer);
        SDL_Texture*lose = loadTexture("image/lose.bmp",renderer);
        renderTexture(lose,renderer,0,0);
        SDL_RenderPresent(renderer);
        SDL_DestroyTexture(lose);
        SDL_Delay(100);
        DrawText("THE CORRECT WORD IS: " + word, 125, 250, 50);
        SDL_Delay(1000);
        SDL_Texture* yesNo = loadTexture("image/yesNo.bmp", renderer);
        renderTexture(yesNo, renderer, 200, 370);
        SDL_RenderPresent(renderer);
        SDL_DestroyTexture(yesNo);
        return false;
    }

}
bool playAgain(bool win) {
    SDL_Event e;
    int x, y;
    while (true) {
        if ( SDL_WaitEvent(&e) != 0){
            if (e.type == SDL_QUIT) exit(0);
            if (e.type == SDL_MOUSEBUTTONDOWN )
            {
                SDL_GetMouseState(&x, &y);
                if (x>=314 && x<=394)
                   if (y>=478 && y<=523) break;
                if (x>=569 && x<=636)
                    if (y>=478 && y<=523) exit (0);
            }
        }
        SDL_Delay(100);
    };
    return true;
}
char readAGuess(){
    int x,y;
    static SDL_Event e;
    while (SDL_WaitEvent(&e)) {
        if (e.type == SDL_QUIT) exit(0);
        if (e.type == SDL_MOUSEBUTTONDOWN) {
        SDL_GetMouseState(&x, &y);
            if(y>=500 && y<=550) return (x-150)/58+'o';
            if(y>=430 && y<=479) return (x-142)/49+'a';
        }
    }
 }
 void close()
 {
    //Free the music
	Mix_FreeMusic( music );
	music = NULL;

	//Destroy window
	SDL_DestroyRenderer( renderer );
	SDL_DestroyWindow( window );
    TTF_Quit();
	Mix_Quit();
	SDL_Quit();
}

 /*void menu () {
    SDL_Event e;
    int x, y;
    SDL_Texture*menu = loadTexture("image/menu.bmp",renderer);
    renderTexture(menu, renderer, 0, 0);
    SDL_RenderPresent(renderer);
    while (SDL_WaitEvent(&e)) {
        if (e.type == SDL_QUIT) exit(0);
        if (e.type == SDL_MOUSEBUTTONDOWN) {
        SDL_GetMouseState(&x, &y);
            if (y>=252 && y<=322)
                if ( x>=382 && x<=614)
                    bool win = Play_Game();
            if ( y>=341 && y<=410)
                if ( x>= 417 && x<=581)
                {
                    SDL_Texture* help = loadTexture("image/help.bmp",renderer);
                    renderTexture(help, renderer, 0,0);
                    SDL_RenderPresent(renderer);
                }
        }
    }
 }*/


// 220x300. 200x370
