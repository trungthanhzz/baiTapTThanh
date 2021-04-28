
#ifndef SDL_2_H_
#define SDL_2_H_

#include <iostream>
#include <SDL.h>
#include <cstdlib>
#include <string>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include<vector>
#include <ctime>
#include <cctype>
#include <vector>
#include <fstream>
#include <algorithm>


const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 600;
const std::string WINDOW_TITLE = "HangMan by Trung Thanh`";
const int MAX_BAD_GUESSES = 7;
const char DATA_FILE[] = "Ogden_Picturable_200.txt";

void logSDLError(std::ostream& os,
                 const std::string &msg, bool fatal = false);

void initSDL(SDL_Window* &window, SDL_Renderer* &renderer);


void waitUntilKeyPressed();


// Hai hàm về texture, lấy nguyên về từtutorial tại:
// http://www.willusher.io/sdl2%20tutorials/2013/08/17/lesson-2-dont-put-everything-in-main

// Hàm nạp texture từ file ảnh, để vẽ lên renderer tương ứng
SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren);

// Hàm đưa texture ra renderer (để hiển thị ra màn hình) tại toạ độ (x,y) và giữ nguyên kích cỡ ban đầu của ảnh
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y);

// Hàm (cùng trên nhưng khác tham số với hàm ngay trên)
// đưa texture ra renderer (để hiển thị ra màn hình) tại toạ độ (x,y) và
// với kích cỡ rộng (w) và cao (h) cho trước.
// Ảnh sẽ được co dãn (phóng to thu nhỏ) cho khớp kích cỡ này.
// Đây là hàm không có trong tutorial ban đầu, được sửa từ hàm trong tutorial.
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h);


#endif // SDL_2_H_
