#include <cstdlib>
#include <iostream>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
using namespace std;
class Tile{
public:
    int x_position;
    int y_position;
    int value;
    bool is_mine = false;
    bool is_flagged = false;
    bool is_revealed = false;
    bool is_blank = false;
    bool is_visited = false;



    /*Tile& west;
    Tile& east;
    Tile& north;
    Tile& south;
    Tile& northwest;
    Tile& northeast;
    Tile& southwest;
    Tile& southeast;*/
    // make a map of references with keys being the directions
    map<string, Tile> tiles_around;

    sf::Texture tile_texture;
    sf::Texture hidden_texture;
    sf::Sprite tile_sprite;
    sf::Sprite overlay;
    void display(sf::RenderWindow &window){
        tile_sprite.setTexture(tile_texture);
        window.draw(tile_sprite);
    }

    void overlay_display(sf::RenderWindow &window){
        overlay.setTexture(hidden_texture);
        window.draw(overlay);
    }

};
struct Name{
    string player_name;
};


void copy_tiles(vector<Tile> &tiles, vector<vector<Tile*>> &tiles_2d, int rows, int columns);
int check_neighbors(int i, int j, int rows, int columns, vector<vector<Tile*>> &tiles_2d);
void checkzero(int i, int j, int rows, int columns, vector<vector<Tile*>> &tiles_2d);
void checknum(int i, int j, int rows, int columns, vector<vector<Tile*>> &tiles_2d, sf::RenderWindow &window);
bool checkwin(int goal, int mines, vector<vector<Tile*>> &tiles_2d, sf::RenderWindow &window);

void setText(sf::Text &text, float x, float y){
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f,
                   textRect.top + textRect.height/2.0f);
    text.setPosition(sf::Vector2f(x, y));
}

int main()
{
    bool game_won;
    bool game_lost;
    vector<int> times;
    vector<int> leaderboardTimes;

    std::string filename2 = "files/board_config.cfg";

    //std::fstream file1(filename, std::ios_base::in | std::ios_base::binary);
    std::ifstream file2(filename2);
    if (file2.is_open()) {
        //std::cout << "Hi" << std::endl;
        std::string line;
        int count = 1;
        int columns;
        int rows;
        int mines;
        while (std::getline(file2, line)) {
            if (count == 1) {
                columns = std::stoi(line);
                count += 1;
            } else if (count == 2) {
                rows = std::stoi(line);
                count += 1;
            } else if (count == 3) {
                mines = std::stoi(line);
                count += 1;
            }
        }

        int width = columns * 32;
        int height = rows * 32;
        height += 100;
        sf::RenderWindow welcome_window(sf::VideoMode(width, height), "Welcome window");
        bool closed_with_x = false;




        sf::Font font;
        font.loadFromFile("files/font.ttf");
        sf::Text text_welcome;
        text_welcome.setString("WELCOME TO MINESWEEPER!");
        text_welcome.setFont(font);
        text_welcome.setCharacterSize(24);
        auto bounds = text_welcome.getGlobalBounds();
        text_welcome.setOrigin(bounds.width/2, bounds.height);
        float x_position = (float) width;
        x_position = x_position/2.0f;
        float y_position = (float) height;
        y_position = y_position/2.0f;
        y_position -= 150.0;
        cout << x_position << endl;
        cout << y_position << endl;
        text_welcome.setPosition(x_position, y_position);
        text_welcome.setStyle(sf::Text::Bold | sf::Text::Underlined);


        sf::Text text_input;
        //setText(text_input,x_input, y_input);
        text_input.setString("Enter your name:");
        text_input.setFont(font);
        text_input.setCharacterSize(20);
        text_input.setFillColor(sf::Color::White);
        text_input.setStyle(sf::Text::Bold);
        auto bounds2 = text_input.getGlobalBounds();
        text_input.setOrigin(bounds2.width/2, bounds2.height);
        float x_input = (float) width;
        x_input = x_input / 2.0f;

        float y_input = (float) height;
        y_input = y_input / 2.0f;
        y_input -= 75.0;
        text_input.setPosition(x_position, y_input);



        sf::String player_string;

        sf::String final_string;
        sf::Text player_text;
        player_text.setFont(font);
        float player_x = (float) width;
        player_x = player_x / 2.0f;
        float player_y = (float) height;
        player_y = player_y / 2.0f;
        player_y -= 45.0;
        //setText(player_text, player_x, player_y);
        auto bounds3 = player_text.getGlobalBounds();
        player_text.setOrigin(bounds3.width/2, bounds3.height);
        player_text.setPosition(player_x,player_y);
        // width/2.0f, height/2.0f - 45


        player_text.setFillColor(sf::Color::Yellow);
        player_text.setCharacterSize(18);

        char cursor = '|';

        while (welcome_window.isOpen())
        {

            sf::Event event;
            while (welcome_window.pollEvent(event)) {

                if (event.type == sf::Event::KeyPressed){
                    if (event.key.code == sf::Keyboard::BackSpace){
                        cout << "delete pressed" << endl;
                        player_string.erase(player_string.getSize() - 1, 1);
                        final_string.erase(final_string.getSize() - 1, 1);
                        cout << (string) final_string << endl;
                        player_text.setString(player_string);
                    }
                    if(event.key.code == sf::Keyboard::Enter){
                        if(player_string.getSize() == 0){
                            cout << "enter but nothing in the string" << endl;
                        }
                        else{
                            cout << "Enter pressed" << endl;
                            welcome_window.close();
                            std::string filename = "files/board_config.cfg";

                            //std::fstream file1(filename, std::ios_base::in | std::ios_base::binary);
                            std::ifstream file1(filename);
                            if (file1.is_open()){

                                std::string line;
                                int count = 1;
                                int columns;
                                int rows;
                                int mines;
                                while (std::getline(file1, line)) {
                                    if (count == 1){
                                        columns = std::stoi(line);
                                        count += 1;
                                    }
                                    else if (count == 2){
                                        rows = std::stoi(line);
                                        count += 1;
                                    }
                                    else if (count == 3){
                                        mines = std::stoi(line);
                                        count += 1;
                                    }
                                }

                                int width = columns * 32;
                                int height = rows * 32;
                                height += 100;
                                int tile_count = columns * rows;


                                sf::RenderWindow window(sf::VideoMode(width, height), "SFML works!");
                                /*sf::CircleShape shape(100.f);
                                shape.setFillColor(sf::Color::Red);
                                sf::RectangleShape rect;
                                rect.setSize(sf::Vector2f(100,100));
                                rect.setPosition(0,0);
                                rect.setFillColor(sf::Color(150, 0, 100));*/


                                int mine_counter = mines;
                                vector<int> tiles_with_mines;
                                bool still_going = true;
                                while (still_going){
                                    if (mine_counter == 0){
                                        still_going = false;
                                    }
                                    else{
                                        int num = ::rand() % tile_count;
                                        auto it = find(tiles_with_mines.begin(), tiles_with_mines.end(), num);

                                        if (it != tiles_with_mines.end()){
                                            bool repeat = true;
                                            while (repeat) {
                                                int number = ::rand() % tile_count;
                                                auto it_2 = find(tiles_with_mines.begin(), tiles_with_mines.end(), number);
                                                if (it_2 == tiles_with_mines.end()){
                                                    num = number;
                                                    repeat = false;
                                                }
                                            }


                                        }
                                        tiles_with_mines.push_back(num);
                                        //cout << num << endl;
                                        mine_counter -= 1;
                                    }
                                }

                                std::vector<Tile> tiles;
                                std::vector<vector<Tile*>> tile_grid;
                                Tile this_one;
                                int x_value = 0;
                                int y_value = 0;



                                for (unsigned int i = 0; i < columns; ++i){

                                    auto it = find(tiles_with_mines.begin(), tiles_with_mines.end(), i);

                                    if (it != tiles_with_mines.end()){
                                        this_one.is_mine = true;
                                        this_one.hidden_texture.loadFromFile("files/images/mine.png");

                                    }
                                    else{
                                        this_one.is_mine = false;
                                        this_one.hidden_texture.loadFromFile("files/images/tile_revealed.png");

                                    }
                                    this_one.x_position = x_value;
                                    this_one.y_position = y_value;
                                    this_one.is_revealed = false;
                                    this_one.tile_texture.loadFromFile("files/images/tile_hidden.png");
                                    this_one.overlay.setTexture(this_one.hidden_texture);
                                    this_one.overlay.setPosition(x_value, y_value);
                                    this_one.tile_sprite.setTexture(this_one.tile_texture);
                                    this_one.tile_sprite.setPosition(x_value, y_value);

                                    tiles.push_back(this_one);
                                    //window.draw(this_one.tile_sprite);
                                    x_value += 32;
                                }
                                for(unsigned int i = columns; i < tile_count; ++i){
                                    //std::cout << i << std::endl;
                                    //std::cout << y_value << std::endl;
                                    if (i % columns == 0){
                                        x_value = 0;
                                        y_value += 32;
                                    }

                                    auto it = find(tiles_with_mines.begin(), tiles_with_mines.end(), i);

                                    if (it != tiles_with_mines.end()){

                                        this_one.is_mine = true;
                                        this_one.hidden_texture.loadFromFile("files/images/mine.png");
                                    }
                                    else{
                                        this_one.is_mine = false;
                                        this_one.hidden_texture.loadFromFile("files/images/tile_revealed.png");
                                    }
                                    this_one.x_position = x_value;
                                    this_one.y_position = y_value;
                                    this_one.tile_texture.loadFromFile("files/images/tile_hidden.png");
                                    this_one.overlay.setTexture(this_one.hidden_texture);
                                    this_one.overlay.setPosition(x_value, y_value);
                                    this_one.tile_sprite.setTexture(this_one.tile_texture);
                                    this_one.tile_sprite.setPosition(x_value, y_value);
                                    tiles.push_back(this_one);
                                    //window.draw(this_one.tile_sprite);
                                    x_value += 32;

                                }



                                sf::Texture debug_tex;
                                debug_tex.loadFromFile("files/images/debug.png");
                                sf::Sprite debug;
                                debug.setTexture(debug_tex);
                                float debug_x = (float) columns;
                                debug_x *= 32.0;
                                debug_x -= 304.0;
                                //std::cout << debug_x << std::endl;
                                float debug_y = (float) rows;
                                debug_y += 0.5f;
                                debug_y *= 32.0;
                                debug.setPosition(debug_x, debug_y);

                                sf::Texture happy_face_tex;
                                happy_face_tex.loadFromFile("files/images/face_happy.png");
                                sf::Sprite happy_face;
                                happy_face.setTexture(happy_face_tex);
                                float face_x = (float) columns;
                                face_x /= 2.0;
                                face_x *= 32.0;
                                float face_y = (float) rows;
                                face_y += 0.5f;
                                face_y *= 32.0;
                                happy_face.setPosition(face_x, face_y);

                                sf::Texture pause_tex;
                                pause_tex.loadFromFile("files/images/pause.png");
                                sf::Sprite pause_button;
                                pause_button.setTexture(pause_tex);
                                float pause_x = (float) columns;
                                pause_x *= 32.0;
                                pause_x -= 240.0;
                                float pause_y = (float) rows + 0.5f;
                                pause_y *= 32.0;
                                pause_button.setPosition(pause_x, pause_y);

                                sf::Texture leaderboard_tex;
                                leaderboard_tex.loadFromFile("files/images/leaderboard.png");
                                sf::Sprite leaderboard_button;
                                leaderboard_button.setTexture(leaderboard_tex);
                                float leader_x = (float) columns;
                                leader_x *= 32.0;
                                leader_x -= 176.0;
                                float leader_y = (float) rows;
                                leader_y += 0.5f;
                                leader_y *= 32.0;
                                leaderboard_button.setPosition(leader_x, leader_y);

                                bool debug_clicked = false;
                                bool leader_board_clicked = false;

                                //this is where the 2d vector comes in to create the grid
                                copy_tiles(tiles, tile_grid, rows, columns);
                                for(int l = 0; l < tile_grid.size(); ++l){
                                    for(int m = 0; m < tile_grid[l].size(); ++m){
                                        int number = check_neighbors(l, m, rows, columns, tile_grid);
                                        tile_grid[l][m]->value = number;
                                        if(tile_grid[l][m]->is_mine == false) {
                                            if (tile_grid[l][m]->value == 0) {
                                                tile_grid[l][m]->hidden_texture.loadFromFile("files/images/tile_revealed.png");
                                            }
                                            if(tile_grid[l][m]->value == 1){
                                                tile_grid[l][m]->hidden_texture.loadFromFile("files/images/number_1.png");
                                            }
                                            if(tile_grid[l][m]->value == 2){
                                                tile_grid[l][m]->hidden_texture.loadFromFile("files/images/number_2.png");
                                            }
                                            if(tile_grid[l][m]->value == 3){
                                                tile_grid[l][m]->hidden_texture.loadFromFile("files/images/number_3.png");
                                            }
                                            if(tile_grid[l][m]->value == 4){
                                                tile_grid[l][m]->hidden_texture.loadFromFile("files/images/number_4.png");
                                            }
                                            if(tile_grid[l][m]->value == 5){
                                                tile_grid[l][m]->hidden_texture.loadFromFile("files/images/number_5.png");
                                            }
                                            if(tile_grid[l][m]->value == 6){
                                                tile_grid[l][m]->hidden_texture.loadFromFile("files/images/number_6.png");
                                            }
                                            if(tile_grid[l][m]->value == 7){
                                                tile_grid[l][m]->hidden_texture.loadFromFile("files/images/number_7.png");
                                            }
                                            if(tile_grid[l][m]->value == 8){
                                                tile_grid[l][m]->hidden_texture.loadFromFile("files/images/number_8.png");
                                            }
                                        }

                                        if (tile_grid[l][m]->is_mine == false){
                                            if (tile_grid[l][m]->is_flagged == false){
                                                if (number == 0){
                                                    tile_grid[l][m]->is_blank = true;
                                                }
                                            }
                                        }
                                    }
                                }

                                sf::Clock clock;
                                bool paused_time = false;
                                int running_time = 0;
                                int save_time = 0;
                                int elapsed_time = 0;



                                //this is everything to load leaderboard

                                ifstream lead("files/leaderboard.txt", ios::in);
                                vector<string> the_leaderboard;
                                vector<string> names;

                                //lead.open("leaderboard.txt", ios::in);
                                if(lead.is_open()){
                                    string line;
                                    while(getline(lead, line)){

                                        size_t pos1 = line.find(",");
                                        string name = line.substr(pos1 + 1);
                                        names.push_back(name);
                                        string time = line.substr(0, pos1);
                                        size_t pos2 = line.find(":");
                                        string minutes = time.substr(0, pos2);
                                        int min = stoi(minutes);
                                        string seconds = time.substr(pos2 + 1, pos1);
                                        int sec = stoi(seconds);
                                        int full_time = (60 * min) + sec;
                                        times.push_back(full_time);

                                    }
                                }
                                for (int i  =0 ; i < times.size();i ++){
                                    leaderboardTimes.push_back(times[i]);
                                }

                                string leading;
                                for(int i = 0; i < 5; ++i){
                                    string row;
                                    string rank = to_string(i + 1);
                                    row += rank + "." + "\t";
                                    int time = times[i];
                                    int seconds = time % 60;
                                    int minutes = (time - seconds) / 60;
                                    if (minutes < 10){
                                        row += "0";
                                    }
                                    string mins = to_string(minutes);
                                    row += mins + ":";
                                    if (seconds < 10){
                                        row += "0";
                                    }
                                    string secs = to_string(seconds);
                                    row += secs + "\t" + names[i];
                                    leading += row + "\n\n";
                                }
                                sf::Font font1;
                                font1.loadFromFile("files/font.ttf");
                                sf::Text title;
                                title.setFont(font1);
                                title.setString("LEADERBOARD");
                                title.setCharacterSize(20);
                                title.setFillColor(sf::Color::White);
                                auto bounds3 = title.getGlobalBounds();
                                int leader_width = 16 * columns;
                                int leader_height = (rows * 16) + 50;
                                title.setOrigin(bounds3.width/2, bounds3.height);


                                title.setPosition(leader_width/2.0f,(leader_height/2.0f) - 120);
                                title.setStyle(sf::Text::Bold | sf::Text::Underlined);


                                sf::Text lineup;
                                lineup.setFont(font1);
                                lineup.setString(leading);
                                lineup.setCharacterSize(18);
                                lineup.setFillColor(sf::Color::White);
                                auto bounds_line = lineup.getGlobalBounds();
                                int line_x = leader_width / 2.0f;
                                int line_y = (leader_height / 2.0f) + 20;
                                lineup.setOrigin(bounds_line.width/2, bounds_line.height/2);

                                lineup.setPosition(line_x,line_y);
                                lineup.setStyle(sf::Text::Bold);

                                int goal_revealed = tile_count - mines;
                                int total_mines = mines;
                                int winning_time = 0;





                                while (window.isOpen())
                                {
                                    if (!paused_time){
                                        // always updating running time
                                        running_time = clock.getElapsedTime().asSeconds() + save_time;
                                    }
                                    else if (paused_time == true){
                                        save_time = running_time;
                                    }
                                    /*int count_revealed = 0;
                                    int flagged = 0;
                                    for(int i = 0; i < tile_grid.size(); ++i){
                                        for(int j = 0; j < tile_grid[i].size(); ++j){
                                            if(tile_grid[i][j]->is_mine&&tile_grid[i][j]->is_flagged){
                                                flagged += 1;
                                            }
                                            else{
                                                if(tile_grid[i][j]->is_revealed){
                                                    count_revealed += 1;
                                                }
                                            }
                                        }
                                    }*/
                                    if(game_won == true){
                                        game_won = true;
                                        paused_time = true;
                                        for(int i = 0; i < tile_count; i++){
                                            if(tiles[i].is_mine== true){
                                                tiles[i].tile_texture.loadFromFile("files/images/flag.png");
                                                tiles[i].display(window);
                                            }
                                        }
                                        happy_face_tex.loadFromFile("files/images/face_win.png");
                                        winning_time = running_time;

                                        int temp1 = 0;
                                        int temp2 = 0;
                                        string name1;
                                        string name2;
                                        bool readjust = false;
                                        vector<int> temp;
                                        vector<string> names_2;
                                        vector<string> names_5;
                                        for(int i = 0; i < leaderboardTimes.size(); ++i){
                                            if(readjust == true){
                                                temp.push_back(leaderboardTimes[i-1]);
                                                names_2.push_back(names[i]);
                                                names_5.push_back(names[i]);
                                            }
                                            else if(winning_time < leaderboardTimes[i]){
                                                temp.push_back(winning_time);
                                                names_2.push_back(final_string + "*");
                                                names_5.push_back(final_string);
                                                readjust = true;
                                            }
                                            else{
                                                cout << leaderboardTimes[i];
                                                temp.push_back(leaderboardTimes[i]);
                                                names_2.push_back(names[i]);
                                                names_5.push_back(names[i]);
                                            }
                                        }

                                        times = temp;
                                        cout << times.size() << endl;
                                        for (int i =0; i < times.size(); ++i){
                                            cout << times[i] << endl;
                                        }
                                        string leading = "";
                                        for(int i = 0; i < 5; ++i){
                                            string row;
                                            string rank = to_string(i + 1);
                                            row += rank + "." + "\t";
                                            int time = temp[i];
                                            int seconds = time % 60;
                                            int minutes = (time - seconds) / 60;
                                            if (minutes < 10){
                                                row += "0";
                                            }
                                            string mins = to_string(minutes);
                                            row += mins + ":";
                                            if (seconds < 10){
                                                row += "0";
                                            }
                                            string secs = to_string(seconds);
                                            row += secs + "\t" + names_2[i];
                                            leading += row + "\n\n";
                                        }
                                        lineup.setString(leading);
                                        vector<string> words;
                                        for(int i = 0; i < names_2.size(); ++i){
                                            string row;
                                            int sec;
                                            int min;
                                            if(times[i] > 60){
                                                sec = times[i] % 60;
                                                min = (times[i] - sec) / 60;
                                            }
                                            else{
                                                sec = times[i];
                                                min = 0;
                                            }
                                            if(min == 0){
                                                row += "00:";
                                                if(sec > 9){
                                                    row += to_string(sec);
                                                    row += ",";
                                                    row += names_5[i];
                                                }
                                                else{
                                                    row += "0";
                                                    row += to_string(sec);
                                                    row += ",";
                                                    row += names_5[i];
                                                }
                                            }
                                            else{
                                                if(min > 9){
                                                    row += to_string(min);
                                                    row += ":";
                                                    if(sec > 9){
                                                        row += to_string(sec);
                                                        row += ",";
                                                        row += names_5[i];
                                                    }
                                                    else{
                                                        row += "0";
                                                        row += to_string(sec);
                                                        row += ",";
                                                        row += names_5[i];
                                                    }
                                                }
                                                else{
                                                    row += "0";
                                                    row += to_string(min);
                                                    row += ":";
                                                    if(sec > 9){
                                                        row += to_string(sec);
                                                        row += ",";
                                                        row += names_5[i];
                                                    }
                                                    else{
                                                        row += "0";
                                                        row += to_string(sec);
                                                        row += ",";
                                                        row += names_5[i];
                                                    }
                                                }
                                            }
                                            words.push_back(row);
                                        }

                                        fstream file3("files/leaderboard.txt", ios_base::out);
                                        if(file3.is_open()){
                                            for(int i = 0; i < words.size(); ++i){
                                                cout << "wroteline" << endl;
                                                file3 << words[i] << endl;
                                            }
                                        }
                                    }

                                    if(game_lost == true){
                                        for(int i = 0; i < tile_grid.size(); ++i){
                                            for(int j = 0; j < tile_grid[i].size(); ++j){
                                                if(tile_grid[i][j]->is_mine){
                                                    game_lost = true;
                                                    paused_time = true;
                                                }
                                            }
                                        }
                                    }

                                    sf::Event event;
                                    while (window.pollEvent(event))
                                    {

                                        if (event.type == sf::Event::Closed)
                                            window.close();
                                        if (event.type == sf::Event::MouseButtonPressed){

                                            sf::Mouse mouse;
                                            auto coordinate = mouse.getPosition(window);
                                            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){
                                                if(game_won == true){
                                                    cout << "hi" << endl;
                                                    auto leader_bounds = leaderboard_button.getGlobalBounds();
                                                    if(leader_bounds.contains(coordinate.x, coordinate.y)){
                                                        cout << "leaderboard pressed" << endl;
                                                        leader_board_clicked = true;

                                                        for(int i = 0; i < tile_count; ++i){
                                                            tiles[i].tile_texture.loadFromFile("files/images/tile_revealed.png");
                                                            tiles[i].tile_sprite.setTexture(tiles[i].tile_texture);
                                                            tiles[i].display(window);
                                                        }
                                                        window.display();

                                                        if (paused_time == false){
                                                            paused_time = true;
                                                            pause_button.setTexture(pause_tex);
                                                            save_time = running_time;

                                                        }
                                                        else if (paused_time == true){
                                                            paused_time = false;
                                                            pause_button.setTexture(pause_tex);
                                                            int time_passed = clock.getElapsedTime().asSeconds();
                                                            elapsed_time = time_passed - save_time;
                                                            save_time = elapsed_time * -1;
                                                        }

                                                        sf::RenderWindow leaderboard_window(sf::VideoMode(leader_width, leader_height), "Leaderboard");
                                                        while (leaderboard_window.isOpen()) {
                                                            sf::Event event1;

                                                            while (leaderboard_window.pollEvent(event1)) {

                                                                if (event1.type == sf::Event::Closed){
                                                                    leaderboard_window.close();
                                                                    for(int i = 0; i < tile_count; ++i){
                                                                        if(tiles[i].is_revealed == true){
                                                                            tiles[i].tile_texture = tiles[i].hidden_texture;
                                                                            tiles[i].tile_sprite.setTexture(tiles[i].tile_texture);
                                                                        }
                                                                        else{
                                                                            tiles[i].tile_texture.loadFromFile("files/images/tile_hidden.png");
                                                                            tiles[i].tile_sprite.setTexture(tiles[i].tile_texture);
                                                                        }

                                                                        tiles[i].display(window);
                                                                    }
                                                                    window.display();

                                                                    cout << "close" << endl;

                                                                    if (paused_time == false){
                                                                        paused_time = true;
                                                                        pause_button.setTexture(pause_tex);
                                                                        save_time = running_time;
                                                                    }
                                                                    else if (paused_time == true){
                                                                        paused_time = false;
                                                                        pause_button.setTexture(pause_tex);
                                                                        int time_passed = clock.getElapsedTime().asSeconds();
                                                                        elapsed_time = time_passed - save_time;
                                                                        save_time = elapsed_time * -1;
                                                                    }

                                                                }


                                                            }

                                                            leaderboard_window.clear(sf::Color::Blue);
                                                            leaderboard_window.draw(title);
                                                            leaderboard_window.draw(lineup);
                                                            leaderboard_window.display();
                                                        }
                                                    }
                                                    auto reset_bounds = happy_face.getGlobalBounds();
                                                    if(reset_bounds.contains(coordinate.x, coordinate.y)){
                                                        cout << "pressed reset" << endl;
                                                        paused_time = false;
                                                        tiles_with_mines.clear();

                                                        tiles.clear();
                                                        tile_grid.clear();
                                                        clock.restart();
                                                        save_time = 0;
                                                        mine_counter = mines;

                                                        bool still_going = true;
                                                        while (still_going){
                                                            if (mine_counter == 0){
                                                                still_going = false;
                                                            }
                                                            else{
                                                                int num = ::rand() % tile_count;
                                                                auto it = find(tiles_with_mines.begin(), tiles_with_mines.end(), num);

                                                                if (it != tiles_with_mines.end()){
                                                                    bool repeat = true;
                                                                    while (repeat) {
                                                                        int number = ::rand() % tile_count;
                                                                        auto it_2 = find(tiles_with_mines.begin(), tiles_with_mines.end(), number);
                                                                        if (it_2 == tiles_with_mines.end()){
                                                                            num = number;
                                                                            repeat = false;
                                                                        }
                                                                    }


                                                                }
                                                                tiles_with_mines.push_back(num);
                                                                //cout << num << endl;
                                                                mine_counter -= 1;
                                                            }
                                                        }
                                                        Tile this_one;
                                                        int x_value = 0;
                                                        int y_value = 0;



                                                        for (unsigned int i = 0; i < columns; ++i){

                                                            auto it = find(tiles_with_mines.begin(), tiles_with_mines.end(), i);

                                                            if (it != tiles_with_mines.end()){
                                                                this_one.is_mine = true;
                                                                this_one.hidden_texture.loadFromFile("files/images/mine.png");

                                                            }
                                                            else{
                                                                this_one.is_mine = false;
                                                                this_one.hidden_texture.loadFromFile("files/images/tile_revealed.png");

                                                            }
                                                            this_one.x_position = x_value;
                                                            this_one.y_position = y_value;
                                                            this_one.is_revealed = false;
                                                            this_one.tile_texture.loadFromFile("files/images/tile_hidden.png");
                                                            this_one.overlay.setTexture(this_one.hidden_texture);
                                                            this_one.overlay.setPosition(x_value, y_value);
                                                            this_one.tile_sprite.setTexture(this_one.tile_texture);
                                                            this_one.tile_sprite.setPosition(x_value, y_value);

                                                            tiles.push_back(this_one);
                                                            //window.draw(this_one.tile_sprite);
                                                            x_value += 32;
                                                        }
                                                        for(unsigned int i = columns; i < tile_count; ++i){
                                                            //std::cout << i << std::endl;
                                                            //std::cout << y_value << std::endl;
                                                            if (i % columns == 0){
                                                                x_value = 0;
                                                                y_value += 32;
                                                            }

                                                            auto it = find(tiles_with_mines.begin(), tiles_with_mines.end(), i);

                                                            if (it != tiles_with_mines.end()){

                                                                this_one.is_mine = true;
                                                                this_one.hidden_texture.loadFromFile("files/images/mine.png");
                                                            }
                                                            else{
                                                                this_one.is_mine = false;
                                                                this_one.hidden_texture.loadFromFile("files/images/tile_revealed.png");
                                                            }
                                                            this_one.x_position = x_value;
                                                            this_one.y_position = y_value;
                                                            this_one.tile_texture.loadFromFile("files/images/tile_hidden.png");
                                                            this_one.overlay.setTexture(this_one.hidden_texture);
                                                            this_one.overlay.setPosition(x_value, y_value);
                                                            this_one.tile_sprite.setTexture(this_one.tile_texture);
                                                            this_one.tile_sprite.setPosition(x_value, y_value);
                                                            tiles.push_back(this_one);
                                                            //window.draw(this_one.tile_sprite);
                                                            x_value += 32;

                                                        }
                                                        copy_tiles(tiles, tile_grid, rows, columns);
                                                        for(int l = 0; l < tile_grid.size(); ++l){
                                                            for(int m = 0; m < tile_grid[l].size(); ++m){
                                                                int number = check_neighbors(l, m, rows, columns, tile_grid);
                                                                tile_grid[l][m]->value = number;
                                                                if(tile_grid[l][m]->is_mine == false) {
                                                                    if (tile_grid[l][m]->value == 0) {
                                                                        tile_grid[l][m]->hidden_texture.loadFromFile("files/images/tile_revealed.png");
                                                                    }
                                                                    if(tile_grid[l][m]->value == 1){
                                                                        tile_grid[l][m]->hidden_texture.loadFromFile("files/images/number_1.png");
                                                                    }
                                                                    if(tile_grid[l][m]->value == 2){
                                                                        tile_grid[l][m]->hidden_texture.loadFromFile("files/images/number_2.png");
                                                                    }
                                                                    if(tile_grid[l][m]->value == 3){
                                                                        tile_grid[l][m]->hidden_texture.loadFromFile("files/images/number_3.png");
                                                                    }
                                                                    if(tile_grid[l][m]->value == 4){
                                                                        tile_grid[l][m]->hidden_texture.loadFromFile("files/images/number_4.png");
                                                                    }
                                                                    if(tile_grid[l][m]->value == 5){
                                                                        tile_grid[l][m]->hidden_texture.loadFromFile("files/images/number_5.png");
                                                                    }
                                                                    if(tile_grid[l][m]->value == 6){
                                                                        tile_grid[l][m]->hidden_texture.loadFromFile("files/images/number_6.png");
                                                                    }
                                                                    if(tile_grid[l][m]->value == 7){
                                                                        tile_grid[l][m]->hidden_texture.loadFromFile("files/images/number_7.png");
                                                                    }
                                                                    if(tile_grid[l][m]->value == 8){
                                                                        tile_grid[l][m]->hidden_texture.loadFromFile("files/images/number_8.png");
                                                                    }
                                                                }

                                                                if (tile_grid[l][m]->is_mine == false){
                                                                    if (tile_grid[l][m]->is_flagged == false){
                                                                        if (number == 0){
                                                                            tile_grid[l][m]->is_blank = true;
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                        for(int i = 0; i < tile_count; ++i){
                                                            tiles[i].display(window);
                                                        }
                                                        happy_face_tex.loadFromFile("files/images/face_happy.png");
                                                        happy_face.setTexture(happy_face_tex);
                                                        game_won = false;
                                                        game_lost = false;


                                                    }


                                                }
                                                if(game_lost == true){
                                                    cout << "hi" << endl;
                                                    auto leader_bounds = leaderboard_button.getGlobalBounds();
                                                    if(leader_bounds.contains(coordinate.x, coordinate.y)){
                                                        cout << "leaderboard pressed" << endl;
                                                        leader_board_clicked = true;

                                                        for(int i = 0; i < tile_count; ++i){
                                                            tiles[i].tile_texture.loadFromFile("files/images/tile_revealed.png");
                                                            tiles[i].tile_sprite.setTexture(tiles[i].tile_texture);
                                                            tiles[i].display(window);
                                                        }
                                                        window.display();

                                                        if (paused_time == false){
                                                            paused_time = true;
                                                            pause_button.setTexture(pause_tex);
                                                            save_time = running_time;

                                                        }
                                                        else if (paused_time == true){
                                                            paused_time = false;
                                                            pause_button.setTexture(pause_tex);
                                                            int time_passed = clock.getElapsedTime().asSeconds();
                                                            elapsed_time = time_passed - save_time;
                                                            save_time = elapsed_time * -1;
                                                        }

                                                        sf::RenderWindow leaderboard_window(sf::VideoMode(leader_width, leader_height), "Leaderboard");
                                                        while (leaderboard_window.isOpen()) {
                                                            sf::Event event1;

                                                            while (leaderboard_window.pollEvent(event1)) {

                                                                if (event1.type == sf::Event::Closed){
                                                                    leaderboard_window.close();
                                                                    for(int i = 0; i < tile_count; ++i){
                                                                        if(tiles[i].is_revealed == true){
                                                                            tiles[i].tile_texture = tiles[i].hidden_texture;
                                                                            tiles[i].tile_sprite.setTexture(tiles[i].tile_texture);
                                                                        }
                                                                        else{
                                                                            tiles[i].tile_texture.loadFromFile("files/images/tile_hidden.png");
                                                                            tiles[i].tile_sprite.setTexture(tiles[i].tile_texture);
                                                                        }

                                                                        tiles[i].display(window);
                                                                    }
                                                                    window.display();

                                                                    cout << "close" << endl;

                                                                    if (paused_time == false){
                                                                        paused_time = true;
                                                                        pause_button.setTexture(pause_tex);
                                                                        save_time = running_time;
                                                                    }
                                                                    else if (paused_time == true){
                                                                        paused_time = false;
                                                                        pause_button.setTexture(pause_tex);
                                                                        int time_passed = clock.getElapsedTime().asSeconds();
                                                                        elapsed_time = time_passed - save_time;
                                                                        save_time = elapsed_time * -1;
                                                                    }

                                                                }


                                                            }

                                                            leaderboard_window.clear(sf::Color::Blue);
                                                            leaderboard_window.draw(title);
                                                            leaderboard_window.draw(lineup);
                                                            leaderboard_window.display();
                                                        }
                                                    }
                                                    auto reset_bounds = happy_face.getGlobalBounds();
                                                    if(reset_bounds.contains(coordinate.x, coordinate.y)){
                                                        cout << "pressed reset" << endl;
                                                        paused_time = false;
                                                        tiles_with_mines.clear();

                                                        tiles.clear();
                                                        tile_grid.clear();
                                                        clock.restart();
                                                        save_time = 0;
                                                        /*paused_time = false;
                                                        int running_time = 0;
                                                        int save_time = 0;
                                                        int elapsed_time = 0;*/


                                                        mine_counter = mines;

                                                        bool still_going = true;
                                                        while (still_going){
                                                            if (mine_counter == 0){
                                                                still_going = false;
                                                            }
                                                            else{
                                                                int num = ::rand() % tile_count;
                                                                auto it = find(tiles_with_mines.begin(), tiles_with_mines.end(), num);

                                                                if (it != tiles_with_mines.end()){
                                                                    bool repeat = true;
                                                                    while (repeat) {
                                                                        int number = ::rand() % tile_count;
                                                                        auto it_2 = find(tiles_with_mines.begin(), tiles_with_mines.end(), number);
                                                                        if (it_2 == tiles_with_mines.end()){
                                                                            num = number;
                                                                            repeat = false;
                                                                        }
                                                                    }


                                                                }
                                                                tiles_with_mines.push_back(num);
                                                                //cout << num << endl;
                                                                mine_counter -= 1;
                                                            }
                                                        }
                                                        Tile this_one;
                                                        int x_value = 0;
                                                        int y_value = 0;



                                                        for (unsigned int i = 0; i < columns; ++i){

                                                            auto it = find(tiles_with_mines.begin(), tiles_with_mines.end(), i);

                                                            if (it != tiles_with_mines.end()){
                                                                this_one.is_mine = true;
                                                                this_one.hidden_texture.loadFromFile("files/images/mine.png");

                                                            }
                                                            else{
                                                                this_one.is_mine = false;
                                                                this_one.hidden_texture.loadFromFile("files/images/tile_revealed.png");

                                                            }
                                                            this_one.x_position = x_value;
                                                            this_one.y_position = y_value;
                                                            this_one.is_revealed = false;
                                                            this_one.tile_texture.loadFromFile("files/images/tile_hidden.png");
                                                            this_one.overlay.setTexture(this_one.hidden_texture);
                                                            this_one.overlay.setPosition(x_value, y_value);
                                                            this_one.tile_sprite.setTexture(this_one.tile_texture);
                                                            this_one.tile_sprite.setPosition(x_value, y_value);

                                                            tiles.push_back(this_one);
                                                            //window.draw(this_one.tile_sprite);
                                                            x_value += 32;
                                                        }
                                                        for(unsigned int i = columns; i < tile_count; ++i){
                                                            //std::cout << i << std::endl;
                                                            //std::cout << y_value << std::endl;
                                                            if (i % columns == 0){
                                                                x_value = 0;
                                                                y_value += 32;
                                                            }

                                                            auto it = find(tiles_with_mines.begin(), tiles_with_mines.end(), i);

                                                            if (it != tiles_with_mines.end()){

                                                                this_one.is_mine = true;
                                                                this_one.hidden_texture.loadFromFile("files/images/mine.png");
                                                            }
                                                            else{
                                                                this_one.is_mine = false;
                                                                this_one.hidden_texture.loadFromFile("files/images/tile_revealed.png");
                                                            }
                                                            this_one.x_position = x_value;
                                                            this_one.y_position = y_value;
                                                            this_one.tile_texture.loadFromFile("files/images/tile_hidden.png");
                                                            this_one.overlay.setTexture(this_one.hidden_texture);
                                                            this_one.overlay.setPosition(x_value, y_value);
                                                            this_one.tile_sprite.setTexture(this_one.tile_texture);
                                                            this_one.tile_sprite.setPosition(x_value, y_value);
                                                            tiles.push_back(this_one);
                                                            //window.draw(this_one.tile_sprite);
                                                            x_value += 32;

                                                        }
                                                        copy_tiles(tiles, tile_grid, rows, columns);
                                                        for(int l = 0; l < tile_grid.size(); ++l){
                                                            for(int m = 0; m < tile_grid[l].size(); ++m){
                                                                int number = check_neighbors(l, m, rows, columns, tile_grid);
                                                                tile_grid[l][m]->value = number;
                                                                if(tile_grid[l][m]->is_mine == false) {
                                                                    if (tile_grid[l][m]->value == 0) {
                                                                        tile_grid[l][m]->hidden_texture.loadFromFile("files/images/tile_revealed.png");
                                                                    }
                                                                    if(tile_grid[l][m]->value == 1){
                                                                        tile_grid[l][m]->hidden_texture.loadFromFile("files/images/number_1.png");
                                                                    }
                                                                    if(tile_grid[l][m]->value == 2){
                                                                        tile_grid[l][m]->hidden_texture.loadFromFile("files/images/number_2.png");
                                                                    }
                                                                    if(tile_grid[l][m]->value == 3){
                                                                        tile_grid[l][m]->hidden_texture.loadFromFile("files/images/number_3.png");
                                                                    }
                                                                    if(tile_grid[l][m]->value == 4){
                                                                        tile_grid[l][m]->hidden_texture.loadFromFile("files/images/number_4.png");
                                                                    }
                                                                    if(tile_grid[l][m]->value == 5){
                                                                        tile_grid[l][m]->hidden_texture.loadFromFile("files/images/number_5.png");
                                                                    }
                                                                    if(tile_grid[l][m]->value == 6){
                                                                        tile_grid[l][m]->hidden_texture.loadFromFile("files/images/number_6.png");
                                                                    }
                                                                    if(tile_grid[l][m]->value == 7){
                                                                        tile_grid[l][m]->hidden_texture.loadFromFile("files/images/number_7.png");
                                                                    }
                                                                    if(tile_grid[l][m]->value == 8){
                                                                        tile_grid[l][m]->hidden_texture.loadFromFile("files/images/number_8.png");
                                                                    }
                                                                }

                                                                if (tile_grid[l][m]->is_mine == false){
                                                                    if (tile_grid[l][m]->is_flagged == false){
                                                                        if (number == 0){
                                                                            tile_grid[l][m]->is_blank = true;
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                        for(int i = 0; i < tile_count; ++i){
                                                            tiles[i].display(window);
                                                        }
                                                        happy_face_tex.loadFromFile("files/images/face_happy.png");
                                                        happy_face.setTexture(happy_face_tex);
                                                        game_won = false;
                                                        game_lost = false;



                                                    }
                                                }
                                                else{
                                                    auto leader_bounds = leaderboard_button.getGlobalBounds();
                                                    if(leader_bounds.contains(coordinate.x, coordinate.y)){
                                                        cout << "leaderboard pressed" << endl;
                                                        leader_board_clicked = true;

                                                        for(int i = 0; i < tile_count; ++i){
                                                            tiles[i].tile_texture.loadFromFile("files/images/tile_revealed.png");
                                                            tiles[i].tile_sprite.setTexture(tiles[i].tile_texture);
                                                            tiles[i].display(window);
                                                        }
                                                        window.display();

                                                        if (paused_time == false){
                                                            paused_time = true;
                                                            pause_button.setTexture(pause_tex);
                                                            save_time = running_time;

                                                        }
                                                        else if (paused_time == true){
                                                            paused_time = false;
                                                            pause_button.setTexture(pause_tex);
                                                            int time_passed = clock.getElapsedTime().asSeconds();
                                                            elapsed_time = time_passed - save_time;
                                                            save_time = elapsed_time * -1;
                                                        }

                                                        sf::RenderWindow leaderboard_window(sf::VideoMode(leader_width, leader_height), "Leaderboard");
                                                        while (leaderboard_window.isOpen()) {
                                                            sf::Event event1;

                                                            while (leaderboard_window.pollEvent(event1)) {

                                                                if (event1.type == sf::Event::Closed){
                                                                    leaderboard_window.close();
                                                                    for(int i = 0; i < tile_count; ++i){
                                                                        if(tiles[i].is_revealed == true){
                                                                            tiles[i].tile_texture = tiles[i].hidden_texture;
                                                                            tiles[i].tile_sprite.setTexture(tiles[i].tile_texture);
                                                                        }
                                                                        else{
                                                                            tiles[i].tile_texture.loadFromFile("files/images/tile_hidden.png");
                                                                            tiles[i].tile_sprite.setTexture(tiles[i].tile_texture);
                                                                        }

                                                                        tiles[i].display(window);
                                                                    }
                                                                    window.display();

                                                                    cout << "close" << endl;

                                                                    if (paused_time == false){
                                                                        paused_time = true;
                                                                        pause_button.setTexture(pause_tex);
                                                                        save_time = running_time;
                                                                    }
                                                                    else if (paused_time == true){
                                                                        paused_time = false;
                                                                        pause_button.setTexture(pause_tex);
                                                                        int time_passed = clock.getElapsedTime().asSeconds();
                                                                        elapsed_time = time_passed - save_time;
                                                                        save_time = elapsed_time * -1;
                                                                    }

                                                                }


                                                            }

                                                            leaderboard_window.clear(sf::Color::Blue);
                                                            leaderboard_window.draw(title);
                                                            leaderboard_window.draw(lineup);
                                                            leaderboard_window.display();
                                                        }
                                                    }
                                                    auto debug_bounds = debug.getGlobalBounds();

                                                    if(debug_bounds.contains(coordinate.x, coordinate.y)){
                                                        if(game_lost or game_won){
                                                            debug_clicked = true;
                                                        }
                                                        if(debug_clicked == false){
                                                            //cout << debug_clicked << endl;
                                                            for (unsigned int i = 0; i < tiles.size(); ++i){

                                                                if(tiles[i].is_mine == true){

                                                                    tiles[i].tile_texture = tiles[i].hidden_texture;
                                                                    tiles[i].display(window);
                                                                }
                                                                else{
                                                                    if(tiles[i].is_revealed == true){
                                                                        tiles[i].display(window);
                                                                    }
                                                                    else{
                                                                        tiles[i].tile_texture = tiles[i].tile_texture;
                                                                        tiles[i].display(window);
                                                                    }
                                                                }
                                                            }
                                                            debug_clicked = true;
                                                        }
                                                        else if(debug_clicked == true){
                                                            //cout << "true" << endl;
                                                            for (unsigned int i = 0; i < tiles.size(); ++i){
                                                                if(tiles[i].is_revealed == true){
                                                                    tiles[i].display(window);
                                                                }
                                                                else{
                                                                    if(tiles[i].is_flagged == true){
                                                                        tiles[i].tile_texture.loadFromFile("files/images/flag.png");
                                                                    }
                                                                    else{
                                                                        tiles[i].tile_texture.loadFromFile("files/images/tile_hidden.png");
                                                                    }
                                                                    //tiles[i].tile_texture.loadFromFile("files/images/tile_hidden.png");
                                                                    tiles[i].display(window);
                                                                }


                                                            }
                                                            debug_clicked = false;
                                                        }
                                                    }

                                                    auto reset_bounds = happy_face.getGlobalBounds();
                                                    if(reset_bounds.contains(coordinate.x, coordinate.y)){
                                                        cout << "pressed reset" << endl;
                                                        tiles_with_mines.clear();
                                                        tiles.clear();
                                                        tile_grid.clear();
                                                        clock.restart();
                                                        mine_counter = mines;

                                                        bool still_going = true;
                                                        while (still_going){
                                                            if (mine_counter == 0){
                                                                still_going = false;
                                                            }
                                                            else{
                                                                int num = ::rand() % tile_count;
                                                                auto it = find(tiles_with_mines.begin(), tiles_with_mines.end(), num);

                                                                if (it != tiles_with_mines.end()){
                                                                    bool repeat = true;
                                                                    while (repeat) {
                                                                        int number = ::rand() % tile_count;
                                                                        auto it_2 = find(tiles_with_mines.begin(), tiles_with_mines.end(), number);
                                                                        if (it_2 == tiles_with_mines.end()){
                                                                            num = number;
                                                                            repeat = false;
                                                                        }
                                                                    }


                                                                }
                                                                tiles_with_mines.push_back(num);
                                                                //cout << num << endl;
                                                                mine_counter -= 1;
                                                            }
                                                        }
                                                        Tile this_one;
                                                        int x_value = 0;
                                                        int y_value = 0;



                                                        for (unsigned int i = 0; i < columns; ++i){

                                                            auto it = find(tiles_with_mines.begin(), tiles_with_mines.end(), i);

                                                            if (it != tiles_with_mines.end()){
                                                                this_one.is_mine = true;
                                                                this_one.hidden_texture.loadFromFile("files/images/mine.png");

                                                            }
                                                            else{
                                                                this_one.is_mine = false;
                                                                this_one.hidden_texture.loadFromFile("files/images/tile_revealed.png");

                                                            }
                                                            this_one.x_position = x_value;
                                                            this_one.y_position = y_value;
                                                            this_one.is_revealed = false;
                                                            this_one.tile_texture.loadFromFile("files/images/tile_hidden.png");
                                                            this_one.overlay.setTexture(this_one.hidden_texture);
                                                            this_one.overlay.setPosition(x_value, y_value);
                                                            this_one.tile_sprite.setTexture(this_one.tile_texture);
                                                            this_one.tile_sprite.setPosition(x_value, y_value);

                                                            tiles.push_back(this_one);
                                                            //window.draw(this_one.tile_sprite);
                                                            x_value += 32;
                                                        }
                                                        for(unsigned int i = columns; i < tile_count; ++i){
                                                            //std::cout << i << std::endl;
                                                            //std::cout << y_value << std::endl;
                                                            if (i % columns == 0){
                                                                x_value = 0;
                                                                y_value += 32;
                                                            }

                                                            auto it = find(tiles_with_mines.begin(), tiles_with_mines.end(), i);

                                                            if (it != tiles_with_mines.end()){

                                                                this_one.is_mine = true;
                                                                this_one.hidden_texture.loadFromFile("files/images/mine.png");
                                                            }
                                                            else{
                                                                this_one.is_mine = false;
                                                                this_one.hidden_texture.loadFromFile("files/images/tile_revealed.png");
                                                            }
                                                            this_one.x_position = x_value;
                                                            this_one.y_position = y_value;
                                                            this_one.tile_texture.loadFromFile("files/images/tile_hidden.png");
                                                            this_one.overlay.setTexture(this_one.hidden_texture);
                                                            this_one.overlay.setPosition(x_value, y_value);
                                                            this_one.tile_sprite.setTexture(this_one.tile_texture);
                                                            this_one.tile_sprite.setPosition(x_value, y_value);
                                                            tiles.push_back(this_one);
                                                            //window.draw(this_one.tile_sprite);
                                                            x_value += 32;

                                                        }
                                                        copy_tiles(tiles, tile_grid, rows, columns);
                                                        for(int l = 0; l < tile_grid.size(); ++l){
                                                            for(int m = 0; m < tile_grid[l].size(); ++m){
                                                                int number = check_neighbors(l, m, rows, columns, tile_grid);
                                                                tile_grid[l][m]->value = number;
                                                                if(tile_grid[l][m]->is_mine == false) {
                                                                    if (tile_grid[l][m]->value == 0) {
                                                                        tile_grid[l][m]->hidden_texture.loadFromFile("files/images/tile_revealed.png");
                                                                    }
                                                                    if(tile_grid[l][m]->value == 1){
                                                                        tile_grid[l][m]->hidden_texture.loadFromFile("files/images/number_1.png");
                                                                    }
                                                                    if(tile_grid[l][m]->value == 2){
                                                                        tile_grid[l][m]->hidden_texture.loadFromFile("files/images/number_2.png");
                                                                    }
                                                                    if(tile_grid[l][m]->value == 3){
                                                                        tile_grid[l][m]->hidden_texture.loadFromFile("files/images/number_3.png");
                                                                    }
                                                                    if(tile_grid[l][m]->value == 4){
                                                                        tile_grid[l][m]->hidden_texture.loadFromFile("files/images/number_4.png");
                                                                    }
                                                                    if(tile_grid[l][m]->value == 5){
                                                                        tile_grid[l][m]->hidden_texture.loadFromFile("files/images/number_5.png");
                                                                    }
                                                                    if(tile_grid[l][m]->value == 6){
                                                                        tile_grid[l][m]->hidden_texture.loadFromFile("files/images/number_6.png");
                                                                    }
                                                                    if(tile_grid[l][m]->value == 7){
                                                                        tile_grid[l][m]->hidden_texture.loadFromFile("files/images/number_7.png");
                                                                    }
                                                                    if(tile_grid[l][m]->value == 8){
                                                                        tile_grid[l][m]->hidden_texture.loadFromFile("files/images/number_8.png");
                                                                    }
                                                                }

                                                                if (tile_grid[l][m]->is_mine == false){
                                                                    if (tile_grid[l][m]->is_flagged == false){
                                                                        if (number == 0){
                                                                            tile_grid[l][m]->is_blank = true;
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                        for(int i = 0; i < tile_count; ++i){
                                                            tiles[i].display(window);
                                                        }
                                                        happy_face_tex.loadFromFile("files/images/face_happy.png");
                                                        happy_face.setTexture(happy_face_tex);



                                                    }

                                                    auto pause_bounds = pause_button.getGlobalBounds();
                                                    if(pause_bounds.contains(coordinate.x, coordinate.y)){

                                                        if (game_won or game_lost){
                                                            paused_time = true;
                                                            save_time = running_time;
                                                        }
                                                        else if (paused_time == false){
                                                            paused_time = true;
                                                            pause_tex.loadFromFile("files/images/play.png");
                                                            pause_button.setTexture(pause_tex);
                                                            save_time = running_time;
                                                        }
                                                        else if (paused_time == true){
                                                            paused_time = false;
                                                            pause_tex.loadFromFile("files/images/pause.png");
                                                            pause_button.setTexture(pause_tex);
                                                            int time_passed = clock.getElapsedTime().asSeconds();
                                                            elapsed_time = time_passed - save_time;
                                                            save_time = elapsed_time * -1;
                                                        }
                                                    }

                                                    for(int i =0; i < tile_grid.size(); i++){
                                                        for(int j = 0; j < tile_grid[i].size(); j++){


                                                            auto tile_bounds = tile_grid[i][j]->tile_sprite.getGlobalBounds();

                                                            if(tile_bounds.contains(coordinate.x, coordinate.y)){
                                                                if(tile_grid[i][j]->is_mine == true){
                                                                    for(int i = 0; i < tile_grid.size(); ++i){
                                                                        for(int j = 0; j < tile_grid[i].size(); ++j){
                                                                            if(tile_grid[i][j]->is_mine){
                                                                                tile_grid[i][j]->tile_texture.loadFromFile("files/images/mine.png");
                                                                                tile_grid[i][j]->display(window);
                                                                                /*tile_grid[i][j]->hidden_texture.loadFromFile("files/images/mine.png");
                                                                                window.draw(tile_grid[i][j]->overlay);*/
                                                                            }
                                                                        }
                                                                    }
                                                                    game_won = false;
                                                                    game_lost = true;
                                                                    happy_face_tex.loadFromFile("files/images/face_lose.png");
                                                                    paused_time = true;

                                                                }
                                                                if(tile_grid[i][j]->value > 0){
                                                                    if(tile_grid[i][j]->value < 10){
                                                                        int mines_around = check_neighbors(i, j, rows, columns, tile_grid);
                                                                        //cout << mines_around << endl;
                                                                        if(mines_around == 1){
                                                                            tile_grid[i][j]->tile_texture.loadFromFile("files/images/number_1.png");
                                                                            tile_grid[i][j]->is_revealed = true;

                                                                        }
                                                                        if(mines_around == 2){
                                                                            tile_grid[i][j]->tile_texture.loadFromFile("files/images/number_2.png");
                                                                            tile_grid[i][j]->is_revealed = true;
                                                                        }
                                                                        if(mines_around == 3){
                                                                            tile_grid[i][j]->tile_texture.loadFromFile("files/images/number_3.png");
                                                                            tile_grid[i][j]->is_revealed = true;
                                                                        }
                                                                        if(mines_around == 4){
                                                                            tile_grid[i][j]->tile_texture.loadFromFile("files/images/number_4.png");
                                                                            tile_grid[i][j]->is_revealed = true;
                                                                        }
                                                                        if(mines_around == 5){
                                                                            tile_grid[i][j]->tile_texture.loadFromFile("files/images/number_5.png");
                                                                            tile_grid[i][j]->is_revealed = true;
                                                                        }
                                                                        if(mines_around == 6){
                                                                            tile_grid[i][j]->tile_texture.loadFromFile("files/images/number_6.png");
                                                                            tile_grid[i][j]->is_revealed = true;
                                                                        }
                                                                        if(mines_around == 7){
                                                                            tile_grid[i][j]->tile_texture.loadFromFile("files/images/number_7.png");
                                                                            tile_grid[i][j]->is_revealed = true;
                                                                        }
                                                                        if(mines_around == 8){
                                                                            tile_grid[i][j]->tile_texture.loadFromFile("files/images/number_8.png");
                                                                            tile_grid[i][j]->is_revealed = true;
                                                                        }
                                                                        if(mines_around == 0){
                                                                            tile_grid[i][j]->tile_texture.loadFromFile("files/images/tile_revealed.png");
                                                                            tile_grid[i][j]->is_revealed = true;
                                                                        }
                                                                        //tile_grid[i][j]->is_revealed = true;
                                                                        tile_grid[i][j]->display(window);
                                                                        //window.draw(tile_grid[i][j]->overlay);

                                                                        bool win = checkwin(goal_revealed, total_mines, tile_grid, window);
                                                                        if(win){
                                                                            game_won = true;
                                                                            paused_time = true;
                                                                            happy_face_tex.loadFromFile("files/images/face_win.png");
                                                                            winning_time = save_time;
                                                                            cout << winning_time << endl;
                                                                            int temp1;
                                                                            int temp2;
                                                                            bool readjust = false;
                                                                            for(int i = 0; i < times.size(); ++i){
                                                                                if(readjust == true){
                                                                                    temp2 = times[i];
                                                                                    times[i] = temp1;
                                                                                    temp1 = temp2;
                                                                                }
                                                                                else if(winning_time < times[i]){
                                                                                    temp1 = times[i];
                                                                                    times[i] = winning_time;
                                                                                }
                                                                            }
                                                                            for(int i = 0; i < times.size(); ++i){
                                                                                cout << times[i] << endl;
                                                                            }

                                                                        }
                                                                    }
                                                                }
                                                                else{
                                                                    cout << "i " << i << " j " << j << endl;
                                                                    //cout << j << endl;
                                                                    checkzero(i, j, rows, columns, tile_grid);
                                                                    for(int l = 0; l < tile_grid.size(); ++l){
                                                                        for(int m = 0; m < tile_grid[l].size(); ++m){
                                                                            if (tile_grid[l][m]->is_visited == true){
                                                                                int mines_around = check_neighbors(l, m, rows, columns, tile_grid);
                                                                                cout << mines_around << endl;

                                                                                //cout << mines_around << endl;
                                                                                if(mines_around == 1){
                                                                                    tile_grid[l][m]->tile_texture.loadFromFile("files/images/number_1.png");
                                                                                    tile_grid[l][m]->is_revealed = true;

                                                                                }
                                                                                if(mines_around == 2){
                                                                                    tile_grid[l][m]->tile_texture.loadFromFile("files/images/number_2.png");
                                                                                    tile_grid[l][m]->is_revealed = true;
                                                                                }
                                                                                if(mines_around == 3){
                                                                                    tile_grid[i][j]->tile_texture.loadFromFile("files/images/number_3.png");
                                                                                    tile_grid[i][j]->is_revealed = true;
                                                                                }
                                                                                if(mines_around == 4){
                                                                                    tile_grid[i][j]->tile_texture.loadFromFile("files/images/number_4.png");
                                                                                    tile_grid[i][j]->is_revealed = true;
                                                                                }
                                                                                if(mines_around == 5){
                                                                                    tile_grid[i][j]->tile_texture.loadFromFile("files/images/number_5.png");
                                                                                    tile_grid[i][j]->is_revealed = true;
                                                                                }
                                                                                if(mines_around == 6){
                                                                                    tile_grid[i][j]->tile_texture.loadFromFile("files/images/number_6.png");
                                                                                    tile_grid[i][j]->is_revealed = true;
                                                                                }
                                                                                if(mines_around == 7){
                                                                                    tile_grid[i][j]->tile_texture.loadFromFile("files/images/number_7.png");
                                                                                    tile_grid[i][j]->is_revealed = true;
                                                                                }
                                                                                if(mines_around == 8){
                                                                                    tile_grid[i][j]->tile_texture.loadFromFile("files/images/number_8.png");
                                                                                    tile_grid[i][j]->is_revealed = true;
                                                                                }
                                                                                if(mines_around == 0){
                                                                                    tile_grid[l][m]->tile_texture.loadFromFile("files/images/tile_revealed.png");
                                                                                    tile_grid[l][m]->is_revealed = true;
                                                                                }
                                                                                tile_grid[l][m]->display(window);
                                                                                checknum(l, m, rows, columns, tile_grid, window);
                                                                                tile_grid[i][j]->display(window);


                                                                                bool win = checkwin(goal_revealed, total_mines, tile_grid, window);
                                                                                if(win){
                                                                                    game_won = true;
                                                                                    paused_time = true;
                                                                                    happy_face_tex.loadFromFile("files/images/face_win.png");
                                                                                    winning_time = save_time;
                                                                                    cout << winning_time << endl;
                                                                                    int temp1;
                                                                                    int temp2;
                                                                                    bool readjust = false;
                                                                                    for(int i = 0; i < times.size(); ++i){
                                                                                        if(readjust == true){
                                                                                            temp2 = times[i];
                                                                                            times[i] = temp1;
                                                                                            temp1 = temp2;
                                                                                        }
                                                                                        else if(winning_time < times[i]){
                                                                                            temp1 = times[i];
                                                                                            times[i] = winning_time;
                                                                                        }
                                                                                    }
                                                                                    for(int i = 0; i < times.size(); ++i){
                                                                                        cout << times[i] << endl;
                                                                                    }
                                                                                }
                                                                            }
                                                                        }
                                                                    }
                                                                }

                                                            }
                                                        }
                                                    }
                                                }



                                                /*for(unsigned int i = 0; i < tiles.size(); ++i){
                                                    auto tile_bounds = tiles[i].tile_sprite.getGlobalBounds();
                                                    if (tile_bounds.contains(coordinate.x, coordinate.y)){
                                                        tiles[i].tile_texture = tiles[i].hidden_texture;
                                                        tiles[i].display(window);
                                                    }
                                                }*/

                                            }
                                            if(sf::Mouse::isButtonPressed(sf::Mouse::Right)){
                                                //this is if the right button is pressed



                                                for(unsigned int i = 0; i < tiles.size(); ++i){
                                                    auto tile_bounds = tiles[i].tile_sprite.getGlobalBounds();
                                                    if (tile_bounds.contains(coordinate.x, coordinate.y)){
                                                        if (tiles[i].is_flagged){
                                                            tiles[i].tile_texture.loadFromFile("files/images/tile_hidden.png");
                                                            tiles[i].is_flagged = false;
                                                            mines += 1;
                                                            int goal_revealed = tile_count - mines;
                                                            bool win = checkwin(goal_revealed, mines, tile_grid, window);
                                                            if(win){
                                                                game_won = true;
                                                                paused_time = true;
                                                                happy_face_tex.loadFromFile("files/images/face_win.png");
                                                                winning_time = save_time;
                                                                cout << winning_time << endl;
                                                                int temp1;
                                                                int temp2;
                                                                bool readjust = false;
                                                                for(int i = 0; i < times.size(); ++i){
                                                                    if(readjust == true){
                                                                        temp2 = times[i];
                                                                        times[i] = temp1;
                                                                        temp1 = temp2;
                                                                    }
                                                                    else if(winning_time < times[i]){
                                                                        temp1 = times[i];
                                                                        times[i] = winning_time;
                                                                    }
                                                                }

                                                            }
                                                            //cout << mines << endl;
                                                        }
                                                        else{
                                                            tiles[i].tile_texture.loadFromFile("files/images/flag.png");
                                                            tiles[i].is_flagged = true;
                                                            mines-= 1;

                                                            bool win = checkwin(goal_revealed, total_mines, tile_grid, window);
                                                            if(win){
                                                                game_won = true;


                                                            }
                                                            //cout << mines << endl;
                                                        }
                                                        tiles[i].display(window);
                                                        //cout << i << endl;
                                                    }
                                                }


                                            }


                                        }

                                    }
                                    window.clear(sf::Color::White);



                                    // for tiles
                                    for(unsigned int i = 0; i < tiles.size(); ++i){
                                        window.draw(tiles[i].tile_sprite);
                                    }

                                    window.draw(debug);
                                    window.draw(happy_face);

                                    //counter
                                    int count = mines;
                                    //cout << count << endl;
                                    sf::Texture digits;
                                    digits.loadFromFile("files/images/digits.png");

                                    sf::Sprite counter_1(digits,sf::IntRect(0,0,21,32));
                                    sf::Sprite counter_2;
                                    sf::Sprite counter_3;

                                    if(count < 0){
                                        sf::Sprite negative(digits,sf::IntRect(210,0,21,32));
                                        counter_1 = negative;
                                        count /= -1;
                                    }
                                    if (count < 10){
                                        sf::Sprite digit_2(digits,sf::IntRect((0 * 21),0,21,32));
                                        counter_2 = digit_2;
                                    }
                                    else{
                                        int first_digit = (count / 10) % 10;
                                        sf::Sprite digit_2(digits,sf::IntRect((first_digit * 21),0,21,32));
                                        counter_2 = digit_2;
                                    }
                                    int second_digit = count % 10;
                                    sf::Sprite digit_two(digits,sf::IntRect((second_digit * 21),0,21,32));
                                    counter_3 = digit_two;



                                    float counter_position_one_y = (float) rows;
                                    counter_position_one_y = 32 * ((counter_position_one_y)+0.5f)+16;

                                    counter_1.setPosition(33.0, counter_position_one_y);

                                    counter_2.setPosition(54, counter_position_one_y);

                                    counter_3.setPosition(75, counter_position_one_y);

                                    //counter_digits(mine_counter, counter_1, counter_2, counter_3);

                                    window.draw(counter_1);
                                    window.draw(counter_2);
                                    window.draw(counter_3);

                                    //clock

                                    sf::Sprite time_1;
                                    sf::Sprite time_2;
                                    sf::Sprite time_3;
                                    sf::Sprite time_4;
                                    if (!paused_time){
                                        if (running_time >= 60){
                                            float mins = running_time / 60;
                                            int minutes = ::floor(mins);
                                            if (minutes > 9) {
                                                int first_minute = (minutes / 10) % 10;
                                                sf::Sprite min_1(digits, sf::IntRect((first_minute * 21), 0, 21, 32));
                                                time_1 = min_1;
                                            }
                                            else{
                                                sf::Sprite min_1(digits, sf::IntRect((0 * 21), 0, 21, 32));
                                                time_1 = min_1;
                                            }
                                            minutes = minutes % 10;
                                            sf::Sprite min_2(digits, sf::IntRect((minutes * 21), 0, 21, 32));
                                            time_2 = min_2;
                                            int seconds = running_time - (minutes * 60);
                                            if (seconds > 9) {
                                                int first_sec = (seconds / 10) % 10;
                                                sf::Sprite sec_1(digits, sf::IntRect((first_sec * 21), 0, 21, 32));
                                                time_3 = sec_1;
                                            }
                                            else{
                                                sf::Sprite sec_1(digits, sf::IntRect((0 * 21), 0, 21, 32));
                                                time_3 = sec_1;
                                            }
                                            seconds = seconds % 10;
                                            sf::Sprite sec_2(digits, sf::IntRect((seconds * 21), 0, 21, 32));
                                            time_4 = sec_2;

                                        }
                                        else{
                                            sf::Sprite min_1(digits, sf::IntRect((0 * 21), 0, 21, 32));
                                            time_1 = min_1;
                                            sf::Sprite min_2(digits, sf::IntRect((0 * 21), 0, 21, 32));
                                            time_2 = min_2;
                                            int seconds = running_time;
                                            if (seconds > 9) {
                                                int first_sec = (seconds / 10) % 10;
                                                sf::Sprite sec_1(digits, sf::IntRect((first_sec * 21), 0, 21, 32));
                                                time_3 = sec_1;
                                            }
                                            else{
                                                sf::Sprite sec_1(digits, sf::IntRect((0 * 21), 0, 21, 32));
                                                time_3 = sec_1;
                                            }
                                            seconds = seconds % 10;
                                            sf::Sprite sec_2(digits, sf::IntRect((seconds * 21), 0, 21, 32));
                                            time_4 = sec_2;
                                        }
                                    }
                                    else if (paused_time == true){
                                        if (save_time >= 60){
                                            float mins = save_time / 60;
                                            int minutes = ::floor(mins);
                                            if (minutes > 9) {
                                                int first_minute = (minutes / 10) % 10;
                                                sf::Sprite min_1(digits, sf::IntRect((first_minute * 21), 0, 21, 32));
                                                time_1 = min_1;
                                            }
                                            else{
                                                sf::Sprite min_1(digits, sf::IntRect((0 * 21), 0, 21, 32));
                                                time_1 = min_1;
                                            }
                                            minutes = minutes % 10;
                                            sf::Sprite min_2(digits, sf::IntRect((minutes * 21), 0, 21, 32));
                                            time_2 = min_2;
                                            int seconds = save_time - (minutes * 60);
                                            if (seconds > 9) {
                                                int first_sec = (seconds / 10) % 10;
                                                sf::Sprite sec_1(digits, sf::IntRect((first_sec * 21), 0, 21, 32));
                                                time_3 = sec_1;
                                            }
                                            else{
                                                sf::Sprite sec_1(digits, sf::IntRect((0 * 21), 0, 21, 32));
                                                time_3 = sec_1;
                                            }
                                            seconds = seconds % 10;
                                            sf::Sprite sec_2(digits, sf::IntRect((seconds * 21), 0, 21, 32));
                                            time_4 = sec_2;

                                        }
                                        else{
                                            sf::Sprite min_1(digits, sf::IntRect((0 * 21), 0, 21, 32));
                                            time_1 = min_1;
                                            sf::Sprite min_2(digits, sf::IntRect((0 * 21), 0, 21, 32));
                                            time_2 = min_2;
                                            int seconds = save_time;
                                            if (seconds > 9) {
                                                int first_sec = (seconds / 10) % 10;
                                                sf::Sprite sec_1(digits, sf::IntRect((first_sec * 21), 0, 21, 32));
                                                time_3 = sec_1;
                                            }
                                            else{
                                                sf::Sprite sec_1(digits, sf::IntRect((0 * 21), 0, 21, 32));
                                                time_3 = sec_1;
                                            }
                                            seconds = seconds % 10;
                                            sf::Sprite sec_2(digits, sf::IntRect((seconds * 21), 0, 21, 32));
                                            time_4 = sec_2;
                                        }
                                    }


                                    float min_1_position_x = (float) columns;
                                    min_1_position_x *= 32.0;
                                    min_1_position_x -= 97.0;
                                    float min_1_y = (float) rows;
                                    min_1_y += 0.5f;
                                    min_1_y *= 32.0;
                                    min_1_y += 16.0;
                                    time_1.setPosition(min_1_position_x, min_1_y);
                                    time_2.setPosition(min_1_position_x + 21.0, min_1_y);
                                    window.draw(time_1);
                                    window.draw(time_2);
                                    float sec_1_x = min_1_position_x + 43.0;
                                    time_3.setPosition(sec_1_x, min_1_y);
                                    time_4.setPosition(sec_1_x + 21.0, min_1_y);
                                    window.draw(time_3);
                                    window.draw(time_4);



                                    window.draw(pause_button);


                                    window.draw(leaderboard_button);





                                    window.display();
                                }
                            }
                        }

                    }
                }
                if(event.type == sf::Event::TextEntered){
                    if((event.text.unicode > 64) && (event.text.unicode < 123)){
                        if(player_string.getSize() < 10){
                            final_string = "";
                            player_string += event.text.unicode;
                            for(int i = 0; i < player_string.getSize(); ++i){
                                // checks if it's a letter
                                if(std::isalpha(player_string[i])){
                                    //checks if it's the first letter
                                    if(i == 0){
                                        char letter = std::toupper(player_string[i]);
                                        final_string += letter;
                                    }
                                    else{
                                        char letter = std::tolower(player_string[i]);
                                        final_string += letter;
                                    }
                                }
                            }

                            //cout << (string) final_string << endl;
                            player_text.setString(player_string + cursor);
                            auto bounds5 = player_text.getGlobalBounds();
                            player_text.setOrigin(bounds5.width/2, bounds5.height);
                            player_text.setPosition(player_x , player_y);
                        }
                        else if(player_string.getSize() == 10){
                            final_string = "";
                            for(int i = 0; i < player_string.getSize(); ++i){
                                // checks if it's a letter
                                if(std::isalpha(player_string[i])){
                                    //checks if it's the first letter
                                    if(i == 0){
                                        char letter = std::toupper(player_string[i]);
                                        final_string += letter;
                                        //cout << (string) final_string << endl;
                                    }
                                    else{
                                        char letter = std::tolower(player_string[i]);
                                        final_string += letter;
                                    }
                                }
                            }
                            player_text.setString(player_string);
                        }
                        else{
                            final_string = final_string;
                        }
                    }
                }

                if (event.type == sf::Event::Closed) {
                    welcome_window.close();


                } else {


                    welcome_window.clear(sf::Color::Blue);
                    welcome_window.draw(text_welcome);
                    welcome_window.draw(text_input);
                    welcome_window.draw(player_text);

                    welcome_window.display();
                }
            }
        }

    }



    return 0;
}

void copy_tiles(vector<Tile> &tiles, vector<vector<Tile*>> &tiles_2d, int rows, int columns){
    for(int i =0; i < rows; ++i){
        vector<Tile*> temp;
        for(int j = 0; j < columns; ++j){
            //cout << j << endl;
            int num = (columns * i) + j;
            temp.push_back(&tiles.at(num));


        }
        //cout << temp.size() << endl;
        tiles_2d.push_back(temp);
    }
}

int check_neighbors(int i, int j, int rows, int columns, vector<vector<Tile*>> &tiles_2d){
    int count = 0;
    // check if i is 0, so top row
    if(i == 0){
        //check also if j is 0 or columns - 1, this is first and last in the row
        // look at this
        if(j == 0){

            if (tiles_2d[i][j + 1]->is_mine){
                cout << "it exists" << endl;
                count += 1;
            }
            if (tiles_2d[i + 1][j]->is_mine){
                cout << "it exists" << endl;
                count += 1;
            }
            if(tiles_2d[i + 1][j + 1]->is_mine){
                cout << "it exists" << endl;
                count += 1;
            }
        }
         else if (j == (columns - 1)){
            if (tiles_2d[i][j - 1]->is_mine){
                count += 1;
            }
            if (tiles_2d[i + 1][j]->is_mine){
                count += 1;
            }
            if(tiles_2d[i + 1][j - 1]->is_mine){
                count += 1;
            }
        }
        else{
            // to the left
             if (tiles_2d[i][j - 1]->is_mine){
                 count += 1;
             }
             // to the right
             if (tiles_2d[i][j + 1]->is_mine){
                 count += 1;
             }
             // south
             if (tiles_2d[i + 1][j]->is_mine){
                 count += 1;
             }
             // southwest
             if(tiles_2d[i + 1][j - 1]->is_mine){
                 count += 1;
             }
             // southeast
             if(tiles_2d[i + 1][j + 1]->is_mine){
                 count += 1;
             }
        }
    }
    // check if last row
    else if (i == (rows - 1)){
        if(j == 0){
            // right
            if (tiles_2d[i][j + 1]->is_mine){
                count += 1;
            }
            if (tiles_2d[i - 1][j]->is_mine){
                count += 1;
            }
            if(tiles_2d[i - 1][j + 1]->is_mine){
                count += 1;
            }
        }
        else if (j == (columns - 1)){
            if (tiles_2d[i][j - 1]->is_mine){
                count += 1;
            }
            if (tiles_2d[i - 1][j]->is_mine){
                count += 1;
            }
            if(tiles_2d[i - 1][j - 1]->is_mine){
                count += 1;
            }
        }
        else{
            // to the left
            if (tiles_2d[i][j - 1]->is_mine){
                count += 1;
            }
            // to the right
            if (tiles_2d[i][j + 1]->is_mine){
                count += 1;
            }
            // north
            if (tiles_2d[i - 1][j]->is_mine){
                count += 1;
            }
            // northwest
            if(tiles_2d[i - 1][j - 1]->is_mine){
                count += 1;
            }
            // northeast
            if(tiles_2d[i - 1][j + 1]->is_mine){
                count += 1;
            }
        }
    }
    // all the rest of the tiles
    else{
        //check if j is first in row
        if (j == 0){

            // right
            if (tiles_2d[i][j + 1]->is_mine){
                count += 1;
            }
            // north
            if (tiles_2d[i - 1][j]->is_mine){
                count += 1;
            }
            //south
            if (tiles_2d[i + 1][j]->is_mine){
                count += 1;
            }
            // northeast
            if(tiles_2d[i - 1][j + 1]->is_mine){
                count += 1;
            }
            // southeast
            if(tiles_2d[i + 1][j + 1]->is_mine){
                count += 1;
            }
        }
        else if (j == (columns - 1)){
            // left
            if (tiles_2d[i][j - 1]->is_mine){
                count += 1;
            }
            // north
            if (tiles_2d[i - 1][j]->is_mine){
                count += 1;
            }
            //south
            if (tiles_2d[i + 1][j]->is_mine){
                count += 1;
            }
            // northwest
            if(tiles_2d[i - 1][j - 1]->is_mine){
                count += 1;
            }
            // southwest
            if(tiles_2d[i + 1][j - 1]->is_mine){
                count += 1;
            }
        }
        else{
            // this is for all the points
            // left
            if (tiles_2d[i][j - 1]->is_mine){
                count += 1;
            }
            //right
            if (tiles_2d[i][j + 1]->is_mine){
                count += 1;
            }
            // north
            if (tiles_2d[i - 1][j]->is_mine){
                count += 1;
            }
            //south
            if (tiles_2d[i + 1][j]->is_mine){
                count += 1;
            }
            // northwest
            if(tiles_2d[i - 1][j - 1]->is_mine){
                count += 1;
            }
            // southwest
            if(tiles_2d[i + 1][j - 1]->is_mine){
                count += 1;
            }
            // northeast
            if(tiles_2d[i - 1][j + 1]->is_mine){
                count += 1;
            }
            // southeast
            if(tiles_2d[i + 1][j + 1]->is_mine){
                count += 1;
            }
        }
    }
    return count;

};

void checkzero(int i, int j, int rows, int columns, vector<vector<Tile*>> &tiles_2d){
    cout << "i " << i << " j " << j << endl;
    //int count = 0;
    // check if i is 0, so top row
    if(i == 0){
        //check also if j is 0 or columns - 1, this is first and last in the row
        // look at this
        if(j == 0){
            if (tiles_2d[i][j + 1]->is_blank&&tiles_2d[i][j + 1]->is_visited==false){
                tiles_2d[i][j + 1]->is_visited = true;
                cout << "bro" << endl;
                checkzero(i,j+1,rows,columns,tiles_2d);
            }
            if (tiles_2d[i + 1][j]->is_blank&&tiles_2d[i + 1][j]->is_visited==false){
                tiles_2d[i + 1][j]->is_visited = true;
                checkzero(i + 1,j,rows,columns,tiles_2d);
            }
            if(tiles_2d[i + 1][j + 1]->is_blank&&tiles_2d[i+1][j + 1]->is_visited==false){
                tiles_2d[i+1][j + 1]->is_visited = true;
                checkzero(i+1,j+1,rows,columns,tiles_2d);
            }
        }
        else if (j == (columns - 1)){
            if (tiles_2d[i][j - 1]->is_blank&&tiles_2d[i][j - 1]->is_visited==false){
                tiles_2d[i][j - 1]->is_visited = true;
                checkzero(i,j-1,rows,columns,tiles_2d);
            }
            if (tiles_2d[i + 1][j]->is_blank&&tiles_2d[i + 1][j]->is_visited==false){
                tiles_2d[i + 1][j]->is_visited = true;
                checkzero(i+1,j,rows,columns,tiles_2d);
            }
            if(tiles_2d[i + 1][j - 1]->is_blank&&tiles_2d[i + 1][j - 1]->is_visited==false){
                tiles_2d[i + 1][j - 1]->is_visited = true;
                checkzero(i+1,j-1,rows,columns,tiles_2d);
            }
        }
        else{
            // to the left
            if (tiles_2d[i][j - 1]->is_blank&&tiles_2d[i][j - 1]->is_visited==false){
                tiles_2d[i][j - 1]->is_visited = true;
                checkzero(i,j-1,rows,columns,tiles_2d);
            }
            // to the right
            if (tiles_2d[i][j + 1]->is_blank&&tiles_2d[i][j + 1]->is_visited==false){
                tiles_2d[i][j + 1]->is_visited = true;
                checkzero(i,j+1,rows,columns,tiles_2d);
            }
            // south
            if (tiles_2d[i + 1][j]->is_blank&&tiles_2d[i + 1][j]->is_visited==false){
                tiles_2d[i + 1][j]->is_visited = true;
                checkzero(i+1,j,rows,columns,tiles_2d);
            }
            // southwest
            if(tiles_2d[i + 1][j - 1]->is_blank&&tiles_2d[i + 1][j - 1]->is_visited==false){
                tiles_2d[i + 1][j - 1]->is_visited = true;
                checkzero(i+1,j-1,rows,columns,tiles_2d);
            }
            // southeast
            if(tiles_2d[i + 1][j + 1]->is_blank&&tiles_2d[i + 1][j + 1]->is_visited==false){
                tiles_2d[i + 1][j + 1]->is_visited = true;
                checkzero(i + 1,j+1,rows,columns,tiles_2d);
            }
        }
    }
        // check if last row
    else if (i == (rows - 1)){
        if(j == 0){
            // right
            if (tiles_2d[i][j + 1]->is_blank&&tiles_2d[i][j + 1]->is_visited== false){
                tiles_2d[i][j + 1]->is_visited = true;
                checkzero(i,j+1,rows,columns,tiles_2d);
            }
            if (tiles_2d[i - 1][j]->is_blank&&tiles_2d[i - 1][j]->is_visited==false){
                tiles_2d[i - 1][j]->is_visited = true;
                checkzero(i,j+1,rows,columns,tiles_2d);
            }
            if(tiles_2d[i - 1][j + 1]->is_blank&&tiles_2d[i - 1][j + 1]->is_visited==false){
                tiles_2d[i - 1][j + 1]->is_visited = true;
                checkzero(i-1,j+1,rows,columns,tiles_2d);
            }
        }
        else if (j == (columns - 1)){
            if (tiles_2d[i][j - 1]->is_blank&&tiles_2d[i][j - 1]->is_visited==false){
                tiles_2d[i][j - 1]->is_visited = true;
                checkzero(i,j-1,rows,columns,tiles_2d);
            }
            if (tiles_2d[i - 1][j]->is_blank&&tiles_2d[i - 1][j]->is_visited==false){
                tiles_2d[i - 1][j]->is_visited = true;
                checkzero(i-1,j,rows,columns,tiles_2d);
            }
            if(tiles_2d[i - 1][j - 1]->is_blank&&tiles_2d[i - 1][j - 1]->is_visited== false){
                tiles_2d[i - 1][j - 1]->is_visited = true;
                checkzero(i-1,j-1,rows,columns,tiles_2d);
            }
        }
        else{
            // to the left
            if (tiles_2d[i][j - 1]->is_blank&&tiles_2d[i][j - 1]->is_visited==false){
                tiles_2d[i][j - 1]->is_visited = true;
                checkzero(i,j-1,rows,columns,tiles_2d);
            }
            // to the right
            if (tiles_2d[i][j + 1]->is_blank&&tiles_2d[i][j + 1]->is_visited== false){
                tiles_2d[i][j + 1]->is_visited = true;
                checkzero(i,j+1,rows,columns,tiles_2d);
            }
            // north
            if (tiles_2d[i - 1][j]->is_blank&&tiles_2d[i - 1][j]->is_visited==false){
                tiles_2d[i - 1][j]->is_visited = true;
                checkzero(i-1,j,rows,columns,tiles_2d);
            }
            // northwest
            if(tiles_2d[i - 1][j - 1]->is_blank&&tiles_2d[i - 1][j - 1]->is_visited ==false){
                tiles_2d[i - 1][j - 1]->is_visited = true;
                checkzero(i-1,j-1,rows,columns,tiles_2d);
            }
            // northeast
            if(tiles_2d[i - 1][j + 1]->is_blank&&tiles_2d[i - 1][j + 1]->is_visited==false){
                tiles_2d[i - 1][j + 1]->is_visited = true;
                checkzero(i-1,j+1,rows,columns,tiles_2d);
            }
        }
    }
        // all the rest of the tiles
    else{
        //check if j is first in row
        if (j == 0){

            // right
            if (tiles_2d[i][j + 1]->is_blank&&tiles_2d[i][j + 1]->is_visited==false){
                tiles_2d[i][j + 1]->is_visited = true;
                checkzero(i,j+1,rows,columns,tiles_2d);
            }
            // north
            if (tiles_2d[i - 1][j]->is_blank&&tiles_2d[i - 1][j]->is_visited==false){
                tiles_2d[i - 1][j]->is_visited = true;
                checkzero(i-1,j,rows,columns,tiles_2d);
            }
            //south
            if (tiles_2d[i + 1][j]->is_blank&&tiles_2d[i + 1][j]->is_visited==false){
                tiles_2d[i + 1][j]->is_visited = true;
                checkzero(i+1,j,rows,columns,tiles_2d);
            }
            // northeast
            if(tiles_2d[i - 1][j + 1]->is_blank&&tiles_2d[i - 1][j + 1]->is_visited==false){
                tiles_2d[i - 1][j + 1]->is_visited = true;
                checkzero(i-1,j+1,rows,columns,tiles_2d);
            }
            // southeast
            if(tiles_2d[i + 1][j + 1]->is_blank&&tiles_2d[i + 1][j + 1]->is_visited==false){
                tiles_2d[i + 1][j + 1]->is_visited = true;
                checkzero(i+1,j+1,rows,columns,tiles_2d);
            }
        }
        else if (j == (columns - 1)){
            // left
            if (tiles_2d[i][j - 1]->is_blank&&tiles_2d[i][j - 1]->is_visited==false){
                tiles_2d[i][j - 1]->is_visited = true;
                checkzero(i,j-1,rows,columns,tiles_2d);
            }
            // north
            if (tiles_2d[i - 1][j]->is_blank&&tiles_2d[i - 1][j]->is_visited==false){
                tiles_2d[i - 1][j]->is_visited = true;
                checkzero(i-1,j,rows,columns,tiles_2d);
            }
            //south
            if (tiles_2d[i + 1][j]->is_blank&&tiles_2d[i + 1][j]->is_visited==false){
                tiles_2d[i + 1][j]->is_visited = true;
                checkzero(i+1,j,rows,columns,tiles_2d);
            }
            // northwest
            if(tiles_2d[i - 1][j - 1]->is_blank&&tiles_2d[i - 1][j - 1]->is_visited== false){
                tiles_2d[i - 1][j - 1]->is_visited = true;
                checkzero(i-1,j-1,rows,columns,tiles_2d);
            }
            // southwest
            if(tiles_2d[i + 1][j - 1]->is_blank&&tiles_2d[i + 1][j - 1]->is_visited== false){
                tiles_2d[i + 1][j - 1]->is_visited = true;
                checkzero(i+1,j-1,rows,columns,tiles_2d);
            }
        }
        else{
            // this is for all the points
            // left
            if (tiles_2d[i][j - 1]->is_blank&&tiles_2d[i][j - 1]->is_visited== false){
                tiles_2d[i][j - 1]->is_visited = true;
                checkzero(i,j-1,rows,columns,tiles_2d);
            }
            //right
            if (tiles_2d[i][j + 1]->is_blank&&tiles_2d[i][j + 1]->is_visited== false){
                tiles_2d[i][j + 1]->is_visited = true;
                checkzero(i,j+1,rows,columns,tiles_2d);
            }
            // north
            if (tiles_2d[i - 1][j]->is_blank&&tiles_2d[i - 1][j]->is_visited== false){
                tiles_2d[i - 1][j]->is_visited = true;
                checkzero(i-1,j,rows,columns,tiles_2d);
            }
            //south
            if (tiles_2d[i + 1][j]->is_blank&&tiles_2d[i + 1][j]->is_visited==false){
                tiles_2d[i + 1][j]->is_visited = true;
                checkzero(i+1,j,rows,columns,tiles_2d);
            }
            // northwest
            if(tiles_2d[i - 1][j - 1]->is_blank&&tiles_2d[i - 1][j - 1]->is_visited== false){
                tiles_2d[i - 1][j - 1]->is_visited = true;
                checkzero(i-1,j-1,rows,columns,tiles_2d);
            }
            // southwest
            if(tiles_2d[i + 1][j - 1]->is_blank&&tiles_2d[i + 1][j - 1]->is_visited==false){
                tiles_2d[i + 1][j - 1]->is_visited = true;
                checkzero(i+1,j-1,rows,columns,tiles_2d);
            }
            // northeast
            if(tiles_2d[i - 1][j + 1]->is_blank&&tiles_2d[i - 1][j + 1]->is_visited== false){
                tiles_2d[i - 1][j + 1]->is_visited = true;
                checkzero(i - 1,j+1,rows,columns,tiles_2d);
            }
            // southeast
            if(tiles_2d[i + 1][j + 1]->is_blank&&tiles_2d[i + 1][j + 1]->is_visited==false){
                tiles_2d[i + 1][j + 1]->is_visited = true;
                checkzero(i+1,j+1,rows,columns,tiles_2d);
            }
        }
    }

}

void checknum(int i, int j, int rows, int columns, vector<vector<Tile*>> &tiles_2d, sf::RenderWindow &window){
    cout << "i " << i << " j " << j << endl;
    //int count = 0;
    // check if i is 0, so top row
    if(i == 0){
        //check also if j is 0 or columns - 1, this is first and last in the row
        // look at this
        if(j == 0){
            if (!tiles_2d[i][j + 1]->is_mine&&tiles_2d[i][j + 1]->is_flagged ==false){
                tiles_2d[i][j + 1]->is_revealed = true;
                tiles_2d[i][j + 1]->tile_texture = tiles_2d[i][j + 1]->hidden_texture;
                tiles_2d[i][j + 1]->display(window);
            }
            if (!tiles_2d[i + 1][j]->is_mine&&tiles_2d[i + 1][j]->is_flagged==false){
                tiles_2d[i + 1][j]->is_revealed = true;
                tiles_2d[i + 1][j]->tile_texture = tiles_2d[i + 1][j]->hidden_texture;
                tiles_2d[i + 1][j]->display(window);
            }
            if(!tiles_2d[i + 1][j + 1]->is_mine&&tiles_2d[i+1][j + 1]->is_flagged==false){
                tiles_2d[i + 1][j + 1]->is_revealed = true;
                tiles_2d[i + 1][j + 1]->tile_texture = tiles_2d[i + 1][j + 1]->hidden_texture;
                tiles_2d[i + 1][j + 1]->display(window);
            }
        }
        else if (j == (columns - 1)){
            if (!tiles_2d[i][j - 1]->is_mine&&tiles_2d[i][j - 1]->is_flagged==false){
                tiles_2d[i][j - 1]->is_revealed = true;
                tiles_2d[i][j - 1]->tile_texture = tiles_2d[i][j - 1]->hidden_texture;
                tiles_2d[i][j - 1]->display(window);
            }
            if (!tiles_2d[i + 1][j]->is_mine&&tiles_2d[i + 1][j]->is_flagged==false){
                tiles_2d[i + 1][j]->is_revealed = true;
                tiles_2d[i + 1][j]->tile_texture = tiles_2d[i + 1][j]->hidden_texture;
                tiles_2d[i + 1][j]->display(window);
            }
            if(!tiles_2d[i + 1][j - 1]->is_mine&&tiles_2d[i + 1][j - 1]->is_flagged==false){
                tiles_2d[i + 1][j - 1]->is_revealed = true;
                tiles_2d[i + 1][j - 1]->tile_texture = tiles_2d[i + 1][j - 1]->hidden_texture;
                tiles_2d[i + 1][j - 1]->display(window);
            }
        }
        else{
            // to the left
            if (!tiles_2d[i][j - 1]->is_mine&&tiles_2d[i][j - 1]->is_flagged==false){
                tiles_2d[i][j - 1]->is_revealed = true;
                tiles_2d[i][j - 1]->tile_texture = tiles_2d[i][j - 1]->hidden_texture;
                tiles_2d[i][j - 1]->display(window);
            }
            // to the right
            if (!tiles_2d[i][j + 1]->is_mine&&tiles_2d[i][j + 1]->is_flagged==false){
                tiles_2d[i][j + 1]->is_revealed = true;
                tiles_2d[i][j - 1]->tile_texture = tiles_2d[i][j + 1]->hidden_texture;
                tiles_2d[i][j - 1]->display(window);
            }
            // south
            if (!tiles_2d[i + 1][j]->is_mine&&tiles_2d[i + 1][j]->is_flagged==false){
                tiles_2d[i + 1][j]->is_revealed = true;
                tiles_2d[i + 1][j]->tile_texture = tiles_2d[i + 1][j]->hidden_texture;
                tiles_2d[i + 1][j]->display(window);
            }
            // southwest
            if(!tiles_2d[i + 1][j - 1]->is_mine&&tiles_2d[i + 1][j - 1]->is_flagged==false){
                tiles_2d[i + 1][j - 1]->is_revealed = true;
                tiles_2d[i + 1][j - 1]->tile_texture = tiles_2d[i + 1][j - 1]->hidden_texture;
                tiles_2d[i + 1][j - 1]->display(window);
            }
            // southeast
            if(!tiles_2d[i + 1][j + 1]->is_mine&&tiles_2d[i + 1][j + 1]->is_flagged==false){
                tiles_2d[i + 1][j + 1]->is_revealed = true;
                tiles_2d[i + 1][j + 1]->tile_texture = tiles_2d[i + 1][j + 1]->hidden_texture;
                tiles_2d[i + 1][j + 1]->display(window);
            }
        }
    }
        // check if last row
    else if (i == (rows - 1)){
        if(j == 0){
            // right
            if (!tiles_2d[i][j + 1]->is_mine&&tiles_2d[i][j + 1]->is_flagged== false){
                tiles_2d[i][j + 1]->is_revealed = true;
                tiles_2d[i][j + 1]->tile_texture = tiles_2d[i][j + 1]->hidden_texture;
                tiles_2d[i][j + 1]->display(window);
            }
            if (!tiles_2d[i - 1][j]->is_mine&&tiles_2d[i - 1][j]->is_flagged==false){
                tiles_2d[i - 1][j]->is_revealed = true;
                tiles_2d[i - 1][j]->tile_texture = tiles_2d[i - 1][j]->hidden_texture;
                tiles_2d[i - 1][j]->display(window);
            }
            if(!tiles_2d[i - 1][j + 1]->is_mine&&tiles_2d[i - 1][j + 1]->is_flagged==false){
                tiles_2d[i - 1][j + 1]->is_revealed = true;
                tiles_2d[i - 1][j + 1]->tile_texture = tiles_2d[i - 1][j + 1]->hidden_texture;
                tiles_2d[i - 1][j + 1]->display(window);
            }
        }
        else if (j == (columns - 1)){
            if (!tiles_2d[i][j - 1]->is_mine&&tiles_2d[i][j - 1]->is_flagged==false){
                tiles_2d[i][j - 1]->is_revealed = true;
                tiles_2d[i][j - 1]->tile_texture = tiles_2d[i][j - 1]->hidden_texture;
                tiles_2d[i][j - 1]->display(window);
            }
            if (!tiles_2d[i - 1][j]->is_mine&&tiles_2d[i - 1][j]->is_flagged==false){
                tiles_2d[i - 1][j]->is_revealed = true;
                tiles_2d[i - 1][j]->tile_texture = tiles_2d[i - 1][j]->hidden_texture;
                tiles_2d[i - 1][j]->display(window);
            }
            if(!tiles_2d[i - 1][j - 1]->is_mine&&tiles_2d[i - 1][j - 1]->is_flagged== false){
                tiles_2d[i - 1][j - 1]->is_revealed = true;
                tiles_2d[i - 1][j - 1]->tile_texture = tiles_2d[i - 1][j - 1]->hidden_texture;
                tiles_2d[i - 1][j - 1]->display(window);
            }
        }
        else{
            // to the left
            if (!tiles_2d[i][j - 1]->is_mine&&tiles_2d[i][j - 1]->is_flagged==false){
                tiles_2d[i][j - 1]->is_revealed = true;
                tiles_2d[i][j - 1]->tile_texture = tiles_2d[i][j - 1]->hidden_texture;
                tiles_2d[i][j - 1]->display(window);
            }
            // to the right
            if (!tiles_2d[i][j + 1]->is_mine&&tiles_2d[i][j + 1]->is_flagged== false){
                tiles_2d[i][j + 1]->is_revealed = true;
                tiles_2d[i][j + 1]->tile_texture = tiles_2d[i][j + 1]->hidden_texture;
                tiles_2d[i][j + 1]->display(window);
            }
            // north
            if (!tiles_2d[i - 1][j]->is_mine&&tiles_2d[i - 1][j]->is_flagged==false){
                tiles_2d[i - 1][j]->is_revealed = true;
                tiles_2d[i - 1][j]->tile_texture = tiles_2d[i - 1][j]->hidden_texture;
                tiles_2d[i - 1][j]->display(window);
            }
            // northwest
            if(!tiles_2d[i - 1][j - 1]->is_mine&&tiles_2d[i - 1][j]->is_flagged==false){
                tiles_2d[i - 1][j - 1]->is_revealed = true;
                tiles_2d[i - 1][j - 1]->tile_texture = tiles_2d[i - 1][j - 1]->hidden_texture;
                tiles_2d[i - 1][j - 1]->display(window);
            }
            // northeast
            if(!tiles_2d[i - 1][j + 1]->is_mine&&tiles_2d[i - 1][j + 1]->is_flagged==false){
                tiles_2d[i - 1][j + 1]->is_revealed = true;
                tiles_2d[i - 1][j + 1]->tile_texture = tiles_2d[i - 1][j + 1]->hidden_texture;
                tiles_2d[i - 1][j + 1]->display(window);
            }
        }
    }
        // all the rest of the tiles
    else{
        //check if j is first in row
        if (j == 0){

            // right
            if (!tiles_2d[i][j + 1]->is_mine&&tiles_2d[i][j + 1]->is_flagged==false){
                tiles_2d[i][j + 1]->is_revealed = true;
                tiles_2d[i][j + 1]->tile_texture = tiles_2d[i][j + 1]->hidden_texture;
                tiles_2d[i][j + 1]->display(window);
            }
            // north
            if (!tiles_2d[i - 1][j]->is_mine&&tiles_2d[i - 1][j]->is_flagged==false){
                tiles_2d[i - 1][j]->is_revealed = true;
                tiles_2d[i - 1][j]->tile_texture = tiles_2d[i - 1][j]->hidden_texture;
                tiles_2d[i - 1][j]->display(window);
            }
            //south
            if (!tiles_2d[i + 1][j]->is_mine&&tiles_2d[i + 1][j]->is_flagged==false){
                tiles_2d[i + 1][j]->is_revealed = true;
                tiles_2d[i + 1][j]->tile_texture = tiles_2d[i + 1][j]->hidden_texture;
                tiles_2d[i + 1][j]->display(window);
            }
            // northeast
            if(!tiles_2d[i - 1][j + 1]->is_mine&&tiles_2d[i - 1][j + 1]->is_flagged==false){
                tiles_2d[i - 1][j + 1]->is_revealed = true;
                tiles_2d[i - 1][j + 1]->tile_texture = tiles_2d[i - 1][j + 1]->hidden_texture;
                tiles_2d[i - 1][j + 1]->display(window);
            }
            // southeast
            if(!tiles_2d[i + 1][j + 1]->is_mine&&tiles_2d[i + 1][j + 1]->is_flagged==false){
                tiles_2d[i + 1][j + 1]->is_revealed = true;
                tiles_2d[i + 1][j + 1]->tile_texture = tiles_2d[i + 1][j + 1]->hidden_texture;
                tiles_2d[i + 1][j + 1]->display(window);
            }
        }
        else if (j == (columns - 1)){
            // left
            if (!tiles_2d[i][j - 1]->is_mine&&tiles_2d[i][j - 1]->is_flagged==false){
                tiles_2d[i][j - 1]->is_revealed = true;
                tiles_2d[i][j - 1]->tile_texture = tiles_2d[i][j - 1]->hidden_texture;
                tiles_2d[i][j - 1]->display(window);
            }
            // north
            if (!tiles_2d[i - 1][j]->is_mine&&tiles_2d[i - 1][j]->is_flagged==false){
                tiles_2d[i - 1][j]->is_revealed = true;
                tiles_2d[i - 1][j]->tile_texture = tiles_2d[i - 1][j]->hidden_texture;
                tiles_2d[i - 1][j]->display(window);
            }
            //south
            if (!tiles_2d[i + 1][j]->is_mine&&tiles_2d[i + 1][j]->is_flagged==false){
                tiles_2d[i + 1][j]->is_revealed = true;
                tiles_2d[i + 1][j]->tile_texture = tiles_2d[i - 1][j]->hidden_texture;
                tiles_2d[i + 1][j]->display(window);
            }
            // northwest
            if(!tiles_2d[i - 1][j - 1]->is_mine&&tiles_2d[i - 1][j - 1]->is_flagged== false){
                tiles_2d[i - 1][j - 1]->is_revealed = true;
                tiles_2d[i - 1][j - 1]->tile_texture = tiles_2d[i - 1][j - 1]->hidden_texture;
                tiles_2d[i - 1][j - 1]->display(window);
            }
            // southwest
            if(!tiles_2d[i + 1][j - 1]->is_blank&&tiles_2d[i + 1][j - 1]->is_flagged== false){
                tiles_2d[i + 1][j - 1]->is_revealed = true;
                tiles_2d[i + 1][j - 1]->tile_texture = tiles_2d[i + 1][j - 1]->hidden_texture;
                tiles_2d[i + 1][j - 1]->display(window);
            }
        }
        else{
            // this is for all the points
            // left
            if (!tiles_2d[i][j - 1]->is_mine&&tiles_2d[i][j - 1]->is_flagged== false){
                tiles_2d[i][j - 1]->is_revealed = true;
                tiles_2d[i][j - 1]->tile_texture = tiles_2d[i][j - 1]->hidden_texture;
                tiles_2d[i][j - 1]->display(window);
            }
            //right
            if (!tiles_2d[i][j + 1]->is_mine&&tiles_2d[i][j + 1]->is_flagged== false){
                tiles_2d[i][j + 1]->is_revealed = true;
                tiles_2d[i][j + 1]->tile_texture = tiles_2d[i][j + 1]->hidden_texture;
                tiles_2d[i][j + 1]->display(window);
            }
            // north
            if (!tiles_2d[i - 1][j]->is_mine&&tiles_2d[i - 1][j]->is_flagged== false){
                tiles_2d[i - 1][j]->is_revealed = true;
                tiles_2d[i - 1][j]->tile_texture = tiles_2d[i - 1][j]->hidden_texture;
                tiles_2d[i - 1][j]->display(window);
            }
            //south
            if (!tiles_2d[i + 1][j]->is_mine&&tiles_2d[i + 1][j]->is_flagged==false){
                tiles_2d[i + 1][j]->is_revealed = true;
                tiles_2d[i + 1][j]->tile_texture = tiles_2d[i + 1][j]->hidden_texture;
                tiles_2d[i + 1][j]->display(window);
            }
            // northwest
            if(!tiles_2d[i - 1][j - 1]->is_mine&&tiles_2d[i - 1][j - 1]->is_flagged== false){
                tiles_2d[i - 1][j - 1]->is_revealed = true;
                tiles_2d[i - 1][j - 1]->tile_texture = tiles_2d[i - 1][j - 1]->hidden_texture;
                tiles_2d[i - 1][j - 1]->display(window);
            }
            // southwest
            if(!tiles_2d[i + 1][j - 1]->is_mine&&tiles_2d[i + 1][j - 1]->is_flagged==false){
                tiles_2d[i + 1][j - 1]->is_revealed = true;
                tiles_2d[i + 1][j - 1]->tile_texture = tiles_2d[i + 1][j - 1]->hidden_texture;
                tiles_2d[i + 1][j - 1]->display(window);
            }
            // northeast
            if(!tiles_2d[i - 1][j + 1]->is_mine&&tiles_2d[i - 1][j + 1]->is_flagged== false){
                tiles_2d[i - 1][j + 1]->is_revealed = true;
                tiles_2d[i - 1][j + 1]->tile_texture = tiles_2d[i - 1][j + 1]->hidden_texture;
                tiles_2d[i - 1][j + 1]->display(window);
            }
            // southeast
            if(!tiles_2d[i + 1][j + 1]->is_mine&&tiles_2d[i + 1][j + 1]->is_flagged==false){
                tiles_2d[i + 1][j + 1]->is_revealed = true;
                tiles_2d[i + 1][j + 1]->tile_texture = tiles_2d[i + 1][j + 1]->hidden_texture;
                tiles_2d[i + 1][j + 1]->display(window);
            }
        }
    }

}

bool checkwin(int goal, int mines, vector<vector<Tile*>> &tiles_2d, sf::RenderWindow &window){
    int revealed = 0;
    int flagged = 0;
    for(int i = 0; i < tiles_2d.size(); ++i){
        for(int j = 0; j < tiles_2d[i].size(); ++j){
            if(tiles_2d[i][j]->is_revealed == true){
                revealed += 1;
            }
            else if(tiles_2d[i][j]->is_mine == true){
                //tiles_2d[i][j]->is_flagged = true;

                if(tiles_2d[i][j]->is_flagged == true){
                    flagged += 1;
                }
            }
        }
    }
    if(revealed == goal){
        return true;
    }
    return  false;
}


