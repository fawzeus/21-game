#include <SFML/Graphics.hpp>
#include <iostream>

class Jeton
{
public:
    // Default constructor
    Jeton() {}

    // Constructor with initialization of picture and value
    Jeton(sf::Sprite sp, int val)
    {
        picture = sp;
        value = val;
    }

    // Set position of the Jeton
    void setPosition(float x, float y)
    {
        sf::Vector2f pos(x, y);
        picture.setPosition(pos);
    }

    // Get the value of the Jeton
    int getvalue()
    {
        return value;
    }

    // Draw the Jeton to the window
    void drawTo(sf::RenderWindow& window)
    {
        window.draw(picture);
    }

    // Check if the Jeton is pressed
    bool ispressed()
    {
        return active;
    }

    // Set the Jeton as pressed
    void press()
    {
        active = true;
    }

    // Set the Jeton as released
    void release()
    {
        active = false;
    }

    // Check if the mouse is over the Jeton
    bool isMouseOver(sf::RenderWindow& window)
    {
        float mouseX = sf::Mouse::getPosition(window).x;
        float mouseY = sf::Mouse::getPosition(window).y;

        float btnPosX = picture.getPosition().x;
        float btnPosY = picture.getPosition().y;

        float btnxPoswidth = picture.getPosition().x + picture.getLocalBounds().width;
        float btnyPosheight = picture.getPosition().y + picture.getLocalBounds().height;

        // Check if the mouse coordinates are within the Jeton's boundaries
        if (mouseX < btnxPoswidth && mouseX > btnPosX && mouseY < btnyPosheight && mouseY > btnPosY)
        {
            return true; // Mouse is over the Jeton
        }
        return false; // Mouse is not over the Jeton
    }

private:
    sf::Sprite picture; // Sprite for the Jeton
    int value;          // Value associated with the Jeton
    bool active;        // Flag to indicate if the Jeton is currently pressed
};
