#include <SFML/Graphics.hpp>
#include <iostream>

class Button
{
public:
    // Default constructor
    Button() {}

    // Constructor with initialization of pressed and notpressed sprites
    Button(sf::Sprite notpr, sf::Sprite pr)
    {
        pressed = pr;
        notpressed = notpr;
        active = false;
    }

    // Set position of the button
    void setPosition(float x, float y)
    {
        // Set position of both pressed and notpressed sprites
        sf::Vector2f pos(x, y);
        sf::Vector2f pos2(x - 6, y - 6); // Adjusted position for pressed sprite
        pressed.setPosition(pos);
        notpressed.setPosition(pos2);
    }

    // Draw the pressed sprite to the window
    void drawPressedTo(sf::RenderWindow& window)
    {
        window.draw(pressed);
    }

    // Draw the notpressed sprite to the window
    void drawNotPressedTo(sf::RenderWindow& window)
    {
        window.draw(notpressed);
    }

    // Check if the button is pressed
    bool ispressed()
    {
        return active;
    }

    // Set the button as pressed
    void press()
    {
        active = true;
    }

    // Set the button as released
    void release()
    {
        active = false;
    }

    // Check if the mouse is over the button
    bool isMouseOver(sf::RenderWindow& window)
    {
        float mouseX = sf::Mouse::getPosition(window).x;
        float mouseY = sf::Mouse::getPosition(window).y;

        float btnPosX = notpressed.getPosition().x;
        float btnPosY = notpressed.getPosition().y;

        float btnxPoswidth = notpressed.getPosition().x + notpressed.getLocalBounds().width;
        float btnyPosheight = notpressed.getPosition().y + notpressed.getLocalBounds().height;

        // Check if the mouse coordinates are within the button's boundaries
        if (mouseX < btnxPoswidth && mouseX > btnPosX && mouseY < btnyPosheight && mouseY > btnPosY)
        {
            return true; // Mouse is over the button
        }
        return false; // Mouse is not over the button
    }

private:
    sf::Sprite pressed;   // Sprite for the pressed state of the button
    sf::Sprite notpressed; // Sprite for the not pressed state of the button
    bool active;           // Flag to indicate if the button is currently pressed
};
