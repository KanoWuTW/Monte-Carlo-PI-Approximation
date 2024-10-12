#include <SFML/Graphics.hpp>
#include<iostream>
#include<random>
#include<cmath>

using namespace std;

void generate_pixels(vector<sf::Sprite*>& vec, sf::Texture& pixel_texture, mt19937& mt, uniform_real_distribution<double>& fdist) //生成隨機像素
{
    vec.push_back(new sf::Sprite);
    vec.back()->setTexture(pixel_texture);
    
    //設定隨機位置
    sf::Vector2f pos(fdist(mt), fdist(mt));
    vec.back()->setPosition(pos);
}

void drawPixels(sf::RenderWindow& window, vector<sf::Sprite*>& vec)//繪製隨機像素
{
    int size = vec.size();
    for ( int i = 0; i < size; i++)
    {
        window.draw(*vec[i]);
    }
}

void count(const sf::CircleShape& shape, const vector<sf::Sprite*>& pixel_container, double& Pi_Appox, int& inside_Circle, int& total) //計算Pi
{
    total++;
    sf::Vector2f center(499.5, 499.5);
    sf::Vector2f pixel_pos(pixel_container.back()->getPosition());

    //用畢氏定理計算隨機點是否在圓內
    float distance = sqrt((center.x - pixel_pos.x) * (center.x - pixel_pos.x) + (center.y - pixel_pos.y) * (center.y - pixel_pos.y));

    if (distance <= 500.f)
    {
        inside_Circle++;
    }
    //計算PI = 4*(圈內點數/總點數)
    Pi_Appox = 4 * ((float)inside_Circle / (float)total);
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Monte Carlo Pi Approximation");
    
    //半徑500的圓
    sf::CircleShape shape(500.f);
    shape.setFillColor(sf::Color::Blue);

    sf::RectangleShape rectShape(sf::Vector2f(1000.f, 1000.f));
    sf::Texture pixel_texture;
    sf::Texture bgTexture;
    bgTexture.loadFromFile("bg.jpg");
    pixel_texture.loadFromFile("Pixel.jpg");
    rectShape.setTexture(&bgTexture);
    
    //文字
    sf::Text result;
    sf::Font font;
    font.loadFromFile("times.ttf");
    result.setFont(font);
    result.setPosition(1200.f, 400.f);
    result.setCharacterSize(50);
    result.setFillColor(sf::Color::White);

    sf::Clock clock;
    sf::Time timePassed = sf::Time::Zero;
    
    //生成隨機浮點數字(0~999)
    mt19937 mt;
    uniform_real_distribution<double> fdist(0.f, 999.f);
    
    vector<sf::Sprite*> pixel_container;
    
    double Pi_Appox = 0.f;
    int inside_Circle = 0;
    int total = 0;
   
   
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        timePassed = clock.getElapsedTime();
        //每0.5秒生成一個隨機像素
        while (timePassed >= sf::microseconds(500) && total < 400000)
        {
            timePassed = timePassed - sf::microseconds(500);
            generate_pixels(pixel_container, pixel_texture,mt,fdist);
            count(shape, pixel_container, Pi_Appox, inside_Circle, total);
        }

        string str = to_string(Pi_Appox) + " = 4*(" + to_string(inside_Circle) + "/" + to_string(total)+")";
        result.setString(str);
        window.clear(sf::Color::Black);
        window.draw(rectShape);
        window.draw(shape);
        window.draw(result);
        drawPixels(window, pixel_container);
        window.display();
    }

    for (int i = 0; i < pixel_container.size(); i++)
    {
        delete pixel_container[i];
    }

    return 0;
}