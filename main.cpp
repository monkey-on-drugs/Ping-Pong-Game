#include <iostream>
#include <raylib.h>

using namespace std;

Color Green = Color{38, 185, 154, 255};
Color Dark_Green = Color{20, 160, 133, 255};
Color Light_Green = Color{129, 204, 184, 255};
Color Yellow = Color{243, 213, 91, 255};

int player_score = 0; int cpu_score = 0;

class Ball{
    public:
        float x, y;
        int speed_x, speed_y;
        int radius;

    void Draw(){
        DrawCircle(x, y, radius, Yellow);
    }

    void Update(){
        x += speed_x; y += speed_y;

        if(y + radius >= GetScreenHeight() || y - radius <= 0){
            speed_y *= -1;
        }
        if(x + radius >= GetScreenWidth() || x - radius <= 0){
            speed_x *= -1;
        }
        if(x + radius >= GetScreenWidth()){
            cpu_score++; ResetBall();
        }
        if(x - radius <= 0){
            player_score++; ResetBall();
        }
    }

    void ResetBall(){
        x = GetScreenWidth()/2; y = GetScreenHeight()/2;

        int speed_choices[2] = {-1, 1};
        speed_x *= speed_choices[GetRandomValue(0, 1)];
        speed_y *= speed_choices[GetRandomValue(0, 1)];
    }
};

class Paddle{
    protected:

    void LimitMovement(){
        if(y <= 0){
            y = 0;
        }
        if((y + height) >= GetScreenHeight()){
            y = GetScreenHeight() - height;
        }
    }

    public:
        float x, y;
        float width, height;
        int speed;

    void Draw(){
        DrawRectangleRounded(Rectangle{x, y, width, height}, 0.8, 0, WHITE);
    }

    void Update(){
        if(IsKeyDown(KEY_UP)){
            y -= speed;
        }
        if(IsKeyDown(KEY_DOWN)){
            y += speed;
        }
        LimitMovement();
    }
};

class CpuPaddle: public Paddle{
    public:
    
    void Update(int ball_y){
        if((y + height/2) > ball_y){
            y -= speed;
        }
        if((y + height/2) <= ball_y){
            y += speed;
        }
        LimitMovement();
    }
};


Ball ball;
Paddle player;
CpuPaddle cpu;

int main () {

    cout << "Starting the game" << endl;

    //Display
    const int screen_width = 1280; //in pixels
    const int screen_height = 800;
    InitWindow(screen_width, screen_height, "Ping Pong Game");
    SetTargetFPS(60);

    //Ball
    ball.radius = 20; ball.x = screen_width/2; ball.y = screen_height/2;
    ball.speed_x = 7; ball.speed_y = 7; 

    //Player Paddle
    player.height = 120; player.width = 25; player.speed = 6;
    player.x = screen_width - player.width - 10;
    player.y = (screen_height - player.height)/2;

    //CPU Paddler
    cpu.height = 120; cpu.width = 25; cpu.speed = 6;
    cpu.x = 10; cpu.y =  (screen_height - cpu.height)/2;

    while(WindowShouldClose() == false){
        BeginDrawing();

        //Updating
        ball.Update(); player.Update(); cpu.Update(ball.y);

        //Collisons
        if(CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{player.x, player.y, player.width, player.height})){
            ball.speed_x *= -1;
        }
        if(CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{cpu.x, cpu.y, cpu.width, cpu.height})){
            ball.speed_x *= -1;
        }

        //Drawing
        ClearBackground(Dark_Green);
        DrawRectangle(screen_width/2, 0, screen_width/2, screen_height, Green);
        DrawCircle(screen_width/2, screen_height/2, 150, Light_Green);
        DrawLine(screen_width/2, 0, screen_width/2, screen_height, WHITE);
        ball.Draw(); player.Draw(); cpu.Draw();
        DrawText(TextFormat("%i", cpu_score), screen_width/4 - 20, 20, 80, WHITE);
        DrawText(TextFormat("%i", player_score), 3*(screen_width/4 - 20), 20, 80, WHITE);


        EndDrawing();
    }

    CloseWindow();
    return 0;
}